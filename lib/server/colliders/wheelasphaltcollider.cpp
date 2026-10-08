#define NOMINMAX
#include "wheelasphaltcollider.h"

#include "config.h"
#include "world.h"
#include "core/clazz.h"
#include "core/kernel.h"

#include <algorithm>
#include <stdexcept>

#include "retruxx/common.h"
#include "engine/landscape/roads/road.h"
#include "geomobject.h"
#include "server/server.h"
#include "server/objects/vehicle.h"
#include "server/objects/wheel.h"
#include "server/objects/physicbodies/geoms/geom.h"

#include <ode/objects.h>
#include "ode/odecpp.h"

namespace ai
{
    namespace
    {
        // The wheel's spin speed at the rim: its angular velocity (taken into the wheel's frame,
        // which does not change its length) times its radius.
        float GetRimSpeed(Wheel* objWheel)
        {
            dReal const* const w = dBodyGetAngularVel(objWheel->GetBody()->id());
            CVector const angularVel(w[0], w[1], w[2]);
            Quaternion const inv = objWheel->GetRotation().getInversed();
            float const xz = inv.z * inv.x;
            float const xw = inv.x * inv.w;
            float const zw = inv.z * inv.w;
            float const xx = inv.x * inv.x;
            float const xy = inv.y * inv.x;
            float const yz = inv.z * inv.y;
            float const zz = inv.z * inv.z;
            float const yw = inv.y * inv.w;
            float const yy = inv.y * inv.y;
            CMatrix m;
            m._11 = 1.0f - (zz + yy) * 2.0f;
            m._12 = (zw + xy) * 2.0f;
            m._13 = (xz - yw) * 2.0f;
            m._14 = 0.0f;
            m._21 = (xy - zw) * 2.0f;
            m._22 = 1.0f - (zz + xx) * 2.0f;
            m._23 = (xw + yz) * 2.0f;
            m._24 = 0.0f;
            m._31 = (yw + xz) * 2.0f;
            m._32 = (yz - xw) * 2.0f;
            m._33 = 1.0f - (yy + xx) * 2.0f;
            m._34 = 0.0f;
            m._41 = 0.0f;
            m._42 = 0.0f;
            m._43 = 0.0f;
            m._44 = 1.0f;
            CMatrix const rot(m);
            float const lx = rot._11 * angularVel.x + rot._31 * angularVel.z + rot._21 * angularVel.y;
            float const ly = rot._12 * angularVel.x + rot._32 * angularVel.z + rot._22 * angularVel.y;
            float const lz = rot._13 * angularVel.x + rot._33 * angularVel.z + rot._23 * angularVel.y;
            return static_cast<float>(
                objWheel->GetRadius() * sqrt(double(lz) * lz + double(ly) * ly + double(lx) * lx));
        }

        float GetSpeed(CVector const& vel)
        {
            return static_cast<float>(sqrt(double(vel.x) * vel.x + double(vel.z) * vel.z + double(vel.y) * vel.y));
        }

        // Where the wheel meets the ground it leaves a trace, which a skid starts. The trace is
        // lifted off the ground along the contact normal by a tenth of the radius, at most 0.1.
        void TraceSkid(Wheel* objWheel, dContact const* contacts, int soilType)
        {
            m3d::WheelTraceMgr& traceMgr = pServer->GetWorld()->GetWheelTracesMgr();
            if (!traceMgr.IsSkiddingStarted(objWheel))
            {
                traceMgr.StartSkidding(objWheel, soilType);
                return;
            }
            CVector const normal(contacts->geom.normal[0], contacts->geom.normal[1], contacts->geom.normal[2]);
            CVector const pos(contacts->geom.pos[0], contacts->geom.pos[1], contacts->geom.pos[2]);
            float const width = objWheel->GetWidth();
            float const lift = std::min(objWheel->GetRadius() * 0.1f, 0.1f);
            CVector const tracePos(normal.x * lift + pos.x, normal.y * lift + pos.y, normal.z * lift + pos.z);
            traceMgr.AddTrace(tracePos, objWheel->GetRotation(), width, objWheel, soilType, true);
        }

        // Keeps the wheel's splash effect for the surface of splashType at the contact, replacing
        // one left over from another surface; and, if the wheel hits the ground at more than
        // 2 m/s, the wheel-hit effect.
        void MakeSplash(
            Wheel* objWheel,
            dContact const* contacts,
            short splashType,
            CStr const& effectName,
            CVector const& vel)
        {
            if (objWheel->m_SplashType != splashType && objWheel->m_SplashEffect)
            {
                // Everything under the old effect is let go before the effect itself is removed.
                std::vector<m3d::Object*> stack;
                stack.push_back(objWheel->m_SplashEffect);
                while (!stack.empty())
                {
                    m3d::Object* current = stack.back();
                    stack.pop_back();
                    for (m3d::Object* child = current->GetFirstChild(); child; child = child->GetNextSibling())
                    {
                        static_cast<m3d::SgNode*>(child)->CanBeFree();
                        if (child->GetFirstChild())
                        {
                            stack.push_back(child);
                        }
                    }
                }
                objWheel->m_SplashEffect->GetGraph()->InsertInRemoveIfFree(objWheel->m_SplashEffect);
                objWheel->m_SplashEffect = nullptr;
            }
            CVector const pos(contacts->geom.pos[0], contacts->geom.pos[1], contacts->geom.pos[2]);
            if (!objWheel->m_SplashEffect)
            {
                objWheel->m_SplashEffect =
                    PhysicBody::CreateEffectNode(effectName, pos, IdentityQuaternion, false, 1.0f);
            }
            objWheel->m_SplashEffect->SetOriginAbs(pos);
            objWheel->m_SplashType = splashType;
            objWheel->m_MakeSplash = true;

            float const nx = contacts->geom.normal[0];
            float const ny = contacts->geom.normal[1];
            float const nz = contacts->geom.normal[2];
            float const inv =
                static_cast<float>(1.0 / sqrt(double(nz) * nz + double(ny) * ny + double(nx) * nx + 0.00000011920929));
            if (fabs((inv * nx) * vel.x + vel.z * (nz * inv) + vel.y * (ny * inv)) > 2.0 &&
                objWheel->CanCreateCollisionEffect())
            {
                objWheel->SetCollisionEffectCreated();
                PhysicBody::CreateEffectNode(CStr("ET_PS_WHEEL_HIT"), pos, IdentityQuaternion, true, 1.0f);
            }
        }
    }  // namespace

    int CollideWheelDefault(m3d::Object* obj1, m3d::Object*, dContact* contacts, unsigned& numContacts, bool reverse)
    {
        // RVA 0x891430 - every wheel contact: optionally straightens up normals against static
        // geometry, then sets the tyre's grip, with the main friction direction along the tread
        // (across the axle).
        Wheel* objWheel = static_cast<Wheel*>(obj1);
        if (M3D_ENGINE_CFG.m_ai_tweak_wheel_normals.GetB() && numContacts)
        {
            dGeomID const other = reverse ? contacts[0].geom.g2 : contacts[0].geom.g1;
            if (!dGeomGetBody(other))
            {
                // A nearly vertical normal pointing the wrong way for this pair is turned round.
                int const wanted = 2 * !reverse - 1;
                for (unsigned i = 0; i < numContacts; ++i)
                {
                    dReal* const n = contacts[i].geom.normal;
                    int const sign = n[1] >= 0.0f ? 1 : -1;
                    if (sign != wanted && fabs(n[0]) < 0.1 && fabs(n[2]) < 0.1)
                    {
                        n[0] = 0.0f - n[0];
                        n[1] = 0.0f - n[1];
                        n[2] = 0.0f - n[2];
                    }
                }
            }
        }

        for (unsigned i = 0; i < numContacts; ++i)
        {
            dContact& contact = contacts[i];
            contact.surface.mode |= dContactMu2 | dContactFDir1;
            float const nx = contact.geom.normal[0];
            float const ny = contact.geom.normal[1];
            float const nz = contact.geom.normal[2];
            CVector const axis = objWheel->GetDirection();
            float const fz = axis.y * nx - ny * axis.x;
            float const fy = nz * axis.x - axis.z * nx;
            float const fx = axis.z * ny - axis.y * nz;
            float const inv =
                static_cast<float>(1.0 / sqrt(double(fz) * fz + double(fy) * fy + double(fx) * fx + 0.00000011920929));
            contact.fdir1[3] = 0.0f;
            contact.fdir1[0] = inv * fx;
            contact.fdir1[2] = inv * fz;
            contact.fdir1[1] = inv * fy;
            contact.surface.mu = 1.0f;
            contact.surface.mu2 = 1.5f;
            contact.surface.soft_erp = 0.80000001f;
            contact.surface.soft_cfm = static_cast<float>(0.000099999997 / objWheel->GetMass());
        }

        if (Vehicle* vehicle = objWheel->GetVehicle())
        {
            vehicle->IncNumWheelsTouchingGround();
        }
        return 1;
    }

    int CollideWheelAndAsphalt(
        m3d::Object* obj1,
        m3d::Object* asphalt,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x891680 - a wheel on a road: when the tyre spins faster or slower than the wheel
        // moves by more than skidDeltaSpeed it skids, leaving a trace and the road splash.
        Wheel* objWheel = static_cast<Wheel*>(obj1);
        CollideWheelDefault(objWheel, asphalt, contacts, numContacts, reverse);
        m3d::RoadNode* roadNode = static_cast<m3d::GeomObjectRoad*>(asphalt)->GetRoadNode();

        float const rimSpeed = GetRimSpeed(objWheel);
        CVector const vel = objWheel->GetLinearVelocity();
        float const speed = GetSpeed(vel);
        Vehicle* vehicle = objWheel->GetVehicle();
        if (!vehicle)
        {
            return 1;
        }

        if (fabs(rimSpeed - speed) > M3D_ENGINE_CFG.m_skidDeltaSpeed.GetF())
        {
            TraceSkid(objWheel, contacts, roadNode->GetSoilType());
            // Roads use splash type 1000.
            CStr const& effectName = gDynamicScene->GetRoadEffectName(objWheel->m_wheelType, vehicle->bIsBraking());
            MakeSplash(objWheel, contacts, 1000, effectName, vel);
        }

        if (vehicle->GetOnOilMode())
        {
            for (unsigned i = 0; i < numContacts; ++i)
            {
                contacts[i].surface.mu = 0.0f;
                contacts[i].surface.mu2 = 0.0f;
            }
        }
        return 1;
    }

    int CollideWheelAndLandscape(
        m3d::Object* obj1,
        m3d::Object* objLsCollision,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x891DB0 - a wheel on open ground: as on a road, but the trace and splash come from
        // the soil under the contact, whose friction (boosted by the turbo) sets the grip and
        // whose resistance drags on the wheel.
        Wheel* objWheel = static_cast<Wheel*>(obj1);
        CollideWheelDefault(objWheel, objLsCollision, contacts, numContacts, reverse);

        float const rimSpeed = GetRimSpeed(objWheel);
        CVector const vel = objWheel->GetLinearVelocity();
        float const speed = GetSpeed(vel);
        m3d::CWorld* world = pServer->GetWorld();
        float const cellSize = pServer->GetLevelSize() / static_cast<float>(world->GetLandscape().GetTileSize());
        DynamicScene::SoilProps const& soil = gDynamicScene->GetSoilProps(
            static_cast<int>(contacts->geom.pos[0] * (1.0f / cellSize) + 0.5f),
            static_cast<int>(contacts->geom.pos[2] * (1.0f / cellSize) + 0.5f));

        Vehicle* vehicle = objWheel->GetVehicle();
        bool onOil = false;
        if (vehicle)
        {
            if (fabs(rimSpeed - speed) > M3D_ENGINE_CFG.m_skidDeltaSpeed.GetF())
            {
                TraceSkid(objWheel, contacts, soil.m_idx);
                CStr const effectName(
                    gDynamicScene->GetSoilEffectName(objWheel->m_wheelType, soil.m_splashType, vehicle->bIsBraking()));
                MakeSplash(objWheel, contacts, soil.m_splashType, effectName, vel);
            }
            onOil = vehicle->GetOnOilMode();
        }

        // The turbo multiplies the grip, squared once it is past 1.
        float turbo = 1.0f;
        if (vehicle && vehicle->GetTurboThrottleTime() > 0.0000099999997)
        {
            if (vehicle->GetTurboThrottleValue() <= 1.0f)
            {
                turbo = vehicle->GetTurboThrottleValue();
            }
            else
            {
                turbo = vehicle->GetTurboThrottleValue() * vehicle->GetTurboThrottleValue();
            }
        }
        for (unsigned i = 0; i < numContacts; ++i)
        {
            if (onOil)
            {
                contacts[i].surface.mu = 0.0f;
                contacts[i].surface.mu2 = 0.0f;
            }
            else
            {
                contacts[i].surface.mu = soil.m_friction * turbo;
                contacts[i].surface.mu2 = soil.m_friction * turbo * 1.5f;
            }
        }

        // Rolling resistance: against the wheel's motion, scaled by its mass over its radius.
        CVector const v = objWheel->GetLinearVelocity();
        CVector const drag(
            (0.0f - v.x) * soil.m_resistance, (0.0f - v.y) * soil.m_resistance, (0.0f - v.z) * soil.m_resistance);
        float const mass = objWheel->GetMass();
        float const invRadius = 1.0f / objWheel->GetRadius();
        objWheel->AddForce(
            CVector(invRadius * (drag.x * mass), (drag.y * mass) * invRadius, (drag.z * mass) * invRadius));
        return 1;
    }

    int CollideWheelAndWater(
        m3d::Object* obj1,
        m3d::Object* objWater,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x892730 - the wheel keeps its normal ground contact; while it moves at 1 m/s or more
        // it also gets a force and a water splash that follows it.
        Wheel* objWheel = static_cast<Wheel*>(obj1);
        CollideWheelDefault(objWheel, objWater, contacts, numContacts, reverse);

        CVector const vel = objWheel->GetLinearVelocity();
        if (1.0 > std::sqrt(double(vel.z) * vel.z + double(vel.y) * vel.y + double(vel.x) * vel.x))
        {
            return 0;
        }

        // NOTE: the force is the velocity scaled by the squared wheel radius and points along the
        // motion, so the water pushes the wheel on rather than dragging it back.
        double const radiusSq = double(objWheel->GetRadius()) * objWheel->GetRadius();
        CVector const friction(
            static_cast<float>(vel.x * radiusSq),
            static_cast<float>(vel.y * radiusSq),
            static_cast<float>(vel.z * radiusSq));
        objWheel->AddForce(friction);

        // Water uses splash type 2000; a splash left over from another surface is retired first.
        if (objWheel->m_SplashType != 2000 && objWheel->m_SplashEffect)
        {
            std::vector<m3d::Object*> stack;
            stack.push_back(objWheel->m_SplashEffect);
            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();
                for (m3d::Object* child = current->GetFirstChild(); child; child = child->GetNextSibling())
                {
                    static_cast<m3d::SgNode*>(child)->CanBeFree();
                    if (child->GetFirstChild())
                    {
                        stack.push_back(child);
                    }
                }
            }
            objWheel->m_SplashEffect->GetGraph()->InsertInRemoveIfFree(objWheel->m_SplashEffect);
            objWheel->m_SplashEffect = nullptr;
        }

        CVector const contactPos(contacts->geom.pos[0], contacts->geom.pos[1], contacts->geom.pos[2]);
        if (!objWheel->m_SplashEffect)
        {
            objWheel->m_SplashEffect = PhysicBody::CreateEffectNode(
                CStr("ET_PS_WATERSPLASH"), contactPos, objWheel->GetRotation(), false, 1.0f);
        }

        objWheel->m_SplashEffect->SetOriginAbs(contactPos);
        objWheel->m_SplashEffect->SetRotation(objWheel->GetRotation());
        objWheel->m_SplashType = 2000;
        objWheel->m_MakeSplash = true;
        return 0;
    }
}  // namespace ai

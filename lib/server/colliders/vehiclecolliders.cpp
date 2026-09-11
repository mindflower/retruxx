#include "vehiclecolliders.h"

#include <stdexcept>

#include "retruxx/common.h"

#include <config.h>
#include <core/kernel.h>
#include <landscape.h>
#include <world.h>
#include <math/vector.h>
#include <ode/objects.h>
#include <ode/odecpp.h>
#include <scene/nodes/sgnode.h>

#include "defaultcollider.h"
#include "server/ai/aimanager.h"
#include "server/damageinfo.h"
#include "server/dynamicscene.h"
#include "server/server.h"
#include "server/objects/base/globalproperties.h"
#include "server/objects/base/physicobj.h"
#include "server/objects/physicbodies/physicbody.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

namespace ai
{
    namespace
    {
        // Speed thresholds that pick between the fast and the slow spark effect,
        // and below which no collision effect is produced at all.
        float const SPARKLE_SPEED = 10.0f;
        float const SPARKLE_SLOW_SPEED = 1.0f;
    }  // namespace

    float CalcHitVelocity(CVector const& deltaVel, CVector const& normal)
    {
        // RVA 0x88F6A0 - the squared closing speed along the contact normal,
        // with a dead zone so that gentle contact does no damage at all. The
        // configured minimum is in km/h, hence the conversion to m/s.
        float const velProjection =
            fabsf(deltaVel.z * normal.z + deltaVel.y * normal.y + deltaVel.x * normal.x);

        float hitVelocity = velProjection - M3D_ENGINE_CFG.m_ai_min_hit_velocity.GetF() * 0.27777779f;
        if (hitVelocity < 0.0f)
        {
            hitVelocity = 0.0f;
        }
        return hitVelocity * hitVelocity;
    }

    void CalcDamageToVehicles(
        PhysicObj* obj1,
        PhysicObj* obj2,
        dContact* contacts,
        float& dSpeed,
        DamageInfo& damageInfo,
        CVector const& bodyPos)
    {
        // RVA 0x88F700 - turns one contact into a damage record. obj2 may be
        // null, in which case the second body is treated as immovable.
        float const posX = contacts->geom.pos[0];
        float const posY = contacts->geom.pos[1];
        float const posZ = contacts->geom.pos[2];

        float velOde[4];
        dBodyGetPointVel(obj1->GetBody()->id(), posX, posY, posZ, velOde);
        CVector const vel1(velOde[0], velOde[1], velOde[2]);

        CVector vel2(0.0f, 0.0f, 0.0f);
        if (obj2)
        {
            dBodyGetPointVel(obj2->GetBody()->id(), posX, posY, posZ, velOde);
            vel2 = CVector(velOde[0], velOde[1], velOde[2]);
        }

        CVector const deltaVel(vel1.x - vel2.x, vel1.y - vel2.y, vel1.z - vel2.z);

        float const nx = contacts->geom.normal[0];
        float const ny = contacts->geom.normal[1];
        float const nz = contacts->geom.normal[2];
        float const invLen = 1.0f / sqrtf(nz * nz + ny * ny + nx * nx + 0.00000011920929f);

        CVector normal(nx * invLen, ny * invLen, nz * invLen);

        // Make the normal point away from the body we are damaging, whichever
        // way ODE happened to report it.
        float const facing = ((posZ - bodyPos.z) * normal.z + (posY - bodyPos.y) * normal.y) +
            (posX - bodyPos.x) * normal.x;
        if (facing < 0.0f)
        {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }

        dSpeed = fabsf(normal.z * deltaVel.z + normal.y * deltaVel.y + normal.x * deltaVel.x);

        float const hitVelocity = CalcHitVelocity(deltaVel, normal);

        // Short frames would otherwise spread the same impact over more
        // contacts, so the coefficient is scaled by how long this frame ran.
        float const clash = M3D_ENGINE_CFG.m_ai_clash_coeff.GetF();
        float const coeff = (M3D_ENGINE_CFG.m_ai_min_frame_time.GetF() / theAIManager->m_elapsedTime) * clash;

        float const mass1 = obj1->GetMass();
        float damage;
        if (obj2)
        {
            // Two movable bodies share the impact through their reduced mass.
            float const mass2 = obj2->GetMass();
            damage = ((mass2 * mass1) * coeff) * hitVelocity / (mass2 + mass1);
        }
        else
        {
            damage = (mass1 * coeff) * hitVelocity;
        }

        damageInfo.hitPos.x = posX;
        damageInfo.hitPos.y = posY;
        damageInfo.hitPos.z = posZ;
        damageInfo.hitDir = normal;
        damageInfo.normal = normal;
        damageInfo.damage = damage;
        damageInfo.damageType = DAMAGE_BLAST;
        damageInfo.decalId = hitVelocity <= SPARKLE_SPEED ? -1 : gDynamicScene->GetClashDecalId();
    }

    int CollideDummyAndVehiclePart(
        m3d::Object* obj1,
        m3d::Object* obj2,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x88FA90
        (void)numContacts;
        (void)reverse;

        auto* dummy = static_cast<PhysicObj*>(obj1);
        auto* part = static_cast<VehiclePart*>(obj2);

        PhysicObj* owner = part ? part->GetOwner() : nullptr;
        if (!dummy || !owner)
        {
            return 1;
        }

        if (owner->IsKindOf(&Vehicle::m_classVehicle))
        {
            DamageInfo damageInfo;

            // A dummy flagged this way damages the vehicle anonymously rather
            // than crediting the dummy as the attacker.
            CVector const partPos = part->GetPosition();
            float dSpeed = 0.0f;
            if (dummy->GetPrototypeInfo()->m_bIsUpdating)
            {
                CalcDamageToVehicles(owner, nullptr, contacts, dSpeed, damageInfo, partPos);
            }
            else
            {
                CalcDamageToVehicles(owner, dummy, contacts, dSpeed, damageInfo, partPos);
            }

            damageInfo.damagedPartName = part->GetPartName();
            owner->InflictDamage(damageInfo);
        }

        if (owner->CanCreateCollisionEffect())
        {
            // NOTE: the shipped build compares a literal 0.0 against the two
            // thresholds here instead of the impact speed, so none of these
            // branches can ever be taken and a dummy hit never sparks.
            CVector contactPos;
            contactPos.x = contacts->geom.pos[0];
            contactPos.y = contacts->geom.pos[1];
            contactPos.z = contacts->geom.pos[2];

            if (0.0f > SPARKLE_SLOW_SPEED)
            {
                owner->SetCollisionEffectCreated();
            }
            if (0.0f > SPARKLE_SPEED)
            {
                CStr const modelname = (dummy->GetPhysicState() & 1) != 0 ? "ET_PS_VEHICLESPARKLE"
                                                                         : "ET_PS_STATICSSPARKLE";
                PhysicBody::CreateEffectNode(modelname, contactPos, Quaternion(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);
                return 1;
            }
            if (0.0f > SPARKLE_SLOW_SPEED)
            {
                CStr const modelname = (dummy->GetPhysicState() & 1) != 0 ? "ET_PS_VEHICLESPARKLE_SLOW"
                                                                         : "ET_PS_STATICSSPARKLE_SLOW";
                PhysicBody::CreateEffectNode(modelname, contactPos, Quaternion(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);
                return 1;
            }
        }
        return 1;
    }

    int CollideVehiclePartAndVehiclePart(
        m3d::Object* obj1,
        m3d::Object* obj2,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x890430 - both sides can take damage, so the work is done once
        // and the normal is flipped for the second victim.
        (void)numContacts;
        (void)reverse;

        auto* vp1 = static_cast<VehiclePart*>(obj1);
        auto* vp2 = static_cast<VehiclePart*>(obj2);

        PhysicObj* owner1 = vp1->GetOwner();
        PhysicObj* owner2 = vp2->GetOwner();
        if (!owner1 || !owner2)
        {
            return 1;
        }

        bool isVehicle1 = owner1->IsKindOf(&Vehicle::m_classVehicle);
        bool isVehicle2 = owner2->IsKindOf(&Vehicle::m_classVehicle);
        if (!isVehicle1 && !isVehicle2)
        {
            return 1;
        }

        // Always compute from the vehicle's point of view; if only the second
        // side is a vehicle, swap so it becomes the first.
        VehiclePart* refPart = vp1;
        bool bSwap = false;
        if (isVehicle2 && !isVehicle1)
        {
            vp2 = vp1;
            std::swap(owner1, owner2);
            isVehicle1 = true;
            isVehicle2 = false;
            bSwap = true;
        }

        DamageInfo damageInfo;
        float deltaSpeed = 0.0f;
        CVector const refPos = refPart->GetPosition();
        CalcDamageToVehicles(owner1, isVehicle2 ? owner2 : nullptr, contacts, deltaSpeed, damageInfo, refPos);

        if (bSwap)
        {
            damageInfo.normal.x = -damageInfo.normal.x;
            damageInfo.normal.y = -damageInfo.normal.y;
            damageInfo.normal.z = -damageInfo.normal.z;
        }

        if (isVehicle1)
        {
            damageInfo.damagedPartName = refPart->GetPartName();
            if (isVehicle2)
            {
                damageInfo.attackerId = owner2->GetId();
                damageInfo.attackingAgentId = owner2->GetId();
            }
            owner1->InflictDamage(damageInfo);
        }

        if (isVehicle2)
        {
            damageInfo.damagedPartName = vp2->GetPartName();
            damageInfo.normal.x = -damageInfo.normal.x;
            damageInfo.normal.y = -damageInfo.normal.y;
            damageInfo.normal.z = -damageInfo.normal.z;
            if (isVehicle1)
            {
                damageInfo.attackerId = owner1->GetId();
                damageInfo.attackingAgentId = owner1->GetId();
            }
            owner2->InflictDamage(damageInfo);
        }

        if (owner1->CanCreateCollisionEffect() && owner2->CanCreateCollisionEffect())
        {
            if (deltaSpeed > SPARKLE_SLOW_SPEED)
            {
                // NOTE: the shipped build marks the first owner twice and never
                // the second one.
                owner1->SetCollisionEffectCreated();
                owner1->SetCollisionEffectCreated();
            }
            if (deltaSpeed > SPARKLE_SLOW_SPEED)
            {
                CStr const modelname =
                    deltaSpeed > SPARKLE_SPEED ? "ET_PS_VEHICLESPARKLE" : "ET_PS_VEHICLESPARKLE_SLOW";
                CVector pos;
                pos.x = contacts->geom.pos[0];
                pos.y = contacts->geom.pos[1];
                pos.z = contacts->geom.pos[2];
                PhysicBody::CreateEffectNode(modelname, pos, Quaternion(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);
            }
        }
        return 1;
    }

    int CollideVehicleAndLandscape(
        m3d::Object* obj1,
        m3d::Object* obj2,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x88FC20
        (void)obj2;
        (void)numContacts;
        (void)reverse;

        auto* vhcl = static_cast<VehiclePart*>(obj1);
        PhysicObj* owner = vhcl ? vhcl->GetOwner() : nullptr;
        if (!owner)
        {
            return 1;
        }

        DamageInfo damageInfo;
        float speed = 0.0f;
        CVector const bodyPos = vhcl->GetPosition();
        CalcDamageToVehicles(owner, nullptr, contacts, speed, damageInfo, bodyPos);
        damageInfo.damagedPartName = vhcl->GetPartName();
        owner->InflictDamage(damageInfo);

        if (speed > SPARKLE_SPEED && owner->CanCreateCollisionEffect())
        {
            owner->SetCollisionEffectCreated();

            // The splash depends on what the ground is made of at the contact,
            // so the world position is mapped back onto the soil map.
            auto& landscape = pServer->GetWorld()->GetLandscape();
            float const cellSize = pServer->GetLevelSize() / static_cast<float>(landscape.GetTileSize());
            auto const& soil = gDynamicScene->GetSoilProps(
                static_cast<int>((1.0f / cellSize) * contacts->geom.pos[0] + 0.5f),
                static_cast<int>(contacts->geom.pos[2] * (1.0f / cellSize) + 0.5f));

            CVector pos;
            pos.x = contacts->geom.pos[0];
            pos.y = contacts->geom.pos[1];
            pos.z = contacts->geom.pos[2];
            PhysicBody::CreateEffectNode(
                gDynamicScene->GetVehicleSoilEffectName(soil.m_splashType),
                pos,
                Quaternion(0.0f, 0.0f, 0.0f, 1.0f),
                true,
                1.0f);
        }
        return 1;
    }

    int CollideVehicleAndStatics(
        m3d::Object* obj1,
        m3d::Object* obj2,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x8900E0
        (void)obj2;
        (void)numContacts;
        (void)reverse;

        auto* vhcl = static_cast<VehiclePart*>(obj1);
        PhysicObj* owner = vhcl ? vhcl->GetOwner() : nullptr;
        if (!owner)
        {
            return 1;
        }

        DamageInfo damageInfo;
        float speed = 0.0f;
        CVector const bodyPos = vhcl->GetPosition();
        CalcDamageToVehicles(owner, nullptr, contacts, speed, damageInfo, bodyPos);
        damageInfo.damagedPartName = vhcl->GetPartName();
        owner->InflictDamage(damageInfo);

        if (owner->CanCreateCollisionEffect())
        {
            if (speed > SPARKLE_SLOW_SPEED)
            {
                owner->SetCollisionEffectCreated();
            }
            if (speed > SPARKLE_SLOW_SPEED)
            {
                CStr const modelname =
                    speed > SPARKLE_SPEED ? "ET_PS_STATICSSPARKLE" : "ET_PS_STATICSSPARKLE_SLOW";
                CVector pos;
                pos.x = contacts->geom.pos[0];
                pos.y = contacts->geom.pos[1];
                pos.z = contacts->geom.pos[2];
                PhysicBody::CreateEffectNode(modelname, pos, Quaternion(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);
            }
        }
        return 1;
    }

    int CollideVehicleAndRoad(
        m3d::Object* obj1,
        m3d::Object* obj2,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x88FE10
        (void)obj2;

        auto* vhcl = static_cast<VehiclePart*>(obj1);
        if (!vhcl)
        {
            return 1;
        }
        PhysicObj* owner = vhcl->GetOwner();
        if (!owner)
        {
            return 1;
        }

        if (M3D_ENGINE_CFG.m_ai_tweak_vehicle_normals.GetB())
        {
            // Road surfaces sometimes report their normal pointing the wrong
            // way. A near vertical normal facing against the expected side is
            // flipped rather than letting the vehicle be pushed through.
            int const wanted = 2 * !reverse - 1;
            for (unsigned i = 0; i < numContacts; ++i)
            {
                float* n = contacts[i].geom.normal;
                int const side = n[1] >= 0.0f ? 1 : -1;
                if (side != wanted && fabsf(n[0]) < 0.1f && fabsf(n[2]) < 0.1f)
                {
                    n[0] = -n[0];
                    n[1] = -n[1];
                    n[2] = -n[2];
                }
            }
        }

        DamageInfo damageInfo;
        float speed = 0.0f;
        CVector const bodyPos = vhcl->GetPosition();
        CalcDamageToVehicles(owner, nullptr, contacts, speed, damageInfo, bodyPos);
        damageInfo.damagedPartName = vhcl->GetPartName();
        owner->InflictDamage(damageInfo);

        if (owner->CanCreateCollisionEffect())
        {
            if (speed > SPARKLE_SLOW_SPEED)
            {
                owner->SetCollisionEffectCreated();
            }
            if (speed > SPARKLE_SLOW_SPEED)
            {
                CStr const modelname = speed > SPARKLE_SPEED ? "ET_PS_ROADSPARKLE" : "ET_PS_ROADSPARKLE_SLOW";
                CVector pos;
                pos.x = contacts->geom.pos[0];
                pos.y = contacts->geom.pos[1];
                pos.z = contacts->geom.pos[2];
                PhysicBody::CreateEffectNode(modelname, pos, Quaternion(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);
            }
        }
        return 1;
    }

    int CollideVehicleAndWater(
        m3d::Object* obj1,
        m3d::Object* obj2,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x88F560 - water is drag plus a splash, never damage, and the
        // contacts are discarded so the vehicle is not pushed out of it.
        (void)obj2;
        (void)numContacts;
        (void)reverse;

        auto* part = static_cast<VehiclePart*>(obj1);
        PhysicObj* owner = part ? part->GetOwner() : nullptr;
        if (!owner || !owner->IsKindOf(&Vehicle::m_classVehicle))
        {
            return 0;
        }
        auto* vehicle = static_cast<Vehicle*>(owner);

        // Drag proportional to the cross section the vehicle presents.
        CVector const vel = vehicle->GetLinearVelocity();
        float const drag = -(vehicle->GetSize().y * vehicle->GetSize().x);
        CVector friction;
        friction.x = vel.x * drag;
        friction.y = vel.y * drag;
        friction.z = vel.z * drag;
        vehicle->AddForce(friction);

        if (sqrtf(vel.y * vel.y + vel.z * vel.z + vel.x * vel.x) >= 1.0f)
        {
            CVector pos;
            pos.x = contacts->geom.pos[0];
            pos.y = contacts->geom.pos[1];
            pos.z = contacts->geom.pos[2];

            if (!part->m_SplashEffect)
            {
                part->m_SplashEffect = PhysicBody::CreateEffectNode(
                    "ET_PS_WATERSPLASH",
                    pos,
                    vehicle->GetRotation(),
                    false,
                    1.0f);
            }
            part->m_SplashEffect->SetOriginAbs(pos);
            part->m_SplashEffect->SetRotation(vehicle->GetRotation());
            part->m_MakeSplash = true;
        }
        return 0;
    }

    int CollideVehiclePartAndGeomObj(
        m3d::Object* obj1,
        m3d::Object* obj2,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x88F450 - the geometry side does the real work; afterwards the
        // surface is softened so a vehicle rides over scenery rather than
        // catching on it.
        CollideGeomObjAndLandscape(obj2, obj1, contacts, numContacts, !reverse);

        for (unsigned i = 0; i < numContacts; ++i)
        {
            contacts[i].surface.mu = 0.2f;
            contacts[i].surface.soft_erp = 0.60000002f;
            contacts[i].surface.soft_cfm = 0.00050000002f;
        }
        return 1;
    }

    int CollideVehicleAndPassCell(
        m3d::Object* obj1,
        m3d::Object* obj2,
        dContact* contacts,
        unsigned& numContacts,
        bool reverse)
    {
        // RVA 0x8902E0 - the impassable-area map only pushes the player's own
        // vehicle back, and only sideways: contacts whose normal has any
        // vertical component are dropped so it never acts as a floor.
        (void)obj2;
        (void)reverse;

        if (!obj1)
        {
            return 0;
        }
        auto* part = static_cast<VehiclePart*>(obj1);
        PhysicObj* owner = part->GetOwner();
        if (!owner || !owner->IsKindOf(&Vehicle::m_classVehicle))
        {
            return 0;
        }
        auto* vehicle = static_cast<Vehicle*>(owner);
        if (thePlayer && thePlayer->GetVehicle() != vehicle)
        {
            return 0;
        }

        for (unsigned i = 0; i < numContacts; ++i)
        {
            contacts[i].surface.mode = 12312;
            contacts[i].surface.mu = theGlobProp.m_playerPassMapUnpassableMu;
            contacts[i].surface.soft_cfm = theGlobProp.m_playerPassMapUnpassableCfm;
            contacts[i].surface.soft_erp = theGlobProp.m_playerPassMapUnpassableErp;
            contacts[i].surface.mu2 = 0.0f;
            contacts[i].surface.bounce = 10.0f;
        }

        // Compact the array in place, swapping every rejected contact with the
        // last one still in play.
        unsigned i = 0;
        while (i < numContacts)
        {
            if (fabsf(contacts[i].geom.normal[1]) <= 0.001f)
            {
                ++i;
            }
            else if (i != --numContacts)
            {
                std::swap(contacts[i], contacts[numContacts]);
            }
        }
        return numContacts != 0;
    }
}

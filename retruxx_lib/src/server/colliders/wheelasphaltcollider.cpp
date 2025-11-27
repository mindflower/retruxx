#define NOMINMAX
#include "wheelasphaltcollider.h"

#include "config.h"
#include "world.h"
#include "core/clazz.h"
#include "core/kernel.h"

#include <stdexcept>

#include "retruxx/common.h"
#include "server/server.h"
#include "server/objects/vehicle.h"
#include "server/objects/wheel.h"
#include "server/objects/physicbodies/geoms/geom.h"

#include <ode/objects.h>
#include "ode/odecpp.h"

namespace ai
{
    int CollideWheelDefault(m3d::Object* objWheel, m3d::Object* surface, dContact* contact, unsigned& numContacts, bool reverse)
    {
        // TODO: generated code
        if (M3D_ENGINE_CFG.m_ai_tweak_wheel_normals.GetB() && numContacts)
        {
            auto* geom = reverse ? contact[0].geom.g2 : contact[0].geom.g1;
            if (!dGeomGetBody(geom))
            {
                for (unsigned i = 0; i < numContacts; ++i)
                {
                    dContactGeom& contactGeom = contact[i].geom;

                    // Determine direction multiplier based on normal Y component
                    int directionMultiplier = (contactGeom.normal[1] >= 0.0f) ? 1 : -1;
                    int expectedDirection = 2 * !reverse - 1;  // 1 if not reverse, -1 if reverse

                    // Check if we need to flip the normal
                    if (directionMultiplier != expectedDirection && std::fabs(contactGeom.normal[0]) < 0.1f && std::fabs(contactGeom.normal[2]) < 0.1f)
                    {
                        // Flip the normal
                        contactGeom.normal[0] = -contactGeom.normal[0];
                        contactGeom.normal[1] = -contactGeom.normal[1];
                        contactGeom.normal[2] = -contactGeom.normal[2];
                    }
                }
            }
        }

        auto* wheel = RT_DYNCAST(objWheel, Wheel);

        // Process all contacts
        for (unsigned int i = 0; i < numContacts; ++i)
        {
            dContact& currentContact = contact[i];
            dContactGeom& geom = currentContact.geom;

            // Set surface parameters
            currentContact.surface.mode |= 3;  // Enable bounce and soft_erp
            currentContact.surface.mu = 1.0f;
            currentContact.surface.mu2 = 1.5f;
            currentContact.surface.soft_erp = 0.80000001f;
            currentContact.surface.soft_cfm = 0.000099999997f / wheel->GetMass();

            // Calculate wheel axis direction
            CVector wheelAxis = wheel->GetDirection();

            // Calculate contact tangent directions
            const float& nx = geom.normal[0];
            const float& ny = geom.normal[1];
            const float& nz = geom.normal[2];

            const float& wx = wheelAxis.x;
            const float& wy = wheelAxis.y;
            const float& wz = wheelAxis.z;

            // Calculate tangent vector (cross product of wheel axis and normal)
            float tx = (wz * ny) - (wy * nz);
            float ty = (wx * nz) - (wz * nx);
            float tz = (wy * nx) - (wx * ny);

            // Normalize tangent vector
            float tangentLength = std::sqrt(tx * tx + ty * ty + tz * tz + 1.1920929e-7f);
            float invTangentLength = 1.0f / tangentLength;

            // Set friction direction
            currentContact.fdir1[0] = tx * invTangentLength;
            currentContact.fdir1[1] = ty * invTangentLength;
            currentContact.fdir1[2] = tz * invTangentLength;
            currentContact.fdir1[3] = 0.0f;
        }

        // Update vehicle wheel contact count
        ai::Vehicle* vehicle = wheel->GetVehicle();
        if (vehicle)
        {
            vehicle->IncNumWheelsTouchingGround();
        }

        return 1;
	}

	int CollideWheelAndAsphalt(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideWheelAndLandscape(m3d::Object* obj1, m3d::Object* objLsCollision, dContact* contacts, unsigned& numContacts, bool reverse)
	{
		// TODO: generated code CollideWheelAndLandscape
        // Call default wheel collision first

        auto* objWheel = RT_DYNCAST(obj1, Wheel);
        CollideWheelDefault(objWheel, objLsCollision, contacts, numContacts, reverse);

        // Get wheel angular velocity and convert to local space
        auto* bodyAngular = dBodyGetAngularVel(objWheel->GetBody()->id());
        CVector angularVel;
        angularVel.x = bodyAngular[0];
        angularVel.y = bodyAngular[1];
        angularVel.z = bodyAngular[2];

        Quaternion wheelRot = objWheel->GetRotation();
        Quaternion invRot = wheelRot.getInversed();

        // Create rotation matrix from inverse quaternion
        CMatrix rotMatrix = invRot.ToMatrix();

        // Transform angular velocity to local space
        CVector localAngularVel;
        localAngularVel.x = rotMatrix._11 * angularVel.x + rotMatrix._31 * angularVel.z + rotMatrix._21 * angularVel.y;
        localAngularVel.y = rotMatrix._12 * angularVel.x + rotMatrix._32 * angularVel.z + rotMatrix._22 * angularVel.y;
        localAngularVel.z = rotMatrix._13 * angularVel.x + rotMatrix._33 * angularVel.z + rotMatrix._23 * angularVel.y;

        // Calculate wheel properties

        float wheelRadius = objWheel->GetRadius();
        float angularSpeed = localAngularVel.length();
        float linearWheelSpeedFromRotation = wheelRadius * angularSpeed;

        // Get actual linear velocity
        CVector wheelLinearVel = objWheel->GetLinearVelocity();
        float actualLinearSpeed = wheelLinearVel.length();

        // Get soil properties at contact point
        m3d::CWorld* world = ai::pServer->GetWorld();
        float levelSize = pServer->GetLevelSize();
        int tileSize = world->GetLandscape().GetTileSize();
        float scaleFactor = levelSize / tileSize;

        int tileX = (int)(contacts->geom.pos[0] / scaleFactor + 0.5f);
        int tileZ = (int)(contacts->geom.pos[2] / scaleFactor + 0.5f);

        DynamicScene::SoilProps const& soilProps = ai::gDynamicScene->GetSoilProps(tileX, tileZ);

        Vehicle* vehicle = objWheel->GetVehicle();
        bool onOilMode = false;

        if (vehicle)
        {
            // Check for skidding
            float skidDeltaSpeed = M3D_ENGINE_CFG.m_skidDeltaSpeed.GetF();

            if (fabs(linearWheelSpeedFromRotation - actualLinearSpeed) > skidDeltaSpeed)
            {
                m3d::WheelTraceMgr& traceMgr = world->GetWheelTracesMgr();

                if (traceMgr.IsSkiddingStarted(objWheel))
                {
                    // Add skid trace
                    CVector contactPos(contacts->geom.pos);
                    CVector contactNormal(contacts->geom.normal);
                    float wheelWidth = objWheel->GetWidth();
                    float traceOffset = std::min(wheelRadius * 0.1f, 0.1f);

                    // Calculate trace position slightly above contact point
                    CVector tracePos = contactPos + contactNormal * traceOffset;
                    Quaternion traceRot = objWheel->GetRotation();

                    traceMgr.AddTrace(tracePos, traceRot, wheelWidth, objWheel, soilProps.m_idx, 1);
                }
                else
                {
                    traceMgr.StartSkidding(objWheel, soilProps.m_idx);
                }

                // Handle splash effects
                if (objWheel->m_SplashType != soilProps.m_splashType && objWheel->m_SplashEffect)
                {
                    // Recursively remove all children from scene graph
                    std::vector<m3d::Object*> removalStack;
                    removalStack.push_back(reinterpret_cast<m3d::Object*>(objWheel->m_SplashEffect));

                    while (!removalStack.empty())
                    {
                        m3d::Object* current = removalStack.back();
                        removalStack.pop_back();

                        // Add children to stack for processing
                        m3d::Object* child = current->GetFirstChild();
                        while (child)
                        {
                            removalStack.push_back(child);
                            child = child->GetNextSibling();
                        }

                        // Remove from scene graph
                        auto* currentNode = (m3d::SgNode*)current;
                        m3d::SceneGraph* graph = currentNode->GetGraph();
                        if (graph)
                        {
                            graph->InsertInRemoveIfFree(currentNode);
                        }
                    }

                    objWheel->m_SplashEffect = nullptr;
                }

                if (!objWheel->m_SplashEffect)
                {
                    bool isBraking = vehicle->bIsBraking();
                    CStr const effectName = ai::gDynamicScene->GetSoilEffectName(objWheel->m_wheelType, soilProps.m_splashType, isBraking);

                    CVector effectPos(contacts->geom.pos);
                    objWheel->m_SplashEffect = PhysicBody::CreateEffectNode(effectName, effectPos, IdentityQuaternion, false, 1.0f);
                }

                // Update splash effect position
                CVector contactPos(contacts->geom.pos);
                objWheel->m_SplashEffect->SetOriginAbs(contactPos);
                objWheel->m_SplashType = soilProps.m_splashType;
                objWheel->m_MakeSplash = true;

                // Check for collision effects
                CVector normal(contacts->geom.normal[0], contacts->geom.normal[1], contacts->geom.normal[2]);
                float normalLength = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z + 1.1920929e-7f);
                float invNormalLength = 1.0f / normalLength;
                normal.x *= invNormalLength;
                normal.y *= invNormalLength;
                normal.z *= invNormalLength;

                float impactSpeed = normal.x * wheelLinearVel.x + normal.y * wheelLinearVel.y + normal.z * wheelLinearVel.z;
                if (fabs(impactSpeed) > 2.0f && objWheel->CanCreateCollisionEffect())
                {
                    objWheel->SetCollisionEffectCreated();
                    PhysicBody::CreateEffectNode("ET_PS_WHEEL_HIT", contactPos, IdentityQuaternion, true, 1.0f);
                }
            }

            onOilMode = vehicle->GetOnOilMode();
        }

        // Apply friction and resistance based on soil properties and vehicle state
        float turboMultiplier = 1.0f;
        if (vehicle && vehicle->GetTurboThrottleTime() > 1e-8f)
        {
            float turboValue = vehicle->GetTurboThrottleValue();
            turboMultiplier = (turboValue <= 1.0f) ? turboValue : turboValue * turboValue;
        }

        // Update contact friction
        for (unsigned int i = 0; i < numContacts; ++i)
        {
            dContact& contact = contacts[i];

            if (onOilMode)
            {
                contact.surface.mu = 0.0f;
                contact.surface.mu2 = 0.0f;
            }
            else
            {
                contact.surface.mu = soilProps.m_friction * turboMultiplier;
                contact.surface.mu2 = contact.surface.mu * 1.5f;
            }
        }

        // Apply soil resistance as force
        CVector vel = objWheel->GetLinearVelocity();

        CVector resistanceForce = -vel * soilProps.m_resistance;
        float wheelMass = objWheel->GetMass();
        CVector force = resistanceForce * wheelMass;

        // Convert force to torque (simplified)
        float invRadius = 1.0f / wheelRadius;
        CVector torque = force * invRadius;

        objWheel->AddForce(torque);

        return 1;
	}

	int CollideWheelAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}

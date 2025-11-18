#include "wheelasphaltcollider.h"

#include "config.h"
#include "core/clazz.h"
#include "core/kernel.h"

#include <stdexcept>

#include "retruxx/common.h"
#include "server/objects/vehicle.h"
#include "server/objects/wheel.h"
#include "server/objects/physicbodies/geoms/geom.h"

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

	int CollideWheelAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideWheelAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}

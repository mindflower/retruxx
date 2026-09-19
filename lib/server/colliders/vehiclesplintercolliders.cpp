#include "vehiclesplintercolliders.h"

#include "server/objects/base/physicobj.h"
#include "server/objects/physicbodies/physicbody.h"

#include <core/clazz.h>
#include <math/quaternion.h>
#include <math/vector.h>

#include <stdexcept>

#include "retruxx/common.h"

namespace ai
{
	int CollideParticleSplinter(m3d::Object*, m3d::Object*, dContact* contacts, unsigned& numContacts, bool)
	{
		// RVA 0x890D40 - debris lands soft rather than skidding or bouncing off.
		for (unsigned i = 0; i < numContacts; ++i)
		{
			// dContactApprox1 | dContactSoftERP | dContactSoftCFM
			contacts[i].surface.mode = 12312;
			contacts[i].surface.mu = 1.0f;
			contacts[i].surface.soft_erp = 0.1f;
			contacts[i].surface.soft_cfm = 0.001f;
			// NOTE: dContactBounce is not among the mode bits above, so ODE never reads these
			// two - they are written and ignored.
			contacts[i].surface.bounce = 0.0f;
			contacts[i].surface.bounce_vel = 10.0f;
		}
		return 1;
	}

	int CollidePOAndWater(m3d::Object* obj1, m3d::Object*, dContact* contacts, unsigned&, bool)
	{
		// RVA 0x890DD0 - nothing floats: the object passes straight through the water, and all
		// that happens is a splash where it went in, and only if it was moving.
		if (IS_KIND_OF(obj1, PhysicObj))
		{
			auto const* physicObj = static_cast<PhysicObj const*>(obj1);
			if (physicObj->GetLinearVelocity().length() >= 1.0f)
			{
				CVector const pos(contacts->geom.pos[0], contacts->geom.pos[1], contacts->geom.pos[2]);
				PhysicBody::CreateEffectNode(CStr("ET_PS_SPLINTER_WATERSPLASH"), pos,
					IdentityQuaternion, true, 1.0f);
			}
		}
		// Returning 0 discards the contacts, so no contact joint is ever made with water.
		return 0;
	}

	int CollideVehicleSplinterAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		// RVA 0x890D30 - vehicle debris hits the ground on ODE's default surface, so unlike
		// CollideParticleSplinter there is nothing to adjust.
		return 1;
	}

	int CollideParticleSplinterAndWater(m3d::Object* obj1, m3d::Object*, dContact*, unsigned&, bool)
	{
		// RVA 0x890DC0 - a particle splinter that reaches water is simply gone.
		// NOTE: the object is downcast and removed without any type check, so this collider is
		// only safe for the ai::Obj subclasses it is registered against.
		static_cast<Obj*>(obj1)->Remove();
		return 0;
	}
}

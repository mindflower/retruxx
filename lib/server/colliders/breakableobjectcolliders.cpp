#include "breakableobjectcolliders.h"
#include <stdexcept>

#include "retruxx/common.h"

#include <config.h>
#include <core/kernel.h>
#include <ode/objects.h>
#include <ode/odecpp.h>

#include "vehiclecolliders.h"
#include "server/ai/aimanager.h"
#include "server/damageinfo.h"
#include "server/dynamicscene.h"
#include "server/objects/blastwave.h"
#include "server/objects/breakableobject.h"
#include "server/objects/vehicle.h"
#include "server/objects/wheel.h"
#include "server/objects/guns/bullet.h"
#include "server/objects/guns/gun.h"
#include "server/objects/guns/mortarshell.h"
#include "server/objects/guns/rocket.h"
#include "server/objects/physicbodies/physicbody.h"
#include "server/objects/physicbodies/vehiclepart.h"

namespace ai
{
	int CollideVehicleAndBreakableObject(
		m3d::Object* obj1,
		m3d::Object* obj2,
		dContact* contact,
		unsigned& numContacts,
		bool reverse)
	{
		// RVA 0x892A00 - obj1 is a vehicle part, a wheel or a shell. Vehicles are damaged by the
		// impact and knock the object over or break it; shells destroy it outright.
		CVector const contactPos(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2]);

		// NOTE: a Rocket is also a Shell, so it creates its own blast wave here and then the gun's
		// blast wave below as well.
		if (obj1->IsKindOf(RT_CLASS_LOCAL(Rocket)))
		{
			static_cast<Rocket*>(obj1)->CreateBlastWave();
		}
		if (obj1->IsKindOf(RT_CLASS_LOCAL(MortarShell)))
		{
			static_cast<MortarShell*>(obj1)->CreateBlastWave();
		}
		else if (obj1->IsKindOf(RT_CLASS_LOCAL(Shell)))
		{
			Shell* shell = static_cast<Shell*>(obj1);
			shell->GetGunPrototypeInfo()->CreateBlastWave(contactPos, shell->GetGunObjId());
		}
		if (obj1->IsKindOf(RT_CLASS_LOCAL(Bullet)))
		{
			static_cast<Bullet*>(obj1)->SpecifyTracer(contactPos);
		}

		BreakableObject* breakable = static_cast<BreakableObject*>(obj2);
		// NOTE: the shipped code performs this check twice in a row.
		if (!breakable->IsAlive())
		{
			return 0;
		}

		BreakableObject::STATES state = breakable->GetState();
		CStr effectName;
		PhysicObj* physicObj1 = nullptr;
		int result = 0;

		if (obj1->IsKindOf(RT_CLASS_LOCAL(VehiclePart)) || obj1->IsKindOf(RT_CLASS_LOCAL(Wheel)))
		{
			Vehicle* owner;
			if (obj1->IsKindOf(RT_CLASS_LOCAL(VehiclePart)))
			{
				owner = static_cast<Vehicle*>(static_cast<PhysicBody*>(obj1)->GetOwner());
				physicObj1 = owner;
			}
			else
			{
				owner = static_cast<Wheel*>(obj1)->GetVehicle();
				physicObj1 = static_cast<PhysicObj*>(obj1);
			}
			if (!owner)
			{
				return 1;
			}

			float velOde[4];
			dBodyGetPointVel(owner->GetBody()->id(), contactPos.x, contactPos.y, contactPos.z, velOde);
			CVector const vel(velOde[0], velOde[1], velOde[2]);
			CVector const normal =
				CVector(contact->geom.normal[0], contact->geom.normal[1], contact->geom.normal[2]).getNormalized();

			// The closing speed along the contact normal, and the kinetic energy (without the 1/2)
			// the vehicle brings into the object.
			float const hitVelocity = static_cast<float>(
				fabs(double(normal.y) * velOde[1] + double(normal.z) * velOde[2] + double(normal.x) * velOde[0]));
			float const damageVelocity = CalcHitVelocity(vel, normal);
			float const ownerMass = owner->GetMass();
			float const objectMass = breakable->GetMass();
			float const cineticEnergy = static_cast<float>(double(ownerMass) * hitVelocity * hitVelocity);

			if (obj1->IsKindOf(RT_CLASS_LOCAL(VehiclePart)))
			{
				float const clash = M3D_ENGINE_CFG.m_ai_clash_coeff.GetF();
				float const coeff = (M3D_ENGINE_CFG.m_ai_min_frame_time.GetF() / theAIManager->m_elapsedTime) * clash;

				// NOTE: the hit position and normal are taken from the middle contact, while the
				// velocity above was sampled at the first one. The normal is used as ODE reports
				// it, without normalizing.
				dContact const& mid = contact[numContacts >> 1];
				CVector const hitPos(mid.geom.pos[0], mid.geom.pos[1], mid.geom.pos[2]);
				CVector hitNormal(mid.geom.normal[0], mid.geom.normal[1], mid.geom.normal[2]);

				// Make the normal point away from the part being damaged.
				CVector const partPos = static_cast<PhysicBody*>(obj1)->GetPosition();
				float const facing = ((hitPos.z - partPos.z) * hitNormal.z + (hitPos.y - partPos.y) * hitNormal.y) +
					(hitPos.x - partPos.x) * hitNormal.x;
				if (facing < 0.0f)
				{
					hitNormal.x = 0.0f - hitNormal.x;
					hitNormal.y = 0.0f - hitNormal.y;
					hitNormal.z = 0.0f - hitNormal.z;
				}

				// An object that is loose or about to break shares the impact through the reduced
				// mass; one that stays put is treated as immovable.
				float damage;
				if (cineticEnergy > breakable->GetCriticalHitEnergy() || state != BreakableObject::DISABLED)
				{
					damage = coeff * objectMass * ownerMass * damageVelocity / (objectMass + ownerMass);
					if (breakable->IsDestroyable())
					{
						damage = damage * 0.5f;
					}
				}
				else
				{
					damage = coeff * ownerMass * damageVelocity;
				}

				DamageInfo damageInfo;
				damageInfo.damage = damage;
				damageInfo.damageType = DAMAGE_BLAST;
				damageInfo.damagedPartName = static_cast<VehiclePart*>(obj1)->GetPartName();
				damageInfo.hitPos = hitPos;
				damageInfo.hitDir = hitNormal;
				damageInfo.normal = hitNormal;
				damageInfo.decalId = hitVelocity <= 10.0f ? -1 : gDynamicScene->GetClashDecalId();
				owner->InflictDamage(damageInfo);
			}

			result = 1;
			if (cineticEnergy <= breakable->GetCriticalHitEnergy())
			{
				// Too weak to move it: just a knock effect.
				if (hitVelocity > 1.0f)
				{
					effectName = gDynamicScene->GetBoVehicleEffectName(breakable->GetEffectType());
				}
			}
			else if (breakable->IsDestroyable() && breakable->IsExplosive())
			{
				result = 0;
				state = BreakableObject::REMOVED;
				BlastWave* blastWave = breakable->CreateBlastWave();
				if (blastWave)
				{
					breakable->SetInitVelocities(true);
					breakable->SetCausePos(blastWave->GetPosition());
					breakable->SetCauseForce(blastWave->GetDampedForceIntensity(0.0f));
				}
			}
			else
			{
				// Knocked loose: from now on the object is simulated. A destroyable one lets the
				// vehicle pass through.
				state = BreakableObject::ENABLED;
				if (breakable->IsDestroyable())
				{
					result = 0;
				}
			}
		}
		else if (obj1->IsKindOf(RT_CLASS_LOCAL(Shell)))
		{
			// Only shells fired by an object that is being simulated destroy the breakable.
			Shell* shell = static_cast<Shell*>(obj1);
			Obj* emitter = shell->GetEmittedObj();
			if (emitter && emitter->IsKindOf(RT_CLASS_LOCAL(PhysicObj)) &&
				static_cast<PhysicObj*>(emitter)->bIsUpdatingByODE())
			{
				breakable->CreateBlastWave();
				effectName = gDynamicScene->GetBoShellEffectName(
					breakable->GetEffectType(), shell->GetGunPrototypeInfo()->m_explosionType);
			}
			result = 0;
			shell->Remove();
		}

		breakable->SetState(state);

		if (effectName.c_str() && strlen(effectName.c_str()) && breakable->CanCreateCollisionEffect() &&
			(!physicObj1 || physicObj1->CanCreateCollisionEffect()))
		{
			breakable->SetCollisionEffectCreated();
			if (physicObj1)
			{
				physicObj1->SetCollisionEffectCreated();
			}
			CVector const pos(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2]);
			PhysicBody::CreateEffectNode(effectName, pos, breakable->GetRotation(), true, 1.0f);
		}
		return result;
	}
}

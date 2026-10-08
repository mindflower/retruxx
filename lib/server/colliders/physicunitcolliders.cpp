#include "physicunitcolliders.h"

#include <stdexcept>

#include "retruxx/common.h"

#include <config.h>
#include <core/kernel.h>
#include <ode/collision.h>
#include <ode/objects.h>
#include <scene/nodes/sgnode.h>
#include <skelmodel.h>

#include "server/damageinfo.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/shell.h"
#include "server/objects/blastwave.h"
#include "server/objects/guns/bullet.h"
#include "server/objects/guns/gun.h"
#include "server/objects/physicbodies/physicbody.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/objects/physicunit.h"
#include "server/objects/vehicle.h"
#include "server/objects/wheel.h"

namespace ai
{
	float CalcContactDeltaSpeed(dContact const& contact)
	{
		// RVA 0x88D610 - closing speed of the two bodies along the contact normal, never negative.
		CVector const pos(contact.geom.pos[0], contact.geom.pos[1], contact.geom.pos[2]);

		auto const bodyOf = [](dGeomID geom) -> dBodyID {
			if (!geom)
			{
				return nullptr;
			}
			// A geom inside a geom transform has no body of its own.
			dGeomID const transform = dGeomGetParentTransform(geom);
			return dGeomGetBody(transform ? transform : geom);
		};

		dBodyID body1 = bodyOf(contact.geom.g1);
		dBodyID body2 = bodyOf(contact.geom.g2);
		if (!body1 && !body2)
		{
			return 0.0f;
		}

		bool reverse = false;
		if (!body1)
		{
			body1 = body2;
			body2 = nullptr;
			reverse = true;
		}

		dVector3 vel;
		dBodyGetPointVel(body1, pos.x, pos.y, pos.z, vel);
		CVector const vel1(vel[0], vel[1], vel[2]);
		CVector vel2 = ZeroVector;
		if (body2)
		{
			dBodyGetPointVel(body2, pos.x, pos.y, pos.z, vel);
			vel2 = CVector(vel[0], vel[1], vel[2]);
		}

		float const nx = contact.geom.normal[0];
		float const ny = contact.geom.normal[1];
		float const nz = contact.geom.normal[2];
		float const invLen = static_cast<float>(1.0 / sqrt(nz * nz + ny * ny + nx * nx + 0.00000011920929f));
		CVector normal(invLen * nx, ny * invLen, nz * invLen);
		if (reverse)
		{
			normal = CVector(0.0f - normal.x, 0.0f - normal.y, 0.0f - normal.z);
		}

		float const deltaSpeed = normal.z * (vel2.z - vel1.z) + normal.y * (vel2.y - vel1.y) + normal.x * (vel2.x - vel1.x);
		return deltaSpeed >= 0.0f ? deltaSpeed : 0.0f;
	}

	int CollidePhysicUnitAndVehicle(m3d::Object* obj1, m3d::Object* obj2, dContact* contact, unsigned& numContacts, bool)
	{
		// RVA 0x890970
		auto* const unit = static_cast<PhysicUnit*>(obj1);

		// The vehicle that hit the unit, whichever of its bodies touched it.
		int attackerId = static_cast<Obj*>(obj2)->GetId();
		if (obj2->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
		{
			attackerId = static_cast<Obj*>(obj2)->GetId();
		}
		else if (obj2->IsKindOf(RT_CLASS_LOCAL(VehiclePart)))
		{
			attackerId = static_cast<PhysicBody*>(obj2)->GetOwnerId();
		}
		else if (obj2->IsKindOf(RT_CLASS_LOCAL(Wheel)))
		{
			if (Vehicle* const vehicle = static_cast<Wheel*>(obj2)->GetVehicle())
			{
				attackerId = vehicle->GetId();
			}
		}

		float maxDepth = 0.0f;
		for (int i = 0; i < static_cast<int>(numContacts); ++i)
		{
			if (contact[i].geom.depth > maxDepth)
			{
				maxDepth = contact[i].geom.depth;
			}
		}

		// Either pushed deep enough or hit fast enough: the unit is run over.
		if (!(maxDepth < 0.2f) || !(CalcContactDeltaSpeed(*contact) < 0.1f))
		{
			if (attackerId != -1)
			{
				DamageInfo damageInfo;
				damageInfo.attackerId = attackerId;
				damageInfo.attackingAgentId = attackerId;
				unit->InflictDamage(damageInfo);
			}
			if (unit->m_physicBody->m_Node)
			{
				ActionType action = AT_DEATH2;
				unit->m_physicBody->m_Node->SetProperty(8708, &action);
			}
			unit->SetState(PhysicUnit::DEAD);
			return 0;
		}

		unit->OnCollideWithStandingVehicle();
		return 0;
	}

	int CollidePhysicUnitAndShell(m3d::Object* obj1, m3d::Object* obj2, dContact* contact, unsigned&, bool)
	{
		// RVA 0x890780
		auto* const unit = static_cast<PhysicUnit*>(obj1);
		auto* const shell = static_cast<Shell*>(obj2);

		unit->m_initVelocities = true;
		unit->m_causePos = shell->GetPosition();
		unit->m_causeForce = M3D_ENGINE_CFG.m_ai_ragdoll_shell_coeff.GetF();

		if (Obj* const emitter = shell->GetEmittedObj())
		{
			DamageInfo damageInfo;
			damageInfo.attackerId = emitter->GetId();
			damageInfo.attackingAgentId = shell->GetId();
			unit->InflictDamage(damageInfo);
		}

		if (unit->m_physicBody->m_Node)
		{
			ActionType action = AT_DEATH1;
			unit->m_physicBody->m_Node->SetProperty(8708, &action);
		}
		unit->SetState(PhysicUnit::DEAD);

		if (shell->IsKindOf(RT_CLASS_LOCAL(Bullet)))
		{
			static_cast<Bullet*>(shell)->SpecifyTracer(CVector(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2]));
		}
		return 0;
	}

	int CollidePhysicUnitAndBlastWave(m3d::Object* obj1, m3d::Object* obj2, dContact*, unsigned&, bool)
	{
		// RVA 0x890BE0
		auto* const unit = static_cast<PhysicUnit*>(obj1);
		auto* const blastWave = static_cast<BlastWave*>(obj2);

		unit->m_initVelocities = true;
		unit->m_causePos = blastWave->GetPosition();
		float const intensity = blastWave->GetWaveForceIntensity();
		unit->m_causeForce = intensity <= 20.0f ? intensity : 20.0f;

		// Only a blast fired from a gun credits the gun's owner.
		Obj* const emitter = theObjects->GetEntityByObjId(blastWave->GetEmitterId());
		if (emitter && emitter->IsKindOf(RT_CLASS_LOCAL(Gun)))
		{
			if (PhysicObj* const owner = static_cast<PhysicBody*>(emitter)->GetOwner())
			{
				DamageInfo damageInfo;
				damageInfo.attackingAgentId = blastWave->GetId();
				damageInfo.attackerId = owner->GetId();
				unit->InflictDamage(damageInfo);
			}
		}

		if (unit->m_physicBody->m_Node)
		{
			ActionType action = AT_DEATH1;
			unit->m_physicBody->m_Node->SetProperty(8708, &action);
		}
		unit->SetState(PhysicUnit::DEAD);
		return 0;
	}

	int CollideGeomObjAndShell(m3d::Object*, m3d::Object* obj2, dContact* contact, unsigned&, bool)
	{
		// RVA 0x890920
		if (obj2->IsKindOf(RT_CLASS_LOCAL(Bullet)))
		{
			static_cast<Bullet*>(obj2)->SpecifyTracer(CVector(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2]));
		}
		return 0;
	}
}

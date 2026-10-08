#include "passagedata.h"

#include <cmath>
#include <set>
#include <stdexcept>

#include "retruxx/common.h"
#include "core/kernel.h"
#include "core/log.h"
#include "m3dapp.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "server/obstacle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/dynamicquest.h"
#include "server/objects/location.h"
#include "server/objects/npcmotioncontroller.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"
#include "server/objects/physicbodies/physichelpers.h"
#include "server/objects/physicbodies/geoms/sphereforintersection.h"

namespace ai
{
	// RVA 0x672020
	void PassageData::SetPositionToPlayerVehicle()
	{
		if (!thePlayer->GetVehicle())
		{
			return;
		}

		Obj* const entity = theObjects->GetEntityByObjName(m_locationName);
		if (!entity || !entity->IsKindOf(RT_CLASS_LOCAL(Location)))
		{
			M3D_LOG_ERR(CStr("Error: invalid location name '") + m_locationName + CStr("' in passage data"));
			return;
		}
		Location* const location = static_cast<Location*>(entity);

		Vehicle* const vehicle = thePlayer->GetVehicle();
		vehicle->SetPosition(ZeroVector);
		VehiclePrototypeInfo const* const proto = vehicle->GetPrototypeInfo();

		CVector pos;
		{
			float const radius =
				vehicle->m_intersectionObstacle ? vehicle->m_intersectionObstacle->m_intersectionSphere->GetRadius() : 0.0f;
			std::set<m3d::Class*> const noClasses;
			if (!GetValidPosition(location->GetPosition(), radius, proto->m_priority, pos, false, true, noClasses))
			{
				pos = location->GetPosition();
			}
		}
		{
			M3D_LOG_INFO(CStr("New position is set to passed vehicle: ") + CStr(pos));
		}

		CVector dir = vehicle->GetDirection();
		if (m_angle == -1)
		{
			// Face away from the paired passage location.
			Location const* const corresponding = location->GetCorrespondingPassageLocation();
			if (corresponding && corresponding != location)
			{
				dir = (location->GetPosition() - corresponding->GetPosition()).getNormalized();
			}
		}
		else
		{
			CVector const INITIAL_UP_DIRECTION(0.0, 1.0, 0.0);
			Quaternion rot;
			rot.FromAxisAngle(INITIAL_UP_DIRECTION, static_cast<float>(m_angle) * 0.017453292f);
			CMatrix const rotMatrix = rot.ToMatrix();
			// The X axis turned by the rotation.
			dir.x = rotMatrix._11 + (rotMatrix._31 + rotMatrix._21) * 0.0f;
			dir.y = rotMatrix._12 + (rotMatrix._32 + rotMatrix._22) * 0.0f;
			dir.z = rotMatrix._13 + (rotMatrix._33 + rotMatrix._23) * 0.0f;
		}
		vehicle->SetDirection(dir);
		vehicle->SetGamePositionOnGround(pos, true, false);
		vehicle->SetCustomLinearVelocity(0.0f);

		// The first passed NPC vehicle is put next to the player.
		for (Obj* const obj : m_passingObjects)
		{
			if (!obj->IsKindOf(RT_CLASS_LOCAL(NPCMotionController)))
			{
				continue;
			}

			Vehicle* const npcVehicle = static_cast<NPCMotionController*>(obj)->getVehicleUnderControl();
			std::set<m3d::Class*> const noClasses;
			float const radius = npcVehicle->m_intersectionObstacle
				? npcVehicle->m_intersectionObstacle->m_intersectionSphere->GetRadius()
				: 0.0f;
			CVector newPos;
			if (GetValidPosition(pos, radius, 0xFF, newPos, false, false, noClasses))
			{
				npcVehicle->SetGamePositionOnGround(newPos, true, false);
			}
			else
			{
				npcVehicle->SetGamePositionOnGround(pos - dir * 10.0f, true, false);
			}
			return;
		}
	}

	// RVA 0x672750
	void PassageData::PutPassedObjectsToObjContainer()
	{
		M3D_ASSERT(theObjects->empty());

		for (Obj* const obj : m_passingObjects)
		{
			if (!theObjects->AddWithOwnObjId(obj))
			{
				M3D_LOG_ERR(CStr("Error: Object '") + CStr(obj->GetName()) + CStr("' could not pass to new map"));
				SYS_ERROR("!\"Error in passing to another map, see log\"");
			}
			if (obj->GetClass() == RT_CLASS_LOCAL(Player))
			{
				thePlayer = static_cast<Player*>(obj);
			}
		}

		M3D_APP->ImmediateMessage(66544, 0, 0, 0, 0, {}, {});
	}

	// RVA 0x62E7A0
	PassageData::PassageData() :
		m_angle(0),
		m_fadingStartTime(0)
	{
	}

	// RVA 0x671ED0
	void PassageData::TakeNeededObjectsFromObjContainer()
	{
		m_passingObjects.clear();

		for (Obj* const obj : *theObjects)
		{
			if (obj->IsKindOf(RT_CLASS_LOCAL(DynamicQuest)))
			{
				static_cast<DynamicQuest*>(obj)->PassToAnotherMap();
			}
		}

		if (thePlayer)
		{
			thePlayer->SetPassedToAnotherMapStatus();
		}

		for (Obj* const obj : *theObjects)
		{
			if (obj->IsKindOf(RT_CLASS_LOCAL(NPCMotionController)))
			{
				obj->SetPassedToAnotherMapStatus();
			}
		}

		for (Obj* const obj : *theObjects)
		{
			if (obj->GetPassedToAnotherMapStatus())
			{
				m_passingObjects.push_back(obj);
			}
		}
	}

	// RVA 0x671C30
	void PassageData::Dump()
	{
		{
			M3D_LOG_INFO("************************** Passed objects, begin **************************");
		}
		for (Obj* const obj : m_passingObjects)
		{
			CStr addr;
			addr.format("%08x", obj);
			M3D_LOG_INFO(obj->GetDebugDescription() + CStr(", address: ") + addr);
		}
		{
			M3D_LOG_INFO("************************** Passed objects, end **************************");
		}
	}

	// RVA 0x6067F0
	PassageData::~PassageData()
	{
	}
}

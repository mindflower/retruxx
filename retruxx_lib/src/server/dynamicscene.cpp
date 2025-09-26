#include "dynamicscene.h"

#include <stdexcept>
#include <ode/collision_space.h>
#include <ode/objects.h>

#include "geomobject.h"
#include "colliders/breakableobjectcolliders.h"
#include "colliders/bulletcolliders.h"
#include "colliders/colliderkrnl.h"
#include "colliders/defaultcollider.h"
#include "colliders/physicunitcolliders.h"
#include "colliders/vehiclecolliders.h"
#include "colliders/vehiclesplintercolliders.h"
#include "colliders/wheelasphaltcollider.h"
#include "objects/blastwave.h"
#include "objects/breakableobject.h"
#include "objects/dummyobject.h"
#include "objects/particlesplinter.h"
#include "objects/physicunit.h"
#include "objects/player.h"
#include "objects/staticautogun.h"
#include "objects/town.h"
#include "objects/vehiclesplinter.h"
#include "objects/wheel.h"
#include "objects/base/geomobj.h"
#include "objects/base/shell.h"
#include "objects/monsters/boss03part.h"
#include "objects/monsters/boss04part.h"
#include "objects/monsters/boss04stationpart.h"
#include "objects/monsters/bossmetalarmload.h"
#include "objects/physicbodies/vehiclepart.h"
#include "server.h"
#include "core/profilerstack.h"
#include "game/m3dgame.h"
#include "objects/vehicle.h"

namespace ai
{
	extern Player* thePlayer;

	namespace
	{
		dxJointGroup* contactGroup = nullptr;
	}

	RT_CLASS_EXPORTS_BEGIN(DynamicScene)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(DynamicScene);

	void DynamicScene::SoilProps::LoadFromXml(m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	DynamicScene::SoilProps::SoilProps()
	{
		throw retruxx::logic_error("Not implemented");
	}

	short DynamicScene::GetBoEffectTypeByName(CStr const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::DeleteAll()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::PurgeBodies()
	{
        if (thePlayer)
        {
            auto vehicle = thePlayer->GetVehicle();
            if (!vehicle
                || ((vehicle->GetFlags() & 8) != 0)
                || (vehicle->GetFlags() & 2) != 0
                || vehicle->GetParentRepository())
            {
                M3D_APP->ImmediateMessage(66544, 0, 0, 0, 0, {}, {});
                thePlayer->CauseEvent(GE_PLAYER_VEHICLE_CHANGED, 0.0, {}, {});
            }
        }
        theObjects->Purge();
	}

	CStr const& DynamicScene::GetBoEffectTypeName(unsigned short)
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* DynamicScene::Clone()
	{
		return Object::Clone();
	}

	void DynamicScene::LinkNodesFromBodyToSceneGraph(Obj*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	int DynamicScene::ProcessShellAndBody(Shell*, PhysicBody*, dContact*, unsigned&, bool)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::InitClashDecalId()
	{
		m_clashDecalId = AddDecalName("DC_CLASH");
	}

	void DynamicScene::ReadSoilProps(char const*)
	{
        // TODO: implement DynamicScene::ReadSoilProps
        // throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetShellWaterEffectName(unsigned short) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool DynamicScene::LoadSceneFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*, retruxx::vector<m3d::Class*> const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::StepScene(float elapsedTime)
	{
        if (elapsedTime >= 0.0001)
        {
            ai::pServer->GetTmpProfiler()->StartCountdown();
            dWorldQuickStep(ai::gGlobalWorld, elapsedTime);
            ai::pServer->GetTmpProfiler()->EndCountdown();
            dJointGroupEmpty(contactGroup);
        }
	}

	bool DynamicScene::SaveSceneToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	DynamicScene::SoilProps const& DynamicScene::GetSoilProps(unsigned, unsigned) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetShellStaticsEffectName(unsigned short) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetSoilEffectName(unsigned, unsigned short, bool) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::RenderDebugInfo()
	{
        // TODO: implement DynamicScene::RenderDebugInfo
        //throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* DynamicScene::CreateObject()
	{
		return new DynamicScene;
	}

	CStr const& DynamicScene::GetBoVehicleEffectName(unsigned short) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::CollideScene(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetShellVehicleEffectName(unsigned short) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::CreateBoShellEffectNames()
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetVehicleSoilEffectName(unsigned short) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* DynamicScene::GetBaseClass()
	{
		return RT_CLASS_LOCAL(Object);
	}

	CStr const& DynamicScene::GetDecalName(int)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::ClearOnce()
	{
		throw retruxx::logic_error("Not implemented");
	}

	int DynamicScene::GetNumNearCallbacksLastFrame()
	{
		throw retruxx::logic_error("Not implemented");
	}

	int DynamicScene::AddDecalName(CStr const& name)
	{
		for (int i = 0; i < m_decalsNames.size(); ++i)
		{
		    if (m_decalsNames[i] == name)
		    {
				return i;
		    }
		}
		m_decalsNames.push_back(name);
		return m_decalsNames.size() - 1;
	}

	int DynamicScene::GetClashDecalId()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::InitOnce()
	{
		ai::gGlobalSpace = dHashSpaceCreate(0);
		ai::gTempSpace = dHashSpaceCreate(0);
		ai::gBulletSpace = dHashSpaceCreate(ai::gGlobalSpace);
		dSpaceSetCleanup(ai::gGlobalSpace, 0);
		dSpaceSetCleanup(ai::gTempSpace, 0);
		dSpaceSetCleanup(ai::gBulletSpace, 0);
		ai::gIntersectionSpace = dSimpleSpaceCreate(0);
		dSpaceSetCleanup(ai::gIntersectionSpace, 0);
		ai::gSpaceForAllPhysicParticles = dSimpleSpaceCreate(ai::gGlobalSpace);
		dSpaceSetCleanup(ai::gSpaceForAllPhysicParticles, 0);
		ai::gGlobalWorld = dWorldCreate();
		dWorldSetGravity(ai::gGlobalWorld, 0.0, -9.8100004, 0.0);
		dWorldSetContactMaxCorrectingVel(ai::gGlobalWorld, 10.0);
		dWorldSetContactSurfaceLayer(ai::gGlobalWorld, 0.0099999998);
		dWorldSetCFM(ai::gGlobalWorld, 0.000099999997);
		dWorldSetERP(ai::gGlobalWorld, 0.1);
		dWorldSetQuickStepW(ai::gGlobalWorld, 1.0);
		dWorldSetAutoDisableFlag(ai::gGlobalWorld, 0);
		dWorldSetAutoDisableLinearThreshold(ai::gGlobalWorld, 0.1);
		dWorldSetAutoDisableAngularThreshold(ai::gGlobalWorld, 0.1);
		dWorldSetAutoDisableSteps(ai::gGlobalWorld, 5);
		dWorldSetAutoDisableTime(ai::gGlobalWorld, 0.0);
		dWorldSetDampingFlag(ai::gGlobalWorld, 1);
		dWorldSetDampingParameters(ai::gGlobalWorld, 0.1, 0.30000001);
		dWorldSetQuickStepNumIterations(ai::gGlobalWorld, 5);
		ai:contactGroup = dJointGroupCreate(0);
	}

	DynamicScene::~DynamicScene()
	{
		throw retruxx::logic_error("Not implemented");
	}

	unsigned DynamicScene::GetWheelTypeByName(CStr const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetBoShellEffectName(unsigned short, unsigned short)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::CollideBullet(Bullet const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool DynamicScene::LoadSceneFromFile(char const*, retruxx::vector<m3d::Class*> const&)
	{
        // TODO: implement DynamicScene::LoadSceneFromFile
        // throw std::logic_error("Not implemented");
        return true;
	}

	int DynamicScene::ReadNewObjectFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*,
		retruxx::vector<m3d::Class*> const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	short DynamicScene::GetExplosionType(CStr const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool DynamicScene::SaveSceneToFile(char const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	Vehicle* DynamicScene::GetVehicleControlledByPlayer() const
	{
		if (thePlayer)
		{
			return thePlayer->GetVehicle();
		}
		return nullptr;
	}

	CStr const& DynamicScene::GetRoadEffectName(unsigned, bool) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetShellRoadEffectName(unsigned short) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::Clear()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* DynamicScene::GetClass() const
	{
		return Object::GetClass();
	}

	CStr const& DynamicScene::GetShellEffectName(unsigned short, unsigned short) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::UpdateSceneItems(float)
	{
		throw retruxx::logic_error("Not implemented");
	}

	DynamicScene::DynamicScene(DynamicScene const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	DynamicScene::DynamicScene()
	{
		using namespace m3d;

		ColliderKrnl::Init();
		ColliderKrnl::RegisterCollider(0, 0, DefaultCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(GeomObjectWater),
			RT_CLASS_LOCAL(Object),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(DummyObject),
			RT_CLASS_LOCAL(Object),
			DefaultCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(DummyObject),
			RT_CLASS_LOCAL(VehiclePart),
			CollideDummyAndVehiclePart);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(ParticleSplinter),
			RT_CLASS_LOCAL(Object),
			CollideParticleSplinter);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(BreakableObject),
			CollideVehicleAndBreakableObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(VehiclePart),
			CollideVehiclePartAndVehiclePart);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			CollideVehicleAndLandscape);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectStatics),
			CollideVehicleAndStatics);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectRoad),
			CollideVehicleAndRoad);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollideVehicleAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(Town),
			CollideVehicleAndStatics);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Wheel), 0, CollideWheelDefault);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Wheel), RT_CLASS_LOCAL(Object), CollideWheelDefault);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(GeomObject),
			CollideWheelDefault);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(GeomObjectRoad),
			CollideWheelAndAsphalt);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			CollideWheelAndLandscape);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollideWheelAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Wheel),
			RT_CLASS_LOCAL(BreakableObject),
			CollideVehicleAndBreakableObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(PhysicObj),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(Wheel),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(PhysicBody),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(StaticAutoGun),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(VehiclePart),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(GeomObject),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollideShellAndWater);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Shell), 0, (int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(GeomObjectRoad),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(BreakableObject),
			CollideVehicleAndBreakableObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BreakableObject),
			RT_CLASS_LOCAL(GeomObject),
			CollideBreakableObjectAndGeomObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BreakableObject),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollidePOAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehicleSplinter),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			CollideVehicleSplinterAndLandscape);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehicleSplinter),
			RT_CLASS_LOCAL(GeomObjectRoad),
			CollideVehicleSplinterAndLandscape);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehicleSplinter),
			RT_CLASS_LOCAL(ParticleSplinter),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(DummyObject),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollidePOAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(ParticleSplinter),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollideParticleSplinterAndWater);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(BlastWave), RT_CLASS_LOCAL(Object), EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BlastWave),
			RT_CLASS_LOCAL(PhysicObj),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))BlastWave::CollideBlastWaveAndPhysicObj);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BlastWave),
			RT_CLASS_LOCAL(PhysicBody),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))BlastWave::CollideBlastWaveAndPhysicObj);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(PhysicUnit),
			RT_CLASS_LOCAL(VehiclePart),
			CollidePhysicUnitAndVehicle);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(PhysicUnit),
			RT_CLASS_LOCAL(Wheel),
			CollidePhysicUnitAndVehicle);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(PhysicUnit),
			RT_CLASS_LOCAL(Shell),
			CollidePhysicUnitAndShell);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(PhysicUnit),
			RT_CLASS_LOCAL(BlastWave),
			CollidePhysicUnitAndBlastWave);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(GeomObj),
			RT_CLASS_LOCAL(Object),
			CollideGeomObjAndLandscape);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(GeomObj), RT_CLASS_LOCAL(Shell), CollideGeomObjAndShell);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(GeomObj),
			RT_CLASS_LOCAL(GeomObjectWater),
			CollidePOAndWater);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BlastWave),
			RT_CLASS_LOCAL(GeomObj),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))BlastWave::CollideBlastWaveAndPhysicObj);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObj),
			CollideVehiclePartAndGeomObj);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(ParticleSplinter),
			RT_CLASS_LOCAL(GeomObj),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(BossMetalArmLoad),
			RT_CLASS_LOCAL(Object),
			BossMetalArmLoad::CollideBossMetalArmLoadWithObject);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Shell),
			RT_CLASS_LOCAL(BossMetalArmLoad),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))ProcessShellAndBody);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss03Part),
			RT_CLASS_LOCAL(VehiclePart),
			(int(*)(Object*, Object*, dContact*, unsigned&, bool))Boss03Part::CollideBoss03PartAndVehiclePart);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Boss03Part), RT_CLASS_LOCAL(Wheel), EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss04Part),
			RT_CLASS_LOCAL(VehiclePart),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(RT_CLASS_LOCAL(Boss04Part), RT_CLASS_LOCAL(Wheel), EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss04Part),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss04StationPart),
			RT_CLASS_LOCAL(Boss04StationPart),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Boss04StationPart),
			RT_CLASS_LOCAL(GeomObjectLandscape),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(Object),
			RT_CLASS_LOCAL(GeomObjectPassCell),
			EmptyCollider);
		ColliderKrnl::RegisterCollider(
			RT_CLASS_LOCAL(VehiclePart),
			RT_CLASS_LOCAL(GeomObjectPassCell),
			CollideVehicleAndPassCell);
	}

	void DynamicScene::_InitWheelTraces()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::_RecalcWheelEffectNames()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void DynamicScene::_AddSoilEffectNameForWheelTypeName(CStr const&)
	{
		throw retruxx::logic_error("Not implemented");
	}
}

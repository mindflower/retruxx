#include "dynamicscene.h"

#include <stdexcept>
#include <ode/collision.h>
#include <ode/collision_space.h>
#include <ode/objects.h>

#include "geomobject.h"
#include "passagedata.h"
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
#include "core/ini.h"
#include "core/log.h"
#include "core/profilerstack.h"
#include "game/m3dgame.h"
#include "objects/dynamicquestdestroy.h"
#include "objects/vehicle.h"
#include "objects/base/globalproperties.h"
#include "objects/base/jointedobj.h"
#include "objects/base/prototypemanager.h"

namespace ai
{
	extern Player* thePlayer;

	namespace
	{
		dxJointGroup* contactGroup = nullptr;
	    int numNearCallbacksLastFrame = 0;
	}

    void NearCallback(void*, dxGeom*, dxGeom*)
	{
        // TODO: implement NearCallback
        //throw retruxx::logic_error("Not implemented");
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
        // TODO: implement DynamicScene::PurgeBodies
        //throw retruxx::logic_error("Not implemented");
        //if (thePlayer)
        //{
        //    auto vehicle = thePlayer->GetVehicle();
        //    if (!vehicle
        //        || ((vehicle->GetFlags() & 8) != 0)
        //        || (vehicle->GetFlags() & 2) != 0
        //        || vehicle->GetParentRepository())
        //    {
        //        M3D_APP->ImmediateMessage(66544, 0, 0, 0, 0, {}, {});
        //        thePlayer->CauseEvent(GE_PLAYER_VEHICLE_CHANGED, 0.0, {}, {});
        //    }
        //}
        //theObjects->Purge();
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

	bool DynamicScene::LoadSceneFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* rootNode, retruxx::vector<m3d::Class*> const& allowedClasses)
	{
        // TODO: generated code
        // Validate allowed classes
        if (allowedClasses.empty())
        {
            M3D_ASSERT(ai::theObjects->empty());
        }

        // Create XML node for parsing
        ref_ptr xmlNode = xmlFile->CreateNode();

        if (rootNode->IsEmpty())
        {
            return false;
        }

        // Reset physic time accumulator
        this->m_physicTimeAccumulator = 0.0;

        // Load physic time accumulator if doing full save
        if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            m3d::SafeFloatAttrib(m_physicTimeAccumulator, rootNode, "PhysicTimeAccumulator");
        }

        // Load relationships
        rootNode->GetFirstChild(xmlNode, "relationship");
        if (!xmlNode->IsEmpty() && xmlNode->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            // Clean up existing relationship
            delete theRelationship;
            theRelationship = nullptr;

            // Create and load new relationship
            ai::theRelationship = new ai::Relationship();
            ai::theRelationship->LoadFromXML(xmlFile, xmlNode);
            ai::theRelationship->LoadDefaultFromXmlFile(ai::theGlobProp.m_pathToRelationship.c_str());
        }

        // Load target names for destruction
        rootNode->GetFirstChild(xmlNode, "TargetNamesForDestroy");
        ai::DynamicQuestDestroy::LoadNamesForTargetsFromXml(xmlNode);

        // Load runtime object container state for full save
        if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            rootNode->GetFirstChild(xmlNode, "ObjContainerRuntime");
            if (!xmlNode->IsEmpty())
            {
                theObjects->LoadNodeStatesFromXml(xmlFile, xmlNode);
            }
        }

        // Process passage data if exists
        if (ai::thePassageData)
        {
            thePassageData->PutPassedObjectsToObjContainer();
        }

        // Validate event recipients for all objects
        for (auto* obj : *theObjects)
        {
            obj->ValidateEventRecipientsList();
        }

        // Deny object creation during loading for full save
        if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            theObjects->DenyCreation();
        }

        // Load objects from XML
        rootNode->GetFirstChild(xmlNode, "Object");
        while (!xmlNode->IsEmpty())
        {
            this->ReadNewObjectFromXml(xmlFile, xmlNode, allowedClasses);
            xmlNode->GetNextSibling(xmlNode, "Object");
        }

        // Permit creation and purge objects
        if (ai::theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            theObjects->PermitCreation();
        }
        theObjects->Purge();

        // Create player if doesn't exist
        if (!ai::thePlayer)
        {
            CStr prototypeName("player");
            int prototypeId = thePrototypeManager->GetPrototypeId(prototypeName);
            int objId = theObjects->CreateNewObject(prototypeId, "Player1", -1, -1);

            ai::thePlayer = dynamic_cast<ai::Player*>(ai::theObjects->GetEntityByObjId(objId));
            thePlayer->SetBelong(1100);
        }

        // Set player name properly
        CStr playerName(ai::thePlayer->GetName());
        CStr tempName("random name which cannot be used !$%");

        theObjects->SetObjName(ai::thePlayer->GetId(), tempName);
        theObjects->SetObjName(ai::thePlayer->GetId(), playerName);

        // Handle player vehicle naming
        ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
        if (vehicle)
        {
            CStr vehicleName(vehicle->GetName());
            CStr tempVehicleName("random name which cannot be used !$%");

            theObjects->SetObjName(vehicle->GetId(), tempVehicleName);
            theObjects->SetObjName(vehicle->GetId(), vehicleName);
        }

        // Register player as global
        m3d::g_Kernel->UnRegisterGlobal("g_Player");
        m3d::g_Kernel->RegisterGlobal(ai::thePlayer, "g_Player");

        // Post-load processing for all objects
        for (auto* obj : *theObjects)
        {
            obj->PostLoad();

            // Send immediate messages based on object flags
            if (obj->GetFlags() & 2)
            { // Some specific flag
                M3D_APP->ImmediateMessage(66542, (int)obj, 0, 0, 0, {}, {});
            }

            if (obj->GetFlags() & 8)
            {
                M3D_APP->ImmediateMessage(66542, (int)obj, 0, 0, 0, {}, {});
            }

        }

        // Create visual parts for objects that need them
        for (auto* obj : *theObjects)
        {
            if ((obj->GetFlags() & 2) == 0)
            {
                obj->CreateVisualPart();
            }
        }

        // Clean up passage data
        if (ai::thePassageData)
        {
            thePassageData->SetPositionToPlayerVehicle();
            delete ai::thePassageData;
            ai::thePassageData = nullptr;
        }

        // Send completion message
        M3D_APP->EnqueueMessage(66544, 0, 0, 0, 0, {}, {});

        // Load last ID
        int lastId = 0;
        m3d::SafeIntAttrib(lastId, rootNode, "LastId");
        ai::pServer->SetLastId(lastId);

        return 1;
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

	void DynamicScene::CollideScene(float elapsedTime)
	{
        if (elapsedTime >= 0.0001)
        {
            numNearCallbacksLastFrame = 0;
            ai::pServer->GetCollideProfiler()->StartCountdown();

            dSpaceCollide(ai::gGlobalSpace, 0, ai::NearCallback);
            
            ai::pServer->GetCollideProfiler()->EndCountdown();

            theObjects->PostCollide();
        }
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

	bool DynamicScene::LoadSceneFromFile(char const* fileName, retruxx::vector<m3d::Class*> const& allowedClasses)
	{
        CStr err;
        if (ref_ptr xmlFile = m3d::ReadXmlFile(fileName, &err))
        {
            ref_ptr xmlNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(xmlNode, "DynamicScene");

            M3D_LOG_INFO("\t\t Scene loading begin");
            LoadSceneFromXml(xmlFile, xmlNode, allowedClasses);
            M3D_LOG_INFO("\t\t Scene loading end");
            return true;
        }
        M3D_LOG_ERR("Error: No DynamicScene file: " + CStr(fileName));
        return false;
	}

	int DynamicScene::ReadNewObjectFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode,
		retruxx::vector<m3d::Class*> const& allowedClasses)
	{
        CStr name = xmlNode->GetAttribute("Name");
        CStr prototypeName = xmlNode->GetAttribute("Prototype");

        auto prototypeId = thePrototypeManager->GetPrototypeId(prototypeName);
        if (prototypeId == -1)
        {
            M3D_LOG_ERR("Error: unknown prototype in DynamicScene.xml: '" + prototypeName + "'");
            return -1;
        }

        if (!allowedClasses.empty())
        {
            throw retruxx::logic_error("Not implemented");
        }

        int objId = -1;
        m3d::SafeIntAttrib(objId, xmlNode, "ObjectId");

        if (theObjects->GetObjIdByObjName(name) != -1)
        {
            M3D_LOG_ERR("Attempting to load object " + name + " of prototype " + prototypeName + ", but an object with this name already exists!");
        }

        auto entityForLoadId = theObjects->CreateEntityForLoad(prototypeId, name.c_str(), -1, objId);
        if (entityForLoadId == -1)
        {
            return -1;
        }

        auto* obj = theObjects->GetEntityByObjId(entityForLoadId);
        if (!obj)
        {
            M3D_LOG_ERR("Error: object with id " + CStr(entityForLoadId) + " was created but it is not in the ObjContainer");
            return -1;
        }

        obj->LoadFromXML(xmlFile, xmlNode);
        if (obj->GetClass() == RT_CLASS_LOCAL(Player))
        {
            if (ai::thePlayer)
                obj->Remove();
            else
                ai::thePlayer = (Player*)obj;
        }

        if (obj->IsKindOf(&ai::JointedObj::m_classJointedObj) && ai::theObjects->m_SaveType != ObjContainer::SAVE_FULL)
        {
            M3D_CRITICAL_ERROR("invalid object in DynamicScene : " + obj->GetDebugDescription());
        }

        return obj->GetId();
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

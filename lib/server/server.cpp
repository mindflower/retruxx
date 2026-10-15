#include "server.h"

#include <client.h>
#include "affix.h"
#include "quest.h"
#include "resourcemanager.h"
#include "tracelinemanager.h"
#include "objects/vehiclesgenerator.h"
#include "objects/base/prototypemanager.h"
#include "dynamicscene.h"
#include "geomrepository.h"
#include "intersectionmanager.h"
#include "izvratrepository.h"
#include "queststate.h"
#include "static2dpath.h"
#include "ai/aimanager.h"
#include "ai/decisionmatrix.h"
#include "objects/articulatedvehicle.h"
#include "objects/bar.h"
#include "objects/barricade.h"
#include "objects/basket.h"
#include "objects/blastwave.h"
#include "objects/breakableobject.h"
#include "objects/cabin.h"
#include "objects/caravanteam.h"
#include "objects/chassis.h"
#include "objects/cinematicmover.h"
#include "objects/dummyobject.h"
#include "objects/dynamicquestconvoy.h"
#include "objects/dynamicquestdestroy.h"
#include "objects/dynamicquesthunt.h"
#include "objects/dynamicquestpeace.h"
#include "objects/dynamicquestreach.h"
#include "objects/gadget.h"
#include "objects/infectionlair.h"
#include "objects/infectionteam.h"
#include "objects/infectionzone.h"
#include "objects/lair.h"
#include "objects/lightobj.h"
#include "objects/npc.h"
#include "objects/npcmotioncontroller.h"
#include "objects/particlesplinter.h"
#include "objects/physicunit.h"
#include "objects/player.h"
#include "objects/settlement.h"
#include "objects/sgnodeobj.h"
#include "objects/staticautogun.h"
#include "objects/town.h"
#include "objects/trigger.h"
#include "objects/vagabondteam.h"
#include "objects/vehiclerecollection.h"
#include "objects/vehiclesplinter.h"
#include "objects/ware.h"
#include "objects/wheel.h"
#include "objects/workshop.h"
#include "objects/chest.h"
#include "objects/radiomanager.h"
#include "objects/ropeobj.h"
#include "objects/repositoryobjectsgenerator.h"
#include "objects/wanderersgenerator.h"
#include "objects/wanderersmanager.h"
#include "objects/base/animatedcomplexphysicobj.h"
#include "objects/base/complexphysicobj.h"
#include "objects/base/compositeobj.h"
#include "objects/base/geomobj.h"
#include "objects/base/globalproperties.h"
#include "objects/base/jointedobj.h"
#include "objects/base/objprefab.h"
#include "objects/base/physicobj.h"
#include "objects/base/shell.h"
#include "objects/base/simplephysicobj.h"
#include "objects/guns/bullet.h"
#include "objects/guns/bulletlauncher.h"
#include "objects/guns/compoundgun.h"
#include "objects/guns/engineoillocation.h"
#include "objects/guns/gun.h"
#include "objects/guns/locationpusher.h"
#include "objects/guns/naillocation.h"
#include "objects/guns/mine.h"
#include "objects/guns/minepusher.h"
#include "objects/guns/mortar.h"
#include "objects/guns/mortarshell.h"
#include "objects/guns/mortarvolleylauncher.h"
#include "objects/guns/plasmabunch.h"
#include "objects/guns/plasmabunchlauncher.h"
#include "objects/guns/smokescreenlocation.h"
#include "objects/guns/rocket.h"
#include "objects/guns/rocketlauncher.h"
#include "objects/guns/rocketvolleylauncher.h"
#include "objects/guns/thunderboltlauncher.h"
#include "objects/guns/thunderbolt.h"
#include "objects/guns/turboaccelerationpusher.h"
#include "objects/monsters/boss02.h"
#include "objects/monsters/boss02arm.h"
#include "objects/monsters/boss03.h"
#include "objects/monsters/boss03part.h"
#include "objects/monsters/boss04.h"
#include "objects/monsters/boss04drone.h"
#include "objects/monsters/boss04part.h"
#include "objects/monsters/boss04station.h"
#include "objects/monsters/boss04stationpart.h"
#include "objects/monsters/bossmetalarm.h"
#include "objects/monsters/bossmetalarmload.h"
#include "objects/monsters/submarine.h"
#include "objects/physicbodies/boxybody.h"
#include "objects/physicbodies/compoundvehiclepart.h"
#include "objects/physicbodies/physicbody.h"
#include "objects/physicbodies/sphericbody.h"
#include "roles/teamtactic.h"
#include "roles/vehiclerolebarrier.h"
#include "roles/vehiclerolecheater.h"
#include "roles/vehiclerolecoward.h"
#include "roles/vehiclerolemeat.h"
#include "roles/vehicleroleoppressor.h"
#include "roles/vehiclerolependulum.h"
#include "roles/vehiclerolesniper.h"
#include "statistic/favoritestringstatistic.h"
#include "statistic/floatstatistic.h"
#include "statistic/intintratiostatistic.h"
#include "statistic/intstatistic.h"
#include "statistic/timestatistic.h"
#include <config.h>
#include <m3dapp.h>
#include <stdexcept>
#include <core/kernel.h>
#include "core/ini.h"
#include <core/log.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "externalpaths.h"
#include "level.h"
#include "map.h"
#include "playerpassmap.h"
#include "processmanager.h"
#include "world.h"
#include "core/timer.h"
#include "objects/affixgenerator.h"

namespace ai
{
    extern ResourceManager* theResourceManager;
    extern PrototypeManager* thePrototypeManager;
    extern Relationship* theRelationship;
    extern Player* thePlayer;
    extern ProcessManager* theProcessManager;
    extern AIManager* theAIManager;

    int n_AddToCinematic(m3d::sArgStack& scriptStack)
    {
        // RVA 0x5F3440 - the object may be named by id, by name or handed over directly; the
        // optional second argument says whether its children come along.
        if (scriptStack.m_numInArgs == 0 || scriptStack.m_numInArgs > 2)
        {
            return -1;
        }

        m3d::sArg* const objArg = scriptStack.popIn();
        bool withChildren = true;
        if (scriptStack.m_numInArgs == 2)
        {
            m3d::sArg* const flagArg = scriptStack.popIn();
            switch (flagArg->m_type)
            {
            case m3d::sArg::ARGTYPE_INT:
                withChildren = flagArg->GetI() != 0;
                break;
            case m3d::sArg::ARGTYPE_FLOAT:
                withChildren = flagArg->GetF() > 0.0f;
                break;
            case m3d::sArg::ARGTYPE_BOOL:
                withChildren = flagArg->GetB();
                break;
            default:
                break;
            }
        }

        Obj* obj = nullptr;
        switch (objArg->m_type)
        {
        case m3d::sArg::ARGTYPE_INT:
            obj = theObjects->GetEntityByObjId(objArg->GetI());
            break;
        case m3d::sArg::ARGTYPE_FLOAT:
            obj = theObjects->GetEntityByObjId(static_cast<int>(objArg->GetF()));
            break;
        case m3d::sArg::ARGTYPE_STRING:
            obj = theObjects->GetEntityByObjName(CStr(objArg->GetS()));
            break;
        case m3d::sArg::ARGTYPE_OBJECT:
            obj = static_cast<Obj*>(objArg->GetO());
            break;
        default:
            return 1;
        }
        if (obj)
        {
            pServer->AddToCinematic(obj, withChildren);
        }
        return 1;
    }

    int n_EndCinematic(m3d::sArgStack& scriptStack)
    {
        // RVA 0x5F1910 - NOTE: this clears the flag directly and does not reset the objects that
        // were marked, unlike CServer::EndCinematic.
        if (pServer)
        {
            pServer->m_InCinematic = false;
        }
        return 1;
    }

    int n_CreateObjectByClassName(m3d::sArgStack& scriptStack)
    {
        // RVA 0x5F1930 - builds an engine object straight from its RTTI class name.
        if (scriptStack.m_numInArgs != 1)
        {
            return -1;
        }
        m3d::sArg* const classNameArg = scriptStack.popIn();
        if (classNameArg->m_type != m3d::sArg::ARGTYPE_STRING)
        {
            return -1;
        }

        CStr const className(classNameArg->GetS());
        m3d::Object* const newObject = M3D_KERNEL->New(className.c_str());
        scriptStack.newOut()->SetO(newObject);
        return newObject != nullptr;
    }

    void CServer::AddToCinematic(Obj* obj, bool withChildren)
    {
        // RVA 0x5F1780 - NOTE: a null object is accepted and quietly ignored.
        if (!obj)
        {
            return;
        }
        obj->m_flags |= 0x10u;
        if (withChildren)
        {
            for (int i = 0; Obj* const child = obj->GetChild(i); ++i)
            {
                AddToCinematic(child, true);
            }
        }
    }

    void CServer::AddToCinematic(int id, bool withChildren)
    {
        // RVA 0x5F33F0
        AddToCinematic(theObjects->GetEntityByObjId(id), withChildren);
    }

    void CServer::SetPause(bool pause)
    {
        // RVA 0x406E30
        fPause = pause;
    }

    m3d::Profiler* CServer::GetTmpProfiler()
    {
        return m_profilerTmpForServer;
    }

    m3d::Profiler* CServer::GetBulletProfiler()
    {
        return m_bulletProfiler;
    }

    m3d::Profiler* CServer::GetCollideProfiler()
    {
        return m_collideProfiler;
    }

    m3d::Profiler* CServer::GetObjectsUpdateProfiler()
    {
        // RVA 0x5F1320
        return m_objectsUpdateProfiler;
    }

    ExternalPaths const* CServer::GetExternalPaths() const
    {
        return this->m_pExternalPaths;
    }

    PlayerPassMap const* CServer::GetPlayerPassMap() const
    {
        // RVA 0x602680
        return m_pPlayerPassMap;
    }

    void CServer::SetLastId(int id)
    {
        m_lastId = id;
    }

    void CServer::Load(
        StartupMode mode,
        m3d::cmn::XmlFile* xmlFile,
        m3d::cmn::XmlNode const* xmlNode,
        bool bContiniousMap,
        ObjContainer::eSAVE_TYPES saveType)
    {
        M3D_LOG_INFO("AI: Loading Server...");

        auto saveSaveType = ai::theObjects->m_SaveType;
        ai::theObjects->m_SaveType = saveType;

        if (!bContiniousMap)
        {
            M3D_LOG_INFO("\tAI: Loading Relationship");
            ai::theRelationship->LoadFromXmlFile(ai::theGlobProp.m_pathToRelationship.c_str());
            ai::theRelationship->LoadDefaultFromXmlFile(ai::theGlobProp.m_pathToRelationship.c_str());
        }

        M3D_LOG_INFO("\tAI: Loading SoilProps");
        m_pDynamicScene->ReadSoilProps(M3D_ENGINE_CFG.m_pathToSoilProps.GetS());

        M3D_LOG_INFO("\tAI: Loading ExternalPaths");
        m_pExternalPaths->Load(m_level->GetFullPathNameA(this->m_level->m_externalPathsFileName));

        M3D_LOG_INFO("\tAI: Loading PlayerPassMap");
        m_pPlayerPassMap->LoadFromBinaryFile(m_level->GetFullPathNameA(this->m_level->m_playerPassMapFileName));

        if (!bContiniousMap)
        {
            M3D_LOG_INFO("\tAI: Loading GameObjects");
            ai::thePrototypeManager->LoadFromXmlFile(ai::theGlobProp.m_pathToGameObjects);

            M3D_LOG_INFO("\tAI: Initializing VehicleGeneratorInfoCache");
            ai::theVehiclesGeneratorInfoCache->EnsureInitialized();
        }

        M3D_LOG_INFO("\tAI: Refreshing GameObjects");
        ai::thePrototypeManager->RefreshFromXmlFile(ai::theGlobProp.m_pathToGameObjects);

        M3D_LOG_INFO("\tAI: Loading QuestStates");
        ai::theQuestStateManager->LoadFromXmlFile(m_level->GetFullPathNameA(this->m_level->m_questStatesFileName));
        M3D_LOG_INFO("\tAI: QuestStates loaded");

        M3D_LOG_INFO("\tAI: Loading DynamicScene");
        if (xmlNode)
        {
            retruxx::vector<m3d::Class*> classes;
            m_pDynamicScene->LoadSceneFromXml(xmlFile, xmlNode, classes);
        }
        else
        {
            retruxx::vector<m3d::Class*> classes;
            m_pDynamicScene->LoadSceneFromFile(m_level->GetFullPathNameA(this->m_level->m_dsSrvName).c_str(), classes);
        }
        M3D_LOG_INFO("\tAI: DynamicScene loaded");

        ai::UpdateLights();
        m_StartServerUpdates = 1;

        M3D_LOG_INFO("\tAI: Loading Triggers");
        LoadTriggersFromXML(m_level->GetFullPathNameA(this->m_level->m_TriggersName));
        LoadTriggersFromXML(m_level->GetFullPathNameA(this->m_level->m_cinemaTriggersName));
        M3D_LOG_INFO("\tAI: Triggers loaded");

        M3D_LOG_INFO("\tAI: Loading Object Names");
        ai::theObjects->LoadObjectNamesFromXML(m_level->GetFullPathNameA(this->m_level->m_ObjectFullNames));

        LoadPrototypeNamesFromXML(this->m_level->m_prototypeFullNames);
        m_LastSenderID = 0;

        M3D_LOG_INFO("\tAI: Initing Player");
        m3d::sArgStack stack;
        if (auto res = M3D_KERNEL->GetScriptServer().callScriptFunc("InitPlayer", stack, 0))
        {
            auto formatedScriptErrorDesc = M3D_KERNEL->GetScriptServer().getFormatedScriptErrorDesc(res);
            M3D_LOG_ERR(formatedScriptErrorDesc);
        }

        ai::theProcessManager->Update(0.0001f, 1u, 2u);
        if (ai::thePlayer && ai::theObjects->m_SaveType != ai::ObjContainer::eSAVE_TYPES::SAVE_FULL)
        {
            ai::thePlayer->CauseEvent(GE_GAME_START, 0.0, {}, {});
        }

        M3D_LOG_INFO("\t\tBefore first Update");
        ai::theProcessManager->Update(0.050000001f, 1u, 2u);
        if (ai::theObjects->m_SaveType == ai::ObjContainer::eSAVE_TYPES::SAVE_LEVEL)
        {
            Update(0.0099999998f);
            Update(0.0099999998f);
            Update(0.0099999998f);
        }
        M3D_LOG_INFO("\t\tAfter first Update");

        M3D_LOG_INFO("AI: Finished Loading Server");

        ai::theObjects->m_SaveType = saveSaveType;
    }

    bool CServer::GetPause() const
    {
        // RVA 0x406E20
        return fPause;
    }

    int CServer::GetPrototypeId(CStr const& prototypeName)
    {
        // RVA 0x5EFA60
        return thePrototypeManager->GetPrototypeId(prototypeName);
    }

    PrototypeInfo* CServer::CreatePrototypeInfoByClassName(CStr const& className)
    {
        if (className == "Vehicle")
            return new VehiclePrototypeInfo;
        if (className == "ArticulatedVehicle")
            return new ArticulatedVehiclePrototypeInfo;
        if (className == "VehiclePart")
            return new VehiclePartPrototypeInfo;
        if (className == "CompoundVehiclePart")
            return new CompoundVehiclePartPrototypeInfo;
        if (className == "Chassis")
            return new ChassisPrototypeInfo;
        if (className == "Cabin")
            return new CabinPrototypeInfo;
        if (className == "Basket")
            return new BasketPrototypeInfo;
        if (className == "Trigger")
            return new TriggerPrototypeInfo;
        if (className == "DynamicQuestDestroy")
            return new DynamicQuestDestroyPrototypeInfo;
        if (className == "DynamicQuestReach")
            return new DynamicQuestReachPrototypeInfo;
        if (className == "DynamicQuestConvoy")
            return new DynamicQuestConvoyPrototypeInfo;
        if (className == "DynamicQuestPeace")
            return new DynamicQuestPeacePrototypeInfo;
        if (className == "DynamicQuestHunt")
            return new DynamicQuestHuntPrototypeInfo;
        if (className == "Wheel")
            return new WheelPrototypeInfo;
        if (className == "StaticAutoGun")
            return new StaticAutoGunPrototypeInfo;
        if (className == "Town")
            return new TownPrototypeInfo;
        if (className == "Building")
            return new BuildingPrototypeInfo;
        if (className == "Workshop")
            return new WorkshopPrototypeInfo;
        if (className == "Bar")
            return new BarPrototypeInfo;
        if (className == "Npc")
            return new NpcPrototypeInfo;
        if (className == "Lair")
            return new LairPrototypeInfo;
        if (className == "Location")
            return new LocationPrototypeInfo;
        if (className == "Chest")
            return new ChestPrototypeInfo;
        if (className == "Team")
            return new TeamPrototypeInfo;
        if (className == "CaravanTeam")
            return new CaravanTeamPrototypeInfo;
        if (className == "VagabondTeam")
            return new VagabondTeamPrototypeInfo;
        if (className == "BreakableObject")
            return new BreakableObjectPrototypeInfo;
        if (className == "RopeObj")
            return new RopeObjPrototypeInfo;
        if (className == "DummyObject")
            return new DummyObjectPrototypeInfo;
        if (className == "Ware")
            return new WarePrototypeInfo;
        if (className == "Gadget")
            return new GadgetPrototypeInfo;
        if (className == "PhysicUnit")
            return new PhysicUnitPrototypeInfo;
        if (className == "JointedObj")
            return new JointedObjPrototypeInfo;
        if (className == "CompositeObj")
            return new CompositeObjPrototypeInfo;
        if (className == "GeomObj")
            return new GeomObjPrototypeInfo;
        if (className == "VehicleSplinter")
            return new VehicleSplinterPrototypeInfo;
        if (className == "ParticleSplinter")
            return new ParticleSplinterPrototypeInfo;
        if (className == "Player")
            return new PlayerPrototypeInfo;
        if (className == "ObjPrefab")
            return new ObjPrefabPrototypeInfo;
        if (className == "Barricade")
            return new BarricadePrototypeInfo;
        if (className == "SgNodeObj")
            return new SgNodeObjPrototypeInfo;
        if (className == "LightObj")
            return new LightObjPrototypeInfo;
        if (className == "InfectionTeam")
            return new InfectionTeamPrototypeInfo;
        if (className == "InfectionZone")
            return new InfectionZonePrototypeInfo;
        if (className == "InfectionLair")
            return new InfectionLairPrototypeInfo;
        if (className == "BlastWave")
            return new BlastWavePrototypeInfo;
        if (className == "RepositoryObjectsGenerator")
            return new RepositoryObjectsGeneratorPrototypeInfo;
        if (className == "AffixGenerator")
            return new AffixGeneratorPrototypeInfo;
        if (className == "VehicleRecollection")
            return new VehicleRecollectionPrototypeInfo;
        if (className == "VehiclesGenerator")
            return new VehiclesGeneratorPrototypeInfo;
        if (className == "WanderersGenerator")
            return new WanderersGeneratorPrototypeInfo;
        if (className == "WanderersManager")
            return new WanderersManagerPrototypeInfo;
        if (className == "BossMetalArm")
            return new BossMetalArmPrototypeInfo;
        if (className == "BossMetalArmLoad")
            return new BossMetalArmLoadPrototypeInfo;
        if (className == "Boss02")
            return new Boss02PrototypeInfo;
        if (className == "Boss02Arm")
            return new Boss02ArmPrototypeInfo;
        if (className == "Boss03")
            return new Boss03PrototypeInfo;
        if (className == "Boss03Part")
            return new Boss03PartPrototypeInfo;
        if (className == "Boss04")
            return new Boss04PrototypeInfo;
        if (className == "Boss04Part")
            return new Boss04PartPrototypeInfo;
        if (className == "Boss04Station")
            return new Boss04StationPrototypeInfo;
        if (className == "Boss04StationPart")
            return new Boss04StationPartPrototypeInfo;
        if (className == "Boss04Drone")
            return new Boss04DronePrototypeInfo;
        if (className == "Submarine")
            return new SubmarinePrototypeInfo;
        if (className == "TeamTacticWithRoles")
            return new TeamTacticWithRolesPrototypeInfo;
        if (className == "VehicleRoleMeat")
            return new VehicleRoleMeatPrototypeInfo;
        if (className == "VehicleRoleSniper")
            return new VehicleRoleSniperPrototypeInfo;
        if (className == "VehicleRoleOppressor")
            return new VehicleRoleOppressorPrototypeInfo;
        if (className == "VehicleRoleBarrier")
            return new VehicleRoleBarrierPrototypeInfo;
        if (className == "VehicleRoleCoward")
            return new VehicleRoleCowardPrototypeInfo;
        if (className == "VehicleRolePendulum")
            return new VehicleRolePendulumPrototypeInfo;
        if (className == "VehicleRoleCheater")
            return new VehicleRoleCheaterPrototypeInfo;
        if (className == "NPCMotionController")
            return new NPCMotionControllerPrototypeInfo;
        if (className == "CinematicMover")
            return new CinematicMoverPrototypeInfo;
        if (className == "Bullet")
            return new BulletPrototypeInfo;
        if (className == "Rocket")
            return new RocketPrototypeInfo;
        if (className == "BulletLauncher")
            return new BulletLauncherPrototypeInfo;
        if (className == "RocketLauncher")
            return new RocketLauncherPrototypeInfo;
        if (className == "CompoundGun")
            return new CompoundGunPrototypeInfo;
        if (className == "PlasmaBunch")
            return new PlasmaBunchPrototypeInfo;
        if (className == "PlasmaBunchLauncher")
            return new PlasmaBunchLauncherPrototypeInfo;
        if (className == "RocketVolleyLauncher")
            return new RocketVolleyLauncherPrototypeInfo;
        if (className == "Thunderbolt")
            return new ThunderboltPrototypeInfo;
        if (className == "ThunderboltLauncher")
            return new ThunderboltLauncherPrototypeInfo;
        if (className == "MortarShell")
            return new MortarShellPrototypeInfo;
        if (className == "Mortar")
            return new MortarPrototypeInfo;
        if (className == "MortarVolleyLauncher")
            return new MortarVolleyLauncherPrototypeInfo;
        if (className == "Mine")
            return new MinePrototypeInfo;
        if (className == "MinePusher")
            return new MinePusherPrototypeInfo;
        if (className == "LocationPusher")
            return new LocationPusherPrototypeInfo;
        if (className == "EngineOilLocation")
            return new EngineOilLocationPrototypeInfo;
        if (className == "NailLocation")
            return new NailLocationPrototypeInfo;
        if (className == "SmokeScreenLocation")
            return new SmokeScreenLocationPrototypeInfo;
        if (className == "TurboAccelerationPusher")
            return new TurboAccelerationPusherPrototypeInfo;
        if (className == "RadioManager")
            return new RadioManagerPrototypeInfo;
        if (className == "Formation")
            return new FormationPrototypeInfo;
        if (className == "QuestItem")
            return new QuestItemPrototypeInfo;

        return nullptr;
    }

    m3d::CWorld* CServer::GetWorld()
    {
        return m_pWorld;
    }

    void CServer::LoadVisitedMap(CStr const& mapFileName, bool bContiniousMap)
    {
        // RVA 0x5F6710 - the saved scene supplies everything except the barricades, which are
        // dropped and re-read from the level's own dynamic scene so that they stay where the
        // level author put them.
        CStr err;
        ref_ptr xmlFile = m3d::ReadXmlFile(mapFileName.c_str(), &err);
        if (!xmlFile)
        {
            M3D_LOG_ERR(CStr("Error: cannot open DynamicScene file: ") + mapFileName + err);
            Load(LOCAL_GAME, nullptr, nullptr, bContiniousMap, ObjContainer::SAVE_LEVEL);
            return;
        }

        ref_ptr sceneNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(sceneNode, "DynamicScene");
        Load(LOCAL_GAME, xmlFile, sceneNode, bContiniousMap, ObjContainer::SAVE_LEVEL);

        for (auto* obj : *theObjects)
        {
            if (IS_KIND_OF(obj, Barricade))
            {
                obj->Remove();
            }
        }

        retruxx::vector<m3d::Class*> allowedClasses;
        allowedClasses.push_back(RT_CLASS_LOCAL(Barricade));
        m_pDynamicScene->LoadSceneFromFile(m_level->GetFullPathNameA(m_level->m_dsSrvName).c_str(), allowedClasses);
    }

    void CServer::Register()
    {
        M3D_KERNEL->AddClass(&Obj::m_classObj);
        M3D_KERNEL->AddClass(&Trigger::m_classTrigger);
        M3D_KERNEL->AddClass(&ObjContainer::m_classObjContainer);
        M3D_KERNEL->AddClass(&TimeStatistic::m_classTimeStatistic);
        M3D_KERNEL->AddClass(&IntStatistic::m_classIntStatistic);
        M3D_KERNEL->AddClass(&FloatStatistic::m_classFloatStatistic);
        M3D_KERNEL->AddClass(&FavoriteStringStatistic::m_classFavoriteStringStatistic);
        M3D_KERNEL->AddClass(&IntIntRatioStatistic::m_classIntIntRatioStatistic);
        M3D_KERNEL->AddClass(&Player::m_classPlayer);
        M3D_KERNEL->AddClass(&PhysicObj::m_classPhysicObj);
        M3D_KERNEL->AddClass(&SimplePhysicObj::m_classSimplePhysicObj);
        M3D_KERNEL->AddClass(&BreakableObject::m_classBreakableObject);
        M3D_KERNEL->AddClass(&DummyObject::m_classDummyObject);
        M3D_KERNEL->AddClass(&VehicleSplinter::m_classVehicleSplinter);
        M3D_KERNEL->AddClass(&ParticleSplinter::m_classParticleSplinter);
        M3D_KERNEL->AddClass(&ComplexPhysicObj::m_classComplexPhysicObj);
        M3D_KERNEL->AddClass(&ComplexPhysicObjPartDescription::m_classComplexPhysicObjPartDescription);
        M3D_KERNEL->AddClass(&AnimatedComplexPhysicObj::m_classAnimatedComplexPhysicObj);
        M3D_KERNEL->AddClass(&PhysicBody::m_classPhysicBody);
        M3D_KERNEL->AddClass(&SimplePhysicBody::m_classSimplePhysicBody);
        M3D_KERNEL->AddClass(&SphericBody::m_classSphericBody);
        M3D_KERNEL->AddClass(&BoxyBody::m_classBoxyBody);
        M3D_KERNEL->AddClass(&VehiclePart::m_classVehiclePart);
        M3D_KERNEL->AddClass(&CompoundVehiclePart::m_classCompoundVehiclePart);
        M3D_KERNEL->AddClass(&Wheel::m_classWheel);
        M3D_KERNEL->AddClass(&Vehicle::m_classVehicle);
        M3D_KERNEL->AddClass(&ArticulatedVehicle::m_classArticulatedVehicle);
        M3D_KERNEL->AddClass(&Gun::m_classGun);
        M3D_KERNEL->AddClass(&Shell::m_classShell);
        M3D_KERNEL->AddClass(&CompoundGun::m_classCompoundGun);
        M3D_KERNEL->AddClass(&Bullet::m_classBullet);
        M3D_KERNEL->AddClass(&BulletLauncher::m_classBulletLauncher);
        M3D_KERNEL->AddClass(&Rocket::m_classRocket);
        M3D_KERNEL->AddClass(&RocketLauncher::m_classRocketLauncher);
        M3D_KERNEL->AddClass(&RocketVolleyLauncher::m_classRocketVolleyLauncher);
        M3D_KERNEL->AddClass(&PlasmaBunch::m_classPlasmaBunch);
        M3D_KERNEL->AddClass(&PlasmaBunchLauncher::m_classPlasmaBunchLauncher);
        M3D_KERNEL->AddClass(&MortarShell::m_classMortarShell);
        M3D_KERNEL->AddClass(&Mortar::m_classMortar);
        M3D_KERNEL->AddClass(&MortarVolleyLauncher::m_classMortarVolleyLauncher);
        M3D_KERNEL->AddClass(&ThunderboltLauncher::m_classThunderboltLauncher);
        M3D_KERNEL->AddClass(&Mine::m_classMine);
        M3D_KERNEL->AddClass(&MinePusher::m_classMinePusher);
        M3D_KERNEL->AddClass(&LocationPusher::m_classLocationPusher);
        M3D_KERNEL->AddClass(&EngineOilLocation::m_classEngineOilLocation);
        M3D_KERNEL->AddClass(&TurboAccelerationPusher::m_classTurboAccelerationPusher);
        M3D_KERNEL->AddClass(&ObjPrefab::m_classObjPrefab);
        M3D_KERNEL->AddClass(&Barricade::m_classBarricade);
        M3D_KERNEL->AddClass(&Chassis::m_classChassis);
        M3D_KERNEL->AddClass(&Cabin::m_classCabin);
        M3D_KERNEL->AddClass(&Basket::m_classBasket);
        M3D_KERNEL->AddClass(&InfectionZone::m_classInfectionZone);
        M3D_KERNEL->AddClass(&InfectionTeam::m_classInfectionTeam);
        M3D_KERNEL->AddClass(&InfectionLair::m_classInfectionLair);
        M3D_KERNEL->AddClass(&Location::m_classLocation);
        M3D_KERNEL->AddClass(&StaticAutoGun::m_classStaticAutoGun);
        M3D_KERNEL->AddClass(&Settlement::m_classSettlement);
        M3D_KERNEL->AddClass(&Town::m_classTown);
        M3D_KERNEL->AddClass(&Building::m_classBuilding);
        M3D_KERNEL->AddClass(&Workshop::m_classWorkshop);
        M3D_KERNEL->AddClass(&Bar::m_classBar);
        M3D_KERNEL->AddClass(&Npc::m_classNpc);
        M3D_KERNEL->AddClass(&Lair::m_classLair);
        M3D_KERNEL->AddClass(&Team::m_classTeam);
        M3D_KERNEL->AddClass(&CaravanTeam::m_classCaravanTeam);
        M3D_KERNEL->AddClass(&VagabondTeam::m_classVagabondTeam);
        M3D_KERNEL->AddClass(&SgNodeObj::m_classSgNodeObj);
        M3D_KERNEL->AddClass(&LightObj::m_classLightObj);
        M3D_KERNEL->AddClass(&DynamicQuest::m_classDynamicQuest);
        M3D_KERNEL->AddClass(&DynamicQuestDestroy::m_classDynamicQuestDestroy);
        M3D_KERNEL->AddClass(&DynamicQuestReach::m_classDynamicQuestReach);
        M3D_KERNEL->AddClass(&DynamicQuestConvoy::m_classDynamicQuestConvoy);
        M3D_KERNEL->AddClass(&DynamicQuestPeace::m_classDynamicQuestPeace);
        M3D_KERNEL->AddClass(&DynamicQuestHunt::m_classDynamicQuestHunt);
        M3D_KERNEL->AddClass(&DynamicScene::m_classDynamicScene);
        M3D_KERNEL->AddClass(&AIManager::m_classAIManager);
        M3D_KERNEL->AddClass(&DecisionMatrix::m_classDecisionMatrix);
        M3D_KERNEL->AddClass(&QuestStateManager::m_classQuestStateManager);
        M3D_KERNEL->AddClass(&GeomObj::m_classGeomObj);
        M3D_KERNEL->AddClass(&JointedObj::m_classJointedObj);
        M3D_KERNEL->AddClass(&CompositeObj::m_classCompositeObj);
        M3D_KERNEL->AddClass(&PhysicUnit::m_classPhysicUnit);
        M3D_KERNEL->AddClass(&GeomRepository::m_classGeomRepository);
        M3D_KERNEL->AddClass(&IzvratRepository::m_classIzvratRepository);
        M3D_KERNEL->AddClass(&Ware::m_classWare);
        M3D_KERNEL->AddClass(&BlastWave::m_classBlastWave);
        M3D_KERNEL->AddClass(&BossMetalArm::m_classBossMetalArm);
        M3D_KERNEL->AddClass(&BossMetalArmLoad::m_classBossMetalArmLoad);
        M3D_KERNEL->AddClass(&Boss02::m_classBoss02);
        M3D_KERNEL->AddClass(&Boss02Arm::m_classBoss02Arm);
        M3D_KERNEL->AddClass(&Boss03::m_classBoss03);
        M3D_KERNEL->AddClass(&Boss03Part::m_classBoss03Part);
        M3D_KERNEL->AddClass(&Boss04::m_classBoss04);
        M3D_KERNEL->AddClass(&Boss04Part::m_classBoss04Part);
        M3D_KERNEL->AddClass(&Boss04Station::m_classBoss04Station);
        M3D_KERNEL->AddClass(&Boss04StationPart::m_classBoss04StationPart);
        M3D_KERNEL->AddClass(&Boss04Drone::m_classBoss04Drone);
        M3D_KERNEL->AddClass(&Submarine::m_classSubmarine);
        M3D_KERNEL->AddClass(&VehicleRecollection::m_classVehicleRecollection);
        M3D_KERNEL->AddClass(&TeamTacticWithRoles::m_classTeamTacticWithRoles);
        M3D_KERNEL->AddClass(&VehicleRoleMeat::m_classVehicleRoleMeat);
        M3D_KERNEL->AddClass(&VehicleRoleSniper::m_classVehicleRoleSniper);
        M3D_KERNEL->AddClass(&VehicleRoleOppressor::m_classVehicleRoleOppressor);
        M3D_KERNEL->AddClass(&VehicleRoleBarrier::m_classVehicleRoleBarrier);
        M3D_KERNEL->AddClass(&VehicleRolePendulum::m_classVehicleRolePendulum);
        M3D_KERNEL->AddClass(&VehicleRoleCoward::m_classVehicleRoleCoward);
        M3D_KERNEL->AddClass(&VehicleRoleCheater::m_classVehicleRoleCheater);
        M3D_KERNEL->AddClass(&NPCMotionController::m_classNPCMotionController);
        M3D_KERNEL->AddClass(&CinematicMover::m_classCinematicMover);
        M3D_KERNEL->AddClass(&Static2DPath::m_classStatic2DPath);
        M3D_KERNEL->AddClass(&Gadget::m_classGadget);
        M3D_KERNEL->AddClass(&Formation::m_classFormation);
        IntersectionManager::Registration();
    }

    float CServer::GetLevelSize() const
    {
        return this->m_level->land_size * 128.0;
    }

    m3d::Level* CServer::GetLevel() const
    {
        return m_level;
    }

    m3d::Profiler* CServer::GetPathFindingProfiler()
    {
        return m_pathFindingProfiler;
    }

    void CServer::RelinkSceneGraphNodes()
    {
        m_pObjects->RelinkSceneGraphNodes();
    }

    CServer::CServer()
    {
        assert(M3D_KERNEL);

        this->pGlobalMap = 0;
        this->m_pWorld = 0;
        this->m_level = 0;
        this->m_pDynamicScene = 0;
        this->pAIManager = 0;
        this->m_pAffixManager = 0;
        this->m_pExternalPaths = 0;
        this->m_pPlayerPassMap = 0;
        this->cam = CVector(0.0, 0.0, 0.0);
        this->fPause = 0;
        this->m_lastId = 0;
        this->m_averageElapsedTime = 0.0;
        this->m_CurIndex = 0;
        this->m_MaxAverageLength = 20;
        this->m_Accumulation = 1;
        this->m_AveElapsedTimeUsed = 0;

        m_lastElapsedTimes.resize(0x14, 0.0);
        m_pObjects = dynamic_cast<ObjContainer*>(M3D_KERNEL->New("ObjContainer"));
        M3D_KERNEL->UnRegisterGlobal("g_ObjContainer");
        M3D_KERNEL->RegisterGlobal(m_pObjects, "g_ObjContainer");
        SetObjects(m_pObjects);

        auto idx = M3D_APP->GetProfilerStack().AddProfiler("AI physic step", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_profilerTmpForServer = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }

        idx = M3D_APP->GetProfilerStack().AddProfiler("AI collisions", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_collideProfiler = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }

        idx = M3D_APP->GetProfilerStack().AddProfiler("AI bullets update", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_bulletProfiler = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }

        idx = M3D_APP->GetProfilerStack().AddProfiler("AI blast waves update", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_blastWaveProfiler = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }

        idx = M3D_APP->GetProfilerStack().AddProfiler("AI vehicles update", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_pathFindingProfiler = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }

        idx = M3D_APP->GetProfilerStack().AddProfiler("AI total objects update", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_objectsUpdateProfiler = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }

        idx = M3D_APP->GetProfilerStack().AddProfiler("AI full update", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_serverUpdateProfiler = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }

        M3D_ENGINE_CFG.m_console->RegisterCommand("ai_switch_player_physics", 0, this);
        LoadGlobalPropertiesFromXML(M3D_ENGINE_CFG.m_pathToGlobProps.GetS());
    }

    void CServer::LoadTriggersFromXML(CStr const& fileName)
    {
        // RVA 0x5F2D90
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error: cannot open " + fileName);
            return;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (xmlFile->Read(*stream))
        {
            stream->Close();

            ref_ptr triggersNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(triggersNode, "triggers");

            ref_ptr triggerNode = xmlFile->CreateNode();
            for (triggersNode->GetFirstChild(triggerNode, "trigger"); !triggerNode->IsEmpty();
                 triggerNode->GetNextSibling(triggerNode, "trigger"))
            {
                CStr name;
                m3d::SafeStrAttrib(name, triggerNode, "Name");
                if (name.empty())
                {
                    M3D_LOG_ERR("Error: empty trigger name");
                    continue;
                }

                auto* entity = ai::theObjects->GetEntityByObjName(name);
                if (ai::theObjects->m_SaveType == ai::ObjContainer::eSAVE_TYPES::SAVE_LEVEL && entity)
                {
                    M3D_LOG_ERR(
                        "Attempting to load trigger " + name + ", but a trigger with this name already exists!");
                }
                if (!entity)
                {
                    auto id = ai::thePrototypeManager->GetPrototypeId("trigger");
                    auto objId = ai::theObjects->CreateNewObject(id, name.c_str(), -1, -1);
                    entity = ai::theObjects->GetEntityByObjId(objId);
                }

                auto* trigger = dynamic_cast<ai::Trigger*>(entity);
                trigger->LoadFromMapXML(xmlFile, triggerNode);
                trigger->ActivateIfNeeded();
            }
        }
        else
        {
            M3D_LOG_ERR("Error: cannot parse " + fileName);
        }
    }

    void CServer::PostPlayerEvent(eGameEvent eventId)
    {
        if (thePlayer)
        {
            thePlayer->CauseEvent(eventId, 0.0, {}, {});
        }
    }

    int CServer::GetLastId()
    {
        return m_lastId;
    }

    CStr CServer::GetPrototypeFullName(int prototypeId)
    {
        // RVA 0x5EFAA0
        return thePrototypeManager->GetPrototypeFullName(prototypeId);
    }

    CStr CServer::GetPrototypeFullName(CStr const& modelName)
    {
        // RVA 0x5EFA80
        return thePrototypeManager->GetPrototypeFullName(modelName);
    }

    void CServer::Update(float elapsedTime)
    {
        // RVA 0x5F4090 - one server tick: physics step, AI and objects, then collision (whose
        // contacts and forces feed the next tick's step).
        if (!m_StartServerUpdates)
        {
            return;
        }

        // Start profiling server update
        m_serverUpdateProfiler->StartCountdown();

        // Calculate current time
        float CurTime = 0.0;
        if (M3D_KERNEL->GetTimer().GetTimeScale() == 0.0f)
        {
            CurTime = m_LastUpdateTime;
        }
        else
        {
            // The milliseconds are scaled in double before narrowing, so long sessions keep
            // millisecond resolution.
            CurTime = static_cast<float>(static_cast<double>(M3D_KERNEL->GetTimer().GetCurTime()) * 0.001);
        }

        // Handle very small elapsed times
        if (elapsedTime < 0.001f)
        {
            elapsedTime = CurTime - m_LastUpdateTime;
            if (elapsedTime < 0.0f)
            {
                elapsedTime = 0.0f;
            }
        }

        // The step is capped at ai_min_frame_time (despite the name, a maximum).
        // The server also keeps a running average of the last m_MaxAverageLength steps in
        // m_averageElapsedTime, which replaces the step when m_AveElapsedTimeUsed is set.
        float const maxFrameTime = M3D_ENGINE_CFG.m_ai_min_frame_time.GetF();
        float step = elapsedTime;
        if (elapsedTime > maxFrameTime)
        {
            elapsedTime = maxFrameTime;
            step = maxFrameTime;
        }

        // While the history is still filling up the average is taken over what there is.
        if (m_Accumulation)
        {
            int const index = m_CurIndex;
            if (index >= m_MaxAverageLength)
            {
                m_Accumulation = false;
            }
            else
            {
                m_lastElapsedTimes[index] = step;
                m_averageElapsedTime = 0.0f;
                for (int i = 0; i <= m_CurIndex; ++i)
                {
                    m_averageElapsedTime = m_lastElapsedTimes[i] + m_averageElapsedTime;
                }
                m_averageElapsedTime = m_averageElapsedTime / static_cast<float>(index + 1);
                m_CurIndex = index + 1;
            }
        }
        // Then it is updated as a moving average over a ring of samples.
        if (!m_Accumulation)
        {
            if (m_CurIndex >= m_MaxAverageLength)
            {
                m_CurIndex = 0;
            }
            float& oldest = m_lastElapsedTimes[m_CurIndex];
            m_averageElapsedTime = (step - oldest) / static_cast<float>(m_MaxAverageLength) + m_averageElapsedTime;
            oldest = step;
            ++m_CurIndex;
        }

        if (m_AveElapsedTimeUsed)
        {
            step = m_averageElapsedTime;
            elapsedTime = m_averageElapsedTime;
        }

        // Determine work time based on elapsed time
        unsigned int workTime = 2;
        if (step <= 0.0001f)
        {
            workTime = 0;
        }

        // Handle pause
        if (fPause)
        {
            elapsedTime = 0.0f;
        }

        float v14 = elapsedTime;  // Store original elapsed time

        // AI simulation steps
        ai::IntersectionManager::ClearCounters();
        ai::gDynamicScene->StepScene(elapsedTime);
        _PostProcessConsoleCommands();

        ai::Path::QuantAmount = 0;
        ai::theAIManager->m_elapsedTime = elapsedTime;
        ai::theAIManager->m_workTime = workTime;

        // Determine if in cinematic mode
        bool InCinematic = false;
        if (m_InCinematic)
        {
            bool useCinematic = M3D_ENGINE_CFG.m_UseCinematicUpdate.GetB();

            if (useCinematic)
            {
                InCinematic = true;
            }
        }

        // Update process manager with minimum time threshold
        float processTime = (elapsedTime <= 0.0001f) ? elapsedTime : 0.0001f;
        ai::theProcessManager->Update(processTime, 0, workTime);

        // Object container updates with profiling
        m_pObjects->PostCollide();
        ai::gDynamicScene->PurgeBodies();

        m_objectsUpdateProfiler->StartCountdown();
        m_pObjects->Update(v14, workTime, InCinematic);
        m_objectsUpdateProfiler->EndCountdown();

        // Post-update processing
        m_pObjects->PostCollide();
        ai::theProcessManager->Update(v14, 1, workTime);
        m_pObjects->PostCollide();
        ai::gDynamicScene->PurgeBodies();

        // Collision detection
        ai::gDynamicScene->CollideScene(v14);

        // Final process manager update
        processTime = (elapsedTime <= 0.0001f) ? elapsedTime : 0.0001f;
        ai::theProcessManager->Update(processTime, 0, workTime);

        m_pObjects->PostCollide();
        ai::gDynamicScene->PurgeBodies();

        // Update timing and handle step mode
        m_LastUpdateTime = CurTime;
        if (M3D_ENGINE_CFG.m_StepMode.GetI() == 1)
        {
            fPause = true;
        }

        // Final profiling update
        m_serverUpdateProfiler->EndCountdown();
    }

    void CServer::LoadGlobalMapFromRawFile(char const* fileName)
    {
        // RVA 0x5F0370
        pGlobalMap->LoadFromRawFile(fileName);
    }

    void CServer::ClearOnce()
    {
        m3d::TraceLineManager::InitTraceLineRay(false);
        if (m_pDynamicScene)
        {
            m_pDynamicScene->DecRef();
            m_pDynamicScene = nullptr;
            ai::SetDynamicScene(m_pDynamicScene);
        }

        if (pAIManager)
        {
            pAIManager->DecRef();
            pAIManager = nullptr;
            ai::SetAIManager(pAIManager);
        }

        DynamicScene::ClearOnce();

        delete ai::thePrototypeManager;
        thePrototypeManager = nullptr;

        delete ai::theQuestStateManager;
        theQuestStateManager = nullptr;

        delete m_pAffixManager;
        m_pAffixManager = nullptr;

        delete ai::theResourceManager;
        theResourceManager = nullptr;

        delete ai::theStatisticManager;
        theStatisticManager = nullptr;

        delete pAIManager;
        pAIManager = nullptr;

        delete theRelationship;
        theRelationship = nullptr;

        delete theVehiclesGeneratorInfoCache;
        theVehiclesGeneratorInfoCache = nullptr;
    }

    void CServer::EndCinematic()
    {
        m_InCinematic = 0;
    }

    void CServer::GetControlData()
    {
        // RVA 0x5F0330 - a leftover from the networked build; the shipped game does nothing here.
    }

    unsigned CServer::GetGlobalMapValue(long xIndex, long yIndex)
    {
        // RVA 0x5F0350
        return pGlobalMap->GetValue(xIndex, yIndex);
    }

    int CServer::GetPathFindQuant()
    {
        // RVA 0x5EFAC0
        return Path::QuantAmount;
    }

    CStr CServer::GetFullNameByObjID(int objId)
    {
        auto* obj = theObjects->GetEntityByObjId(objId);
        if (obj)
        {
            auto fullName = theObjects->GetObjectFullName(obj->GetName());
            if (!fullName.empty())
            {
                return fullName;
            }
            return thePrototypeManager->GetPrototypeFullName(obj->GetPrototypeInfo()->m_prototypeName);
        }
        return "Model Unknown";
    }

    void CServer::Clear()
    {
        ai::DynamicScene::Clear();
        m_pObjects->Clear(0);
        ai::thePlayer = 0;

        delete m_pExternalPaths;
        m_pExternalPaths = 0;

        delete m_pPlayerPassMap;
        m_pPlayerPassMap = 0;

        delete theProcessManager;
        theProcessManager = 0;

        delete pGlobalMap;
        pGlobalMap = 0;
        m_level = 0;
    }

    void CServer::ResetCinematicObjects()
    {
        // RVA 0x5F3CC0 - clears the cinematic mark from every object in the world.
        for (auto* obj : *theObjects)
        {
            obj->m_flags &= ~0x10u;
        }
    }

    PrototypeInfo const* CServer::GetPrototypeInfo(int prototypeId)
    {
        // RVA 0x5F2D60
        return thePrototypeManager->GetPrototypeInfo(prototypeId);
    }

    void CServer::InitOnce()
    {
        // RVA 0x5F2160
        LoadGlobalPropertiesFromXML(M3D_ENGINE_CFG.m_pathToGlobProps.GetS());
        theResourceManager = new ResourceManager;
        theResourceManager->Init();
        m_pAffixManager = new AffixManager;
        m_pAffixManager->LoadFromXmlFile(ai::theGlobProp.m_pathToAffixes.c_str());
        DynamicScene::InitOnce();
        m_pDynamicScene = dynamic_cast<DynamicScene*>(M3D_KERNEL->New("DynamicScene"));
        m_pDynamicScene->IncRef();
        SetDynamicScene(m_pDynamicScene);
        m3d::TraceLineManager::InitTraceLineRay(true);
        m_pDynamicScene->InitClashDecalId();
        theQuestManager = new QuestManager;
        theQuestManager->LoadFromXmlFile(ai::theGlobProp.m_pathToQuests.c_str());
        theQuestStateManager = dynamic_cast<QuestStateManager*>(M3D_KERNEL->New("QuestStateManager"));
        M3D_KERNEL->UnRegisterGlobal("g_QuestStateManager");
        M3D_KERNEL->RegisterGlobal(theQuestStateManager, "g_QuestStateManager");
        theStatisticManager = new StatisticManager;
        pAIManager = dynamic_cast<AIManager*>(M3D_KERNEL->New("AIManager"));
        pAIManager->IncRef();
        SetAIManager(pAIManager);
        M3D_KERNEL->UnRegisterGlobal("g_AIManager");
        M3D_KERNEL->RegisterGlobal(pAIManager, "g_AIManager");
        thePrototypeManager = new PrototypeManager;
        theRelationship = new Relationship;
        theVehiclesGeneratorInfoCache = new VehiclesGeneratorInfoCache;
    }

    eTolerance CServer::CheckTolerance(int PlayerID1, int PlayerID2)
    {
        return ai::theRelationship->CheckTolerance(PlayerID1, PlayerID2);
    }

    void CServer::LoadPrototypeNamesFromXML(CStr const& fileName)
    {
        // RVA 0x5F5430 - maps each prototype name to the display name the UI shows, plus which
        // grammatical form that display name takes.
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error: cannot open " + fileName);
            return;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (!xmlFile->Read(*stream))
        {
            M3D_LOG_ERR("Error: cannot parse " + fileName + CStr(" ( ") + CStr(xmlFile->GetError()) + CStr(" ) "));
            return;
        }
        stream->Close();

        ref_ptr rootNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(rootNode, "ModelNames");
        if (rootNode->IsEmpty())
        {
            // NOTE: the message names <models> although the tag actually looked for is
            // <ModelNames>.
            M3D_LOG_ERR(CStr("Error: Tag <models> not found in file: ") + fileName);
            return;
        }

        ref_ptr itemNode = xmlFile->CreateNode();
        for (rootNode->GetFirstChild(itemNode, "Item"); !itemNode->IsEmpty();
             itemNode->GetNextSibling(itemNode, "Item"))
        {
            CStr const name(itemNode->GetAttribute("id"));
            CStr const fullName(itemNode->GetAttribute("value"));
            int locForm = 0;
            m3d::SafeIntAttrib(locForm, itemNode, "locForm");
            if (locForm < 0)
            {
                locForm = 0;
            }
            thePrototypeManager->m_prototypeFullNames.add(name, fullName);
            thePrototypeManager->m_prototypeFullNamesLocalizedForms.add(name, locForm);
        }
    }

    void CServer::LoadGlobalPropertiesFromXML(CStr const& fileName)
    {
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error: cannot open " + fileName);
            return;
        }
        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (xmlFile->Read(*stream))
        {
            stream->Close();
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            xmlFile->GetFirstChild(node, "Properties");
            theGlobProp.LoadFromXML(xmlFile, node);
            return;
        }
        M3D_LOG_ERR("Error: cannot parse " + fileName);
    }

    AffixManager* CServer::GetAffixManager() const
    {
        // RVA 0x4D98B0
        return m_pAffixManager;
    }

    void CServer::StartCinematic()
    {
        this->m_InCinematic = 1;
    }

    void CServer::Init(m3d::CWorld* world)
    {
        m_InCinematic = false;
        m_LastUpdateTime = M3D_KERNEL->GetTimer().GetCurTime() * 0.001f;
        m_pWorld = world;
        _SetLevel(world->m_level);
        m_pObjects->AllowSave(true);
        thePlayer = nullptr;
        m_pExternalPaths = new ExternalPaths;
        m_pPlayerPassMap = new PlayerPassMap;
        M3D_ENGINE_CFG.m_console->executeCommand("/conScript data\\scripts\\server.lua");
        theProcessManager = new ProcessManager;
        theAIManager->RegisterMatrix("void", nullptr);
        ai::Obj::Registration();
        ai::Trigger::Registration();
        ai::DynamicQuest::Registration();
        ai::PhysicObj::Registration();
        ai::SimplePhysicObj::Registration();
        ai::PhysicBody::Registration();
        ai::Player::Registration();
        ai::PhysicUnit::Registration();
        ai::Vehicle::Registration();
        ai::Chassis::Registration();
        ai::Team::Registration();
        ai::Npc::Registration();
        ai::Location::Registration();
        ai::SgNodeObj::Registration();
        ai::LightObj::Registration();
        ai::Gadget::Registration();
        ai::VehiclePart::Registration();
        ai::Cabin::Registration();
        ai::VehiclePart::Registration();
        ai::Gun::Registration();
        ai::BulletLauncher::Registration();
        ai::VehiclePart::Registration();
        ai::Ware::Registration();
        ai::SimplePhysicObj::Registration();
        ai::Barricade::Registration();
        ai::Settlement::Registration();
        ai::Town::Registration();
        ai::Lair::Registration();
        ai::InfectionZone::Registration();
        ai::Team::Registration();
        ai::Settlement::Registration();
        ai::SimplePhysicObj::Registration();
        ai::DummyObject::Registration();
        ai::Gun::Registration();
        ai::Obj::Registration();
        ai::Boss03::Registration();
        ai::Boss04::Registration();
        ai::VehiclePart::Registration();
        ai::Boss04Station::Registration();
        ai::Boss04StationPart::Registration();
        ai::Boss04Drone::Registration();
        ai::Submarine::Registration();
        ai::Obj::Registration();

        auto& scriptServer = M3D_KERNEL->GetScriptServer();
        scriptServer.registerGlobalFunction(
            n_AddToCinematic,
            "AddToCinematic",
            "void",
            "Object, [bool WithChildren]",
            "adds object to update list for cinematic state.");
        scriptServer.registerGlobalFunction(
            n_EndCinematic, "EndCinematic", "void", "void", "Revert Server to normal update mode");
        scriptServer.registerGlobalFunction(
            n_CreateObjectByClassName, "CreateObjectByClassName", "Object*", "const char* className", "Creates Object");

        pGlobalMap = new Map;
        pGlobalMap->Create(
            m_level->m_passMapCellSize,
            m_level->m_passMapCellSize,
            pServer->m_level->land_size * 128.0f,
            pServer->m_level->land_size * 128.0f,
            nullptr);
        pGlobalMap->Clear();
        Map::SetGlobalMap(pGlobalMap);
        pGlobalMap->LoadFromRawFile(m_level->GetFullPathNameA(m_level->m_passMapName).c_str());
    }

    void CServer::PutGameData()
    {
        // RVA 0x5F0340 - a leftover from the networked build; the shipped game does nothing here.
    }

    void CServer::HandleCommand(int cmdID, m3d::CConsoleParams const&)
    {
        // RVA 0x5F4070 - console commands are queued and acted on at the top of the next update,
        // so that they never run in the middle of a physics step.
        m_consoleCommandsToPostProcess.push(cmdID);
    }

    CServer::~CServer()
    {
        // RVA 0x5F3AA0 - the object container is reference counted; everything else the server
        // owns outlives it or is freed elsewhere.
        if (m_pObjects)
        {
            m_pObjects->DecRef();
        }
        m_pObjects = nullptr;
        SetObjects(nullptr);
    }

    bool CServer::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
    {
        // RVA 0x5EFA50 - the server registers no cvars of its own.
        return false;
    }

    void CServer::SaveVisitedMap(CStr const& mapFileName)
    {
        // RVA 0x5F3B40 - everything transient is thrown away before the map is written, so that
        // a revisited map does not come back littered with debris and shells in flight.
        theObjects->m_SaveType = ObjContainer::SAVE_LEVEL;
        for (auto* obj : *theObjects)
        {
            if (IS_KIND_OF(obj, ParticleSplinter) || IS_KIND_OF(obj, VehicleSplinter) ||
                IS_KIND_OF(obj, CompositeObj) || IS_KIND_OF(obj, JointedObj) || IS_KIND_OF(obj, Shell) ||
                IS_KIND_OF(obj, Thunderbolt) || IS_KIND_OF(obj, EngineOilLocation) ||
                IS_KIND_OF(obj, SmokeScreenLocation) || IS_KIND_OF(obj, InfectionTeam))
            {
                obj->Remove();
            }
            else if (IS_KIND_OF(obj, Workshop))
            {
                static_cast<Workshop*>(obj)->OnSaveVisitedMap();
            }
        }
        m_pDynamicScene->PurgeBodies();
        m_pDynamicScene->SaveSceneToFile(mapFileName.c_str());
    }

    void CServer::_PostProcessConsoleCommands()
    {
        // RVA 0x5F3CF0 - command 0 is ai_switch_player_physics, which toggles whether the
        // player's own truck is simulated by ODE.
        while (!m_consoleCommandsToPostProcess.empty())
        {
            auto const command = m_consoleCommandsToPostProcess.front();
            if (command == 0)
            {
                Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
                if (playerVehicle)
                {
                    playerVehicle->SetUpdatingByODE(!playerVehicle->bIsUpdatingByODE());
                }
            }
            m_consoleCommandsToPostProcess.pop();
        }
    }

    void CServer::_RegisterConsoleCommands()
    {
        // RVA 0x5F0380 - NOTE: the shipped build hands a table of commands and a count to an
        // IConsole::RegisterCommand overload this port does not expose, and that count is zero,
        // so nothing is registered here. The one command the server actually handles,
        // ai_switch_player_physics, is registered by name in InitOnce.
    }

    void CServer::RestorePrevCinematicState()
    {
        // RVA 0x5F1340 - the shipped build saves and restores nothing.
    }

    void CServer::SavePrevCinematicState()
    {
        // RVA 0x5F1330 - the shipped build saves and restores nothing.
    }

    void CServer::_SetLevel(m3d::Level* newLevel)
    {
        if (m_level)
        {
            M3D_LOG_ERR("Error: attempt to assign level info twice");
        }
        else
        {
            m_level = newLevel;
        }
    }

    void SetDynamicScene(DynamicScene* DS)
    {
        gDynamicScene = DS;
    }

    void UpdateLights()
    {
        // RVA 0x82EB40 - weather that hides the shadows hides the lights that cast them, so each
        // light object drops or recreates its scene graph node as the sky changes.
        bool const shadowsHidden = m3d::pClient->GetWorld().GetWeatherManager().GetShadowVisibilityFromWeather();
        for (auto* obj : *theObjects)
        {
            if (IS_KIND_OF(obj, LightObj))
            {
                auto* const lightObj = static_cast<SgNodeObj*>(obj);
                if (shadowsHidden)
                {
                    lightObj->DeleteSgNode();
                }
                else
                {
                    lightObj->SetSgNode();
                }
            }
        }
    }
}  // namespace ai

#include "server.h"
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
        throw retruxx::logic_error("Not implemented");
    }

    int n_EndCinematic(m3d::sArgStack& scriptStack)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int n_CreateObjectByClassName(m3d::sArgStack& scriptStack)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::AddToCinematic(Obj*, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::AddToCinematic(int, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::SetPause(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetTmpProfiler()
    {
        return m_profilerTmpForServer;
    }

    m3d::Profiler* CServer::GetBulletProfiler()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetCollideProfiler()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetObjectsUpdateProfiler()
    {
        throw retruxx::logic_error("Not implemented");
    }

    ExternalPaths const* CServer::GetExternalPaths() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    PlayerPassMap const* CServer::GetPlayerPassMap() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::SetLastId(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::Load(StartupMode mode, m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode, bool bContiniousMap, ObjContainer::eSAVE_TYPES saveType)
    {
        // TODO: implement CServer::Load
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
        m_pDynamicScene->ReadSoilProps(M3D_KERNEL->GetEngineCfg().m_pathToSoilProps.GetS());

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
       // ai::theObjects->LoadObjectNamesFromXML(m_level->GetFullPathNameA(this->m_level->m_ObjectFullNames));
 
       // LoadPrototypeNamesFromXML(this->m_level->m_prototypeFullNames);
        m_LastSenderID = 0;
        
        M3D_LOG_INFO("\tAI: Initing Player");  
        m3d::sArgStack stack;
        if (auto res = M3D_KERNEL->GetScriptServer().callScriptFunc("InitPlayer", stack, 0))
        {
            auto formatedScriptErrorDesc = M3D_KERNEL->GetScriptServer().getFormatedScriptErrorDesc(res);
            M3D_LOG_ERR(formatedScriptErrorDesc);

        }

        ai::theProcessManager->Update(0.0001, 1u, 2u);
        if (ai::thePlayer && ai::theObjects->m_SaveType != ai::ObjContainer::eSAVE_TYPES::SAVE_FULL)
        {
            ai::thePlayer->CauseEvent(GE_GAME_START, 0.0, {}, {});
        }

        M3D_LOG_INFO("\t\tBefore first Update");
        ai::theProcessManager->Update(0.050000001, 1u, 2u);
        if (ai::theObjects->m_SaveType == ai::ObjContainer::eSAVE_TYPES::SAVE_LEVEL)
        {
            Update(0.0099999998);
            Update(0.0099999998);
            Update(0.0099999998);
        }
        M3D_LOG_INFO("\t\tAfter first Update");

        M3D_LOG_INFO("AI: Finished Loading Server");

        ai::theObjects->m_SaveType = saveSaveType;
    }

    bool CServer::GetPause() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CServer::GetPrototypeId(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    PrototypeInfo* CServer::CreatePrototypeInfoByClassName(CStr const& className)
    {
        // TODO: implement CServer::CreatePrototypeInfoByClassName
        if (className != "Trigger")
        {
            return nullptr;
        }


        if (className == "Vehicle") return new VehiclePrototypeInfo;
        else if (className == "ArticulatedVehicle") return new ArticulatedVehiclePrototypeInfo;
        else if (className == "VehiclePart") return new VehiclePartPrototypeInfo;
        else if (className == "CompoundVehiclePart") return new CompoundVehiclePartPrototypeInfo;
        else if (className == "Chassis") return new ChassisPrototypeInfo;
        else if (className == "Cabin") return new CabinPrototypeInfo;
        else if (className == "Basket") return new BasketPrototypeInfo;
        else if (className == "Trigger") return new TriggerPrototypeInfo;
        else if (className == "DynamicQuestDestroy") return new DynamicQuestDestroyPrototypeInfo;
        else if (className == "DynamicQuestReach") return new DynamicQuestReachPrototypeInfo;
        else if (className == "DynamicQuestConvoy") return new DynamicQuestConvoyPrototypeInfo;
        else if (className == "DynamicQuestPeace") return new DynamicQuestPeacePrototypeInfo;
        else if (className == "DynamicQuestHunt") return new DynamicQuestHuntPrototypeInfo;
        else if (className == "Wheel") return new WheelPrototypeInfo;
        else if (className == "StaticAutoGun") return new StaticAutoGunPrototypeInfo;
        else if (className == "Town") return new TownPrototypeInfo;
        else if (className == "Building") return new BuildingPrototypeInfo;
        else if (className == "Workshop") return new WorkshopPrototypeInfo;
        else if (className == "Bar") return new BarPrototypeInfo;
        else if (className == "Npc") return new NpcPrototypeInfo;
        else if (className == "Lair") return new LairPrototypeInfo;
        else if (className == "Location") return new LocationPrototypeInfo;
        else if (className == "Chest") return new ChestPrototypeInfo;
        else if (className == "Team") return new TeamPrototypeInfo;
        else if (className == "CaravanTeam") return new CaravanTeamPrototypeInfo;
        else if (className == "VagabondTeam") return new VagabondTeamPrototypeInfo;
        else if (className == "BreakableObject") return new BreakableObjectPrototypeInfo;
        else if (className == "RopeObj") return new RopeObjPrototypeInfo;
        else if (className == "DummyObject") return new DummyObjectPrototypeInfo;
        else if (className == "Ware") return new WarePrototypeInfo;
        else if (className == "Gadget") return new GadgetPrototypeInfo;
        else if (className == "PhysicUnit") return new PhysicUnitPrototypeInfo;
        else if (className == "JointedObj") return new JointedObjPrototypeInfo;
        else if (className == "CompositeObj") return new CompositeObjPrototypeInfo;
        else if (className == "GeomObj") return new GeomObjPrototypeInfo;
        else if (className == "VehicleSplinter") return new VehicleSplinterPrototypeInfo;
        else if (className == "ParticleSplinter") return new ParticleSplinterPrototypeInfo;
        else if (className == "Player") return new PlayerPrototypeInfo;
        else if (className == "ObjPrefab") return new ObjPrefabPrototypeInfo;
        else if (className == "Barricade") return new BarricadePrototypeInfo;
        else if (className == "SgNodeObj") return new SgNodeObjPrototypeInfo;
        else if (className == "LightObj") return new LightObjPrototypeInfo;
        else if (className == "InfectionTeam") return new InfectionTeamPrototypeInfo;
        else if (className == "InfectionZone") return new InfectionZonePrototypeInfo;
        else if (className == "InfectionLair") return new InfectionLairPrototypeInfo;
        else if (className == "BlastWave") return new BlastWavePrototypeInfo;
        else if (className == "RepositoryObjectsGenerator") return new RepositoryObjectsGeneratorPrototypeInfo;
        else if (className == "AffixGenerator") return new AffixGeneratorPrototypeInfo;
        else if (className == "VehicleRecollection") return new VehicleRecollectionPrototypeInfo;
        else if (className == "VehiclesGenerator") return new VehiclesGeneratorPrototypeInfo;
        else if (className == "WanderersGenerator") return new WanderersGeneratorPrototypeInfo;
        else if (className == "WanderersManager") return new WanderersManagerPrototypeInfo;
        else if (className == "BossMetalArm") return new BossMetalArmPrototypeInfo;
        else if (className == "BossMetalArmLoad") return new BossMetalArmLoadPrototypeInfo;
        else if (className == "Boss02") return new Boss02PrototypeInfo;
        else if (className == "Boss02Arm") return new Boss02ArmPrototypeInfo;
        else if (className == "Boss03") return new Boss03PrototypeInfo;
        else if (className == "Boss03Part") return new Boss03PartPrototypeInfo;
        else if (className == "Boss04") return new Boss04PrototypeInfo;
        else if (className == "Boss04Part") return new Boss04PartPrototypeInfo;
        else if (className == "Boss04Station") return new Boss04StationPrototypeInfo;
        else if (className == "Boss04StationPart") return new Boss04StationPartPrototypeInfo;
        else if (className == "Boss04Drone") return new Boss04DronePrototypeInfo;
        else if (className == "Submarine") return new SubmarinePrototypeInfo;
        else if (className == "TeamTacticWithRoles") return new TeamTacticWithRolesPrototypeInfo;
        else if (className == "VehicleRoleMeat") return new VehicleRoleMeatPrototypeInfo;
        else if (className == "VehicleRoleSniper") return new VehicleRoleSniperPrototypeInfo;
        else if (className == "VehicleRoleOppressor") return new VehicleRoleOppressorPrototypeInfo;
        else if (className == "VehicleRoleBarrier") return new VehicleRoleBarrierPrototypeInfo;
        else if (className == "VehicleRoleCoward") return new VehicleRoleCowardPrototypeInfo;
        else if (className == "VehicleRolePendulum") return new VehicleRolePendulumPrototypeInfo;
        else if (className == "VehicleRoleCheater") return new VehicleRoleCheaterPrototypeInfo;
        else if (className == "NPCMotionController") return new NPCMotionControllerPrototypeInfo;
        else if (className == "CinematicMover") return new CinematicMoverPrototypeInfo;
        else if (className == "Bullet") return new BulletPrototypeInfo;
        else if (className == "Rocket") return new RocketPrototypeInfo;
        else if (className == "BulletLauncher") return new BulletLauncherPrototypeInfo;
        else if (className == "RocketLauncher") return new RocketLauncherPrototypeInfo;
        else if (className == "CompoundGun") return new CompoundGunPrototypeInfo;
        else if (className == "PlasmaBunch") return new PlasmaBunchPrototypeInfo;
        else if (className == "PlasmaBunchLauncher") return new PlasmaBunchLauncherPrototypeInfo;
        else if (className == "RocketVolleyLauncher") return new RocketVolleyLauncherPrototypeInfo;
        else if (className == "Thunderbolt") return new ThunderboltPrototypeInfo;
        else if (className == "ThunderboltLauncher") return new ThunderboltLauncherPrototypeInfo;
        else if (className == "MortarShell") return new MortarShellPrototypeInfo;
        else if (className == "Mortar") return new MortarPrototypeInfo;
        else if (className == "MortarVolleyLauncher") return new MortarVolleyLauncherPrototypeInfo;
        else if (className == "Mine") return new MinePrototypeInfo;
        else if (className == "MinePusher") return new MinePusherPrototypeInfo;
        else if (className == "LocationPusher") return new LocationPusherPrototypeInfo;
        else if (className == "EngineOilLocation") return new EngineOilLocationPrototypeInfo;
        else if (className == "NailLocation") return new NailLocationPrototypeInfo;
        else if (className == "SmokeScreenLocation") return new SmokeScreenLocationPrototypeInfo;
        else if (className == "TurboAccelerationPusher") return new TurboAccelerationPusherPrototypeInfo;
        else if (className == "RadioManager") return new RadioManagerPrototypeInfo;
        else if (className == "Formation") return new FormationPrototypeInfo;
        else if (className == "QuestItem") return new QuestItemPrototypeInfo;

        return nullptr;
    }

    m3d::CWorld* CServer::GetWorld()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::LoadVisitedMap(CStr const&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::Register()
    {
        m3d::g_Kernel->AddClass(&Obj::m_classObj);
        m3d::g_Kernel->AddClass(&Trigger::m_classTrigger);
        m3d::g_Kernel->AddClass(&ObjContainer::m_classObjContainer);
        m3d::g_Kernel->AddClass(&TimeStatistic::m_classTimeStatistic);
        m3d::g_Kernel->AddClass(&IntStatistic::m_classIntStatistic);
        m3d::g_Kernel->AddClass(&FloatStatistic::m_classFloatStatistic);
        m3d::g_Kernel->AddClass(&FavoriteStringStatistic::m_classFavoriteStringStatistic);
        m3d::g_Kernel->AddClass(&IntIntRatioStatistic::m_classIntIntRatioStatistic);
        m3d::g_Kernel->AddClass(&Player::m_classPlayer);
        m3d::g_Kernel->AddClass(&PhysicObj::m_classPhysicObj);
        m3d::g_Kernel->AddClass(&SimplePhysicObj::m_classSimplePhysicObj);
        m3d::g_Kernel->AddClass(&BreakableObject::m_classBreakableObject);
        m3d::g_Kernel->AddClass(&DummyObject::m_classDummyObject);
        m3d::g_Kernel->AddClass(&VehicleSplinter::m_classVehicleSplinter);
        m3d::g_Kernel->AddClass(&ParticleSplinter::m_classParticleSplinter);
        m3d::g_Kernel->AddClass(&ComplexPhysicObj::m_classComplexPhysicObj);
        m3d::g_Kernel->AddClass(&ComplexPhysicObjPartDescription::m_classComplexPhysicObjPartDescription);
        m3d::g_Kernel->AddClass(&AnimatedComplexPhysicObj::m_classAnimatedComplexPhysicObj);
        m3d::g_Kernel->AddClass(&PhysicBody::m_classPhysicBody);
        m3d::g_Kernel->AddClass(&SimplePhysicBody::m_classSimplePhysicBody);
        m3d::g_Kernel->AddClass(&SphericBody::m_classSphericBody);
        m3d::g_Kernel->AddClass(&BoxyBody::m_classBoxyBody);
        m3d::g_Kernel->AddClass(&VehiclePart::m_classVehiclePart);
        m3d::g_Kernel->AddClass(&CompoundVehiclePart::m_classCompoundVehiclePart);
        m3d::g_Kernel->AddClass(&Wheel::m_classWheel);
        m3d::g_Kernel->AddClass(&Vehicle::m_classVehicle);
        m3d::g_Kernel->AddClass(&ArticulatedVehicle::m_classArticulatedVehicle);
        m3d::g_Kernel->AddClass(&Gun::m_classGun);
        m3d::g_Kernel->AddClass(&Shell::m_classShell);
        m3d::g_Kernel->AddClass(&CompoundGun::m_classCompoundGun);
        m3d::g_Kernel->AddClass(&Bullet::m_classBullet);
        m3d::g_Kernel->AddClass(&BulletLauncher::m_classBulletLauncher);
        m3d::g_Kernel->AddClass(&Rocket::m_classRocket);
        m3d::g_Kernel->AddClass(&RocketLauncher::m_classRocketLauncher);
        m3d::g_Kernel->AddClass(&RocketVolleyLauncher::m_classRocketVolleyLauncher);
        m3d::g_Kernel->AddClass(&PlasmaBunch::m_classPlasmaBunch);
        m3d::g_Kernel->AddClass(&PlasmaBunchLauncher::m_classPlasmaBunchLauncher);
        m3d::g_Kernel->AddClass(&MortarShell::m_classMortarShell);
        m3d::g_Kernel->AddClass(&Mortar::m_classMortar);
        m3d::g_Kernel->AddClass(&MortarVolleyLauncher::m_classMortarVolleyLauncher);
        m3d::g_Kernel->AddClass(&ThunderboltLauncher::m_classThunderboltLauncher);
        m3d::g_Kernel->AddClass(&Mine::m_classMine);
        m3d::g_Kernel->AddClass(&MinePusher::m_classMinePusher);
        m3d::g_Kernel->AddClass(&LocationPusher::m_classLocationPusher);
        m3d::g_Kernel->AddClass(&EngineOilLocation::m_classEngineOilLocation);
        m3d::g_Kernel->AddClass(&TurboAccelerationPusher::m_classTurboAccelerationPusher);
        m3d::g_Kernel->AddClass(&ObjPrefab::m_classObjPrefab);
        m3d::g_Kernel->AddClass(&Barricade::m_classBarricade);
        m3d::g_Kernel->AddClass(&Chassis::m_classChassis);
        m3d::g_Kernel->AddClass(&Cabin::m_classCabin);
        m3d::g_Kernel->AddClass(&Basket::m_classBasket);
        m3d::g_Kernel->AddClass(&InfectionZone::m_classInfectionZone);
        m3d::g_Kernel->AddClass(&InfectionTeam::m_classInfectionTeam);
        m3d::g_Kernel->AddClass(&InfectionLair::m_classInfectionLair);
        m3d::g_Kernel->AddClass(&Location::m_classLocation);
        m3d::g_Kernel->AddClass(&StaticAutoGun::m_classStaticAutoGun);
        m3d::g_Kernel->AddClass(&Settlement::m_classSettlement);
        m3d::g_Kernel->AddClass(&Town::m_classTown);
        m3d::g_Kernel->AddClass(&Building::m_classBuilding);
        m3d::g_Kernel->AddClass(&Workshop::m_classWorkshop);
        m3d::g_Kernel->AddClass(&Bar::m_classBar);
        m3d::g_Kernel->AddClass(&Npc::m_classNpc);
        m3d::g_Kernel->AddClass(&Lair::m_classLair);
        m3d::g_Kernel->AddClass(&Team::m_classTeam);
        m3d::g_Kernel->AddClass(&CaravanTeam::m_classCaravanTeam);
        m3d::g_Kernel->AddClass(&VagabondTeam::m_classVagabondTeam);
        m3d::g_Kernel->AddClass(&SgNodeObj::m_classSgNodeObj);
        m3d::g_Kernel->AddClass(&LightObj::m_classLightObj);
        m3d::g_Kernel->AddClass(&DynamicQuest::m_classDynamicQuest);
        m3d::g_Kernel->AddClass(&DynamicQuestDestroy::m_classDynamicQuestDestroy);
        m3d::g_Kernel->AddClass(&DynamicQuestReach::m_classDynamicQuestReach);
        m3d::g_Kernel->AddClass(&DynamicQuestConvoy::m_classDynamicQuestConvoy);
        m3d::g_Kernel->AddClass(&DynamicQuestPeace::m_classDynamicQuestPeace);
        m3d::g_Kernel->AddClass(&DynamicQuestHunt::m_classDynamicQuestHunt);
        m3d::g_Kernel->AddClass(&DynamicScene::m_classDynamicScene);
        m3d::g_Kernel->AddClass(&AIManager::m_classAIManager);
        m3d::g_Kernel->AddClass(&DecisionMatrix::m_classDecisionMatrix);
        m3d::g_Kernel->AddClass(&QuestStateManager::m_classQuestStateManager);
        m3d::g_Kernel->AddClass(&GeomObj::m_classGeomObj);
        m3d::g_Kernel->AddClass(&JointedObj::m_classJointedObj);
        m3d::g_Kernel->AddClass(&CompositeObj::m_classCompositeObj);
        m3d::g_Kernel->AddClass(&PhysicUnit::m_classPhysicUnit);
        m3d::g_Kernel->AddClass(&GeomRepository::m_classGeomRepository);
        m3d::g_Kernel->AddClass(&IzvratRepository::m_classIzvratRepository);
        m3d::g_Kernel->AddClass(&Ware::m_classWare);
        m3d::g_Kernel->AddClass(&BlastWave::m_classBlastWave);
        m3d::g_Kernel->AddClass(&BossMetalArm::m_classBossMetalArm);
        m3d::g_Kernel->AddClass(&BossMetalArmLoad::m_classBossMetalArmLoad);
        m3d::g_Kernel->AddClass(&Boss02::m_classBoss02);
        m3d::g_Kernel->AddClass(&Boss02Arm::m_classBoss02Arm);
        m3d::g_Kernel->AddClass(&Boss03::m_classBoss03);
        m3d::g_Kernel->AddClass(&Boss03Part::m_classBoss03Part);
        m3d::g_Kernel->AddClass(&Boss04::m_classBoss04);
        m3d::g_Kernel->AddClass(&Boss04Part::m_classBoss04Part);
        m3d::g_Kernel->AddClass(&Boss04Station::m_classBoss04Station);
        m3d::g_Kernel->AddClass(&Boss04StationPart::m_classBoss04StationPart);
        m3d::g_Kernel->AddClass(&Boss04Drone::m_classBoss04Drone);
        m3d::g_Kernel->AddClass(&Submarine::m_classSubmarine);
        m3d::g_Kernel->AddClass(&VehicleRecollection::m_classVehicleRecollection);
        m3d::g_Kernel->AddClass(&TeamTacticWithRoles::m_classTeamTacticWithRoles);
        m3d::g_Kernel->AddClass(&VehicleRoleMeat::m_classVehicleRoleMeat);
        m3d::g_Kernel->AddClass(&VehicleRoleSniper::m_classVehicleRoleSniper);
        m3d::g_Kernel->AddClass(&VehicleRoleOppressor::m_classVehicleRoleOppressor);
        m3d::g_Kernel->AddClass(&VehicleRoleBarrier::m_classVehicleRoleBarrier);
        m3d::g_Kernel->AddClass(&VehicleRolePendulum::m_classVehicleRolePendulum);
        m3d::g_Kernel->AddClass(&VehicleRoleCoward::m_classVehicleRoleCoward);
        m3d::g_Kernel->AddClass(&VehicleRoleCheater::m_classVehicleRoleCheater);
        m3d::g_Kernel->AddClass(&NPCMotionController::m_classNPCMotionController);
        m3d::g_Kernel->AddClass(&CinematicMover::m_classCinematicMover);
        m3d::g_Kernel->AddClass(&Static2DPath::m_classStatic2DPath);
        m3d::g_Kernel->AddClass(&Gadget::m_classGadget);
        m3d::g_Kernel->AddClass(&Formation::m_classFormation);
        IntersectionManager::Registration();
    }

    float CServer::GetLevelSize() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Level* CServer::GetLevel() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetPathFindingProfiler()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::RelinkSceneGraphNodes()
    {
        m_pObjects->RelinkSceneGraphNodes();
    }

    CServer::CServer()
    {
        assert(m3d::g_Kernel);

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
        m_pObjects = dynamic_cast<ObjContainer*>(m3d::g_Kernel->New("ObjContainer"));
        m3d::g_Kernel->UnRegisterGlobal("g_ObjContainer");
        m3d::g_Kernel->RegisterGlobal(m_pObjects, "g_ObjContainer");
        SetObjects(m_pObjects);

        auto idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI physic step", 0x1E);
        if (idx < m3d::Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profilerTmpForServer = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI collisions", 0x1E);
        if (idx < m3d::Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_collideProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI bullets update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_bulletProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI blast waves update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_blastWaveProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI vehicles update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_pathFindingProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI total objects update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_objectsUpdateProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI full update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_serverUpdateProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        m3d::g_Kernel->GetEngineCfg().m_console->RegisterCommand("ai_switch_player_physics", 0, this);
        LoadGlobalPropertiesFromXML(m3d::g_Kernel->GetEngineCfg().m_pathToGlobProps.GetS());
    }

    void CServer::LoadTriggersFromXML(CStr const& fileName)
    {
        scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
        if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error: cannot open " + fileName);
            return;
        }

        ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
        if (xmlFile->Read(*stream))
        {
            stream->Close();

            ref_ptr triggersNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(triggersNode, "triggers");

            ref_ptr triggerNode = xmlFile->CreateNode();
            for (triggersNode->GetFirstChild(triggerNode, "trigger"); !triggerNode->IsEmpty(); triggerNode->GetNextSibling(triggerNode, "trigger"))
            {
                CStr name;
                m3d::SafeStrAttrib(name, triggerNode, "Name");
                if (name.empty())
                {
                    M3D_LOG_ERR("Error: empty trigger name");
                    continue;
                }

                // TODO: check this
                auto* entity = ai::theObjects->GetEntityByObjName(name);
                if (ai::theObjects->m_SaveType == ai::ObjContainer::eSAVE_TYPES::SAVE_LEVEL && entity)
                {
                    M3D_LOG_ERR("Attempting to load trigger " + name + ", but a trigger with this name already exists!");
                }
                if (!entity)
                {
                    auto id = ai::thePrototypeManager->GetPrototypeId("trigger");
                    auto objId = ai::theObjects->CreateNewObject(id, "trigger", -1, -1);
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

    void CServer::PostPlayerEvent(eGameEvent)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CServer::GetLastId()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr CServer::GetPrototypeFullName(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr CServer::GetPrototypeFullName(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::Update(float elapsedTime)
    {
        // TODO: generated code
        if (!m_StartServerUpdates)
        {
            return;
        }

        // Start profiling server update
        m_serverUpdateProfiler->StartCountdown();

        // Calculate current time
        float CurTime = 0.0;
        if (m3d::g_Kernel->GetTimer().GetTimeScale() == 0.0f)
        {
            CurTime = m_LastUpdateTime;
        }
        else
        {
            CurTime = static_cast<float>(m3d::g_Kernel->GetTimer().GetCurTime()) * 0.001f;
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

        // Apply minimum frame time constraint
        float minFrameTime = m3d::g_Kernel->GetEngineCfg().m_ai_min_frame_time.GetF();

        float m_averageElapsedTime = elapsedTime;
        if (elapsedTime > minFrameTime)
        {
            elapsedTime = minFrameTime;
            m_averageElapsedTime = minFrameTime;
        }

        // Update moving average of elapsed times
        if (m_Accumulation)
        {
            if (m_CurIndex >= m_MaxAverageLength)
            {
                m_Accumulation = false;
            }
            else
            {
                m_lastElapsedTimes[m_CurIndex] = m_averageElapsedTime;

                // Calculate new average
                m_averageElapsedTime = 0.0f;
                for (int i = 0; i <= m_CurIndex; ++i)
                {
                    m_averageElapsedTime += m_lastElapsedTimes[i];
                }
                m_averageElapsedTime /= static_cast<float>(m_CurIndex + 1);
                m_CurIndex++;
            }
        }

        if (!m_Accumulation)
        {
            if (m_CurIndex >= m_MaxAverageLength)
            {
                m_CurIndex = 0;
            }

            float* currentSlot = &m_lastElapsedTimes[m_CurIndex];
            m_averageElapsedTime += (m_averageElapsedTime - *currentSlot) / static_cast<float>(m_MaxAverageLength);
            *currentSlot = m_averageElapsedTime;
            m_CurIndex++;
        }

        // Use average elapsed time if configured
        if (m_AveElapsedTimeUsed)
        {
            m_averageElapsedTime = m_averageElapsedTime;
            elapsedTime = m_averageElapsedTime;
        }

        // Determine work time based on elapsed time
        unsigned int workTime = 2;
        if (m_averageElapsedTime <= 0.0001f)
        {
            workTime = 0;
        }

        // Handle pause
        if (fPause)
        {
            elapsedTime = 0.0f;
        }

        float v14 = elapsedTime; // Store original elapsed time

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
            bool useCinematic = m3d::g_Kernel->GetEngineCfg().m_UseCinematicUpdate.GetB();

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
        if (m3d::g_Kernel->GetEngineCfg().m_StepMode.GetI() == 1)
        {
            fPause = true;
        }

        // Final profiling update
        m_serverUpdateProfiler->EndCountdown();
    }

    void CServer::LoadGlobalMapFromRawFile(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::ClearOnce()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::EndCinematic()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::GetControlData()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CServer::GetGlobalMapValue(long, long)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CServer::GetPathFindQuant()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr CServer::GetFullNameByObjID(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::Clear()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::ResetCinematicObjects()
    {
        throw retruxx::logic_error("Not implemented");
    }

    PrototypeInfo const* CServer::GetPrototypeInfo(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::InitOnce()
    {
        LoadGlobalPropertiesFromXML(m3d::g_Kernel->GetEngineCfg().m_pathToGlobProps.GetS());
        theResourceManager = new ResourceManager;
        theResourceManager->Init();
        m_pAffixManager = new AffixManager;
        DynamicScene::InitOnce();
        m_pDynamicScene = dynamic_cast<DynamicScene*>(m3d::g_Kernel->New("DynamicScene"));
        m_pDynamicScene->IncRef();
        SetDynamicScene(m_pDynamicScene);
        m3d::TraceLineManager::InitTraceLineRay(true);
        m_pDynamicScene->InitClashDecalId();
        theQuestManager = new QuestManager;
        theQuestManager->LoadFromXmlFile(ai::theGlobProp.m_pathToQuests.c_str());
        theQuestStateManager = dynamic_cast<QuestStateManager*>(m3d::g_Kernel->New("QuestStateManager"));
        m3d::g_Kernel->UnRegisterGlobal("g_QuestStateManager");
        m3d::g_Kernel->RegisterGlobal(theQuestStateManager, "g_QuestStateManager");
        theStatisticManager = new StatisticManager;
        pAIManager = dynamic_cast<AIManager*>(m3d::g_Kernel->New("AIManager"));
        pAIManager->IncRef();
        SetAIManager(pAIManager);
        m3d::g_Kernel->UnRegisterGlobal("g_AIManager");
        m3d::g_Kernel->RegisterGlobal(pAIManager, "g_AIManager");
        thePrototypeManager = new PrototypeManager;
        theRelationship = new Relationship;
        theVehiclesGeneratorInfoCache = new VehiclesGeneratorInfoCache;
    }

    eTolerance CServer::CheckTolerance(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::LoadPrototypeNamesFromXML(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::LoadGlobalPropertiesFromXML(CStr const& fileName)
    {
        scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
        if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error: cannot open " + fileName);
            return;
        }
        ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
        if (xmlFile->Read(*stream))
        {
            stream->Close();
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            xmlFile->GetFirstChild(node, "Properties");
            theGlobProp.LoadFromXML(xmlFile, node);
            return;
        }
        M3D_LOG_ERR("Error: cannot parse " + fileName);\
    }

    AffixManager* CServer::GetAffixManager() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::StartCinematic()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::Init(m3d::CWorld* world)
    {
        m_InCinematic = false;
        m_LastUpdateTime = M3D_KERNEL->GetTimer().GetCurTime() * 0.001;
        m_pWorld = world;
        _SetLevel(world->m_level);
        m_pObjects->AllowSave(true);
        thePlayer = nullptr;
        m_pExternalPaths = new ExternalPaths;
        m_pPlayerPassMap = new PlayerPassMap;
        M3D_KERNEL->GetEngineCfg().m_console->executeCommand("/conScript data\\scripts\\server.lua");
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
            n_EndCinematic,
            "EndCinematic",
            "void",
            "void",
            "Revert Server to normal update mode");
        scriptServer.registerGlobalFunction(
            n_CreateObjectByClassName,
            "CreateObjectByClassName",
            "Object*",
            "const char* className",
            "Creates Object");

        pGlobalMap = new Map;
        pGlobalMap->Create(
            m_level->m_passMapCellSize,
            m_level->m_passMapCellSize,
            pServer->m_level->land_size * 128.0,
            pServer->m_level->land_size * 128.0,
            nullptr
        );
        pGlobalMap->Clear();
        Map::SetGlobalMap(pGlobalMap);
        pGlobalMap->LoadFromRawFile(m_level->GetFullPathNameA(m_level->m_passMapName).c_str());
    }

    void CServer::PutGameData()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::HandleCommand(int, m3d::CConsoleParams const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CServer::~CServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool CServer::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::SaveVisitedMap(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::_PostProcessConsoleCommands()
    {
        while (!m_consoleCommandsToPostProcess.empty())
        {
            auto command = m_consoleCommandsToPostProcess.front();
            m_consoleCommandsToPostProcess.pop();
            throw retruxx::logic_error("Not implemented");
        }
    }

    void CServer::_RegisterConsoleCommands()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::RestorePrevCinematicState()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CServer::SavePrevCinematicState()
    {
        throw retruxx::logic_error("Not implemented");
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
        // TODO: implement UpdateLights
        // throw std::logic_error("Not implemented");
    }
}

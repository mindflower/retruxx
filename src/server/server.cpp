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
#include "objects/guns/mine.h"
#include "objects/guns/minepusher.h"
#include "objects/guns/mortar.h"
#include "objects/guns/mortarshell.h"
#include "objects/guns/mortarvolleylauncher.h"
#include "objects/guns/plasmabunch.h"
#include "objects/guns/plasmabunchlauncher.h"
#include "objects/guns/rocket.h"
#include "objects/guns/rocketlauncher.h"
#include "objects/guns/rocketvolleylauncher.h"
#include "objects/guns/thunderboltlauncher.h"
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

namespace ai
{
    extern ResourceManager* theResourceManager;
    extern PrototypeManager* thePrototypeManager;
    extern Relationship* theRelationship;
    extern DynamicScene* gDynamicScene;
    extern Player* thePlayer;
    extern ProcessManager* theProcessManager;
    extern AIManager* theAIManager;

    int n_AddToCinematic(m3d::sArgStack& scriptStack)
    {
        throw std::logic_error("Not implemented");
    }

    int n_EndCinematic(m3d::sArgStack& scriptStack)
    {
        throw std::logic_error("Not implemented");
    }

    int n_CreateObjectByClassName(m3d::sArgStack& scriptStack)
    {
        throw std::logic_error("Not implemented");
    }

    CServer* pServer = nullptr;

    void CServer::AddToCinematic(Obj*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::AddToCinematic(int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::SetPause(bool)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetTmpProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetBulletProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetCollideProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetObjectsUpdateProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    ExternalPaths const* CServer::GetExternalPaths() const
    {
        throw std::logic_error("Not implemented");
    }

    PlayerPassMap const* CServer::GetPlayerPassMap() const
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::SetLastId(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::Load(StartupMode, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*, bool, ObjContainer::eSAVE_TYPES)
    {
        throw std::logic_error("Not implemented");
    }

    bool CServer::GetPause() const
    {
        throw std::logic_error("Not implemented");
    }

    int CServer::GetPrototypeId(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    PrototypeInfo* CServer::CreatePrototypeInfoByClassName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::CWorld* CServer::GetWorld()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::LoadVisitedMap(CStr const&, bool)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    m3d::Level* CServer::GetLevel() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Profiler* CServer::GetPathFindingProfiler()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::RelinkSceneGraphNodes()
    {
        throw std::logic_error("Not implemented");
    }

    CServer::CServer()
    {
        assert(m3d::g_Kernel);
        m_lastElapsedTimes.resize(0x14, 0.0);
        m_pObjects = dynamic_cast<ObjContainer*>(m3d::g_Kernel->New("ObjContainer"));
        m3d::g_Kernel->UnRegisterGlobal("g_ObjContainer");
        m3d::g_Kernel->RegisterGlobal(m_pObjects, "g_ObjContainer");
        SetObjects(m_pObjects);

        auto idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI physic step", 0x1E);
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            m_profilerTmpForServer = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI collisions", 0x1E);
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            m_collideProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI bullets update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            m_bulletProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI blast waves update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            m_blastWaveProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI vehicles update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            m_pathFindingProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI total objects update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            m_objectsUpdateProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        idx = m3d::Application::g_pApp->GetProfilerStack().AddProfiler("AI full update", 0x1E);
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            m_serverUpdateProfiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }

        m3d::g_Kernel->GetEngineCfg().m_console->RegisterCommand("ai_switch_player_physics", 0, this);
        LoadGlobalPropertiesFromXML(m3d::g_Kernel->GetEngineCfg().m_pathToGlobProps.GetS());
    }

    void CServer::LoadTriggersFromXML(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::PostPlayerEvent(eGameEvent)
    {
        throw std::logic_error("Not implemented");
    }

    int CServer::GetLastId()
    {
        throw std::logic_error("Not implemented");
    }

    CStr CServer::GetPrototypeFullName(int)
    {
        throw std::logic_error("Not implemented");
    }

    CStr CServer::GetPrototypeFullName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::Update(float)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::LoadGlobalMapFromRawFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::ClearOnce()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::EndCinematic()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::GetControlData()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CServer::GetGlobalMapValue(long, long)
    {
        throw std::logic_error("Not implemented");
    }

    int CServer::GetPathFindQuant()
    {
        throw std::logic_error("Not implemented");
    }

    CStr CServer::GetFullNameByObjID(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::Clear()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::ResetCinematicObjects()
    {
        throw std::logic_error("Not implemented");
    }

    PrototypeInfo const* CServer::GetPrototypeInfo(int)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void CServer::LoadPrototypeNamesFromXML(CStr const&)
    {
        throw std::logic_error("Not implemented");
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
            xmlFile->GetFirstChild_(node, "Properties");
            theGlobProp.LoadFromXML(xmlFile, node);
            return;
        }
        M3D_LOG_ERR("Error: cannot parse " + fileName);\
    }

    AffixManager* CServer::GetAffixManager() const
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::StartCinematic()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void CServer::HandleCommand(int, m3d::CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    CServer::~CServer()
    {
        throw std::logic_error("Not implemented");
    }

    bool CServer::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::SaveVisitedMap(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::_PostProcessConsoleCommands()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::_RegisterConsoleCommands()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::RestorePrevCinematicState()
    {
        throw std::logic_error("Not implemented");
    }

    void CServer::SavePrevCinematicState()
    {
        throw std::logic_error("Not implemented");
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
}

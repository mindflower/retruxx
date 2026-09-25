#include "geomobject.h"
#include "server/processmanager.h"
#include "server/queststate.h"
#include "server/statistic/statisticmanager.h"
#include "globalscriptfuncs.h"
#include "m3dgame.h"
#include "profile.h"
#include "vivisectionblock.h"
#include "impulses/truxximpulses.h"
#include "music/blockmusicmanager.h"
#include "music/radioengine.h"
#include "music/townmusicmanager.h"
#include "uimanager/truxxuimanager.h"
#include "uimisc/guihelper.h"
#include "uimisc/savesmanager.h"
#include "uiwindows/charwindows/motherpanel.h"
#include "uiwindows/miscwindows/mainmenu.h"
#include <cameracontroller.h>
#include <cinematic.h>
#include <client.h>
#include <math/coremath.h>
#include <config.h>
#include <landscape.h>
#include <world.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <posteffects/posteffectmanager.h>
#include <server/server.h>
#include <ctime>
#include <stdexcept>
#include <core/log.h>
#include <scene/servers/dataserver.h>

#include "level.h"
#include "video.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "server/dynamicscene.h"
#include "server/passagedata.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/geomobj.h"
#include "uimisc/questinfo.h"
#include "uiwindows/miscwindows/cinemapanel.h"
#include <algorithm>
#include <game/uimisc/weapongroup.h>
#include "uimanager/uidefs.h"
#include "server/event.h"
#include "uiwindows/miscwindows/gamemenu.h"

#include "uiwindows/miscwindows/cinemafadepanel.h"

extern "C"
{
#include <engine/ode/sources/collision_kernel.h>
}

#include <server/objects/player.h>

#undef GetFirstChild
#undef GetNextSibling

extern Vivisector* g_Vivisector;

bool LightActivated = false;

namespace m3d
{
    extern CClient* pClient;
}

namespace ai
{
    extern CServer* pServer;
}

namespace
{
    m3d::CConsoleCommands conCommands[] = {
        {"map", 0x1000},
        {"music", 0x1001},
        {"debug", 0x1002},
        {"reload", 0x1003},
        {"noclip", 0x1005},
        {"goto", 0x1006},
        {"saveWorld", 0x1007},
        {"saveServers", 0x1008},
        {"resetUnits", 0x1009},
        {"cinematic", 0x1011},
        {"setProfile", 0x1012},
        {"netStartStats", 0x1013},
        {"netPrintStats", 0x1014},
        {"miniDump", 0x1015},
        {"nextmap", 0x1016},
        {"g_rebuildshores", 0x1017},
        {"showPlayerStats", 0x1019},
        {"g_postEffectReload", 0x1020},
        {"g_postEffect", 0x1021},
        {"g_postEffectSetParam", 0x1022},
        {"g_postEffectKill", 0x1023},
        {"dxCursor", 0x1024},
    };

    int videoNum = 0;

    CinemaPanel* GetCinemaPanel()
    {
        auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
        auto wnd = app->m_pInterfaceManager->GetWindow(18);
        if (!wnd)
        {
            return nullptr;
        }
        if (!wnd->IsKindOf(RT_CLASS_LOCAL(CinemaPanel)))
        {
            return nullptr;
        }
        return dynamic_cast<CinemaPanel*>(&*wnd);
    }
}  // namespace

unsigned m_profiler_Client = 0;
unsigned m_profiler_GetPackets = 0;
unsigned m_profiler_ServerUpdate = 0;
unsigned m_profiler_ClientUpdate = 0;

RT_CLASS_EXPORTS_BEGIN(CMiracle3d)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CMiracle3d);

GameState CMiracle3d::CurGameMode::Get() const
{
    return m_mode;
}

void CMiracle3d::CurGameMode::Set(GameState mode)
{
    //TODO: check this
    auto oldMode = m_mode;
    m_mode = mode;
    g_pApp->ImmediateMessage(65683, mode, oldMode, 0, 0, {}, {});
}

void CMiracle3d::Player::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x419100
    auto cameraMode = m_cameraMode;
    if (!xmlNode->IsEmpty())
    {
        if (auto const* attr = xmlNode->GetAttribute("CameraMode"))
        {
            cameraMode = static_cast<CameraModes>(atoi(attr));
        }
    }
    m_cameraMode = cameraMode;
    m3d::SafeVectorAttrib(m_gameLookAt, xmlNode, "GameLookAt");
    m3d::SafeQuaternionAttrib(m_lastobjQuat, xmlNode, "LastObjQuat");
    if (!xmlNode->IsEmpty())
    {
        if (auto const* attr = xmlNode->GetAttribute("ElapsedTime"))
        {
            m_elapsedtime = atoi(attr);
        }
    }
    if (!xmlNode->IsEmpty())
    {
        if (auto const* attr = xmlNode->GetAttribute("DesiredDistance"))
        {
            m_desiredDistance = static_cast<float>(atof(attr));
        }
    }
}

void CMiracle3d::Player::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x4191A0
    xmlNode->SetAttribute("CameraMode", CStr(static_cast<int>(m_cameraMode)).c_str());
    xmlNode->SetAttribute("GameLookAt", CStr(m_gameLookAt).c_str());
    CStr quat;
    quat.format("%.4f %.4f %.4f %.4f", m_lastobjQuat.x, m_lastobjQuat.y, m_lastobjQuat.z, m_lastobjQuat.w);
    xmlNode->SetAttribute("LastObjQuat", quat.c_str());
    xmlNode->SetAttribute("ElapsedTime", CStr(m_elapsedtime).c_str());
    xmlNode->SetAttribute("DesiredDistance", CStr(m_desiredDistance).c_str());
}

int CMiracle3d::OnChangeMode(m3d::AuxImpulseInfo const& impInfo)
{
    if (!impInfo.m_state)
    {
        return 1;
    }

    if (m_curGameMode.Get() == GS_CINEMATIC && impInfo.m_impId != 2)
    {
        if (m_cinematic->m_state != m3d::CINEMATIC_NOT_INITED && m_cinematic->m_state != m3d::CINEMATIC_EXIT_FADE_OUT &&
            m_cinematic->m_state != m3d::CINEMATIC_EXIT_FADE_IN)
        {
            CinematicInterrupt();
            HandleCinematic(0.0);
            return 1;
        }
    }

    if (m_curGameMode.Get() != GS_MAINMENU && impInfo.m_impId == 1)
    {
        StopPlayingMusic();
        if (m_gameInited)
        {
            ClearViewportToBlack();
            CinematicClear();

            auto* savesManager = M3D_APP->m_pInterfaceManager->GetSavesManager();
            CStr const tempMaps = savesManager->GetPathForTemporaryMaps();
            help::DeleteAllFilesInDirectory(tempMaps.c_str());
            if (m3d::pClient)
            {
                ProcessAllEvents();
                M3D_APP->m_pInterfaceManager->ShowWindow(
                    IW_WND_FADE_PANEL_BEFORE_NEXT_MAP, false, false, false, false, nullptr);
                M3D_APP->m_pInterfaceManager->LaunchEvent(IE_CUST_END_LEVEL, GUI_EVENT_CUSTOM, nullptr);

                ai::pServer->Clear();
                ai::pServer->ClearOnce();

                m3d::pClient->Reset();
                m3d::pClient->GetWorld().Release();

                DiscardAllEvents();
                M3D_APP->m_pImpulses->ResetAllImpulses(true);
                m_bRenderAsBackground = false;
                m_bBackgroundTextureIsValid = false;
            }
        }
        m_curGameMode.Set(GS_MAINMENU);
        //TODO: check this (1.0)
        M3D_KERNEL->GetTimer().SetTimeScale(1.0);
        m_saveTimeScale = M3D_KERNEL->GetTimer().GetTimeScale();

        AllowRendering();
        if (!LoadMainMenuLevel())
        {
            GameDone();
        }

        M3D_APP->m_pInterfaceManager->Show(false, true);
        M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_MAINMENU, true, true, false, false, nullptr);

        SetCursorShow(true);
        CaptureMouse(nullptr);

        if (!m_bDoNotLoadMainmenuLevel)
        {
            M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_CINEMA_FADE_PANEL, 1, 1, false, false, nullptr);
            ref_ptr<Wnd> wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_CINEMA_FADE_PANEL);
            if (IsDirectChild(wnd))
            {
                MoveChildToFirstPosition(wnd);
            }
        }
        return 1;
    }

    if (m_curGameMode.Get() == GS_MAINMENU && impInfo.m_impId == 3)
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_MAINMENU, false, true, false, false, nullptr);
        CleanMainMenuLevel();
        if (!m_gameInited)
        {
            GameInit();
        }

        // TODO: check this
        M3D_APP->m_pInterfaceManager->StartSplashing(11);
        int loadRes =
            LoadLevel(
                M3D_KERNEL->GetEngineCfg().m_levFileName.GetS(),
                {},
                false,
                true,
                false,
                nullptr,
                nullptr,
                ai::ObjContainer::SAVE_LEVEL) ==
            0;
        if (loadRes)
        {
            return 0;
        }
        CaptureMouse(nullptr);
        m_curGameMode.Set(GS_GAME);
    }
    if (impInfo.m_impId == 0)
    {
        if (m_curGameMode.Get() == GS_GAME)
        {
            M3D_APP->m_pInterfaceManager->GetSavesManager()->MakeCurGameScreenshot();
        }
        M3D_APP->EnqueueMessage(UM_SHOWGAMEMENU, 0, 0, 0, 0, GameMenuWnd::ROOT_LEVEL_NAME, {});
        return 1;
    }

    if (impInfo.m_impId == 2)
    {
        CinematicInit();
        return 1;
    }

    if (impInfo.m_impId == 3)
    {
        m_curGameMode.Set(GS_GAME);
        if (m_curGameMode.Get() != GS_CINEMATIC)
        {
            SetCursorShow(true);
        }
        CaptureMouse(nullptr);
    }
    return 1;
}

void CMiracle3d::SkipCinematicMessage()
{
    // RVA 0x41E400
    OnChangeMode(m3d::AuxImpulseInfo(3, true, m_curGameMode.m_mode, 0, 0));
}

int CMiracle3d::OnGameDrag(m3d::AuxImpulseInfo const& impInfo)
{
    // RVA 0x402F80
    if (GetWndMouseOver() != this)
    {
        m_pImpulses->ResetImpulseWithoutNotification(impInfo.m_impId);
        return 1;
    }

    // A press grabs the mouse; a release (or a second press) lets it go.
    if (!impInfo.m_state || GetCapture() == this)
    {
        CaptureMouse(nullptr);
    }
    else
    {
        CaptureMouse(this);
    }
    m_dragHit.x = static_cast<float>(M3D_APP->GetMouseX());
    m_dragHit.y = static_cast<float>(M3D_APP->GetMouseY());
    return 0;
}

float CMiracle3d::GetMinTimeScale() const
{
    // RVA 0x40C4E0
    return m_minTimeScale;
}

int CMiracle3d::OnFinishVideoPlaying()
{
    if (M3D_APP->m_sound)
    {
        M3D_APP->m_sound->PauseAllSounds(false);
    }

    auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_MAINMENU);
    if (auto* mainMenu = RT_DYNCAST(wnd.get(), MainMenuUI))
    {
        if (mainMenu->IsChildOf(M3D_APP))
        {
            mainMenu->OnFinishVideoPlaying();
        }
    }

    return 1;
}

void CMiracle3d::SetMinTimeScale(float value)
{
    // RVA 0x40C4D0
    m_minTimeScale = value;
}

bool CMiracle3d::LoadSavedGame(CStr const& saveDir)
{
    // RVA 0x4202C0
    ClearViewportToBlack();
    M3D_LOG_INFO("Game loading: \"" + saveDir + "\" ...");

    CStr const fileName = saveDir + "\\currentmap.xml";
    m3d::g_Kernel->GetFileServer().AddFile(fileName.c_str());
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        M3D_ENGINE_CFG.m_console->PrintF("File " + fileName + " not found\n");
        return false;
    }

    m3d::g_Kernel->GetTimer().SetActiveState(1);
    ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
    xmlFile->Read(*stream);

    ref_ptr ndGeneral = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    xmlFile->GetFirstChild(ndGeneral, "General");
    ndGeneral->IsEmpty();
    bool res = true;
    CStr const sMapName = ndGeneral->GetAttribute("MapName");

    ref_ptr ndDynamicScene = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    ndGeneral->GetFirstChild(ndDynamicScene, "DynamicScene");
    ndDynamicScene->IsEmpty();

    if (help::GetMapNameFromFileName(CStr(M3D_ENGINE_CFG.m_levFileName.GetS())) != sMapName)
    {
        // The save was made on another map: load that map from scratch.
        m_pInterfaceManager->StartSplashing(12);
        res = LoadMap(sMapName, false, xmlFile, ndDynamicScene, ai::ObjContainer::SAVE_FULL);
        if (!res)
        {
            return false;
        }
    }
    else
    {
        // Same map: reset the running level and reload its objects in place.
        m_pInterfaceManager->StartSplashing(1);
        CleanLevel(true, false);
        PutSplash(0, GetStringByStringId0("SavedGameLoading").c_str());
        m3d::pClient->GetWorld().GetWheelTracesMgr().ClearTraces();
        ai::pServer->Init(&m3d::pClient->GetWorld());
        m3d::pClient->GetWorld().GetLandscape().ManageLandScapeCollisionTriMeshes();
        PutSplash(2, GetStringByStringId0("SavedGameLoading").c_str());
        m_pInterfaceManager->LaunchEvent(84, GUI_EVENT_CUSTOM, nullptr);
        ai::pServer->Load(ai::LOCAL_GAME, xmlFile, ndDynamicScene, true, ai::ObjContainer::SAVE_FULL);
        PutSplash(55, GetStringByStringId0("SavedGameLoading").c_str());
        m_pInterfaceManager->GetQuestInfoManager()->Init();
    }

    PutSplash(65, GetStringByStringId0("SavedGameLoading").c_str());
    CStr const tmpMapsDir = m_pInterfaceManager->GetSavesManager()->GetPathForTemporaryMaps();
    help::CopyDirectory(saveDir.c_str(), tmpMapsDir.c_str());
    DeleteFileA((tmpMapsDir + "\\currentmap.xml").c_str());
    m_pInterfaceManager->Load(xmlFile, ndGeneral);

    PutSplash(70, GetStringByStringId0("SavedGameLoading").c_str());
    ref_ptr ndQuestManager = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    ndGeneral->GetFirstChild(ndQuestManager, "QuestManager");
    ndQuestManager->IsEmpty();
    ai::theQuestStateManager->LoadFromXml(xmlFile, ndQuestManager);

    PutSplash(85, GetStringByStringId0("SavedGameLoading").c_str());
    ref_ptr ndApplication = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    ndGeneral->GetFirstChild(ndApplication, "Application");
    ndApplication->IsEmpty();
    LoadFromXml(xmlFile, ndApplication);

    PutSplash(86, GetStringByStringId0("SavedGameLoading").c_str());
    ref_ptr ndGameTime = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    ndGeneral->GetFirstChild(ndGameTime, "GameTime");
    ndGameTime->IsEmpty();
    ai::theObjects->m_GameTime.LoadFromXML(xmlFile, ndGameTime);

    PutSplash(87, GetStringByStringId0("SavedGameLoading").c_str());
    ref_ptr ndWeatherState = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    ndGeneral->GetFirstChild(ndWeatherState, "WeatherState");
    ndWeatherState->IsEmpty();
    m3d::pClient->GetWorld().GetWeatherManager().LoadWeatherStateFromXMLNode(xmlFile, ndWeatherState);

    PutSplash(88, GetStringByStringId0("SavedGameLoading").c_str());
    ref_ptr ndProcessManager = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    ndGeneral->GetFirstChild(ndProcessManager, "ProcessManager");
    ndProcessManager->IsEmpty();
    ai::theProcessManager->LoadFromXML(xmlFile, ndProcessManager);

    PutSplash(89, GetStringByStringId0("SavedGameLoading").c_str());
    ref_ptr ndStatistics = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    ndGeneral->GetFirstChild(ndStatistics, "Statistics");
    ndStatistics->IsEmpty();
    ai::theStatisticManager->LoadFromXml(xmlFile, ndStatistics);

    PutSplash(90, GetStringByStringId0("SavedGameLoading").c_str());
    m3d::g_Kernel->GetTimer().SetTimeScale(1.0f);
    m_serverAnimatedModels->GenerateImpostorsIfNeeded();

    PutSplash(100, GetStringByStringId0("SavedGameLoading").c_str());
    StartLevelType startType = FROM_SAVE;
    m_pInterfaceManager->LaunchEvent(85, GUI_EVENT_CUSTOM, &startType);
    M3D_LOG_INFO("Game loaded: \"" + saveDir + "\"");

    m3d::g_Kernel->GetTimer().SetActiveState(1);
    if (AppActive() && M3D_ENGINE_CFG.m_clipCursorWithinRenderWnd.GetB())
    {
        CaptureAndClipSystemCursor(true);
    }
    m_bRenderAsBackground = false;
    m_bBackgroundTextureIsValid = false;
    return res;
}

bool CMiracle3d::GetMouseHitPoint(CVector& hitPoint, m3d::SgNode*& sgNode)
{
    using namespace ai;
    using namespace m3d;

    static scoped_ptr mouseRay = ai::Ray::CreateObject(nullptr, 1.0, nullptr);
    mouseRay->SetLength(10000.0);

    dGeomSetPosition(
        mouseRay->GetGeomId(), m_curCamera.m_worldOrigin.x, m_curCamera.m_worldOrigin.y, m_curCamera.m_worldOrigin.z);

    CMatrix viewMatrix;
    viewMatrix.rotYPR(m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);

    CVector viewDir;
    viewDir.x = viewMatrix._13;
    viewDir.y = viewMatrix._23;
    viewDir.z = viewMatrix._33;

    mouseRay->SetDirection(viewDir);

    sgNode = nullptr;

    static dContact contact;
    if (TraceLine(*mouseRay, contact, false, false, true, false, nullptr, false, false))
    {
        hitPoint.x = contact.geom.pos[0];
        hitPoint.y = contact.geom.pos[1];
        hitPoint.z = contact.geom.pos[2];

        auto* obj = static_cast<m3d::Object*>(dGeomGetData(contact.geom.g2));
        if (!obj)
        {
            return true;
        }

        if (IS_KIND_OF(obj, PhysicBody))
        {
            auto* physBody = RT_DYNCAST(obj, PhysicBody);
            sgNode = physBody->m_Node;
            return true;
            ;
        }

        if (IS_KIND_OF(obj, GeomObjectLandscape) || IS_KIND_OF(obj, GeomObjectStatics) ||
            IS_KIND_OF(obj, GeomObjectWater) || IS_KIND_OF(obj, GeomObjectRoad) || IS_KIND_OF(obj, GeomObj))
        {
            sgNode = &m3d::pClient->GetWorld().GetLandscape();
        }

        return true;
    }

    return false;
}

//Verified: CMiracle3d::OnFinishIntroVideoPlaying
int CMiracle3d::OnFinishIntroVideoPlaying()
{
    if (M3D_KERNEL->GetEngineCfg().m_autoPlayVideo.GetB() && videoNum >= 0 && videoNum < 4)
    {
        auto const videoFile = M3D_KERNEL->GetEngineCfg().m_video[videoNum].GetS();
        ++videoNum;
        StartPlayingVideo(videoFile, &CMiracle3d::OnFinishIntroVideoPlaying);
    }
    else
    {
        StartMainMenu();
    }
    return 1;
}

int CMiracle3d::GameDone()
{
    m_gameInited = false;
    delete m3d::pClient;
    M3D_KERNEL->GetEngineCfg().m_levFileName.Set("Empty", true);
    return 1;
}

int CMiracle3d::OnFlyMouse(m3d::AuxImpulseInfo const& impInfo)
{
    // RVA 0x401020
    if (GetCapture() != this)
    {
        CaptureMouse(this);
    }

    if (impInfo.m_state)
    {
        float dx = 0.0f;
        float dy = 0.0f;
        impInfo.UnpackXy(nullptr, nullptr, &dx, &dy);
        if (IsMouseYAxisFlipped())
        {
            dy = 0.0f - dy;
        }
        if (IsMouseXAxisFlipped())
        {
            dx = 0.0f - dx;
        }
        auto const sensitivity = GetMouseSensitivity();
        m_flyCamTurn.x += dx * sensitivity * 0.003f;
        m_flyCamTurn.y += sensitivity * dy * 0.003f;
    }
    return 1;
}

int CMiracle3d::GameInit()
{
    m3d::pClient = new m3d::CClient;
    m3d::pClient->Init();
    m_gameInited = true;
    return 1;
}

bool CMiracle3d::GetCursorShow() const
{
    auto app = dynamic_cast<CMiracle3d*>(g_pApp);
    if (app->m_pInterfaceManager->IsGameModeValidForSmartCursor(m_curGameMode.Get()))
    {
        return false;
    }
    return Wnd::GetCursorShow();
}

bool CMiracle3d::GetCursorShow0() const
{
    return Wnd::GetCursorShow();
}

void CMiracle3d::SetCursorShow(bool state)
{
    auto app = dynamic_cast<CMiracle3d*>(g_pApp);
    Wnd::SetCursorShow(state);
    //TODO: check this
    if (app->m_pInterfaceManager->IsGameModeValidForSmartCursor(m_curGameMode.Get()))
    {
        ImmediateMessage(65682, state, 0, 0, 0, {}, {});
    }
}

int CMiracle3d::CinematicInit()
{
    ai::pServer->StartCinematic();
    M3D_APP->m_pImpulses->ResetAllImpulses(false);

    if (GetCapture() != this)
    {
        auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(72);
        if (!wnd || !wnd->IsChildOf(this))
        {
            CaptureMouse(this);
        }
    }

    auto* station = GetStation();
    station->SetCursorShow(false);
    m_curGameMode.Set(GS_CINEMATIC);
    m_cinematic->m_playTime = 0;
    m_cinematic->m_fadeStartTime = m_cinematic->m_playTime;
    m_cinematic->m_state = m3d::CINEMATIC_ENTER_FADE_OUT;

    auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(19);
    if (wnd)
    {
        auto* cinemaFadePanel = (CinemaFadePanel*)&(*wnd);
        cinemaFadePanel->AttachToScreenCinematicRelated();
    }
    return 1;
}

void CMiracle3d::PlayHackedMusic(HackedMusicType musicType, bool bForceRestart)
{
    if (M3D_KERNEL->GetEngineCfg().m_mus_Enable.GetB())
    {
        if (musicType == HACKMUSIC_GAME)
        {
            m_blockMusicManager->PlayCurrentMusic();
            this->m_hackedMusicType = HACKMUSIC_GAME;
            this->m_bMustStartNewMusic = 0;
        }
        else
        {
            m_bMustStartNewMusic = this->m_bMustStartNewMusic;
            if (m_bMustStartNewMusic && musicType == HACKMUSIC_BAR)
            {
                m_townMusicManager->Activate();
                this->m_hackedMusicType = HACKMUSIC_BAR;
                this->m_bMustStartNewMusic = 0;
            }
            else
            {
                if (musicType != HACKMUSIC_CUSTOM &&
                    (m_bMustStartNewMusic || bForceRestart || musicType != this->m_hackedMusicType) &&
                    m3d::Application::g_pApp->m_sound && musicType >= HACKMUSIC_MENU && musicType < m_musicNames.size())
                {
                    m3d::Application::StartPlayingMusic(this->m_musicNames[musicType].c_str(), 1, 1);
                }
                this->m_hackedMusicType = musicType;
                this->m_bMustStartNewMusic = 0;
            }
        }
    }
}

namespace
{
    bool l_modalsJustClosed = false;

    CinemaFadePanel* GetCinemaFadePanel()
    {
        auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(19);
        auto cinemaFadePanel = (CinemaFadePanel*)&(*wnd);
        return cinemaFadePanel;
    }
}  // namespace

bool CMiracle3d::CinematicFade()
{
    // TODO: generated code
    int fadeTime = m_cinematic->m_playTime - m_cinematic->m_fadeStartTime;
    double fadePeriodDouble = m_cinematic->GetFadePeriodForState(m_cinematic->m_state) * 1000.0;
    int fadePeriod = static_cast<int>(fadePeriodDouble);

    bool result = true;

    // Handle skipped cinematic case
    if (m_cinematic->m_bWasSkippedInEnterFadeOut)
    {
        m3d::CinematicState m_state = m_cinematic->m_state;
        if (m_state == m3d::CINEMATIC_ENTER_FADE_IN || m_state == m3d::CINEMATIC_EXIT_FADE_OUT)
        {
            fadeTime = fadePeriod;
        }
    }

    CinemaPanel* cinemaPanel = GetCinemaPanel();  // Assuming this returns CinemaPanel*

    if (fadeTime < fadePeriod)
    {
        if (l_modalsJustClosed)
        {
            l_modalsJustClosed = 0;
        }
        return result;
    }

    m3d::Cinematic* currentCinematic = m_cinematic;

    switch (currentCinematic->m_state)
    {
    case m3d::CINEMATIC_ENTER_FADE_OUT:
    {
        // Store GUI state and hide interface
        m_bGuiWasHiddenBeforeCinematic = M3D_APP->m_pInterfaceManager->IsHiddenByUser();

        ref_ptr wndMainMenu = M3D_APP->m_pInterfaceManager->GetWindow(72);

        // Show interface and close modals if needed
        if (!wndMainMenu || !M3D_APP || !wndMainMenu->IsChildOf(M3D_APP))
        {
            M3D_APP->m_pInterfaceManager->Show(false, true);
            CloseAllModalWithCancelRet();
            l_modalsJustClosed = true;
        }

        // Clear cinema panel if exists
        if (cinemaPanel)
        {
            cinemaPanel->Clear();
        }

        // Show/hide windows based on cinematic flags
        if ((m_cinematic->m_curItem.m_flags & 4) == 0)
        {
            M3D_APP->m_pInterfaceManager->ShowWindow(18, true, true, false, false, false);
        }

        // Update cinematic state and post event
        m_cinematic->m_fadeStartTime = m_cinematic->m_playTime;
        m_cinematic->m_state = m3d::CINEMATIC_ENTER_FADE_IN;
        ai::pServer->PostPlayerEvent(static_cast<ai::eGameEvent>(61));

        // Handle fade panel if needed
        if ((m_cinematic->m_curItem.m_flags & 1) != 0)
        {
            M3D_APP->m_pInterfaceManager->ShowWindow(19, true, true, false, false, false);

            CinemaFadePanel* fadePanel = GetCinemaFadePanel();
            if (fadePanel)
            {
                if (m3d::Object::IsDirectChild(fadePanel))
                {
                    m3d::Object::MoveChildToFirstPosition(fadePanel);
                }
            }
        }

        // Clean up window reference
        result = false;
        break;
    }

    case m3d::CINEMATIC_ENTER_FADE_IN:
    {
        if (currentCinematic->m_bWasSkippedInEnterFadeOut)
        {
            currentCinematic->m_fadeStartTime = currentCinematic->m_playTime;
            m_cinematic->m_state = m3d::CINEMATIC_EXIT_FADE_OUT;
            result = false;
        }
        else
        {
            currentCinematic->m_state = m3d::CINEMATIC_IS_PLAYING;
            M3D_APP->m_pInterfaceManager->ShowWindow(19, false, false, false, false, false);
            result = true;
        }
        break;
    }

    case m3d::CINEMATIC_EXIT_FADE_OUT:
    {
        currentCinematic->Stop();

        // Update engine configuration
        m3d::g_Kernel->GetEngineCfg().m_FogOfWar.SetI(1, false);
        ai::pServer->EndCinematic();

        // Handle cinematic skipping or continuation
        if (m_cinematic->m_bWasSkipped)
        {
            ai::pServer->PostPlayerEvent(ai::GE_SKIP_CINEMATIC);
            while (m_cinematic->SkipCinematic())
            {
                // Continue skipping
            }
            m_cinematic->m_bWasSkipped = false;
        }
        else if (m_cinematic->m_cinematicItems.size() > 0)
        {
            if (ai::thePlayer)
            {
                CStr flyPathName = m_cinematic->GetNextFlyPathName();
                m3d::AIParam aiParam(flyPathName);
                ai::thePlayer->CauseEvent(ai::GE_START_CINEMATIC_FLY, 0.0, aiParam, m3d::AIParam());
            }
            else
            {
                ai::pServer->PostPlayerEvent(ai::GE_SKIP_CINEMATIC);
                while (m_cinematic->SkipCinematic())
                {
                    // Continue skipping
                }
            }
        }
        else
        {
            ai::pServer->PostPlayerEvent(ai::GE_END_CINEMATIC);
        }

        // Update cinematic state
        m_cinematic->m_fadeStartTime = m_cinematic->m_playTime;
        m_cinematic->m_state = m3d::CINEMATIC_EXIT_FADE_IN;

        // Handle next cinematic or cleanup
        if (m_cinematic->bMustBeNextCinematic())
        {
            if (cinemaPanel)
            {
                cinemaPanel->Clear();
                // Note: The deque tidy operation would need proper context
            }
            result = false;
        }
        else
        {
            if (cinemaPanel)
            {
                cinemaPanel->OnHide();
            }

            // Restore GUI state
            bool showGui = !m_bGuiWasHiddenBeforeCinematic;
            M3D_APP->m_pInterfaceManager->Show(showGui, true);

            // Change game mode
            m3d::AuxImpulseInfo impulseInfo(3, 1, m_curGameMode.m_mode, 1u, false);
            OnChangeMode(impulseInfo);
            result = false;
        }
        break;
    }

    case m3d::CINEMATIC_EXIT_FADE_IN:
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(19, false, false, false, false, false);

        if (m_cinematic->bMustBeNextCinematic())
        {
            m_cinematic->StartCinematic();
        }
        else
        {
            m_cinematic->LoadDefaults();
            SetCursorShow(true);
        }
        result = true;
        break;
    }

    default:
        result = true;
        break;
    }

    return result;
}

m3d::Class* CMiracle3d::GetBaseClass()
{
    return RT_CLASS_LOCAL(Application);
}

m3d::TownMusicManager* CMiracle3d::GetTownMusicManager()
{
    return m_townMusicManager;
}

bool CMiracle3d::AddPostEffect(CStr const& effectName, float effParam)
{
    return this->m_postEffect->AddEffect(effectName, effParam, 0);
}

int CMiracle3d::HandleCinematic(float dT)
{
    if (m_cinematic->bCanUpdate())
    {
        m_cinematic->Update(m_curCamera, dT);
    }
    m_cinematic->m_playTime += dT * 1000;

    while (true)
    {
        switch (this->m_cinematic->m_state)
        {
        case m3d::CINEMATIC_ENTER_FADE_OUT:
        case m3d::CINEMATIC_ENTER_FADE_IN:
        case m3d::CINEMATIC_EXIT_FADE_OUT:
        case m3d::CINEMATIC_EXIT_FADE_IN:
        {
            if (!CinematicFade())
                break;
            return 1;

        case m3d::CINEMATIC_IS_PLAYING:
            // TODO: check this
            ai::pServer->PostPlayerEvent(ai::GE_IN_CINEMATIC);
            auto fadePeriod = this->m_cinematic->GetFadePeriodForState(m3d::CINEMATIC_EXIT_FADE_OUT);

            auto timeToShowDlg = 0.0;
            if (!m_cinematic->InPlay() || m_cinematic->GetTimeToTheEnd() >= 0.0)
            {
                timeToShowDlg = m_cinematic->GetTimeToTheEnd();
            }
            auto cinemaPanel = GetCinemaPanel();
            if (m_cinematic->bWaitWhenStop() || fadePeriod < timeToShowDlg || cinemaPanel && (cinemaPanel->HasMsg()))
            {
                return 1;
            }

            CinematicInterrupt();
            break;
        }
        case m3d::CINEMATIC_NOT_INITED:
            return 0;

        default:
            return 1;
        }
    }
    return 0;
}

m3d::Object* CMiracle3d::CreateObject()
{
    // RVA 0x414DC0
    return new CMiracle3d;
}

void CMiracle3d::OnChangeProfile()
{
    // RVA 0x419470 - apply the newly selected profile's input settings.
    auto* profile = m_profileManager->GetCurProfile();
    if (!profile)
    {
        return;
    }

    m_pImpulses->LoadFromProfile();

    m3d::AIParam mouseSensitivity;
    if (profile->GetParam(PP_MOUSE_SENSITIVITY, mouseSensitivity))
    {
        SetMouseSensitivity(mouseSensitivity.GetAsFloat());
    }
    m3d::AIParam mouseYAxisFlip;
    if (profile->GetParam(PP_MOUSE_YAXIS_FLIP, mouseYAxisFlip))
    {
        SetMouseYAxisFlipped(mouseYAxisFlip.GetAsID() != 0);
    }
    m3d::AIParam mouseXAxisFlip;
    if (profile->GetParam(PP_MOUSE_XAXIS_FLIP, mouseXAxisFlip))
    {
        SetMouseXAxisFlipped(mouseXAxisFlip.GetAsID() != 0);
    }
    m3d::AIParam langParam;
    if (profile->GetParam(PP_INPUT_LANGUAGE, langParam))
    {
        auto const language = static_cast<unsigned>(langParam.GetAsID());
        if (language <= 1 && m_input)
        {
            m_input->SetLanguage(static_cast<m3d::input::Language>(language));
        }
    }
}

void CMiracle3d::CleanMainMenuLevel()
{
    // RVA 0x404300 - the same teardown as CleanLevel(false, true), skipped
    // entirely when no main-menu level was loaded.
    if (m_bDoNotLoadMainmenuLevel)
    {
        return;
    }

    ClearViewportToBlack();
    CinematicClear();
    help::DeleteAllFilesInDirectory(m_pInterfaceManager->GetSavesManager()->GetPathForTemporaryMaps().c_str());
    if (m3d::pClient)
    {
        ProcessAllEvents();
        m_pInterfaceManager->ShowWindow(166, false, false, false, false, nullptr);
        m_pInterfaceManager->LaunchEvent(86, GUI_EVENT_CUSTOM, nullptr);
        ai::pServer->Clear();
        ai::pServer->ClearOnce();
        m3d::pClient->Reset();
        m3d::pClient->GetWorld().Release();
        DiscardAllEvents();
        m_pImpulses->ResetAllImpulses(true);
        m_bRenderAsBackground = false;
        m_bBackgroundTextureIsValid = false;
    }
}

void CMiracle3d::BeginModalDlg(bool forcePause)
{
    // RVA 0x401250
    if (forcePause)
    {
        Pause();
    }
    M3D_APP->m_pImpulses->ResetAllImpulses(false);
    m_gameSlideAuto = CVector(0.0f, 0.0f, 0.0f);
}

m3d::ui::MbRetCodes CMiracle3d::RunMsgBoxDlg(CStr const& caption, CStr const& message, unsigned flags, bool bPause)
{
    return M3D_APP->m_pInterfaceManager->RunMsgBoxDlg(caption, message, flags, bPause);
}

int CMiracle3d::CleanLevel(bool beforeContinuousMap, bool releaseWorld)
{
    ClearViewportToBlack();
    CinematicClear();
    if (beforeContinuousMap && releaseWorld)
    {
        // RVA 0x417BA0 - moving on to a connected map: keep the current map's
        // state among the temporary maps so it can be returned to.
        CStr const levelName = help::GetMapNameFromFileName(CStr(M3D_ENGINE_CFG.m_levFileName.GetS()));
        ai::pServer->SaveVisitedMap(
            M3D_APP->m_pInterfaceManager->GetSavesManager()->GetPathForTemporaryMaps() + levelName + CStr(".xml"));
    }
    else
    {
        auto* savesManager = M3D_APP->m_pInterfaceManager->GetSavesManager();
        auto path = savesManager->GetPathForTemporaryMaps();
        help::DeleteAllFilesInDirectory(path.c_str());
    }

    if (m3d::pClient)
    {
        ProcessAllEvents();
        M3D_APP->m_pInterfaceManager->ShowWindow(166, false, false, false, false, nullptr);
        if (beforeContinuousMap && releaseWorld)
        {
            M3D_APP->m_pInterfaceManager->LaunchEvent(87, GUI_EVENT_CUSTOM, nullptr);
        }
        else
        {
            M3D_APP->m_pInterfaceManager->LaunchEvent(86, GUI_EVENT_CUSTOM, nullptr);
        }

        ai::pServer->Clear();
        if (!beforeContinuousMap)
        {
            ai::pServer->ClearOnce();
        }
        m3d::pClient->Reset();
        if (releaseWorld)
        {
            m3d::pClient->GetWorld().Release();
        }
        DiscardAllEvents();
        M3D_APP->m_pImpulses->ResetAllImpulses(true);

        m_bRenderAsBackground = false;
        m_bBackgroundTextureIsValid = false;
    }
    return true;
}

int CMiracle3d::OnGameZoom(m3d::AuxImpulseInfo const& impInfo)
{
    // RVA 0x403030
    if (impInfo.m_state && m_player.m_cameraMode == CM_FOLLOWMODE)
    {
        auto const wheel = impInfo.UnpackWheel();
        m_gameCameraRho -= wheel;
        m_player.m_desiredDistance -= wheel;
    }
    return 1;
}

void CMiracle3d::PutSplash(int proc, char const* text)
{
    m_pInterfaceManager->ShowSplash(proc, text);
}

bool CMiracle3d::bIsMousePointing() const
{
    return m_curGameMode.m_mode == GS_GAME;
}

void CMiracle3d::ClearSomeGameElementsBeforeModal()
{
    // RVA 0x4012A0
    M3D_APP->m_pImpulses->ResetAllImpulses(false);
    m_gameSlideAuto = CVector(0.0f, 0.0f, 0.0f);
}

void CMiracle3d::ChangeLanguage()
{
    // RVA 0x419610
    Application::ChangeLanguage();
    auto* profile = m_profileManager->GetCurProfile();
    if (profile && m_input)
    {
        // An ID-typed parameter holding the language index.
        m3d::AIParam paramVal(static_cast<int>(m_input->GetLanguage()));
        profile->SetParam(PP_INPUT_LANGUAGE, paramVal);
    }
}

float CMiracle3d::getZoom()
{
    if (!this->zoomInited)
    {
        this->m_Fov0 = m_fov.GetF();
        this->zoomInited = 1;
    }

    return m_fov.GetF() / this->m_Fov0;
}

int CMiracle3d::CinematicClear()
{
    if (m_cinematic->m_state != m3d::CINEMATIC_NOT_INITED)
    {
        m_cinematic->Stop();
        if (auto panel = GetCinemaPanel())
        {
            panel->OnHide();
        }
        auto app = dynamic_cast<CMiracle3d*>(g_pApp);
        app->m_pInterfaceManager->ShowWindow(19, false, false, false, false, nullptr);
        app->m_pInterfaceManager->Show(!m_bGuiWasHiddenBeforeCinematic, true);
        GetStation()->SetCursorShow(true);
        m3d::g_Kernel->GetEngineCfg().m_FogOfWar.SetI(1);
        if ((m_cinematic->GetCurItem().m_flags & 2) != 0)
        {
            ai::pServer->PostPlayerEvent(ai::GE_END_CINEMATIC);
        }
        m_cinematic->LoadDefaults();
    }
    return 1;
}

void CMiracle3d::OnBeforeDeviceReset()
{
    // RVA 0x417170 - the captured background does not survive a device reset.
    m_bBackgroundTextureIsValid = false;
}

void CMiracle3d::CinematicInterrupt()
{
    if (m_cinematic->m_state)
    {
        auto v5 = m_cinematic->m_state - 1;
        if (v5)
        {
            if (v5 == 1)
            {
                auto playTime = m_cinematic->m_playTime;
                m_cinematic->m_fadeStartTime = (int)playTime;
                m_cinematic->m_state = m3d::CINEMATIC_EXIT_FADE_OUT;
                auto cinemaFadePanel = GetCinemaFadePanel();
                if (cinemaFadePanel)
                    cinemaFadePanel->AttachToScreenCinematicRelated();
            }
        }
        else
        {
            auto v11 = m_cinematic->m_playTime - m_cinematic->m_fadeStartTime;
            m_cinematic->m_state = m3d::CINEMATIC_EXIT_FADE_OUT;
            m_cinematic->m_fadeStartTime = v11 + m_cinematic->m_playTime -
                (int)(m_cinematic->GetFadePeriodForState(m3d::CINEMATIC_ENTER_FADE_IN) * 1000.0);
        }
    }
    else
    {
        m_cinematic->m_bWasSkippedInEnterFadeOut = 1;
    }
}

void CMiracle3d::setZoom(float zoom)
{
    if (!this->zoomInited)
    {
        auto m_f = m_fov.GetF();
        this->m_Fov0 = m_f;
        this->zoomInited = 1;
    }
    setFov(this->m_Fov0 / zoom);
}

void CMiracle3d::OnAfterDeviceReset()
{
    // RVA 0x417160 - nothing to do.
}

int CMiracle3d::OnSkipCinematic(m3d::AuxImpulseInfo const& impInfo)
{
    if (impInfo.m_state)
    {
        this->m_cinematic->m_bWasSkipped = true;
        m3d::AuxImpulseInfo info(3, 1, this->m_curGameMode.m_mode, 0, 0);
        OnChangeMode(info);
    }
    return 1;
}

void CMiracle3d::RenderAsBackground(bool bAsBackground)
{
    m_bRenderAsBackground = bAsBackground;
    m_bBackgroundTextureIsValid = false;
}

void CMiracle3d::UpdateCameraPosition(ai::PhysicObj* trackedObj)
{
    // RVA 0x401B40
    float const tlen = static_cast<float>(M3D_KERNEL->GetTimer().GetLastFrameTime()) * 0.001f;

    switch (m_player.m_cameraMode)
    {
    case CM_BUMPER:
    {
        if (!trackedObj || trackedObj->GetClass() != &ai::Vehicle::m_classVehicle)
        {
            break;
        }
        auto* vehicle = static_cast<ai::Vehicle*>(trackedObj);

        CVector const oldorg = m_curCamera.m_worldOrigin;
        CVector const org = vehicle->GetPositionAtRelPoint(vehicle->GetBumperPoint());

        // Chase the bumper point rather than snapping to it, and never let the
        // lag grow past two units.
        float const k = tlen * 5.0f;
        CVector lag;
        lag.x = (org.x - oldorg.x) * k + oldorg.x - org.x;
        lag.y = (org.y - oldorg.y) * k + oldorg.y - org.y;
        lag.z = (org.z - oldorg.z) * k + oldorg.z - org.z;
        CVector const move = lag.clampLength(2.0f);

        m_curCamera.m_worldOrigin.x = org.x + move.x;
        m_curCamera.m_worldOrigin.y = move.y + org.y;
        m_curCamera.m_worldOrigin.z = move.z + org.z;

        // The orientation lags the same way. rotTranslate with a zero origin is
        // the quaternion to matrix conversion the original inlines here.
        m_player.m_lastobjQuat = ::SLerp(m_player.m_lastobjQuat, vehicle->GetRotation(), k);

        CMatrix rot;
        rot.rotTranslate(m_player.m_lastobjQuat, CVector(0.0f, 0.0f, 0.0f));
        rot.getTransposed().getYPR(m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);
        break;
    }

    case CM_FOLLOWMODE:
    {
        if (!trackedObj)
        {
            break;
        }
        auto* vehicle = static_cast<ai::Vehicle*>(trackedObj);

        // NOTE: the velocity is fetched and then never used - the shipped code
        // does the call anyway.
        CVector const vel = vehicle->GetLinearVelocity();
        (void)vel;

        float const maxDist = vehicle->GetCameraMaxDist();
        if (m_gameCameraRho < 0.0f)
        {
            m_gameCameraRho = 0.0f;
        }
        if (m_gameCameraRho > maxDist)
        {
            m_gameCameraRho = maxDist;
        }
        float const maxDist2 = vehicle->GetCameraMaxDist();
        if (m_player.m_desiredDistance < 0.0f)
        {
            m_player.m_desiredDistance = 0.0f;
        }
        if (m_player.m_desiredDistance > maxDist2)
        {
            m_player.m_desiredDistance = maxDist2;
        }

        CMatrix sightLine;
        sightLine.rotYPR(m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);

        // The camera offset: the manual turn offset, pushed back along the
        // sight line by the current follow distance.
        CVector const turn(m_flyCamTurn.x, m_flyCamTurn.y, m_flyCamTurn.z - m_gameCameraRho);
        CVector back;
        back.x = (sightLine._11 * turn.x + sightLine._13 * turn.z) + sightLine._12 * turn.y;
        back.y = (sightLine._21 * turn.x + sightLine._23 * turn.z) + sightLine._22 * turn.y;
        back.z = (sightLine._31 * turn.x + sightLine._33 * turn.z) + sightLine._32 * turn.y;

        // Plus whatever the automatic slide is contributing this frame.
        CVector const slide(m_gameSlideAuto.x * tlen, m_gameSlideAuto.y * tlen, m_gameSlideAuto.z * tlen);
        CVector auto_;
        auto_.x = (slide.x * sightLine._11 + slide.z * sightLine._13) + slide.y * sightLine._12;
        auto_.y = (sightLine._21 * slide.x + slide.z * sightLine._23) + slide.y * sightLine._22;
        auto_.z = (sightLine._31 * slide.x + slide.z * sightLine._33) + slide.y * sightLine._32;

        float const camHeight = vehicle->GetCameraHeight();
        CVector const objPos = vehicle->GetPosition();

        m_curCamera.m_worldOrigin.x = (objPos.x + auto_.x) + back.x;
        m_curCamera.m_worldOrigin.y = ((objPos.y + camHeight) + auto_.y) + back.y;
        m_curCamera.m_worldOrigin.z = (objPos.z + auto_.z) + back.z;

        CVector lookAtPoint;
        lookAtPoint.x = objPos.x;
        lookAtPoint.y = objPos.y + camHeight;
        lookAtPoint.z = objPos.z;

        CVector toTarget;
        toTarget.x = lookAtPoint.x - m_curCamera.m_worldOrigin.x;
        toTarget.y = lookAtPoint.y - m_curCamera.m_worldOrigin.y;
        toTarget.z = lookAtPoint.z - m_curCamera.m_worldOrigin.z;

        CollideCamera(m_curCamera.m_worldOrigin, m_gameCameraRho, toTarget, toTarget);
        m_curCamera.lookAt(lookAtPoint);
        break;
    }

    case CM_FLYCAMERA:
    {
        CMatrix sightLine;
        sightLine.rotYPR(m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);

        CVector const local(
            m_flyCamMove.x + m_gameSlideAuto.x * tlen,
            m_flyCamMove.y + m_gameSlideAuto.y * tlen,
            m_flyCamMove.z + m_gameSlideAuto.z * tlen);

        CVector move;
        move.x = (sightLine._11 * local.x + sightLine._13 * local.z) + sightLine._12 * local.y;
        move.y = (sightLine._21 * local.x + sightLine._23 * local.z) + sightLine._22 * local.y;
        move.z = (sightLine._33 * local.z + sightLine._32 * local.y) + sightLine._31 * local.x;

        m_curCamera.m_worldOrigin.x = m_curCamera.m_worldOrigin.x + move.x;
        m_curCamera.m_worldOrigin.y = m_curCamera.m_worldOrigin.y + move.y;
        m_curCamera.m_worldOrigin.z = m_curCamera.m_worldOrigin.z + move.z;
        break;
    }

    default:
        break;
    }
}

float CMiracle3d::GetMaxTimeScale() const
{
    // RVA 0x40C500
    return m_maxTimeScale;
}

void CMiracle3d::SetMaxTimeScale(float value)
{
    // RVA 0x40C4F0
    m_maxTimeScale = value;
}

int CMiracle3d::OnObtainingFocus()
{
    if (M3D_APP->m_pInterfaceManager->IsWindowVisible(72))
    {
        auto wnd = M3D_APP->m_pInterfaceManager->GetWindow(72);
        CaptureFocus(wnd);
    }
    else
    {
        m_gotFocus = true;
    }
    return 1;
}

int CMiracle3d::LoadLevel(
    CStr const& name,
    CStr const&,
    bool bQuiet,
    bool LoadServers,
    bool bContinuousMap,
    m3d::cmn::XmlFile* dynamicSceneXmlFile,
    m3d::cmn::XmlNode const* dynamicSceneXmlNode,
    ai::ObjContainer::eSAVE_TYPES saveType)
{
    // RVA 0x418100 - loads the level's world (unless LoadServers is false) and then its dynamic
    // scene: from the given XML, or - when continuing onto a map visited before - from the copy
    // saved on leaving it. saveDir is unused (the optimised build does not even pass it).
    if (!m_gameInited)
    {
        return 0;
    }
    M3D_LOG_INFO(CStr("-- Loading Level: ") + name + CStr(" --"));
    if (!bContinuousMap)
    {
        ai::pServer->InitOnce();
    }
    if (LoadServers && !m3d::pClient->GetWorld().Load(name, m_curCamera, bQuiet))
    {
        M3D_LOG_INFO(CStr((CStr("Level file ") + name + CStr(" not found")).c_str()));
        EnqueueMessage(1, 0, 0, 0, 0, {}, {});
        return 0;
    }
    m_blockMusicManager->Init();
    // The binary reads these through g_pApp, which is this application.
    auto* app = static_cast<CMiracle3d*>(g_pApp);
    if (!dynamicSceneXmlFile)
    {
        app->m_serverAnimatedModels->GenerateImpostorsIfNeeded();
    }

    M3D_LOG_INFO(CStr("Load Server begin..."));
    m_cinematic->SetFolder(m3d::pClient->GetWorld().m_level->GetFullPathNameA(CStr("")).c_str());
    app->m_pInterfaceManager->LaunchEvent(84, GUI_EVENT_CUSTOM, nullptr);
    if (bContinuousMap)
    {
        // A map left earlier in this game was saved to the temporary maps folder.
        CStr const mapName = help::GetMapNameFromFileName(name);
        CStr const ext(".xml");
        CStr const visitedMap =
            app->m_pInterfaceManager->GetSavesManager()->GetPathForTemporaryMaps() + mapName + ext;
        DWORD const attr = GetFileAttributesA(visitedMap.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
        {
            ai::pServer->Load(ai::LOCAL_GAME, dynamicSceneXmlFile, dynamicSceneXmlNode, bContinuousMap, saveType);
        }
        else
        {
            ai::pServer->LoadVisitedMap(visitedMap, bContinuousMap);
        }
    }
    else
    {
        ai::pServer->Load(ai::LOCAL_GAME, dynamicSceneXmlFile, dynamicSceneXmlNode, false, saveType);
    }
    M3D_LOG_INFO(CStr("Load server end"));

    if (!bContinuousMap)
    {
        app->m_pInterfaceManager->GetQuestInfoManager()->Init();
    }
    int continuous = bContinuousMap ? 1 : 0;
    app->m_pInterfaceManager->LaunchEvent(85, GUI_EVENT_CUSTOM, &continuous);

    if (ai::gDynamicScene->GetVehicleControlledByPlayer())
    {
        m_curCamera.m_worldOrigin = ai::gDynamicScene->GetVehicleControlledByPlayer()->GetPosition();
    }
    m_blockMusicManager->Reset();
    m3d::g_Kernel->GetTimer().SetActiveState(1);
    if (app->AppActive() && m3d::g_Kernel->GetEngineCfg().m_clipCursorWithinRenderWnd.GetB())
    {
        CaptureAndClipSystemCursor(true);
    }
    m_bRenderAsBackground = false;
    m_bBackgroundTextureIsValid = false;
    return 1;
}

void CMiracle3d::FullSystyemAndUserUnpause()
{
    // RVA 0x402420
    m3d::g_Kernel->GetTimer().SetTimeScale(m_normalTimeScale);
    m_paused = false;
    m_userPaused = false;
    m_saveTimeScale = 0.0f;
    if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
    {
        M3D_APP->m_sound->PauseGroup(2, false);
    }
}

bool CMiracle3d::GetPostEffectParam(CStr const& effectName, float& effParam)
{
    // RVA 0x4159C0
    return m_postEffect->GetParam(effectName, effParam);
}

bool CMiracle3d::SetPostEffectParam(CStr const& effectName, float effParam)
{
    // RVA 0x4159D0
    return m_postEffect->SetParam(effectName, effParam);
}

//Verified: CMiracle3d::StartMainMenu
void CMiracle3d::StartMainMenu()
{
    if (m_profileManager)
    {
        m_profileManager->Init();
        OnChangeMode(m3d::AuxImpulseInfo{1, true, -1, 0, 0});
    }
    else
    {
        M3D_LOG_INFO("CMiracle3d::InitMedia error - cannot instantiate ProfileManager object");
    }
}

void CMiracle3d::SetMouseSensitivity(float sensitivity)
{
    // RVA 0x4192D0
    Application::SetMouseSensitivity(sensitivity);
    auto* profile = m_profileManager->GetCurProfile();
    if (profile)
    {
        m3d::AIParam paramVal(GetMouseSensitivity());
        profile->SetParam(PP_MOUSE_SENSITIVITY, paramVal);
    }
}

void CMiracle3d::UpdateCinematicCameraRotation()
{
    // RVA 0x41EFA0
    m_cinematic->UpdateCameraRotation(m_curCamera);
}

void CMiracle3d::SetCurHackedMusicType(HackedMusicType musicType)
{
    if (m_hackedMusicType != musicType)
    {
        m_bMustStartNewMusic = 1;
        m_hackedMusicType = musicType;
        if (musicType == HACKMUSIC_GAME)
        {
            m_blockMusicManager->Reset();
        }
    }
}

HackedMusicType CMiracle3d::GetCurHackedMusicType() const
{
    // RVA 0x41DDC0
    return m_hackedMusicType;
}

int CMiracle3d::ValidateCameraOrigin(bool)
{
    // RVA 0x402650
    return 1;
}

void CMiracle3d::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    // RVA 0x418F40
    Application::LoadFromXml(xmlFile, xmlNode);

    ref_ptr playerNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    xmlNode->GetFirstChild(playerNode, "Player");
    m_player.LoadFromXml(xmlFile, playerNode);

    auto mode = m_curGameMode.m_mode;
    if (!xmlNode->IsEmpty())
    {
        if (auto const* attr = xmlNode->GetAttribute("CurGameMode"))
        {
            mode = static_cast<GameState>(atoi(attr));
        }
    }
    m_curGameMode.Set(mode);
}

int CMiracle3d::OnGameSwitchCamera(m3d::AuxImpulseInfo const& impInfo)
{
    // RVA 0x4034F0 - cycles follow -> bumper -> fly -> follow.
    if (!impInfo.m_state || !M3D_ENGINE_CFG.m_g_switchCameraAllow.GetB())
    {
        return 1;
    }

    switch (m_player.m_cameraMode)
    {
    case CM_BUMPER:
    {
        CVector aim = m_curCamera.m_worldOrigin;
        aim.x += 1.0f;
        m_curCamera.lookAt(aim);
        m_player.m_cameraMode = CM_FLYCAMERA;
        break;
    }
    case CM_FOLLOWMODE:
        m_player.m_cameraMode = CM_BUMPER;
        break;
    case CM_FLYCAMERA:
    case CM_CONST:
        m_player.m_cameraMode = CM_FOLLOWMODE;
        break;
    default:
        M3D_LOG_ERR("Error: invalid camera mode: " + CStr(static_cast<int>(m_player.m_cameraMode)));
        break;
    }
    return 1;
}

int CMiracle3d::OnGameMouse(m3d::AuxImpulseInfo const& impInfo)
{
    m_gameSlideAuto = ZeroVector;

    float x = 0.0;
    float y = 0.0;
    float dx = 0.0;
    float dy = 0.0;
    impInfo.UnpackXy(&x, &y, &dx, &dy);

    int res = 0;
    if (m_curGameMode.Get() == GS_GAME)
    {
        auto const cameraMode = m_player.m_cameraMode;
        if (cameraMode == CM_FOLLOWMODE || cameraMode == CM_FLYCAMERA)
        {
            res = 1;
        }
    }

    if (M3D_APP->m_pImpulses->GetImpulseState(9) || !HasChildModalRunning() && res)
    {
        if (GetCapture() != this)
        {
            auto const mouseSense = GetMouseSensitivity();
            m_flyCamTurn.x = dx * mouseSense * 0.003;
            m_flyCamTurn.y = dy * mouseSense * 0.003;
            if (IsMouseYAxisFlipped())
            {
                m_flyCamTurn.y = 0.0 - m_flyCamTurn.y;
            }
            if (IsMouseXAxisFlipped())
            {
                m_flyCamTurn.x = 0.0 - m_flyCamTurn.x;
            }

            float x = 512.0;
            float y = 384.0;
            M3D_RENDERER->RelToAbs(x, y);
            SetMouseXy(x, y);
        }
    }
    return 1;
}

void CMiracle3d::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
{
    // RVA 0x419000
    Application::SaveToXml(xmlFile, xmlNode);

    ref_ptr playerNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Player");
    xmlNode->AddChild(playerNode);
    m_player.SaveToXml(xmlFile, playerNode);

    xmlNode->SetAttribute("CurGameMode", CStr(static_cast<int>(m_curGameMode.m_mode)).c_str());
    xmlNode->SetAttribute("Paused", CStr(static_cast<int>(m_paused)).c_str());
}

void CMiracle3d::EndModalDlg()
{
    // RVA 0x401290
    UnPause();
}

bool CMiracle3d::CanLaunchIfaceWindow()
{
    // RVA 0x4188E0
    return !M3D_APP->GetStation()->HasChildModalRunning();
}

int CMiracle3d::LoadMainMenuLevel()
{
    if (m_bDoNotLoadMainmenuLevel)
    {
        return 0;
    }
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    CStr mapName = m3d::g_Kernel->GetEngineCfg().m_mainMenuLevelName.GetS();
    if (stream->Open(mapName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        stream->Close();
        if (!m_gameInited && !GameInit())
        {
            M3D_LOG_INFO("Failed to GameInit() when loading main menu level...");
            m_bDoNotLoadMainmenuLevel = true;
            return 0;
        }
        auto app = dynamic_cast<CMiracle3d*>(g_pApp);
        app->m_pInterfaceManager->StartSplashing(11);
        //TODO: check this
        auto res = LoadLevel(mapName, {}, false, true, false, nullptr, nullptr, ai::ObjContainer::SAVE_LEVEL);
        if (res == 0)
        {
            M3D_LOG_INFO("Could not load main menu level...");
            m_bDoNotLoadMainmenuLevel = true;
            return 0;
        }
        m3d::g_Kernel->GetEngineCfg().m_levFileName.Set(mapName.c_str());
        return 1;
    }
    M3D_LOG_INFO("Could not find main menu level...");
    m_bDoNotLoadMainmenuLevel = true;
    return 0;
}

int CMiracle3d::OnDebug(m3d::AuxImpulseInfo const& impInfo)
{
    // RVA 0x4012D0 - IM_DEBUG_0..9 (12..21) latch a server key; IM_DEBUG_WIREFRAME
    // (22) toggles the landscape draw mode in debug mode.
    if (impInfo.m_state)
    {
        if (impInfo.m_impId != 22)
        {
            m_srvKeys[impInfo.m_impId - 12] = true;
        }
        if (impInfo.m_impId == 22 && M3D_ENGINE_CFG.m_debugMode.GetB())
        {
            m3d::pClient->GetWorld().GetLandscape().SwitchDrawMode();
        }
    }
    return 1;
}

bool CMiracle3d::LoadMapFromConsole(m3d::CConsoleParams const& params, bool isContinuousMap)
{
    if (params.NumOfTokens(32) == 2)
    {
        M3D_APP->m_pInterfaceManager->StartSplashing(11);
        CStr mapName = params.UnsafeStringToken(1, 32);

        auto res = LoadMap(mapName, isContinuousMap, nullptr, nullptr, ai::ObjContainer::SAVE_LEVEL);
        return res;
    }
    CStr usageString;
    if (isContinuousMap)
    {
        usageString = "Usage: /map <map_name>\n";
    }
    else
    {
        usageString = "Usage: /nextmap <map_name>\n";
    }

    M3D_ENGINE_CFG.m_console->PrintF(usageString);
    return false;
}

int CMiracle3d::StartPlayingVideo(char const* videoFile, int (CMiracle3d::*onFinishCallback)())
{
    //TODO: check this!!!
    CStr file = videoFile;
    if (g_pApp->m_sound && !file.empty())
    {
        g_pApp->m_sound->PauseAllSounds(true);
    }

    int res = 0;

    if (M3dVideoPlayer->IsVideoPlaing())
    {
        M3D_LOG_INFO("Warning: video is already playing");
        res = 1;
    }
    else
    {
        //TODO; check this
        g_pApp->ClearViewportToBlack();
        if (file.empty() || M3dVideoPlayer->Play(videoFile))
        {
            res = 0;
            m_playingVideo = true;
            m_onFinishVideoPlaying = onFinishCallback;
            return res;
        }
        M3D_LOG_INFO("Error: couldn't load video '" + file);
        res = 2;
    }
    if (onFinishCallback)
    {
        (this->*onFinishCallback)();
    }
    return res;
}

CMiracle3d::~CMiracle3d()
{
    // RVA 0x4154F0 - only the members are released.
}

float CMiracle3d::GetNormalTimeScale() const
{
    // RVA 0x40C4C0
    return m_normalTimeScale;
}

void CMiracle3d::SetNormalTimeScale(float value)
{
    // RVA 0x40C4B0
    m_normalTimeScale = value;
}

bool CMiracle3d::SaveGame(CStr const& saveDir, bool bQuiet)
{
    // RVA 0x421630
    if (CStr(M3D_ENGINE_CFG.m_levFileName.GetS()).empty())
    {
        M3D_ENGINE_CFG.m_console->PrintF("Nothing to save");
        return false;
    }

    if (!bQuiet)
    {
        m_pInterfaceManager->StartSplashing(1);
        PutSplash(0, GetStringByStringId0("GameSaving").c_str());
    }
    M3D_LOG_INFO("Begin saving game '" + saveDir + "'...");

    CStr const tmpMapsDir = m_pInterfaceManager->GetSavesManager()->GetPathForTemporaryMaps();
    help::CopyDirectory(tmpMapsDir.c_str(), saveDir.c_str());
    if (!bQuiet)
    {
        PutSplash(5, GetStringByStringId0("GameSaving").c_str());
    }

    CStr const fileName = saveDir + "\\currentmap.xml";
    scoped_ptr stream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_WRITE))
    {
        M3D_ENGINE_CFG.m_console->PrintF("File " + fileName + " couldn't be open for writing\n");
        return false;
    }

    if (!bQuiet)
    {
        CaptureMouse(this);
    }
    m3d::g_Kernel->GetTimer().SetActiveState(1);

    ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
    ref_ptr ndGeneral = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "General");
    xmlFile->AddChild(ndGeneral);
    ndGeneral->SetAttribute(
        "MapName", help::GetMapNameFromFileName(CStr(M3D_ENGINE_CFG.m_levFileName.GetS())).c_str());
    m_pInterfaceManager->Save(xmlFile, ndGeneral);
    if (!bQuiet)
    {
        PutSplash(15, GetStringByStringId0("GameSaving").c_str());
    }

    ref_ptr ndDynamicScene = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "DynamicScene");
    ndGeneral->AddChild(ndDynamicScene);
    auto const saveType = ai::theObjects->m_SaveType;
    ai::theObjects->m_SaveType = ai::ObjContainer::SAVE_FULL;
    M3D_LOG_INFO("\tSaving Dynamic scene...");
    bool const res = ai::gDynamicScene->SaveSceneToXml(xmlFile, ndDynamicScene);
    M3D_LOG_INFO("\tDynamic scene saved.");
    ai::theObjects->m_SaveType = saveType;
    if (!bQuiet)
    {
        PutSplash(65, GetStringByStringId0("GameSaving").c_str());
    }

    ref_ptr ndQuestManager = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "QuestManager");
    ndGeneral->AddChild(ndQuestManager);
    M3D_LOG_INFO("\tSaving quest states...");
    ai::theQuestStateManager->SaveToXml(xmlFile, ndQuestManager);
    M3D_LOG_INFO("\tQuest states saved.");
    if (!bQuiet)
    {
        PutSplash(75, GetStringByStringId0("GameSaving").c_str());
    }

    ref_ptr ndApplication = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Application");
    ndGeneral->AddChild(ndApplication);
    SaveToXml(xmlFile, ndApplication);
    if (!bQuiet)
    {
        PutSplash(80, GetStringByStringId0("GameSaving").c_str());
    }

    ref_ptr ndGameTime = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "GameTime");
    ndGeneral->AddChild(ndGameTime);
    M3D_LOG_INFO("\tSaving Game Time...");
    ai::theObjects->m_GameTime.SaveToXML(xmlFile, ndGameTime);
    M3D_LOG_INFO("\tGame Time saved.");
    if (!bQuiet)
    {
        PutSplash(82, GetStringByStringId0("GameSaving").c_str());
    }

    ref_ptr ndWeatherState = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "WeatherState");
    ndGeneral->AddChild(ndWeatherState);
    M3D_LOG_INFO("\tSaving Weather State...");
    m3d::pClient->GetWorld().GetWeatherManager().SaveWeatherStateToXMLNode(xmlFile, ndWeatherState);
    M3D_LOG_INFO("\tWeather State saved.");
    if (!bQuiet)
    {
        PutSplash(84, GetStringByStringId0("GameSaving").c_str());
    }

    ref_ptr ndProcessManager = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "ProcessManager");
    ndGeneral->AddChild(ndProcessManager);
    M3D_LOG_INFO("\tSaving ProcessManager...");
    ai::theProcessManager->SaveToXML(xmlFile, ndProcessManager);
    M3D_LOG_INFO("\tProcessManager saved.");
    if (!bQuiet)
    {
        PutSplash(86, GetStringByStringId0("GameSaving").c_str());
    }

    ref_ptr ndStatistics = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Statistics");
    ndGeneral->AddChild(ndStatistics);
    M3D_LOG_INFO("\tSaving statistics...");
    ai::theStatisticManager->SaveToXml(xmlFile, ndStatistics);
    M3D_LOG_INFO("\tStatistics saved.");
    if (!bQuiet)
    {
        PutSplash(88, GetStringByStringId0("GameSaving").c_str());
    }

    xmlFile->Write(*stream);
    if (!bQuiet)
    {
        PutSplash(100, GetStringByStringId0("GameSaving").c_str());
    }
    stream->Close();
    M3D_LOG_INFO("Game '" + fileName + "' saved.");

    m3d::g_Kernel->GetTimer().SetActiveState(1);
    if (AppActive() && M3D_ENGINE_CFG.m_clipCursorWithinRenderWnd.GetB())
    {
        CaptureAndClipSystemCursor(true);
    }
    return res;
}

bool CMiracle3d::LoadMap(
    CStr const& mapname,
    bool isContinuousMap,
    m3d::cmn::XmlFile* dynamicSceneXmlFile,
    m3d::cmn::XmlNode const* dynamicSceneXmlNode,
    ai::ObjContainer::eSAVE_TYPES saveType)
{
    CStr fullMapName = mapname;
    auto strres = strstr(mapname.c_str(), ".ssl");
    if (!strres || strres - mapname.c_str() == -1)
    {
        fullMapName = "data\\maps\\" + fullMapName + ".ssl";
    }

    scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
    if (!stream->Open(fullMapName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        CStr error = "Map " + fullMapName + " not found\n";
        M3D_ENGINE_CFG.m_console->PrintF(error);
        return false;
    }
    stream->Close();

    m_curGameMode.Set(GS_GAME);
    M3D_APP->m_pInterfaceManager->ShowWindow(72, false, true, false, false, nullptr);
    CaptureMouse(this);

    if (!m_gameInited)
    {
        GameInit();
    }

    if (m_gameInited)
    {
        CleanLevel(isContinuousMap, true);
        LoadLevel(fullMapName, {}, false, true, isContinuousMap, dynamicSceneXmlFile, dynamicSceneXmlNode, saveType);
    }

    if (M3D_ENGINE_CFG.m_mus_Enable.GetB())
    {
        if (M3D_APP->m_hackedMusicType != HACKMUSIC_GAME)
        {
            M3D_APP->m_bMustStartNewMusic = true;
            M3D_APP->m_hackedMusicType = HACKMUSIC_GAME;
        }
    }

    M3D_ENGINE_CFG.m_console->Toggle(false);
    SetKeyboardFocus(this);
    return true;
}

int CMiracle3d::ValidateCameraAngles()
{
    if (m_curCamera.m_rotYaw > 3.1415927)
        m_curCamera.m_rotYaw = m_curCamera.m_rotYaw -
            (float)((float)(int)(float)((float)(m_curCamera.m_rotYaw + 3.1415927) * 0.15915494) * 6.2831855);
    if (m_curCamera.m_rotYaw < -3.1415927)
        m_curCamera.m_rotYaw =
            (float)((float)(int)(float)((float)(3.1415927 - m_curCamera.m_rotYaw) * 0.15915494) * 6.2831855) +
            m_curCamera.m_rotYaw;

    auto v2 = m_curCamera.m_rotPitch;
    if (v2 > 3.1415927)
        m_curCamera.m_rotPitch =
            m_curCamera.m_rotPitch - (float)((float)(int)(float)((float)(v2 + 3.1415927) * 0.15915494) * 6.2831855);
    if (m_curCamera.m_rotPitch < -3.1415927)
        m_curCamera.m_rotPitch =
            (float)((float)(int)(float)((float)(3.1415927 - m_curCamera.m_rotPitch) * 0.15915494) * 6.2831855) +
            m_curCamera.m_rotPitch;

    auto v3 = m_minDist.GetF();
    auto v4 = m_maxDist.GetF();

    auto v5 = m_curGameMode.m_mode;

    if (v5 == GS_GAME || v5 == GS_MAINMENU && !m_bDoNotLoadMainmenuLevel)
    {
        v3 = v3 * 0.70921987;
        v4 = v4 * 0.70921987;
    }
    if (v3 > m_gameCameraRho)
        m_gameCameraRho = v3;
    if (m_gameCameraRho > v4)
        m_gameCameraRho = v4;
    if (v3 > m_player.m_desiredDistance)
        m_player.m_desiredDistance = v3;
    if (m_player.m_desiredDistance > v4)
        m_player.m_desiredDistance = v4;
    return 1;
}

m3d::ui::Wnd* CMiracle3d::CaptureMouse(m3d::ui::Wnd* wnd)
{
    //TODO: check isModal!!!!!!!!!!!!
    auto const oldCapture = m_wndMouseCapture;
    auto const isModal = M3D_APP->m_pInterfaceManager->IsModalEqualWndRunning();
    if (wnd || isModal)
    {
        m_wndMouseCapture = wnd;
    }
    else
    {
        float x = 512.0;
        float y = 384.0;
        M3D_APP->m_renderer->RelToAbs(x, y);
        M3D_APP->SetMouseXy(x, y);
        //TODO: check this!!!!1
        m_wndMouseCapture = nullptr;
    }
    if (!isModal && (m_wndMouseCapture == nullptr || !m3d::g_Kernel->GetEngineCfg().m_r_dxcursor.GetB()))
    {
        EnableDXCursor(false);
    }
    else
    {
        EnableDXCursor(true);
    }
    return oldCapture;
}

int CMiracle3d::CollideCamera(CVector& pos, float& dist, CVector const& dir, CVector const& prevPos)
{
    // RVA 0x402660 - pulls the camera in until nothing is between it and the
    // thing it is looking at. The two probe geoms are built once and reused.
    static scoped_ptr<ai::Sphere> cameraSphere(ai::Sphere::CreateObject(nullptr, 1.0f, nullptr));
    static scoped_ptr<ai::Ray> viewRay(ai::Ray::CreateObject(nullptr, 1.0f, nullptr));

    (void)prevPos;

    float const camGeomDist = m_collideCameraRadius.GetF() - 0.1f;

    // The aim point: where the camera would sit at the current distance.
    float invLen = 1.0f / sqrtf(dir.y * dir.y + dir.z * dir.z + dir.x * dir.x + 0.00000011920929f);
    CVector aim;
    aim.x = pos.x + dir.x * invLen * dist;
    aim.y = pos.y + dir.y * invLen * dist;
    aim.z = pos.z + dir.z * invLen * dist;

    // The camera is only allowed to close or open the gap so fast.
    float const possibleMoveDist =
        static_cast<float>(M3D_KERNEL->GetTimer().GetLastFrameTime()) * 0.001f * 20.0f;
    if (possibleMoveDist < fabsf(m_player.m_desiredDistance - dist))
    {
        float const sign = (dist - m_player.m_desiredDistance) >= 0.0f ? 1.0f : -1.0f;
        invLen = 1.0f / sqrtf(dir.y * dir.y + dir.z * dir.z + dir.x * dir.x + 0.00000011920929f);
        pos.x = pos.x + dir.x * invLen * possibleMoveDist * sign;
        pos.y = pos.y + dir.y * invLen * possibleMoveDist * sign;
        pos.z = pos.z + dir.z * invLen * possibleMoveDist * sign;
    }
    else
    {
        invLen = 1.0f / sqrtf(dir.y * dir.y + dir.z * dir.z + dir.x * dir.x + 0.00000011920929f);
        pos.x = aim.x - dir.x * invLen * m_player.m_desiredDistance;
        pos.y = aim.y - dir.y * invLen * m_player.m_desiredDistance;
        pos.z = aim.z - dir.z * invLen * m_player.m_desiredDistance;
    }
    dist = sqrtf(
        (aim.x - pos.x) * (aim.x - pos.x) + (aim.z - pos.z) * (aim.z - pos.z) +
        (aim.y - pos.y) * (aim.y - pos.y));

    // Never let the camera sink into the water.
    float const waterLimit = m3d::pClient->GetWorld().GetLandscape().getWaterHeight(
                                 static_cast<int>(pos.x * 0.03125f),
                                 static_cast<int>(pos.z * 0.03125f)) +
        m_collideCameraRadius.GetF() + 0.1f;
    if (waterLimit > pos.y)
    {
        pos.y = waterLimit;
        dist = sqrtf(
            (aim.x - pos.x) * (aim.x - pos.x) + (aim.z - pos.z) * (aim.z - pos.z) +
            (aim.y - waterLimit) * (aim.y - waterLimit));
    }

    // Binary search between the aim point and the wanted camera position for
    // the furthest spot with a clear line of sight.
    CVector altPos = pos;
    CVector leftPos = aim;
    CVector rightPos = pos;
    bool continueCycle = true;
    int iteration = 0;
    do
    {
        pos = altPos;
        dist = sqrtf(
            (aim.x - pos.x) * (aim.x - pos.x) + (aim.z - pos.z) * (aim.z - pos.z) +
            (aim.y - pos.y) * (aim.y - pos.y));

        float const length = sqrtf(
            (aim.x - altPos.x) * (aim.x - altPos.x) + (aim.z - altPos.z) * (aim.z - altPos.z) +
            (aim.y - altPos.y) * (aim.y - altPos.y));
        viewRay->SetLength(length);

        CVector direction;
        direction.x = aim.x - altPos.x;
        direction.y = aim.y - altPos.y;
        direction.z = aim.z - altPos.z;
        viewRay->SetDirection(direction);
        viewRay->SetPosition(altPos);

        float const n = 1.0f / sqrtf(dir.y * dir.y + dir.z * dir.z + dir.x * dir.x + 0.00000011920929f);
        CVector spherePos;
        spherePos.x = dir.x * n * camGeomDist + altPos.x;
        spherePos.y = dir.y * n * camGeomDist + altPos.y;
        spherePos.z = dir.z * n * camGeomDist + altPos.z;
        cameraSphere->SetPosition(spherePos);
        cameraSphere->SetRadius(m_collideCameraRadius.GetF());

        dContact contact;
        bool const rayHit = ai::TraceLine(*viewRay, contact, false, true, true, true, nullptr, true, false);
        bool const sphereHit = ai::CollideGeom(*cameraSphere, true, true, true, true);

        if (rayHit || sphereHit)
        {
            // Blocked: move towards the aim point.
            rightPos = altPos;
            altPos.x = (leftPos.x + altPos.x) * 0.5f;
            altPos.y = (leftPos.y + altPos.y) * 0.5f;
            altPos.z = (leftPos.z + altPos.z) * 0.5f;
        }
        else
        {
            // Clear: try to back off further.
            leftPos = altPos;
            altPos.x = (rightPos.x + altPos.x) * 0.5f;
            altPos.y = (rightPos.y + altPos.y) * 0.5f;
            altPos.z = (rightPos.z + altPos.z) * 0.5f;
            if (sqrtf(
                    (pos.z - altPos.z) * (pos.z - altPos.z) + (pos.y - altPos.y) * (pos.y - altPos.y) +
                    (pos.x - altPos.x) * (pos.x - altPos.x)) <= 0.0099999998f)
            {
                continueCycle = false;
            }
        }
        ++iteration;
    } while (continueCycle && iteration <= 20);

    return 1;
}

int CMiracle3d::OnSkipCinematicMessage(m3d::AuxImpulseInfo const& impInfo)
{
    // RVA 0x41E430
    if (!impInfo.m_state)
    {
        if (auto* cinemaPanel = GetCinemaPanel())
        {
            cinemaPanel->m_bSkipMessage = true;
        }
    }
    return 1;
}

m3d::BlockMusicManager* CMiracle3d::GetBlockMusicManager()
{
    return m_blockMusicManager;
}

bool CMiracle3d::IsRenderAsBackground() const
{
    // RVA 0x4196B0
    return m_bRenderAsBackground;
}

void CMiracle3d::EmergencyRedrawAllObjs()
{
    // RVA 0x418000 - nothing to do.
}

int CMiracle3d::Controls(double t0, double tlen)
{
    // TODO: generated code

    if (!m3d::pClient)
    {
        return 1;
    }

    auto vehicle = m3d::pClient->GetWorld().GetVehicleControlledByPlayer();

    if (vehicle && !vehicle->bIsMovingAlongExternalPath())
    {
        // Weapon controls
        auto weaponManager = M3D_APP->m_pInterfaceManager->GetWeaponGroupManager();
        weaponManager->KeepFire();

        // Horn control
        bool hornState = M3D_APP->m_pImpulses->GetImpulseState(IM_CAR_HORN);
        vehicle->SetHorn(hornState);

        // Steering
        auto* impulses = M3D_APP->m_pImpulses;
        auto* input = M3D_APP->m_input;

        if (impulses->GetImpulseState(IM_CAR_LEFT) || input->GetParam(m3d::input::DeviceParam::DP_JOY_X) < -300)
        {
            vehicle->SetSteer(0.78539819f);
        }
        else if (impulses->GetImpulseState(IM_CAR_RIGHT) || input->GetParam(m3d::input::DeviceParam::DP_JOY_X) > 300)
        {
            vehicle->SetSteer(-0.78539819f);
        }
        else
        {
            vehicle->SetSteer(0.0f);
        }

        // Throttle and braking
        if (impulses->GetImpulseState(IM_CAR_ACC))
        {
            vehicle->SetThrottle(1.0f, true);
        }
        else if (impulses->GetImpulseState(IM_CAR_BREAK))
        {
            vehicle->SetThrottle(-1.0f, true);
        }
        else
        {
            vehicle->ReleaseAllPedals();
        }

        if (impulses->GetImpulseState(IM_CAR_HAND_BREAK))
        {
            vehicle->SetHandBrake();
        }

        // Special controls
        if (impulses->GetImpulseState(IM_CAR_TURNTOWHEELS))
        {
            auto turnToWheelsAllowed = m3d::g_Kernel->GetEngineCfg().m_ai_turntowheels_allowed;
            if (turnToWheelsAllowed.GetB())
            {
                CVector torque{0.0f, 0.0f, 0.0f};
                CVector force{0.0f, 1.0f, 0.0f};
                CVector pos{1.0f, 0.0f, 0.0f};
                vehicle->SetTurningToGroundForceAndTorque(pos, force, torque);
            }
        }

        if (impulses->GetImpulseState(IM_CAR_LIGHTS))
        {
            impulses->ResetImpulseWithoutNotification(IM_CAR_LIGHTS);
            LightActivated = !LightActivated;
            vehicle->ActivateHeadLights(LightActivated);
        }

        if (impulses->GetImpulseState(IM_CAR_GET_OUT_OF_DIFFICULT_PLACE))
        {
            impulses->ResetImpulseWithoutNotification(IM_CAR_GET_OUT_OF_DIFFICULT_PLACE);
            vehicle->GetOutOfDifficultPlace();
        }
    }

    // Game pause toggle
    if (m_curGameMode.m_mode == GS_GAME && M3D_APP->m_pImpulses->GetImpulseStateAndReset(IM_PAUSE))
    {
        ai::pServer->SetPause(!ai::pServer->GetPause());
    }

    // Fly camera movement
    m_flyCamMove = {0.0f, 0.0f, 0.0f};
    auto* impulses = M3D_APP->m_pImpulses;

    if (impulses->GetImpulseState(IM_FWD))
        m_flyCamMove.z += 1.0f;
    if (impulses->GetImpulseState(IM_BK))
        m_flyCamMove.z -= 1.0f;
    if (impulses->GetImpulseState(IM_RIGHT))
        m_flyCamMove.x += 1.0f;
    if (impulses->GetImpulseState(IM_LEFT))
        m_flyCamMove.x -= 1.0f;

    // Apply camera speed and time delta
    float cameraSpeed = m_cameraSpeed.GetF();
    float scale = cameraSpeed * static_cast<float>(t0);

    m_flyCamMove.x *= scale;
    m_flyCamMove.y *= scale;
    m_flyCamMove.z *= scale;

    // Camera rotation
    if (m_player.m_cameraMode != CM_BUMPER)
    {
        float mouseSensitivity = GetMouseSensitivity();

        m_curCamera.m_rotPitch += (m_gameSlideAuto.z * 0.00015000001f) - m_flyCamTurn.y;
        m_curCamera.m_rotYaw += (-m_flyCamTurn.x) - (m_gameSlideAuto.x * 0.00030000001f);
        m_curCamera.m_rotRoll = 0.0f;

        // Clamp pitch in follow mode
        if (m_player.m_cameraMode == CM_FOLLOWMODE)
        {
            m_curCamera.m_rotPitch = std::clamp(m_curCamera.m_rotPitch, -0.44879895f, 0.44879895f);
        }
    }

    ValidateCameraAngles();

    // Update camera position
    if (m_curGameMode.m_mode == GS_GAME || (m_curGameMode.m_mode == GS_MAINMENU && !m_bDoNotLoadMainmenuLevel))
    {
        UpdateCameraPosition(vehicle);
    }

    return 1;
}

float CMiracle3d::GetMeanHigh(float x, float y)
{
    // RVA 0x401330 - samples the ground (or water surface, whichever is higher)
    // on a 10 m grid over the 100 m square centred on (x, y).
    auto const landSize = static_cast<float>(16 * m3d::pClient->GetWorld().m_level->land_size) * 8.0f;
    float meanZ = 0.0f;
    int count = 0;
    for (float xx = x - 50.0f; xx <= x + 50.0f; xx += 10.0f)
    {
        for (float yy = y - 50.0f; yy <= y + 50.0f; yy += 10.0f)
        {
            if (xx >= 0.0f && yy >= 0.0f && landSize > xx && landSize > yy)
            {
                auto& landscape = m3d::pClient->GetWorld().GetLandscape();
                float const waterHeight = landscape.getWaterHeight(
                    static_cast<int>(xx * 0.03125f), static_cast<int>(yy * 0.03125f));
                float const height = landscape.GetHeight(xx, yy, -1, false);
                float const z = waterHeight > height ? waterHeight : height;
                meanZ += z;
                ++count;
            }
        }
    }
    return count > 0 ? meanZ / static_cast<float>(count) : meanZ;
}

float CMiracle3d::GetMaxHigh(float x, float y)
{
    // RVA 0x4014F0 - samples the ground (or water surface, whichever is higher)
    // on a 10 m grid over the 100 m square centred on (x, y).
    auto const landSize = static_cast<float>(16 * m3d::pClient->GetWorld().m_level->land_size) * 8.0f;
    float maxZ = -99999.0f;
    for (float xx = x - 50.0f; xx <= x + 50.0f; xx += 10.0f)
    {
        for (float yy = y - 50.0f; yy <= y + 50.0f; yy += 10.0f)
        {
            if (xx >= 0.0f && yy >= 0.0f && landSize > xx && landSize > yy)
            {
                auto& landscape = m3d::pClient->GetWorld().GetLandscape();
                float const waterHeight = landscape.getWaterHeight(
                    static_cast<int>(xx * 0.03125f), static_cast<int>(yy * 0.03125f));
                float const height = landscape.GetHeight(xx, yy, -1, false);
                float const z = waterHeight > height ? waterHeight : height;
                if (z > maxZ)
                {
                    maxZ = z;
                }
            }
        }
    }
    return maxZ;
}

float CMiracle3d::getFov() const
{
    // RVA 0x418900
    return m_fov.GetF();
}

void CMiracle3d::initZoom()
{
    // RVA 0x418E20
    if (!zoomInited)
    {
        m_Fov0 = m_fov.GetF();
        zoomInited = true;
    }
}

int CMiracle3d::CreateInterfaceManager()
{
    // RVA 0x4187D0
    m_pInterfaceManager = new TruxxUiManager;
    if (!m_pInterfaceManager)
    {
        return 0;
    }
    m_pInterfaceManager->Init();
    return 1;
}

void CMiracle3d::DrawBackground()
{
    M3D_RENDERER->SetTexture(0, m_backgroundTexture, -1.0);
    float y2 = 768.0;
    float x2 = 1024.0;
    float y1 = 0.0;
    float x1 = 0.0;
    M3D_RENDERER->RelToAbs(x1, y1);
    M3D_RENDERER->RelToAbs(x2, y2);
    PutSprite2Abs(x1, y1, 0.0, 0.0, x2, y2, 1.0, 1.0, 0xFFFFFFFF);
}

void CMiracle3d::CaptureBackground()
{
    M3D_RENDERER->CopyRenderTargetToTexture(m_backgroundTexture);
}

CMiracle3d::CMiracle3d() :
    Application("ExMachina.log"),
    m_minDist("minDist", "120", m3d::CVar::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_maxDist("maxDist", "350", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_cameraHeight("camHeight", "20", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_collideCameraRadius("camcolradius", "2", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_smoothCameraRadius("smoothcamradius", "8", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_cameraSpeed("camSpeed", "200", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_maxAngle("maxAngle", "0.55", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_minAngle("minAngle", "0.2", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_fov("fov", "90", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE)
{
    m_curGameMode.m_mode = GS_GAME;
    m_maxTimeScale = 2.0;
    m_normalTimeScale = 1.0;
    m_minTimeScale = 0.0;
    m_player.m_cameraMode = CM_FOLLOWMODE;
    m_gameSlideAuto.x = 0.0;
    m_gameSlideAuto.y = 0.0;
    m_gameSlideAuto.z = 0.0;
    m_gameCameraRho = 25.0;

    GetCameraController()->AttachCamera(&m_curCamera);

    m_player.m_desiredDistance = 25.0f;

    memset(m_srvKeys, 0, sizeof(m_srvKeys));
    m_noclip = false;
    m_gameInited = false;

    m3d::Landscape::Register();
    m3d::CWorld::Register();

    m_profiler_Client = GetProfilerStack().AddProfiler("Client (FrameMove)", 30);
    m_profiler_GetPackets = GetProfilerStack().AddProfiler(" - GetPackets", 30);
    m_profiler_ServerUpdate = GetProfilerStack().AddProfiler(" - ServerUpdate", 30);
    m_profiler_ClientUpdate = GetProfilerStack().AddProfiler(" - ClientUpdate", 30);

    m_paused = 0;
    m_userPaused = 0;
    m_oldPositionValue.z = 1.0;
    m_oldPositionValue.y = 1.0;
    m_oldPositionValue.x = 1.0;
    m_pInterfaceManager = 0;
    zoomInited = 0;
    m_playingVideo = 0;
    m_onFinishVideoPlaying = 0;
    m_profileManager = 0;

    m_blockMusicManager = new m3d::BlockMusicManager;
    m_townMusicManager = new m3d::TownMusicManager;
    m_radioEngine = new m3d::RadioEngine;

    m_bMustStartNewMusic = 0;
    m_bBackgroundTextureIsValid = 0;
    m_bRenderAsBackground = 0;
    m_hackedMusicType = HACKMUSIC_LAST;

    m_oldPositionValue.one();
}

void CMiracle3d::Pause()
{
    if (!m_paused && m_curGameMode.Get() != GS_MAINMENU)
    {
        if (m3d::g_Kernel->GetEngineCfg().m_snd_Enable.GetB())
        {
            PauseSound();
        }
        m_saveTimeScale = m3d::g_Kernel->GetTimer().GetTimeScale();
        m3d::g_Kernel->GetTimer().SetTimeScale(0.0);
        m_paused = true;
    }
}

void CMiracle3d::UnPause()
{
    if (m_paused)
    {
        m3d::g_Kernel->GetTimer().SetTimeScale(m_saveTimeScale);
        if (!m_userPaused)
        {
            UnPauseSound();
        }
        m_paused = 0;
    }
}

int CMiracle3d::GetCurDifficultyLevel() const
{
    if (!m_profileManager)
    {
        return 0;
    }
    auto* profile = m_profileManager->GetCurProfile();
    if (!profile)
    {
        return 0;
    }
    m3d::AIParam diffLevel;
    profile->GetParam(PP_DIFFICULTY_LEVEL, diffLevel);
    return diffLevel.GetAsID();
}

void CMiracle3d::UnPauseSound()
{
    if (m3d::g_Kernel->GetEngineCfg().m_snd_Enable.GetB())
    {
        g_pApp->m_sound->PauseGroup(2, false);
    }
}

m3d::Class* CMiracle3d::GetClass() const
{
    return &m_classCMiracle3d;
}

void CMiracle3d::PauseSound()
{
    if (m3d::g_Kernel->GetEngineCfg().m_snd_Enable.GetB())
    {
        g_pApp->m_sound->PauseGroup(2, true);
    }
}

char const* CMiracle3d::GetCallbackName() const
{
    return nullptr;
}

bool CMiracle3d::KillPostEffect(CStr const& effectName)
{
    return m_postEffect->KillEffect(effectName);
}

void CMiracle3d::ReloadPostEffects()
{
    if (!m_postEffect->Reload())
    {
        SYS_ERROR("m_postEffect->Reload()");
    }
}

int CMiracle3d::GetCurGameMode()
{
    return m_curGameMode.Get();
}

void CMiracle3d::SetMouseYAxisFlipped(bool bFlip)
{
    Application::SetMouseYAxisFlipped(bFlip);
    if (auto* profile = m_profileManager->GetCurProfile(); profile)
    {
        //TODO: check tis
        m3d::AIParam const param(static_cast<int>(IsMouseYAxisFlipped()));
        profile->SetParam(PP_MOUSE_YAXIS_FLIP, param);
    }
}

void CMiracle3d::SetMouseXAxisFlipped(bool bFlip)
{
    Application::SetMouseXAxisFlipped(bFlip);
    if (auto* profile = m_profileManager->GetCurProfile(); profile)
    {
        //TODO: check tis
        m3d::AIParam const param(static_cast<int>(IsMouseXAxisFlipped()));
        profile->SetParam(PP_MOUSE_XAXIS_FLIP, param);
    }
}

ProfileManager* CMiracle3d::GetProfileManager() const
{
    return m_profileManager;
}

int CMiracle3d::OnEvent(m3d::Event const& ev)
{
    //TODO: check this and refactor
    auto app = dynamic_cast<CMiracle3d*>(g_pApp);
    switch (ev.m_eventType)
    {
    case 7:
    case 8:
    case 9:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xF:
    case 0x26:
        goto LABEL_5;
    default:
    {
        if (app->m_pInterfaceManager)
        {
            auto res = app->m_pInterfaceManager->HandleAppEvent(ev);
            if (res)
            {
                return res;
            }
        }
    LABEL_5:
        if (ev.m_eventType > 66550)
        {
            auto evNum = ev.m_eventType - 66555;
            if (evNum)
            {
                if (evNum == 5)
                {
                    //TODO: check this
                    m_radioEngine->PlaySoundMessage(ev.m_intEv[0], ev.m_intEv[1], ev.m_strEv);
                    return 0;
                }
            }
            else
            {
                //TODO: check this
                m_blockMusicManager->SetMusicType(static_cast<m3d::BlockMusicManager::BlockMusicType>(ev.m_intEv[0]));
            }
        }
        else if (ev.m_eventType == 66550)
        {
            M3D_LOG_INFO("MessageBox called");
            return 0;
        }
        else
        {
            auto evNum = ev.m_eventType - 65650;
            if (!evNum)
            {
                //TODO: check this
                app->m_pInterfaceManager->ShowWindow(154, false, false, false, false, nullptr);
                m3d::AuxImpulseInfo info(1, true, -1, 0, 0);
                OnChangeMode(info);
                return 0;
            }
            auto enNum2 = evNum - 28;
            if (!enNum2)
            {
                app->OnChangeProfile();
                return 0;
            }
            if (enNum2 == 871)
            {
                m3d::g_Kernel->GetEngineCfg().m_console->executeCommand("/nextmap " + ai::thePassageData->m_mapName);
                return 0;
            }
        }
    }
        return 0;
    }
}

int CMiracle3d::AddChild(m3d::Object* node)
{
    auto result = Wnd::AddChild(node);
    if (!node)
    {
        return result;
    }
    if (node->IsKindOf(RT_CLASS_LOCAL(MotherPanel)))
    {
        // TODO: whats this??
        //*(&this->m_playingVideo + 1) = 1;
        //this->m_playingVideo = 0;
    }
    return result;
}

int CMiracle3d::RemoveChildForce(m3d::Object* object)
{
    auto result = Wnd::RemoveChildForce(object);
    if (object)
    {
        if (object->IsKindOf(&MotherPanel::m_classMotherPanel))
        {
            // TODO: whats this??
            //*(&this->m_playingVideo + 1) = 0;
            //this->m_playingVideo = 0;
        }
    }
    return result;
}

int CMiracle3d::Render(bool needToRedrawAllObjs)
{
    // TODO: generated code
    if (!m_playingVideo)
    {
        if (m_curGameMode.m_mode == GS_MAINMENU && m_bDoNotLoadMainmenuLevel)
        {
            M3D_RENDERER->ClearViewport(m3d::rend::ClearFlags::M3DCLEAR_CZ, 0xFF000000);
            return 1;
        }

        auto const fov = m_fov.GetF();
        auto const viewport = M3D_RENDERER->GetViewport();
        if (viewport.m_width <= viewport.m_height)
        {
            m_curCamera.m_fovX = fov;
            m_curCamera.m_fovY = ((float)viewport.m_width / viewport.m_height) * fov;
        }
        else
        {
            m_curCamera.m_fovX = ((float)viewport.m_width / viewport.m_height) * fov;
            m_curCamera.m_fovY = fov;
        }

        auto const oldWorldOrigin = m_curCamera.m_worldOrigin;

        CMatrix rotationMatrix;
        rotationMatrix.rotYPR(m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);

        CMatrix addRotZ;
        addRotZ.zero();

        CMatrix vv;
        vv.zero();

        auto const shakingRolling = GetCameraController()->GetShakingRolling();
        float const shakeSin = sin(shakingRolling);
        float const shakeCos = cos(shakingRolling);
        vv._11 = (((addRotZ._41 * rotationMatrix._14) + (addRotZ._31 * rotationMatrix._13)) +
                  (rotationMatrix._12 * (0.0 - shakeSin))) +
            (rotationMatrix._11 * shakeCos);
        vv._12 = addRotZ._42 * rotationMatrix._14 + addRotZ._32 * rotationMatrix._13 + rotationMatrix._12 * shakeCos +
            rotationMatrix._11 * shakeSin;
        vv._13 = (((addRotZ._43 * rotationMatrix._14) + (addRotZ._23 * rotationMatrix._12)) +
                  (addRotZ._13 * rotationMatrix._11)) +
            rotationMatrix._13;
        vv._14 = (((addRotZ._34 * rotationMatrix._13) + (addRotZ._24 * rotationMatrix._12)) +
                  (addRotZ._14 * rotationMatrix._11)) +
            rotationMatrix._14;
        vv._21 = (((rotationMatrix._24 * addRotZ._41) + (rotationMatrix._23 * addRotZ._31)) +
                  (rotationMatrix._22 * (0.0 - shakeSin))) +
            (rotationMatrix._21 * shakeCos);
        vv._22 = rotationMatrix._24 * addRotZ._42 + rotationMatrix._23 * addRotZ._32 + rotationMatrix._22 * shakeCos +
            rotationMatrix._21 * shakeSin;
        vv._23 = (((rotationMatrix._24 * addRotZ._43) + (rotationMatrix._22 * addRotZ._23)) +
                  (rotationMatrix._21 * addRotZ._13)) +
            rotationMatrix._23;
        vv._24 = (((rotationMatrix._23 * addRotZ._34) + (rotationMatrix._22 * addRotZ._24)) +
                  (rotationMatrix._21 * addRotZ._14)) +
            rotationMatrix._24;
        vv._31 = (((addRotZ._41 * rotationMatrix._34) + (addRotZ._31 * rotationMatrix._33)) +
                  ((0.0 - shakeSin) * rotationMatrix._32)) +
            (shakeCos * rotationMatrix._31);
        vv._32 = (((addRotZ._42 * rotationMatrix._34) + (addRotZ._32 * rotationMatrix._33)) +
                  (shakeCos * rotationMatrix._32)) +
            (shakeSin * rotationMatrix._31);
        vv._33 = (((addRotZ._43 * rotationMatrix._34) + (addRotZ._23 * rotationMatrix._32)) +
                  (addRotZ._13 * rotationMatrix._31)) +
            rotationMatrix._33;
        vv._34 = (((addRotZ._24 * rotationMatrix._32) + (addRotZ._14 * rotationMatrix._31)) +
                  (addRotZ._34 * rotationMatrix._33)) +
            rotationMatrix._34;
        vv._41 = (((addRotZ._31 * rotationMatrix._43) + ((0.0 - shakeSin) * rotationMatrix._42)) +
                  (shakeCos * rotationMatrix._41)) +
            (addRotZ._41 * rotationMatrix._44);
        vv._42 =
            (((shakeCos * rotationMatrix._42) + (shakeSin * rotationMatrix._41)) + (addRotZ._42 * rotationMatrix._44)) +
            (addRotZ._32 * rotationMatrix._43);
        vv._43 = (((addRotZ._43 * rotationMatrix._44) + (addRotZ._23 * rotationMatrix._42)) +
                  (addRotZ._13 * rotationMatrix._41)) +
            rotationMatrix._43;
        vv._44 = (((addRotZ._34 * rotationMatrix._43) + (addRotZ._24 * rotationMatrix._42)) +
                  (addRotZ._14 * rotationMatrix._41)) +
            rotationMatrix._44;
        addRotZ = vv;
        addRotZ.getYPR(m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);

        auto const shakingTranslation = GetCameraController()->GetShakingTranslation();
        m_curCamera.m_worldOrigin = m_curCamera.m_worldOrigin + shakingTranslation;

        CMatrix viewMatrix;
        m_curCamera.createViewMatrix(viewMatrix);
        M3D_RENDERER->MatSet(viewMatrix);
        // TODO: check this
        M3D_RENDERER->SetViewMatrix(viewMatrix);

        CMatrix projMatrix;
        m_curCamera.createProjectionMatrix(projMatrix, 1.0);
        M3D_RENDERER->MatSetProj(projMatrix);

        // TODO: check this
        m_curCamera.m_worldOrigin = oldWorldOrigin;
        rotationMatrix.getYPR(m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);

        if (m_gameInited)
        {
            if (m_bRenderAsBackground)
            {
                if (m_bBackgroundTextureIsValid)
                {
                    DrawBackground();
                }
                else
                {
                    m3d::pClient->GetWorld().Render();
                    CaptureBackground();
                    m_bBackgroundTextureIsValid = 1;
                }
            }
            else
            {
                M3D_RENDERER->PushMultiSample(M3D_ENGINE_CFG.m_g_antiAliasing.GetB());
                m3d::pClient->GetWorld().Render();
                M3D_RENDERER->PopMultiSample();
            }
        }

        M3D_RENDERER->SetFog(false, false);
        m_postEffect->Render(m_bBackgroundTextureIsValid);

        // RVA 0x415B40 - debug overlays: camera position / speed / angles in the
        // top-right corner, and object and vehicle counters.
        if (M3D_ENGINE_CFG.m_camInfo.GetB() && m3d::pClient)
        {
            M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);

            CStr strText;
            strText.format(
                "%0.4f %0.4f %0.4f", m_curCamera.m_worldOrigin.x, m_curCamera.m_worldOrigin.y, m_curCamera.m_worldOrigin.z);
            DrawTextRel(1024.0f - static_cast<float>(strText.length()) * 10.0f, 12.0f, 0xFFFF0000, strText, 0, -1);

            if (auto* vehicle = m3d::pClient->GetWorld().GetVehicleControlledByPlayer())
            {
                CStr const speed(vehicle->GetLinearVelocity().length() * 3.5999999f);
                DrawTextRel(1024.0f - static_cast<float>(speed.length()) * 10.0f, 24.5f, 0xFFFF0000, speed, 0, -1);
            }

            CStr angles;
            angles.format("Y=%0.4f P=%0.4f R=%0.4f", m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);
            DrawTextRel(1024.0f - static_cast<float>(angles.length()) * 10.0f, 37.0f, 0xFFFF0000, angles, 0, -1);

            M3D_RENDERER->PopZbState();
        }

        if (m3d::pClient && M3D_ENGINE_CFG.m_ai_vehicle_stats.GetB())
        {
            // Every scene-graph node below the root.
            int numNodes = 0;
            std::vector<m3d::Object*> stack;
            stack.push_back(m3d::pClient->GetWorld().GetGraph().GetRootNode());
            while (!stack.empty())
            {
                auto* node = stack.back();
                stack.pop_back();
                for (auto* child = node->GetFirstChild(); child; child = child->GetNextSibling())
                {
                    ++numNodes;
                    if (child->GetFirstChild())
                    {
                        stack.push_back(child);
                    }
                }
            }

            // Objects whose exact class is Vehicle.
            int numVehicles = 0;
            for (auto id = ai::theObjects->m_allObjects.m_firstNodeId; id != -1;
                 id = ai::theObjects->m_allObjects.m_records[id].m_nextId)
            {
                if (ai::theObjects->m_allObjects.m_records[id].m_value->GetClass() == &ai::Vehicle::m_classVehicle)
                {
                    ++numVehicles;
                }
            }

            M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);
            DrawTextRel(800.0f, 413.0f, 0xFFFF0000, CStr(ai::theObjects->m_GameTime.Diff()) + CStr(" game time"), 0, -1);
            DrawTextRel(800.0f, 426.0f, 0xFFFF0000, CStr(ai::gGlobalSpace->count) + CStr(" geoms in global space"), 0, -1);
            DrawTextRel(800.0f, 439.0f, 0xFFFF0000, CStr(numNodes) + CStr(" nodes"), 0, -1);
            DrawTextRel(800.0f, 452.0f, 0xFFFF0000, CStr(ai::theObjects->m_allObjects.m_size) + CStr(" objects"), 0, -1);
            DrawTextRel(
                800.0f, 465.0f, 0xFFFF0000, CStr(ai::theObjects->m_updatingObjects.m_size) + CStr(" updating objects"), 0, -1);
            DrawTextRel(800.0f, 478.0f, 0xFFFF0000, CStr(numVehicles) + CStr(" vehicles"), 0, -1);
            DrawTextRel(
                800.0f,
                491.0f,
                0xFFFF0000,
                CStr(ai::gDynamicScene->GetNumNearCallbacksLastFrame()) + CStr(" near callbacks"),
                0,
                -1);
            DrawTextRel(800.0f, 504.0f, 0xFFFF0000, CStr(ai::theObjects->m_numRemovalsLastFrame) + CStr(" removals"), 0, -1);

            if (auto* vehicle = m3d::pClient->GetWorld().GetVehicleControlledByPlayer())
            {
                DrawTextRel(270.0f, 717.0f, 0xFFFF0000, CStr(vehicle->Fuel().value().get()) + CStr(" fuel"), 0, -1);
                DrawTextRel(
                    270.0f,
                    730.0f,
                    0xFFFF0000,
                    CStr(vehicle->GetLinearVelocity().length() * 3.5999999f) + CStr(" km/h"),
                    0,
                    -1);
                DrawTextRel(270.0f, 743.0f, 0xFFFF0000, CStr(vehicle->GetEngineRpm()) + CStr(" rpm"), 0, -1);
                DrawTextRel(270.0f, 756.0f, 0xFFFF0000, CStr(vehicle->GetCurrentGear()) + CStr(" gear"), 0, -1);
            }
            M3D_RENDERER->PopZbState();
        }
    }

    return 1;
}

int CMiracle3d::RemoveChild(m3d::Object* node)
{
    auto result = Wnd::RemoveChild(node);
    if (!result || !node)
        return result;
    if (!node->IsKindOf(RT_CLASS_LOCAL(MotherPanel)))
        return result;

    //TODO: check this
    //*(&this->m_playingVideo + 1) = 0;
    //this->m_playingVideo = false;
    return result;
}

int CMiracle3d::DoneMedia()
{
    M3D_LOG_INFO("--- Done Media ---");
    if (g_pApp->m_sound != nullptr)
    {
        g_pApp->m_sound->StopAllSounds();
    }
    if (g_Vivisector != nullptr)
    {
        delete g_Vivisector;
        g_Vivisector = nullptr;
    }
    ClearViewportToBlack();
    CinematicClear();
    //TODO: mb ref
    auto* g_pGame = dynamic_cast<CMiracle3d*>(g_pApp);
    auto* savesManager = g_pGame->m_pInterfaceManager->GetSavesManager();
    auto const pathToTempMaps = savesManager->GetPathForTemporaryMaps();
    help::DeleteAllFilesInDirectory(pathToTempMaps.c_str());
    if (m3d::pClient != nullptr)
    {
        ProcessAllEvents();
        g_pGame->m_pInterfaceManager->ShowWindow(166, false, false, false, false, nullptr);
        g_pGame->m_pInterfaceManager->LaunchEvent(86, GUI_EVENT_CUSTOM, nullptr);
        ai::pServer->Clear();
        ai::pServer->ClearOnce();
        m3d::pClient->Reset();
        m3d::pClient->GetWorld().Release();
        DiscardAllEvents();
        g_pGame->m_pImpulses->ResetAllImpulses(true);
        m_bRenderAsBackground = false;
        m_bBackgroundTextureIsValid = false;
    }
    m_gameInited = false;
    delete m3d::pClient;

    m3d::g_Kernel->GetEngineCfg().m_levFileName.Set("Empty");
    if (m_pInterfaceManager->DecRef() <= 0)
    {
        m_pInterfaceManager = nullptr;
    }
    auto* profileManager = GetProfileManager();
    auto* curProfile = profileManager->GetCurProfile();
    profileManager->SaveProfile(curProfile);
    //TODO: check this
    delete profileManager;
    delete m_radioEngine;
    delete m_blockMusicManager;
    delete m_townMusicManager;

    g_pGame->m_renderer->UnregisterResetCallback(this);
    delete m_postEffect;

    g_pGame->m_renderer->ReleaseTexture(m_backgroundTexture);
    M3D_LOG_INFO("--- Done Media: Ok ---");
    return 1;
}

int CMiracle3d::InitMedia()
{
    M3D_LOG_INFO("--- Init Media ---");
    m_curGameMode.Set(GS_INITIALIZATION);
    g_pApp->m_renderer->RegisterResetCallback(this);
    m_postEffect = new PostEffectManager{};
    if (!m_postEffect->Initialize())
    {
        SYS_ERROR("m_postEffect->Initialize()");
    }
    RegisterGlobalNatives();
    //m3d::g_Kernel->GetTimer().GetCurTime();
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_maxDist, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_minDist, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cameraSpeed, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cameraHeight, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_collideCameraRadius, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_smoothCameraRadius, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_minAngle, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_maxAngle, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_fov, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cinematic->m_fadePeriod, nullptr);
    m3d::g_Kernel->AddClass(RT_CLASS_LOCAL(Profile));
    m3d::g_Kernel->AddClass(RT_CLASS_LOCAL(ProfileManager));
    GetBlockMusicManager()->InitOnce();

    m_pInterfaceManager = new TruxxUiManager{};
    if (m_pInterfaceManager != nullptr)
    {
        m_pInterfaceManager->Init();
        m_profileManager = dynamic_cast<ProfileManager*>(m3d::g_Kernel->New("ProfileManager"));
        m_townMusicManager->Init();
        m_radioEngine->Init();
        g_pApp->m_pImpulses->LoadFromDefaults();
        RegisterConsoleCommands();
        initVivisectionBlock();
        if (m3d::g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
        {
            g_pApp->m_sound->SetGroupVolume(1, m3d::g_Kernel->GetEngineCfg().m_snd_2dVolume.GetC());
            g_pApp->m_sound->SetGroupVolume(2, m3d::g_Kernel->GetEngineCfg().m_snd_3dVolume.GetC());
        }

        std::srand(std::time(nullptr));
        GetMusicServer().AddItem("file:", "mainmenu");
        m_musicNames.resize(4);
        m_musicNames[0] = "mainmenu";
        m_bDoNotLoadMainmenuLevel = m3d::g_Kernel->GetEngineCfg().m_DoNotLoadMainmenuLevel.GetB();
        CaptureMouse(nullptr);
        SetCursorShow(false);
        InitBackgroundTexture();
        OnFinishIntroVideoPlaying();
        return 1;
    }
    M3D_LOG_INFO("Fail to create interface manager");
    return 0;
}

int CMiracle3d::FrameMove()
{
    //TODO: implement CMiracle3d::FrameMove
    if (!m_playingVideo || m_enginePlayingVideo)
    {
        auto* profiler = GetProfilerStack().GetProfiler(m_profiler_Client);
        profiler->StartCountdown();
        PlayHackedMusic(m_hackedMusicType, false);
        m3d::RadioEngine::GetInstance()->PlayNextSoundMessage();
        auto startTime = m3d::g_Kernel->GetTimer().GetFrameStartTime();
        auto lastTime = m3d::g_Kernel->GetTimer().GetLastFrameTime();
        auto dT = lastTime * 0.001;
        if (m3d::pClient)
        {
            GetCameraController()->Update();
            if (m_cinematic->m_state != m3d::CINEMATIC_NOT_INITED)
            {
                ref_ptr wndTown = m_pInterfaceManager->GetWindow(4);
                if (wndTown && wndTown->IsChildOf(M3D_APP))
                {
                    HandleCinematic(m3d::g_Kernel->GetTimer().GetLastFrameTimeUnscaled() * 0.001);
                }
                else
                {
                    HandleCinematic(m3d::g_Kernel->GetTimer().GetLastFrameTime() * 0.001);
                }
            }
            m3d::pClient->Update(startTime, lastTime);
            if (m_cinematic->m_state != m3d::CinematicState::CINEMATIC_NOT_INITED)
            {
                HandleCinematic(0.0);
                m_cinematic->UpdateCameraRotation(this->m_curCamera);
            }
            if (this->m_curGameMode.m_mode != GS_CINEMATIC)
            {
                // TODO: check this!!!!!!
                Controls(startTime, dT);
            }
            M3D_APP->m_pInterfaceManager->Update();
        }
        else
        {
            M3D_APP->m_pInterfaceManager->Update();
        }
        profiler->EndCountdown();
    }
    else
    {
        m_playingVideo = false;
        if (m_onFinishVideoPlaying)
        {
            (this->*m_onFinishVideoPlaying)();
            return 1;
        }
    }
    return 1;
}

void CMiracle3d::HandleCommand(int i, m3d::CConsoleParams const& consoleParams)
{
    Application::HandleCommand(i, consoleParams);
    // RVA 0x41F230
    auto* console = M3D_ENGINE_CFG.m_console;
    switch (i)
    {
    case 4096:
    {
        LoadMapFromConsole(consoleParams, false);
        break;
    }
    case 4112: // load
    {
        if (consoleParams.NumOfTokens(' ') == 2)
        {
            LoadSavedGame(CStr(consoleParams.UnsafeStringToken(1, ' ')));
        }
        else
        {
            console->PrintF("Usage: /load <file_name>\n");
        }
        break;
    }
    case 4117:
    {
        M3D_APP->MiniDump();
        break;
    }
    case 4118:
    {
        LoadMapFromConsole(consoleParams, true);
        break;
    }
    case 4119:
    {
        m3d::pClient->GetWorld().GetLandscape().ReBuildShoresVb();
        break;
    }
    case 4120: // save
    {
        if (consoleParams.NumOfTokens(' ') == 2)
        {
            SaveGame(CStr(consoleParams.UnsafeStringToken(1, ' ')), true);
        }
        else
        {
            console->PrintF("Usage: /save <file_name>\n");
        }
        break;
    }
    case 4121:
    {
        console->PrintF(ai::theStatisticManager->GetAllStatisticsDescription());
        break;
    }
    case 4128:
    {
        ReloadPostEffects();
        break;
    }
    case 4129: // add post effect
    {
        if (consoleParams.NumOfTokens(' ') < 2)
        {
            console->PrintF("input name of effect\n");
            break;
        }
        char buf[50];
        consoleParams.StringToken(1, buf, 50, ' ');
        bool const added = consoleParams.NumOfTokens(' ') == 2
            ? AddPostEffect(CStr(buf), 0.0f)
            : AddPostEffect(CStr(buf), consoleParams.FloatToken(2, ' '));
        if (!added)
        {
            console->PrintF("there is no effect with name '" + CStr(buf) + CStr("'\n"));
        }
        break;
    }
    case 4130: // get / set post effect param
    {
        if (consoleParams.NumOfTokens(' ') < 2)
        {
            console->PrintF("input name of param\n");
            break;
        }
        char buf[50];
        consoleParams.StringToken(1, buf, 50, ' ');
        if (consoleParams.NumOfTokens(' ') == 2)
        {
            float value = 0.0f;
            if (GetPostEffectParam(CStr(buf), value))
            {
                console->PrintF(CStr(buf) + CStr(" = ") + CStr(value) + CStr("\n"));
            }
            else
            {
                console->PrintF("Param " + CStr(buf) + CStr(" not exist\n"));
            }
        }
        else if (consoleParams.NumOfTokens(' ') == 3)
        {
            if (SetPostEffectParam(CStr(buf), consoleParams.FloatToken(2, ' ')))
            {
                console->PrintF(CStr(buf) + CStr(" set to ") + CStr(consoleParams.FloatToken(2, ' ')) + CStr("\n"));
            }
            else
            {
                console->PrintF("Param " + CStr(buf) + CStr(" not exist\n"));
            }
        }
        else
        {
            console->PrintF("too many parameters\n");
        }
        break;
    }
    case 4131: // kill post effect
    {
        if (consoleParams.NumOfTokens(' ') < 2)
        {
            console->PrintF("input name of effect\n");
            break;
        }
        char buf[50];
        consoleParams.StringToken(1, buf, 50, ' ');
        if (!KillPostEffect(CStr(buf)))
        {
            console->PrintF("there is no effect with name '" + CStr(buf) + CStr("' or effect not running\n"));
        }
        break;
    }
    case 4132: // hardware cursor
    {
        // NOTE: the original only requires one token (the command itself), so
        // with no argument IntToken(1) reads past the parameters.
        if (consoleParams.NumOfTokens(' ') < 1)
        {
            console->PrintF("input name of param\n");
            break;
        }
        bool const bEnable = consoleParams.IntToken(1, ' ') != 0;
        M3D_ENGINE_CFG.m_r_dxcursor.SetB(bEnable, false);
        if (!bEnable || GetCapture() != this)
        {
            EnableDXCursor(bEnable);
        }
        break;
    }
    default:
        break;
    }
}

bool CMiracle3d::HandleCVar(m3d::CVar const* cvar, m3d::CConsoleParams const& params)
{
    //TODO: check this
    return Application::HandleCVar(cvar, params);
}

int CMiracle3d::NewFrame()
{
    //TODO: chgck this
    m_flyCamTurn.zero();
    return 1;
}

void CMiracle3d::setFov(float value)
{
    m_fov.SetF(value);
}

void CMiracle3d::RegisterConsoleCommands()
{
    auto& config = m3d::g_Kernel->GetEngineCfg();
    for (auto const& command : conCommands)
    {
        config.m_console->RegisterCommand(command.m_name, command.m_id, this);
    }
}

int CMiracle3d::InitImpulses()
{
    using namespace m3d;
    g_Kernel->AddClass(RT_CLASS_LOCAL(GameImpulse));
    g_Kernel->AddClass(RT_CLASS_LOCAL(TruxxImpulse));
    m_pImpulses = dynamic_cast<TruxxImpulse*>(m3d::g_Kernel->New("TruxxImpulse"));
    if (m_pImpulses == nullptr)
    {
        return 0;
    }
    auto const res = m_pImpulses->Init();
    if (res == 0)
    {
        M3D_LOG_INFO("Fail to init impulses");
    }
    return res;
}

void CMiracle3d::InitBackgroundTexture()
{
    m_backgroundTexture = g_pApp->m_renderer->GetFullFrameFrameBufferTexture();
    g_pApp->m_renderer->SetTextureParameter(m_backgroundTexture, m3d::rend::TM_WRAP_S, 3);
    g_pApp->m_renderer->SetTextureParameter(m_backgroundTexture, m3d::rend::TM_WRAP_T, 3);
}

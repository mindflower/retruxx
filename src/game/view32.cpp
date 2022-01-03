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
#include <cameracontroller.h>
#include <cinematic.h>
#include <client.h>
#include <config.h>
#include <landscape.h>
#include <world.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <posteffects/posteffectmanager.h>
#include <server/server.h>
#include <ctime>
#include <stdexcept>
#include <scene/servers/dataserver.h>

extern Vivisector* g_Vivisector;
namespace ai
{
    extern CServer* pServer;
}

namespace m3d
{
    extern CClient* pClient;
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
}

unsigned m_profiler_Client = 0;
unsigned m_profiler_GetPackets = 0;
unsigned m_profiler_ServerUpdate = 0;
unsigned m_profiler_ClientUpdate = 0;

int CMiracle3d::OnChangeMode(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::SkipCinematicMessage()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnGameDrag(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

float CMiracle3d::GetMinTimeScale() const
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnFinishVideoPlaying()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::SetMinTimeScale(float)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::LoadSavedGame(CStr const&)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::GetMouseHitPoint(CVector&, m3d::SgNode*&)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnFinishIntroVideoPlaying()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::GameDone()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnFlyMouse(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::GameInit()
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::GetCursorShow() const
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::GetCursorShow0() const
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::SetCursorShow(bool)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::CinematicInit()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::PlayHackedMusic(HackedMusicType, bool)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::CinematicFade()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CMiracle3d::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::TownMusicManager* CMiracle3d::GetTownMusicManager()
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::AddPostEffect(CStr const&, float)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::HandleCinematic(float)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CMiracle3d::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::OnChangeProfile()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::CleanMainMenuLevel()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::BeginModalDlg(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::ui::MbRetCodes CMiracle3d::RunMsgBoxDlg(CStr const&, CStr const&, unsigned, bool)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::CleanLevel(bool, bool)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnGameZoom(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::PutSplash(int, char const*)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::bIsMousePointing() const
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::ClearSomeGameElementsBeforeModal()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::ChangeLanguage()
{
    throw std::logic_error("Not implemented");
}

float CMiracle3d::getZoom()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::CinematicClear()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::OnBeforeDeviceReset()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::CinematicInterrupt()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::setZoom(float)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::OnAfterDeviceReset()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnSkipCinematic(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::RenderAsBackground(bool)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::UpdateCameraPosition(ai::PhysicObj*)
{
    throw std::logic_error("Not implemented");
}

float CMiracle3d::GetMaxTimeScale() const
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::SetMaxTimeScale(float)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnObtainingFocus()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::LoadLevel(CStr const&, CStr const&, bool, bool, bool, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*, ai::ObjContainer::eSAVE_TYPES)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::FullSystyemAndUserUnpause()
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::GetPostEffectParam(CStr const&, float&)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::SetPostEffectParam(CStr const&, float)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::StartMainMenu()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::SetMouseSensitivity(float)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::UpdateCinematicCameraRotation()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::SetCurHackedMusicType(HackedMusicType)
{
    throw std::logic_error("Not implemented");
}

HackedMusicType CMiracle3d::GetCurHackedMusicType() const
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::ValidateCameraOrigin(bool)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnGameSwitchCamera(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnGameMouse(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::EndModalDlg()
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::CanLaunchIfaceWindow()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::LoadMainMenuLevel()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnDebug(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::LoadMapFromConsole(m3d::CConsoleParams const&, bool)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::StartPlayingVideo(char const*, int(CMiracle3d::*)())
{
    throw std::logic_error("Not implemented");
}

CMiracle3d::~CMiracle3d()
{
    throw std::logic_error("Not implemented");
}

float CMiracle3d::GetNormalTimeScale() const
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::SetNormalTimeScale(float)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::SaveGame(CStr const&, bool)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::LoadMap(CStr const&, bool, m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*, ai::ObjContainer::eSAVE_TYPES)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::ValidateCameraAngles()
{
    throw std::logic_error("Not implemented");
}

m3d::ui::Wnd* CMiracle3d::CaptureMouse(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::CollideCamera(CVector&, float&, CVector const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnSkipCinematicMessage(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

m3d::BlockMusicManager* CMiracle3d::GetBlockMusicManager()
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::IsRenderAsBackground() const
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::EmergencyRedrawAllObjs()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::Controls(double, double)
{
    throw std::logic_error("Not implemented");
}

float CMiracle3d::GetMeanHigh(float, float)
{
    throw std::logic_error("Not implemented");
}

float CMiracle3d::GetMaxHigh(float, float)
{
    throw std::logic_error("Not implemented");
}

float CMiracle3d::getFov() const
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::initZoom()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::CreateInterfaceManager()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::DrawBackground()
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::CaptureBackground()
{
    throw std::logic_error("Not implemented");
}

CMiracle3d::CMiracle3d() :
    Application("ExMachina.log"),
    m_minDist("minDist", "120", m3d::CVar::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_maxDist("maxDist", "350", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_cameraHeight("camHeight", "20", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_collideCameraRadius("camcolradius", "2", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_smoothCameraRadius("smoothcamradius", "8", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_cameraSpeed("camSpeed", "8", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_maxAngle("maxAngle", "0.55", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_minAngle("minAngle", "0.2", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE),
    m_fov("fov", "90", m3d::CVar::eType::CVAR_FLOAT, m3d::CVar::eFlags::CVAR_ARCHIVE)
{
    GetCameraController()->AttachCamera(&m_curCamera);

    m_player.m_desiredDistance = 25.0f;

    m3d::Landscape::Register();
    m3d::CWorld::Register();

    m_profiler_Client = GetProfilerStack().AddProfiler("Client (FrameMove)", 30);
    m_profiler_GetPackets = GetProfilerStack().AddProfiler(" - GetPackets", 30);
    m_profiler_ServerUpdate = GetProfilerStack().AddProfiler(" - ServerUpdate", 30);
    m_profiler_ClientUpdate = GetProfilerStack().AddProfiler(" - ClientUpdate", 30);

    m_blockMusicManager = new m3d::BlockMusicManager{};
    m_townMusicManager = new m3d::TownMusicManager{};
    m_radioEngine = new m3d::RadioEngine{};

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

int CMiracle3d::OnEvent(m3d::Event const&)
{
    //TODO: ...
    throw std::logic_error("Not implemented");
}

int CMiracle3d::AddChild(m3d::Object* node)
{
    auto result = Wnd::AddChild(node);
    if (!node)
    {
        return result;
    }
    if (node->IsKindOf(&MotherPanel::m_classMotherPanel) == false)
    {
        return result;
    }
    //TODO: what here?
    //*(&this->m_playingVideo + 1) = 1;
    //this->m_playingVideo = 0;
    return result;
}

int CMiracle3d::RemoveChildForce(m3d::Object* object)
{
    //TODO: ...
    throw std::logic_error("Not implemented");
}

int CMiracle3d::Render(bool)
{
    //TODO: ...
    throw std::logic_error("Not implemented");
}

int CMiracle3d::RemoveChild(m3d::Object* object)
{
    //TODO: ...
    throw std::logic_error("Not implemented");
}

int CMiracle3d::DoneMedia()
{
    LOG("--- Done Media ---", LOG_INFO);
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
    LOG("--- Done Media: Ok ---", LOG_INFO);
    return 1;

}

int CMiracle3d::InitMedia()
{
    LOG("--- Init Media ---", LOG_INFO);
    m_curGameMode.Set(GS_INITIALIZATION);
    g_pApp->m_renderer->RegisterResetCallback(this);
    m_postEffect = new PostEffectManager{};
    if (!m_postEffect->Initialize())
    {
        SYS_ERROR("m_postEffect->Initialize()");
    }
    RegisterGlobalNatives();
    //m3d::g_Kernel->GetTimer().GetCurTime();
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_maxDist, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_minDist, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_cameraSpeed, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_cameraHeight, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_collideCameraRadius, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_smoothCameraRadius, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_minAngle, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_maxAngle, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_fov, nullptr);
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(m_cinematic->m_fadePeriod, nullptr);
    m3d::g_Kernel->AddClass(&Profile::m_classProfile);
    m3d::g_Kernel->AddClass(&ProfileManager::m_classProfileManager);
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
        CaptureMouse(this);
        SetCursorShow(this);
        InitBackgroundTexture();
        OnFinishIntroVideoPlaying();
        return 1;
    }
    LOG("Fail to create interface manager", LOG_INFO);
    return 0;
}

int CMiracle3d::FrameMove()
{
    //TODO: ...
    throw std::logic_error("Not implemented");
    if (!m_playingVideo || m_enginePlayingVideo)
    {
        auto* profiler = GetProfilerStack().GetProfiler(m_profiler_Client);
        profiler->StartCountdown();
        PlayHackedMusic(m_hackedMusicType, false);
        m3d::RadioEngine::GetInstance()->PlayNextSoundMessage();
        //TODO:...
        if (m3d::pClient != nullptr)
        {
            GetCameraController()->Update();
            if (m_cinematic->m_state != m3d::CINEMATIC_NOT_INITED)
            {
                auto wndTown = m_pInterfaceManager->GetWindow(4);
            }
        }
    }
}

void CMiracle3d::HandleCommand(int i, m3d::CConsoleParams const& consoleParams)
{
    //TODO: ...
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::HandleCVar(m3d::CVar const* cvar, m3d::CConsoleParams const& params)
{
    //TODO: check this
    return Application::HandleCVar(cvar, params);
}

int CMiracle3d::NewFrame()
{
    //TODO: cehgck this
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
    m3d::g_Kernel->AddClass(&m3d::GameImpulse::m_classGameImpulse);
    m3d::g_Kernel->AddClass(&TruxxImpulse::m_classTruxxImpulse);
    m_pImpulses = dynamic_cast<TruxxImpulse*>(m3d::g_Kernel->New("TruxxImpulse"));
    if (m_pImpulses == nullptr)
    {
        return 0;
    }
    auto const res = m_pImpulses->Init();
    if (res == 0)
    {
        LOG("Fail to init impulses", LOG_INFO); 
    }
    return res;
}

void CMiracle3d::InitBackgroundTexture()
{
    m_backgroundTexture = g_pApp->m_renderer->GetFullFrameFrameBufferTexture();
    g_pApp->m_renderer->SetTextureParameter(m_backgroundTexture, m3d::rend::TM_WRAP_S, 3);
    g_pApp->m_renderer->SetTextureParameter(m_backgroundTexture, m3d::rend::TM_WRAP_T, 3);
}

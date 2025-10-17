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
#include "uimisc/questinfo.h"
#include "uiwindows/miscwindows/cinemapanel.h"
#include <algorithm>
#include <game/uimisc/weapongroup.h>

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
}

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

void CMiracle3d::Player::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::Player::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnChangeMode(m3d::AuxImpulseInfo const& impInfo)
{
    if (!impInfo.m_state)
    {
        return 1;
    }
    if (m_curGameMode.Get() == GS_CINEMATIC && impInfo.m_impId != 2)
    {
	    if (m_cinematic->m_state != 5 && m_cinematic->m_state != 3 && m_cinematic->m_state != 4)
	    {
            CinematicInterrupt();
            HandleCinematic(0.0);
            return 1;
	    }
    }
    if (m_curGameMode.Get() != GS_MAINMENU && impInfo.m_impId == 1)
    {
        auto app = dynamic_cast<CMiracle3d*>(g_pApp);
        StopPlayingMusic();
        if (m_gameInited)
        {
            ClearViewportToBlack();
            CinematicClear();
            auto savesManager = app->m_pInterfaceManager->GetSavesManager();
            auto tempMaps = savesManager->GetPathForTemporaryMaps();
            help::DeleteAllFilesInDirectory(tempMaps.c_str());
            if (m3d::pClient)
            {
                ProcessAllEvents();
                app->m_pInterfaceManager->ShowWindow(166, false, false, false, false, nullptr);
                app->m_pInterfaceManager->LaunchEvent(86, GUI_EVENT_CUSTOM, nullptr);
                ai::pServer->Clear();
                ai::pServer->ClearOnce();
                m3d::pClient->Reset();
                m3d::pClient->GetWorld().Release();
                DiscardAllEvents();
                app->m_pImpulses->ResetAllImpulses(true);
                m_bRenderAsBackground = false;
                m_bBackgroundTextureIsValid = false;
            }
        }
        m_curGameMode.Set(GS_MAINMENU);
        //TODO: check this (1.0)
        m3d::g_Kernel->GetTimer().SetTimeScale(1.0);
        m_saveTimeScale = m3d::g_Kernel->GetTimer().GetTimeScale();
        AllowRendering();
        if (!LoadMainMenuLevel())
        {
            GameDone();
        }
        app->m_pInterfaceManager->Show(false, true);
        app->m_pInterfaceManager->ShowWindow(72, true, true, false, false, nullptr);
        SetCursorShow(true);
        CaptureMouse(nullptr);
        if (!m_bDoNotLoadMainmenuLevel)
        {
            app->m_pInterfaceManager->ShowWindow(19, 1, 1, false, false, nullptr);
            auto wnd = app->m_pInterfaceManager->GetWindow(19);
            if (IsDirectChild(wnd))
            {
                MoveChildToFirstPosition(wnd);
            }
        }
        return 1;
    }
    if (m_curGameMode.Get() == GS_MAINMENU && impInfo.m_impId == 3)
    {
        M3D_APP->m_pInterfaceManager->ShowWindow(72, false, true, false, false, nullptr);
        CleanMainMenuLevel();
        if (!m_gameInited)
        {
            GameInit();
        }

        // TODO: check this
        M3D_APP->m_pInterfaceManager->StartSplashing(11);
        auto v22 = LoadLevel(M3D_KERNEL->GetEngineCfg().m_levFileName.GetS(), {}, true, false, false, nullptr, nullptr, (ai::ObjContainer::eSAVE_TYPES)(ai::ObjContainer::SAVE_EDITOR | ai::ObjContainer::SAVE_FULL | 0x8)) == 0;
        if (v22)
        {
            return 0;
        }
        CaptureMouse(0);
        m_curGameMode.Set(GS_GAME);
    }
    if (!impInfo.m_impId)
    {
        if (m_curGameMode.Get() == GS_GAME)
        {
            M3D_APP->m_pInterfaceManager->GetSavesManager()->MakeCurGameScreenshot();
        }
        M3D_APP->EnqueueMessage(65656,
            0,
            0,
            0,
            0,
            {},
            {});
        return 1;
    }

    if (impInfo.m_impId == 2)
    {
        CMiracle3d::CinematicInit();
        return 1;
    }
    if (impInfo.m_impId != 3)
        return 1;

    m_curGameMode.Set(GS_GAME);
    if (m_curGameMode.Get() != GS_CINEMATIC)
    {
        SetCursorShow(1);
    }
    CaptureMouse(0);
    return 1;
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

int CMiracle3d::OnFlyMouse(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::GameInit()
{
    m3d::pClient = new m3d::CClient;
    //inject::injectMemoryAddress(0x00A1185C, inject::cast<uint32_t>(m3d::pClient));

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
    throw std::logic_error("Not implemented");
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
                if (musicType != HACKMUSIC_CUSTOM
                    && (m_bMustStartNewMusic || bForceRestart || musicType != this->m_hackedMusicType)
                    && m3d::Application::g_pApp->m_sound
                    && musicType >= HACKMUSIC_MENU
                    && musicType < m_musicNames.size())
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
}

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

    CinemaPanel* cinemaPanel = GetCinemaPanel(); // Assuming this returns CinemaPanel*

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
        m_bGuiWasHiddenBeforeCinematic =
            M3D_APP->m_pInterfaceManager->IsHiddenByUser();

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

int CMiracle3d::HandleCinematic(float dT)
{
    if (m_cinematic->bCanUpdate())
    {
        m_cinematic->Update(m_curCamera, dT);
    }
    m_cinematic->m_playTime += dT * 1000;

    while(true)
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

m3d::ui::MbRetCodes CMiracle3d::RunMsgBoxDlg(CStr const& caption, CStr const& message, unsigned flags, bool bPause)
{
    return M3D_APP->m_pInterfaceManager->RunMsgBoxDlg(caption, message, flags, bPause);
}

int CMiracle3d::CleanLevel(bool, bool)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::OnGameZoom(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::PutSplash(int proc, char const* text)
{
    m_pInterfaceManager->ShowSplash(proc, text);
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
    throw std::logic_error("Not implemented");
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
            m_cinematic->m_fadeStartTime = v11 + m_cinematic->m_playTime
                - (int)(m_cinematic->GetFadePeriodForState(m3d::CINEMATIC_ENTER_FADE_IN) * 1000.0);
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

int CMiracle3d::LoadLevel(CStr const& name, CStr const& saveDir, bool LoadServers, bool bQuiet, bool bContinuousMap, m3d::cmn::XmlFile* dynamicSceneXmlFile, m3d::cmn::XmlNode const* dynamicSceneXmlNode, ai::ObjContainer::eSAVE_TYPES saveType)
{
    //TODO: check bQuiet, continiousMap and LoadServers!!!!
    if (!m_gameInited)
    {
        return 0;
    }
    M3D_LOG_INFO("-- Loading Level: " + name + " --");
    if (LoadServers)
    {
        ai::pServer->InitOnce();
    }
    //TODO: check this
    if (!bQuiet && !m3d::pClient->GetWorld().Load(name, m_curCamera, bQuiet))
    {
	    M3D_LOG_INFO("Level file " + name + " not found");
        EnqueueMessage(1, 0, 0, 0, 0, {}, {});
        return 0;
    }
    m_blockMusicManager->Init();
    auto app = dynamic_cast<CMiracle3d*>(g_pApp);
    if (LoadServers)
    {
        app->m_serverAnimatedModels->GenerateImpostorsIfNeeded();
    }
    M3D_LOG_INFO("Load Server begin...");
    auto levelFullPath = m3d::pClient->GetWorld().m_level->GetFullPathNameA({});
    m_cinematic->SetFolder(levelFullPath.c_str());
    app->m_pInterfaceManager->LaunchEvent(84, GUI_EVENT_CUSTOM, nullptr);
    if (bContinuousMap)
    {
        auto xmlName = help::GetMapNameFromFileName(name);
        auto tempMapsPath = app->m_pInterfaceManager->GetSavesManager()->GetPathForTemporaryMaps();
        auto mapXmlPath = tempMapsPath + xmlName += ".xml";
        auto attr = GetFileAttributesA(mapXmlPath.c_str());
        if (attr == -1 || (attr & 0x10) != 0)
        {
            ai::pServer->Load(ai::LOCAL_GAME, dynamicSceneXmlFile, dynamicSceneXmlNode, bContinuousMap, saveType);
        }
        else
        {
            ai::pServer->LoadVisitedMap(mapXmlPath, bContinuousMap);
        }
    }
    else
    {
        ai::pServer->Load(ai::LOCAL_GAME, dynamicSceneXmlFile, dynamicSceneXmlNode, false, saveType);
    }
    M3D_LOG_INFO("Load Server end");
    if (!bContinuousMap)
    {
        app->m_pInterfaceManager->GetQuestInfoManager()->Init();
    }
    //TODO: check this!!!
    int data = M3D_APP->GetCurGameMode();
    app->m_pInterfaceManager->LaunchEvent(85, GUI_EVENT_CUSTOM, &data);
    if (auto vehicle = ai::gDynamicScene->GetVehicleControlledByPlayer())
    {
        m_curCamera.m_worldOrigin = vehicle->GetPosition();
    }
    m_blockMusicManager->Reset();
    m3d::g_Kernel->GetTimer().SetActiveState(1);
    if (app->AppActive())
    {
        if (m3d::g_Kernel->GetEngineCfg().m_clipCursorWithinRenderWnd.GetB())
        {
            CaptureAndClipSystemCursor(true);
        }
    }
    m_bRenderAsBackground = false;
    m_bBackgroundTextureIsValid = false;
    return 1;
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

//Verified: CMiracle3d::StartMainMenu
void CMiracle3d::StartMainMenu()
{
    if (m_profileManager)
    {
        m_profileManager->Init();
        OnChangeMode(m3d::AuxImpulseInfo{ 1, true, -1, 0, 0 });
    }
    else
    {
        M3D_LOG_INFO("CMiracle3d::InitMedia error - cannot instantiate ProfileManager object");
    }
}

void CMiracle3d::SetMouseSensitivity(float)
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::UpdateCinematicCameraRotation()
{
    throw std::logic_error("Not implemented");
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
        auto res = LoadLevel(mapName, {}, true, false, false, nullptr, nullptr, ai::ObjContainer::SAVE_LEVEL);
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

int CMiracle3d::OnDebug(m3d::AuxImpulseInfo const&)
{
    throw std::logic_error("Not implemented");
}

bool CMiracle3d::LoadMapFromConsole(m3d::CConsoleParams const&, bool)
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::StartPlayingVideo(char const* videoFile, int(CMiracle3d::* onFinishCallback)())
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
    if(m_curCamera.m_rotYaw > 3.1415927)
        m_curCamera.m_rotYaw = m_curCamera.m_rotYaw
        - (float)((float)(int)(float)((float)(m_curCamera.m_rotYaw + 3.1415927) * 0.15915494)
            * 6.2831855);
    if (m_curCamera.m_rotYaw < -3.1415927)
        m_curCamera.m_rotYaw = (float)((float)(int)(float)((float)(3.1415927 - m_curCamera.m_rotYaw) * 0.15915494)
            * 6.2831855)
        + m_curCamera.m_rotYaw;

    auto v2 = m_curCamera.m_rotPitch;
    if (v2 > 3.1415927)
        m_curCamera.m_rotPitch = m_curCamera.m_rotPitch
        - (float)((float)(int)(float)((float)(v2 + 3.1415927) * 0.15915494) * 6.2831855);
    if (m_curCamera.m_rotPitch < -3.1415927)
        m_curCamera.m_rotPitch = (float)((float)(int)(float)((float)(3.1415927 - m_curCamera.m_rotPitch) * 0.15915494)
            * 6.2831855)
        + m_curCamera.m_rotPitch;

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
    return m_blockMusicManager;
}

bool CMiracle3d::IsRenderAsBackground() const
{
    throw std::logic_error("Not implemented");
}

void CMiracle3d::EmergencyRedrawAllObjs()
{
    throw std::logic_error("Not implemented");
}

int CMiracle3d::Controls(double t0, double tlen)
{
    // TODO: generated code

    if(!m3d::pClient)
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
        bool hornState = M3D_APP->m_pImpulses->GetImpulseState(28);
        vehicle->SetHorn(hornState);

        // Steering
        auto* impulses = M3D_APP->m_pImpulses;
        auto* input = M3D_APP->m_input;

        if (impulses->GetImpulseState(26) || input->GetParam(m3d::input::DeviceParam::DP_JOY_X) < -300)
        {
            vehicle->SetSteer(0.78539819f);
        }
        else if (impulses->GetImpulseState(27) || input->GetParam(m3d::input::DeviceParam::DP_JOY_X) > 300)
        {
            vehicle->SetSteer(-0.78539819f);
        }
        else {
            vehicle->SetSteer(0.0f);
        }

        // Throttle and braking
        if (impulses->GetImpulseState(23))
        {
            vehicle->SetThrottle(1.0f, true);
        }
        else if (impulses->GetImpulseState(24))
        {
            vehicle->SetThrottle(-1.0f, true);
        }
        else {
            vehicle->ReleaseAllPedals();
        }

        if (impulses->GetImpulseState(25))
        {
            vehicle->SetHandBrake();
        }

        // Special controls
        if (impulses->GetImpulseState(29))
        {
            auto turnToWheelsAllowed = m3d::g_Kernel->GetEngineCfg().m_ai_turntowheels_allowed;
            if (turnToWheelsAllowed.GetB())
            {
                CVector torque{ 0.0f, 0.0f, 0.0f };
                CVector force{ 0.0f, 1.0f, 0.0f };
                CVector pos{ 1.0f, 0.0f, 0.0f };
                vehicle->SetTurningToGroundForceAndTorque(pos, force, torque);
            }
        }

        if (impulses->GetImpulseState(31))
        {
            impulses->ResetImpulseWithoutNotification(31);
            LightActivated = !LightActivated;
            vehicle->ActivateHeadLights(LightActivated);
        }

        if (impulses->GetImpulseState(32))
        {
            impulses->ResetImpulseWithoutNotification(32);
            vehicle->GetOutOfDifficultPlace();
        }
    }

    // Game pause toggle
    if (m_curGameMode.m_mode == GS_GAME &&
        M3D_APP->m_pImpulses->GetImpulseStateAndReset(51))
    {
        ai::pServer->SetPause(!ai::pServer->GetPause());
    }

    // Fly camera movement
    m_flyCamMove = { 0.0f, 0.0f, 0.0f };
    auto* impulses = M3D_APP->m_pImpulses;

    if (impulses->GetImpulseState(4)) m_flyCamMove.z += 1.0f;
    if (impulses->GetImpulseState(5)) m_flyCamMove.z -= 1.0f;
    if (impulses->GetImpulseState(7)) m_flyCamMove.x += 1.0f;
    if (impulses->GetImpulseState(6)) m_flyCamMove.x -= 1.0f;

    // Apply camera speed and time delta
    float cameraSpeed = m_cameraSpeed.GetF();
    float scale = cameraSpeed * static_cast<float>(t0);

    m_flyCamMove.x *= scale;
    m_flyCamMove.y *= scale;
    m_flyCamMove.z *= scale;

    // Camera rotation
    if (m_player.m_cameraMode != CM_BUMPER) {
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
    if (m_curGameMode.m_mode == GS_GAME ||
        (m_curGameMode.m_mode == GS_MAINMENU && !m_bDoNotLoadMainmenuLevel)) {
        UpdateCameraPosition(vehicle);
    }

    return 1;
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
    if (node->IsKindOf(RT_CLASS_LOCAL(MotherPanel)) == false)
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
    throw std::logic_error("Not implemented");
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

        // Calculate FOV
        float fovValue = m_fov.GetF();

        // Get viewport dimensions
        m3d::rend::Viewport viewport = M3D_RENDERER->GetViewport();
        float viewportWidth = static_cast<float>(viewport.m_width);
        float viewportHeight = static_cast<float>(viewport.m_height);
        float aspectRatio = viewportWidth / viewportHeight;

        // Calculate FOV based on aspect ratio
        if (aspectRatio > 1.0f)
        {
            m_curCamera.m_fovX = fovValue * aspectRatio;
            m_curCamera.m_fovY = fovValue;
        }
        else
        {
            m_curCamera.m_fovX = fovValue;
            m_curCamera.m_fovY = fovValue / aspectRatio;
        }

        // Save original camera state
        CVector originalPosition = m_curCamera.m_worldOrigin;
        float originalYaw = m_curCamera.m_rotYaw;
        float originalPitch = m_curCamera.m_rotPitch;
        float originalRoll = m_curCamera.m_rotRoll;

        // Create rotation matrix from original angles
        CMatrix rotationMatrix;
        rotationMatrix.rotYPR(originalYaw, originalPitch, originalRoll);

        // Apply camera shaking rotation
        float shakingRoll = m_cameraController->GetShakingRolling();
        float sinRoll = sin(shakingRoll);
        float cosRoll = cos(shakingRoll);

        CMatrix shakeRotationMatrix;
        memset(&shakeRotationMatrix, 0, sizeof(shakeRotationMatrix));

        // Apply rotation shaking (matrix multiplication)
        shakeRotationMatrix._11 = rotationMatrix._11 * cosRoll + rotationMatrix._12 * -sinRoll;
        shakeRotationMatrix._12 = rotationMatrix._11 * sinRoll + rotationMatrix._12 * cosRoll;
        shakeRotationMatrix._13 = rotationMatrix._13;
        shakeRotationMatrix._14 = rotationMatrix._14;

        shakeRotationMatrix._21 = rotationMatrix._21 * cosRoll + rotationMatrix._22 * -sinRoll;
        shakeRotationMatrix._22 = rotationMatrix._21 * sinRoll + rotationMatrix._22 * cosRoll;
        shakeRotationMatrix._23 = rotationMatrix._23;
        shakeRotationMatrix._24 = rotationMatrix._24;

        shakeRotationMatrix._31 = rotationMatrix._31 * cosRoll + rotationMatrix._32 * -sinRoll;
        shakeRotationMatrix._32 = rotationMatrix._31 * sinRoll + rotationMatrix._32 * cosRoll;
        shakeRotationMatrix._33 = rotationMatrix._33;
        shakeRotationMatrix._34 = rotationMatrix._34;

        shakeRotationMatrix._41 = rotationMatrix._41 * cosRoll + rotationMatrix._42 * -sinRoll;
        shakeRotationMatrix._42 = rotationMatrix._41 * sinRoll + rotationMatrix._42 * cosRoll;
        shakeRotationMatrix._43 = rotationMatrix._43;
        shakeRotationMatrix._44 = rotationMatrix._44;

        // Extract new rotation angles from shaken matrix
        shakeRotationMatrix.getYPR(m_curCamera.m_rotYaw, m_curCamera.m_rotPitch, m_curCamera.m_rotRoll);

        // Apply translation shaking
        CVector shakingTranslation = m_cameraController->GetShakingTranslation();
        m_curCamera.m_worldOrigin.x += shakingTranslation.x;
        m_curCamera.m_worldOrigin.y += shakingTranslation.y;
        m_curCamera.m_worldOrigin.z += shakingTranslation.z;

        // Create and set view matrix
        CMatrix viewMatrix;
        CAffineXForm xForm;
        xForm.createViewMatrix(viewMatrix);
        M3D_RENDERER->MatSet(viewMatrix);
        M3D_RENDERER->SetViewMatrix(viewMatrix);

        // Create and set projection matrix
        CMatrix projectionMatrix;
        m_curCamera.createProjectionMatrix(projectionMatrix, 1.0f);
        M3D_RENDERER->MatSetProj(projectionMatrix);

        // Restore original camera position for rendering calculations
        m_curCamera.m_worldOrigin = originalPosition;

        // CMatrix::getYPR(
        //(CMatrix*)&carSpeed.m_allocSz,
        //    & this->m_curCamera.m_rotYaw,
        //    & this->m_curCamera.m_rotPitch,
        //    & this->m_curCamera.m_rotRoll);

        // Main rendering logic
        if (m_gameInited)
        {
            if (m_bRenderAsBackground)
            {
                if (m_bBackgroundTextureIsValid)
                {
                    // Draw existing background
                    DrawBackground();
                    // Background drawing implementation would go here
                }
                else {
                    // Render scene and capture as background
                    m3d::pClient->GetWorld().Render();
                    // Background capture implementation would go here
                    CaptureBackground();
                    m_bBackgroundTextureIsValid = true;
                }
            }
            else
            {
                // Normal rendering with anti-aliasing
                bool antiAliasing = M3D_KERNEL->GetEngineCfg().m_g_antiAliasing.GetB();

                M3D_RENDERER->PushMultiSample(antiAliasing);
                m3d::pClient->GetWorld().Render();
                M3D_RENDERER->PopMultiSample();
            }
        }

        // Post-processing
        M3D_RENDERER->SetFog(false, 0);
        m_postEffect->Render(m_bBackgroundTextureIsValid);

        // Debug information display
        bool showCameraInfo = M3D_KERNEL->GetEngineCfg().m_camInfo.GetB();

        if (showCameraInfo && m3d::pClient)
        {
            M3D_RENDERER->PushZbState(m3d::rend::ZbState::ZB_DISABLE);

            // Display camera position   
            CStr posText = CStr::format_("%0.4f %0.4f %0.4f",
                m_curCamera.m_worldOrigin.x,
                m_curCamera.m_worldOrigin.y,
                m_curCamera.m_worldOrigin.z);

            float textX = 1024.0f - posText.length() * 10.0f;
            DrawTextRel(textX, 12.0f, 0xFFFF0000, posText, 0, -1);

            // Display vehicle speed if available
            ai::Vehicle* playerVehicle = m3d::pClient->GetWorld().GetVehicleControlledByPlayer();
            if (playerVehicle)
            {
                CVector velocity = playerVehicle->GetLinearVelocity();
                float speedKmh = velocity.length() * 3.6f;
                CStr speedText(speedKmh);
                float speedTextX = 1024.0f - speedText.length() * 10.0f;
                m3d::Application::DrawTextRel(speedTextX, 24.5f, 0xFFFF0000, speedText, 0, -1);
            }

            // Display camera rotation
            CStr rotText = CStr::format_("Y=%0.4f P=%0.4f R=%0.4f",
                m_curCamera.m_rotYaw,
                m_curCamera.m_rotPitch,
                m_curCamera.m_rotRoll);

            float rotTextX = 1024.0f - rotText.length() * 10.0f;
            DrawTextRel(rotTextX, 37.0f, 0xFFFF0000, rotText, 0, -1);

            M3D_RENDERER->PopZbState();
        }

        // AI vehicle statistics display
        bool showAIVehicleStats = M3D_KERNEL->GetEngineCfg().m_ai_vehicle_stats.GetB();

        if (showAIVehicleStats && m3d::pClient)
        {
            // Count scene graph nodes (simplified version)
            int nodeCount = 0;
            retruxx::vector<m3d::Object*> nodeStack;
            if (auto* rootNode = m3d::pClient->GetWorld().GetGraph().GetRootNode())
            {
                nodeStack.push_back(rootNode);
            }

            while (!nodeStack.empty())
            {
                m3d::Object* currentNode = nodeStack.back();
                nodeStack.pop_back();
                nodeCount++;

                // Add children to stack
                m3d::Object* child = currentNode->GetFirstChild();
                while (child)
                {
                    nodeStack.push_back(child);
                    child = child->GetNextSibling();
                }
            }

            // Count vehicles
            int vehicleCount = 0;
            for (const auto& obj : *ai::theObjects)
            {
                if (obj->GetClass() == &ai::Vehicle::m_classVehicle)
                {
                    ++vehicleCount;
                }
            }

            M3D_RENDERER->PushZbState(m3d::rend::ZbState::ZB_DISABLE);

            // Display various statistics
            float statY = 413.0f;
            float statX = 800.0f;

            // Game time
            float gameTime = ai::theObjects->GetGameTimeDiff();
            auto timeText = CStr(gameTime) + " game time";
            m3d::Application::DrawTextRel(statX, statY, 0xFFFF0000, timeText, 0, -1);
            statY += 13.0f;

            // Geometry count
            auto geomText = CStr(ai::gGlobalSpace->count) + " geoms in global space";
            m3d::Application::DrawTextRel(statX, statY, 0xFFFF0000, geomText, 0, -1);
            statY += 13.0f;

            // Node count
            auto nodeText = CStr(nodeCount) + " nodes";
            m3d::Application::DrawTextRel(statX, statY, 0xFFFF0000, nodeText.c_str(), 0, -1);
            statY += 13.0f;

            // Object count
            auto objText = CStr(ai::theObjects->size()) + (" objects");
            m3d::Application::DrawTextRel(statX, statY, 0xFFFF0000, objText.c_str(), 0, -1);
            statY += 13.0f;

            // Updating objects
            auto updateText = CStr(ai::theObjects->GetNumUpdatingObjects()) + ("updating objects");
            m3d::Application::DrawTextRel(statX, statY, 0xFFFF0000, updateText.c_str(), 0, -1);
            statY += 13.0f;

            // Vehicle count
            auto vehicleText = CStr(vehicleCount) + (" vehicles");
            m3d::Application::DrawTextRel(statX, statY, 0xFFFF0000, vehicleText.c_str(), 0, -1);
            statY += 13.0f;

            // Near callbacks
            int nearCallbacks = ai::gDynamicScene->GetNumNearCallbacksLastFrame();
            auto callbackText = CStr(nearCallbacks) + (" near callbacks");
            m3d::Application::DrawTextRel(statX, statY, 0xFFFF0000, callbackText.c_str(), 0, -1);
            statY += 13.0f;

            // Removals
            auto removalText = CStr(ai::theObjects->GetNumRemovalsLastFrame()) + (" removals");
            m3d::Application::DrawTextRel(statX, statY, 0xFFFF0000, removalText.c_str(), 0, -1);

            // Player vehicle specific info
            ai::Vehicle* playerVehicle = m3d::pClient->GetWorld().GetVehicleControlledByPlayer();
            if (playerVehicle)
            {
                float vehicleStatY = 717.0f;
                float vehicleStatX = 270.0f;

                // Fuel
                float fuel = playerVehicle->Fuel().value().get();
                auto fuelText = CStr(fuel) + (" fuel");
                m3d::Application::DrawTextRel(vehicleStatX, vehicleStatY, 0xFFFF0000, fuelText.c_str(), 0, -1);
                vehicleStatY += 13.0f;

                // Speed
                CVector velocity = playerVehicle->GetLinearVelocity();
                float speedKmh = velocity.length() * 3.6f;
                auto speedText = CStr(speedKmh) + (" km/h");
                m3d::Application::DrawTextRel(vehicleStatX, vehicleStatY, 0xFFFF0000, speedText.c_str(), 0, -1);
                vehicleStatY += 13.0f;

                // RPM
                float rpm = playerVehicle->GetEngineRpm();
                auto rpmText = CStr(rpm) + (" rpm");
                m3d::Application::DrawTextRel(vehicleStatX, vehicleStatY, 0xFFFF0000, rpmText.c_str(), 0, -1);
                vehicleStatY += 13.0f;

                // Gear
                int gear = playerVehicle->GetCurrentGear();
                auto gearText = CStr(gear) + (" gear");
                m3d::Application::DrawTextRel(vehicleStatX, vehicleStatY, 0xFFFF0000, gearText.c_str(), 0, -1);
            }

            m3d::Application::g_pApp->m_renderer->PopZbState();
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
        auto lastTime= m3d::g_Kernel->GetTimer().GetLastFrameTime();
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
    throw std::logic_error("Not implemented");
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

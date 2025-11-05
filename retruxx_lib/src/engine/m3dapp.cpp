#define NOMINMAX
#include "soundconhandler.h"
#include "video/video.h"
#include <atomic>
#include <cameracontroller.h>
#include <cassert>
#include <cinematic.h>
#include <config.h>
#include <intrin.h>
#include <landscape.h>
#include <level.h>
#include <m3dapp.h>
#include <skelmodel.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>
#include <core/stackwalker.h>
#include <core/timer.h>
#include <game/globalscriptfuncs.h>
#include <scene/nodes/sgnodeanimatedmodel.h>
#include <scene/nodes/sgnodedecals.h>
#include <scene/nodes/sgnodegameunit.h>
#include <scene/nodes/sgnodelines.h>
#include <scene/nodes/sgnodeloadpoint.h>
#include <scene/nodes/sgnodeparticles.h>
#include <scene/nodes/sgnodepointlightsource.h>
#include <scene/nodes/sgnodeprojector.h>
#include <scene/nodes/sgnodesound.h>
#include <scene/nodes/sgnodesprite.h>
#include <scene/nodes/sgnodestaticmodel.h>
#include <scene/servers/dataserver.h>
#include <scene/servers/serverfactories.h>
#include <server/server.h>
#include <ui/button.h>
#include <ui/comboboxwnd.h>
#include <ui/edit.h>
#include <ui/glyphbutton.h>
#include <ui/image.h>
#include <ui/linewnd.h>
#include <ui/listbox.h>
#include <ui/menu.h>
#include <ui/modelarraywnd.h>
#include <ui/modelwnd.h>
#include <ui/progressbarwnd.h>
#include <ui/scroll.h>
#include <ui/sgnodearraywnd.h>
#include <ui/slider.h>
#include <ui/tabwnd.h>
#include <ui/textbox.h>
#include <ui/uidialogs.h>

#include "impulses/i_impulses.h"
#include <core/ini.h>

namespace
{
    retruxx::set<size_t>* codePagesStringsPtr;

    BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString)
    {
        codePagesStringsPtr->insert(std::atoi(lpCodePageString));
        return TRUE;
    }

    void __fastcall logDeviceFunc(CStr const& str)
    {
        M3D_LOG_INFO("d3d: " + str);
    }

    void __fastcall logSoundFunc(CStr const& str)
    {
        M3D_LOG_INFO("snd: " + str);
    }

    void __fastcall logInputFunc(CStr const& str)
    {
        M3D_LOG_INFO("input: " + str);
    }

    m3d::CConsoleCommands conCommands[] = {
        {"exit", 0},
        {"benchmark", 1},
        {"mem_footprint", 3},
        {"mem_debug", 4},
        {"mem_showstats", 5},
        {"showProfilers", 6},
        {"showCounters", 7},
        {"resetFilesystem", 8},
        {"descExport", 9},
        {"descNative", 10},
        {"descClass", 11},
        {"r_videomode", 2},
        {"r_reloadShaders", 13},
        {"r_showStats", 14},
        {"r_showDeviceMemStats", 15},
        {"r_showGraphicalStats", 16},
        {"r_reloadTextures", 17},
        {"r_repainTextures", 18},
        {"r_texturesInfo", 19},
        {"r_vbInfo", 20},
        {"r_ibInfo", 21},
        {"s_modelsInfo", 22},
    };

    using CreateIRendererType = m3d::rend::IRenderer* (__cdecl*)(m3d::Kernel*);
    using CreateIInputType = m3d::input::IInput* (__cdecl*)(m3d::Kernel*);
    using CreateISoundType = snd::ISound* (__cdecl*)(m3d::Kernel*);
}

namespace ai
{
    extern CServer* pServer;
}

namespace m3d
{
    Application* Application::g_pApp = nullptr;

    int Application::init(HINSTANCE hInstance, HICON hIcon, CStr const& configName, HWND forcedWnd, CStr const& cmdLine)
    {
        M3D_LOG_INFO(m_strWindowTitle);

        m_cmdLine.Init(cmdLine.c_str());
        if (m_cmdLine.CheckParam("-console"))
        {
            m_isConsoleAllowed = true;
        }

        auto const startTime = g_Kernel->GetTimer().GetCurTime();

        InitAllocCheck(ACOutput_Advanced, 1, 0);
        m_cfgName = configName;
        m_frameClearColor = false;

        if (!CheckAndLogPlatform())
        {
            return 0;
        }
        if (g_Kernel->GetEngineCfg().Load(configName))
        {
            SetCodepage();
            if (!createRenderer())
            {
                return 0;
            }
            if (!m_renderer->Create(logDeviceFunc, g_Kernel))  //TODO: logFunc
            {
                M3D_LOG_ERR("ERROR! Application::init -- cannot create device");
                return false;
            }
            RegisterConsoleCommands();
            //TODO: forcedWnd is right?
            g_Kernel->GetEngineCfg().m_mainWnd = forcedWnd;
            if (!g_Kernel->GetEngineCfg().m_mainWnd)
            {
                WNDCLASS wndClass = {};
                wndClass.lpfnWndProc = &Application::WndProc;
                wndClass.cbClsExtra = 0;
                wndClass.cbWndExtra = 0;
                wndClass.hInstance = hInstance; //TODO: not sure about hInstance
                wndClass.hIcon = hIcon;
                wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
                wndClass.hbrBackground = 0;
                wndClass.lpszMenuName = 0;
                wndClass.lpszClassName = TEXT("Miracle3d");
                ::RegisterClass(&wndClass);

                RECT rc;
                ::SetRect(&rc, 0, 0, g_Kernel->GetEngineCfg().m_r_width.GetI(), g_Kernel->GetEngineCfg().m_r_height.GetI());
                auto const windowStyle = GetStyleForRenderWindow(g_Kernel->GetEngineCfg().m_r_fullScreen.GetB());
                ::AdjustWindowRect(&rc, windowStyle, FALSE);
                g_Kernel->GetEngineCfg().m_mainWnd = ::CreateWindowEx(
                    0,
                    TEXT("Miracle3d"),
                    m_strWindowTitle,
                    windowStyle,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    rc.right - rc.left,
                    rc.bottom - rc.top,
                    NULL,
                    NULL,
                    hInstance,
                    NULL
                );
            }
            m_dwWindowStyle = ::GetWindowLongPtr(g_Kernel->GetEngineCfg().m_mainWnd, GWL_STYLE);
            ::GetWindowRect(g_Kernel->GetEngineCfg().m_mainWnd, &m_rcWindowBounds);
            ::GetClientRect(g_Kernel->GetEngineCfg().m_mainWnd, &m_rcWindowClient);
            if (m_rcWindowBounds.bottom == m_rcWindowBounds.top || m_rcWindowBounds.left == m_rcWindowBounds.right)
            {
                M3D_LOG_INFO("WARNING! Application::init -- zero window rect, expanding");
                ::SetWindowPos(g_Kernel->GetEngineCfg().m_mainWnd, 0, 0, 0, 800, 600, SWP_NOMOVE | SWP_NOZORDER);
                ::GetWindowRect(g_Kernel->GetEngineCfg().m_mainWnd, &m_rcWindowBounds);
                ::GetClientRect(g_Kernel->GetEngineCfg().m_mainWnd, &m_rcWindowClient);
            }
            if (!g_Kernel->GetEngineCfg().m_r_fullScreen.GetB())
            {
                g_Kernel->GetEngineCfg().m_r_width.SetI(m_rcWindowClient.right);
                g_Kernel->GetEngineCfg().m_r_height.SetI(m_rcWindowClient.bottom);
            }
            if (m_renderer->CreateDevice())
            {
                if (!g_Kernel->GetEngineCfg().m_r_fullScreen.GetB())
                {
                    ::SetWindowPos(
                        g_Kernel->GetEngineCfg().m_mainWnd,
                        HWND_NOTOPMOST,
                        m_rcWindowBounds.left,
                        m_rcWindowBounds.top,
                        m_rcWindowBounds.right - m_rcWindowBounds.left,
                        m_rcWindowBounds.bottom - m_rcWindowBounds.top,
                        SWP_SHOWWINDOW
                    );
                }

                auto const viewport = m_renderer->GetViewport();
                auto viewportHeight = viewport.m_height;
                auto viewportWidth = viewport.m_width;
                g_Kernel->GetEngineCfg().m_console->Init(viewportWidth, viewportHeight);

                m_prevMouseCoord.x = m_mouseX;
                m_prevMouseCoord.y = m_mouseY;
                g_pApp->m_renderer->AbsToRel(m_prevMouseCoord.x, m_prevMouseCoord.y);

                m_renderWindow = g_Kernel->GetEngineCfg().m_mainWnd;
                if (forcedWnd)
                {
                    M3D_LOG_INFO("NOTE! Application::init -- input module was not inited deliberately");
                }
                else if (!createInput())
                {
                    M3D_LOG_ERR("ERROR! Application::init -- cannot initialize input");
                    doneRenderer();
                    return 0;
                }
                else if (!createSound())
                {
                    g_Kernel->GetEngineCfg().m_snd_Enable.SetB(false);
                    g_Kernel->GetEngineCfg().m_mus_Enable.SetB(false);
                }
                createSprite();
                createProcTexThread();
                createUi();
                AnimatedModel::CreateTexFileMapping();

                M3dVideoPlayer = new mVideoPlayer{};
                M3dVideoPlayer->Init();

                m_serverStaticModels = ServerFactories::CreateStaticModelServer();
                m_serverAnimatedModels = ServerFactories::CreateAnimatedModelServer();
                m_serverLights = ServerFactories::CreateLightsServer();
                m_serverSprites = ServerFactories::CreateSpritesServer();
                m_serverLines = ServerFactories::CreateLinesServer();
                m_serverParticles = ServerFactories::CreateParticlesServer();
                m_serverSound = ServerFactories::CreateSoundServer();
                m_serverMusic = ServerFactories::CreateMusicServer();
                m_serverProjectors = ServerFactories::CreateProjectorsServer();
                m_serverDecals = ServerFactories::CreateDecalsServer();
                if (g_Kernel->GetEngineCfg().m_hasServers.GetB())
                {
                    m_serverStaticModels->Init();
                    m_serverAnimatedModels->Init();
                    m_serverLights->Init();
                    m_serverSprites->Init();
                    m_serverLines->Init();
                    m_serverParticles->Init();
                    m_serverSound->Init();
                    m_serverMusic->Init();
                    m_serverProjectors->Init();
                    m_serverDecals->Init();
                }

                if (!InitImpulses())
                {
                    M3D_LOG_ERR("Error: fail to init impulses");
                    //TODO: doneRenderer???
                    return 0;
                }

                ai::CServer::Register();
                ai::pServer = new ai::CServer{};

                EnableDXCursor(g_Kernel->GetEngineCfg().m_r_dxcursor.GetB());
                if (g_Kernel->GetEngineCfg().m_clipCursorWithinRenderWnd.GetB())
                {
                    CaptureAndClipSystemCursor(true);
                }
                if (InitMedia())
                {
                    g_Kernel->GetEngineCfg().m_console->executeCommand("/conScript data\\scripts\\core.lua");
                    g_Kernel->GetEngineCfg().m_levFileName.Set("Empty", true);

                    auto const diffTime = g_Kernel->GetTimer().GetCurTime() - startTime;
                    M3D_LOG_INFO("----------------------- Engine inited in: " + CStr(diffTime));
                    return 1;
                }
                doneUi();
                doneProcTexThread();
                doneSprite();
                doneSound();
                doneInput();
                doneRenderer();
                return 0;
            }
            g_Kernel->MessageBox(NULL, TEXT("error"), TEXT("cannot initialize 3d"), MB_ICONHAND);
            M3D_LOG_ERR("ERROR! Application::init -- cannot initialize 3d");
            return 0;
        }
        M3D_LOG_ERR("ERROR! Application::init -- cannot open cfg: " + configName);
        g_Kernel->MessageBox(NULL, TEXT("Error!!! Can't open config!!!"), TEXT("Critical error"), MB_ICONHAND);
        return 0;
    }

    bool Application::IsMouseYAxisFlipped() const
    {
        return m_bMouseYAxisFlipped;
    }

    int Application::ImmediateMessage(int msg, int p0, int p1, int p2, int p3, CStr const& p4, AIParam const& p5)
    {
        Event ev;
        ev.m_timeStamp = g_Kernel->GetTimer().GetCurTime();
        ev.m_eventType = msg;   //TODO: check this
        ev.m_intEv[0] = p0;
        ev.m_intEv[1] = p1;
        ev.m_intEv[2] = p2;
        ev.m_intEv[3] = p3;
        ev.m_strEv = p4;
        ev.m_aiParamEv = p5;
        return HandleEvent(ev);
    }

    bool Application::IsMouseXAxisFlipped() const
    {
        return m_bMouseXAxisFlipped;
    }

    int Application::OneFrame()
    {
        if (M3dVideoPlayer->IsVideoPlaing())
        {
            m_enginePlayingVideo = true;
            M3dVideoPlayer->Update();
            return 1;
        }

        m_enginePlayingVideo = false;
        if (!m_waitForAnykey)
        {
            g_Kernel->GetTimer().NewFrame();
            g_pApp->m_renderer->ResetStats();
            if (g_Kernel->GetEngineCfg().m_snd_Enable.GetB() ||
                g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
            {
                auto const lastFrameTime = g_Kernel->GetTimer().GetLastFrameTime() * 0.001;
                g_pApp->m_sound->Update(lastFrameTime);
            }
            FrameMove();
            if (!m_waitForAnykey && m_isRenderingAllowed)
            {
                auto rend = m_renderer->CanRender();
                if (rend)
                {
                    auto* renderProfiler = m_profilerStack.GetProfiler(m_profiler_Render);
                    FrameProfilerPtr renderProfilerPtr(renderProfiler);
                    if (m_renderer->BeginScene())
                    {
                        //TODO: ClearViewport second arg
                        if (m_renderer->IsFeatureSupported(rend::FEATURE_STENCIL))
                        {
                            m_renderer->ClearViewport(rend::M3DCLEAR_CZS, m_frameClearColor);
                        }
                        else
                        {
                            m_renderer->ClearViewport(rend::M3DCLEAR_CZ, m_frameClearColor);
                        }
                        //TODO: check this
                        if (rend == 2 ||m_appNeedToRedraw)
                        {
                            Render(true);
                            m_appNeedToRedraw = 0;
                        }
                        else
                        {
                            Render(false);
                        }
                        auto* uiProfiler = m_profilerStack.GetProfiler(m_profiler_UiRender);
                        FrameProfilerPtr uiProfilerPtr(uiProfiler);
                        Repaint();
                        FlushGfx(m_renderer);
                        //TODO: theck this
                        g_pApp->m_renderer->PushZbState(rend::ZB_DISABLE);
                        if (m_bDrawGraph)
                        {
                            RETRUXX_NOT_IMPLEMENTED;
                        }
                        if (m_bDrawMemoryStats)
                        {
                            RETRUXX_NOT_IMPLEMENTED;
                        }
                        if (m_bDrawStats)
                        {
                            RETRUXX_NOT_IMPLEMENTED;
                        }
                        if (m_bDrawCounters)
                        {
                            RETRUXX_NOT_IMPLEMENTED;
                        }
                        g_pApp->m_renderer->PopZbState();
                        m_counterStack.ClearStringStack();
                        if (g_Kernel->GetEngineCfg().m_console)
                        {
                            g_Kernel->GetEngineCfg().m_console->Render();
                        }
                        g_pApp->m_renderer->PushZbState(rend::ZB_DISABLE);
                        if (m_bShowDeviceMemStats)
                        {
                            RETRUXX_NOT_IMPLEMENTED;
                        }
                        if (m_bShowRenderStats)
                        {
                            RETRUXX_NOT_IMPLEMENTED;
                        }
                        if (g_Kernel->GetEngineCfg().m_g_showEffectsStats.GetB())
                        {
                            g_pApp->m_renderer->ShowStats();
                        }
                        g_pApp->m_renderer->PopZbState();
                        m_renderer->EndScene();
                    }
                    if (m_screenShotPending)
                    {
                        RETRUXX_NOT_IMPLEMENTED;
                    }
                    m_renderer->PresentScene();
                }
                //TODO: add strings
                CStr fps = " fps [";
            }
        }
        return 1;
    }

    int Application::run()
    {
        while (!m_breakLoop)
        {
            if (!processWinMessages())
            {
                break;
            }
            FillEngineMessages();

            m_profilerStack.StartFrame();
            if (m_isAppActive)
            {
                NewFrame();
            }

            ProcessAllEvents();
            if (m_isAppActive)
            {
                auto* profiler = m_profilerStack.GetProfiler(m_profiler_OneFrame);
                profiler->StartCountdown();
                auto const frameRes = OneFrame();
                profiler->EndCountdown();
                if (!frameRes)
                {
                    EnqueueMessage(1, 0, 0, 0, 0, {}, {});
                }
            }
            m_profilerStack.EndFrame();
        }
        return 1;
    }

    int Application::processWinMessages()
    {
        //TODO: wmquitmsg - static atomic or what?
        static bool wmQuitMsg = false;
        m_mouseInfo.ResetDelta();
        MSG msg;
        if (!PeekMessage(&msg, 0, 0, 0, 1))
        {
            return !wmQuitMsg;
        }
        while (msg.message != 18)
        {
            if (!m_isAppActive && GetMessage(&msg, 0, 0, 0))
            {
                do
                {
                    if (m_isAppActive)
                        break;
                    DispatchMessageA(&msg);
                } while (GetMessageA(&msg, 0, 0, 0));
            }
            DispatchMessage(&msg);
            if (!PeekMessage(&msg, 0, 0, 0, 1))
            {
                return !wmQuitMsg;
            }
        }
        wmQuitMsg = true;
        return 0;
    }

    int Application::FillEngineMessages()
    {
        if (m_input)
        {
            NewFrame();
        }
        if (!m_input)
        {
            return 1;
        }
        unsigned short key = 0;
        unsigned char param2 = 0;
        bool param3 = false;
        double time = 0.0;

        if (m_input->GetLastKbdEvent(key, param2, param3, time, true))
        {
	        do
	        {
                Event ev;
                ev.m_timeStamp = time;
                ev.m_ushortEv[0] = key;
                if (!key && param2 == 0x9C)
                {
                    key |= 4;
                }
                ev.m_byteEv[3] = param2;
                auto oldHead = m_eventsQueueHead;
                auto head = m_eventsQueueHead + 1;
                if (head >= 0x1388)
                {
                    m_eventsQueueHead = 0;
                }
                if (m_eventsQueueTail != head)
                {
                    m_eventsQueue[oldHead] = ev;
                    m_eventsQueueHead = head;
                }
            } while (m_input->GetLastKbdEvent(key, param2, param3, time, true));
        }
        auto deltaX = 0;
        auto deltaY = 0;
        if (m_bDXCursorEnabled)
        {
            m_mouseX = m_mouseInfo.GetLastPos().x;
            m_mouseY = m_mouseInfo.GetLastPos().y;
            deltaX = m_mouseInfo.GetDeltaDuringGameFrame().x;
            deltaY = m_mouseInfo.GetDeltaDuringGameFrame().y;
        }
        else
        {
            deltaX = m_input->GetMouseX();
            deltaY = m_input->GetMouseY();
            if (!deltaX && !deltaY)
            {
            LABEL_35:
                m3d::EventType joystickBtnEvents[10];
                joystickBtnEvents[0] = EV_MOUSE_LBTN;
                joystickBtnEvents[1] = EV_MOUSE_RBTN;
                joystickBtnEvents[2] = EV_MOUSE_MBTN;
                int gamepadBtnMask = 0;
                int bits = 0;
                for (int i =0; ; bits = i)
                {
                    auto btnsMask = 1 << bits;
                    auto mouseB = m_input->GetMouseB(bits);
                    auto prevBtnMask = m_prevBtnsMask;
                    auto mask = mouseB != 0 ? btnsMask : 0;
                	gamepadBtnMask |= mask;
                    if (mask != (btnsMask & prevBtnMask))
                    {
                        m3d::Event ev;
                        ev.m_timeStamp = g_Kernel->GetTimer().GetCurTime() * 0.001;
                        ev.m_eventType = joystickBtnEvents[i];
                        ev.m_ushortEv[0] = m_mouseX;
                        ev.m_ushortEv[1] = m_mouseY;
                        ev.m_ushortEv[2] = mask != 0;

                        auto head = m_eventsQueueHead + 1;
                        if (head >= 0x1388)
                        {
                            head = 0;
                        }
                        if (m_eventsQueueTail != head)
                        {
                            m_eventsQueue[m_eventsQueueHead] = ev;
                            m_eventsQueueHead = head;
                        }
                    }
                    if (++i >= 3)
                        break;
                }
                m_prevBtnsMask = gamepadBtnMask;
                if (auto mouseZ = m_input->GetMouseZ())
                {
                    m3d::Event ev;
                    ev.m_timeStamp = g_Kernel->GetTimer().GetCurTime() * 0.001;
                    ev.m_ushortEv[1] = m_mouseY;
                    ev.m_ushortEv[0] = m_mouseX;
                    ev.m_eventType = 15;

                    auto head = m_eventsQueueHead + 1;
                    if (head >= 0x1388)
                    {
                        head = 0;
                    }
                    ev.m_ushortEv[2] = mouseZ / 120;
                    if (m_eventsQueueTail !=head)
                    {
                        m_eventsQueue[m_eventsQueueHead] = ev;
                        m_eventsQueueHead = head;
                    }
                }
                auto joyB = m_input->GetParam(input::DP_JOY_B);
                gamepadBtnMask = joyB;
                if (joyB != m_prevJoystickBtnsMask)
                {
                    joystickBtnEvents[0] = EV_JOYSTICK_BTN0;
                    joystickBtnEvents[1] = EV_JOYSTICK_BTN1;
                    joystickBtnEvents[2] = EV_JOYSTICK_BTN2;
                    joystickBtnEvents[3] = EV_JOYSTICK_BTN3;
                    joystickBtnEvents[4] = EV_JOYSTICK_BTN4;
                    joystickBtnEvents[5] = EV_JOYSTICK_BTN5;
                    joystickBtnEvents[6] = EV_JOYSTICK_BTN6;
                    joystickBtnEvents[7] = EV_JOYSTICK_BTN7;
                    joystickBtnEvents[8] = EV_JOYSTICK_BTN8;
                    joystickBtnEvents[9] = EV_JOYSTICK_BTN9;
                    for (int j = 0; j < 0xA; ++j)
                    {
                        auto btnsMask = joyB & (1 << j);
                        if (btnsMask !=((1 <<j) & m_prevJoystickBtnsMask))
                        {
                            m3d::Event ev;
                            ev.m_timeStamp = g_Kernel->GetTimer().GetCurTime() * 0.001;
                            ev.m_eventType = joystickBtnEvents[j];
                            ev.m_ushortEv[2] = btnsMask != 0;


                            auto head = m_eventsQueueHead + 1;
                            if (head >= 0x1388)
                            {
                                head = 0;
                            }
                            if (m_eventsQueueTail != head)
                            {
                                m_eventsQueue[m_eventsQueueHead] = ev;
                                m_eventsQueueHead = head;
                            }
                        }
                        joyB = gamepadBtnMask;
                    }
                }
                m_prevJoystickBtnsMask = gamepadBtnMask;
                return 1;
            }
            m_mouseX += deltaX;
            m_mouseY += deltaY;
            auto viewport = m_renderer->GetViewport();
            if (m_mouseX < viewport.m_x0)
            {
                m_mouseX = viewport.m_x0;
            }
            if (this->m_mouseX > viewport.m_width + viewport.m_x0 - 1)
                this->m_mouseX = viewport.m_width + viewport.m_x0 - 1;
            if (this->m_mouseY < viewport.m_y0)
                this->m_mouseY = viewport.m_y0;
            if (this->m_mouseY > viewport.m_height + viewport.m_y0 - 1)
                this->m_mouseY = viewport.m_height + viewport.m_y0 - 1;
        }
        if (deltaX || deltaY)
        {
            m3d::Event ev;
            ev.m_timeStamp = g_Kernel->GetTimer().GetCurTime() * 0.001;
            ev.m_ushortEv[0] = m_mouseX;
            ev.m_ushortEv[1] = m_mouseY;
            ev.m_ushortEv[2] = deltaX;
            ev.m_ushortEv[3] = deltaY;
            ev.m_eventType = 9;

            auto head = m_eventsQueueHead + 1;
            if (head >= 0x1388)
            {
                head = 0;
            }
            if (m_eventsQueueTail != head)
            {
                m_eventsQueue[m_eventsQueueHead] = ev;
                m_eventsQueueHead = head;
            }
        }
        goto LABEL_35;
        return 1;
    }

    void Application::ProcessAllEvents()
    {
        do
        {
            auto tail = m_eventsQueueTail;
            if (tail == m_eventsQueueHead)
            {
                break;
            }
            auto const ev = m_eventsQueue[tail];
            auto newTail = m_eventsQueueTail + 1;
            if (newTail >= 0x1388)
            {
                newTail = 0;
            }
            m_eventsQueueTail = newTail;
            HandleEvent(ev);
            
        } while (!m_breakLoop);
    }

    int Application::CheckAndLogPlatform()
    {
        M3D_LOG_INFO("Path: " + m_startupFolder);
        M3D_LOG_INFO("Exe: " + m_imageName);

        OSVERSIONINFO osinfo;
        memset(&osinfo, 0, sizeof(OSVERSIONINFO));
        osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        ::GetVersionEx(&osinfo);
        M3D_LOG_INFO(
            "Windows version: " +
            CStr(osinfo.dwMajorVersion) + "." +
            CStr(osinfo.dwMinorVersion) + "." +
            CStr(osinfo.dwBuildNumber)
        );

        TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
        DWORD size = sizeof(computerName);
        ::GetComputerName(computerName, &size);
        M3D_LOG_INFO("Computer name: " + CStr(computerName));

        char cpuInfo[16] = { 0 };
        __cpuid(reinterpret_cast<int*>(cpuInfo), 0x80000000);

        //TODO: other info...
        //RETRUXX_NOT_IMPLEMENTED;
        return 1;
    }

    long Application::MsgProc(HWND hWnd, unsigned uMsg, unsigned wParam, long lParam)
    {
        //TODO: check this and refactor
        int result; // eax
        PointBase<int> curMousePos; // [esp+8h] [ebp-30h] BYREF
        CStr param4; // [esp+10h] [ebp-28h] BYREF

        if (uMsg <= 0x21)
        {
            if (uMsg < 0x20)
            {
                switch (uMsg)
                {
                case 2u:
                    PostQuitMessage(0);
                    result = 0;
                    break;
                case 6u:
                case 7u:
                    goto $L141541;
                case 0x10u:
                    DestroyWindow(hWnd);
                    result = 0;
                    break;
                case 0x1Cu:
                    ImmediateMessage(2, wParam, 0, 0, 0, {}, {});
                    result = 0;
                    break;
                default:
                    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
                }
                return result;
            }
        $L141541:
            SetCursor(0);
            if (this->m_bDXCursorEnabled)
            {
                if (M3D_RENDERER)
                    M3D_RENDERER->ShowDXCursor(1);
            }
            return 1;
        }
        if (uMsg > 0x200)
        {
            if (uMsg == 536)
                return 1112363332;
            return DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
        if (uMsg != 512)
        {
            if (uMsg == 134)
                goto $L141541;
            if (uMsg == 274 && ((wParam & 0xFFF0) == 61760 || (wParam & 0xFFF0) == 61808))
                return 0;
            return DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
        if (!this->m_isAppActive || !this->m_bDXCursorEnabled)
            return 0;
        curMousePos.x = LOWORD(lParam);
        curMousePos.y = HIWORD(lParam);
        m_mouseInfo.SetUpForCurPos(curMousePos);
        if (this->m_bDXCursorEnabled && M3D_RENDERER)
            M3D_RENDERER->MoveDXCursor(LOWORD(lParam), HIWORD(lParam));
        return 0;
    }

    void Application::SetCodepage()
    {
        //TODO: check this
        retruxx::set<size_t> codePagesStrings;
        codePagesStringsPtr = &codePagesStrings;
        ::EnumSystemCodePages(EnumCodePagesProc, CP_INSTALLED);
        CStr const codePageName = g_Kernel->GetEngineCfg().m_ui_codePageName.GetS();
        UINT codePage = 0;
        if (codePageName == CStr("CP_UTF8"))
        {
            codePage = CP_UTF8;
        }
        else if (auto const offset = codePageName.findsubstr("windows-"); offset != CStr_npos)
        {
            codePage = std::atoi(codePageName.substr(strlen("windows-")).c_str());
        }
        if (::GetCPInfoEx(codePage, 0, &g_pApp->m_codePage) == FALSE)
        {
            M3D_LOG_INFO("SetCodepage -- code page is not supported : " + codePageName + " forcing ANSI, some chars will be not available");
            //TODO: handle this
            if (::GetCPInfoEx(0, 0, &g_pApp->m_codePage) == FALSE)
            {
                SYS_ERROR("GetCPInfoEx ANSI failed");
            }
        }
        M3D_LOG_INFO(CStr("SetCodepage -- using codepage ") + g_pApp->m_codePage.CodePageName);
     }

    int Application::createRenderer()
    {
        CStr inputDriverName("dxrender9.dll");
        m_hRenderDll = ::LoadLibrary(inputDriverName.c_str());
        const auto ee = ::GetLastError();
        if (m_hRenderDll == NULL)
        {
            M3D_LOG_ERR("ERROR! Application::CreateRenderer -- cannot locate renderer driver " + inputDriverName);
            M3D_LOG_ERR("GetLastError() = " + CStr(::GetLastError()));
            return 0;
        }
        auto createIRenderer = reinterpret_cast<CreateIRendererType>(::GetProcAddress(m_hRenderDll, "createIRenderer"));
        if (createIRenderer == NULL)
        {
            M3D_LOG_ERR("ERROR! Application::CreateRenderer -- cannot get factory");
        }

        m_renderer = createIRenderer(g_Kernel);
        m_renderer->IncRef();
        M3D_LOG_INFO("NOTE! renderer is bind to " + inputDriverName);
        return true;
    }

    LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return g_pApp->MsgProc(hWnd, uMsg, wParam, lParam);
    }

    void Application::doneRenderer()
    {
        if (m_renderer)
        {
            m_renderer->DecRef();
            m_renderer = nullptr;
        }
        if (m_hRenderDll != nullptr)
        {
            ::FreeLibrary(m_hRenderDll);
        }
    }

    void Application::RegisterConsoleCommands()
    {
        auto& config = g_Kernel->GetEngineCfg();
        for (auto const& command : conCommands)
        {
            config.m_console->RegisterCommand(command.m_name, command.m_id, this);
        }
    }

    __int64 Application::GetMeasuredCpuFrequency() const
    {
        return m_cpuSpeed;
    }

    int Application::createInput()
    {
        CStr inputDriverName("input_di8.dll");
        m_hInputDll = ::LoadLibrary(inputDriverName.c_str());
        if (m_hInputDll == NULL)
        {
            M3D_LOG_ERR("ERROR! Application::CreateInput -- cannot locate input driver " + inputDriverName);
            M3D_LOG_ERR("GetLastError() = " + CStr(::GetLastError()));
            return 0;
        }
        auto createIInput = reinterpret_cast<CreateIInputType>(::GetProcAddress(m_hInputDll, "createIInput"));
        if (createIInput == NULL)
        {
            M3D_LOG_ERR("ERROR! Application::CreateInput -- cannot get factory");
        }

        m_input = createIInput(g_Kernel);
        m_input->IncRef();
        M3D_LOG_INFO("NOTE! input is bind to " + inputDriverName);
        m_input->Init(g_Kernel, logInputFunc);
        return 1;
    }

    int Application::createSound()
    {
        //TODO: check correctness
        auto& config = g_Kernel->GetEngineCfg();
        if (!config.m_snd_Enable.GetB() && !config.m_mus_Enable.GetB())
        {
            return 1;
        }
        CStr inputDriverName("sound.dll");
        m_hSoundDll = ::LoadLibrary(inputDriverName.c_str());
        if (m_hSoundDll == NULL)
        {
            M3D_LOG_ERR("ERROR! m3dApplication::CreateSound -- cannot locate sound driver " + inputDriverName);
            M3D_LOG_ERR("GetLastError() = " + CStr(::GetLastError()));
            config.m_snd_Enable.SetB(false);
            config.m_mus_Enable.SetB(false);
            return 0;
        }
        auto createISound = reinterpret_cast<CreateISoundType>(::GetProcAddress(m_hSoundDll, "createISound"));
        if (createISound == NULL)
        {
            M3D_LOG_ERR("ERROR! m3dApplication::CreateSound -- cannot get factory");
            config.m_snd_Enable.SetB(false);
            config.m_mus_Enable.SetB(false);
            return 0;
        }
        for (int trial = 0; trial < 3; ++trial)
        {
            m_sound = createISound(g_Kernel);
            if (m_sound != nullptr)
            {
                break;
            }
            M3D_LOG_INFO("Warning: sound was not created after trial " + CStr(trial));
            ::Sleep(1000);
        }
        if (m_sound != nullptr)
        {
            m_sound->IncRef();
            M3D_LOG_INFO("NOTE! sound is bind to " + inputDriverName);
            auto isSoundInit = false;
            for (int trial = 0; trial < 3; ++trial)
            {
                auto const sampleRate = config.m_snd_SampleRate.GetI();
                auto const maxSounds = config.m_snd_MaxSounds.GetI();
                auto const pathToSoundGroup = config.m_snd_pathToSoundGroups.GetS();
                auto const bitsPerSample = config.m_snd_BitsPerSample.GetI();
                if (m_sound->Init(logSoundFunc, sampleRate, bitsPerSample, maxSounds, pathToSoundGroup))
                {
                    isSoundInit = true;
                    break;
                }
                M3D_LOG_INFO("Warning: sound was not inited after trial " + CStr(trial));
                ::Sleep(1000);
            }
            if (isSoundInit)
            {
                //TODO: improve logic
                m_soundConHandler = new SoundConHandler{};
                config.m_mus_Volume.SetHandler(m_soundConHandler);
                config.m_snd_2dVolume.SetHandler(m_soundConHandler);
                config.m_snd_3dVolume.SetHandler(m_soundConHandler);
                return true;
            }
        }
        config.m_snd_Enable.SetB(false);
        config.m_mus_Enable.SetB(false);
        return false;
    }

    void Application::doneSprite()
    {
    }

    int Application::createProcTexThread()
    {
        return 1;
    }

    int Application::createSprite()
    {
        return 1;
    }

    int Application::createUi()
    {
        auto& config = g_Kernel->GetEngineCfg();
        auto schemaName = config.m_ui_pathToSchema.GetS();
        auto stringsName = config.m_ui_pathToResource.GetS();
        Create(stringsName, schemaName);
        return 1;
    }

    void Application::doneUi()
    {
        Done();
    }

    void Application::doneInput()
    {
        if (m_input != nullptr)
        {
            m_input->DecRef();
            m_input = nullptr;
        }
        if (m_hInputDll != NULL)
        {
            ::FreeLibrary(m_hInputDll);
        }
    }

    int Application::HandleEvent(Event const& ev)
    {
        //TODO: imlement Application::HandleEvent
        switch (ev.m_eventType)
        {
        case 1:
	    {
            ::PostMessageA(g_Kernel->GetEngineCfg().m_mainWnd, 0x10, 0, 0);
            return 1;
	    }
        case 2:
	    {
            //TODO: check this
            m_isAppActive = ev.m_intEv[0];
            if (m_sound)
            {
                m_sound->PauseAllSounds(m_isAppActive);
            }
            if (m_renderer)
            {
                m_renderer->SetActiveState(m_isAppActive);
            }
            g_Kernel->GetTimer().SetActiveState(m_isAppActive);
            if (m_isAppActive)
            {
	            if (g_Kernel->GetEngineCfg().m_clipCursorWithinRenderWnd.GetB())
	            {
                    CaptureAndClipSystemCursor(true);
	            }
                //TODO: check this
                ShowSystemCursor(m_bDXCursorEnabled);
                if (g_pApp->m_sound)
                {
                    g_pApp->m_sound->PauseAllSounds(false);
                    return 1;
                }
            }
            else
            {
                ReleaseCapture();
                ClipCursor(nullptr);
                while (ShowCursor(1) < 0);
                g_pApp->m_sound->PauseAllSounds(true);
            }
            return 1;
	    }
        case 3:
	    {
            //TODO: check this
            SwitchDisplayModes(g_Kernel->GetEngineCfg().m_mainWnd, ev.m_intEv[0], ev.m_intEv[1], ev.m_intEv[2] != 0);
            return 1;
	    }
        case 4:
        {
            auto viewport = m_renderer->GetViewport();
            M3D_KERNEL->GetEngineCfg().m_console->CheckResize(viewport.m_width, viewport.m_height);
            m_appNeedToRedraw = true;
            break;
        }
        case 7:
            RETRUXX_NOT_IMPLEMENTED;
        case 8:
            RETRUXX_NOT_IMPLEMENTED;
        case 0xA:
        case 0xB:
        case 0xC:
        {
            if (!m_waitForAnykey)
            {
                break;
            }
            m_timeFromLevelLoaded = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
            M3D_KERNEL->GetTimer().SetActiveState(1);
            m_waitForAnykey = false;
            return 1;
        }
        default:
        {
            break;
        }
        }
        if ((ev.m_eventType == 7 || ev.m_eventType == 8) && m_focusKbdEntity != nullptr)
        {
            return m_focusKbdEntity->HandleEvent(ev) != 0;
        }
        //TODO: check this
        if (ev.m_eventType != 15)
        {
            if (M3D_KERNEL->GetEngineCfg().m_console == m_focusKbdEntity)
            {
                return M3D_KERNEL->GetEngineCfg().m_console->HandleEvent(ev) != 0;
            }
            return ProcessEvent(ev);
        }
        if (M3D_KERNEL->GetEngineCfg().m_console->isActive())
        {
            if (M3D_APP)
            {
                SetKeyboardFocus(M3D_APP);
                return 1;
            }
        }
        else
        {
            if (!m_isConsoleAllowed)
            {
                return 1;
            }
            if (M3D_KERNEL->GetEngineCfg().m_console)
            {
                SetKeyboardFocus(M3D_KERNEL->GetEngineCfg().m_console);
                return 1;
            }
        }
        static bool bCtrlShiftActive = false;
        if (bCtrlShiftActive)
        {
            return 1;
        }
        bCtrlShiftActive = true;
        ChangeLanguage();
        return 1;
    }

    void Application::doneProcTexThread()
    {
    }

    void Application::doneSound()
    {
        if (m_sound != nullptr)
        {
            m_sound->DecRef();
            m_sound = nullptr;
        }
        if (m_hSoundDll != NULL)
        {
            ::FreeLibrary(m_hSoundDll);
        }
    }

    int Application::OnLoosingFocus()
    {
        if (m_pImpulses)
        {
            m_pImpulses->ResetAllImpulses(false);
        }
        m_gotFocus = false;
        //TODO: check this
        m_mouseDown = 0;
        return 1;
    }

    int Application::InitImpulses()
    {
        return 1;
    }

    void Application::DoneImpulses()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DetailSettings::RestoreGameSettings()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DetailSettings::SaveGameSettings()
    {
        m_dsShadows = g_Kernel->GetEngineCfg().m_dsShadows.GetB();
        m_lsViewDistanceDivider = g_Kernel->GetEngineCfg().m_lsViewDistanceDivider.GetF();
        m_NPatchLevel = g_Kernel->GetEngineCfg().m_NPatchLevel.GetF();
    }

    void Application::DetailSettings::SetMenuLevelSettings()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Application::DetailSettings::DetailSettings()
    {
        SaveGameSettings();
    }

    Application::LoadScreenInfo::LoadScreenInfo()
    {
    }

    Application::LoadScreenInfo::~LoadScreenInfo()
    {
        if (m_picture.IsValid())
        {
            g_pApp->m_renderer->ReleaseTexture(m_picture);
        }
    }

    void Application::MouseInfo::ResetDelta()
    {
        m_deltaDuringGameFrame.x = 0;
        m_deltaDuringGameFrame.y = 0;
    }

    PointBase<int> const& Application::MouseInfo::GetDeltaDuringGameFrame() const
    {
        return m_deltaDuringGameFrame;
    }

    PointBase<int> const& Application::MouseInfo::GetLastPos() const
    {
        return m_lastPos;
    }

    Application::MouseInfo::MouseInfo() :
        m_deltaDuringGameFrame(0, 0),
        m_lastPos(0, 0)
    {
    }

    void Application::MouseInfo::SetUpForCurPos(PointBase<int> const& curPos)
    {
        m_deltaDuringGameFrame.x += curPos.x - m_lastPos.x;
        m_deltaDuringGameFrame.y += curPos.y - m_lastPos.y;
        m_lastPos = curPos;
    }

    DataServer& Application::GetSoundServer()
    {
        return *m_serverSound;
    }

    void Application::texGenProcess(IGeneratedTexture*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DrawTri(CVector*, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::ForbidRendering()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* Application::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer& Application::GetStaticModelsServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DrawCross(CVector const&, float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& Application::GetImageName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::GetCurDifficultyLevel() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    char* Application::GetWindowTitle() const
    {
        return m_strWindowTitle;
    }

    void Application::MiniDump()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DrawLogo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Application::GetOnScreenSize(CVector const& o, float radius)
    {
        CVector myOrg = o;
        CVector camOrg = M3D_RENDERER->MatGetOrgInv();
        auto oa = sqrt(
            (myOrg.z - camOrg.z) * (myOrg.z - camOrg.z)
            + (myOrg.y - camOrg.y) * (myOrg.y - camOrg.y)
            + (myOrg.x - camOrg.x) * (myOrg.x - camOrg.x));

        CVector r, u, f;
        M3D_RENDERER->MatGetBasis(r, u, f);

        CVector v1;
        CVector v0;
        v0.x = f.x * oa;
        v0.y = f.y * oa;
        v0.z = f.z * oa;
        v1.x = (r.x * radius) + (f.x * oa);
        v1.y = (r.y * radius) + (f.y * oa);
        v1.z = (r.z * radius) + (f.z * oa);

        auto res0 = M3D_RENDERER->Project(v0);
        auto res1 = M3D_RENDERER->Project(v1);
        return fabs(res1.x - res0.x);
    }

    int Application::HandleCinematic(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PutSplashCallBack(int proc, void* data)
    {
        M3D_APP->PutSplash(proc, *(const char**)data);
    }

    void Application::AllowRendering()
    {
        m_isRenderingAllowed = 1;
    }

    bool Application::StartPlayingMusic(char const* musicName, bool bDoLoop, bool bImmediate)
    {
        auto item = M3D_APP->m_serverMusic->GetItemByName(musicName, true);
        if (item == -1)
        {
            if (M3D_KERNEL->GetEngineCfg().m_mus_Enable.GetB())
            {
                M3D_LOG_ERR("Error: invalid music name: '" + CStr(musicName) + "'");
                return false;
            }
        }

        struct RenderInfo
        {
            /* 0x0000 */ bool m_bDoLoop;
            /* 0x0001 */ bool m_bImmediate;
            /* 0x0002 */ char Padding_330[2];
            /* 0x0004 */ int m_channelId;
        }; /* size: 0x0008 */

        RenderInfo ri;
        ri.m_bDoLoop = bDoLoop;
        ri.m_bImmediate = bImmediate;
        ri.m_channelId = -1;
        M3D_APP->m_serverMusic->RenderItem(item, &ri);
    }

    DataServer& Application::GetProjectorsServer()
    {
        return *m_serverProjectors;
    }

    bool Application::CheckCommandLineParam(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned long Application::GetLocale() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DrawQuad(CVector*, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::SaveToXml(cmn::XmlFile*, cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Application::~Application()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::TexSoften(rend::TexHandle, rend::TexHandle, int, float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Application::IsTextHieroglyphic(CStr const&) const
    {
        return false;
    }

    CStr Application::GetNativeFuncDesc(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::StopPlayingMusic()
    {
        if (g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
        {
            g_pApp->m_sound->StopGroup(0);
        }
    }

    void Application::SetMouseSensitivity(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Application::getZoom()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x006A6FC0, Application::LoadServers)
    bool Application::LoadServers(CStr const& filename, bool bQuiet)
    {
        struct {
            m3d::DataServer* m_server;
            const char* m_name;
            CStr m_diz;
        } servers[9];

        servers[0].m_server = this->m_serverAnimatedModels;
        servers[0].m_diz = GetStringByStringId0("AnimatedModelsServer");
        servers[1].m_server = m_serverStaticModels;
        servers[1].m_diz = GetStringByStringId0("StaticModelsServer");
        servers[2].m_server = m_serverLights;
        servers[2].m_diz = GetStringByStringId0("LightsServer");
        servers[3].m_server = m_serverSprites;
        servers[3].m_diz = GetStringByStringId0("SpritesServer");
        servers[4].m_server = m_serverParticles;
        servers[4].m_diz = GetStringByStringId0("ParticlesServer");
        servers[5].m_server = m_serverSound;
        servers[5].m_diz = GetStringByStringId0("SoundsServer");
        servers[6].m_server = m_serverMusic;
        servers[6].m_diz = GetStringByStringId0("MusicServer");
        servers[7].m_server = m_serverProjectors;
        servers[0].m_name = "AnimatedModelsServer";
        servers[1].m_name = "StaticModelsServer";
        servers[2].m_name = "LightsServer";
        servers[3].m_name = "SpritesServer";
        servers[4].m_name = "ParticlesServer";
        servers[5].m_name = "SoundsServer";
        servers[6].m_name = "MusicServer";
        servers[7].m_name = "ProjectorsServer";
        servers[7].m_diz = GetStringByStringId0("ProjectorsServer");
        servers[8].m_server = m_serverDecals;
        servers[8].m_name = "DecalsServer";
        servers[8].m_diz = GetStringByStringId0("DecalsServer");

        M3D_LOG_INFO("Loading Servers: " + filename);

        CStr err;
        if (ref_ptr xmlFile = m3d::ReadXmlFile(filename.c_str(), &err))
        {
            ref_ptr serversNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(serversNode, "Servers");
            if (serversNode->IsEmpty())
            {
                M3D_LOG_INFO("Load servers: cannot find Servers");
                return 0;
            }

            ref_ptr node = xmlFile->CreateNode();
            for (const auto& server : servers)
            {
                auto loadingSplash = GetStringByStringId0("Loading") + " " + server.m_diz;
                if (bQuiet)
                {
                    server.m_server->m_fnLoadCallback = PutSplashCallBackQuiet;
                }
                else
                {
                    server.m_server->m_fnLoadCallback = m3d::Application::PutSplashCallBack;
                }
                server.m_server->m_fnLoadCallbackData = &loadingSplash;

                serversNode->GetFirstChild(node, server.m_name);
                if (node->IsEmpty())
                {
                    continue;
                }

                if (!server.m_server->ReadFromXmlNode(xmlFile, node))
                {
                    M3D_LOG_INFO("Load servers: server " + CStr(server.m_name) + " failed to load");
                    return 0;
                }

                loadingSplash = GetStringByStringId0("Precaching") + ":" + server.m_diz;
                server.m_server->m_fnLoadCallbackData = &loadingSplash;
                server.m_server->RenderItem(-4, 0);
            }
            return 1;
        }
        else
        {
            M3D_LOG_INFO("Load servers: " + err);
        }
        return 0;
    }

    bool Application::SetPostEffectParam(CStr const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::ChangeLanguage()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DrawWireRect(CVector2 const&, CVector2 const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PostLoadServers()
    {
        struct {
            m3d::DataServer* m_server;
            const char* m_name;
            CStr m_diz;
        } servers[9];

        servers[0].m_server = this->m_serverAnimatedModels;
        servers[0].m_diz = GetStringByStringId0("AnimatedModelsServer");
        servers[1].m_server = m_serverStaticModels;
        servers[1].m_diz = GetStringByStringId0("StaticModelsServer");
        servers[2].m_server = m_serverLights;
        servers[2].m_diz = GetStringByStringId0("LightsServer");
        servers[3].m_server = m_serverSprites;
        servers[3].m_diz = GetStringByStringId0("SpritesServer");
        servers[4].m_server = m_serverParticles;
        servers[4].m_diz = GetStringByStringId0("ParticlesServer");
        servers[5].m_server = m_serverSound;
        servers[5].m_diz = GetStringByStringId0("SoundsServer");
        servers[6].m_server = m_serverMusic;
        servers[6].m_diz = GetStringByStringId0("MusicServer");
        servers[7].m_server = m_serverProjectors;
        servers[0].m_name = "AnimatedModelsServer";
        servers[1].m_name = "StaticModelsServer";
        servers[2].m_name = "LightsServer";
        servers[3].m_name = "SpritesServer";
        servers[4].m_name = "ParticlesServer";
        servers[5].m_name = "SoundsServer";
        servers[6].m_name = "MusicServer";
        servers[7].m_name = "ProjectorsServer";
        servers[7].m_diz = GetStringByStringId0("ProjectorsServer");
        servers[8].m_server = m_serverDecals;
        servers[8].m_name = "DecalsServer";
        servers[8].m_diz = GetStringByStringId0("DecalsServer");
        for (auto& server : servers)
        {
            server.m_server->PostLoad();
        }
    }

    void Application::OnBeforeDeviceReset()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::FinishQuads()
    {
        if (this->m_numPointsVerts)
            FlushQuads();
    }

    void Application::setZoom(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PutSpriteRelRot(float, float, float, float, unsigned, float, float, float, float, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::SetMouseXAxisFlipped(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::SetMouseYAxisFlipped(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::SetFlushQuadsShader(rend::IEffect*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::AppActive() const
    {
        return m_isAppActive;
    }

    rend::VertexXYZCT1* Application::RenderQuadXyzct1GetNextPtr()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::LoadFromXml(cmn::XmlFile*, cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::SwitchDisplayModes(HWND__*, int, int, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::done()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::OnAfterDeviceReset()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::ReloadPostEffects()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Application::GetPostEffectParam(CStr const&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    namespace
    {
        bool IsEscSymbolBeforeSymbol(CStr const& str, int symbolPos)
        {
            if (str.empty() || symbolPos <= 0 || symbolPos >= str.length())
                return 0;

            auto result = 0;
            for (int i = 0; i < symbolPos; ++i)
            {
                result = (str[i] == '#' && !result);
            }
            return result;
        }

        void GetWord(CStr const& src, int i, m3d::TextWrapFlags wrapFlags, CStr& word, int& nextWordPos)
        {
            // TODO: generated code
            // Initialize with empty string
            word = "";
            nextWordPos = -1;

            const char* srcStr = src.c_str();
            int srcLen = src.length();

            if (i < 0 || i >= srcLen) {
                nextWordPos = -1;
                return;
            }

            if (wrapFlags == TW_CHAR_WRAP)
            {
                // Character-based wrapping
                nextWordPos = 1; // Default: single character word

                if (!IsEscSymbolBeforeSymbol(src, i))
                {
                    char currentChar = srcStr[i];

                    switch (currentChar)
                    {
                    case '#':
                        // Color code - typically 2 characters (# followed by color code)
                        if (i + 1 < srcLen)
                        {
                            nextWordPos = 2;
                        }
                        break;

                    case '@':
                        // Special command - typically 9 characters
                        if (i + 9 <= srcLen)
                        {
                            nextWordPos = 9;
                        }
                        break;

                    case '|':
                        // Line break character
                        nextWordPos = 0;
                        break;

                    default:
                        // Regular character - keep default of 1
                        break;
                    }
                }
            }
            else
            {
                // Word-based wrapping (space-separated)
                int v8 = 0;

                while (true) {
                    // Find next space or pipe character
                    int foundPos = src.findOneOf(" |", v8 + i);

                    if (foundPos == -1)
                    {
                        // No delimiter found - take rest of string
                        nextWordPos = -1;
                        break;
                    }

                    int relativePos = foundPos + v8;
                    nextWordPos = relativePos;

                    if (relativePos == -1)
                    {
                        break;
                    }

                    char foundChar = srcStr[foundPos + i];

                    if (foundChar != '|')
                    {
                        // Found a space, not a pipe
                        break;
                    }

                    // Found a pipe character
                    if (!IsEscSymbolBeforeSymbol(src, foundPos + i))
                    {
                        // Unescaped pipe - treat as word boundary
                        break;
                    }

                    // Escaped pipe - continue searching
                    if (foundPos + i+ 1 >= srcLen)
                    {
                        nextWordPos = -1;
                        break;
                    }

                    v8 = relativePos + 1;
                    if (nextWordPos == -1)
                    {
                        break;
                    }
                }

                // Handle special case: if we found a space at the beginning
                if (nextWordPos != -1)
                {
                    char boundaryChar = srcStr[i + nextWordPos];
                    if (boundaryChar == ' ' && nextWordPos == 0)
                    {
                        nextWordPos = 1; // Single space word
                    }
                }
            }

            // Extract the actual word
            if (nextWordPos == -1)
            {
                // Take the rest of the string from position i
                if (i < srcLen)
                {
                    word = src.substr(i, srcLen - i);
                }
            }
            else if (nextWordPos == 0)
            {
                // Special case: pipe character as a word
                word = "|";
            }
            else
            {
                // TODO: check this
                // Extract substring of specified length
                word = src.substr(i, nextWordPos+i);
            }
        }
    }

    int Application::GetTextFit(CStr const& strText, PointBase<float>& size, float maxX, TextWrapFlags flags)
    {
        // TODO: generated code
        if (flags == TW_NOWRAP)
        {
            GetTextExtent(strText, size, -1, 0, 0, 0, 0, 0);
            return 1;
        }

        std::vector<ui::FormattedLine> linesOfText;
        CStr src = strText;

        CStr line;
        CStr lhs;
        float x = 0.0f;
        float lineHeight = 0.0f;
        float totalHeight = 0.0f;
        //float maxLineWidth = 0.0f;
        int lastColor = 0;
        int wordCount = 0;
        int lastWordPos = 0;

        int textLen = src.length();
        int currentPos = 0;

        float availableWidth = maxX + 0.01f; // Small epsilon

        while (currentPos < textLen)
        {
            CStr word;
            int nextWordPos;

            GetWord(src, currentPos, flags, word, nextWordPos);

            bool forceBreak = false;
            if (lastWordPos != -1)
            {
                int prevPos = currentPos - 1;
                if (prevPos < textLen && prevPos > 0 && src[prevPos] == '|')
                {
                    forceBreak = !IsEscSymbolBeforeSymbol(src, prevPos);
                }
            }

            // Update position
            if (nextWordPos == 0)
            {
                currentPos++;
            }
            else if (nextWordPos == -1)
            {
                currentPos = textLen;
            }
            else
            {
                int newPos = currentPos + nextWordPos;
                if (newPos < textLen && src[newPos] == '|' && !IsEscSymbolBeforeSymbol(src, newPos))
                {
                    currentPos = newPos + 1;
                }
                else {
                    currentPos = newPos;
                }
            }

            PointBase<float> wordSize;
            if (flags == TW_CHAR_WRAP)
            {
                GetTextExtent(word, wordSize, -1, 0, 0, 0, 0, 0);
            }
            else
            {
                // Handle word wrapping with spaces
                CStr temp = word;
                if (line.c_str()[0] != '\0' && !(lhs == " ") && !(word == " "))
                {
                    temp += " ";
                }
                GetTextExtent(temp, wordSize, -1, 0, 0, 0, 0, 0);
            }

            if (wordSize.x + x > availableWidth || forceBreak)
            {
                // Create new line
                ui::FormattedLine newLine;
                newLine.m_origin.x = 0.0f;
                newLine.m_origin.y = totalHeight;

                if (wordCount > 0)
                {
                    newLine.m_text = line;
                }
                else
                {
                    newLine.m_text = word;
                }

                newLine.m_color = static_cast<unsigned int>(lastColor);
                newLine.m_isHieroglyphic = false;
                newLine.m_format = TF_LEFT;

                linesOfText.push_back(newLine);

                // Update color tracking
                int foundColor = FindLastColorInStr(line);
                if (foundColor != 0)
                {
                    lastColor = foundColor;
                }

                if (lineHeight == 0.0f)
                {
                    lineHeight = wordSize.y;
                }
                totalHeight += lineHeight;

                // Reset line
                if (wordCount > 0)
                {
                    line = word;
                    wordCount = 1;
                }
                else
                {
                    line = "";
                    wordCount = 0;
                }

                GetTextExtent(line, wordSize, -1, 0, 0, 0, 0, 0);
                x = wordSize.x;
                lineHeight = wordSize.y;
            }
            else
            {
                // Add to current line
                if (flags == TW_CHAR_WRAP)
                {
                    line += word;
                }
                else
                {
                    if (line.c_str()[0] != '\0' && !(lhs == " ") && !(word == " "))
                    {
                        line += " ";
                    }
                    line += word;
                }
                wordCount++;

                GetTextExtent(line, wordSize, -1, 0, 0, 0, 0, 0);
                x = wordSize.x;
                if (wordSize.y > lineHeight)
                {
                    lineHeight = wordSize.y;
                }
            }

            lhs = word;
            lastWordPos = nextWordPos;
        }

        // Handle remaining text
        if (line.c_str()[0] != '\0')
        {
            ui::FormattedLine newLine;
            newLine.m_origin.x = 0.0f;
            newLine.m_origin.y = totalHeight;
            newLine.m_text = line;
            newLine.m_color = static_cast<unsigned int>(lastColor);
            newLine.m_isHieroglyphic = false;
            newLine.m_format = TF_LEFT;
            linesOfText.push_back(newLine);
        }

        // Calculate final dimensions
        float maxWidth = 0.0f;
        float totalTextHeight = 0.0f;

        for (const auto& formattedLine : linesOfText)
        {
            PointBase<float> lineSize;
            if (formattedLine.m_text.c_str()[0] != '\0')
            {
                GetTextExtent(formattedLine.m_text, lineSize, -1, 0, 0, 0, 0, 0);
            }
            else
            {
                // Measure a single character for empty lines
                CStr singleChar("A");
                GetTextExtent(singleChar, lineSize, -1, 0, 0, 0, 0, 0);
                lineSize.x = 0.0f;
            }

            if (lineSize.x > maxWidth)
            {
                maxWidth = lineSize.x;
            }
            totalTextHeight += lineSize.y;
        }

        size.x = maxWidth;
        size.y = totalTextHeight;

        return 1;
    }

    int Application::StartExclusiveMsgLoop()
    {
        m_breakLoop = false;
        auto res = run();
        m_breakLoop = false;
        return res;
    }

    DataServer& Application::GetSpritesServer()
    {
        return *m_serverSprites;
    }

    void Application::panic()
    {
        doneProcTexThread();
        if (m_input)
        {
            m_input->DecRef();
            m_input = nullptr;
        }
        if (m_hInputDll)
        {
            ::FreeLibrary(m_hInputDll);
        }
        if (m_renderer)
        {
            m_renderer->DecRef();
            m_renderer = nullptr;
        }
        if (m_hRenderDll)
        {
            ::FreeLibrary(m_hRenderDll);
        }
    }

    bool Application::SaveServers(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Application::GetMouseHitPoint(CVector&, SgNode*&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::StartQuads(rend::VertexType vt)
    {
        m_numPointsVerts = 0;
        CreateQuadsIb();
        if (vt == rend::VERTEX_XYZWCT1)
        {
            m_pointsVertsVb = M3D_APP->m_renderer->GetVbStreaming(rend::VERTEX_XYZWCT1);
            m_pointsVertsSz = sizeof(rend::VertexXYZWCT1);
            m_sourceVerts = m_pointsVertsWct1;
        }
        else if (vt == rend::VERTEX_XYZCT1)
        {
            m_pointsVertsVb = M3D_APP->m_renderer->GetVbStreaming(rend::VERTEX_XYZCT1);
            m_pointsVertsSz = 24;
            m_sourceVerts = m_pointsVertsCt1;
        }
    }

    int Application::GetTextExtent(CStr const& str, PointBase<float>& size, int fid, BoundsBase<float>* csz, int* minc, int* maxc, CStr* leftInvisibleSubstr, CStr* rightInvisibleSubstr)
    {
        // Initialize output parameters
        if (leftInvisibleSubstr && !leftInvisibleSubstr->empty()) {
            leftInvisibleSubstr->erase();
        }
        if (rightInvisibleSubstr && !rightInvisibleSubstr->empty()) {
            rightInvisibleSubstr->erase();
        }

        // Check for empty string
        if (str.empty()) {
            size.x = 0.0f;
            size.y = 0.0f;
            return 0;
        }

        // Get font
        ui::Font* font = nullptr;
        if (fid == -1)
        {
            font = GetGfxServer()->GetCurFont();
        }
        else {
            font = GetGfxServer()->GetFontById(fid);
        }

        if (!font)
        {
            return 0;
        }

        const char* text = str.c_str();
        float totalWidth = 0.0f;
        float maxHeight = 0.0f;
        float scaledWidth = 0.0f;
        int numChars = 0;
        int len = strlen(text);

        bool inColorCode = false;  // For '#' sequences
        bool inSpecialMode = false; // For '&' sequences
        int charIndex = 0;

        // Process each character
        for (int i = 0; i < len; i++)
        {
            unsigned char ch = text[i];
            if (ch == '\0') break;

            // Handle control characters and special sequences
            if (ch < ' ')
            {
                inColorCode = false;
                continue;
            }

            switch (ch)
            {
            case '#':
                if (!inColorCode)
                {
                    inColorCode = true;
                    continue;
                }
                break;

            case '&':
                if (!inColorCode)
                {
                    inSpecialMode = true;
                    continue;
                }
                break;

            case '|':
                if (!inColorCode)
                {
                    if (inSpecialMode)
                    {
                        inSpecialMode = false;
                        continue;
                    }
                }
                break;

            case '@':
                if (!inColorCode)
                {
                    // Skip 8 characters (probably a special sequence)
                    i += 8;
                    continue;
                }
                break;

            case '$':
                if (!inColorCode)
                {
                    continue; // Skip '$' character
                }
                break;

            default:
                break;
            }

            // Reset color code flag for non-special characters
            if (ch != '#' && inColorCode)
            {
                inColorCode = false;
            }

            // Skip processing if we're in a color code or special mode
            if (inColorCode || inSpecialMode)
            {
                continue;
            }

            // Check right boundary for invisible text
            if (csz && maxc)
            {
                float scaledTotalWidth = scaledWidth / (font->m_heightScaled / font->m_heightUnscaled);
                if (scaledTotalWidth > (csz->width + csz->x0))
                {
                    *maxc = charIndex;
                    if (rightInvisibleSubstr) {
                        CStr charStr(std::string(1, ch).c_str());
                        *rightInvisibleSubstr += charStr;
                    }
                    break;
                }
            }

            // Calculate character metrics
            float charWidth = font->GetCharWidthAdvanced(ch);
            totalWidth += charWidth;
            scaledWidth = totalWidth;

            PointBase<float> glyphSize = font->GetGlyphSz(ch);

            if (glyphSize.y > maxHeight)
            {
                maxHeight = glyphSize.y;
            }

            // Check left boundary for invisible text
            if (csz && minc)
            {
                float currentScaledWidth = totalWidth / (font->m_heightScaled / font->m_heightUnscaled);
                if (csz->x0 > currentScaledWidth)
                {
                    *minc = charIndex;
                    if (leftInvisibleSubstr)
                    {
                        CStr charStr(std::string(1, ch).c_str());
                        *leftInvisibleSubstr += charStr;
                    }
                }
            }

            numChars++;
            charIndex++;
        }

        // If no characters were processed, use 'A' as reference
        if (numChars == 0)
        {
            if (font->m_symbols['A'])
            {
                maxHeight = font->m_symbols['A']->m_precalcedGlyphSz.y;
            }
        }

        // Convert to scaled coordinates
        float scaleFactor = font->m_heightScaled / font->m_heightUnscaled;
        size.x = scaledWidth / scaleFactor;
        size.y = maxHeight / scaleFactor;

        return numChars;
    }

    int Application::DrawTextRelT(float, float, unsigned, CStr const&, unsigned, int)
    {
        RETRUXX_NOT_IMPLEMENTED; 
    }

    int Application::FormatText(retruxx::vector<ui::FormattedLine>& linesOfText, PointBase<float> const& at, CStr const& textIn, ui::DrawInfo const& di, TextWrapFlags wrapFlags, TextFormatFlags formatFlags)
    {
        // TODO: generated code
        CStr src = textIn;
        CStr line;
        CStr prevWord;

        int wordsInLine = 0;
        float lineWidth = 0.0f;
        float lineHeight = 0.0f;
        float availableWidth = 0.0f;

        PointBase<float> origin = at;
        int prevWordPos = -1;
        int lineColor = -1;

        // Calculate available width based on text format
        switch (formatFlags)
        {
        case TF_CENTER:
            availableWidth = di.m_clientRect.width;
            break;

        case TF_LEFT:
        case TF_FULL:
            availableWidth = di.m_clientRect.width - (at.x - di.m_clientRect.x0);
            break;

        case TF_RIGHT:
            availableWidth = at.x - di.m_clientRect.x0;
            break;

        default:
            availableWidth = di.m_clientRect.width;
            break;
        }

        // Add small epsilon to avoid floating point precision issues
        float maxWidth = availableWidth + 0.01f;

        // Get source text length
        const char* srcText = src.c_str();
        int textLen = srcText ? strlen(srcText) : 0;
        int currentPos = 0;

        if (textLen <= 0)
        {
            if (line.c_str()[0] != '\0')
            {
                m3d::ui::FormattedLine formattedLine;
                formattedLine.m_origin = origin;
                formattedLine.m_text = line;
                formattedLine.m_color = lineColor;
                formattedLine.m_isHieroglyphic = false;
                formattedLine.m_format = (formatFlags != TF_FULL) ? formatFlags : TF_LEFT;

                linesOfText.push_back(formattedLine);
            }

            return 1;
        }

        // Process text word by word
        do
        {
            CStr word;
            int wordLen;
            bool forceBreak = false;

            // Get next word
            GetWord(src, currentPos, wrapFlags, word, wordLen);

            // Check for forced line break (pipe character)
            if (prevWordPos != -1)
            {
                int prevCharPos = currentPos - 1;
                if (prevCharPos < textLen && prevCharPos > 0 && srcText[prevCharPos] == '|')
                {
                    forceBreak = !IsEscSymbolBeforeSymbol(src, prevCharPos);
                }
            }

            // Update current position
            if (wordLen == -1)
            {
                currentPos = textLen;
            }
            else
            {
                int newPos = currentPos + wordLen;
                if (newPos < textLen && srcText[newPos] == '|' && !IsEscSymbolBeforeSymbol(src, newPos))
                {
                    currentPos = newPos + 1;
                }
                else
                {
                    currentPos = newPos;
                }
            }

            prevWordPos = wordLen;

            PointBase<float> wordSize;

            if (wrapFlags == TW_CHAR_WRAP)
            {
                // Character-based wrapping - measure word directly
                GetTextExtent(word, wordSize, -1, 0, 0, 0, 0, 0);
            }
            else
            {
                // Word-based wrapping - measure word with potential space
                CStr tempWord = word;
                if (line.c_str()[0] != '\0' && !(prevWord == " ") && !(word == " "))
                {
                    tempWord += " ";
                }
                GetTextExtent(tempWord, wordSize, -1, 0, 0, 0, 0, 0);
            }

            // Check if we need to break the line
            if (wordSize.x + lineWidth > maxWidth || forceBreak)
            {
                // Create formatted line
                m3d::ui::FormattedLine formattedLine;
                formattedLine.m_origin = origin;

                if (wordsInLine > 0)
                {
                    formattedLine.m_text = line;
                }
                else
                {
                    formattedLine.m_text = word;
                }

                formattedLine.m_color = lineColor;

                // Update color tracking
                float foundColor = FindLastColorInStr(line);
                if (foundColor != 0.0f)
                {
                    lineColor = static_cast<int>(foundColor);
                }

                formattedLine.m_isHieroglyphic = false;

                // Set format - special handling for TF_FULL with forced breaks
                if (formatFlags != TF_FULL || !forceBreak)
                {
                    formattedLine.m_format = formatFlags;
                }
                else
                {
                    formattedLine.m_format = TF_LEFT;
                }

                // Add to lines vector
                linesOfText.push_back(formattedLine);

                // Update line height if this is the first line
                if (lineHeight == 0.0f)
                {
                    lineHeight = wordSize.y;
                }

                // Move origin down for next line
                origin.y += lineHeight;

                // Reset line buffer
                if (wordsInLine > 0)
                {
                    line = word;
                    wordsInLine = 1;
                }
                else
                {
                    line = "";
                    wordsInLine = 0;
                }

                // Measure new line
                GetTextExtent(line, wordSize, -1, 0, 0, 0, 0, 0);
                lineWidth = wordSize.x;
                lineHeight = wordSize.y;
            }
            else
            {
                // Add word to current line
                if (wrapFlags == TW_CHAR_WRAP)
                {
                    line += word;
                }
                else
                {
                    // Add space between words if appropriate
                    if (line.c_str()[0] != '\0' && !(prevWord == " ") && !(word == " "))
                    {
                        line += " ";
                    }
                    line += word;
                }
                wordsInLine++;

                // Measure updated line
                GetTextExtent(line, wordSize, -1, 0, 0, 0, 0, 0);
                lineWidth = wordSize.x;

                // Update line height if this word is taller
                if (wordSize.y > lineHeight)
                {
                    lineHeight = wordSize.y;
                }
            }

            prevWord = word;

        } while (currentPos < textLen);

        // Handle any remaining text in the line buffer
        if (line.c_str()[0] != '\0') {
            m3d::ui::FormattedLine formattedLine;
            formattedLine.m_origin = origin;
            formattedLine.m_text = line;
            formattedLine.m_color = lineColor;
            formattedLine.m_isHieroglyphic = false;
            formattedLine.m_format = (formatFlags != TF_FULL) ? formatFlags : TF_LEFT;

            linesOfText.push_back(formattedLine);
        }

        return 1;
    }

    void Application::WaitForAnykey()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::SetFrameClearColor(unsigned color)
    {
        this->m_frameClearColor = color;
    }

    int Application::DrawTextAbs(float, float, unsigned, CStr const&, unsigned, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Application::Application(char const* logName)
    {
        if (g_Kernel->OpenLog(logName))
        {
            M3D_LOG_INFO("Starting up...");
            g_pApp = this;

            //inject::injectMemoryAddress(0x00A0A55C, inject::cast<uint32_t>(g_pApp));

            m_focusKbdEntity = this;
            m_strWindowTitle = "retruxx - release version build v0.01";

            char buf[0x400] = { 0 };
            ::GetCurrentDirectoryA(sizeof(buf), buf);
            m_startupFolder = buf;
            ::GetModuleFileNameA(GetModuleHandleA(NULL), buf, sizeof(buf));
            m_imageName = buf;

            using namespace ui;
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgStaticModelNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgAnimatedModelNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgGameUnitNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgSpriteNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgLinesNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgParticlesNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgSoundSourceNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgLoadpointNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgPointLightSourceNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgProjectorNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgDecalsNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgParticlesOpaqueNode));
            g_Kernel->AddClass(RT_CLASS_LOCAL(Wnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ImageWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(EditWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SliderWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ScrollWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ButtonWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(GlyphButton));
            g_Kernel->AddClass(RT_CLASS_LOCAL(CheckWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ModalWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(MenuWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(StringsListBoxWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(FormattedStringsListBoxWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(TextBoxWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(MsgBoxDlg));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ArtDlg));
            g_Kernel->AddClass(RT_CLASS_LOCAL(LineWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(TabWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ModelWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ModelArrayWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(SgNodeArrayWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ComboBoxWnd));
            g_Kernel->AddClass(RT_CLASS_LOCAL(Landscape));
            g_Kernel->AddClass(RT_CLASS_LOCAL(Level));
            g_Kernel->AddClass(RT_CLASS_LOCAL(Cinematic));
            g_Kernel->AddClass(RT_CLASS_LOCAL(ProgressBarWnd));

            m_cpuSpeed = GetCpuFrequency(0x64);
            m_profiler_OneFrame = m_profilerStack.AddProfiler("OneFrame", 30);
            m_profiler_Render = m_profilerStack.AddProfiler("Render", 30);
            m_profiler_UiRender = m_profilerStack.AddProfiler(" - UI Render", 30);

            g_Kernel->GetScriptServer().registerGlobalFunction(n_GetComputerName, "GetComputerName", "const char*", "", "returns network name for the current computer");

            m_cameraController = new CameraController;
            m_cinematic = dynamic_cast<Cinematic*>(g_Kernel->New("Cinematic"));
        }
        else
        {
            MessageBoxA(NULL, "Error!!! Can't open log!!!", "Critical error", 16);
        }
    }

    void Application::sysError(CStr const& whence, CStr const& assertion)
    {

        RETRUXX_NOT_IMPLEMENTED;
        auto const description = "Assertion failed at " + whence + "\nexpression: " + assertion;
        M3D_LOG_INFO(description);
        //TODO: check this
        panic();
    }

    int Application::FinishExclusiveMsgLoop()
    {
        m_breakLoop = true;
        return 1;
    }

    void Application::UnPause()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* Application::GetBaseClass()
    {
        return RT_CLASS_LOCAL(WndStation);
    }

    void Application::PutSpriteAbsRot(float, float, float, float, unsigned, float, float, float, float, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Profiler* Application::CreateProfiler(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PutSprite2Rel(float x1, float y1, float tu1, float tv1, float x2, float y2, float tu2, float tv2, unsigned c)
    {
        this->m_renderer->RelToAbs(x1, y1);
        this->m_renderer->RelToAbs( x2, y2);
        m3d::Application::PutSprite2Abs(x1, y1, tu1, tv1, x2, y2, tu2, tv2, c);
    }

    void Application::SetKeyboardFocus(IEventHandler*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::OnChangeMode(AuxImpulseInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DrawWireRectRel(CVector2 const&, CVector2 const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer& Application::GetLightsServer()
    {
        return *m_serverLights;
    }

    int Application::SetFont(CStr const&, float, unsigned, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Application::SaveUsedOnlyServers(CStr const&, retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::Render3DText0(CVector const&, CStr const&, unsigned, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    rend::VertexXYZWCT1* Application::RenderQuadXyzwct1GetNextPtr()
    {
        if (this->m_numPointsVerts + 4 >= 0xFA0)
            m3d::Application::FlushQuads();

        auto result = &this->m_pointsVertsWct1[m_numPointsVerts];
        this->m_numPointsVerts = m_numPointsVerts + 4;
        return result;
    }

    bool Application::AddPostEffect(CStr const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::FindLastColorInStr(CStr const& line)
    {
        // TODO: check this
        if (line.empty())
        {
            return 0;
        }

        int nColor = 0;
        bool bEsc = false;
        int v3 = 0;  // position index
        int v4 = 8;  // some offset counter


        while (true)
        {
            int v6 = line.length();

            if (v3 >= v6)
                return nColor;

            char v7 = line[v3];

            if (v7 == '#')
            {
                if (bEsc) {
                    bEsc = false;
                    v3++;
                    v4++;
                    continue;
                }
                ++v3;
                bEsc = true;
                ++v4;
            }
            else
            {
                if (v7 != '@' || bEsc)
                {
                    bEsc = false;
                    v3++;
                    v4++;
                    continue;
                }

                int v8 = v6;
                if (v8 <= v4) {
                    v3++;
                    v4++;
                    continue;
                }

                // Extract color code
                char color[9];  // 8 chars + null terminator
                color[0] = line[v3 + 1];
                color[1] = line[v3 + 2];
                color[2] = line[v3 + 3];
                color[3] = line[v3 + 4];
                color[4] = line[v3 + 5];
                color[5] = line[v3 + 6];
                color[6] = line[v3 + 7];
                color[7] = line[v3 + 8];
                color[8] = 0;

                sscanf(color, "%x", &nColor);
                v3 += 9;
                v4 += 9;
            }
        }
        return 0;
    }

    void Application::PutSplashMainMenuLevelLoad(int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PutSplashCallBackQuiet(int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::RenderQuadXyzwct1(float, float, float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DrawBoundingRadius(CVector const&, float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::SetMouseXy(float absX, float absY)
    {
        if (m_bDXCursorEnabled)
        {
            g_pApp->m_renderer->MoveDXCursor(absX, absY);
        }
        else
        {
            m_mouseX = absX;
            m_mouseY = absY;
        }
    }

    int Application::GetCurGameMode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::HeightmapToNormalmap(unsigned short*, unsigned*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::SetFontSelfMaking(CStr const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PutSplash(int, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::DrawTextRel(float, float, unsigned, CStr const&, unsigned, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    HWND Application::GetRenderWindow() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PutSprite2Abs(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tu1, float tv1, float tu2, float tv2, float tu3, float tv3, float tu4, float tv4, float zval, unsigned c)
    {
        //TODO: check this and refactor!!!
        int vofs = 0;
        auto vbs = g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZWCT1);
        auto mem = static_cast<float*>(g_pApp->m_renderer->LockVbStreaming(vbs, rend::VERTEX_XYZWCT1, vofs, nullptr));

        auto* memInt = reinterpret_cast<unsigned*>(mem);
        mem[5] = tu1;
        mem[6] = tv1;
        mem[0] = x1;
        mem[1] = y1;
        mem[2] = zval;
        mem[3] = 0.1;
        memInt[4] = c;
        mem[7] = x2;
        mem[8] = y2;
        mem[12] = tu2;
        mem[13] = tv2;
        mem[9] = zval;
        mem[10] = 0.1;
        memInt[11] = c;
        mem[14] = x3;
        mem[15] = y3;
        mem[19] = tu3;
        mem[16] = zval;
        mem[20] = tv3;
        mem[17] = 0.1;
        memInt[18] = c;
        mem[23] = zval;
        mem[21] = x4;
        mem[26] = tu4;
        mem[22] = y4;
        mem[24] = 0.1;
        memInt[25] = c;
        mem[27] = tv4;

        g_pApp->m_renderer->UnlockVb(vbs);
        g_pApp->m_renderer->SetToStream0(vbs);
        g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_TRIANGLESTRIP, vofs, 2);
    }

    void Application::PutSprite2Abs(float x1, float y1, float tu1, float tv1, float x2, float y2, float tu2, float tv2, unsigned int c)
    {
        PutSprite2Abs(x1, y2, x1, y1, x2, y2, x2, y1, tu1, tv2, tu1, tv1, tu2, tv2, tu2, tv1, 0.0, c);
    }

    char const* Application::GetCallbackName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ui::MbRetCodes Application::RunMsgBoxDlg(CStr const&, CStr const&, unsigned, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::DrawTextAbsT(float, float, unsigned, CStr const&, unsigned, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::SaveExportDescToFile(fs::FileStream&, bool) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::DrawLine(CVector2 const&, CVector2 const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(0x007B0010, Application::DrawLine, void(Application::*)(CVector const&, CVector const&, unsigned))
    void Application::DrawLine(CVector const&, CVector const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::Pause()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Application::GetClassDesc(Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::ClearViewportToBlack()
    {
        //TODO: check this and refactor 
        int v2; // edi
        int v3; // esi

        auto& config = g_Kernel->GetEngineCfg();
        if (IsWindow(config.m_mainWnd))
        {
            v2 = M3D_RENDERER->InScene();
            v3 = 0;
            if (v2 || (v3 = M3D_RENDERER->BeginScene()) != 0)
            {
                M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, -16777216u);
                if (!v2)
                {
                    if (v3)
                        M3D_RENDERER->EndScene();
                }
            }
            M3D_RENDERER->PresentScene();
        }
    }

    CStr const& Application::GetStartupFolder() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PutSpriteAbs(float, float, float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::PutSpriteAbs(float x1, float y1, float x2, float y2, unsigned c)
    {
        PutSprite2Abs(x1, y2, x1, y1, x2, y2, x2, y1, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, c);
    }

    void Application::PutSpriteAbs(float, float, float, float, float, float, float, float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer& Application::GetAnimatedModelsServer()
    {
        return *m_serverAnimatedModels;
    }

    bool Application::KillPostEffect(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer& Application::GetDecalsServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::DrawTextRelClip(PointBase<float> const& at , CStr const& str, ui::DrawInfo const& di, TextWrapFlags wrapFlag, TextFormatFlags formatFlag)
    {
        enterFontRender();
        StartQuads(rend::VERTEX_XYZWCT1);
        if (wrapFlag)
        {
            retruxx::vector<m3d::ui::FormattedLine> linesOfText;
            FormatText(linesOfText, at, str, di, wrapFlag, formatFlag);
            if (formatFlag == TF_FULL && !linesOfText.empty())
            {
                linesOfText.back().m_format = TF_LEFT;
            }
            for (auto const& line : linesOfText)
            {
                DrawStringRelClip(line, di);
            }
        }
        else
        {
            ui::FormattedLine line;
            line.m_origin.x = at.x;
            line.m_origin.y = at.y;
            line.m_color = -1;
            line.m_text = str;
            line.m_format = formatFlag;
            line.m_isHieroglyphic = false;
            DrawStringRelClip(line, di);
        }
        FinishQuads();
        finishFontRender();
        return 1;
    }

    DataServer& Application::GetParticlesServer()
    {
        return *m_serverParticles;
    }

    bool Application::bIsMousePointing() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DbgCounter* Application::CreateCounter(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::texGenEnqueue(IGeneratedTexture*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::HandleCommand(int cmdId, CConsoleParams const&)
    {
        if (cmdId > 21)
        {
            return;
        }
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::RunBenchmark()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::ShowSystemCursor(bool bShow)
    {
        if (bShow)
        {
            while (ShowCursor(1) < 0);
        }
        else
        {
            while (ShowCursor(0) >= 0);
        }
    }

    //TODO: return MHZ?
    long long Application::GetCpuFrequency(unsigned)
    {
        int cpuInfo[4] = {0};
        __cpuid(cpuInfo, 0);
        if (cpuInfo[0] >= 0x16)
        {
            __cpuid(cpuInfo, 0x16);
            return cpuInfo[0];
        }
        return 0;
    }

    void Application::FlushQuads()
    {
        int vOfs = 0;
        if (m_numPointsVerts)
        {
            auto vbStream = M3D_RENDERER->LockVbStreaming(m_pointsVertsVb, m_numPointsVerts, vOfs, nullptr);
            memcpy(vbStream, m_sourceVerts, this->m_numPointsVerts * this->m_pointsVertsSz);

            M3D_RENDERER->UnlockVb(this->m_pointsVertsVb);
            // TODO: check this
            M3D_RENDERER->SetIndices(this->m_pointsVertsIb, vOfs);
            M3D_RENDERER->SetToStream0(this->m_pointsVertsVb);
            if (m_flushQuadsShader)
            {
                M3D_RENDERER->DrawIndexedPrimitiveEffect(rend::M3DPT_TRIANGLELIST, m_flushQuadsShader, 0, m_numPointsVerts, 0, 2 * (this->m_numPointsVerts / 4));
            }
            else
            {
                M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLELIST,
                    0,
                    this->m_numPointsVerts,
                    0,
                    2 * (this->m_numPointsVerts / 4));
            }
            this->m_numPointsVerts = 0;

        }
    }

    void Application::CreateQuadsIb()
    {
        if (!m_pointsVertsIb.IsValid())
        {
            m_pointsVertsIb = M3D_RENDERER->AddIb(6000, false);

            auto v3 = (WORD*)M3D_RENDERER->LockIb(m_pointsVertsIb, 0, 0, 0);
            auto v4 = 2;
            auto v5 = 1000;
            do
            {
                *v3 = v4 - 2;
                v3[3] = v4 - 2;
                v3[2] = v4;
                v3[4] = v4;
                v3[1] = v4 - 1;
                v3[5] = v4 + 1;
                v4 += 4;
                v3 += 6;
                --v5;
            } while (v5);

            M3D_RENDERER->UnlockIb(m_pointsVertsIb);
        }
    }

    void Application::EnqueueEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Application::GetEvent(Event&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::finishFontRender()
    {
        if (GetGfxServer()->GetCurFont())
        {
            M3D_RENDERER->PopCull();
            M3D_RENDERER->PopBlend();
        }
    }

    int Application::DrawStringRelClip(ui::FormattedLine const& fl, ui::DrawInfo const& di)
    {
        // TODO: generated code
        if (fl.m_isHieroglyphic && M3D_KERNEL->GetEngineCfg().m_ui_forceHieroglyphicFont.GetB())
        {
            GetGfxServer()->SetFont(GetGfxServer()->m_hieroglyphicFontId);
        }
        
        ui::Font* fnt = GetGfxServer()->GetCurFont();
        if (!fnt)
        {
            return 0;
        }

        CStr text = fl.m_text;
        int lastLeadingSpacePos = -1;
        int textLen = text.length();

        // Handle TF_FULL formatting (justified text)
        if (fl.m_format == TF_FULL)
        {
            // Count leading spaces
            int leadingSpaces = 0;
            if (textLen > 0 && text[0] == '@')
            {
                leadingSpaces = 9; // Skip command prefix
            }

            // Count consecutive leading spaces
            for (int i = leadingSpaces; i < textLen; i++)
            {
                if (text[i] != ' ') break;
                leadingSpaces++;
                lastLeadingSpacePos = i;
            }

            // Count trailing spaces
            int trailingSpaces = 0;
            for (int i = textLen - 1; i >= 0; i--)
            {
                if (text[i] != ' ') break;
                trailingSpaces++;
            }

            // Trim trailing spaces
            if (trailingSpaces > 0)
            {
                text = text.substr(0, textLen - trailingSpaces);
                textLen = text.length();
            }
        }

        // Measure text extent
        PointBase<float> sz;
        GetTextExtent(text, sz, -1, 0, 0, 0, 0, 0);

        // Calculate text bounds based on format
        BoundsBase<float> textBounds;
        switch (fl.m_format)
        {
        case TF_CENTER:
            textBounds.x0 = fl.m_origin.x - (sz.x * 0.5f);
            textBounds.y0 = fl.m_origin.y;
            textBounds.width = ((sz.x * 0.5f) + fl.m_origin.x) - fl.m_origin.x - (sz.x * 0.5);
            textBounds.height = fl.m_origin.y + sz.y - fl.m_origin.y;
            break;

        case TF_LEFT:
            textBounds.x0 = fl.m_origin.x;
            textBounds.y0 = fl.m_origin.y;
            textBounds.width = (fl.m_origin.x + sz.x) - fl.m_origin.x;
            textBounds.height = (fl.m_origin.y + sz.y) - fl.m_origin.y;
            break;

        case TF_RIGHT:
            textBounds.x0 = fl.m_origin.x - sz.x;
            textBounds.y0 = fl.m_origin.y;
            textBounds.width = (fl.m_origin.x + sz.x) - fl.m_origin.x;
            textBounds.height = (fl.m_origin.y + sz.y) - fl.m_origin.y;
            break;

        case TF_FULL:
            textBounds.x0 = di.m_clientRect.x0;
            textBounds.y0 = fl.m_origin.y;
            textBounds.width = di.m_clientRect.width + di.m_clientRect.x0 - di.m_clientRect.x0;
            textBounds.height = fl.m_origin.y + sz.y - fl.m_origin.y;
            break;
        }

        // Calculate clipping
        BoundsBase<float> clipRect = di.m_clientClippedRect;
        BoundsBase<float> visibleBounds;

        // Check if text is completely outside clip region
        if (textBounds.x0 + textBounds.width < clipRect.x0 ||
            textBounds.x0 > clipRect.x0 + clipRect.width ||
            textBounds.y0 + textBounds.height < clipRect.y0 ||
            textBounds.y0 > clipRect.y0 + clipRect.height)
        {
            // Text is completely invisible
            return 0;
        }

        // Calculate visible portion
        visibleBounds.x0 = std::max(textBounds.x0, clipRect.x0);
        visibleBounds.y0 = std::max(textBounds.y0, clipRect.y0);
        visibleBounds.width = std::min(textBounds.x0 + textBounds.width, clipRect.x0 + clipRect.width) - visibleBounds.x0;
        visibleBounds.height = std::min(textBounds.y0 + textBounds.height, clipRect.y0 + clipRect.height) - visibleBounds.y0;

        bool doClip = (textBounds.x0 != visibleBounds.x0 ||
            textBounds.y0 != visibleBounds.y0 ||
            textBounds.width != visibleBounds.width ||
            textBounds.height != visibleBounds.height);

        // Handle text clipping
        int firstInvisibleChar = textLen;
        CStr leftInvisibleSubstr;
        CStr rightInvisibleSubstr;
        int firstVisibleChar = -1;

        if (doClip)
        {
            // Convert clip coordinates to text space
            BoundsBase<float> textSpaceClip = visibleBounds;

            switch (fl.m_format)
            {
            case TF_CENTER:
                textSpaceClip.x0 = visibleBounds.x0 - (fl.m_origin.x - (sz.x * 0.5f));
                textSpaceClip.y0 = visibleBounds.y0 - fl.m_origin.y;
                break;

            case TF_LEFT:
            case TF_FULL:
                textSpaceClip.x0 = visibleBounds.x0 - fl.m_origin.x;
                textSpaceClip.y0 = visibleBounds.y0 - fl.m_origin.y;
                break;

            case TF_RIGHT:
                textSpaceClip.x0 = visibleBounds.x0 - (fl.m_origin.x - sz.x);
                textSpaceClip.y0 = visibleBounds.y0 - fl.m_origin.y;
                break;

            default:
                break;
            }

            // Get clipped text portions
            GetTextExtent(text, sz, -1, &textSpaceClip, &firstVisibleChar,
                &firstInvisibleChar, &leftInvisibleSubstr, &rightInvisibleSubstr);
        }

        // Calculate starting position
        float curX = 0.0f;
        if (doClip && !leftInvisibleSubstr.empty())
        {
            // Calculate width of invisible left portion
            for (int i = 0; i < leftInvisibleSubstr.length(); i++)
            {
                unsigned char ch = leftInvisibleSubstr.c_str()[i];
                if (ch < 32) continue;

                ui::Font::SymbolInfo* sym = fnt->m_symbols[ch];
                if (sym)
                {
                    curX += sym->m_precalcedABCWidth;
                }
            }
        }

        // Calculate text origin based on format
        PointBase<float> at;
        switch (fl.m_format)
        {
        case TF_CENTER:
            at.x = fl.m_origin.x - (sz.x * 0.5f);
            at.y = fl.m_origin.y;
            break;

        case TF_LEFT:
        case TF_FULL:
            at = fl.m_origin;
            break;

        case TF_RIGHT:
            at.x = fl.m_origin.x - sz.x;
            at.y = fl.m_origin.y;
            break;

        default:
            at = fl.m_origin;
            break;
        }

        // Convert to absolute coordinates
        M3D_RENDERER->RelToAbs(at.x, at.y);
        BoundsBase<float> absClip = visibleBounds;
        M3D_RENDERER->RelToAbs(absClip.x0, absClip.y0);
        M3D_RENDERER->RelToAbs(absClip.width, absClip.height);

        // Get color
        unsigned int clr = fl.m_color;
        if (firstVisibleChar != -1)
        {
            CStr visibleText = text.substr(0, firstVisibleChar);
            float foundColor = FindLastColorInStr(visibleText);
            if (foundColor != 0.0f)
            {
                clr = static_cast<unsigned int>(foundColor);
            }
        }

        // Calculate space width for justified text
        float spaceW = 0.0f;
        float extraSpace = 0.0f;

        if (fnt->m_symbols[' '])
        {
            spaceW = fnt->m_symbols[' ']->m_precalcedABCWidth;
        }

        if (fl.m_format == TF_FULL)
        {
            // Count spaces for justification
            int spaceCount = 0;
            for (int i = lastLeadingSpacePos + 1; i < textLen; i++) {
                if (text.c_str()[i] == ' ')
                {
                    spaceCount++;
                }
            }

            if (spaceCount > 0) {
                // Calculate extra space to distribute
                PointBase<float> absSz = sz;
                M3D_RENDERER->RelToAbs(absSz.x, absSz.y);

                BoundsBase<float> absClient = di.m_clientRect;
                M3D_RENDERER->RelToAbs(absClient.x0, absClient.y0);
                M3D_RENDERER->RelToAbs(absClient.width, absClient.height);

                float availableWidth = absClient.width;
                float textWidth = absSz.x;
                extraSpace = (availableWidth - textWidth) / spaceCount;
                spaceW += extraSpace;
            }
        }

        // Render visible characters
        bool inColorCode = false;
        int charIndex = (firstVisibleChar != -1) ? firstVisibleChar : 0;

        while (charIndex < firstInvisibleChar)
        {
            unsigned char ch = text.c_str()[charIndex];

            if (ch < 32)
            {
                // Control character
                inColorCode = false;
                charIndex++;
                continue;
            }

            if (ch == '@' && !inColorCode)
            {
                // Command prefix
                charIndex+=9;
                continue;
            }

            if (ch == '#' && !inColorCode)
            {
                // Color code start
                inColorCode = true;
                charIndex++;
                continue;
            }

            if ((ch == '$' || ch == '&') && !inColorCode)
            {
                // Other special characters
                charIndex++;
                continue;
            }

            if (inColorCode)
            {
                // Parse color code
                if (charIndex + 8 <= textLen)
                {
                    char colorStr[9];
                    strncpy(colorStr, text.c_str() + charIndex, 8);
                    colorStr[8] = '\0';
                    sscanf(colorStr, "%x", &clr);
                    charIndex += 8;
                }
                inColorCode = false;
                continue;
            }

            // Render character
            ui::Font::SymbolInfo* sym = fnt->m_symbols[ch];
            if (!sym)
            {
                charIndex++;
                continue;
            }

            float charWidth = sym->m_precalcedABCWidth;
            float glyphWidth = sym->m_precalcedGlyphSz.x;
            float glyphHeight = sym->m_precalcedGlyphSz.y;


            // Adjust space width for justified text
            if (ch == ' ' && charIndex > lastLeadingSpacePos)
            {
                charWidth = spaceW;
                glyphWidth = spaceW;
            }

            // Calculate character position
            float x0 = at.x + curX + sym->m_abc.m_A + 0.5;
            float y0 = at.y + 0.5;
            float x1 = x0 + glyphWidth;
            float y1 = y0 + glyphHeight;

            // Get texture coordinates
            float tx0 = sym->m_tcs.m_coordinates[0];
            float ty0 = sym->m_tcs.m_coordinates[1];
            float tx1 = sym->m_tcs.m_coordinates[2];
            float ty1 = sym->m_tcs.m_coordinates[3];

            // Apply clipping
            float clippedX0 = x0;
            float clippedX1 = x1;
            float clippedY0 = y0;
            float clippedY1 = y1;
            float clippedTx0 = tx0;
            float clippedTx1 = tx1;
            float clippedTy0 = ty0;
            float clippedTy1 = ty1;

            if (doClip)
            {
                // Horizontal clipping
                if (charIndex == firstVisibleChar && absClip.x0 > x0)
                {
                    float clipRatio = (absClip.x0 - x0) / glyphWidth;
                    clippedX0 = absClip.x0;
                    clippedTx0 = tx0 + (tx1 - tx0) * clipRatio;
                }

                if (charIndex == firstInvisibleChar - 1 && x1 > absClip.x0 + absClip.width)
                {
                    float clipRatio = (x1 - (absClip.x0 + absClip.width)) / glyphWidth;
                    clippedX1 = absClip.x0 + absClip.width;
                    clippedTx1 = tx1 - (tx1 - tx0) * clipRatio;
                }

                // Vertical clipping
                if (absClip.y0 > y0)
                {
                    float clipRatio = (absClip.y0 - y0) / glyphHeight;
                    clippedY0 = absClip.y0;
                    clippedTy0 = ty0 + (ty1 - ty0) * clipRatio;
                }

                if (y1 > absClip.y0 + absClip.height)
                {
                    float clipRatio = (y1 - (absClip.y0 + absClip.height)) / glyphHeight;
                    clippedY1 = absClip.y0 + absClip.height;
                    clippedTy1 = ty1 - (ty1 - ty0) * clipRatio;
                }
            }

            if (sym->m_tcs.m_texId >= 0)
            {
                // Check if texture needs to be changed
                if (fnt->m_textures[sym->m_tcs.m_texId] != GetGfxServer()->m_curFontTexture)
                {
                    if (GetGfxServer()->m_curFontTexture.IsValid())
                    {
                        FlushQuads();
                    }
                    M3D_RENDERER->SetTexture(0, fnt->m_textures[sym->m_tcs.m_texId], -1.0f);
                    GetGfxServer()->m_curFontTexture = fnt->m_textures[sym->m_tcs.m_texId];
                }
            }

            // Render quad
            rend::VertexXYZWCT1* vertices = RenderQuadXyzwct1GetNextPtr();
            if (vertices)
            {
                // Set up quad vertices
                for (int i = 0; i < 4; i++)
                {
                    vertices[i].z = 0.0f;
                    vertices[i].w = 0.5f;
                    vertices[i].c = clr;
                }

                // Vertex 0: top-left
                vertices[0].x = clippedX0;
                vertices[0].y = clippedY0;
                vertices[0].tu = clippedTx0;
                vertices[0].tv = clippedTy0;

                // Vertex 1: top-right
                vertices[1].x = clippedX1;
                vertices[1].y = clippedY0;
                vertices[1].tu = clippedTx1;
                vertices[1].tv = clippedTy0;

                // Vertex 2: bottom-right
                vertices[2].x = clippedX1;
                vertices[2].y = clippedY1;
                vertices[2].tu = clippedTx1;
                vertices[2].tv = clippedTy1;

                // Vertex 3: bottom-left
                vertices[3].x = clippedX0;
                vertices[3].y = clippedY1;
                vertices[3].tu = clippedTx0;
                vertices[3].tv = clippedTy1;
            }

            // Advance cursor
            if (ch == ' ' && charIndex > lastLeadingSpacePos)
            {
                curX += spaceW;
            }
            else
            {
                curX += charWidth;
            }

            charIndex++;
        }

        return 1;

    }

    unsigned long Application::texGenThread(void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Application::AdjustWindowForChange(HWND, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Application::enterFontRender()
    {
        if (GetGfxServer()->GetCurFont())
        {
            M3D_RENDERER->SetTexture(0, GetGfxServer()->m_curFontTexture, -1.0);
            M3D_RENDERER->PushBlend(rend::BM_ALPHA);
            M3D_RENDERER->SetAlphaTest(1);
            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
            M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
            M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
            M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
            M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
        }
    }

    int Application::EnableDXCursor(bool bEnable)
    {
        m_bDXCursorEnabled = bEnable;
        if (m_bDXCursorEnabled)
        {
            auto const& cursor = GetCurrentCursor();
            g_pApp->m_renderer->SetupDXCursor(cursor.m_tex, cursor.m_spot.x, cursor.m_spot.y, 0);
            while (::ShowCursor(TRUE) < 0);
        }
        else
        {
            while (::ShowCursor(FALSE) >= 0);
        }
        return 1;
    }

    void Application::CaptureAndClipSystemCursor(bool bState)
    {
        if (bState)
        {
            if (m_renderWindow)
            {
                ::SetCapture(m_renderWindow);

                RECT rect;
                ::GetClientRect(m_renderWindow, &rect);
                //TODO: check this
                ::ClientToScreen(this->m_renderWindow, reinterpret_cast<LPPOINT>(&rect.left));
                ::ClientToScreen(this->m_renderWindow, reinterpret_cast<LPPOINT>(&rect.right));
                ::ClipCursor(&rect);
            }
        }
        else
        {
            ::ReleaseCapture();
            ::ClipCursor(nullptr);
        }
    }

    bool Application::HandleCVar(CVar const*, CConsoleParams const&)
    {
        return false;
    }

    void Application::DiscardAllEvents()
    {
        // TODO: check this
        while (true)
        {
            if (m_eventsQueueTail == m_eventsQueueHead)
            {
                break;
            }

            m_eventsQueue[m_eventsQueueTail] = {};

            auto v3 = m_eventsQueueTail + 1;
            if (v3 >= 5000)
                v3 = 0;
            m_eventsQueueTail = v3;
        }
    }

    unsigned long Application::GetStyleForRenderWindow(bool bFullScreen) const
    {
        //TODO: consts
        return bFullScreen ? -1878523904 : -1865547776;
    }

    DbgCounterStack& Application::GetDbgCounterStack()
    {
        return m_counterStack;
    }

    float Application::GetMouseSensitivity() const
    {
        return m_mouseSensitivity;
    }

    CameraController* Application::GetCameraController()
    {
        return m_cameraController;
    }

    void Application::EnqueueMessage(int msg, int param0, int param1, int p2, int p3, CStr const& param4, AIParam const& param5)
    {
        Event ev;
        ev.m_timeStamp = g_Kernel->GetTimer().GetCurTime() * 0.001;
        ev.m_eventType = msg;
        //TODO: check order
        ev.m_intEv[0] = param0;
        ev.m_intEv[1] = param1;
        ev.m_intEv[2] = p2;
        ev.m_intEv[3] = p3;
        ev.m_strEv = param4;
        ev.m_aiParamEv = param5;

        auto head = m_eventsQueueHead;
        auto newHead = head + 1;
        if (newHead >=5000)
        {
            newHead = 0;
        }
        if (newHead != m_eventsQueueTail)
        {
            m_eventsQueue[head] = ev;
            m_eventsQueueHead = newHead;
        }   
    }

    void Application::PutSpriteRel(float x1, float y1, float x2, float y2, unsigned c)
    {
        m_renderer->RelToAbs(x1, y1);
        m_renderer->RelToAbs(x2, y2);
        PutSpriteAbs(x1, y1, x2, y2, c);
    }

    bool Application::IsDXCursorEnabled() const
    {
        return m_bDXCursorEnabled;
    }

    ProfilerStack& Application::GetProfilerStack()
    {
        return m_profilerStack;
    }

    DataServer& Application::GetMusicServer()
    {
        return *m_serverMusic;
    }

    int Application::GetMouseY() const
    {
        return m_mouseY;
    }

    int Application::GetMouseX() const
    {
        return m_mouseX;
    }
}

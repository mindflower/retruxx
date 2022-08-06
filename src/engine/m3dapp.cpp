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

namespace
{
    std::set<size_t>* codePagesStringsPtr;

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

    using CreateIRendererType = m3d::rend::IRenderer* (*)(m3d::Kernel*);
    using CreateIInputType = m3d::input::IInput* (*)(m3d::Kernel*);
    using CreateISoundType = snd::ISound* (*)(m3d::Kernel*);
}

namespace m3d
{
    Application* Application::g_pApp = nullptr;

    int Application::init(HINSTANCE hInstance, HICON hIcon, CStr const& configName, HWND forcedWnd, CStr const& cmdLine)
    {
        M3D_LOG_INFO("ProjectApocalypse - release version build v0.01");

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
                    //m_serverAnimatedModels->Init();
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
                ai::CServer::pServer = new ai::CServer{};

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

        if (!m_waitForAnykey)
        {
            g_Kernel->GetTimer().NewFrame();
            g_pApp->m_renderer->ResetStats();
            if (g_Kernel->GetEngineCfg().m_snd_Enable.GetB() ||
                g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
            {
                auto const lastFrameTime = g_Kernel->GetTimer().GetLastFrameTime();
                g_pApp->m_sound->Update(lastFrameTime);
            }
            FrameMove();
            if (!m_waitForAnykey && m_isRenderingAllowed)
            {
                if (m_renderer->CanRender())
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
                        if (m_appNeedToRedraw)
                        {
                            Render(1);
                            m_appNeedToRedraw = 0;
                        }
                        else
                        {
                            Render(0);
                        }
                        {
                            auto* uiProfiler = m_profilerStack.GetProfiler(m_profiler_UiRender);
                            FrameProfilerPtr uiProfilerPtr(uiProfiler);
                            Repaint();
                            FlushGfx(m_renderer);
                        }
                    }
                   //TODO:...
                    throw std::logic_error("Not implemented");
                }
            }
        }
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
        long double time = 0.0;

        if (m_input->GetLastKbdEvent(&key, &param2, &param3, &time, true))
        {
	        do
	        {
                throw std::logic_error("Not implemented");
            } while (m_input->GetLastKbdEvent(&key, &param2, &param3, &time, true));
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
            ev.m_ushortEv[2] = deltaX;
            ev.m_ushortEv[0] = m_mouseX;
            ev.m_ushortEv[1] = m_mouseY;
            ev.m_eventType = 9;
            ev.m_ushortEv[3] = deltaY;

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
        while (!m_breakLoop && m_eventsQueueTail != m_eventsQueueHead)
        {
            auto const ev = m_eventsQueue[m_eventsQueueTail++];
            if (m_eventsQueueTail >= sizeof(m_eventsQueue) / sizeof(Event))
            {
                m_eventsQueueTail = 0;
            }
            HandleEvent(ev);
        }
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
        //throw std::logic_error("Not implemented");
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
                if (m3d::Application::g_pApp->m_renderer)
                    m3d::Application::g_pApp->m_renderer->ShowDXCursor(1);
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
        curMousePos.x = lParam;
        curMousePos.y = HIWORD(lParam);
        m_mouseInfo.SetUpForCurPos(curMousePos);
        if (this->m_bDXCursorEnabled && m3d::Application::g_pApp->m_renderer)
            m3d::Application::g_pApp->m_renderer->MoveDXCursor(lParam, HIWORD(lParam));
        return 0;
    }

    void Application::SetCodepage()
    {
        //TODO: check this
        std::set<size_t> codePagesStrings;
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
        //TODO: ...
        switch (ev.m_eventType)
        {
        case 1:
	        {
                PostMessageA(g_Kernel->GetEngineCfg().m_mainWnd, 0x10, 0, 0);
                return 1;
	        }
        case 2:
	        {
                //TODO: check this
                m_isAppActive = ev.m_intEv[0];
                if (m_sound)
                    m_sound->PauseAllSounds(m_isAppActive);
                if (m_renderer)
                    m_renderer->SetActiveState(m_isAppActive);
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
                    }
                    return 1;
                }
                break;
	        }
        case 3:
	        {
                //TODO: check this
                SwitchDisplayModes(g_Kernel->GetEngineCfg().m_mainWnd, ev.m_intEv[0], ev.m_intEv[1], ev.m_intEv[2] != 0);
                return 1;
	        }
        case 4:
            throw std::logic_error("Not implemented");
        case 7:
            throw std::logic_error("Not implemented");
        case 8:
            throw std::logic_error("Not implemented");
        case 0xA:
            throw std::logic_error("Not implemented");
        case 0xB:
            throw std::logic_error("Not implemented");
        case 0xC:
            throw std::logic_error("Not implemented");
        default:
        {
            if (ev.m_eventType != 15)
            {
                return ProcessEvent(ev);
            }
            throw std::logic_error("Not implemented");
        }
        }
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
        //TODO: ...
        throw std::logic_error("Not implemented");
    }

    int Application::InitImpulses()
    {
        return 1;
    }

    void Application::DoneImpulses()
    {
        //TODO: ...
        throw std::logic_error("Not implemented");
    }

    void Application::DetailSettings::RestoreGameSettings()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DetailSettings::SaveGameSettings()
    {
        m_dsShadows = g_Kernel->GetEngineCfg().m_dsShadows.GetB();
        m_lsViewDistanceDivider = g_Kernel->GetEngineCfg().m_lsViewDistanceDivider.GetF();
        m_NPatchLevel = g_Kernel->GetEngineCfg().m_NPatchLevel.GetF();
    }

    void Application::DetailSettings::SetMenuLevelSettings()
    {
        throw std::logic_error("Not implemented");
    }

    Application::DetailSettings::DetailSettings()
    {
        SaveGameSettings();
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
        throw std::logic_error("Not implemented");
    }

    PointBase<int> const& Application::MouseInfo::GetLastPos() const
    {
        throw std::logic_error("Not implemented");
    }

    void Application::MouseInfo::SetUpForCurPos(PointBase<int> const& curPos)
    {
        m_deltaDuringGameFrame.x += curPos.x - m_lastPos.x;
        m_deltaDuringGameFrame.y += curPos.y - m_lastPos.y;
        m_lastPos = curPos;
    }

    DataServer& Application::GetSoundServer()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::texGenProcess(IGeneratedTexture*)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawTri(CVector*, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::ForbidRendering()
    {
        throw std::logic_error("Not implemented");
    }

    Class* Application::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    DataServer& Application::GetStaticModelsServer()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawCross(CVector const&, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Application::GetImageName() const
    {
        throw std::logic_error("Not implemented");
    }

    int Application::GetCurDifficultyLevel() const
    {
        throw std::logic_error("Not implemented");
    }

    char* Application::GetWindowTitle() const
    {
        return m_strWindowTitle;
    }

    void Application::MiniDump()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawLogo()
    {
        throw std::logic_error("Not implemented");
    }

    float Application::GetOnScreenSize(CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::HandleCinematic(float)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSplashCallBack(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::AllowRendering()
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::StartPlayingMusic(char const*, bool, bool)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer& Application::GetProjectorsServer()
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::CheckCommandLineParam(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned long Application::GetLocale() const
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawQuad(CVector*, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::SaveToXml(cmn::XmlFile*, cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    Application::~Application()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::TexSoften(rend::TexHandle, rend::TexHandle, int, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::IsTextHieroglyphic(CStr const&) const
    {
        return false;
    }

    CStr Application::GetNativeFuncDesc(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Application::StopPlayingMusic()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::SetMouseSensitivity(float)
    {
        throw std::logic_error("Not implemented");
    }

    float Application::getZoom()
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::LoadServers(CStr const&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::SetPostEffectParam(CStr const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::ChangeLanguage()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawWireRect(CVector2 const&, CVector2 const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PostLoadServers()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::OnBeforeDeviceReset()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::FinishQuads()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::setZoom(float)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSpriteRelRot(float, float, float, float, unsigned, float, float, float, float, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::SetMouseXAxisFlipped(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::SetMouseYAxisFlipped(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::SetFlushQuadsShader(rend::IEffect*)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::AppActive() const
    {
        throw std::logic_error("Not implemented");
    }

    rend::VertexXYZCT1* Application::RenderQuadXyzct1GetNextPtr()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::LoadFromXml(cmn::XmlFile*, cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::SwitchDisplayModes(HWND__*, int, int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::done()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::OnAfterDeviceReset()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::ReloadPostEffects()
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::GetPostEffectParam(CStr const&, float&)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::GetTextFit(CStr const&, PointBase<float>&, float, TextWrapFlags)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::StartExclusiveMsgLoop()
    {
        throw std::logic_error("Not implemented");
    }

    DataServer& Application::GetSpritesServer()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    bool Application::GetMouseHitPoint(CVector&, SgNode*&)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::StartQuads(rend::VertexType)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::GetTextExtent(CStr const& str, PointBase<float>& size, int fid, BoundsBase<float>* csz, int* minc, int* maxc, CStr* leftInvisibleSubstr, CStr* rightInvisibleSubstr)
    {
        //TODO: recreate this
        if (leftInvisibleSubstr)
        {
            leftInvisibleSubstr->erase();
        }
        if (rightInvisibleSubstr)
        {
            rightInvisibleSubstr->erase();
        }
        if (str.empty())
        {
            size.x = 0;
            size.y = 0;
            return 0;
        }

        //temp!
        size.x = 0;
        size.y = 0;
        return 0;
        //

        auto fnt = fid == -1 ? GetGfxServer()->GetCurFont() : GetGfxServer()->GetFontById(fid);
        if (!fnt)
        {
            return 0;
        }

        bool flag1 = false;
        bool flag2 = false;
        float width = 0.0;
        for (int i = 0; i < str.length(); ++i)
        {
            if (str[i] > ' ')
            {
                switch (str[i])
                {
                case '#':
                    if (!flag1)
                    {
                        flag1 = true;
                        continue;
                    }
                    break;
                case '&':
                    if (!flag1)
                    {
                        flag2 = true;
                        continue;
                    }
                    break;
                case '|':
                    if (!flag1)
                    {
                        if (!flag2)
                        {
                            continue;
                        }
                        flag2 = false;
                    }
                    break;
                case '@':
                    if (!flag1)
                    {
                        i += 8;
                        continue;
                    }
                    break;
                default:
                    if (str[i] == '$' && !flag1)
                    {
                        continue;
                    }
                    break;
                }

                flag1 = false;
                if (str[i] && maxc /* && (float)(v14 / (float)(v11->m_heightScaled / v11->m_heightUnscaled)) > (float)(v16->width + v16->x0) */)
                {
                    //TODO: recreate this logic
                    *maxc = i;
                    if (rightInvisibleSubstr != nullptr)
                    {
                        (*rightInvisibleSubstr) += CStr(str[i]);
                    }
                    
                    throw std::logic_error("Not implemented");
                }
                width = fnt->GetCharWidthAdvanced(str[i]) + width;

            }
        }
        throw std::logic_error("Not implemented");
    }

    int Application::DrawTextRelT(float, float, unsigned, CStr const&, unsigned, int)
    {
        throw std::logic_error("Not implemented"); 
    }

    int Application::FormatText(std::vector<ui::FormattedLine, std::allocator<ui::FormattedLine>>&, PointBase<float> const&, CStr const&, ui::DrawInfo const&, TextWrapFlags, TextFormatFlags)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::WaitForAnykey()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::SetFrameClearColor(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::DrawTextAbs(float, float, unsigned, CStr const&, unsigned, int)
    {
        throw std::logic_error("Not implemented");
    }

    Application::Application(char const* logName)
    {
        if (g_Kernel->OpenLog(logName))
        {
            M3D_LOG_INFO("Starting up...");
            g_pApp = this;
            m_focusKbdEntity = this;
            m_strWindowTitle = "ProjectApocalypse - release version release build v0.01";

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

        throw std::logic_error("Not implemented");
        auto const description = "Assertion failed at " + whence + "\nexpression: " + assertion;
        M3D_LOG_INFO(description);
        //TODO: check this
        panic();
    }

    int Application::FinishExclusiveMsgLoop()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::UnPause()
    {
        throw std::logic_error("Not implemented");
    }

    Class* Application::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSpriteAbsRot(float, float, float, float, unsigned, float, float, float, float, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    Profiler* Application::CreateProfiler(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSprite2Rel(float, float, float, float, float, float, float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::SetKeyboardFocus(IEventHandler*)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::OnChangeMode(AuxImpulseInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawWireRectRel(CVector2 const&, CVector2 const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer& Application::GetLightsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int Application::SetFont(CStr const&, float, unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::SaveUsedOnlyServers(CStr const&, std::set<int, std::less<int>, std::allocator<int>>&)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::Render3DText0(CVector const&, CStr const&, unsigned, float)
    {
        throw std::logic_error("Not implemented");
    }

    rend::VertexXYZWCT1* Application::RenderQuadXyzwct1GetNextPtr()
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::AddPostEffect(CStr const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::FindLastColorInStr(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSplashMainMenuLevelLoad(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::RenderQuadXyzwct1(float, float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawBoundingRadius(CVector const&, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::SetMouseXy(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::GetCurGameMode()
    {
        throw std::logic_error("Not implemented");
    }

    int Application::HeightmapToNormalmap(unsigned short*, unsigned*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::SetFontSelfMaking(CStr const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSplash(int, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::DrawTextRel(float, float, unsigned, CStr const&, unsigned, int)
    {
        throw std::logic_error("Not implemented");
    }

    HWND Application::GetRenderWindow() const
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSprite2Abs(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSprite2Abs(float, float, float, float, float, float, float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    char const* Application::GetCallbackName() const
    {
        throw std::logic_error("Not implemented");
    }

    ui::MbRetCodes Application::RunMsgBoxDlg(CStr const&, CStr const&, unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::DrawTextAbsT(float, float, unsigned, CStr const&, unsigned, int)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::SaveExportDescToFile(fs::FileStream&, bool) const
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawLine(CVector2 const&, CVector2 const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::DrawLine(CVector const&, CVector const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::Pause()
    {
        throw std::logic_error("Not implemented");
    }

    CStr Application::GetClassDesc(Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Application::ClearViewportToBlack()
    {
        //TODO: check this and refactor 
        int v2; // edi
        int v3; // esi

        auto& config = g_Kernel->GetEngineCfg();
        if (IsWindow(config.m_mainWnd))
        {
            v2 = m3d::Application::g_pApp->m_renderer->InScene();
            v3 = 0;
            if (v2 || (v3 = m3d::Application::g_pApp->m_renderer->BeginScene()) != 0)
            {
                m3d::Application::g_pApp->m_renderer->ClearViewport(rend::M3DCLEAR_CZ, -16777216u);
                if (!v2)
                {
                    if (v3)
                        m3d::Application::g_pApp->m_renderer->EndScene();
                }
            }
            m3d::Application::g_pApp->m_renderer->PresentScene();
        }
    }

    CStr const& Application::GetStartupFolder() const
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSpriteAbs(float, float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSpriteAbs(float, float, float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::PutSpriteAbs(float, float, float, float, float, float, float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer& Application::GetAnimatedModelsServer()
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::KillPostEffect(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer& Application::GetDecalsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int Application::DrawTextRelClip(PointBase<float> const&, CStr const&, ui::DrawInfo const&, TextWrapFlags, TextFormatFlags)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer& Application::GetParticlesServer()
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::bIsMousePointing() const
    {
        throw std::logic_error("Not implemented");
    }

    DbgCounter* Application::CreateCounter(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::texGenEnqueue(IGeneratedTexture*)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::HandleCommand(int, CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::RunBenchmark()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Application::CreateQuadsIb()
    {
        throw std::logic_error("Not implemented");
    }

    void Application::EnqueueEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Application::GetEvent(Event&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::finishFontRender()
    {
        throw std::logic_error("Not implemented");
    }

    int Application::DrawStringRelClip(ui::FormattedLine const&, ui::DrawInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned long Application::texGenThread(void*)
    {
        throw std::logic_error("Not implemented");
    }

    int Application::AdjustWindowForChange(HWND, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Application::enterFontRender()
    {
        throw std::logic_error("Not implemented");
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
        //TODO: ...
        throw std::logic_error("Not implemented");
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

        auto idx = m_eventsQueueHead + 1;
        if (idx >=5000)
        {
            idx = 0;
        }
        if (idx != m_eventsQueueTail)
        {
            m_eventsQueue[idx] = ev;
            m_eventsQueueHead = idx;
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

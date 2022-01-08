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

    void logDeviceFunc(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void logSoundFunc(CStr const&)
    {
        throw std::logic_error("Not implemented");
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
                wndClass.hInstance = hInstance; //TODO: not sure about hInstance
                wndClass.hIcon = hIcon;
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
                    rc.bottom - rc.top,
                    rc.left - rc.right,
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
                    M3D_LOG_INFO("----------------------- Engine inited in: " + std::to_string(diffTime));
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
        static std::atomic_bool wmQuitMsg = false;
        m_mouseInfo.ResetDelta();
        MSG msg;
        if (::PeekMessage(&msg, NULL, 0, 0, 0) != FALSE)
        {
            return !wmQuitMsg;
        }
        while (msg.message != WM_QUIT)
        {
            if (!m_isAppActive && ::GetMessage(&msg, NULL, 0, 0) != FALSE)
            {
                while (m_isAppActive)
                {
                    ::DispatchMessage(&msg);
                    if (::GetMessage(&msg, NULL, 0, 0) == FALSE)
                    {
                        break;
                    }
                }
            }
            ::DispatchMessage(&msg);
            if (::PeekMessage(&msg, NULL, 0, 0, 0) != FALSE)
            {
                return !wmQuitMsg;
            }
        }
        wmQuitMsg = true;
        return 0;
    }

    int Application::FillEngineMessages()
    {
        if (m_input == nullptr)
        {
            return 1;
        }
        //m_input->NewFrame();

        //TODO:...
        throw std::logic_error("Not implemented");
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
            std::to_string(osinfo.dwMajorVersion) + '.' +
            std::to_string(osinfo.dwMinorVersion) + '.' +
            std::to_string(osinfo.dwBuildNumber)NFO
        );

        TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
        DWORD size = sizeof(computerName);
        ::GetComputerName(computerName, &size);
        M3D_LOG_INFO("Computer name: " + computerName);

        char cpuInfo[16] = { 0 };
        __cpuid(reinterpret_cast<int*>(cpuInfo), 0x80000000);
        M3D_LOG_INFO("Cpu: " + cpuInfo);

        //TODO: other info...
        throw std::logic_error("Not implemented");
        return 1;
    }

    long Application::MsgProc(HWND hWnd, unsigned uMsg, unsigned wParam, long lParam)
    {
        if (uMsg <= 0x21)
        {
            if (uMsg < 0x20)
            {
                switch (uMsg)
                {
                case 2:
                {
                    ::PostQuitMessage(0);
                    return 0;
                }
                case 6:
                case 7:
                {
                    ::SetCursor(NULL);
                    if (!m_bDXCursorEnabled)
                    {
                        return 1;
                    }
                    if (g_pApp->m_renderer != nullptr)
                    {
                        g_pApp->m_renderer->ShowDXCursor(true);
                    }
                    return 1;
                }
                case 0x10:
                {
                    ::DestroyWindow(hWnd);
                    return 0;
                }
                case 0x1C:
                {
                    //TODO: check this
                    ImmediateMessage(2, wParam, 0, 0, 0, {}, {});
                    return 0;
                }
                }
            }
        }
        if (uMsg != 512)
        {
            if (uMsg == 134)
            {
                ::SetCursor(NULL);
                if (!m_bDXCursorEnabled)
                {
                    return 1;
                }
                if (g_pApp->m_renderer != nullptr)
                {
                    g_pApp->m_renderer->ShowDXCursor(true);
                }
                return 1;
            }
            if (uMsg == 274 && ((wParam & 0xFFF0) == 61760 || (wParam & 0xFFF0) == 61808))
            {
                return 0;
            }
            return ::DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
        if (!m_isAppActive || !m_bDXCursorEnabled)
        {
            return 0;
        }

        PointBase<int> const curMousePos(LOWORD(lParam), HIWORD(lParam));   //TODO: check this
        m_mouseInfo.SetUpForCurPos(curMousePos);
        if (m_bDXCursorEnabled && g_pApp->m_renderer != nullptr)
        {
            g_pApp->m_renderer->MoveDXCursor(LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    }

    void Application::SetCodepage()
    {
        std::set<size_t> codePagesStrings;
        ::EnumSystemCodePages(EnumCodePagesProc, CP_INSTALLED);
        auto const* codePageName = g_Kernel->GetEngineCfg().m_ui_codePageName.GetS();
        UINT codePage = 0;
        //if (codePageName == CStr("CP_UTF8"))
        //{
        //    codePage = CP_UTF8;
        //}
        //else if (auto const offset = codePageName.find("windows-"); offset != CStr::npos)
        //{
        //    codePage = std::atoi(codePageName.substr(offset).c_str());
        //}
        //if (::GetCPInfoEx(codePage, 0, &g_pApp->m_codePage) == FALSE)
        //{
        //    //TODO: handle this
        //    if (::GetCPInfoEx(0, 0, &g_pApp->m_codePage) == FALSE)
        //    {
        //        //TODO: handle this
        //    }
        //}
        //TODO:...
        throw std::logic_error("Not implemented");
    }

    int Application::createRenderer()
    {
        CStr inputDriverName("dxrender9.dll");
        m_hRenderDll = ::LoadLibrary(inputDriverName.c_str());
        if (m_hRenderDll == NULL)
        {
            M3D_LOG_ERR("ERROR! Application::CreateRenderer -- cannot locate renderer driver " + inputDriverName);
            M3D_LOG_ERR("GetLastError() = " + std::to_string(::GetLastError()));
            return 0;
        }
        auto createIRenderer = ::GetProcAddress(m_hRenderDll, "createIRenderer");
        if (createIRenderer == NULL)
        {
            M3D_LOG_ERR("ERROR! Application::CreateRenderer -- cannot get factory");
        }

        m_renderer = reinterpret_cast<rend::IRenderer*>(createIRenderer());
        m_renderer->IncRef();
        M3D_LOG_INFO("NOTE! renderer is bind to " + inputDriverName);
        return true;
    }

    long Application::WndProc(HWND hWnd, unsigned uMsg, unsigned wParam, long lParam)
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
            M3D_LOG_ERR("GetLastError() = " + std::to_string(::GetLastError()));
            return 0;
        }
        auto createIInput = ::GetProcAddress(m_hInputDll, "createIInput");
        if (createIInput == NULL)
        {
            M3D_LOG_ERR("ERROR! Application::CreateInput -- cannot get factory");
        }

        m_input = reinterpret_cast<input::IInput*>(createIInput());
        m_input->IncRef();
        M3D_LOG_INFO("NOTE! input is bind to " + inputDriverName);
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
            M3D_LOG_ERR("GetLastError() = " + std::to_string(::GetLastError()));
            config.m_snd_Enable.SetB(false);
            config.m_mus_Enable.SetB(false);
            return 0;
        }
        auto createISound = ::GetProcAddress(m_hSoundDll, "createISound");
        if (createISound == NULL)
        {
            M3D_LOG_ERR("ERROR! m3dApplication::CreateSound -- cannot get factory");
            config.m_snd_Enable.SetB(false);
            config.m_mus_Enable.SetB(false);
            return 0;
        }
        for (int trial = 0; trial < 3; ++trial)
        {
            m_sound = reinterpret_cast<snd::ISound*>(createISound());
            if (m_sound != nullptr)
            {
                break;
            }
            M3D_LOG_INFO("Warning: sound was not created after trial " + std::to_string(trial));
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
                M3D_LOG_INFO("Warning: sound was not inited after trial " + std::to_string(trial));
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

    int Application::HandleEvent(Event const&)
    {
        //TODO: ...
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    int Application::GetTextExtent(CStr const&, PointBase<float>&, int, BoundsBase<float>*, int*, int*, CStr*, CStr*)
    {
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

            g_Kernel->AddClass(RT_CLASS_LOCAL(SgNode));
            g_Kernel->AddClass(&SgStaticModelNode::m_classSgStaticModelNode);
            g_Kernel->AddClass(&SgAnimatedModelNode::m_classSgAnimatedModelNode);
            g_Kernel->AddClass(&SgGameUnitNode::m_classSgGameUnitNode);
            g_Kernel->AddClass(&SgSpriteNode::m_classSgSpriteNode);
            g_Kernel->AddClass(&SgLinesNode::m_classSgLinesNode);
            g_Kernel->AddClass(&SgParticlesNode::m_classSgParticlesNode);
            g_Kernel->AddClass(&SgSoundSourceNode::m_classSgSoundSourceNode);
            g_Kernel->AddClass(&SgLoadpointNode::m_classSgLoadpointNode);
            g_Kernel->AddClass(&SgPointLightSourceNode::m_classSgPointLightSourceNode);
            g_Kernel->AddClass(&SgProjectorNode::m_classSgProjectorNode);
            g_Kernel->AddClass(&SgDecalsNode::m_classSgDecalsNode);
            g_Kernel->AddClass(&SgParticlesOpaqueNode::m_classSgParticlesOpaqueNode);
            g_Kernel->AddClass(&ui::Wnd::m_classWnd);
            g_Kernel->AddClass(&ui::ImageWnd::m_classImageWnd);
            g_Kernel->AddClass(&ui::EditWnd::m_classEditWnd);
            g_Kernel->AddClass(&ui::SliderWnd::m_classSliderWnd);
            g_Kernel->AddClass(&ui::ScrollWnd::m_classScrollWnd);
            g_Kernel->AddClass(&ui::ButtonWnd::m_classButtonWnd);
            g_Kernel->AddClass(&ui::GlyphButton::m_classGlyphButton);
            g_Kernel->AddClass(&ui::CheckWnd::m_classCheckWnd);
            g_Kernel->AddClass(&ui::ModalWnd::m_classModalWnd);
            g_Kernel->AddClass(&ui::MenuWnd::m_classMenuWnd);
            g_Kernel->AddClass(&ui::StringsListBoxWnd::m_classStringsListBoxWnd);
            g_Kernel->AddClass(&ui::FormattedStringsListBoxWnd::m_classFormattedStringsListBoxWnd);
            g_Kernel->AddClass(&ui::TextBoxWnd::m_classTextBoxWnd);
            g_Kernel->AddClass(&ui::MsgBoxDlg::m_classMsgBoxDlg);
            g_Kernel->AddClass(&ui::ArtDlg::m_classArtDlg);
            g_Kernel->AddClass(&ui::LineWnd::m_classLineWnd);
            g_Kernel->AddClass(&ui::TabWnd::m_classTabWnd);
            g_Kernel->AddClass(&ui::ModelWnd::m_classModelWnd);
            g_Kernel->AddClass(&ui::ModelArrayWnd::m_classModelArrayWnd);
            g_Kernel->AddClass(&ui::SgNodeArrayWnd::m_classSgNodeArrayWnd);
            g_Kernel->AddClass(&ui::ComboBoxWnd::m_classComboBoxWnd);
            g_Kernel->AddClass(&Landscape::m_classLandscape);
            g_Kernel->AddClass(&Level::m_classLevel);
            g_Kernel->AddClass(&Cinematic::m_classCinematic);
            g_Kernel->AddClass(&ui::ProgressBarWnd::m_classProgressBarWnd);

            m_cpuSpeed = GetCpuFrequency(0x64);
            m_profiler_OneFrame = m_profilerStack.AddProfiler("OneFrame", 30);
            m_profiler_Render = m_profilerStack.AddProfiler("Render", 30);
            m_profiler_UiRender = m_profilerStack.AddProfiler(" - UI Render", 30);

            g_Kernel->GetScriptServer().registerGlobalFunction(n_GetComputerName, "GetComputerName", "const char*", "", "returns network name for the current computer");

            m_cameraController = new CameraController;
            m_cinematic = dynamic_cast<Cinematic*>(g_Kernel->New("Cinematic"));
            throw std::logic_error("Not implemented");
        }
        else
        {
            MessageBoxA(NULL, "Error!!! Can't open log!!!", "Critical error", 16);
        }
    }

    void Application::sysError(CStr const& whence, CStr const& assertion)
    {
        panic();
        auto const description = "Assertion failed at " + whence + "\nexpression^ " + assertion;
        M3D_LOG_INFO(description);
        //TODO: check this
        DbgAssert(assertion.c_str(), whence.c_str(), 0);
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
        throw std::logic_error("Not implemented");
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

    void Application::ShowSystemCursor(bool)
    {
        throw std::logic_error("Not implemented");
    }

    long long Application::GetCpuFrequency(unsigned)
    {
        throw std::logic_error("Not implemented");
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
            //TODO: check this
            //g_pApp->m_renderer->SetupDXCursor(&cursor, cursor.m_spot.x, cursor.m_spot.y, 0);
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

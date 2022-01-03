#include "soundconhandler.h"
#include "video/video.h"
#include <atomic>
#include <config.h>
#include <intrin.h>
#include <m3dapp.h>
#include <skelmodel.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>
#include <core/stackwalker.h>
#include <core/timer.h>
#include <scene/servers/dataserver.h>
#include <scene/servers/serverfactories.h>
#include <server/server.h>

namespace
{
    std::set<size_t>* codePagesStringsPtr;
    BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString)
    {
        codePagesStringsPtr->insert(std::atoi(lpCodePageString));
        return TRUE;
    }

    void logDeviceFunc(CStr const&);
    void logSoundFunc(CStr const&);

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
        LOG("ProjectApocalypse - release version build v0.01", LOG_INFO);

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
                LOG("ERROR! Application::init -- cannot create device", LOG_ERR);
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
                LOG("WARNING! Application::init -- zero window rect, expanding", LOG_INFO);
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
                    LOG("NOTE! Application::init -- input module was not inited deliberately", LOG_INFO);
                }
                else if (!createInput())
                {
                    LOG("ERROR! Application::init -- cannot initialize input", LOG_ERR);
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
                    LOG("Error: fail to init impulses", LOG_ERR);
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
                    LOG("----------------------- Engine inited in: " + std::to_string(diffTime), LOG_INFO);
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
            LOG("ERROR! Application::init -- cannot initialize 3d", LOG_ERR);
            return 0;
        }
        LOG("ERROR! Application::init -- cannot open cfg: " + configName, LOG_ERR);
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
        LOG("Path: " + m_startupFolder, LOG_INFO);
        LOG("Exe: " + m_imageName, LOG_INFO);

        OSVERSIONINFO osinfo;
        memset(&osinfo, 0, sizeof(OSVERSIONINFO));
        osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        ::GetVersionEx(&osinfo);
        LOG(
            "Windows version: " +
            std::to_string(osinfo.dwMajorVersion) + '.' +
            std::to_string(osinfo.dwMinorVersion) + '.' +
            std::to_string(osinfo.dwBuildNumber),
            LOG_INFO
        );

        TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
        DWORD size = sizeof(computerName);
        ::GetComputerName(computerName, &size);
        LOG("Computer name: " + computerName);

        char cpuInfo[16] = { 0 };
        __cpuid(reinterpret_cast<int*>(cpuInfo), 0x80000000);
        LOG("Cpu: " + cpuInfo);

        //TODO: other info...

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
    }

    int Application::createRenderer()
    {
        CStr inputDriverName("dxrender9.dll");
        m_hRenderDll = ::LoadLibrary(inputDriverName.c_str());
        if (m_hRenderDll == NULL)
        {
            LOG("ERROR! Application::CreateRenderer -- cannot locate renderer driver " + inputDriverName, LOG_ERR);
            LOG("GetLastError() = " + std::to_string(::GetLastError()));
            return 0;
        }
        auto createIRenderer = ::GetProcAddress(m_hRenderDll, "createIRenderer");
        if (createIRenderer == NULL)
        {
            LOG("ERROR! Application::CreateRenderer -- cannot get factory", LOG_ERR);
        }

        m_renderer = reinterpret_cast<rend::IRenderer*>(createIRenderer());
        m_renderer->IncRef();
        LOG("NOTE! renderer is bind to " + inputDriverName, LOG_INFO);
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
            LOG("ERROR! Application::CreateInput -- cannot locate input driver " + inputDriverName, LOG_ERR);
            LOG("GetLastError() = " + std::to_string(::GetLastError()));
            return 0;
        }
        auto createIInput = ::GetProcAddress(m_hInputDll, "createIInput");
        if (createIInput == NULL)
        {
            LOG("ERROR! Application::CreateInput -- cannot get factory", LOG_ERR);
        }

        m_input = reinterpret_cast<input::IInput*>(createIInput());
        m_input->IncRef();
        LOG("NOTE! input is bind to " + inputDriverName, LOG_INFO);
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
            LOG("ERROR! m3dApplication::CreateSound -- cannot locate sound driver " + inputDriverName, LOG_ERR);
            LOG("GetLastError() = " + std::to_string(::GetLastError()));
            config.m_snd_Enable.SetB(false);
            config.m_mus_Enable.SetB(false);
            return 0;
        }
        auto createISound = ::GetProcAddress(m_hSoundDll, "createISound");
        if (createISound == NULL)
        {
            LOG("ERROR! m3dApplication::CreateSound -- cannot get factory", LOG_ERR);
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
            LOG("Warning: sound was not created after trial " + std::to_string(trial), LOG_INFO);
            ::Sleep(1000);
        }
        if (m_sound != nullptr)
        {
            m_sound->IncRef();
            LOG("NOTE! sound is bind to " + inputDriverName, LOG_INFO);
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
                LOG("Warning: sound was not inited after trial " + std::to_string(trial), LOG_INFO);
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

    void Application::MouseInfo::ResetDelta()
    {
        m_deltaDuringGameFrame.x = 0;
        m_deltaDuringGameFrame.y = 0;
    }

    void Application::MouseInfo::SetUpForCurPos(PointBase<int> const& curPos)
    {
        m_deltaDuringGameFrame.x += curPos.x - m_lastPos.x;
        m_deltaDuringGameFrame.y += curPos.y - m_lastPos.y;
        m_lastPos = curPos;
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

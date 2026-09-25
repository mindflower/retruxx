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
#include <core/scoped_ptr.h>
#include "graphinfo.h"
#include <file/fileserver.h>
#include <file/filestream.h>
#include <client.h>
#include <world.h>
#include <unordered_map>
#include <cstdio>

// The CRT assertion handler the original calls directly.
extern "C" void __cdecl _assert(char const* message, char const* file, unsigned line);

namespace
{
    // RVA 0x8BC9E0
    // Draws the two-triangle quad at vOfs 20 times a frame for two seconds; returns frames drawn per second.
    double TestQuadFps(unsigned vOfs, m3d::cmn::Timer const* timer)
    {
        unsigned const startTime = timer->GetCurTime();
        int numDraws = 0;
        unsigned elapsed = 0;
        do
        {
            m3d::Application::g_pApp->m_renderer->BeginScene();
            numDraws += 20;
            for (int i = 20; i; --i)
            {
                m3d::Application::g_pApp->m_renderer->DrawPrimitive(m3d::rend::M3DPT_TRIANGLESTRIP, vOfs, 2u);
            }
            Sleep(2u);
            m3d::Application::g_pApp->m_renderer->EndScene();
            m3d::Application::g_pApp->m_renderer->PresentScene();
            elapsed = timer->GetCurTime() - startTime;
        } while (elapsed < 2000);
        return static_cast<double>(numDraws) * 1000.0 / static_cast<double>(elapsed);
    }

    // RVA 0x8BCAB0
    // NOTE: the triangle list is drawn as a strip, as shipped.
    double TestGeom(unsigned numTris, m3d::cmn::Timer const* timer)
    {
        unsigned const startTime = timer->GetCurTime();
        int numDraws = 0;
        unsigned elapsed = 0;
        do
        {
            m3d::Application::g_pApp->m_renderer->BeginScene();
            numDraws += 20;
            for (int i = 20; i; --i)
            {
                m3d::Application::g_pApp->m_renderer->DrawPrimitive(m3d::rend::M3DPT_TRIANGLESTRIP, 0, numTris);
            }
            Sleep(2u);
            m3d::Application::g_pApp->m_renderer->EndScene();
            m3d::Application::g_pApp->m_renderer->PresentScene();
            elapsed = timer->GetCurTime() - startTime;
        } while (elapsed < 2000);
        return static_cast<double>(numDraws) * 1000.0 / static_cast<double>(elapsed);
    }
}  // namespace

namespace
{
    unsigned int __fastcall VecToRgba(CVector const& normal, float h)
    {
        // RVA 0x7AF9F0 - packs a unit normal into RGB and the height into alpha.
        return (static_cast<unsigned int>(static_cast<int>(h * 255.0f)) << 24) |
            (static_cast<unsigned char>(static_cast<int>(normal.x * 127.0f + 128.0f)) << 16) |
            (static_cast<unsigned char>(static_cast<int>(normal.y * 127.0f + 128.0f)) << 8) |
            static_cast<unsigned char>(static_cast<int>(normal.z * 127.0f + 128.0f));
    }
}  // namespace

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

    using CreateIRendererType = m3d::rend::IRenderer*(__cdecl*)(m3d::Kernel*);
    using CreateIInputType = m3d::input::IInput*(__cdecl*)(m3d::Kernel*);
    using CreateISoundType = snd::ISound*(__cdecl*)(m3d::Kernel*);
}  // namespace

namespace ai
{
    extern CServer* pServer;
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Application)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Application);

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
                wndClass.hInstance = hInstance;  //TODO: not sure about hInstance
                wndClass.hIcon = hIcon;
                wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
                wndClass.hbrBackground = 0;
                wndClass.lpszMenuName = 0;
                wndClass.lpszClassName = TEXT("Miracle3d");
                ::RegisterClass(&wndClass);

                RECT rc;
                ::SetRect(
                    &rc, 0, 0, g_Kernel->GetEngineCfg().m_r_width.GetI(), g_Kernel->GetEngineCfg().m_r_height.GetI());
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
                    NULL);
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
                        SWP_SHOWWINDOW);
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
        ev.m_eventType = msg;  //TODO: check this
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
            if (g_Kernel->GetEngineCfg().m_snd_Enable.GetB() || g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
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
                        if (rend == 2 || m_appNeedToRedraw)
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
                            // RVA 0x5A3AD0 (performance graphs)
                            static unsigned const colArray[7] = {
                                0xFFFF0000, 0xFF0000FF, 0xFF00FF00, 0xFFFF00FF, 0xFF00FFFF, 0xFFFFFF00, 0xFF888888};
                            static rend::VbHandle m_graphsVB =
                                g_pApp->m_renderer->AddVb(rend::VERTEX_XYZWC, 7025, CStr("GraphStats"), 512u);
                            static GraphInfo infos[7];

                            rend::RenderStats rs;
                            g_pApp->m_renderer->GetStats(rs);
                            int stage = 0;
                            float xx = 20.0f;
                            SetFont(CStr("Lucida Console"), 10.0f, 1u, g_pApp->m_codePage.CodePage);
                            if (g_Kernel->GetEngineCfg().m_r_graphicalStats_Fps.GetB())
                            {
                                unsigned const frameTime = g_Kernel->GetTimer().m_lastFrameTime;
                                float const ft = frameTime ? static_cast<float>(frameTime) : 1.0f;
                                infos[0].AddFiltred(1000.0f / ft, 50);
                                DrawTextAbs(20.0f, 700.0f, colArray[0], CStr("fps"), 0, -1);
                                stage = 1;
                                xx = 90.0f;
                            }
                            auto const addGraph = [&](char const* label, float value)
                            {
                                DrawTextAbs(xx, 700.0f, colArray[stage], CStr(label), 0, -1);
                                infos[stage++].AddValue(value);
                                xx += 70.0f;
                            };
                            if (g_Kernel->GetEngineCfg().m_r_graphicalStats_Dip.GetB())
                            {
                                addGraph("dip/f", static_cast<float>(rs.DIPs));
                            }
                            if (g_Kernel->GetEngineCfg().m_r_graphicalStats_Dp.GetB())
                            {
                                addGraph("dp/f", static_cast<float>(rs.DPs));
                            }
                            if (g_Kernel->GetEngineCfg().m_r_graphicalStats_Tris.GetB())
                            {
                                addGraph("tris/f", static_cast<float>(rs.polyCount));
                            }
                            if (g_Kernel->GetEngineCfg().m_r_graphicalStats_Fillrate.GetB())
                            {
                                addGraph("pixs/f", static_cast<float>(m_frameFillRate));
                            }
                            if (g_Kernel->GetEngineCfg().m_r_graphicalStats_MemUsed.GetB())
                            {
                                addGraph("memUsed", static_cast<float>(g_Kernel->debugMemUsed()));
                            }
                            if (g_Kernel->GetEngineCfg().m_r_graphicalStats_MemAlloc.GetB())
                            {
                                addGraph("memAlloc", static_cast<float>(g_Kernel->debugMemAllocated()));
                            }
                            if (stage != 7 && g_Kernel->GetEngineCfg().m_r_graphicalStats_MemOverhead.GetB())
                            {
                                DrawTextAbs(xx, 700.0f, colArray[stage], CStr("memOverh"), 0, -1);
                                infos[stage++].AddValue(static_cast<float>(g_Kernel->debugMemOverhead()));
                            }

                            int vofs = 0;
                            auto* v = static_cast<rend::VertexXYZWC*>(
                                g_pApp->m_renderer->LockVbStreaming(m_graphsVB, 1000 * stage + 25, vofs, nullptr));
                            auto const set = [](rend::VertexXYZWC& vert, float x, float y, unsigned c)
                            {
                                vert.x = x;
                                vert.y = y;
                                vert.z = 0.0f;
                                vert.w = 0.1f;
                                vert.c = c;
                            };
                            // Axes, then a horizontal grid line every 70 pixels.
                            set(v[0], 10.0f, 20.0f, 0xFFFFFFFF);
                            set(v[1], 10.0f, 700.0f, 0xFFFFFFFF);
                            set(v[2], 1015.0f, 700.0f, 0xFFFFFFFF);
                            for (int line = 0; line < 9; ++line)
                            {
                                float const y = 630.0f - 70.0f * static_cast<float>(line);
                                set(v[3 + line * 2], 10.0f, y, 0xFF777777);
                                set(v[4 + line * 2], 1000.0f, y, 0xFF777777);
                            }
                            // NOTE: vertex 21 is left unwritten; the graphs start at vertex 22.
                            for (int g = 0; g < stage; ++g)
                            {
                                for (int i = 0; i < 1000; ++i)
                                {
                                    set(v[22 + g * 1000 + i],
                                        static_cast<float>(i) + 12.0f,
                                        700.0f - infos[g].GetValue(i) * 350.0f,
                                        colArray[g]);
                                }
                            }
                            g_pApp->m_renderer->UnlockVb(m_graphsVB);
                            g_pApp->m_renderer->SetAlphaTest(0);
                            g_pApp->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
                            g_pApp->m_renderer->DisableTextureStages(1);
                            g_pApp->m_renderer->SetToStream0(m_graphsVB);
                            g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_LINESTRIP, 0, 2u);
                            g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_LINELIST, 3u, 9u);
                            for (int g = 0; g < stage; ++g)
                            {
                                g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_LINESTRIP, 22 + g * 1000, 999u);
                            }
                            for (int g = 0; g < stage; ++g)
                            {
                                int n = 0;
                                for (int j = 70; j < 700; j += 70)
                                {
                                    ++n;
                                    CStr const text(
                                        static_cast<float>(static_cast<float>(n) * infos[g].GetAverValue()) * 0.2f);
                                    DrawTextAbs(
                                        18.0f,
                                        (700.0f - static_cast<float>(j)) + static_cast<float>(g * 10),
                                        colArray[g],
                                        text,
                                        0,
                                        -1);
                                }
                            }
                        }
                        float statsY = 30.0f;
                        if (m_bDrawMemoryStats)
                        {
                            ui::FontParams params;
                            params.ttfParams.codePage = g_pApp->m_codePage.CodePage;
                            params.ttfParams.style = 1;
                            ui::Wnd::GetGfxServer()->SetFont(
                                CStr("Lucida Console"), 10.0f, ui::FONT_TYPE_WINDOWS, params);
                            CStr statStr;
                            statStr.format("mem used:          %10d", g_Kernel->debugMemUsed());
                            DrawTextAbs(0.5f, 30.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("mem allocated:     %10d", g_Kernel->debugMemAllocated());
                            DrawTextAbs(0.5f, 45.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("mem overhead:      %10d", g_Kernel->debugMemOverhead());
                            DrawTextAbs(0.5f, 60.0f, 0xFFFFFFFF, statStr, 0, -1);
                            float y = 75.0f;
                            if (g_pApp->m_sound)
                            {
                                unsigned curUsedBySound = 0;
                                unsigned maxUsedBySound = 0;
                                g_pApp->m_sound->GetMemUsage(curUsedBySound, maxUsedBySound);
                                statStr.format("mem used by sound: %10d", curUsedBySound);
                                DrawTextAbs(0.5f, 75.0f, 0xFFFFFFFF, statStr, 0, -1);
                                y = 90.0f;
                            }
                            statsY = y + 10.0f;
                        }
                        if (m_bDrawStats)
                        {
                            ui::FontParams params;
                            params.ttfParams.codePage = m_codePage.CodePage;
                            params.ttfParams.style = 1;
                            ui::Wnd::GetGfxServer()->SetFont(
                                CStr("Lucida Console"), 10.0f, ui::FONT_TYPE_WINDOWS, params);
                            for (unsigned k = 0; k < m_profilerStack.m_numProfilers; ++k)
                            {
                                Profiler* const profiler = m_profilerStack.m_stack[k];
                                CStr line;
                                double const ms = static_cast<double>(profiler->m_averageClocks) /
                                    profiler->m_performanceCounterFrequency * 1000.0;
                                line.format("%s: %.04f", profiler->GetName(), ms);
                                DrawTextAbs(0.5f, statsY, 0xFFFFFFFF, line, 0, -1);
                                statsY += 15.0f;
                            }
                        }
                        if (m_bDrawCounters)
                        {
                            float y = statsY + 15.0f;
                            ui::FontParams params;
                            params.ttfParams.codePage = g_pApp->m_codePage.CodePage;
                            params.ttfParams.style = 1;
                            ui::Wnd::GetGfxServer()->SetFont(
                                CStr("Lucida Console"), 10.0f, ui::FONT_TYPE_WINDOWS, params);
                            for (unsigned m = 0; m < m_counterStack.m_numCounters; ++m)
                            {
                                DbgCounter* const counter = m_counterStack.m_stack[m];
                                CStr statStr = CStr(counter->m_name.c_str()) + CStr(": ");
                                switch (counter->m_curType)
                                {
                                case DbgCounter::DBG_COUNTER_INT:
                                    statStr += CStr(counter->m_i);
                                    break;
                                case DbgCounter::DBG_COUNTER_FLOAT:
                                {
                                    CStr value;
                                    value.format("%.04f", counter->m_f);
                                    statStr += value;
                                    break;
                                }
                                case DbgCounter::DBG_COUNTER_BOOL:
                                    statStr += CStr(counter->m_b ? "true" : "false");
                                    break;
                                default:
                                    statStr += CStr(counter->m_s.c_str());
                                    break;
                                }
                                CStr text;
                                text = statStr;
                                DrawTextAbsT(0.5f, y, 0xFFFFFFFF, text, 0, -1);
                                y += 15.0f;
                            }
                            for (unsigned n = 0; n < m_counterStack.m_numStrings; ++n)
                            {
                                CStr text;
                                text = CStr(m_counterStack.m_stringStack[n].c_str());
                                DrawTextAbsT(0.5f, y, 0xFFFFFFFF, text, 0, -1);
                                y += 15.0f;
                            }
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
                            ui::FontParams params;
                            params.ttfParams.codePage = g_pApp->m_codePage.CodePage;
                            params.ttfParams.style = 1;
                            ui::Wnd::GetGfxServer()->SetFont(
                                CStr("Lucida Console"), 10.0f, ui::FONT_TYPE_WINDOWS, params);
                            CStr statStr;
                            rend::DeviceMemStats ms;
                            g_pApp->m_renderer->GetDeviceMemStats(ms);
                            double const kb = 0.0009765625;
                            statStr.format("DynTex count:    %7d", ms.DynamicTexCount);
                            DrawTextRel(774.0f, 200.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("DynTex size:%#05.3fKb", static_cast<double>(ms.DynamicTexSize) * kb);
                            DrawTextRel(774.0f, 212.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("StatTex count:   %7d", ms.StaticTexCount);
                            DrawTextRel(774.0f, 224.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("StatTex size:%#05.3fKb", static_cast<double>(ms.StaticTexSize) * kb);
                            DrawTextRel(774.0f, 236.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format(
                                "TotalTex size:%#05.3fKb",
                                static_cast<double>(ms.DynamicTexSize + ms.StaticTexSize) * kb);
                            DrawTextRel(774.0f, 248.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("DynVB count:     %7d", ms.DynamicVBCount);
                            DrawTextRel(774.0f, 268.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("DynVB size: %5.3fKb", static_cast<double>(ms.DynamicVBSize) * kb);
                            DrawTextRel(774.0f, 280.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("StatVB count:    %7d", ms.StaticVBCount);
                            DrawTextRel(774.0f, 292.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("StatVB size:%5.3fKb", static_cast<double>(ms.StaticVBSize) * kb);
                            DrawTextRel(774.0f, 304.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("VBpool size:%5.3fKb", static_cast<double>(ms.VBPoolsSize) * kb);
                            DrawTextRel(774.0f, 316.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format(
                                "TotalVB size:%5.3fKb",
                                static_cast<double>(ms.DynamicVBSize + ms.StaticVBSize + ms.VBPoolsSize) * kb);
                            DrawTextRel(774.0f, 328.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("DynIB count:     %7d", ms.DynamicIBCount);
                            DrawTextRel(774.0f, 348.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("DynIB size: %5.3fKb", static_cast<double>(ms.DynamicIBSize) * kb);
                            DrawTextRel(774.0f, 360.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("StatIB count:    %7d", ms.StaticIBCount);
                            DrawTextRel(774.0f, 372.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("StatIB size:%5.3fKb", static_cast<double>(ms.StaticIBSize) * kb);
                            DrawTextRel(774.0f, 384.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("IBpool size:%5.3fKb", static_cast<double>(ms.IBPoolsSize) * kb);
                            DrawTextRel(774.0f, 396.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format(
                                "TotalIB size:%5.3fKb",
                                static_cast<double>(ms.DynamicIBSize + ms.StaticIBSize + ms.IBPoolsSize) * kb);
                            DrawTextRel(774.0f, 408.0f, 0xFFFFFFFF, statStr, 0, -1);
                            unsigned const total = ms.DynamicTexSize + ms.StaticTexSize + ms.DynamicVBSize +
                                ms.StaticVBSize + ms.VBPoolsSize + ms.DynamicIBSize + ms.StaticIBSize + ms.IBPoolsSize +
                                ms.RtTexSize;
                            statStr.format(
                                "Total size:%5.3fKb(%5.3fMb)",
                                static_cast<double>(total) * kb,
                                0.00000095367432 * static_cast<double>(total));
                            DrawTextRel(724.0f, 428.0f, 0xFFFFFFFF, statStr, 0, -1);
                        }
                        if (m_bShowRenderStats)
                        {
                            ui::FontParams params;
                            params.ttfParams.codePage = g_pApp->m_codePage.CodePage;
                            params.ttfParams.style = 1;
                            ui::Wnd::GetGfxServer()->SetFont(
                                CStr("Lucida Console"), 10.0f, ui::FONT_TYPE_WINDOWS, params);
                            CStr statStr;
                            rend::RenderStats rs;
                            g_pApp->m_renderer->GetStats(rs);
                            DrawTextRel(
                                1024.0f - static_cast<float>(m_frameStats.length()) * 9.0f,
                                0.5f,
                                0xFFFFFFFF,
                                m_frameStats,
                                0,
                                -1);
                            statStr.format("%-12s %8d", "t/f:", rs.polyCount);
                            DrawTextRel(824.0f, 24.0f, 0xFFFFFFFF, statStr, 0, -1);
                            int const fps =
                                static_cast<int>(g_Kernel->GetTimer().m_timescale * g_Kernel->GetTimer().m_fps);
                            statStr.format("%-12s %8d", "t/sec:", rs.polyCount * fps);
                            DrawTextRel(824.0f, 36.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "dip/f:", rs.DIPs);
                            DrawTextRel(824.0f, 48.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "dp/f:", rs.DPs);
                            DrawTextRel(824.0f, 60.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "tex/f:", rs.swTextures);
                            DrawTextRel(824.0f, 72.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "rs/f:", rs.swRenderStates);
                            DrawTextRel(824.0f, 84.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "vs/f:", rs.swVertexShaders);
                            DrawTextRel(824.0f, 96.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "ps/f:", rs.swPixelShaders);
                            DrawTextRel(824.0f, 108.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "stageS/f:", rs.swTextureStageStates);
                            DrawTextRel(824.0f, 120.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "samplerS/f:", rs.swTextureSamplerStates);
                            DrawTextRel(824.0f, 132.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "xform/f:", rs.swMatrices);
                            DrawTextRel(824.0f, 144.0f, 0xFFFFFFFF, statStr, 0, -1);
                            statStr.format("%-12s %8d", "rt/f:", rs.swRenderTargets);
                            DrawTextRel(824.0f, 156.0f, 0xFFFFFFFF, statStr, 0, -1);
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
                        m_renderer->ScreenShot(0, -1, -1);
                        if (!m_screenShotPendingAlways)
                        {
                            m_screenShotPending = 0;
                            M3D_ENGINE_CFG.m_console->PrintF("Screenshot saved\n");
                            M3D_LOG_INFO("Screen shot");
                        }
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
        if (!m_input)
        {
            return 1;
        }

        m_input->NewFrame();

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
                ev.m_eventType = 8 - (param3 != 0);
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
                for (int i = 0;; bits = i)
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
                    if (m_eventsQueueTail != head)
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
                        if (btnsMask != ((1 << j) & m_prevJoystickBtnsMask))
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
        // RVA 0x59D2B0
        M3D_LOG_INFO(CStr("Path: ") + m_startupFolder);
        M3D_LOG_INFO(CStr("Exe: ") + m_imageName);

        OSVERSIONINFOA osinfo;
        memset(&osinfo, 0, sizeof(osinfo));
        osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
        ::GetVersionExA(&osinfo);
        M3D_LOG_INFO(
            CStr("Windows version: ") + CStr(osinfo.dwMajorVersion) + CStr(".") + CStr(osinfo.dwMinorVersion) +
            CStr(".") + CStr(osinfo.dwBuildNumber));

        char computerName[1024];
        DWORD size = 1024;
        ::GetComputerNameA(computerName, &size);
        M3D_LOG_INFO(CStr("Computer name: ") + CStr(computerName));

        int cpuInfo[4] = {0};
        __cpuid(cpuInfo, 0);
        char cpuName[16];
        memcpy(cpuName, &cpuInfo[1], 4);      // ebx
        memcpy(cpuName + 4, &cpuInfo[3], 4);  // edx
        memcpy(cpuName + 8, &cpuInfo[2], 4);  // ecx
        memset(cpuName + 12, 0, 4);
        M3D_LOG_INFO(CStr("Cpu: ") + CStr(cpuName));
        M3D_LOG_INFO(CStr("Cpu clock: ~") + CStr(static_cast<int64_t>(m_cpuSpeed / 1000000)));

        __cpuid(cpuInfo, 1);
        if (!(cpuInfo[3] & 0x800000))
        {
            M3D_LOG_INFO("Processor should support mmx in order to run this program");
            return 0;
        }
        M3D_LOG_INFO("Processor is mmx able");

        MEMORYSTATUS memStatus;
        memset(&memStatus, 0, sizeof(memStatus));
        memStatus.dwLength = sizeof(MEMORYSTATUS);
        ::GlobalMemoryStatus(&memStatus);
        M3D_LOG_INFO(
            CStr("Physical memory: ") + CStr(static_cast<unsigned long>(memStatus.dwTotalPhys >> 20)) + CStr(" MB"));
        M3D_LOG_INFO(
            CStr("Total memory:    ") + CStr(static_cast<unsigned long>(memStatus.dwTotalPageFile >> 20)) +
            CStr(" MB"));
        M3D_LOG_INFO(
            CStr("Physical memory available: ") + CStr(static_cast<unsigned long>(memStatus.dwAvailPhys >> 20)) +
            CStr(" MB"));
        M3D_LOG_INFO(
            CStr("Total memory available:    ") + CStr(static_cast<unsigned long>(memStatus.dwAvailVirtual >> 20)) +
            CStr(" MB"));
        return 1;
    }

    long Application::MsgProc(HWND hWnd, unsigned uMsg, unsigned wParam, long lParam)
    {
        //TODO: check this and refactor
        int result;                  // eax
        PointBase<int> curMousePos;  // [esp+8h] [ebp-30h] BYREF
        CStr param4;                 // [esp+10h] [ebp-28h] BYREF

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
            M3D_LOG_INFO(
                "SetCodepage -- code page is not supported : " + codePageName +
                " forcing ANSI, some chars will be not available");
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
        auto const ee = ::GetLastError();
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
        // RVA 0x5A85B0
        static bool bAltEnterActive = false;
        static bool bCtrlShiftActive = false;

        int res = 0;
        switch (ev.m_eventType)
        {
        case EV_APP_QUIT:
        {
            ::PostMessageA(M3D_ENGINE_CFG.m_mainWnd, WM_CLOSE, 0, 0);
            return 1;
        }
        case EV_ACTIVATE_APP:
        {
            m_isAppActive = ev.m_intEv[0];
            if (m_input)
            {
                m_input->SetActiveState(m_isAppActive);
            }
            if (m_renderer)
            {
                m_renderer->SetActiveState(m_isAppActive);
            }
            M3D_KERNEL->GetTimer().SetActiveState(m_isAppActive);
            if (m_isAppActive)
            {
                if (M3D_ENGINE_CFG.m_clipCursorWithinRenderWnd.GetB())
                {
                    CaptureAndClipSystemCursor(true);
                }
                ShowSystemCursor(m_bDXCursorEnabled);
                if (M3D_APP->m_sound)
                {
                    M3D_APP->m_sound->PauseAllSounds(false);
                }
            }
            else
            {
                ::ReleaseCapture();
                ::ClipCursor(nullptr);
                while (::ShowCursor(TRUE) < 0)
                {
                }
                if (M3D_APP->m_sound)
                {
                    M3D_APP->m_sound->PauseAllSounds(true);
                }
            }
            return 1;
        }
        case EV_CHANGE_DISPLAY_MODE:
        {
            SwitchDisplayModes(M3D_ENGINE_CFG.m_mainWnd, ev.m_intEv[0], ev.m_intEv[1], ev.m_uintEv[2] != 0);
            return 1;
        }
        case EV_DISPLAY_CHANGED:
        {
            // Not consumed: still goes on to the focus handler and the windows below.
            M3D_ENGINE_CFG.m_console->CheckResize(
                m_renderer->GetViewport().m_width, m_renderer->GetViewport().m_height);
            m_appNeedToRedraw = 1;
            break;
        }
        case EV_KEY_DOWN:
        {
            if (ev.m_byteEv[0] == KBD_F11)
            {
                // F11 toggles a screenshot every frame; switching it on needs debug mode.
                if (m_screenShotPendingAlways)
                {
                    m_screenShotPendingAlways = 0;
                }
                else if (M3D_ENGINE_CFG.m_debugMode.GetB())
                {
                    m_screenShotPendingAlways = 1;
                    m_screenShotPending = 1;
                }
                res = 1;
            }
            else if (ev.m_byteEv[0] == KBD_F12)
            {
                m_screenShotPending = 1;
                res = 1;
            }
            else if (m_waitForAnykey)
            {
                // NOTE: unlike key up and the mouse buttons, key down only restamps the time and
                // leaves m_waitForAnykey set and the timer paused.
                m_timeFromLevelLoaded = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
                res = 1;
            }

            if (ev.m_ushortEv[0] == (KBD_LALT | KBD_ENTER))
            {
                if (M3D_ENGINE_CFG.m_g_altEnterAllow.GetB() && !bAltEnterActive)
                {
                    bAltEnterActive = true;
                    bool const fullScreen = !M3D_ENGINE_CFG.m_r_fullScreen.GetB();
                    M3D_ENGINE_CFG.m_console->executeCommand(
                        CStr("/r_videomode ") + CStr(M3D_ENGINE_CFG.m_r_width.GetI()) + CStr(" ") +
                        CStr(M3D_ENGINE_CFG.m_r_height.GetI()) + CStr(" ") + CStr(static_cast<int>(fullScreen)));
                }
                res = 1;
            }

            if ((ev.m_ushortEv[0] & 0x8000) && (ev.m_ushortEv[0] & KBD_LSHIFT))
            {
                if (!bCtrlShiftActive)
                {
                    bCtrlShiftActive = true;
                    ChangeLanguage();
                }
                return 1;
            }

            if (res)
            {
                return res;
            }

            if (ev.m_byteEv[3] != KEY_GRAVE)
            {
                break;
            }

            // The console key.
            if (M3D_ENGINE_CFG.m_console->isActive())
            {
                SetKeyboardFocus(M3D_APP ? static_cast<IEventHandler*>(M3D_APP) : nullptr);
                return 1;
            }
            if (!m_isConsoleAllowed)
            {
                return 1;
            }
            SetKeyboardFocus(
                M3D_ENGINE_CFG.m_console ? static_cast<IEventHandler*>(M3D_ENGINE_CFG.m_console) : nullptr);
            return 1;
        }
        case EV_KEY_UP:
        {
            if (M3dVideoPlayer->IsVideoPlaing() && ev.m_ushortEv[0] == KBD_ESC)
            {
                M3dVideoPlayer->Stop();
                res = 1;
            }
            if (m_waitForAnykey)
            {
                m_timeFromLevelLoaded = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
                M3D_KERNEL->GetTimer().SetActiveState(1);
                m_waitForAnykey = false;
                res = 1;
            }
            if (bAltEnterActive)
            {
                auto const key = static_cast<short>(ev.m_ushortEv[0]);
                if (key == KBD_ENTER || key == KBD_LALT || key == (KBD_LALT | KBD_ENTER))
                {
                    bAltEnterActive = false;
                    res = 1;
                }
            }
            if (bCtrlShiftActive && (ev.m_ushortEv[0] & 0xA000) != 0)
            {
                bCtrlShiftActive = false;
                return 1;
            }
            if (res)
            {
                return res;
            }
            break;
        }
        case EV_MOUSE_LBTN:
        case EV_MOUSE_RBTN:
        case EV_MOUSE_MBTN:
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
            break;
        }

        // Keys go to the keyboard focus first, unless the application itself holds it.
        if ((ev.m_eventType == EV_KEY_DOWN || ev.m_eventType == EV_KEY_UP) && m_focusKbdEntity &&
            m_focusKbdEntity != static_cast<IEventHandler*>(this))
        {
            res = m_focusKbdEntity->HandleEvent(ev);
            if (res)
            {
                return res;
            }
        }

        // The wheel scrolls the console while it has the focus.
        if (ev.m_eventType == EV_MOUSE_WHEEL)
        {
            IConsole* const console = M3D_ENGINE_CFG.m_console;
            if (m_focusKbdEntity == (console ? static_cast<IEventHandler*>(console) : nullptr))
            {
                // NOTE: no null check here; with no console and no focus this calls through null.
                res = static_cast<IEventHandler*>(M3D_ENGINE_CFG.m_console)->HandleEvent(ev);
                if (res)
                {
                    return res;
                }
            }
        }

        return WndStation::ProcessEvent(ev);
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
        // RVA 0x59C500
        delete m_pImpulses;
        m_pImpulses = nullptr;
    }

    void Application::DetailSettings::RestoreGameSettings()
    {
        // RVA 0x5A1340
        // The binary inlines CVar::Set with flags honoured (read-only cvars are left alone).
        char buffer[64];
        sprintf_s(buffer, "%d", m_dsShadows);
        g_Kernel->GetEngineCfg().m_dsShadows.Set(buffer, false);
        sprintf_s(buffer, "%.2f", m_lsViewDistanceDivider);
        g_Kernel->GetEngineCfg().m_lsViewDistanceDivider.Set(buffer, false);
        sprintf_s(buffer, "%.2f", m_NPatchLevel);
        g_Kernel->GetEngineCfg().m_NPatchLevel.Set(buffer, false);
    }

    void Application::DetailSettings::SaveGameSettings()
    {
        m_dsShadows = g_Kernel->GetEngineCfg().m_dsShadows.GetB();
        m_lsViewDistanceDivider = g_Kernel->GetEngineCfg().m_lsViewDistanceDivider.GetF();
        m_NPatchLevel = g_Kernel->GetEngineCfg().m_NPatchLevel.GetF();
    }

    void Application::DetailSettings::SetMenuLevelSettings()
    {
        // RVA 0x5A21C0
        char buffer[64];
        sprintf_s(buffer, "%d", 1);
        g_Kernel->GetEngineCfg().m_dsShadows.Set(buffer, false);
        sprintf_s(buffer, "%.2f", 1.0);
        g_Kernel->GetEngineCfg().m_lsViewDistanceDivider.Set(buffer, false);
        sprintf_s(buffer, "%.2f", 0.0);
        g_Kernel->GetEngineCfg().m_NPatchLevel.Set(buffer, false);
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

    Application::MouseInfo::MouseInfo() : m_deltaDuringGameFrame(0, 0), m_lastPos(0, 0)
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
        // RVA 0x75C9F0
    }

    void Application::DrawTri(CVector* tri, unsigned clr)
    {
        auto vb = m3d::Application::g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZCT1);
        int vofs = 0;
        auto v3 = (m3d::rend::VertexXYZCT1*)m3d::Application::g_pApp->m_renderer->LockVbStreaming(vb, 3, vofs, 0);
        v3->x = tri->x;
        v3->y = tri->y;
        v3->z = tri->z;
        v3->c = clr;
        v3->tu = 0.0;
        v3->tv = 0.0;
        v3[1].x = tri[1].x;
        v3[1].y = tri[1].y;
        v3[1].z = tri[1].z;
        v3[1].c = clr;
        v3[1].tu = 1.0;
        v3[1].tv = 0.0;
        v3[2].x = tri[2].x;
        v3[2].y = tri[2].y;
        v3[2].z = tri[2].z;
        v3[2].c = clr;
        v3[2].tu = 1.0;
        v3[2].tv = 1.0;
        m3d::Application::g_pApp->m_renderer->UnlockVb(vb);
        m3d::Application::g_pApp->m_renderer->SetToStream0(vb);
        m3d::Application::g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_TRIANGLELIST, vofs, 1u);
    }

    void Application::ForbidRendering()
    {
        // RVA 0x59C4A0
        m_isRenderingAllowed = false;
    }

    Class* Application::GetClass() const
    {
        // RVA 0x59BC10
        return RT_CLASS_LOCAL(Application);
    }

    DataServer& Application::GetStaticModelsServer()
    {
        // RVA 0x6A5A50
        return *m_serverStaticModels;
    }

    void Application::DrawCross(CVector const& org, float size, unsigned color)
    {
        // TODO: generated code Application::DrawCross
        // Draw a 3D cross centered at 'org' with arms of length 'size' in each axis

        // Loop through each axis (X, Y, Z)
        for (int axis = 0; axis < 3; ++axis)
        {
            // Create vectors for the positive and negative ends of this axis
            CVector positiveEnd = ZeroVector;
            CVector negativeEnd = ZeroVector;

            // Set the appropriate component for this axis
            switch (axis)
            {
            case 0:  // X-axis
                positiveEnd.x = size;
                negativeEnd.x = -size;
                break;

            case 1:  // Y-axis
                positiveEnd.y = size;
                negativeEnd.y = -size;
                break;

            case 2:  // Z-axis
                positiveEnd.z = size;
                negativeEnd.z = -size;
                break;
            }

            // Calculate the actual world positions
            CVector from = org + negativeEnd;
            CVector to = org + positiveEnd;

            // Draw the line for this axis
            DrawLine(from, to, color);
        }
    }

    CStr const& Application::GetImageName() const
    {
        // RVA 0x59BF70
        return m_imageName;
    }

    int Application::GetCurDifficultyLevel() const
    {
        // RVA 0x5AA950
        return 0;
    }

    char* Application::GetWindowTitle() const
    {
        return m_strWindowTitle;
    }

    void Application::MiniDump()
    {
        // RVA 0x59BC80
        unsigned long dummy = 0;
        WriteDump("game.dmp", nullptr, dummy);
    }

    void Application::DrawLogo()
    {
        // RVA 0x59EE90
        g_pApp->m_renderer->PushBlend(rend::BM_COLOR);
        g_pApp->m_renderer->PushZbState(rend::ZB_DISABLE);
        g_pApp->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
        g_pApp->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
        g_pApp->m_renderer->SetTexture(0, m_texLogo, -1.0);
        float x1 = 924.0f;
        float y1 = 0.0f;
        float x2 = 1024.0f;
        float y2 = 100.0f;
        g_pApp->m_renderer->RelToAbs(x1, y1);
        g_pApp->m_renderer->RelToAbs(x2, y2);
        g_pApp->PutSpriteAbs(x1, y1, x2, y2, 0x80FFFFFF);
        g_pApp->m_renderer->PopZbState();
        g_pApp->m_renderer->PopBlend();
    }

    float Application::GetOnScreenSize(CVector const& o, float radius)
    {
        CVector myOrg = o;
        CVector camOrg = M3D_RENDERER->MatGetOrgInv();
        auto oa = sqrt(
            (myOrg.z - camOrg.z) * (myOrg.z - camOrg.z) + (myOrg.y - camOrg.y) * (myOrg.y - camOrg.y) +
            (myOrg.x - camOrg.x) * (myOrg.x - camOrg.x));

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
        // RVA 0x5AA990
        return 0;
    }

    void Application::PutSplashCallBack(int proc, void* data)
    {
        M3D_APP->PutSplash(proc, *(char const**)data);
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

    bool Application::CheckCommandLineParam(char const* param) const
    {
        // RVA 0x5A32D0
        return m_cmdLine.CheckParam(param);
    }

    unsigned long Application::GetLocale() const
    {
        // RVA 0x59F5B0
        int langId = 0;
        sscanf_s(g_Kernel->GetEngineCfg().m_input_additionalKeyboardLayot.GetS(), "%x", &langId);
        return static_cast<unsigned short>(langId);
    }

    void Application::DrawQuad(CVector* tri, unsigned clr)
    {
        // RVA 0x7B06C0
        auto vb = g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZCT1);
        int vofs = 0;
        auto* v = static_cast<rend::VertexXYZCT1*>(g_pApp->m_renderer->LockVbStreaming(vb, 6, vofs, 0));
        // NOTE: the texture coordinates of the second triangle do not match the first one's
        // (tri[2] gets (1, 0) and tri[3] gets (1, 1)); kept as shipped.
        v[0].x = tri[0].x;
        v[0].y = tri[0].y;
        v[0].z = tri[0].z;
        v[0].c = clr;
        v[0].tu = 0.0f;
        v[0].tv = 0.0f;
        v[1].x = tri[1].x;
        v[1].y = tri[1].y;
        v[1].z = tri[1].z;
        v[1].c = clr;
        v[1].tu = 1.0f;
        v[1].tv = 0.0f;
        v[2].x = tri[2].x;
        v[2].y = tri[2].y;
        v[2].z = tri[2].z;
        v[2].c = clr;
        v[2].tu = 1.0f;
        v[2].tv = 1.0f;
        v[3].x = tri[0].x;
        v[3].y = tri[0].y;
        v[3].z = tri[0].z;
        v[3].c = clr;
        v[3].tu = 0.0f;
        v[3].tv = 0.0f;
        v[4].x = tri[2].x;
        v[4].y = tri[2].y;
        v[4].z = tri[2].z;
        v[4].c = clr;
        v[4].tu = 1.0f;
        v[4].tv = 0.0f;
        v[5].x = tri[3].x;
        v[5].y = tri[3].y;
        v[5].z = tri[3].z;
        v[5].c = clr;
        v[5].tu = 1.0f;
        v[5].tv = 1.0f;
        g_pApp->m_renderer->UnlockVb(vb);
        g_pApp->m_renderer->SetToStream0(vb);
        g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_TRIANGLELIST, vofs, 2u);
    }

    void Application::SaveToXml(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode) const
    {
        // RVA 0x59F1F0
        ref_ptr cameraNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Camera");
        xmlNode->AddChild(cameraNode);
        m_curCamera.SaveToXml(xmlFile, cameraNode);

        ref_ptr cinematicNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Cinematic");
        xmlNode->AddChild(cinematicNode);
        m_cinematic->SaveToXml(xmlFile, cinematicNode);
    }

    Application::~Application()
    {
        // RVA 0x5AA9C0
        // Only the members' destructors run.
    }

    void Application::TexSoften(
        rend::TexHandle dstTex,
        rend::TexHandle srcTex,
        int numJitters,
        float jitterDist,
        unsigned clr)
    {
        // RVA 0x7AFB10
        int sx = 0;
        int sy = 0;
        g_pApp->m_renderer->GetDims(srcTex, sx, sy);
        if (!g_pApp->m_renderer->RenderToTexStart(dstTex, false))
        {
            M3D_LOG_INFO("TexSoften: cannot set rt to tex");
            return;
        }

        g_pApp->m_renderer->ClearViewport(rend::M3DCLEAR_C, 0xFFFFFFFF);
        g_pApp->m_renderer->SetTexture(0, srcTex, -1.0);
        g_pApp->m_renderer->PushBlend(rend::BM_ALPHA);
        if (numJitters > 1)
        {
            float const adder = static_cast<float>(sx) * jitterDist * 2.0f / static_cast<float>(numJitters);
            float const half = static_cast<float>(sx) * 0.5f;
            float s = 0.0f - jitterDist;
            for (int i = 0; i < numJitters; ++i)
            {
                float t = 0.0f - jitterDist;
                for (int j = 0; j < numJitters; ++j)
                {
                    PutSpriteAbs(half + s, half + t, half, clr);
                    t += adder;
                }
                s += adder;
            }
        }
        else
        {
            float const half = static_cast<float>(sx) * 0.5f;
            PutSpriteAbs(half, half, half + 0.5f, clr);
        }
        g_pApp->m_renderer->PopBlend();
        g_pApp->m_renderer->RenderToTexFinish();
    }

    bool Application::IsTextHieroglyphic(CStr const&) const
    {
        return false;
    }

    CStr Application::GetNativeFuncDesc(char const* funcName) const
    {
        // RVA 0x8BC2C0
        auto const& funcDescs = g_Kernel->GetScriptServer().getRegisteredFunctionsDesc();
        auto const it = funcDescs.find(CStr(funcName));
        if (it == funcDescs.end())
        {
            return CStr("");
        }

        auto const& desc = it->second;
        CStr descStr("");
        if (!desc.returnValue.empty())
        {
            descStr += CStr(desc.returnValue) + CStr(" ");
        }
        else
        {
            descStr += CStr("??? ");
        }
        descStr += it->first;
        if (!CStr(desc.params).empty())
        {
            descStr += CStr("( ") + CStr(desc.params.c_str()) + CStr(" )");
        }
        else
        {
            descStr += CStr("()");
        }
        if (!CStr(desc.shortDesc).empty())
        {
            descStr += CStr("   /* ") + CStr(desc.shortDesc.c_str()) + CStr(" */");
        }
        return descStr;
    }

    void Application::StopPlayingMusic()
    {
        if (g_Kernel->GetEngineCfg().m_mus_Enable.GetB())
        {
            g_pApp->m_sound->StopGroup(0);
        }
    }

    void Application::SetMouseSensitivity(float sensitivity)
    {
        // RVA 0x59F2E0
        m_mouseSensitivity = sensitivity;
        float const maxSensitivity = g_Kernel->GetEngineCfg().m_mouseMaxSensitivity.GetF();
        float const minSensitivity = g_Kernel->GetEngineCfg().m_mouseMinSensitivity.GetF();
        if (minSensitivity > m_mouseSensitivity)
        {
            m_mouseSensitivity = minSensitivity;
        }
        if (m_mouseSensitivity > maxSensitivity)
        {
            m_mouseSensitivity = maxSensitivity;
        }
    }

    float Application::getZoom()
    {
        // RVA 0x5AA910
        return 1.0f;
    }

    bool Application::LoadServers(CStr const& filename, bool bQuiet)
    {
        struct
        {
            m3d::DataServer* m_server;
            char const* m_name;
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
            for (auto const& server : servers)
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
        // RVA 0x59C650
        return false;
    }

    void Application::ChangeLanguage()
    {
        // RVA 0x59C5E0
        if (m_input)
        {
            m_input->ChangeLanguage();
        }
    }

    void Application::DrawWireRect(CVector2 const& from, CVector2 const& to, unsigned color)
    {
        // RVA 0x7B01E0
        auto vb = g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZWCT1);
        int vofs = 0;
        auto* v = static_cast<rend::VertexXYZWCT1*>(g_pApp->m_renderer->LockVbStreaming(vb, 8, vofs, 0));
        auto const set = [color](rend::VertexXYZWCT1& vert, float x, float y, float t)
        {
            vert.x = x;
            vert.y = y;
            vert.z = 0.5f;
            vert.w = 0.5f;
            vert.c = color;
            vert.tu = t;
            vert.tv = t;
        };
        set(v[0], from.x, from.y, 0.0f);
        set(v[1], to.x, from.y, 1.0f);
        set(v[2], to.x, from.y, 0.0f);
        set(v[3], to.x, to.y, 1.0f);
        set(v[4], to.x, to.y, 0.0f);
        set(v[5], from.x, to.y, 1.0f);
        set(v[6], from.x, to.y, 0.0f);
        set(v[7], from.x, from.y, 1.0f);
        g_pApp->m_renderer->UnlockVb(vb);
        g_pApp->m_renderer->SetToStream0(vb);
        g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_LINELIST, vofs, 4u);
    }

    void Application::PostLoadServers()
    {
        struct
        {
            m3d::DataServer* m_server;
            char const* m_name;
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
        // RVA 0x59C610
    }

    void Application::FinishQuads()
    {
        if (this->m_numPointsVerts)
            FlushQuads();
    }

    void Application::setZoom(float)
    {
        // RVA 0x5AA900
    }

    void Application::PutSpriteRelRot(
        float centerX,
        float centerY,
        float sizeX,
        float sizeY,
        unsigned int color,
        float vertexAngle,
        float vertexRotationCenterX,
        float vertexRotationCenterY,
        float imageAngle,
        float imageRotationCenterX,
        float imageRotationCenterY)
    {
        // TODO: generated code Application::PutSpriteRelRot
        // Initialize vertex positions (unrotated quad centered at origin)
        float x0 = -sizeX;
        float y0 = -sizeY;
        float x1 = sizeX;
        float y1 = -sizeY;
        float x2 = sizeX;
        float y2 = sizeY;
        float x3 = -sizeX;
        float y3 = sizeY;

        // Apply vertex rotation if needed
        if (vertexAngle != 0.0f)
        {
            float sinVert = std::sin(vertexAngle);
            float cosVert = std::cos(vertexAngle);

            // Rotate each vertex around the vertex rotation center
            auto rotatePoint = [&](float& x, float& y)
            {
                float dx = x - vertexRotationCenterX;
                float dy = y - vertexRotationCenterY;
                x = dx * cosVert - dy * sinVert + vertexRotationCenterX;
                y = dy * cosVert + dx * sinVert + vertexRotationCenterY;
            };

            rotatePoint(x0, y0);
            rotatePoint(x1, y1);
            rotatePoint(x2, y2);
            rotatePoint(x3, y3);
        }

        // Initialize texture coordinates (unrotated)
        float u0 = 0.0f, v0 = 0.0f;  // top-left
        float u1 = 1.0f, v1 = 0.0f;  // top-right
        float u2 = 1.0f, v2 = 1.0f;  // bottom-right
        float u3 = 0.0f, v3 = 1.0f;  // bottom-left

        // Convert all coordinates from relative to absolute
        m_renderer->RelToAbs(centerX, centerY);
        m_renderer->RelToAbs(sizeX, sizeY);  // Note: sizeX/sizeY not used after this
        m_renderer->RelToAbs(x0, y0);
        m_renderer->RelToAbs(x1, y1);
        m_renderer->RelToAbs(x2, y2);
        m_renderer->RelToAbs(x3, y3);
        m_renderer->RelToAbs(vertexRotationCenterX, vertexRotationCenterY);
        m_renderer->RelToAbs(imageRotationCenterX, imageRotationCenterY);

        // Apply texture rotation if needed
        if (imageAngle != 0.0f)
        {
            float sinImg = std::sin(imageAngle);
            float cosImg = std::cos(imageAngle);

            // Rotate each texture coordinate around the image rotation center
            auto rotateTexCoord = [&](float& u, float& v)
            {
                float du = u - imageRotationCenterX;
                float dv = v - imageRotationCenterY;
                u = du * cosImg - dv * sinImg + imageRotationCenterX;
                v = dv * cosImg + du * sinImg + imageRotationCenterY;
            };

            rotateTexCoord(u0, v0);
            rotateTexCoord(u1, v1);
            rotateTexCoord(u2, v2);
            rotateTexCoord(u3, v3);
        }

        // Call the absolute positioning function with translated vertices
        PutSprite2Abs(
            x0 + centerX,
            y0 + centerY,  // vertex 0
            x1 + centerX,
            y1 + centerY,  // vertex 1
            x3 + centerX,
            y3 + centerY,  // vertex 3
            x2 + centerX,
            y2 + centerY,  // vertex 2
            u0,
            v0,  // texcoord 0
            u1,
            v1,  // texcoord 1
            u3,
            v3,  // texcoord 3
            u2,
            v2,    // texcoord 2
            0.0f,  // unknown parameter (possibly z-coordinate)
            color  // color
        );
    }

    void Application::SetMouseXAxisFlipped(bool bFlip)
    {
        // RVA 0x59C5D0
        m_bMouseXAxisFlipped = bFlip;
    }

    void Application::SetMouseYAxisFlipped(bool bFlip)
    {
        // RVA 0x59C5B0
        m_bMouseYAxisFlipped = bFlip;
    }

    void Application::SetFlushQuadsShader(rend::IEffect* shader)
    {
        m_flushQuadsShader = shader;
    }

    int Application::AppActive() const
    {
        return m_isAppActive;
    }

    rend::VertexXYZCT1* Application::RenderQuadXyzct1GetNextPtr()
    {
        // TODO: check this
        if ((unsigned int)(this->m_numPointsVerts + 4) >= 0xFA0)
            FlushQuads();
        this->m_numPointsVerts = m_numPointsVerts + 4;
        return &this->m_pointsVertsCt1[m_numPointsVerts];
    }

    void Application::LoadFromXml(cmn::XmlFile* xmlFile, cmn::XmlNode const* xmlNode)
    {
        // RVA 0x59F0F0
        ref_ptr cameraNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetLastChild(cameraNode, "Camera");
        m_curCamera.LoadFromXml(xmlFile, cameraNode);

        ref_ptr cinematicNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(cinematicNode, "Cinematic");
        m_cinematic->LoadFromXml(xmlFile, cinematicNode);
    }

    int Application::SwitchDisplayModes(HWND__* wnd, int dwWidth, int dwHeight, bool bFullScreen)
    {
        // RVA 0x5A8150
        static int reenter = 0;

        int ret = 1;
        if (reenter >= 2)
        {
            return ret;
        }

        auto& cfg = g_Kernel->GetEngineCfg();
        if (cfg.m_r_width.GetI() == dwWidth && cfg.m_r_height.GetI() == dwHeight &&
            cfg.m_r_fullScreen.GetB() == bFullScreen)
        {
            return ret;
        }

        ++reenter;
        bool const saveFs = cfg.m_r_fullScreen.GetB();
        int const saveW = cfg.m_r_width.GetI();
        int const saveH = cfg.m_r_height.GetI();

        char buffer[16];
        sprintf_s(buffer, "%d", bFullScreen ? 1 : 0);
        cfg.m_r_fullScreen.Set(buffer, true);
        SetWindowLongA(wnd, GWL_STYLE, GetStyleForRenderWindow(bFullScreen));

        int width = dwWidth;
        int height = dwHeight;
        if (!bFullScreen)
        {
            DWORD const style = GetWindowLongA(wnd, GWL_STYLE);
            RECT rc;
            SetRect(&rc, 0, 0, dwWidth, dwHeight);
            AdjustWindowRect(&rc, style, FALSE);
            SetWindowPos(wnd, HWND_NOTOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
            RECT clientRect;
            GetClientRect(wnd, &clientRect);
            width = clientRect.right - clientRect.left;
            height = clientRect.bottom - clientRect.top;
        }

        sprintf_s(buffer, "%d", width);
        cfg.m_r_width.Set(buffer, true);
        sprintf_s(buffer, "%d", height);
        cfg.m_r_height.Set(buffer, true);

        if (!m_renderer->SwitchDisplayModes(wnd, width, height, bFullScreen))
        {
            M3D_LOG_INFO("trying to fallback to previous mode");
            if (!SwitchDisplayModes(m_renderWindow, saveW, saveH, saveFs))
            {
                M3D_LOG_ERR("ERROR! Application::SwitchDisplayMode() failed to recover");
                EnqueueMessage(1, 0, 0, 0, 0, CStr(), AIParam());
                ret = 0;
            }
        }

        --reenter;
        m_renderWindow = wnd;
        CaptureAndClipSystemCursor(cfg.m_clipCursorWithinRenderWnd.GetB());
        g_pApp->ImmediateMessage(4, 0, 0, 0, 0, CStr(), AIParam());
        return ret;
    }

    void Application::done()
    {
        // RVA 0x59FAF0
        g_Kernel->GetEngineCfg().m_levFileName.Set("Empty", false);
        g_Kernel->GetEngineCfg().Save(m_cfgName);
        DoneMedia();

        delete M3dVideoPlayer;
        M3dVideoPlayer = nullptr;

        // NOTE: ClearOnce is called before the null check on the server.
        ai::pServer->ClearOnce();
        delete ai::pServer;
        ai::pServer = nullptr;

        delete m_serverStaticModels;
        m_serverStaticModels = nullptr;
        delete m_serverAnimatedModels;
        m_serverAnimatedModels = nullptr;
        delete m_serverLights;
        m_serverLights = nullptr;
        delete m_serverSprites;
        m_serverSprites = nullptr;
        delete m_serverLines;
        m_serverLines = nullptr;
        delete m_serverParticles;
        m_serverParticles = nullptr;
        delete m_serverSound;
        m_serverSound = nullptr;
        delete m_serverMusic;
        m_serverMusic = nullptr;
        delete m_serverProjectors;
        m_serverProjectors = nullptr;
        delete m_serverDecals;
        m_serverDecals = nullptr;

        g_pApp->m_renderer->ReleaseIb(m_pointsVertsIb);
        doneProcTexThread();
        ui::WndStation::Done();
        doneSprite();

        if (m_sound)
        {
            m_sound->DecRef();
            m_sound = nullptr;
        }
        if (m_hSoundDll)
        {
            FreeLibrary(m_hSoundDll);
        }
        if (m_input)
        {
            m_input->DecRef();
            m_input = nullptr;
        }
        if (m_hInputDll)
        {
            FreeLibrary(m_hInputDll);
        }
        if (m_renderer)
        {
            m_renderer->DecRef();
            m_renderer = nullptr;
        }
        if (m_hRenderDll)
        {
            FreeLibrary(m_hRenderDll);
        }

        DoneImpulses();
        DeInitAllocCheck();

        // The camera controller and the sound console handler are released without running their destructors.
        ::operator delete(m_cameraController);
        m_cameraController = nullptr;
        delete m_cinematic;
        m_cinematic = nullptr;
        ::operator delete(m_soundConHandler);
        m_soundConHandler = nullptr;
        delete m_log;
        m_log = nullptr;
    }

    void Application::OnAfterDeviceReset()
    {
        // RVA 0x59C600
    }

    void Application::ReloadPostEffects()
    {
        // RVA 0x59C680
    }

    bool Application::GetPostEffectParam(CStr const&, float&)
    {
        // RVA 0x59C640
        return false;
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

        // RVA 0x686F10 (inlined) - words are joined with a space, except where that would
        // double one up: at the start of a line, or when either side is itself a space.
        CStr FormatTextSeparator(CStr const& line, CStr const& prevWord, CStr const& word)
        {
            if (!line.c_str() || !strlen(line.c_str()) || prevWord == " " || word == " ")
            {
                return CStr("");
            }
            return CStr(" ");
        }

        void GetWord(CStr const& src, int i, m3d::TextWrapFlags wrapFlags, CStr& word, int& nextWordPos)
        {
            // RVA 0x685000 - the length of the word starting at i: one character (or a whole '#'
            // colour or '@' code) when wrapping by characters, otherwise up to the next space or
            // unescaped '|'; -1 for the rest of the string. An unescaped '|' at i is a word of
            // length 0, so the caller steps over it and breaks the line.
            word = CStr("");
            nextWordPos = -1;
            int offset = 0;
            int const len = src.c_str() ? static_cast<int>(strlen(src.c_str())) : 0;
            if (wrapFlags == TW_CHAR_WRAP)
            {
                nextWordPos = 1;
                if (!IsEscSymbolBeforeSymbol(src, i))
                {
                    switch (src.c_str()[i])
                    {
                    case '#':
                        if (i + 1 < len)
                        {
                            nextWordPos = 2;
                        }
                        break;
                    case '@':
                        if (i + 9 <= len)
                        {
                            nextWordPos = 9;
                        }
                        break;
                    case '|':
                        nextWordPos = 0;
                        break;
                    default:
                        break;
                    }
                }
            }
            else
            {
                while (true)
                {
                    // findOneOf gives the offset from where the search started.
                    int const found = src.findOneOf(" |", offset + i);
                    if (found == -1)
                    {
                        nextWordPos = -1;
                        break;
                    }
                    int const pos = found + offset;
                    nextWordPos = pos;
                    if (pos == -1)
                    {
                        break;
                    }
                    char const c = src.c_str()[pos + i];
                    if (c != '|')
                    {
                        // A space at i is a word of its own.
                        if (c == ' ' && pos == 0)
                        {
                            nextWordPos = 1;
                        }
                        break;
                    }
                    if (!IsEscSymbolBeforeSymbol(src, pos + i))
                    {
                        break;
                    }
                    // An escaped '|' is part of the word.
                    if (pos + i + 1 >= len)
                    {
                        nextWordPos = -1;
                        break;
                    }
                    offset = pos + 1;
                    if (nextWordPos == -1)
                    {
                        break;
                    }
                }
            }
            word = src.substr(i, nextWordPos);
        }
    }  // namespace

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

        float availableWidth = maxX + 0.01f;  // Small epsilon

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
                else
                {
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

        for (auto const& formattedLine : linesOfText)
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

    bool Application::SaveServers(CStr const& filename)
    {
        // RVA 0x6A6490
        struct
        {
            DataServer* m_server;
            char const* m_name;
            CStr m_diz;
        } servers[9];

        // The localised descriptions are looked up but not used when saving.
        servers[0].m_diz = GetStringByStringId0("AnimatedModelsServer");
        servers[1].m_diz = GetStringByStringId0("StaticModelsServer");
        servers[2].m_diz = GetStringByStringId0("LightsServer");
        servers[3].m_diz = GetStringByStringId0("SpritesServer");
        servers[4].m_diz = GetStringByStringId0("ParticlesServer");
        servers[5].m_diz = GetStringByStringId0("SoundsServer");
        servers[6].m_diz = GetStringByStringId0("MusicServer");
        servers[7].m_diz = GetStringByStringId0("ProjectorsServer");
        servers[8].m_diz = GetStringByStringId0("DecalsServer");
        servers[0].m_server = m_serverAnimatedModels;
        servers[1].m_server = m_serverStaticModels;
        servers[2].m_server = m_serverLights;
        servers[3].m_server = m_serverSprites;
        servers[4].m_server = m_serverParticles;
        servers[5].m_server = m_serverSound;
        servers[6].m_server = m_serverMusic;
        servers[7].m_server = m_serverProjectors;
        servers[8].m_server = m_serverDecals;
        servers[0].m_name = "AnimatedModelsServer";
        servers[1].m_name = "StaticModelsServer";
        servers[2].m_name = "LightsServer";
        servers[3].m_name = "SpritesServer";
        servers[4].m_name = "ParticlesServer";
        servers[5].m_name = "SoundsServer";
        servers[6].m_name = "MusicServer";
        servers[7].m_name = "ProjectorsServer";
        servers[8].m_name = "DecalsServer";

        ref_ptr xmlFile = g_Kernel->CreateXmlFile();
        ref_ptr root = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Servers");
        for (auto const& server : servers)
        {
            ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, server.m_name);
            server.m_server->WriteToXmlNode(xmlFile, node);
            root->AddChild(node);
        }
        xmlFile->AddChild(root);

        scoped_ptr<fs::FileStream> stream(g_Kernel->GetFileServer().CreateFileStream());
        if (stream->Open(filename.c_str(), fs::IStream::OPEN_WRITE))
        {
            xmlFile->Write(*stream);
            stream->Close();
        }
        else
        {
            M3D_LOG_INFO("Save servers: could not save servers into file " + filename);
        }
        return true;
    }

    bool Application::GetMouseHitPoint(CVector&, SgNode*&)
    {
        // RVA 0x5AA970
        return false;
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

    int Application::GetTextExtent(
        CStr const& str,
        PointBase<float>& size,
        int fid,
        BoundsBase<float>* csz,
        int* minc,
        int* maxc,
        CStr* leftInvisibleSubstr,
        CStr* rightInvisibleSubstr)
    {
        // RVA 0x685610 - measures a string in the markup the UI uses, where '#' escapes the
        // next control character, '@' introduces an eight digit colour code, and '&' opens a
        // span that '|' closes. Only the characters that actually get drawn are counted.
        //
        // With a clip rect it also reports which character first falls outside it on each side,
        // so a caller can scroll a field without measuring it twice.
        if (leftInvisibleSubstr)
        {
            *leftInvisibleSubstr = CStr("");
        }
        if (rightInvisibleSubstr)
        {
            *rightInvisibleSubstr = CStr("");
        }

        if (str.empty())
        {
            size.x = 0.0f;
            size.y = 0.0f;
            return 0;
        }

        ui::Font* const font = fid == -1 ? ui::Wnd::m_gfx->GetCurFont() : ui::Wnd::m_gfx->GetFontById(fid);
        if (!font)
        {
            return 0;
        }

        // Glyph metrics are held at the font's unscaled size, so everything is measured in
        // those units and divided down once at the end.
        float const scale = font->m_heightScaled / font->m_heightUnscaled;

        char const* const text = str.c_str();
        int const len = text ? static_cast<int>(strlen(text)) : 0;

        float width = 0.0f;
        float maxGlyphHeight = 0.0f;
        int numChars = 0;

        bool escaped = false;
        bool inSpan = false;

        for (int i = 0; i < len; ++i)
        {
            unsigned char const c = text[i];
            if (!c)
            {
                break;
            }

            // A control character cannot be escaped and is never drawn.
            if (c < ' ')
            {
                escaped = false;
                continue;
            }

            if (!escaped)
            {
                if (c == '#')
                {
                    escaped = true;
                    continue;
                }
                if (c == '&')
                {
                    inSpan = true;
                    continue;
                }
                if (c == '|')
                {
                    // A bar outside a span is markup; the one that closes a span is drawn.
                    if (!inSpan)
                    {
                        continue;
                    }
                    inSpan = false;
                }
                else if (c == '@')
                {
                    // '@' plus eight digits of colour.
                    i += 8;
                    continue;
                }
                else if (c == '$')
                {
                    continue;
                }
            }
            escaped = false;

            // Past the right edge of the clip rect: report where it happened and stop.
            if (csz && maxc && width / scale > csz->width + csz->x0)
            {
                *maxc = i;
                if (rightInvisibleSubstr)
                {
                    // NOTE: only the one character is recorded before the loop stops, so this
                    // is the first character off the right edge rather than all of them.
                    *rightInvisibleSubstr += CStr(&text[i], 1);
                }
                break;
            }

            width += font->GetCharWidthAdvanced(c);

            float const glyphHeight = font->GetGlyphSz(c).y;
            if (glyphHeight > maxGlyphHeight)
            {
                maxGlyphHeight = glyphHeight;
            }

            // Still short of the left edge, so this character is scrolled out of view.
            if (csz && minc && csz->x0 > width / scale)
            {
                *minc = i;
                if (leftInvisibleSubstr)
                {
                    *leftInvisibleSubstr += CStr(&text[i], 1);
                }
            }

            ++numChars;
        }

        // A line of pure markup still has to report a height, so it borrows the one from 'A'.
        if (!numChars)
        {
            maxGlyphHeight = 0.0f;
            if (font->m_symbols.size() > 'A' && font->m_symbols['A'])
            {
                maxGlyphHeight = font->m_symbols['A']->m_precalcedGlyphSz.y;
            }
        }

        size.x = width / scale;
        size.y = maxGlyphHeight / scale;
        return numChars;
    }

    int Application::DrawTextRelT(float x, float y, unsigned dwColor, CStr const& strText, unsigned dwFlags, int fid)
    {
        // RVA 0x41D720
        m_renderer->RelToAbs(x, y);
        return DrawTextAbsT(x, y, dwColor, strText, dwFlags, fid);
    }

    int Application::FormatText(
        retruxx::vector<ui::FormattedLine>& linesOfText,
        PointBase<float> const& at,
        CStr const& textIn,
        ui::DrawInfo const& di,
        TextWrapFlags wrapFlags,
        TextFormatFlags formatFlags)
    {
        // RVA 0x686F10 - breaks the text into lines that fit the client rect, honouring the
        // explicit '|' breaks the strings carry and remembering the colour each line starts in.
        CStr src = textIn;
        CStr line;

        int wordsInLine = 0;
        float width = 0.0f;
        float height = 0.0f;

        // How much room a line has depends on which edge the text is anchored to.
        float available = 0.0f;
        switch (formatFlags)
        {
        case TF_CENTER:
            available = di.m_clientRect.width;
            break;

        case TF_LEFT:
        case TF_FULL:
            available = di.m_clientRect.width - (at.x - di.m_clientRect.x0);
            break;

        case TF_RIGHT:
            available = at.x - di.m_clientRect.x0;
            break;

        default:
            // NOTE: any other format leaves no room at all, so every word wraps onto its own
            // line rather than falling back to the full width.
            break;
        }

        PointBase<float> org = at;
        int prevPos = -1;
        int lineColor = -1;
        CStr prevWord;

        int const len = src.c_str() ? static_cast<int>(strlen(src.c_str())) : 0;
        int i = 0;

        if (len > 0)
        {
            // The epsilon keeps a word that measures exactly the full width from wrapping.
            float const maxWidth = available + 0.01f;

            do
            {
                int const wordStart = i;

                CStr word;
                int pos = -1;
                GetWord(src, i, wrapFlags, word, pos);

                // A '|' immediately before this word ends the previous line, unless it was
                // escaped and so meant literally.
                bool forceBreak = false;
                if (prevPos != -1)
                {
                    int const barPos = wordStart - 1;
                    if (barPos < len && barPos > 0 && src.c_str()[barPos] == '|')
                    {
                        forceBreak = !IsEscSymbolBeforeSymbol(src, barPos);
                    }
                }

                prevPos = pos;
                if (pos == -1)
                {
                    // Nothing left to read.
                    i = len;
                }
                else
                {
                    int const next = pos + i;
                    if (next < 0 || next >= len || src.c_str()[next] != '|' || IsEscSymbolBeforeSymbol(src, next))
                    {
                        // A zero-length word would leave the position where it was, so it is
                        // stepped over by hand to keep the loop moving.
                        i = pos ? next : i + 1;
                    }
                    else
                    {
                        // Step past the bar itself; the break is noticed on the next word.
                        i = next + 1;
                    }
                }

                // Character wrapping measures the character alone; word wrapping has to include
                // the space that joining the word to the line would add.
                PointBase<float> sz;
                if (wrapFlags == TW_CHAR_WRAP)
                {
                    GetTextExtent(word, sz, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
                }
                else
                {
                    CStr const separator = FormatTextSeparator(line, prevWord, word);
                    GetTextExtent(word + separator, sz, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
                }

                if (sz.x + width > maxWidth || forceBreak)
                {
                    int const hadWords = wordsInLine;

                    ui::FormattedLine formattedLine;
                    formattedLine.m_origin = org;
                    // With nothing on the line yet it is this word itself that overflows, so it
                    // goes out on its own rather than being held back.
                    formattedLine.m_text = hadWords ? line : word;
                    formattedLine.m_color = lineColor;

                    // Colour carries across the break into the line that follows.
                    int const lastColor = FindLastColorInStr(line);
                    if (lastColor)
                    {
                        lineColor = lastColor;
                    }

                    formattedLine.m_isHieroglyphic = false;
                    // A line broken by an explicit '|' is the end of its paragraph, so it is not
                    // stretched even in justified text.
                    formattedLine.m_format = (formatFlags == TF_FULL && forceBreak) ? TF_LEFT : formatFlags;
                    linesOfText.push_back(formattedLine);

                    if (height == 0.0f)
                    {
                        height = sz.y;
                    }
                    org.y += height;

                    if (hadWords)
                    {
                        line = word;
                        wordsInLine = 1;
                    }
                    else
                    {
                        line = CStr("");
                    }

                    GetTextExtent(line, sz, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
                    width = sz.x;
                    height = sz.y;
                }
                else
                {
                    if (wrapFlags == TW_CHAR_WRAP)
                    {
                        line += word;
                    }
                    else
                    {
                        line += FormatTextSeparator(line, prevWord, word) + word;
                    }
                    ++wordsInLine;

                    GetTextExtent(line, sz, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
                    width = sz.x;
                    if (sz.y > height)
                    {
                        height = sz.y;
                    }
                }

                prevWord = word;
            } while (i < len);
        }

        // Whatever is still in hand becomes the last line.
        if (line.c_str() && strlen(line.c_str()))
        {
            ui::FormattedLine formattedLine;
            formattedLine.m_origin = org;
            formattedLine.m_text = line;
            formattedLine.m_color = lineColor;
            formattedLine.m_isHieroglyphic = false;
            // The last line of justified text is never stretched.
            formattedLine.m_format = (formatFlags == TF_FULL) ? TF_LEFT : formatFlags;
            linesOfText.push_back(formattedLine);
        }

        return 1;
    }

    void Application::WaitForAnykey()
    {
        // RVA 0x59C470
        if (!m_waitForAnykey)
        {
            m_waitForAnykey = true;
            m_eventsQueueTail = 0;
            m_eventsQueueHead = 0;
            m_input->ClearBuffer();
        }
    }

    void Application::SetFrameClearColor(unsigned color)
    {
        this->m_frameClearColor = color;
    }

    int Application::DrawTextAbs(float x, float y, unsigned dwColor, CStr const& strText, unsigned dwFlags, int fid)
    {
        // RVA 0x59CA60
        CStr tStrText;
        tStrText = strText;
        return DrawTextAbsT(x, y, dwColor, tStrText, dwFlags, fid);
    }

    Application::Application(char const* logName)
    {
        if (g_Kernel->OpenLog(logName))
        {
            M3D_LOG_INFO("Starting up...");
            g_pApp = this;

            m_focusKbdEntity = this;
            m_strWindowTitle = "retruxx - release version build v0.01";

            char buf[0x400] = {0};
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

            g_Kernel->GetScriptServer().registerGlobalFunction(
                n_GetComputerName,
                "GetComputerName",
                "const char*",
                "",
                "returns network name for the current computer");

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
        // RVA 0x59E970
        panic();
        M3D_LOG_INFO(CStr((CStr("Assertion failed at ") + whence + CStr("\nexpression ") + assertion).c_str()));
        g_currentBinaryName = CStr("");
        _assert(assertion.c_str(), whence.c_str(), 0);
    }

    int Application::FinishExclusiveMsgLoop()
    {
        m_breakLoop = true;
        return 1;
    }

    void Application::UnPause()
    {
        // RVA 0x5AA930
    }

    Class* Application::GetBaseClass()
    {
        return RT_CLASS_LOCAL(WndStation);
    }

    void Application::PutSpriteAbsRot(
        float cx,
        float cy,
        float sx,
        float sy,
        unsigned c,
        float angleVerts,
        float avx,
        float avy,
        float angleImage,
        float aix,
        float aiy)
    {
        // RVA 0x6734F0
        float x0 = 0.0f - sx;
        float y0 = 0.0f - sy;
        float x1 = sx;
        float y1 = 0.0f - sy;
        float x2 = sx;
        float y2 = sy;
        float x3 = 0.0f - sx;
        float y3 = sy;
        if (angleVerts != 0.0f)
        {
            float const s = sin(angleVerts);
            float const co = cos(angleVerts);
            auto const rotate = [&](float& x, float& y)
            {
                float const dx = x - avx;
                float const dy = y - avy;
                x = dx * co - dy * s + avx;
                y = dy * co + dx * s + avy;
            };
            rotate(x0, y0);
            rotate(x1, y1);
            rotate(x2, y2);
            rotate(x3, y3);
        }

        float u0 = 0.0f;
        float v0 = 0.0f;
        float u1 = 1.0f;
        float v1 = 0.0f;
        float u2 = 1.0f;
        float v2 = 1.0f;
        float u3 = 0.0f;
        float v3 = 1.0f;
        if (angleImage != 0.0f)
        {
            float const s = sin(angleImage);
            float const co = cos(angleImage);
            auto const rotate = [&](float& u, float& v)
            {
                float const du = u - aix;
                float const dv = v - aiy;
                u = du * co - dv * s + aix;
                v = dv * co + du * s + aiy;
            };
            rotate(u0, v0);
            rotate(u1, v1);
            rotate(u2, v2);
            rotate(u3, v3);
        }

        // The quad is emitted as a strip: corner 0, 1, 3, 2.
        PutSprite2Abs(
            x0 + cx,
            y0 + cy,
            x1 + cx,
            y1 + cy,
            x3 + cx,
            y3 + cy,
            x2 + cx,
            y2 + cy,
            u0,
            v0,
            u1,
            v1,
            u3,
            v3,
            u2,
            v2,
            0.0f,
            c);
    }

    Profiler* Application::CreateProfiler(char const* name)
    {
        // RVA 0x698600
        unsigned const idx = m_profilerStack.AddProfiler(name, 30u);
        if (idx < m_profilerStack.m_numProfilers)
        {
            return m_profilerStack.m_stack[idx];
        }
        return nullptr;
    }

    void Application::PutSprite2Rel(
        float x1,
        float y1,
        float tu1,
        float tv1,
        float x2,
        float y2,
        float tu2,
        float tv2,
        unsigned c)
    {
        // RVA 0x41CFD0 - the corners arrive in relative coordinates; everything below this
        // works in absolute ones.
        m_renderer->RelToAbs(x1, y1);
        m_renderer->RelToAbs(x2, y2);
        PutSprite2Abs(x1, y1, tu1, tv1, x2, y2, tu2, tv2, c);
    }

    void Application::SetKeyboardFocus(IEventHandler* entity)
    {
        if (m_focusKbdEntity)
        {
            m3d::Event ev;
            ev.m_eventType = 5;
            ev.m_void[0] = entity;
            m_focusKbdEntity->HandleEvent(ev);
        }

        m_focusKbdEntity = entity;
        if (m_focusKbdEntity)
        {
            m3d::Event ev;
            ev.m_eventType = 6;
            ev.m_void[0] = entity;
            m_focusKbdEntity->HandleEvent(ev);
        }
    }

    int Application::OnChangeMode(AuxImpulseInfo const&)
    {
        // RVA 0x5AA980
        return 1;
    }

    void Application::DrawWireRectRel(CVector2 const& from, CVector2 const& to, unsigned color)
    {
        // RVA 0x7B0410
        CVector2 a = from;
        CVector2 b = to;
        g_pApp->m_renderer->RelToAbs(a.x, a.y);
        g_pApp->m_renderer->RelToAbs(b.x, b.y);
        DrawWireRect(a, b, color);
    }

    DataServer& Application::GetLightsServer()
    {
        return *m_serverLights;
    }

    int Application::SetFont(CStr const& fontName, float height, unsigned flags, unsigned cp)
    {
        // RVA 0x59C530
        ui::FontParams params;
        params.ttfParams.style = flags;
        params.ttfParams.codePage = cp;
        return ui::Wnd::GetGfxServer()->SetFont(fontName, height, ui::FONT_TYPE_WINDOWS, params);
    }

    bool Application::SaveUsedOnlyServers(
        CStr const& filename,
        retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& usedAnimatedHandles)
    {
        // RVA 0x6A8DC0
        struct
        {
            DataServer* m_server;
            char const* m_name;
            CStr m_diz;
        } servers[9];

        servers[0].m_diz = GetStringByStringId0("AnimatedModelsServer");
        servers[1].m_diz = GetStringByStringId0("StaticModelsServer");
        servers[2].m_diz = GetStringByStringId0("LightsServer");
        servers[3].m_diz = GetStringByStringId0("SpritesServer");
        servers[4].m_diz = GetStringByStringId0("ParticlesServer");
        servers[5].m_diz = GetStringByStringId0("SoundsServer");
        servers[6].m_diz = GetStringByStringId0("MusicServer");
        servers[7].m_diz = GetStringByStringId0("ProjectorsServer");
        servers[8].m_diz = GetStringByStringId0("DecalsServer");
        servers[0].m_server = m_serverAnimatedModels;
        servers[1].m_server = m_serverStaticModels;
        servers[2].m_server = m_serverLights;
        servers[3].m_server = m_serverSprites;
        servers[4].m_server = m_serverParticles;
        servers[5].m_server = m_serverSound;
        servers[6].m_server = m_serverMusic;
        servers[7].m_server = m_serverProjectors;
        servers[8].m_server = m_serverDecals;
        servers[0].m_name = "AnimatedModelsServer";
        servers[1].m_name = "StaticModelsServer";
        servers[2].m_name = "LightsServer";
        servers[3].m_name = "SpritesServer";
        servers[4].m_name = "ParticlesServer";
        servers[5].m_name = "SoundsServer";
        servers[6].m_name = "MusicServer";
        servers[7].m_name = "ProjectorsServer";
        servers[8].m_name = "DecalsServer";

        ref_ptr xmlFile = g_Kernel->CreateXmlFile();
        ref_ptr root = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Servers");

        // Server -> its index in the table above.
        std::unordered_map<DataServer*, int> serverIndices;
        for (int i = 0; i < 9; ++i)
        {
            serverIndices.emplace(servers[i].m_server, i);
        }

        // Collect the item handles referenced by the scene graph, per server.
        std::set<int> handles[16];
        std::vector<Object*> stack;
        stack.push_back(pClient->GetWorld().m_sceneGraph.GetRootNode());
        int index = 0;
        while (!stack.empty())
        {
            Object* parent = stack.back();
            stack.pop_back();
            for (Object* child = parent->GetFirstChild(); child; child = child->GetNextSibling())
            {
                if (DataServer* server = static_cast<SgNode*>(child)->GetServer())
                {
                    int handle = -1;
                    child->GetProperty(0x1108, &handle);
                    auto const it = serverIndices.find(server);
                    // NOTE: a server missing from the table reuses the previous index (the binary starts
                    // with garbage there and would index past the handle sets; such items are skipped here).
                    if (it != serverIndices.end())
                    {
                        index = it->second;
                    }
                    if (handle != -1 && index >= 0 && index < 16)
                    {
                        handles[index].insert(handle);
                    }
                }
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }
        }

        for (int const handle : usedAnimatedHandles)
        {
            handles[0].insert(handle);
        }

        CStr const staticModelsServer("StaticModelsServer");
        CStr const animatedModelsServer("AnimatedModelsServer");
        for (int i = 0; i < 9; ++i)
        {
            ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, servers[i].m_name);
            // NOTE: only the model servers get their items written; every other server is saved as an empty node.
            if (CStr(servers[i].m_name) == staticModelsServer || CStr(servers[i].m_name) == animatedModelsServer)
            {
                for (int const handle : handles[i])
                {
                    ref_ptr item = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Item");
                    servers[i].m_server->WriteItemToXmlNode(handle, xmlFile, item);
                    node->AddChild(item);
                }
            }
            root->AddChild(node);
        }
        xmlFile->AddChild(root);

        scoped_ptr<fs::FileStream> stream(g_Kernel->GetFileServer().CreateFileStream());
        if (stream->Open(filename.c_str(), fs::IStream::OPEN_WRITE))
        {
            xmlFile->Write(*stream);
            stream->Close();
        }
        else
        {
            M3D_LOG_INFO("Save servers: could not save servers into file " + filename);
        }
        return true;
    }

    int Application::Render3DText0(CVector const& at, CStr const& str, unsigned dwFlags, float scale)
    {
        // RVA 0x670490
        // "^0".."^7" select one of these colours for the rest of the text.
        static unsigned const colors[8] = {
            0xFF000000, 0xFF0000FF, 0xFF00FF00, 0xFF00FFFF, 0xFFFF0000, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF};

        ui::Font* const fnt = ui::Wnd::GetGfxServer()->GetCurFont();
        if (!fnt)
        {
            return 0;
        }
        if (str.empty())
        {
            return 0;
        }

        g_pApp->m_renderer->SetToStream0(g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZCT1));
        enterFontRender();
        auto const* text = reinterpret_cast<unsigned char const*>(str.c_str());
        int ntris = 0;
        CVector aax;
        CVector aay;
        CVector az;
        g_pApp->m_renderer->MatGetBasis(aax, aay, az);
        bool const noCull = (dwFlags & 2) != 0;
        if (noCull)
        {
            g_pApp->m_renderer->PushCull(rend::M3DCULL_NONE);
        }

        float x = 0.0f;
        float y = 0.0f;
        if (dwFlags & 1)
        {
            // Centred: measured without the colour codes.
            // NOTE: after removing a "^x" pair the next character is skipped, so "^1^2" keeps "^2".
            CStr o(reinterpret_cast<char const*>(text));
            for (int i = 0; i < o.length(); ++i)
            {
                if (o.c_str()[i] == '^')
                {
                    o.del(i, 2);
                }
            }
            PointBase<float> sz;
            GetTextExtent(o, sz, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
            x = sz.x * scale * -0.5f;
            y = sz.y * scale * -0.5f;
        }

        unsigned color = 0xFFFFFFFF;
        float const startX = x;
        while (unsigned char const c = *text)
        {
            ++text;
            if (c == '\n')
            {
                x = startX;
                float c1 = 0.0f;
                float c3 = 0.0f;
                if (fnt->m_symbols[0])
                {
                    c1 = fnt->m_symbols[0]->m_tcs.m_coordinates[1];
                    c3 = fnt->m_symbols[0]->m_tcs.m_coordinates[3];
                }
                int texSzX = 0;
                int texSzY = 0;
                if (!fnt->m_textures.empty())
                {
                    g_pApp->m_renderer->GetDims(fnt->m_textures[0], texSzX, texSzY);
                }
                y -= (c3 - c1) * static_cast<float>(texSzY) * scale;
                continue;
            }
            if (c < ' ')
            {
                continue;
            }
            if (c == '^')
            {
                unsigned char code = *text;
                if (code)
                {
                    ++text;
                    if (code < '0')
                    {
                        code = '0';
                    }
                    else if (code > '7')
                    {
                        code = '7';
                    }
                    color = colors[code - '0'];
                }
                continue;
            }

            auto const tcs = fnt->GetTexCoord(c);
            auto const texSz = fnt->GetTexSz();
            CVector const right(aax.x * 2.0f, aax.y * 2.0f, aax.z * 2.0f);
            CVector const up(aay.x * 2.0f, aay.y * 2.0f, aay.z * 2.0f);
            float const w = (tcs.m_coordinates[2] - tcs.m_coordinates[0]) * static_cast<float>(texSz.x) * scale;
            float const h = (tcs.m_coordinates[3] - tcs.m_coordinates[1]) * static_cast<float>(texSz.y) * scale;
            float const abcA = fnt->m_symbols[c] ? fnt->m_symbols[c]->m_abc.m_A : 0.0f;
            float const ofsX = (0.0f - w - abcA) * 0.1f;
            float const ofsY = h * -0.1f;

            int vofs = 0;
            auto* v = static_cast<rend::VertexXYZCT1*>(g_pApp->m_renderer->LockVbStreaming(
                g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZCT1), 12, vofs, nullptr));

            auto const set =
                [&](rend::VertexXYZCT1& vert, float px, float py, bool shadow, unsigned vcolor, float tu, float tv)
            {
                float vx = at.x + right.x * px + up.x * py;
                float vy = at.y + right.y * px + up.y * py;
                float vz = at.z + right.z * px + up.z * py;
                if (shadow)
                {
                    vx += az.x;
                    vy += az.y;
                    vz += az.z;
                }
                vert.x = vx;
                vert.y = vy;
                vert.z = vz;
                vert.c = vcolor;
                vert.tu = tu;
                vert.tv = tv;
            };

            float const u0 = tcs.m_coordinates[0];
            float const v0 = tcs.m_coordinates[1];
            float const u1 = tcs.m_coordinates[2];
            float const v1 = tcs.m_coordinates[3];

            // Shadow, pushed along the third basis vector and offset by a tenth of the glyph size.
            float const sx0 = ofsX + x;
            float const sy0 = ofsY + y;
            float const sx1 = ofsX + w + x;
            float const sy1 = ofsY + h + y;
            set(v[0], sx0, sy0, true, 0xFF000000, u0, v1);
            set(v[1], sx0, sy1, true, 0xFF000000, u0, v0);
            set(v[2], sx1, sy0, true, 0xFF000000, u1, v1);
            set(v[3], sx1, sy1, true, 0xFF000000, u1, v0);
            set(v[4], sx1, sy0, true, 0xFF000000, u1, v1);
            set(v[5], sx0, sy1, true, 0xFF000000, u0, v0);

            set(v[6], x, y, false, color, u0, v1);
            set(v[7], x, h + y, false, color, u0, v0);
            set(v[8], w + x, y, false, color, u1, v1);
            set(v[9], w + x, h + y, false, color, u1, v0);
            set(v[10], w + x, y, false, color, u1, v1);
            set(v[11], x, h + y, false, color, u0, v0);

            g_pApp->m_renderer->UnlockVb(g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZCT1));

            std::vector<rend::TexHandle> const fontTextures = fnt->m_textures;
            if (tcs.m_texId >= 0 && tcs.m_texId < static_cast<int>(fontTextures.size()))
            {
                auto const& tex = fontTextures[tcs.m_texId];
                if (tex != ui::Wnd::GetGfxServer()->m_curFontTexture)
                {
                    g_pApp->m_renderer->SetTexture(0, tex, -1.0);
                    ui::Wnd::GetGfxServer()->m_curFontTexture = tex;
                }
            }

            g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_TRIANGLELIST, vofs, 4);
            ntris += 4;
            // NOTE: the pen advances by the unscaled glyph width.
            x += fnt->m_symbols[c] ? fnt->m_symbols[c]->m_precalcedABCWidth : 0.0f;
        }

        finishFontRender();
        if (noCull)
        {
            g_pApp->m_renderer->PopCull();
        }
        return ntris;
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
        // RVA 0x59C660
        return false;
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
                if (bEsc)
                {
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
                if (v8 <= v4)
                {
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
        // RVA 0x59C410
    }

    void Application::PutSplashCallBackQuiet(int proc, void*)
    {
        // RVA 0x6A6E00
        g_pApp->m_renderer->BeginScene();
        g_pApp->m_renderer->ClearViewport(rend::M3DCLEAR_C, 0);
        g_pApp->m_renderer->PushZbState(rend::ZB_DISABLE);
        // The first call's time is remembered but never used.
        static unsigned const startTime = g_Kernel->GetTimer().GetCurTime();
        (void)startTime;
        g_Kernel->GetTimer().GetCurTime();
        g_pApp->m_renderer->SetBlend(rend::BM_NONE, false);
        g_pApp->m_renderer->SetAlphaTest(0);
        g_pApp->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
        g_pApp->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
        g_pApp->m_renderer->SetWhiteTexture(0);
        float x1 = 3.0f;
        float y1 = 750.0f;
        float x2 = static_cast<float>(static_cast<double>(proc) * 10.21 + 3.0);
        float y2 = 765.0f;
        g_pApp->m_renderer->RelToAbs(x1, y1);
        g_pApp->m_renderer->RelToAbs(x2, y2);
        g_pApp->PutSprite2Abs(x1, y1, 0.0f, 0.0f, x2, y2, 1.0f, 1.0f, 0xFFAAAAAA);
        g_pApp->m_renderer->PopZbState();
        g_pApp->m_renderer->EndScene();
        g_pApp->m_renderer->PresentScene();
    }

    void Application::RenderQuadXyzwct1(float x, float y, float sz, unsigned clr)
    {
        // RVA 0x7AFDC0
        if (static_cast<unsigned>(m_numPointsVerts + 4) >= 4000)
        {
            FlushQuads();
        }

        auto* v = &m_pointsVertsWct1[m_numPointsVerts];
        m_numPointsVerts += 4;
        auto const set = [clr](rend::VertexXYZWCT1& vert, float vx, float vy, float tu, float tv)
        {
            vert.x = vx;
            vert.y = vy;
            vert.z = 0.0f;
            vert.w = 0.5f;
            vert.c = clr;
            vert.tu = tu;
            vert.tv = tv;
        };
        set(v[0], x - sz, y - sz, 0.0f, 0.0f);
        set(v[1], x + sz, y - sz, 1.0f, 0.0f);
        set(v[2], x + sz, y + sz, 1.0f, 1.0f);
        set(v[3], x - sz, y + sz, 0.0f, 1.0f);
    }

    void Application::DrawBoundingRadius(CVector const& org, float r, unsigned clr)
    {
        // RVA 0x7B05A0
        DrawCross(org, r, clr);
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
        // RVA 0x5AA940
        return -1;
    }

    int Application::HeightmapToNormalmap(unsigned short* src, unsigned* dst, int sx, int sy)
    {
        // RVA 0x7AFF10
        // NOTE: each row converts only sx - 2 texels but skips a single source texel, so rows after the
        // first drift by one texel per row and the last rows read past the height map; kept as shipped.
        for (int row = sy; row > 0; --row)
        {
            for (int col = sx - 2; col > 0; --col)
            {
                int const h = src[0];
                float const dx = static_cast<float>(src[1] - h);
                float const dy = static_cast<float>(src[sx] - h);
                float const len = static_cast<float>(
                    1.0 / sqrt(static_cast<double>(dy) * dy + static_cast<double>(dx) * dx + 1.0000001f));
                CVector normal;
                normal.x = len * dx;
                normal.y = len * dy;
                normal.z = len;
                *dst = VecToRgba(normal, static_cast<float>(h));
                ++src;
                ++dst;
            }
            ++src;
        }
        return 1;
    }

    int Application::SetFontSelfMaking(CStr const& fontName, float height)
    {
        // RVA 0x59C560
        // NOTE: the code page part of the parameters is an uninitialised stack slot in the binary.
        ui::FontParams params;
        params.ttfParams.style = 1;
        params.ttfParams.codePage = 0;
        return ui::Wnd::GetGfxServer()->SetFont(fontName, height, ui::FONT_TYPE_SELFMAKING, params);
    }

    void Application::PutSplash(int proc, char const* text)
    {
        // RVA 0x5A0B10
        BoundsBase<float> dizRect;
        dizRect.x0 = 125.0f;
        dizRect.y0 = 490.0f;
        dizRect.width = 450.0f;
        dizRect.height = 110.0f;

        g_pApp->m_renderer->BeginScene();
        g_pApp->m_renderer->ClearViewport(rend::M3DCLEAR_C, 0);
        g_pApp->m_renderer->PushZbState(rend::ZB_DISABLE);
        g_pApp->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
        g_pApp->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
        if (m_texSplash.IsValid())
        {
            g_pApp->m_renderer->SetTexture(0, m_texSplash, -1.0);
            g_pApp->PutSpriteRel(0.0f, 0.0f, 1024.0f, 768.0f, 0xFFFFFFFF);
        }

        g_pApp->m_renderer->SetTexture(
            0, m_loadScreenInfo.m_picture.IsValid() ? m_loadScreenInfo.m_picture : m_defLoadPicture, -1.0);
        float x1 = 110.0f;
        float y1 = 87.0f;
        float x2 = 909.0f;
        float y2 = 624.0f;
        g_pApp->m_renderer->RelToAbs(x1, y1);
        g_pApp->m_renderer->RelToAbs(x2, y2);
        g_pApp->PutSpriteAbs(x1, y1, x2, y2, 0xFFFFFFFF);

        g_pApp->m_renderer->PushBlend(rend::BM_ALPHA);
        g_pApp->m_renderer->SetAlphaTest(g_Kernel->GetEngineCfg().m_alphaTestInterface.GetI());
        g_pApp->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
        g_pApp->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
        g_pApp->m_renderer->SetTexture(0, m_texProgr, -1.0);
        g_pApp->PutSprite2Rel(
            292.0f,
            686.0f,
            0.0f,
            0.0f,
            static_cast<float>(proc) * 4.68f + 292.0f,
            717.0f,
            static_cast<float>(proc) * 0.01f,
            1.0f,
            0xFFFFFFFF);
        g_pApp->m_renderer->SetAlphaTest(0);

        if (m_texSplash1.IsValid())
        {
            g_pApp->m_renderer->SetTexture(0, m_texSplash1, -1.0);
            g_pApp->PutSpriteRel(74.0f, 48.0f, 367.0f, 341.0f, 0xFFFFFFFF);
        }

        // NOTE: the code page of the font parameters is an uninitialised stack slot in the binary.
        ui::FontParams fontParams;
        fontParams.ttfParams.style = 1;
        fontParams.ttfParams.codePage = 0;

        if (text)
        {
            ui::Wnd::GetGfxServer()->SetFont(CStr("SM_TimesNormal"), 15.0f, ui::FONT_TYPE_SELFMAKING, fontParams);
            PointBase<float> size;
            GetTextExtent(CStr(text), size, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
            float x = 526.0f - size.x * 0.5f;
            float y = 701.5f - size.y * 0.5f;
            m_renderer->RelToAbs(x, y);
            DrawTextAbsT(x, y, 0xFFFFFFFF, CStr(text), 0, -1);
        }

        CStr name;
        if (!m_loadScreenInfo.m_diz0.empty())
        {
            name = m_loadScreenInfo.m_diz0;
        }
        else
        {
            name = CStr(g_Kernel->GetEngineCfg().m_levFileName.GetS());
        }
        ui::Wnd::GetGfxServer()->SetFont(CStr("SM_TimesNormal"), 17.0f, ui::FONT_TYPE_SELFMAKING, fontParams);
        {
            // The measured size is not used.
            PointBase<float> size;
            GetTextExtent(name, size, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
            float x = 125.0f;
            float y = 410.0f;
            m_renderer->RelToAbs(x, y);
            DrawTextAbsT(x, y, 0xFFF5E6BD, name, 0, -1);
        }

        if (!m_loadScreenInfo.m_diz1.empty())
        {
            ui::Wnd::GetGfxServer()->SetFont(CStr("SM_TimesNormal"), 20.0f, ui::FONT_TYPE_SELFMAKING, fontParams);
            PointBase<float> size;
            GetTextExtent(m_loadScreenInfo.m_diz1, size, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
            float x = 125.0f;
            float y = 440.0f;
            m_renderer->RelToAbs(x, y);
            DrawTextAbsT(x, y, 0xFFF5E6BD, m_loadScreenInfo.m_diz1, 0, -1);
        }

        if (!m_loadScreenInfo.m_diz2.empty())
        {
            // NOTE: only the client rectangles of the draw info are filled in; the destination window,
            // original and clipped rectangles are uninitialised stack memory in the binary (zeroed here).
            ui::DrawInfo di{};
            di.m_clientRect = dizRect;
            di.m_clientClippedRect = dizRect;

            char colorBuf[16];
            sprintf_s(colorBuf, "%x", 0xFFF5E6BD);
            CStr const colorCode = CStr("@") + CStr(colorBuf);
            CStr const coloredText = colorCode + m_loadScreenInfo.m_diz2;
            PointBase<float> at(0.0f, 0.0f);
            ui::Wnd::GetGfxServer()->AddText(di, at, coloredText, 1, TW_WORD_WRAP, TF_LEFT);
        }

        g_pApp->m_renderer->PopBlend();
        g_pApp->m_renderer->PopZbState();
        g_pApp->m_renderer->EndScene();
        g_pApp->m_renderer->PresentScene();
    }

    int Application::DrawTextRel(float x, float y, unsigned dwColor, CStr const& strText, unsigned dwFlags, int fid)
    {
        // RVA 0x41D680
        CStr tStrText;
        tStrText = strText;
        m_renderer->RelToAbs(x, y);
        return DrawTextAbsT(x, y, dwColor, tStrText, dwFlags, fid);
    }

    HWND Application::GetRenderWindow() const
    {
        // RVA 0x59C690
        return m_renderWindow;
    }

    void Application::PutSprite2Abs(
        float x1,
        float y1,
        float x2,
        float y2,
        float x3,
        float y3,
        float x4,
        float y4,
        float tu1,
        float tv1,
        float tu2,
        float tv2,
        float tu3,
        float tv3,
        float tu4,
        float tv4,
        float zval,
        unsigned c)
    {
        auto const handle = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZWCT1);
        int vofs = 0;
        auto* vertex = static_cast<m3d::rend::VertexXYZWCT1*>(
            M3D_RENDERER->LockVbStreaming(handle, rend::VERTEX_XYZWCT1, vofs, nullptr));

        vertex[0].x = x1;
        vertex[0].y = y1;
        vertex[0].z = zval;
        vertex[0].w = 0.1f;
        vertex[0].c = c;
        vertex[0].tu = tu1;
        vertex[0].tv = tv1;

        vertex[1].x = x2;
        vertex[1].y = y2;
        vertex[1].z = zval;
        vertex[1].w = 0.1f;
        vertex[1].c = c;
        vertex[1].tu = tu2;
        vertex[1].tv = tv2;

        vertex[2].x = x3;
        vertex[2].y = y3;
        vertex[2].z = zval;
        vertex[2].w = 0.1f;
        vertex[2].c = c;
        vertex[2].tu = tu3;
        vertex[2].tv = tv3;

        vertex[3].x = x4;
        vertex[3].y = y4;
        vertex[3].z = zval;
        vertex[3].w = 0.1f;
        vertex[3].c = c;
        vertex[3].tu = tu4;
        vertex[3].tv = tv4;

        M3D_RENDERER->UnlockVb(handle);
        M3D_RENDERER->SetToStream0(handle);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLESTRIP, vofs, 2u);
    }

    void Application::PutSprite2Abs(
        float x1,
        float y1,
        float tu1,
        float tv1,
        float x2,
        float y2,
        float tu2,
        float tv2,
        unsigned int c)
    {
        PutSprite2Abs(x1, y2, x1, y1, x2, y2, x2, y1, tu1, tv2, tu1, tv1, tu2, tv2, tu2, tv1, 0.0, c);
    }

    char const* Application::GetCallbackName() const
    {
        // RVA 0x59C620
        return nullptr;
    }

    ui::MbRetCodes Application::RunMsgBoxDlg(CStr const& caption, CStr const& message, unsigned flags, bool bPause)
    {
        // RVA 0x59C770
        return ui::RunMsgBoxDlg(caption, message, flags, bPause);
    }

    int Application::DrawTextAbsT(float sx, float sy, unsigned dwColor, CStr const& str, unsigned dwFlags, int fid)
    {
        // RVA 0x66FC10
        ui::Font* font = nullptr;
        if (IsTextHieroglyphic(str) && g_Kernel->GetEngineCfg().m_ui_forceHieroglyphicFont.GetB())
        {
            ui::Wnd::GetGfxServer()->SetFont(ui::Wnd::GetGfxServer()->m_hieroglyphicFontId);
            font = ui::Wnd::GetGfxServer()->GetCurFont();
            if (!font)
            {
                return 0;
            }
        }
        else if (fid != -1)
        {
            if (!ui::Wnd::GetGfxServer()->SetFont(fid))
            {
                return 0;
            }
            font = ui::Wnd::GetGfxServer()->GetFontById(fid);
            if (!font)
            {
                return 0;
            }
        }
        else
        {
            font = ui::Wnd::GetGfxServer()->GetCurFont();
            if (!font)
            {
                return 0;
            }
        }

        int ntris = 0;
        if (str.empty())
        {
            return 0;
        }

        if (dwFlags & 1)
        {
            // Centred on the given point.
            CStr strPure(str);
            PointBase<float> size;
            GetTextExtent(strPure, size, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
            sx -= size.x * 0.5f;
            sy -= size.y * 0.5f;
        }

        auto const* text = reinterpret_cast<unsigned char const*>(str.c_str());
        enterFontRender();
        g_pApp->m_renderer->SetToStream0(g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZWCT1));

        float const startX = sx;
        for (; *text; ++text)
        {
            unsigned char const c = *text;
            if (c == '\n')
            {
                auto const tcsDef = font->GetTexCoord(0);
                sx = startX;
                // NOTE: a new line moves the pen up by the glyph height measured in texture pixels.
                sy -= (tcsDef.m_coordinates[3] - tcsDef.m_coordinates[1]) * static_cast<float>(font->GetTexSz().x);
                continue;
            }
            if (c < ' ')
            {
                continue;
            }

            auto const tcs = font->GetTexCoord(c);
            auto const* sym = font->m_symbols[c];
            PointBase<float> glyphSz(0.0f, 0.0f);
            float abcA = 0.0f;
            if (sym)
            {
                glyphSz = sym->m_precalcedGlyphSz;
                abcA = sym->m_abc.m_A;
            }

            // The drop shadow is offset down by a tenth of the glyph height.
            float const ofsY = glyphSz.y * 0.1f;
            sx = static_cast<float>(lrintf(sx));
            sy = static_cast<float>(lrintf(sy));

            int vofs = 0;
            auto* v = static_cast<rend::VertexXYZWCT1*>(g_pApp->m_renderer->LockVbStreaming(
                g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZWCT1), 12, vofs, nullptr));

            float const left = sx - 0.5f;
            float const right = sx + glyphSz.x - 0.5f;
            float const top = sy - 0.5f;
            float const bottom = glyphSz.y + sy - 0.5f;
            float const u0 = tcs.m_coordinates[0];
            float const v0 = tcs.m_coordinates[1];
            float const u1 = tcs.m_coordinates[2];
            float const v1 = tcs.m_coordinates[3];

            auto const set =
                [](rend::VertexXYZWCT1& vert, float x, float y, float z, float w, unsigned color, float tu, float tv)
            {
                vert.x = x;
                vert.y = y;
                vert.z = z;
                vert.w = w;
                vert.c = color;
                vert.tu = tu;
                vert.tv = tv;
            };

            // Shadow, shifted right by the glyph's A width and down by ofsY.
            // NOTE: the shadow uses z = 0.9, w = 1.0 and the glyph z = 0.8, w = 0.9, as shipped.
            unsigned const black = 0xFF000000;
            set(v[0], left + abcA, bottom + ofsY, 0.9f, 1.0f, black, u0, v1);
            set(v[1], left + abcA, top + ofsY, 0.9f, 1.0f, black, u0, v0);
            set(v[2], right + abcA, bottom + ofsY, 0.9f, 1.0f, black, u1, v1);
            set(v[3], right + abcA, top + ofsY, 0.9f, 1.0f, black, u1, v0);
            set(v[4], right + abcA, bottom + ofsY, 0.9f, 1.0f, black, u1, v1);
            set(v[5], left + abcA, top + ofsY, 0.9f, 1.0f, black, u0, v0);

            set(v[6], left, bottom, 0.8f, 0.9f, dwColor, u0, v1);
            set(v[7], left, top, 0.8f, 0.9f, dwColor, u0, v0);
            set(v[8], right, bottom, 0.8f, 0.9f, dwColor, u1, v1);
            set(v[9], right, top, 0.8f, 0.9f, dwColor, u1, v0);
            set(v[10], right, bottom, 0.8f, 0.9f, dwColor, u1, v1);
            set(v[11], left, top, 0.8f, 0.9f, dwColor, u0, v0);

            g_pApp->m_renderer->UnlockVb(g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZWCT1));

            std::vector<rend::TexHandle> const fontTextures = font->m_textures;
            if (tcs.m_texId >= 0 && tcs.m_texId < static_cast<int>(fontTextures.size()))
            {
                auto const& tex = fontTextures[tcs.m_texId];
                if (tex != ui::Wnd::GetGfxServer()->m_curFontTexture)
                {
                    g_pApp->m_renderer->SetTexture(0, tex, -1.0);
                    ui::Wnd::GetGfxServer()->m_curFontTexture = tex;
                }
            }

            g_pApp->m_renderer->DrawPrimitive(rend::M3DPT_TRIANGLELIST, vofs, 4);
            ntris += 4;
            sx += font->m_symbols[c] ? font->m_symbols[c]->m_precalcedABCWidth : 0.0f;
        }

        finishFontRender();
        return ntris;
    }

    int Application::SaveExportDescToFile(fs::FileStream& outputFile, bool skipEmpty) const
    {
        // RVA 0x8BC720
        outputFile << "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n";
        outputFile << "-==\n";
        outputFile << "-== Log category  : Generic export description\n";
        outputFile << "-== Type\t\t  : ";
        if (skipEmpty)
        {
            outputFile << "Skipping those classes w/ empty export map\n";
        }
        else
        {
            outputFile << "All\n";
        }
        outputFile << "-== Build         : " << "retruxx - release version build v0.01" << "\n";
        outputFile << "-==\n";
        outputFile << "-== Note, this is a run-time info, so some functions and classes might\n";
        outputFile << "-== be unavailable if, for instance, server is not loaded yet...\n";
        outputFile << "-==\n";
        outputFile << "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n\n";

        Class** classes = nullptr;
        unsigned numClasses = 0;
        g_Kernel->GetListOfClasses(classes, numClasses);
        outputFile << "** Classes **\n\n";
        for (unsigned i = 0; i < numClasses; ++i)
        {
            Class* cls = classes[i];
            bool print = true;
            if (skipEmpty && CStr(cls->m_lExports->name).empty())
            {
                print = false;
            }
            if (print)
            {
                outputFile << GetClassDesc(cls).c_str();
                outputFile << "\n\n";
            }
        }
        delete[] classes;

        outputFile << "\n\n\n";
        outputFile << "** Global native functions **\n\n";
        for (auto const& [name, desc] : g_Kernel->GetScriptServer().getRegisteredFunctionsDesc())
        {
            outputFile << GetNativeFuncDesc(name.c_str()).c_str();
            outputFile << "\n\n";
        }
        return 1;
    }

    void Application::DrawLine(CVector2 const& from, CVector2 const& to, unsigned color)
    {
        // RVA 0x7B00E0
        auto vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZWCT1);
        int vofs = 0;
        auto* v = static_cast<rend::VertexXYZWCT1*>(M3D_RENDERER->LockVbStreaming(vb, 2, vofs, nullptr));
        v[0].x = from.x;
        v[0].y = from.y;
        v[0].z = 0.5f;
        v[0].w = 0.5f;
        v[0].c = color;
        v[0].tu = 0.0f;
        v[0].tv = 0.0f;
        v[1].x = to.x;
        v[1].y = to.y;
        v[1].z = 0.5f;
        v[1].w = 0.5f;
        v[1].c = color;
        v[1].tu = 1.0f;
        v[1].tv = 1.0f;
        M3D_RENDERER->UnlockVb(vb);
        M3D_RENDERER->SetToStream0(vb);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_LINELIST, vofs, 1u);
    }

    void Application::DrawLine(CVector const& from, CVector const& to, unsigned color)
    {
        auto vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZC);

        int vofs = 0;
        auto* stream = (m3d::rend::VertexXYZC*)M3D_RENDERER->LockVbStreaming(vb, 2, vofs, nullptr);
        stream[0].x = from.x;
        stream[0].y = from.y;
        stream[0].z = from.z;
        stream[0].c = color;

        stream[1].x = to.x;
        stream[1].y = to.y;
        stream[1].z = to.z;
        stream[1].c = color;

        M3D_RENDERER->UnlockVb(vb);
        M3D_RENDERER->SetToStream0(vb);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_LINELIST, vofs, 1u);
    }

    void Application::Pause()
    {
        // RVA 0x5AA920
    }

    CStr Application::GetClassDesc(Class* cls) const
    {
        // RVA 0x8BB710
        if (!cls)
        {
            return CStr("");
        }

        CStr desc("");
        Class* const baseClass = cls->m_fnGetBaseClass();
        ExportInfo* exportInfo = cls->m_lExports;
        desc = CStr("Class ") + CStr(cls->m_className);
        if (baseClass)
        {
            desc += CStr(" : public ") + CStr(baseClass->m_className);
        }
        desc += CStr("\n");
        desc += CStr("{\n");
        if (!exportInfo->name)
        {
            desc += CStr("\t[Empty export table so far]\n");
        }
        for (; exportInfo->name; ++exportInfo)
        {
            desc += CStr("\t");
            if (exportInfo->type == METHOD)
            {
                desc += CStr("[M] ");
            }
            if (exportInfo->type == NATIVE_METHOD)
            {
                desc += CStr("[N] ");
            }
            if (!CStr(exportInfo->returns).empty())
            {
                desc += CStr(exportInfo->returns) + CStr(" ");
            }
            else
            {
                desc += CStr("??? ");
            }
            desc += CStr(exportInfo->name);
            if (!CStr(exportInfo->params).empty())
            {
                desc += CStr("( ") + CStr(exportInfo->params) + CStr(" )");
            }
            else
            {
                desc += CStr("()");
            }
            if (!CStr(exportInfo->desc).empty())
            {
                desc += CStr("   /* ") + CStr(exportInfo->desc) + CStr(" */");
            }
            desc += CStr("\n");
        }
        desc += CStr("}");
        return desc;
    }

    void Application::ClearViewportToBlack()
    {
        //TODO: check this and refactor
        int v2;  // edi
        int v3;  // esi

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
        // RVA 0x59BF60
        return m_startupFolder;
    }

    void Application::PutSpriteAbs(float x, float y, float size2, unsigned c)
    {
        // RVA 0x673950
        PutSprite2Abs(
            x - size2,
            y + size2,
            x - size2,
            y - size2,
            x + size2,
            y + size2,
            x + size2,
            y - size2,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            0.0f,
            0.0f,
            c);
    }

    void Application::PutSpriteAbs(float x1, float y1, float x2, float y2, unsigned c)
    {
        PutSprite2Abs(x1, y2, x1, y1, x2, y2, x2, y1, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, c);
    }

    void Application::PutSpriteAbs(
        float x1,
        float y1,
        float x2,
        float y2,
        float x3,
        float y3,
        float x4,
        float y4,
        unsigned c)
    {
        // RVA 0x673A30
        PutSprite2Abs(x1, y1, x2, y2, x3, y3, x4, y4, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, c);
    }

    DataServer& Application::GetAnimatedModelsServer()
    {
        return *m_serverAnimatedModels;
    }

    bool Application::KillPostEffect(CStr const&)
    {
        // RVA 0x59C670
        return false;
    }

    DataServer& Application::GetDecalsServer()
    {
        return *m_serverDecals;
    }

    int Application::DrawTextRelClip(
        PointBase<float> const& at,
        CStr const& str,
        ui::DrawInfo const& di,
        TextWrapFlags wrapFlag,
        TextFormatFlags formatFlag)
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
        return false;
    }

    DbgCounter* Application::CreateCounter(char const* name)
    {
        // RVA 0x5B2A40
        unsigned const idx = m_counterStack.AddCounter(name);
        if (idx >= m_counterStack.m_numCounters)
        {
            return nullptr;
        }
        return m_counterStack.m_stack[idx];
    }

    void Application::texGenEnqueue(IGeneratedTexture*)
    {
        // RVA 0x75C9E0
    }

    void Application::HandleCommand(int cmdId, CConsoleParams const& params)
    {
        // RVA 0x693050
        switch (cmdId)
        {
        case 0:
            EnqueueMessage(1, 0, 0, 0, 0, CStr(), AIParam());
            return;
        case 1:
            RunBenchmark();
            return;
        case 2:
            if (params.NumOfTokens(' ') >= 3)
            {
                int const width = atoi(params.UnsafeStringToken(1, ' '));
                int const height = atoi(params.UnsafeStringToken(2, ' '));
                int fullScreen = 0;
                if (params.NumOfTokens(' ') == 4)
                {
                    fullScreen = atoi(params.UnsafeStringToken(3, ' '));
                }
                EnqueueMessage(3, width, height, fullScreen, 0, CStr(), AIParam());
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Usage: /videomode <width> <heigth> [<fullscreen>]\n"));
            }
            return;
        case 3:
            g_Kernel->DumpMem("");
            return;
        case 5:
            m_bDrawMemoryStats = !m_bDrawMemoryStats;
            return;
        case 6:
            m_bDrawStats = !m_bDrawStats;
            return;
        case 7:
            m_bDrawCounters = !m_bDrawCounters;
            return;
        case 8:
            g_Kernel->GetFileServer().Reinitialize("data\\datasources.txt");
            return;
        case 9:
        {
            if (params.NumOfTokens(' ') < 2)
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(
                    CStr("Usage: /descExport <file name> [skip empty? (yes by default)]\n"));
                return;
            }
            CStr const fileName(params.UnsafeStringToken(1, ' '));
            int skipEmpty = 1;
            if (params.NumOfTokens(' ') > 2)
            {
                skipEmpty = params.IntToken(2, ' ');
            }
            scoped_ptr<fs::FileStream> stream(g_Kernel->GetFileServer().CreateFileStream());
            if (!stream->Open(fileName.c_str(), fs::IStream::OPEN_WRITE))
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Could not create file ") + fileName + CStr("\n"));
                return;
            }
            SaveExportDescToFile(*stream, skipEmpty == 1);
            g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Export description is saved to ") + fileName + CStr("\n"));
            return;
        }
        case 10:
        {
            if (params.NumOfTokens(' ') < 2)
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Usage: /descNative <native function name>\n"));
                return;
            }
            CStr const desc = GetNativeFuncDesc(params.UnsafeStringToken(1, ' '));
            g_Kernel->GetEngineCfg().m_console->PrintF(CStr("\nDescription:\n"));
            if (desc.empty())
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("No such native found...\n"));
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(desc + CStr("\n"));
            }
            g_Kernel->GetEngineCfg().m_console->PrintF(CStr("\n"));
            return;
        }
        case 11:
        {
            if (params.NumOfTokens(' ') < 2)
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Usage: /descClass <class name>\n"));
                return;
            }
            CStr const className(params.UnsafeStringToken(1, ' '));
            CStr desc("");
            Class** classes = nullptr;
            unsigned numClasses = 0;
            g_Kernel->GetListOfClasses(classes, numClasses);
            for (unsigned i = 0; i < numClasses; ++i)
            {
                if (CStr(classes[i]->m_className) == className)
                {
                    desc = GetClassDesc(classes[i]);
                    break;
                }
            }
            delete[] classes;
            g_Kernel->GetEngineCfg().m_console->PrintF(CStr("\nDescription:\n"));
            if (desc.empty())
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("No such class found...\n"));
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(desc + CStr("\n"));
            }
            g_Kernel->GetEngineCfg().m_console->PrintF(CStr("\n"));
            return;
        }
        case 12:
            if (g_pApp->m_renderer->ReloadShaders())
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Reloaded shaders OK\n"));
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(
                    CStr("Some errors occured while reloading shaders. See log...\n"));
            }
            return;
        case 13:
            m_bShowRenderStats = !m_bShowRenderStats;
            return;
        case 14:
            m_bShowDeviceMemStats = !m_bShowDeviceMemStats;
            return;
        case 15:
            m_bDrawGraph = !m_bDrawGraph;
            return;
        case 16:
        {
            int const numReloaded = g_pApp->m_renderer->ReloadTextures();
            if (numReloaded)
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(
                    CStr("Reloaded ") + CStr(numReloaded) + CStr(" textures OK\n"));
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("No textures to reload\n"));
            }
            return;
        }
        case 17:
            g_pApp->m_renderer->RepaintAllTexturesMips();
            return;
        case 18:
        {
            bool const ok = params.NumOfTokens(' ') >= 2 ?
                g_pApp->m_renderer->ReportTexturesInfo(params.UnsafeStringToken(1, ' ')) :
                g_pApp->m_renderer->ReportTexturesInfo("UsedTextures.txt");
            if (ok)
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Textures info saved ok...\n"));
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Failed to save textures info...\n"));
            }
            return;
        }
        case 19:
        {
            char const defaultName[] = "vb_info.txt";
            bool const ok = params.NumOfTokens(' ') >= 2 ?
                g_pApp->m_renderer->ReportVbsInfo(params.UnsafeStringToken(1, ' ')) :
                g_pApp->m_renderer->ReportVbsInfo(defaultName);
            if (ok)
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Vertex buffers info saved ok...\n"));
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Failed to save vertex buffers info...\n"));
            }
            return;
        }
        case 20:
        {
            char const defaultName[] = "ib_info.txt";
            bool const ok = params.NumOfTokens(' ') >= 2 ?
                g_pApp->m_renderer->ReportIbsInfo(params.UnsafeStringToken(1, ' ')) :
                g_pApp->m_renderer->ReportIbsInfo(defaultName);
            if (ok)
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Index buffers info saved ok...\n"));
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Failed to save index buffers info...\n"));
            }
            return;
        }
        case 21:
        {
            CStr const levelName = NameFromFileName(CStr(g_Kernel->GetEngineCfg().m_levFileName.GetS()));
            CStr const defaultLogFile =
                CStr("models_") + CStr(levelName.substr(0, levelName.rfind('.')).c_str()) + CStr(".txt");
            bool const ok = params.NumOfTokens(' ') >= 2 ?
                g_pApp->m_serverAnimatedModels->ReportServerInfo(params.UnsafeStringToken(1, ' ')) :
                g_pApp->m_serverAnimatedModels->ReportServerInfo(defaultLogFile.c_str());
            if (ok)
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Models info saved ok...\n"));
            }
            else
            {
                g_Kernel->GetEngineCfg().m_console->PrintF(CStr("Failed to save models info...\n"));
            }
            return;
        }
        default:
            return;
        }
    }

    void Application::RunBenchmark()
    {
        // RVA 0x8BCBD0
        auto* const r = g_pApp->m_renderer;
        r->MatPush();
        r->MatPushProj();
        r->PushBlend();
        r->PushZbState();
        int const wasInScene = r->InScene();
        if (wasInScene)
        {
            r->EndScene();
        }

        CMatrix viewMat;
        viewMat.lookAtLH(CVector(0.0f, 0.0f, -100.0f), CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 1.0f, 0.0f));
        CMatrix projMat;
        memset(&projMat, 0, sizeof(projMat));
        projMat._11 = 0.002f;
        projMat._22 = 0.002f;
        projMat._33 = 0.0005f;
        projMat.m[3][2] = 0.5f;
        projMat.m[3][3] = 1.0f;
        r->MatSet(viewMat);
        r->MatSetProj(projMat);

        rend::TexHandle tex = r->AddTexture(CStr("data\\env\\bgmount.tga"), 2u);

        // ---- Fill rate: a full-screen quad.
        int const width = r->GetViewport().m_width;
        int const height = r->GetViewport().m_height;
        int bpp = 0;
        r->GetCurBppStr(&bpp);
        float const mbPerFrame = static_cast<float>((bpp == 32 ? 4 : 2) * height * width) * 9.5367432e-7f;
        M3D_LOG_INFO(
            CStr("\n---------------------------------------------------------\nFillrate (") + CStr(width) + CStr("x") +
            CStr(height) + CStr(")\n---------------------------------------------------------"));

        auto quadVb = r->GetVbStreaming(rend::VERTEX_XYZCT1);
        int vOfs = 0;
        auto* q = static_cast<rend::VertexXYZCT1*>(r->LockVbStreaming(quadVb, 4, vOfs, nullptr));
        auto const setQ = [](rend::VertexXYZCT1& v, float x, float y, unsigned c, float tu, float tv)
        {
            v.x = x;
            v.y = y;
            v.z = 0.0f;
            v.c = c;
            v.tu = tu;
            v.tv = tv;
        };
        setQ(q[0], -500.0f, -500.0f, 0x7F0000FF, 0.0f, 0.0f);
        setQ(q[1], 500.0f, -500.0f, 0x7F00FF00, 1.0f, 0.0f);
        setQ(q[2], -500.0f, 500.0f, 0x7FFF00FF, 0.0f, 1.0f);
        setQ(q[3], 500.0f, 500.0f, 0x7FFF0000, 1.0f, 1.0f);
        r->UnlockVb(quadVb);

        rend::TexHandle const noTex;
        r->SetTexture(0, noTex, -1.0);
        r->SetTexture(1, noTex, -1.0);
        r->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
        r->SetStageState(0, rend::BM_ALPHA, rend::TS_DIFFUSE);
        r->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        r->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
        r->SetBlend(rend::BM_NONE, false);
        r->SetZbState(rend::ZB_DISABLE, false);
        r->SetToStream0(quadVb);

        auto const logFill = [&](char const* what)
        {
            float const mbPerSec = static_cast<float>(TestQuadFps(vOfs, &g_Kernel->GetTimer()) * mbPerFrame);
            M3D_LOG_INFO(CStr(what) + CStr(mbPerSec) + CStr(" MB/sec"));
        };
        logFill("No texture, no zbuffer, no blend: ");

        r->SetTexture(0, tex, -1.0);
        r->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        r->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        logFill("Single texture, no zbuffer, no blend: ");

        r->SetZbState(rend::ZB_ENABLE, false);
        r->SetZFunc(rend::M3DCMP_LESSEQUAL, false);
        logFill("Single texture, zbuffer accepts 100%, no blend: ");

        r->SetZbState(rend::ZB_ENABLE, false);
        r->SetZFunc(rend::M3DCMP_LESS, false);
        logFill("Single texture, zbuffer denies 100%, no blend: ");

        r->SetZbState(rend::ZB_ENABLE, false);
        r->SetZFunc(rend::M3DCMP_LESSEQUAL, false);
        r->SetBlend(rend::BM_ALPHA, false);
        logFill("Single texture, zbuffer accepts 100%, alpha blend: ");

        r->SetBlend(rend::BM_1_1, false);
        logFill("Single texture, zbuffer accepts 100%, color blend: ");

        r->SetBlend(rend::BM_NONE, false);
        r->SetTexture(1, tex, -1.0);
        r->SetStageState(1, rend::BM_COLOR, rend::TS_TEX_MODULATE_PREV);
        r->SetStageState(1, rend::BM_ALPHA, rend::TS_DIFF_MODULATE_PREV);
        logFill("Two textures, zbuffer accepts 100%, no blend: ");

        r->SetBlend(rend::BM_1_1, false);
        logFill("Two textures, zbuffer accepts 100%, color blend: ");

        // ---- Transform and lighting: a tube of 1201 rings of 6 vertices.
        // NOTE: the vertex and index arrays are never freed in the binary.
        auto* const verts = new rend::VertexXYZNT1[7206];
        auto* v = verts;
        for (unsigned i = 0; i <= 1200; ++i)
        {
            double const di = static_cast<double>(i);
            float const ringX = static_cast<float>(di * 0.0027083333f);
            double const angle = 0.15707636f * di;
            double const c = cos(angle);
            double const s = sin(angle);
            float const py = static_cast<float>(0.05f * c);
            float const pz = static_cast<float>(0.05f * s);
            float const ny = static_cast<float>(c);
            float const nz = static_cast<float>(s);
            float const tv = static_cast<float>(di * 0.00083333335f);
            for (unsigned j = 0; j <= 5; ++j)
            {
                double const dj = static_cast<double>(j);
                v->x = static_cast<float>(0.0079999994f * dj + ringX - 1.625f);
                v->y = py;
                v->z = pz;
                v->nx = 0.0f;
                v->ny = ny;
                v->nz = nz;
                v->tu = static_cast<float>(dj * 0.2f);
                v->tv = tv;
                ++v;
            }
        }

        // A zig-zag strip over the rings, then expanded to a triangle list.
        auto* const strip = new unsigned short[13201];
        strip[0] = 0;
        unsigned short* out = strip + 1;
        int base = 0;
        for (unsigned row = 0; row < 1200 && base <= 0xFFFF; ++row)
        {
            out[0] = static_cast<unsigned short>(base + 6);
            out[1] = static_cast<unsigned short>(base + 1);
            out[2] = static_cast<unsigned short>(base + 7);
            out[3] = static_cast<unsigned short>(base + 2);
            out[4] = static_cast<unsigned short>(base + 8);
            out[5] = static_cast<unsigned short>(base + 3);
            out[6] = static_cast<unsigned short>(base + 9);
            out[7] = static_cast<unsigned short>(base + 4);
            out[8] = static_cast<unsigned short>(base + 10);
            out[9] = static_cast<unsigned short>(base + 5);
            out[10] = static_cast<unsigned short>(base + 11);
            out += 11;
            base += 11;
            if (++row >= 1200)
            {
                break;
            }
            out[0] = static_cast<unsigned short>(base + 6);
            out[1] = static_cast<unsigned short>(base - 1);
            out[2] = static_cast<unsigned short>(base + 5);
            out[3] = static_cast<unsigned short>(base - 2);
            out[4] = static_cast<unsigned short>(base + 4);
            out[5] = static_cast<unsigned short>(base - 3);
            out[6] = static_cast<unsigned short>(base + 3);
            out[7] = static_cast<unsigned short>(base - 4);
            out[8] = static_cast<unsigned short>(base + 2);
            out[9] = static_cast<unsigned short>(base - 5);
            out[10] = static_cast<unsigned short>(base + 1);
            out += 11;
            base += 1;
        }

        auto* const indices = new unsigned short[39597];
        {
            unsigned short a0 = strip[0];
            unsigned short a1 = strip[1];
            unsigned short* dst = indices;
            for (int k = 2; k < 13201; ++k)
            {
                dst[0] = a0;
                dst[1] = a1;
                dst[2] = strip[k];
                if (k & 1)
                {
                    a1 = strip[k];
                }
                else
                {
                    a0 = strip[k];
                }
                dst += 3;
            }
        }
        delete[] strip;

        M3D_LOG_INFO(
            CStr("\n---------------------------------------------------------\nTnL (scene has ") + CStr(7206) +
            CStr(" vertices and ") + CStr(13199) + CStr(" triangles), vertex size ") + CStr(32u) +
            CStr("\n---------------------------------------------------------"));

        rend::Material mtrl;
        auto const setMaterial = [&]()
        {
            memset(&mtrl, 0, sizeof(mtrl));
            mtrl.m_diffuse.r = mtrl.m_diffuse.g = mtrl.m_diffuse.b = mtrl.m_diffuse.a = 1.0f;
            mtrl.m_ambient.r = mtrl.m_ambient.g = mtrl.m_ambient.b = mtrl.m_ambient.a = 1.0f;
            r->MaterialSet(mtrl);
        };
        auto const fill = [&](rend::VbHandle const& vb, rend::IbHandle const& ib)
        {
            memcpy(r->LockVb(vb, 0, 0, 0), verts, sizeof(rend::VertexXYZNT1) * 7206);
            r->UnlockVb(vb);
            memcpy(r->LockIb(ib, 0, 0, 0), indices, sizeof(unsigned short) * 39597);
            r->UnlockIb(ib);
        };
        auto const logGeom = [&](char const* what)
        {
            float const mtris = static_cast<float>(TestGeom(13199, &g_Kernel->GetTimer()) * 0.013199f);
            M3D_LOG_INFO(CStr(what) + CStr(mtris) + CStr(" MTris/sec"));
        };

        rend::VbHandle vb = r->AddVb(rend::VERTEX_XYZNT1, 7206, CStr("Benchmark"), 0);
        rend::IbHandle ib = r->AddIb(39597, false);
        fill(vb, ib);
        r->SetTexture(0, noTex, -1.0);
        r->SetTexture(1, noTex, -1.0);
        r->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
        r->SetStageState(0, rend::BM_ALPHA, rend::TS_DIFFUSE);
        r->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        r->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
        r->SetBlend(rend::BM_NONE, false);
        r->SetZbState(rend::ZB_ENABLE, false);
        r->SetToStream0(vb);
        r->SetIndices(ib, 0);
        setMaterial();
        r->SetLighting(true, false);
        logGeom("Non optimized vb (managed), lighting on, no tex ");

        // NOTE: the "optimized" pass just refills the same buffers.
        fill(vb, ib);
        setMaterial();
        r->SetLighting(true, false);
        logGeom("Optimized vb (managed), lighting on, no tex ");

        r->ReleaseVb(vb);
        r->ReleaseIb(ib);

        vb = r->AddVb(rend::VERTEX_XYZNT1, 7206, CStr("Benchmark"), 0x200);
        ib = r->AddIb(39597, true);
        fill(vb, ib);
        setMaterial();
        r->SetTexture(0, tex, -1.0);
        r->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        r->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        r->SetLighting(true, false);
        logGeom("Optimized vb (default), lighting on, tex on (explicit uv) ");

        r->TgEnableSetLinearSt(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, 0.0f, 0.0f, 1.0f, 1.0f);
        logGeom("Optimized vb (default), lighting on, tex on (texgen) ");

        r->ReleaseVb(vb);
        r->ReleaseIb(ib);
        r->ReleaseTexture(tex);
        if (wasInScene)
        {
            r->BeginScene();
        }
        r->PopZbState();
        r->PopBlend();
        r->MatPop(false);
        r->MatPopProj();
    }

    void Application::ShowSystemCursor(bool bShow)
    {
        if (bShow)
        {
            while (ShowCursor(1) < 0)
                ;
        }
        else
        {
            while (ShowCursor(0) >= 0)
                ;
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
                M3D_RENDERER->DrawIndexedPrimitiveEffect(
                    rend::M3DPT_TRIANGLELIST,
                    m_flushQuadsShader,
                    0,
                    m_numPointsVerts,
                    0,
                    2 * (this->m_numPointsVerts / 4));
            }
            else
            {
                M3D_RENDERER->DrawIndexedPrimitive(
                    rend::M3DPT_TRIANGLELIST, 0, this->m_numPointsVerts, 0, 2 * (this->m_numPointsVerts / 4));
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

    void Application::EnqueueEvent(Event const& event)
    {
        // RVA 0x59EBE0
        int head = m_eventsQueueHead + 1;
        if (static_cast<unsigned>(head) >= 5000)
        {
            head = 0;
        }
        // A full queue drops the event.
        if (m_eventsQueueTail != head)
        {
            m_eventsQueue[m_eventsQueueHead] = event;
            m_eventsQueueHead = head;
        }
    }

    bool Application::GetEvent(Event& event, bool removeFromQueue)
    {
        // RVA 0x59BF10
        if (m_eventsQueueTail == m_eventsQueueHead)
        {
            return false;
        }

        event = m_eventsQueue[m_eventsQueueTail];
        if (removeFromQueue)
        {
            int tail = m_eventsQueueTail + 1;
            if (static_cast<unsigned>(tail) >= 5000)
            {
                tail = 0;
            }
            m_eventsQueueTail = tail;
        }
        return true;
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
        // RVA 0x685920
        if (fl.m_isHieroglyphic && g_Kernel->GetEngineCfg().m_ui_forceHieroglyphicFont.GetB())
        {
            ui::Wnd::GetGfxServer()->SetFont(ui::Wnd::GetGfxServer()->m_hieroglyphicFontId);
        }

        ui::Font* const fnt = ui::Wnd::GetGfxServer()->GetCurFont();
        if (!fnt)
        {
            return 0;
        }

        CStr text(fl.m_text);
        int lastLeadingSpacePos = -1;
        int trimmedLen = fl.m_text.length();
        if (fl.m_format == TF_FULL)
        {
            int const len = fl.m_text.length();
            char const* const src = fl.m_text.c_str();
            // A leading "@xxxxxxxx" colour code is skipped when counting the indentation.
            int i = src[0] == '@' ? 9 : 0;
            if (i < len)
            {
                int count = -1;
                for (; i < len && src[i] == ' '; ++i)
                {
                    ++count;
                }
                // NOTE: this is the number of leading spaces minus one, not a position, so it is off by
                // nine when the line starts with a colour code.
                lastLeadingSpacePos = count;
            }
            for (int j = len - 1; j >= 0 && src[j] == ' '; --j)
            {
                --trimmedLen;
            }
            text = text.substr(0, trimmedLen);
        }

        int const textLen = text.length();
        PointBase<float> sz;
        GetTextExtent(text, sz, -1, nullptr, nullptr, nullptr, nullptr, nullptr);

        BoundsBase<float> textBounds;
        switch (fl.m_format)
        {
        case TF_CENTER:
            textBounds.x0 = fl.m_origin.x - sz.x * 0.5f;
            textBounds.y0 = fl.m_origin.y;
            textBounds.width = (sz.x * 0.5f + fl.m_origin.x) - textBounds.x0;
            textBounds.height = (fl.m_origin.y + sz.y) - fl.m_origin.y;
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
            textBounds.width = fl.m_origin.x - textBounds.x0;
            textBounds.height = (textBounds.y0 + sz.y) - textBounds.y0;
            break;
        case TF_FULL:
            textBounds.x0 = di.m_clientRect.x0;
            textBounds.y0 = fl.m_origin.y;
            textBounds.width = (di.m_clientRect.width + di.m_clientRect.x0) - textBounds.x0;
            textBounds.height = (textBounds.y0 + sz.y) - textBounds.y0;
            break;
        default:
            break;
        }

        BoundsBase<float> const visible = di.m_clientClippedRect.Intersect(textBounds);
        if (visible.Empty())
        {
            return 0;
        }

        bool const doClip =
            !(textBounds.x0 == visible.x0 && textBounds.y0 == visible.y0 && textBounds.width == visible.width &&
              textBounds.height == visible.height);

        int firstVisibleChar = -1;
        int firstInvisibleChar = textLen;
        CStr leftInvisibleSubstr;
        CStr rightInvisibleSubstr;
        if (doClip)
        {
            // The visible part in the text's own coordinates.
            BoundsBase<float> localClip = visible;
            switch (fl.m_format)
            {
            case TF_CENTER:
                localClip.x0 = (0.0f - (fl.m_origin.x - sz.x * 0.5f)) + visible.x0;
                localClip.y0 = (0.0f - fl.m_origin.y) + visible.y0;
                break;
            case TF_LEFT:
            case TF_FULL:
                localClip.x0 = (0.0f - fl.m_origin.x) + visible.x0;
                localClip.y0 = (0.0f - fl.m_origin.y) + visible.y0;
                break;
            case TF_RIGHT:
                localClip.x0 = (0.0f - (fl.m_origin.x - sz.x)) + visible.x0;
                localClip.y0 = (0.0f - fl.m_origin.y) + visible.y0;
                break;
            default:
                break;
            }
            PointBase<float> clippedSz;
            GetTextExtent(
                text,
                clippedSz,
                -1,
                &localClip,
                &firstVisibleChar,
                &firstInvisibleChar,
                &leftInvisibleSubstr,
                &rightInvisibleSubstr);
        }

        float curX = 0.0f;
        for (int j = 0; j < leftInvisibleSubstr.length(); ++j)
        {
            auto const* sym = fnt->m_symbols[static_cast<unsigned char>(leftInvisibleSubstr.c_str()[j])];
            curX += sym ? sym->m_precalcedABCWidth : 0.0f;
        }

        PointBase<float> at;
        switch (fl.m_format)
        {
        case TF_CENTER:
            at.x = fl.m_origin.x - sz.x * 0.5f;
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
            break;
        }
        g_pApp->m_renderer->RelToAbs(at.x, at.y);
        BoundsBase<float> lclip = visible;
        g_pApp->m_renderer->RelToAbs(lclip.x0, lclip.y0);
        g_pApp->m_renderer->RelToAbs(lclip.width, lclip.height);

        unsigned clr = fl.m_color;
        if (firstVisibleChar != -1)
        {
            if (unsigned const lastColor = FindLastColorInStr(text.substr(0, firstVisibleChar)))
            {
                clr = lastColor;
            }
        }

        float spaceW = fnt->m_symbols[' '] ? fnt->m_symbols[' ']->m_precalcedABCWidth : 0.0f;
        if (fl.m_format == TF_FULL)
        {
            int numSpaces = 0;
            for (int i = lastLeadingSpacePos + 1; i < textLen; ++i)
            {
                if (text.c_str()[i] == ' ')
                {
                    ++numSpaces;
                }
            }
            PointBase<float> absSz = sz;
            g_pApp->m_renderer->RelToAbs(absSz.x, absSz.y);
            BoundsBase<float> absClient = di.m_clientRect;
            g_pApp->m_renderer->RelToAbs(absClient.x0, absClient.y0);
            g_pApp->m_renderer->RelToAbs(absClient.width, absClient.height);
            // NOTE: a justified line without inner spaces divides by zero here.
            spaceW =
                (static_cast<float>(numSpaces) * spaceW + (absClient.width - absSz.x)) / static_cast<float>(numSpaces);
        }

        // '@' + 8 hex digits sets the colour; '#' escapes the next '@', '#', '$' or '&'; '$' and '&' are dropped.
        bool escaped = false;
        for (int idx = firstVisibleChar + 1; idx < firstInvisibleChar; ++idx)
        {
            unsigned char const ch = text.c_str()[idx];
            if (ch < ' ')
            {
                escaped = false;
                continue;
            }
            if (!escaped)
            {
                if (ch == '@')
                {
                    if (textLen > idx + 8)
                    {
                        char colorBuf[9];
                        memcpy(colorBuf, text.c_str() + idx + 1, 8);
                        colorBuf[8] = 0;
                        sscanf_s(colorBuf, "%x", &clr);
                        idx += 8;
                    }
                    continue;
                }
                if (ch == '#')
                {
                    escaped = true;
                    continue;
                }
                if (ch == '$' || ch == '&')
                {
                    continue;
                }
            }
            escaped = false;

            auto const* sym = fnt->m_symbols[ch];
            float glyphW = sym ? sym->m_precalcedGlyphSz.x : 0.0f;
            float const glyphH = sym ? sym->m_precalcedGlyphSz.y : 0.0f;
            bool const stretchedSpace = ch == ' ' && idx > lastLeadingSpacePos;
            float abcA = 0.0f;
            if (stretchedSpace)
            {
                glyphW = spaceW;
            }
            else if (sym)
            {
                abcA = sym->m_abc.m_A;
            }
            float const w = glyphW;

            float x0 = static_cast<float>(static_cast<int>(abcA + at.x + curX)) + 0.5f;
            float y0 = static_cast<float>(static_cast<int>(at.y)) + 0.5f;
            float x1 = x0 + glyphW;
            float y1 = y0 + glyphH;

            ui::Font::TextureCoordinates tcs;
            if (sym)
            {
                tcs = sym->m_tcs;
            }
            float tx0 = tcs.m_coordinates[0];
            float ty0 = tcs.m_coordinates[1];
            float tx1 = tcs.m_coordinates[2];
            float ty1 = tcs.m_coordinates[3];
            if (doClip)
            {
                float const clipRight = lclip.width + lclip.x0;
                float const clipBottom = lclip.height + lclip.y0;
                float const origX1 = x1;
                float const origY1 = y1;
                if (idx == firstVisibleChar + 1 && lclip.x0 > x0)
                {
                    tx0 = (lclip.x0 - x0) / w * (tcs.m_coordinates[2] - tcs.m_coordinates[0]) + tcs.m_coordinates[0];
                    x0 = lclip.x0;
                }
                if (idx == firstInvisibleChar - 1 && origX1 > clipRight)
                {
                    tx1 =
                        (origX1 - clipRight) / w * (tcs.m_coordinates[0] - tcs.m_coordinates[2]) + tcs.m_coordinates[2];
                    x1 = clipRight;
                }
                float const origY0 = y0;
                if (lclip.y0 > origY0)
                {
                    ty0 = (lclip.y0 - origY0) / glyphH * (tcs.m_coordinates[3] - tcs.m_coordinates[1]) +
                        tcs.m_coordinates[1];
                    y0 = lclip.y0;
                }
                if (origY1 > clipBottom)
                {
                    ty1 = (origY1 - clipBottom) / glyphH * (tcs.m_coordinates[1] - tcs.m_coordinates[3]) +
                        tcs.m_coordinates[3];
                    y1 = clipBottom;
                }
            }

            if (tcs.m_texId >= 0 && tcs.m_texId < static_cast<int>(fnt->m_textures.size()))
            {
                auto const& tex = fnt->m_textures[tcs.m_texId];
                if (tex != ui::Wnd::GetGfxServer()->m_curFontTexture)
                {
                    if (ui::Wnd::GetGfxServer()->m_curFontTexture.IsValid())
                    {
                        FlushQuads();
                    }
                    g_pApp->m_renderer->SetTexture(0, tex, -1.0);
                    ui::Wnd::GetGfxServer()->m_curFontTexture = tex;
                }
            }

            auto* v = RenderQuadXyzwct1GetNextPtr();
            v[0].x = x0;
            v[0].y = y0;
            v[0].tu = tx0;
            v[0].tv = ty0;
            v[1].x = x1;
            v[1].y = y0;
            v[1].tu = tx1;
            v[1].tv = ty0;
            v[2].x = x1;
            v[2].y = y1;
            v[2].tu = tx1;
            v[2].tv = ty1;
            v[3].x = x0;
            v[3].y = y1;
            v[3].tu = tx0;
            v[3].tv = ty1;
            for (int k = 0; k < 4; ++k)
            {
                v[k].z = 0.0f;
                v[k].w = 0.5f;
                v[k].c = clr;
            }

            curX += stretchedSpace ? spaceW : (sym ? sym->m_precalcedABCWidth : 0.0f);
        }
        return 1;
    }

    unsigned long Application::texGenThread(void*)
    {
        // RVA 0x75C9D0
        return 1;
    }

    int Application::AdjustWindowForChange(HWND hWnd, bool bFullScreen)
    {
        // RVA 0x59BC50
        SetWindowLongA(hWnd, GWL_STYLE, GetStyleForRenderWindow(bFullScreen));
        return 1;
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
            while (::ShowCursor(TRUE) < 0)
                ;
        }
        else
        {
            while (::ShowCursor(FALSE) >= 0)
                ;
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

    void Application::EnqueueMessage(
        int msg,
        int param0,
        int param1,
        int p2,
        int p3,
        CStr const& param4,
        AIParam const& param5)
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
        if (newHead >= 5000)
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
}  // namespace m3d

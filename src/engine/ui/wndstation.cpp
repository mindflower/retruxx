#include <cassert>
#include <m3dapp.h>
#include <ui/ui_srv.h>
#include <ui/wnd.h>
#include <ui/wndstation.h>

char const STR_OK[] = "ok";
char const STR_CANCEL[] = "cancel";
char const STR_YES[] = "yes";
char const STR_NO[] = "no";
char const STR_ERROR[] = "error";

namespace m3d
{
    namespace ui
    {
        int WndStation::Done()
        {
            StopAllAnimations();
            RemoveAllChildren();
            GetGfxServer()->Done();
            delete m_curDefault;
            Application::g_pApp->m_renderer->ReleaseTexture(&m_currentCursor.m_tex);   
            delete GetGfxServer();
            return 1;
        }

        Cursor const& WndStation::GetCurrentCursor() const
        {
            return m_currentCursor;
        }

        int WndStation::Create(CStr const& stringsName, CStr const& schemaName)
        {
            m_curDefault->Create("data\\cursors\\default.cursor");
            GetGfxServer()->Create();
            GetGfxServer()->SetSchema(schemaName);
            CreateDefaultStrings();
            LoadStrings(stringsName);
            return 1;
        }

        int WndStation::Repaint()
        {
            Application::g_pApp->m_renderer->PushZbState();
            Application::g_pApp->m_renderer->PushBlend();
            Application::g_pApp->m_renderer->PushLighting();
            if (m_wndMouseCapture == nullptr)
            {
                for (auto* it = GetFirstNestling(); it != nullptr; it = it->GetNextRelative())
                {
                    auto* wnd = reinterpret_cast<Wnd*>(it);
                    if ((wnd->GetStyle() & 0x200) != 0 && wnd->IsPtInBounds(m_prevMouseCoord))
                    {
                        m_wndMouseCapture = GetWndForMousePoint(wnd, m_prevMouseCoord, false);
                        if (m_wndMouseCapture != nullptr)
                        {
                            break;
                        }
                    }
                }
                //TODO: check correctness
                if (m_wndMouseCapture == nullptr)
                {
                    m_wndMouseCapture = (GetStyle() & 0x102) == 0 ? this : nullptr;
                }
            }
            auto* wnd = ModalOverride(m_wndMouseCapture);
            if (wnd != nullptr)
            {
                UpdateOnMouseInOut(wnd);
            }
            if (m_wndForTooltip != nullptr && wnd != m_wndForTooltip)
            {
                RemoveCurrentTooltip();
            }
            assert(m_curDefault);
            auto oldCursor = m_currentCursor;
            if (m_wndMouseOver != nullptr && m_wndMouseOver->GetCursorShow())
            {
                if (m_wndMouseOver->GetCursor(m_currentCursor))
                {
                    m_currentCursor = *m_curDefault;
                }
                if (Application::g_pApp->IsDXCursorEnabled())
                {
                    if (m_currentCursor == oldCursor)
                    {
                        Application::g_pApp->m_renderer->UpdateDXCursorFrame();
                    }
                    else
                    {
                        //TODO: check this
                        Application::g_pApp->m_renderer->SetupDXCursor(&m_currentCursor.m_tex, m_currentCursor.m_spot.x, m_currentCursor.m_spot.y, 0);
                    }
                    Application::g_pApp->m_renderer->ShowDXCursor(true);
                }
                else
                {
                    Application::g_pApp->m_renderer->SetBlend(rend::BM_ALPHA, false);
                    Application::g_pApp->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
                    Application::g_pApp->m_renderer->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
                    Application::g_pApp->m_renderer->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
                    Application::g_pApp->m_renderer->SetTexture(0, &m_currentCursor.m_tex, -1.0);   //TODO: check this
                    auto mouseX = static_cast<float>(Application::g_pApp->GetMouseX());
                    auto mouseY = static_cast<float>(Application::g_pApp->GetMouseY());
                    Application::g_pApp->m_renderer->AbsToRel(mouseX, mouseY);
                    Application::g_pApp->PutSpriteRel(
                        mouseX - m_currentCursor.m_spot.x,
                        mouseY - m_currentCursor.m_spot.y,
                        m_currentCursor.m_sz.x + (mouseX - m_currentCursor.m_spot.x),
                        m_currentCursor.m_sz.y + (mouseY - m_currentCursor.m_spot.y),
                        -1
                    );
                }
            }
            else if(Application::g_pApp->IsDXCursorEnabled())
            {
                Application::g_pApp->m_renderer->ShowDXCursor(false);
            }
            Application::g_pApp->m_renderer->PopBlend();
            Application::g_pApp->m_renderer->PopZbState();
            Application::g_pApp->m_renderer->PopLighting();
            return 1;
        }

        int WndStation::FlushGfx(rend::IRenderer*)
        {
            return 1;
        }

        void WndStation::CreateDefaultStrings()
        {
            m_strings.add(STR_OK, "Ok");
            m_strings.add(STR_CANCEL, "Cancel");
            m_strings.add(STR_YES, "Yes");
            m_strings.add(STR_NO, "No");
            m_strings.add(STR_ERROR, "Error");
        }

        Wnd* WndStation::ModalOverride(Wnd* w)
        {
            if (m_wndModalStack.empty())
            {
                return w;
            }
            auto* wnd = m_wndModalStack.back();
            if (w == wnd || w->IsChildOf(wnd))
            {
                return w;
            }
            return wnd;
        }

        void WndStation::RemoveCurrentTooltip()
        {
            if (m_wndForTooltip != nullptr)
            {
                if (m_wndForTooltip->m_toolTipWnd != nullptr)
                {
                    if (IsDirectChild(m_wndForTooltip->m_toolTipWnd))
                    {
                        GetStation()->RemoveChild(m_wndForTooltip->m_toolTipWnd);
                    }
                    delete m_wndForTooltip->m_toolTipWnd;
                    m_wndForTooltip->m_toolTipWnd = nullptr;
                    m_wndForTooltip->m_toolTipTimeOut = -1;
                }
                m_wndForTooltip = nullptr;
            }
        }

        void WndStation::UpdateOnMouseInOut(Wnd* newWnd)
        {
            if (m_wndMouseOver != newWnd)
            {
                if (m_wndMouseOver != nullptr)
                {
                    m_wndMouseOver->OnMouseOut();
                }
                if (m_wndKbdCapture && m_wndKbdCapture != m_wndMouseOver && m_wndKbdCapture != newWnd && !m_wndKbdCapture->IsChildOf(newWnd))
                {
                    m_wndKbdCapture->OnMouseOut();
                }
                newWnd->OnMouseIn();
                m_wndMouseOver = newWnd;
            }
        }
    }
}

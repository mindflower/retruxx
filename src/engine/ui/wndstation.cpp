#include <cassert>
#include <m3dapp.h>
#include <stdexcept>
#include <core/ini.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <ui/ui_srv.h>
#include <ui/wnd.h>
#include <ui/wndstation.h>

char const STR_OK[] = "ok";
char const STR_CANCEL[] = "cancel";
char const STR_YES[] = "yes";
char const STR_NO[] = "no";
char const STR_ERROR[] = "error";

namespace
{
    m3d::ui::GfxServer* gfxserver = nullptr;
}

namespace m3d
{
    namespace ui
    {
        int WndStation::DispatchMouse(Event const&)
        {
            throw std::logic_error("Not implemented");
        }

        bool WndStation::IsWndAlive(Wnd const*, int) const
        {
            throw std::logic_error("Not implemented");
        }

        Class* WndStation::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* WndStation::CaptureFocus(Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        ModalWnd* WndStation::GetTopModal()
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::DispatchPaint(Wnd*, BoundsBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::CheckForMouseClick(Wnd*, bool, PointBase<float> const*)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* WndStation::GetWndForMousePoint(Wnd*, PointBase<float> const&, bool)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::DoModal(ModalWnd*)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::PulseKeyForWindow(Wnd*, unsigned short, unsigned char)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* WndStation::GetCapture() const
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* WndStation::GetWndMouseOver()
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::EnableAnimation(bool)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::Activate(Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::OnAddWnd(Wnd*, Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* WndStation::CaptureMouse(Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        CStr WndStation::GetStringByStringId0(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::GetStringByStringId(CStr&, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        bool WndStation::HasChildModalRunning()
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::AddNotifyForWnd(Wnd*, Wnd*, unsigned, AIParam const&, bool)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::CheckForMouseDblClick(Wnd*, PointBase<float> const&, unsigned, PointBase<float>&)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::GetDefaultCursor(Cursor&)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::LoadStrings(CStr const& stringsName)
        {
            if (stringsName.empty())
            {
                return 1;
            }
            CStr err;
            ref_ptr xmlFile = ReadXmlFile(stringsName.c_str(), &err);
            if (!xmlFile)
            {
                M3D_LOG_INFO("WndStation::LoadStrings " + err);
                return 0;
            }
            ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            xmlFile->GetFirstChild_(node, "resource");
            if (!node->IsEmpty())
            {
                for (node->GetFirstChild_(node, "string"); !node->IsEmpty(); node->GetNextSibling_(node, "string"))
                {
                    auto id = node->GetAttribute("id");
                    auto value = node->GetAttribute("value");
                    m_strings.add(id, value);
                }
                return 1;
            }
            M3D_LOG_INFO("WndStation:: Create cannot find resources in " + stringsName);
            return 0;
        }

        void WndStation::CloseAllModalWithCancelRet()
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::StopAllAnimations()
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* WndStation::GetFocus() const
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::ProcessEvent(Event const&)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* WndStation::GetWndByUniqueId(int) const
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::EndModal(ModalWnd*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* WndStation::GetActive() const
        {
            throw std::logic_error("Not implemented");
        }

        bool WndStation::IsModal(ModalWnd*)
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::OnEndAnimation(Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        bool WndStation::IsAnimationEnabled() const
        {
            throw std::logic_error("Not implemented");
        }

        CStr WndStation::InitializeStringUsingIds(CStr const& src)
        {
            if (!src.empty())
            {
                if (src.find('^') == CStr_npos)
                {
                    //TODO: check correctness
                    return src;
                }
                throw std::logic_error("Not implemented");
            }
            else
            {
                //TODO: return value
                return {};
            }
        }

        WndStation::~WndStation()
        {
            m_wndStation = nullptr;
        }

        int WndStation::OnRemoveWnd(Wnd*, Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        WndStation::WndStation()
        {
            if (gfxserver == nullptr)
            {
                gfxserver = new GfxServer;
            }
            m_gfx = gfxserver;

            BoundsBase<float> const rc(0.0, 0.0, 1024.0, 768.0);
            CreateWnd("WndStation", 1, rc, 0);
            m_curDefault = new Cursor;
            m_uniqueId = 0;
            m_prevMouseCoord.x = 100.0;
            m_prevMouseCoord.y = 100.0;
        }

        int WndStation::DispatchJoystick(Event const&)
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::OnCloseComboBox(ComboBoxWnd*)
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::OnOpenComboBox(ComboBoxWnd*)
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::ForEachChild(Wnd*, void(Wnd::*)())
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::RegisterWnd(Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        GfxServer* WndStation::getGfxServer()
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::UnregisterWnd(Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::DispatchKey(Event const&)
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::Done()
        {
            StopAllAnimations();
            RemoveAllChildren();
            GetGfxServer()->Done();
            delete m_curDefault;
            Application::g_pApp->m_renderer->ReleaseTexture(m_currentCursor.m_tex);   
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

        Class* WndStation::GetRtClass() const
        {
            return &m_classWndStation;
        }

        int WndStation::Repaint()
        {
            Application::g_pApp->m_renderer->PushZbState();
            Application::g_pApp->m_renderer->PushBlend();
            Application::g_pApp->m_renderer->PushLighting();
            if (m_wndMouseCapture == nullptr)
            {
                for (auto* it = GetFirstChild_(); it != nullptr; it = it->GetNextSibling_())
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

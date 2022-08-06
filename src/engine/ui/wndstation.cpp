#include <cassert>
#include <m3dapp.h>
#include <stdexcept>
#include <core/ini.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <ui/comboboxwnd.h>
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

        bool WndStation::IsWndAlive(Wnd const* w, int uniqueId) const
        {
            if (!w)
            {
                return false;
            }
            int res = 0;
            if (!m_allWindows.getValueByKey(reinterpret_cast<int>(w), res))
            {
                return false;
            }
            if (uniqueId == -1)
            {
                return true;
            }
            return res == uniqueId;
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
            return 1;
        }

        Wnd* WndStation::CaptureMouse(Wnd*)
        {
            throw std::logic_error("Not implemented");
        }

        CStr WndStation::GetStringByStringId0(CStr const& id)
        {
            CStr result;
            GetStringByStringId(result, id);
            return result;
        }

        int WndStation::GetStringByStringId(CStr& dest, CStr const& id)
        {
            if (!m_strings.get(id, dest))
            {
                dest = "MISSING!";
                return 0;
            }
            return 1;
        }

        bool WndStation::HasChildModalRunning()
        {
            throw std::logic_error("Not implemented");
        }

        int WndStation::AddNotifyForWnd(Wnd* from, Wnd* to, unsigned msg, AIParam const& data, bool urgent)
        {
            if (urgent)
            {
                Application::g_pApp->ImmediateMessage(40, reinterpret_cast<int>(from), reinterpret_cast<int>(to), msg, 0, {}, data);
            }
            else
            {
                Application::g_pApp->EnqueueMessage(40, reinterpret_cast<int>(from), reinterpret_cast<int>(to), msg, 0, {}, data);
            }
            return 1;
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

        int WndStation::ProcessEvent(Event const& ev)
        {
            int v3; // ebx
            m3d::ui::ModalWnd* v6; // ebx
            m3d::ui::Wnd* v7; // ebp
            m3d::ui::Wnd* v8; // ebp
            m3d::ui::WndStation* v9; // eax
            m3d::ui::WndStation* v10; // eax
            m3d::ui::Wnd* v11; // [esp-4h] [ebp-34h]
            void* msg; // [esp+10h] [ebp-20h]
            m3d::ui::Wnd* eventa; // [esp+34h] [ebp+4h]


            AIParam data = ev.m_aiParamEv;

            v3 = 0;
            switch (ev.m_eventType)
            {
            case 4:
                ForEachChild(this, &Wnd::OnDisplayChanged);
                goto LABEL_3;
            case 5:
            case 6:
                goto $L118755;
            case 7:
            case 8:
                v3 = DispatchKey(ev);
                break;
            case 9:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xF:
                v3 = DispatchMouse(ev);
                break;
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
                v3 = DispatchJoystick(ev);
                break;
            case 0x27:
                v6 = reinterpret_cast<ModalWnd*>(ev.m_void[0]);
                if (v6->CanClose())
                    EndModal(v6, ev.m_uintEv[1]);
            $L118755:
                v3 = 1;
                break;
            case 0x28:
                v7 = reinterpret_cast<Wnd*>(ev.m_void[0]);
                eventa = reinterpret_cast<Wnd*>(ev.m_void[1]);
                if (m3d::ui::WndStation::IsWndAlive(v7, -1) && m3d::ui::WndStation::IsWndAlive(eventa, -1))
                {
                    eventa->OnWndNotify( v7, v7->m_id, ev.m_uintEv[2], data);
                    v3 = 1;
                }
                break;
            case 0x29:
            case 0x2E:
                goto $L118771;
            case 0x2A:
                m3d::ui::WndStation::OnEndAnimation(reinterpret_cast<Wnd*>(ev.m_void[0]));
            $L118771:
                v3 = this->OnEvent(ev);
                break;
            case 0x2B:
                v3 = 0;
                m3d::ui::WndStation::StopAllAnimations();
                break;
            case 0x2C:
                m3d::ui::WndStation::RemoveCurrentTooltip();
                v8 = reinterpret_cast<Wnd*>(ev.m_void[0]);
                if (m3d::ui::WndStation::IsWndAlive(v8, -1))
                {
                    this->m_wndForTooltip = v8;
                    if (v8->m_toolTipWnd)
                    {
                        GetStation()->AddChild(this->m_wndForTooltip->m_toolTipWnd);
                        v11 = this->m_wndForTooltip->m_toolTipWnd;
                        GetStation()->MoveChildToFirstPosition(v11);
                    }
                }
                break;
            case 0x2D:
                m3d::ui::WndStation::RemoveCurrentTooltip();
                break;
            case 0x2F:
                v3 = 1;
                m3d::ui::WndStation::OnOpenComboBox(reinterpret_cast<ComboBoxWnd*>(ev.m_void[0]));
                break;
            case 0x30:
                v3 = 1;
                m3d::ui::WndStation::OnCloseComboBox(reinterpret_cast<ComboBoxWnd*>(ev.m_void[0]));
                break;
            default:
            LABEL_3:
                v3 = 0;
                break;
                }
                if (ev.m_eventType >= 0x10000)
                    this->OnEvent(ev);
                return v3;

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
                CStr newStr = src;
                auto startPos = 0;
                while(true)
                {
                    auto const pos1 = newStr.find('^');
                    if (pos1 == CStr_npos)
                    {
                        break;
                    }
                    auto const pos2 = newStr.find('^', pos1 + 1);
                    if (pos2 == CStr_npos)
                    {
                        break;
                    }
                    CStr replacedSubstr;
                    GetStringByStringId(replacedSubstr, newStr.substr(pos1 + 1, pos2));
                    newStr = newStr.substr(startPos, pos1) + replacedSubstr + newStr.substr(pos2 + 1);
                    startPos = pos2 + 1;
                }
                return newStr;
            }
            return src;
        }

        WndStation::~WndStation()
        {
            m_wndStation = nullptr;
        }

        int WndStation::OnRemoveWnd(Wnd* parent, Wnd* wnd)
        {
            //TODO: check this
            if (wnd->IsKindOf(RT_CLASS_LOCAL(ModalWnd)))
            {
                auto modalWnd = dynamic_cast<ModalWnd*>(wnd);
                M3D_ASSERT(!IsModal(modalWnd));
            }
            if (wnd == m_wndActive || m_wndActive->IsChildOf(wnd))
            {
                if (wnd->IsKindOf(RT_CLASS_LOCAL(ModalWnd)))
                {
                    if (!m_wndModalStack.empty())
                    {
                        Activate(m_wndModalStack.back());
                    }
                    else
                    {
                        Activate(nullptr);
                    }
                }
                else
                {
                    Activate(parent);
                }
            }
            if (wnd->IsKindOf(RT_CLASS_LOCAL(ModalWnd)))
            {
                Application::g_pApp->EnqueueMessage(41, reinterpret_cast<int>(wnd), 0, 0, 0, {}, {});
            }
            if (m_wndKbdCapture && (wnd == m_wndKbdCapture || m_wndKbdCapture->IsChildOf(wnd)))
            {
                if (m_wndKbdCapture)
                {
                    m_wndKbdCapture->OnLoosingFocus();
                }
                m_wndKbdCapture = this;
                m_wndKbdCapture->OnObtainingFocus();
            }
            if (m_wndMouseCapture && (wnd == m_wndMouseCapture || m_wndMouseCapture->IsChildOf(wnd)))
            {
                CaptureMouse(nullptr);
            }
            if (m_wndMouseOver == wnd || m_wndMouseOver->IsChildOf(wnd))
            {
                m_wndMouseOver->OnMouseOut();
                m_wndMouseOver = parent;
            }
            if (m_wndForTooltip && (wnd == m_wndForTooltip || m_wndForTooltip->IsChildOf(wnd)))
            {
                RemoveCurrentTooltip();
            }
            if (m_wndCandidateForDblClick && (wnd == m_wndCandidateForDblClick || m_wndCandidateForDblClick->IsChildOf(wnd)))
            {
                m_wndCandidateForDblClick = nullptr;
            }
            if (!wnd->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)) || wnd != m_wndOpenedComboBox)
            {
                return 1;
            }
            if (wnd)
            {
                m_wndOpenedComboBox = nullptr;
            }
            return 1;
        }

        WndStation::WndStation()
        {
            m_wndStation = this;
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

        void WndStation::OnCloseComboBox(ComboBoxWnd* combo)
        {
            if (combo)
            {
	            if (m_wndOpenedComboBox == combo)
	            {
                    m_wndOpenedComboBox = nullptr;
	            }
            }
        }

        void WndStation::OnOpenComboBox(ComboBoxWnd*)
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::ForEachChild(Wnd*, void(Wnd::*)())
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::RegisterWnd(Wnd* w)
        {
            if (w)
            {
                if (w->m_uniqueId == -1)
                {
                    w->m_uniqueId = m_nextUniqueId;
                    m_allWindows.addValueByKey(reinterpret_cast<unsigned>(w), m_nextUniqueId);
                    m_allWindowsById.addValueByKey(m_nextUniqueId, w);
                    ++m_nextUniqueId;
                }
            }
        }

        GfxServer* WndStation::getGfxServer()
        {
            throw std::logic_error("Not implemented");
        }

        void WndStation::UnregisterWnd(Wnd* w)
        {
            //TODO: check this and refactor
            m3d::ui::Wnd* v2; // esi

            v2 = w;
            if (w)
            {
                if (w->m_uniqueId != -1)
                {
                    m_allWindowsById.removeByKey(w->m_uniqueId);
                    m_allWindows.removeByKey(reinterpret_cast<unsigned>(w));
                    v2->m_uniqueId = -1;
                }
            }
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
                        Application::g_pApp->m_renderer->SetupDXCursor(m_currentCursor.m_tex, m_currentCursor.m_spot.x, m_currentCursor.m_spot.y, 0);
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

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

#include "core/timer.h"
#include "impulses/i_impulses.h"
#include "ui/button.h"

#include "thirdparty/injecttools.h"

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
        RT_CLASS_EXPORTS_BEGIN(WndStation)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(WndStation);

        int WndStation::DispatchMouse(Event const& ev)
        {
            float x = ev.m_shortEv[0];
            float y = ev.m_shortEv[1];
            float x1 = ev.m_shortEv[2];
            float y1 = ev.m_shortEv[3];
            M3D_APP->m_renderer->AbsToRel(x, y);
            M3D_APP->m_renderer->AbsToRel(x1, y1);
            m_prevMouseCoord.x = x;
            m_prevMouseCoord.y = y;
            PointBase<float> sxy{x, y};
            PointBase<float> sxy1{x1, y1};
            auto captureWnd = m_wndMouseCapture;
            if (!captureWnd)
            {
                for (auto* it = GetFirstChild(); it != nullptr; it = it->GetNextSibling())
                {
                    auto* wnd = reinterpret_cast<Wnd*>(it);
                    if ((wnd->GetStyle() & 0x200) != 0 && wnd->IsPtInBounds(m_prevMouseCoord))
                    {
                        captureWnd = GetWndForMousePoint(wnd, m_prevMouseCoord, false);
                        if (captureWnd != nullptr)
                        {
                            break;
                        }
                    }
                }
                //TODO: check correctness
                if (captureWnd == nullptr)
                {
                    captureWnd = (GetStyle() & 0x102) == 0 ? this : nullptr;
                }
            }
            auto* wnd = ModalOverride(captureWnd);
            float xx = 0.0;
            float yy = 0.0;
            for (auto* parent = wnd; parent; parent = dynamic_cast<Wnd*>(parent->GetParent()))
            {
                auto const bounds = parent->GetBounds();
                xx += bounds.x0;
                yy += bounds.y0;
            }
            PointBase<float> lc;
            lc.x = sxy.x - xx;
            lc.y = sxy.y - yy;
            UpdateOnMouseInOut(wnd);
            auto handled = 0;
            auto state = ev.m_shortEv[2];
            PointBase<float> firstClickLc{ 0.0, 0.0 };
            switch (ev.m_eventType)
            {
            case 9:
            {
                handled = wnd->OnMouseMove(lc, sxy1);
                if (wnd!=this)
                {
                    Event newEv = ev;
                    newEv.m_eventType = 38;
                    OnEvent(newEv);
                }
                break;
            }
            case 10:
            {
                handled = wnd->OnMouseButton0(state, lc);
                if ((wnd->GetStyle() & 0x20000) != 0)
                {
                    if (CheckForMouseDblClick(wnd, lc, state, firstClickLc))
                    {
                        handled |= wnd->OnMouseDblClick(firstClickLc, lc);
                        if (wnd == this)
                        {
                            Event newEv = ev;
                            newEv.m_eventType = 13;
                            OnEvent(newEv);
                        }
                    }
                }
                break;
            }
            default:
                throw retruxx::logic_error("Not implemented");
            }
            if (ev.m_eventType == 10 || ev.m_eventType == 11 || ev.m_eventType == 12)
            {
                if (state)
                {
                    if (m_wndOpenedComboBox)
                    {
                        throw retruxx::logic_error("Not implemented");
                    }
                }
            }
            if (wnd == this)
            {
                handled = OnEvent(ev);
            }
            M3D_APP->m_pImpulses->HandleKeyboardMouseEvent(ev, wnd);
            return handled;
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
            return RT_CLASS_LOCAL(Wnd);
        }

        Wnd* WndStation::CaptureFocus(Wnd* wnd)
        {
            auto const oldCapture = m_wndKbdCapture;
            if (oldCapture)
            {
                oldCapture->OnLoosingFocus();
            }
            if (wnd)
            {
                m_wndKbdCapture = wnd;
                wnd->OnObtainingFocus();
            }
            else
            {
                m_wndKbdCapture = this;
                OnObtainingFocus();
            }
            return oldCapture;
        }

        ModalWnd* WndStation::GetTopModal()
        {
            throw retruxx::logic_error("Not implemented");
        }

        int WndStation::DispatchPaint(Wnd* curWnd, BoundsBase<float> const& clipTo)
        {
            auto childBounds = clipTo;
            DrawInfo info{};
            OnTick(M3D_KERNEL->GetTimer().GetCurTimeUnscaled(), M3D_KERNEL->GetTimer().GetLastFrameTimeUnscaled());
            if (curWnd->m_bounds.width + curWnd->m_bounds.x0 - curWnd->m_bounds.x0 != 0.0 ||
                curWnd->m_bounds.y0 - (curWnd->m_bounds.height + curWnd->m_bounds.y0) != 0.0)
            {
                if ((curWnd->m_style & 1) == 0 && (curWnd->m_style & 0x200) != 0)
                {
                    BoundsBase<float> drawReserved{};
                    drawReserved.x0 = 0.0;
                    drawReserved.y0 = 0.0;
                    drawReserved.width = curWnd->m_bounds.width;
                    drawReserved.height = curWnd->m_bounds.height;
                    info.m_originalRect = curWnd->ToScreen(drawReserved);
                    info.m_clippedRect = clipTo.Intersect(info.m_originalRect);

                    info.m_clientRect = curWnd->ToScreen(curWnd->GetClientBounds());
                    info.m_clientClippedRect = clipTo.Intersect(info.m_clientRect);

                    if (info.m_clippedRect.width + info.m_clippedRect.x0 - info.m_clippedRect.x0 != 0.0
                        || info.m_clippedRect.y0 - (info.m_clippedRect.height + info.m_clippedRect.y0) != 0.0)
                    {
                        M3D_APP->m_renderer->SetWhiteTexture(0);
                        M3D_APP->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
                        M3D_APP->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
                        M3D_APP->m_renderer->DisableTextureStages(1);
                        info.m_wndDest = curWnd;
                        curWnd->OnPaint(info);
                        GetGfxServer()->FlushWindow(curWnd);
                    }
                    childBounds = info.m_clippedRect;
                }
            }

            //TODO: check child order
            retruxx::vector<Wnd*> wnds;
            for (auto child = curWnd->GetFirstChild(); child; child = child->GetNextSibling())
            {
                if (auto const wnd = dynamic_cast<Wnd*>(child))
                {
                    wnds.insert(wnds.begin(), wnd);
                }
            }
            for (auto const& wnd : wnds)
            {
                DispatchPaint(wnd, childBounds);
            }

            if (curWnd->m_bounds.width + curWnd->m_bounds.x0 - curWnd->m_bounds.x0 != 0.0 ||
                curWnd->m_bounds.y0 - (curWnd->m_bounds.height + curWnd->m_bounds.y0) != 0.0)
            {
                if ((curWnd->m_style & 1) == 0 &&
                    (curWnd->m_style & 0x200) != 0 &&
                    (info.m_clippedRect.width + info.m_clippedRect.x0 - info.m_clippedRect.x0 != 0.0 ||
                        info.m_clippedRect.y0 - (info.m_clippedRect.height + info.m_clippedRect.y0) != 0.0))
                {
                    M3D_APP->m_renderer->SetWhiteTexture(0);
                    M3D_APP->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
                    M3D_APP->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
                    M3D_APP->m_renderer->DisableTextureStages(1);
                    curWnd->OnPaintOverChildren(info);
                }
            }
            return 1;
        }

        int WndStation::CheckForMouseClick(Wnd*, bool, PointBase<float> const*)
        {
            throw retruxx::logic_error("Not implemented");
        }

        Wnd* WndStation::GetWndForMousePoint(Wnd* curWnd, PointBase<float> const& pt, bool affectAll)
        {
            //TODO: check this
            auto res = curWnd;
            if (curWnd)
            {
                for (auto obj = curWnd->GetFirstChild(); obj; obj = obj->GetNextSibling())
                {
                    auto wnd = (Wnd*)(obj);
                    if ((wnd->GetStyle() & 0x200) != 0 || affectAll)
                    {
                        if (wnd->IsPtInBounds(pt))
                        {
                            res = GetWndForMousePoint(wnd, pt, affectAll);
                            if (res)
                            {
                                return res;
                            }
                        }
                    }
                }
                res = curWnd;
            }
            if (!affectAll && (res->GetStyle() & 0x102) != 0)
            {
                return nullptr;
            }
            return res;
        }

        int WndStation::DoModal(ModalWnd* wnd)
        {
            m_wndModalStack.push_back(wnd);
            CaptureMouse(nullptr);
            wnd->m_modalAttachedToStation = !wnd->GetParent() || this == wnd->GetParent() && wnd->m_bSuspendedUnlink;
            if (wnd->m_modalAttachedToStation)
            {
                AddChild(wnd);
            }
            wnd->OnInitModal();
            Activate(wnd);
            M3D_APP->StartExclusiveMsgLoop();
            if (m_wndModalStack.empty())
            {
                return m_wndModalRetVal;
            }
            for (auto const& modal : m_wndModalStack)
            {
                if (modal == wnd)
                {
                    EndModal(wnd, 1);
                    break;
                }
            }
            return m_wndModalRetVal;
        }

        int WndStation::PulseKeyForWindow(Wnd*, unsigned short, unsigned char)
        {
            throw retruxx::logic_error("Not implemented");
        }

        Wnd* WndStation::GetCapture() const
        {
            return m_wndMouseCapture;
        }

        Wnd* WndStation::GetWndMouseOver()
        {
            throw retruxx::logic_error("Not implemented");
        }

        void WndStation::EnableAnimation(bool)
        {
            throw retruxx::logic_error("Not implemented");
        }

        int WndStation::Activate(Wnd* wnd)
        {
            auto wndToActive = wnd;
            if (!wndToActive)
            {
                wndToActive = this;
            }
            if (wnd == m_wndActive)
            {
                return 0;
            }
            m_wndActive->OnActivate(false);
            if ((m_wndActive->GetStyle() & 0x1000) != 0)
            {
                if (m_wndKbdCapture)
                {
                    m_wndKbdCapture->OnLoosingFocus();
                }
                m_wndKbdCapture = this;
                OnObtainingFocus();
            }
            m_wndActive = wndToActive;
            m_wndActive->OnActivate(true);
            if ((m_wndActive->GetStyle() & 0x1000) != 0)
            {
                CaptureFocus(m_wndActive);
            }
            for (Object* obj = wndToActive; obj; obj = obj->GetParent())
            {
                auto parent = obj->GetParent();
                if (parent)
                {
                    parent->MoveChildToFirstPosition(obj);
                }
            }
            return 1;
        }

        int WndStation::OnAddWnd(Wnd*, Wnd*)
        {
            return 1;
        }

        Wnd* WndStation::CaptureMouse(Wnd*)
        {
            throw retruxx::logic_error("Not implemented");
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
            return !m_wndModalStack.empty();
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


        RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x00593210, WndStation::CheckForMouseDblClick);
        int WndStation::CheckForMouseDblClick(Wnd*, PointBase<float> const&, unsigned, PointBase<float>&)
        {
            throw retruxx::logic_error("Not implemented");
        }

        int WndStation::GetDefaultCursor(Cursor& cur)
        {
            cur = *m_curDefault;
            return 1;
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
            xmlFile->GetFirstChild(node, "resource");
            if (!node->IsEmpty())
            {
                for (node->GetFirstChild(node, "string"); !node->IsEmpty(); node->GetNextSibling(node, "string"))
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
            for (auto& modal : m_wndModalStack)
            {
                modal->CloseModal(0);
            }
        }

        int WndStation::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw retruxx::logic_error("Not implemented");
        }

        void WndStation::StopAllAnimations()
        {
            throw retruxx::logic_error("Not implemented");
        }

        Wnd* WndStation::GetFocus() const
        {
            throw retruxx::logic_error("Not implemented");
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
                if (IsWndAlive(v7, -1) && IsWndAlive(eventa, -1))
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
            throw retruxx::logic_error("Not implemented");
        }

        void WndStation::EndModal(ModalWnd* wnd, unsigned toRet)
        {
            if (!m_wndModalStack.empty() && m_wndModalStack[m_wndModalStack.size() - 1])
            {
                auto& wndFromStack = m_wndModalStack[m_wndModalStack.size() - 1];
                if (wnd == wndFromStack)
                {
                    wndFromStack->OnCloseModal(toRet);
                    m_wndModalStack.pop_back();
                    m_wndModalRetVal = toRet;
                    CaptureMouse(nullptr);
                    RemoveCurrentTooltip();
                    if (m_modalAttachedToStation)
                    {
                        RemoveChild(wndFromStack);
                    }
                    M3D_APP->FinishExclusiveMsgLoop();
                }
            }
        }

        Wnd* WndStation::GetActive() const
        {
            throw retruxx::logic_error("Not implemented");
        }

        bool WndStation::IsModal(ModalWnd* wnd)
        {
            for (auto const& elem : m_wndModalStack)
            {
                if (wnd == elem)
                {
                    return true;
                }
            }
            return false;
        }

        void WndStation::OnEndAnimation(Wnd*)
        {
            throw retruxx::logic_error("Not implemented");
        }

        bool WndStation::IsAnimationEnabled() const
        {
            return m_bAnimationEnabled;
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

            //inject::injectMemoryAddress(0x00A0A2D4, inject::cast<uint32_t>(m_wndStation));
            //inject::cast<WndStation*>(0x00A0A2D4);

            if (gfxserver == nullptr)
            {
                gfxserver = new GfxServer;
            }
            m_gfx = gfxserver;

            this->m_wndMouseCapture = 0;
            this->m_wndMouseOver = this;
            this->m_wndKbdCapture = this;
            this->m_wndActive = this;
            this->m_style = 0;
            this->m_wndForTooltip = 0;
            this->m_wndCandidateForDblClick = 0;


            BoundsBase<float> const rc(0.0, 0.0, 1024.0, 768.0);
            CreateWnd("WndStation", 1, rc, 0);
            m_bAnimationEnabled = true;
            m_curDefault = new Cursor;
            m_uniqueId = 0;
            m_prevMouseCoord.x = 100.0;
            m_prevMouseCoord.y = 100.0;
            this->m_wndOpenedComboBox = nullptr;
        }

        int WndStation::DispatchJoystick(Event const&)
        {
            throw retruxx::logic_error("Not implemented");
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
            throw retruxx::logic_error("Not implemented");
        }

        void WndStation::ForEachChild(Wnd*, void(Wnd::*)())
        {
            throw retruxx::logic_error("Not implemented");
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
            throw retruxx::logic_error("Not implemented");
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
            throw retruxx::logic_error("Not implemented");
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
            Application::g_pApp->m_renderer->PushZbState(rend::ZB_DISABLE);
            Application::g_pApp->m_renderer->PushLighting(false);
            Application::g_pApp->m_renderer->PushBlend(rend::BM_NONE);
            DispatchPaint(this, m_bounds);
            auto captureWnd = m_wndMouseCapture;
            if (!captureWnd)
            {
                for (auto* it = GetFirstChild(); it != nullptr; it = it->GetNextSibling())
                {
                    auto* wnd = reinterpret_cast<Wnd*>(it);
                    if ((wnd->GetStyle() & 0x200) != 0 && wnd->IsPtInBounds(m_prevMouseCoord))
                    {
                        captureWnd = GetWndForMousePoint(wnd, m_prevMouseCoord, false);
                        if (captureWnd != nullptr)
                        {
                            break;
                        }
                    }
                }
                //TODO: check correctness
                if (captureWnd == nullptr)
                {
                    captureWnd = (GetStyle() & 0x102) == 0 ? this : nullptr;
                }
            }
            auto* wnd = ModalOverride(captureWnd);
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
            if (m_wndMouseOver && m_wndMouseOver->GetCursorShow())
            {
                if (!m_wndMouseOver->GetCursor(m_currentCursor))
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
                    Application::g_pApp->m_renderer->SetTexture(0, m_currentCursor.m_tex, -1.0);   //TODO: check this
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

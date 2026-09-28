#include <cassert>
#include <cmath>
#include <config.h>
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

char const STR_OK[] = "ok";
char const STR_CANCEL[] = "cancel";
char const STR_YES[] = "yes";
char const STR_NO[] = "no";
char const STR_ERROR[] = "error";

namespace
{
    m3d::ui::GfxServer* gfxserver = nullptr;

    // Where CheckForMouseDblClick has got to in the press / release / press
    // sequence it is watching for.
    enum DblClickState
    {
        DBLCLICK_NOTHING = 0,
        DBLCLICK_PRESS = 1,
        DBLCLICK_PRESS_RELEASE = 2,
    };
}

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(WndStation)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(WndStation);

        static Wnd* FindWndUnderPoint(WndStation& station, PointBase<float> const& pt)
        {
            // Not a function in the binary: this loop is inlined in both
            // DispatchMouse (RVA 0x593690) and Repaint (RVA 0x5916A0). The first
            // visible top-level window containing the point that yields a hit wins;
            // otherwise the station itself, unless it is transparent or disabled.
            for (Object* child = station.GetFirstChild(); child != nullptr; child = child->GetNextSibling())
            {
                auto* const wnd = static_cast<Wnd*>(child);
                if (wnd->IsVisible() && wnd->IsPtInBounds(pt))
                {
                    if (Wnd* const hit = station.GetWndForMousePoint(wnd, pt, false))
                    {
                        return hit;
                    }
                }
            }
            return (station.GetStyle() & (WS_TRANSPARENT | WS_DISABLE)) == 0 ? &station : nullptr;
        }

        int WndStation::DispatchMouse(Event const& ev)
        {
            // RVA 0x593690 - finds the window under the cursor (or the capture
            // window), converts the point to its client space and routes the
            // event to the matching mouse handler.
            float x = ev.m_shortEv[0];
            float y = ev.m_shortEv[1];
            float x1 = ev.m_shortEv[2];
            float y1 = ev.m_shortEv[3];

            M3D_APP->m_renderer->AbsToRel(x, y);
            M3D_APP->m_renderer->AbsToRel(x1, y1);

            m_prevMouseCoord.x = x;
            m_prevMouseCoord.y = y;
            PointBase<float> const sxy{x, y};
            PointBase<float> const sxy1{x1, y1};

            Wnd* target = m_wndMouseCapture;
            if (!target)
            {
                target = FindWndUnderPoint(*this, sxy);
            }

            Wnd* const wnd = ModalOverride(target);

            // Station-space point relative to the window's origin.
            PointBase<float> org{0.0f, 0.0f};
            for (Object* parent = wnd; parent != nullptr; parent = parent->GetParent())
            {
                org.x += static_cast<Wnd*>(parent)->m_bounds.x0;
                org.y += static_cast<Wnd*>(parent)->m_bounds.y0;
            }
            PointBase<float> lc{sxy.x - org.x, sxy.y - org.y};

            UpdateOnMouseInOut(wnd);

            int handled = 0;
            unsigned const state = ev.m_shortEv[2];
            PointBase<float> firstClickLc{0.0, 0.0};

            switch (ev.m_eventType)
            {
            case EV_MOUSE_MOVE:
            {
                handled = wnd->OnMouseMove(lc, sxy1);
                if (wnd != this)
                {
                    Event newEv = ev;
                    newEv.m_eventType = EV_MOUSE_MOVE_ON_UI;
                    OnEvent(newEv);
                }
                break;
            }
            case EV_MOUSE_LBTN:
            {
                handled = wnd->OnMouseButton0(state, lc);
                if ((wnd->GetStyle() & WS_DBLCLICK_REACT) != 0)
                {
                    if (CheckForMouseDblClick(wnd, lc, state, firstClickLc))
                    {
                        handled |= wnd->OnMouseDblClick(firstClickLc, lc);
                        if (wnd == this)
                        {
                            Event newEv = ev;
                            newEv.m_eventType = EV_MOUSE_DBLCLICK;
                            OnEvent(newEv);
                        }
                    }
                }
                break;
            }
            case EV_MOUSE_RBTN:
            {
                handled = wnd->OnMouseButton1(state, lc);
                break;
            }
            case EV_MOUSE_MBTN:
            {
                handled = wnd->OnMouseButton2(state, lc);
                break;
            }
            case EV_MOUSE_WHEEL:
            {
                handled = wnd->OnMouseWheel(state, lc);
                break;
            }
            default:
                break;
            }

            // Pressing any button outside the open combo box closes it.
            bool const isButton =
                ev.m_eventType == EV_MOUSE_LBTN || ev.m_eventType == EV_MOUSE_RBTN || ev.m_eventType == EV_MOUSE_MBTN;
            if (isButton && state && m_wndOpenedComboBox && IsWndAlive(m_wndOpenedComboBox, -1) && IsWndAlive(wnd, -1) &&
                wnd != m_wndOpenedComboBox && !wnd->IsChildOf(m_wndOpenedComboBox))
            {
                m_wndOpenedComboBox->Close();
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
            if (!m_wndModalStack.empty())
            {
                return m_wndModalStack.back();
            }
            return nullptr;
        }

        int WndStation::DispatchPaint(Wnd* curWnd, BoundsBase<float> const& clipTo)
        {
            // RVA 0x591250 - paints a window, then its children clipped to it,
            // then whatever the window draws over its children.
            auto& renderer = *M3D_APP->m_renderer;
            BoundsBase<float> childBounds = clipTo;
            DrawInfo info{};
            curWnd->OnTick(M3D_KERNEL->GetTimer().GetCurTimeUnscaled(), M3D_KERNEL->GetTimer().GetLastFrameTimeUnscaled());

            // Evaluated again after the children, as the shipped code does.
            auto const isDrawable = [curWnd] {
                return !curWnd->m_bounds.Empty() && (curWnd->m_style & WS_NODRAW) == 0 &&
                       (curWnd->m_style & WS_IS_VISIBLE) != 0;
            };
            if (isDrawable())
            {
                info.m_originalRect = curWnd->ToScreen(curWnd->m_bounds.SizeRect());
                info.m_clippedRect = clipTo.Intersect(info.m_originalRect);
                info.m_clientRect = curWnd->ToScreen(curWnd->GetClientBounds());
                info.m_clientClippedRect = clipTo.Intersect(info.m_clientRect);

                if (!info.m_clippedRect.Empty())
                {
                    renderer.SetWhiteTexture(0);
                    renderer.SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
                    renderer.SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
                    renderer.DisableTextureStages(1);
                    info.m_wndDest = curWnd;
                    curWnd->OnPaint(info);
                    GetGfxServer()->FlushWindow(curWnd);
                }
                childBounds = info.m_clippedRect;
            }

            // Children are collected in list order and painted from the last one
            // back, so the first child ends up on top.
            retruxx::vector<Wnd*> drawReversed;
            for (Object* child = curWnd->GetFirstChild(); child; child = child->GetNextSibling())
            {
                drawReversed.push_back(static_cast<Wnd*>(child));
            }
            for (auto it = drawReversed.rbegin(); it != drawReversed.rend(); ++it)
            {
                DispatchPaint(*it, childBounds);
            }

            if (isDrawable() && !info.m_clippedRect.Empty())
            {
                renderer.SetWhiteTexture(0);
                renderer.SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
                renderer.SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
                renderer.DisableTextureStages(1);
                curWnd->OnPaintOverChildren(info);
            }
            return 1;
        }

        int WndStation::CheckForMouseClick(Wnd* w, bool set, PointBase<float> const* pt)
        {
            // RVA 0x592E30 - the second half of the click machine: the press and
            // release are recorded by "set", and the click is only delivered once
            // the double-click window has passed without a second press.
            static unsigned clickTime = g_Kernel->GetTimer().GetCurTimeUnscaled();
            static PointBase<float> savePt{0.0f, 0.0f};
            static bool isSet = false;

            if (!w)
            {
                return 0;
            }
            if (w != m_wndCandidateForDblClick)
            {
                return 0;
            }
            if (!m_wndCandidateForDblClick)
            {
                isSet = false;
                return 0;
            }

            if (set)
            {
                if (pt)
                {
                    clickTime = g_Kernel->GetTimer().GetCurTimeUnscaled();
                    savePt = *pt;
                    isSet = true;
                }
                return 0;
            }

            if (!isSet)
            {
                return 0;
            }
            auto const now = g_Kernel->GetTimer().GetCurTimeUnscaled();
            if (now - clickTime <= static_cast<unsigned>(M3D_ENGINE_CFG.m_ui_dblClickDelay.GetI()))
            {
                return 0;
            }

            isSet = false;
            w->OnMouseClick(savePt);
            if (w == this)
            {
                // A click on the station itself is reported as an event instead.
                Event clickEvent;
                clickEvent.m_timeStamp = g_Kernel->GetTimer().GetCurTimeUnscaled() * 0.001;
                clickEvent.m_eventType = EV_MOUSE_CLICK;
                clickEvent.m_ushortEv[0] = static_cast<unsigned short>(savePt.x);
                clickEvent.m_ushortEv[1] = static_cast<unsigned short>(savePt.y);
                OnEvent(clickEvent);
            }
            return 1;
        }

        Wnd* WndStation::GetWndForMousePoint(Wnd* curWnd, PointBase<float> const& pt, bool affectAll)
        {
            // RVA 0x590040 - depth-first search for the innermost window under the
            // point. Hidden children are skipped unless affectAll is set, and without
            // affectAll a transparent or disabled hit counts as no hit.
            Wnd* res = curWnd;
            for (Object* child = curWnd->GetFirstChild(); child; child = child->GetNextSibling())
            {
                auto* const wnd = static_cast<Wnd*>(child);
                if ((wnd->IsVisible() || affectAll) && wnd->IsPtInBounds(pt))
                {
                    if (Wnd* const hit = GetWndForMousePoint(wnd, pt, affectAll))
                    {
                        return hit;
                    }
                }
            }
            if (!affectAll && (res->GetStyle() & (WS_TRANSPARENT | WS_DISABLE)) != 0)
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

        int WndStation::PulseKeyForWindow(Wnd* w, unsigned short key, unsigned char scanCode)
        {
            // RVA 0x58CBE0 - a synthetic press followed immediately by a release.
            w->OnKey(key, scanCode, 1);
            w->OnKey(key, scanCode, 0);
            return 1;
        }

        Wnd* WndStation::GetCapture() const
        {
            return m_wndMouseCapture;
        }

        Wnd* WndStation::GetWndMouseOver()
        {
            // RVA 0x58CAF0
            return m_wndMouseOver;
        }

        void WndStation::EnableAnimation(bool bEnable)
        {
            // RVA 0x58CC70
            m_bAnimationEnabled = bEnable;
        }

        int WndStation::Activate(Wnd* wnd)
        {
            // RVA 0x58EB60
            auto wndToActive = wnd;
            if (!wndToActive)
            {
                wndToActive = this;
            }
            if (wndToActive == m_wndActive)
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

        Wnd* WndStation::CaptureMouse(Wnd* wnd)
        {
            // RVA 0x58CB30 - hands back whoever held the capture before.
            auto* prev = m_wndMouseCapture;
            m_wndMouseCapture = wnd;
            return prev;
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

        int WndStation::CheckForMouseDblClick(Wnd* w, PointBase<float> const& pt, unsigned mouseState,
                                              PointBase<float>& prevClickPt)
        {
            // RVA 0x593210 - press / release / press on the same window inside the
            // double-click delay makes a double click; anything else restarts the
            // sequence. Returns 1 exactly on the closing press.
            static PointBase<float> firstClickCoords{-1.0f, -1.0f};
            static unsigned firstClickTime = 0;
            static DblClickState clickState = DBLCLICK_NOTHING;

            if (!w)
            {
                return 0;
            }

            auto const startSequence = [&]()
            {
                firstClickTime = g_Kernel->GetTimer().GetCurTimeUnscaled();
                clickState = DBLCLICK_PRESS;
                firstClickCoords = pt;
            };
            auto const resetSequence = [&]()
            {
                firstClickTime = 0;
                clickState = DBLCLICK_NOTHING;
                m_wndCandidateForDblClick = nullptr;
                firstClickCoords.x = -1.0f;
                firstClickCoords.y = -1.0f;
            };

            if (w != m_wndCandidateForDblClick)
            {
                if (mouseState)
                {
                    m_wndCandidateForDblClick = w;
                    startSequence();
                }
                else
                {
                    resetSequence();
                }
                prevClickPt = firstClickCoords;
                return 0;
            }

            auto const now = g_Kernel->GetTimer().GetCurTimeUnscaled();
            if (now - firstClickTime > static_cast<unsigned>(M3D_ENGINE_CFG.m_ui_dblClickDelay.GetI()))
            {
                // Too slow: this press, if any, becomes the start of a new pair.
                if (mouseState)
                {
                    startSequence();
                }
                else
                {
                    resetSequence();
                }
                prevClickPt = firstClickCoords;
                return 0;
            }

            if (clickState == DBLCLICK_PRESS)
            {
                if (!mouseState)
                {
                    clickState = DBLCLICK_PRESS_RELEASE;
                    CheckForMouseClick(m_wndCandidateForDblClick, true, &pt);
                }
                else
                {
                    firstClickTime = g_Kernel->GetTimer().GetCurTimeUnscaled();
                    firstClickCoords = pt;
                }
                prevClickPt = firstClickCoords;
                return 0;
            }

            if (clickState != DBLCLICK_PRESS_RELEASE)
            {
                prevClickPt = firstClickCoords;
                return 0;
            }

            clickState = DBLCLICK_NOTHING;
            firstClickTime = 0;
            if (mouseState)
            {
                prevClickPt = firstClickCoords;
                m_wndCandidateForDblClick = nullptr;
                firstClickCoords.x = -1.0f;
                firstClickCoords.y = -1.0f;
                return 1;
            }
            m_wndCandidateForDblClick = nullptr;
            firstClickCoords.x = -1.0f;
            firstClickCoords.y = -1.0f;
            prevClickPt = firstClickCoords;
            return 0;
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
            // RVA 0x58E9A0 - the station is not an ordinary window: the shipped
            // build raises a system error here and returns success anyway. Use
            // Create(stringsName, schemaName) instead.
            SYS_ERROR("0");
            return 1;
        }

        void WndStation::StopAllAnimations()
        {
            // Process children using iterative DFS
            std::vector<m3d::Object*> stack;
            stack.push_back(this);

            while (!stack.empty())
            {
                auto* current = stack.back();
                stack.pop_back();

                // Process all siblings of the current node
                auto* sibling = dynamic_cast<Wnd*>(current->GetFirstChild());
                while (sibling)
                {
                    sibling->StopAnimation(true);
                    // If this sibling has children, add to stack for processing
                    if (sibling->GetFirstChild())
                    {
                        stack.push_back(sibling->GetFirstChild());
                    }

                    // Move to next sibling
                    sibling = dynamic_cast<Wnd*>(sibling->GetNextSibling());
                }
            }
        }

        Wnd* WndStation::GetFocus() const
        {
            // RVA 0x58CB60
            return m_wndKbdCapture;
        }

        int WndStation::ProcessEvent(Event const& event)
        {
            // RVA 0x593DD0 - the station's event entry point. Raw input is handed to the
            // dispatchers, which route it to the focused or hovered window; everything else is
            // the UI talking to itself about modal windows, tooltips, animations and combos.
            int handled = 0;

            switch (event.m_eventType)
            {
            case EV_DISPLAY_CHANGED:
                ForEachChild(this, &Wnd::OnDisplayChanged);
                // NOTE: reported as unhandled even though every window was just notified, so
                // the event carries on to the rest of the application.
                break;

            case EV_LOOSING_FOCUS:
            case EV_OBTAINED_FOCUS:
                // Swallowed without doing anything: the station keeps its focus across an
                // application focus change.
                handled = 1;
                break;

            case EV_KEY_DOWN:
            case EV_KEY_UP:
                handled = DispatchKey(event);
                break;

            case EV_MOUSE_MOVE:
            case EV_MOUSE_LBTN:
            case EV_MOUSE_RBTN:
            case EV_MOUSE_MBTN:
            case EV_MOUSE_WHEEL:
                // EV_MOUSE_DBLCLICK and EV_MOUSE_CLICK are deliberately absent: the station
                // produces those itself out of the button events and posts them outwards, so
                // they are results of dispatching rather than input to it.
                handled = DispatchMouse(event);
                break;

            case EV_JOYSTICK_BTN0:
            case EV_JOYSTICK_BTN1:
            case EV_JOYSTICK_BTN2:
            case EV_JOYSTICK_BTN3:
            case EV_JOYSTICK_BTN4:
            case EV_JOYSTICK_BTN5:
            case EV_JOYSTICK_BTN6:
            case EV_JOYSTICK_BTN7:
            case EV_JOYSTICK_BTN8:
            case EV_JOYSTICK_BTN9:
                // NOTE: only the ten buttons reach the UI - the axis events fall through to
                // the default case and are dropped, so the joystick cannot move the cursor.
                handled = DispatchJoystick(event);
                break;

            case EV_UI_CLOSE_MODAL_WND:
            {
                auto* modalWnd = static_cast<ModalWnd*>(event.m_void[0]);
                if (modalWnd->CanClose())
                {
                    EndModal(modalWnd, event.m_uintEv[1]);
                }
                // NOTE: counted as handled even when the window refused to close.
                handled = 1;
                break;
            }

            case EV_UI_NOTIFY_WND:
            {
                auto* sender = static_cast<Wnd*>(event.m_void[0]);
                auto* receiver = static_cast<Wnd*>(event.m_void[1]);
                auto msg = event.m_uintEv[2];

                // The notification takes its own copy of the parameter, released again on the
                // way out of this case (the shipped build calls AIParam::Detach here).
                AIParam data(event.m_aiParamEv);

                // Either window may have been destroyed between posting and now.
                if (IsWndAlive(sender, -1) && IsWndAlive(receiver, -1))
                {
                    receiver->OnWndNotify(sender, sender->m_id, msg, data);
                    handled = 1;
                }
                break;
            }

            case EV_UI_END_WND_ANIMATION:
                OnEndAnimation(static_cast<Wnd*>(event.m_void[0]));
                handled = OnEvent(event);
                break;

            case EV_UI_MODAL_WND_IS_CLOSED:
            case EV_KEYBINDINGS_CHANGED:
                // Nothing for the station to do; passed straight out to the listeners.
                handled = OnEvent(event);
                break;

            case EV_UI_STOP_ALL_WND_ANIMATIONS:
                StopAllAnimations();
                handled = 0;
                break;

            case EV_INSERT_CREATED_TOOLTIP:
            {
                RemoveCurrentTooltip();

                auto* wnd = static_cast<Wnd*>(event.m_void[0]);
                if (IsWndAlive(wnd, -1))
                {
                    m_wndForTooltip = wnd;
                    if (wnd->m_toolTipWnd)
                    {
                        // Re-parented to the station and lifted to the front so that it draws
                        // over everything else.
                        GetStation()->AddChild(m_wndForTooltip->m_toolTipWnd);
                        GetStation()->MoveChildToFirstPosition(m_wndForTooltip->m_toolTipWnd);
                    }
                }
                break;
            }

            case EV_REMOVE_TOOLTIP:
                RemoveCurrentTooltip();
                break;

            case EV_UI_COMBO_OPENED:
                OnOpenComboBox(static_cast<ComboBoxWnd*>(event.m_void[0]));
                handled = 1;
                break;

            case EV_UI_COMBO_CLOSED:
                OnCloseComboBox(static_cast<ComboBoxWnd*>(event.m_void[0]));
                handled = 1;
                break;

            default:
                break;
            }

            // NOTE: EV_USER and above is far past every label above, so a user event always
            // lands in the default case. Its OnEvent result is dropped on the floor here and
            // ProcessEvent reports every user event as unhandled.
            if (event.m_eventType >= EV_USER)
            {
                OnEvent(event);
            }
            return handled;
        }

        Wnd* WndStation::GetWndByUniqueId(int uniqueId) const
        {
            // RVA 0x591B80
            Wnd* wnd = nullptr;
            if (m_allWindowsById.getValueByKey(static_cast<unsigned>(uniqueId), wnd))
            {
                return wnd;
            }
            return nullptr;
        }

        void WndStation::EndModal(ModalWnd* wnd, unsigned toRet)
        {
            // RVA 0x5900B0
            if (!m_wndModalStack.empty())
            {
                // A copy, not a reference: the slot is popped below.
                ModalWnd* const wndFromStack = m_wndModalStack.back();
                if (wnd == wndFromStack)
                {
                    wndFromStack->OnCloseModal(toRet);
                    m_wndModalStack.pop_back();
                    m_wndModalRetVal = toRet;
                    CaptureMouse(nullptr);
                    RemoveCurrentTooltip();
                    // The dialog's own flag, set by DoModal; removing it lets
                    // OnRemoveWnd hand activation and keyboard focus back.
                    if (wndFromStack->m_modalAttachedToStation)
                    {
                        RemoveChild(wndFromStack);
                    }
                    M3D_APP->FinishExclusiveMsgLoop();
                }
            }
        }

        Wnd* WndStation::GetActive() const
        {
            // RVA 0x58CB70
            return m_wndActive;
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

        void WndStation::OnEndAnimation(Wnd* wnd)
        {
            // RVA 0x591CA0 - a window whose unlink was suspended until its
            // animation ended is detached once neither it nor any descendant is
            // still animating. The walk continues up the parent chain, so suspended
            // ancestors waiting on this window are released too; it stops at the
            // first suspended window that still has an animating descendant.
            if (!IsWndAlive(wnd, -1))
            {
                return;
            }

            for (Object* cur = wnd; cur != nullptr;)
            {
                Object* const parent = cur->GetParent();
                if (IS_KIND_OF(cur, Wnd) && static_cast<Wnd*>(cur)->m_bSuspendedUnlink)
                {
                    bool canRemove = true;
                    retruxx::vector<Object*> stack;
                    stack.push_back(cur);
                    while (canRemove && !stack.empty())
                    {
                        Object* const obj = stack.back();
                        stack.pop_back();
                        for (Object* child = obj->GetFirstChild(); child; child = child->GetNextSibling())
                        {
                            if (IS_KIND_OF(child, Wnd) && static_cast<Wnd*>(child)->IsAnimatingNow())
                            {
                                canRemove = false;
                                break;
                            }
                            if (child->GetFirstChild())
                            {
                                stack.push_back(child);
                            }
                        }
                    }

                    if (!canRemove)
                    {
                        return;
                    }
                    if (parent && IS_KIND_OF(parent, Wnd))
                    {
                        static_cast<Wnd*>(parent)->RemoveChildForce(cur);
                    }
                }
                cur = parent;
            }
        }

        bool WndStation::IsAnimationEnabled() const
        {
            return m_bAnimationEnabled;
        }

        CStr WndStation::InitializeStringUsingIds(CStr const& src)
        {
            // RVA 0x665660 - replaces each ^id^ with the string of that id.
            // NOTE: CStr::find returns positions relative to where the search starts. Both are used correctly
            // for the first marker, but a later search starts past the previous one and its relative result
            // is used as if it were absolute, so any marker after the first is mangled.
            if (src.empty())
            {
                return CStr("");
            }
            CStr accum;
            int const len = src.length();
            int pos = 0;
            while (true)
            {
                int const pos1 = src.find('^', pos);
                if (pos1 < 0)
                {
                    accum += src.substr(pos);
                    break;
                }
                int const idStart = pos1 + 1;
                int const idLen = src.find('^', idStart);
                if (idLen < 0)
                {
                    accum += src.substr(pos);
                    break;
                }
                CStr const id = src.substr(idStart, idLen);
                CStr dest;
                GetStringByStringId(dest, id);
                accum += src.substr(pos, pos1 - pos) + dest;
                pos = pos1 + idLen + 2;
                if (pos >= len)
                {
                    break;
                }
            }
            return accum;
        }

        WndStation::~WndStation()
        {
            m_wndStation = nullptr;
        }

        int WndStation::OnRemoveWnd(Wnd* parent, Wnd* wnd)
        {
            // RVA 0x592A90 - drops every reference the station holds to a window
            // (or to one of its descendants) that is being removed.
            bool const isModal = wnd->IsKindOf(RT_CLASS_LOCAL(ModalWnd));
            if (isModal)
            {
                M3D_ASSERT(!IsModal(RT_DYNCAST(wnd, ModalWnd)));
            }

            if (wnd == m_wndActive || m_wndActive->IsChildOf(wnd))
            {
                if (isModal)
                {
                    Activate(m_wndModalStack.empty() ? nullptr : m_wndModalStack.back());
                }
                else
                {
                    Activate(parent);
                }
            }
            if (isModal)
            {
                Application::g_pApp->EnqueueMessage(41, reinterpret_cast<int>(wnd), 0, 0, 0, {}, {});
            }
            if (m_wndKbdCapture && (wnd == m_wndKbdCapture || m_wndKbdCapture->IsChildOf(wnd)))
            {
                m_wndKbdCapture->OnLoosingFocus();
                m_wndKbdCapture = this;
                OnObtainingFocus();
            }
            if (m_wndMouseCapture && (wnd == m_wndMouseCapture || m_wndMouseCapture->IsChildOf(wnd)))
            {
                CaptureMouse(nullptr);
            }
            if (wnd == m_wndMouseOver || m_wndMouseOver->IsChildOf(wnd))
            {
                m_wndMouseOver->OnMouseOut();
                m_wndMouseOver = parent;
            }
            if (m_wndForTooltip && (wnd == m_wndForTooltip || m_wndForTooltip->IsChildOf(wnd)))
            {
                RemoveCurrentTooltip();
            }
            if (m_wndCandidateForDblClick &&
                (wnd == m_wndCandidateForDblClick || m_wndCandidateForDblClick->IsChildOf(wnd)))
            {
                m_wndCandidateForDblClick = nullptr;
            }
            if (wnd->IsKindOf(RT_CLASS_LOCAL(ComboBoxWnd)) && wnd == m_wndOpenedComboBox)
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

        int WndStation::DispatchJoystick(Event const& event)
        {
            // RVA 0x58CB00 - joystick input is not a window concern; it goes
            // straight to the impulse layer.
            Application::g_pApp->m_pImpulses->HandleKeyboardMouseEvent(event, this);
            return 1;
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

        void WndStation::OnOpenComboBox(ComboBoxWnd* combo)
        {
            if (!combo || m_wndOpenedComboBox == combo)
            {
                return;
            }

            int val = -1;
            auto const res = m_allWindows.getValueByKey(reinterpret_cast<unsigned>(combo), val);
            if (res)
            {
                if (combo->IsChildOf(this))
                {
                    if (m_wndOpenedComboBox && IsWndAlive(m_wndOpenedComboBox, -1) && m_wndOpenedComboBox->IsChildOf(this) &&
                        m_wndOpenedComboBox->IsOpen())
                    {
                        m_wndOpenedComboBox->Close();
                    }
                    m_wndOpenedComboBox = combo;
                }
            }
        }

        void WndStation::ForEachChild(Wnd* curWnd, void (Wnd::*fn)())
        {
            // RVA 0x58DCD0 - pre-order walk of the whole subtree.
            (curWnd->*fn)();
            for (auto* child = RT_DYNCAST(curWnd->GetFirstChild(), Wnd); child;
                 child = RT_DYNCAST(child->GetNextSibling(), Wnd))
            {
                ForEachChild(child, fn);
            }
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
            // RVA 0x58CC20 - the one graphics server is made on first use.
            if (!gfxserver)
            {
                gfxserver = new GfxServer;
            }
            return gfxserver;
        }

        void WndStation::UnregisterWnd(Wnd* w)
        {
            // RVA 0x5943B0
            if (w && w->m_uniqueId != -1)
            {
                m_allWindowsById.removeByKey(w->m_uniqueId);
                m_allWindows.removeByKey(reinterpret_cast<unsigned>(w));
                w->m_uniqueId = -1;
            }
        }

        int WndStation::DispatchKey(Event const& ev)
        {
            int res = 0;
            if (m_wndKbdCapture)
            {
                auto wnd = ModalOverride(m_wndKbdCapture);
                auto parent = wnd;
                while (parent)
                {
                    res = parent->OnKey(ev.m_ushortEv[0], ev.m_byteEv[3], ev.m_eventType == 7);
                    if (res)
                    {
                        break;
                    }
                    parent = dynamic_cast<Wnd*>(parent->GetParent());
                }
                if (this == m_wndKbdCapture)
                {
                    if (m_wndModalStack.empty())
                    {
                        res = OnEvent(ev);
                    }
                }
                M3D_APP->m_pImpulses->HandleKeyboardMouseEvent(ev, wnd);
            }
            return res;
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
            // RVA 0x5916A0 - paints the whole window tree, refreshes the mouse-over
            // window and tooltip, then draws the cursor (as a hardware cursor when
            // enabled, otherwise as a sprite).
            auto& renderer = *Application::g_pApp->m_renderer;
            renderer.PushZbState(rend::ZB_DISABLE);
            renderer.PushLighting(false);
            renderer.PushBlend(rend::BM_NONE);
            DispatchPaint(this, m_bounds);

            Wnd* target = m_wndMouseCapture;
            if (!target)
            {
                target = FindWndUnderPoint(*this, m_prevMouseCoord);
            }
            Wnd* const wnd = ModalOverride(target);
            if (wnd != nullptr)
            {
                UpdateOnMouseInOut(wnd);
            }
            if (m_wndForTooltip != nullptr && wnd != m_wndForTooltip)
            {
                RemoveCurrentTooltip();
            }

            assert(m_curDefault);
            Cursor const oldCursor = m_currentCursor;
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
                        renderer.UpdateDXCursorFrame();
                    }
                    else
                    {
                        // The hot spot is rounded to nearest (a bare fistp), not truncated.
                        renderer.SetupDXCursor(m_currentCursor.m_tex, static_cast<int>(lrintf(m_currentCursor.m_spot.x)),
                                               static_cast<int>(lrintf(m_currentCursor.m_spot.y)), 0);
                    }
                    renderer.ShowDXCursor(true);
                }
                else
                {
                    renderer.SetBlend(rend::BM_ALPHA, false);
                    renderer.SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
                    renderer.SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
                    renderer.SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
                    renderer.SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
                    renderer.SetTexture(0, m_currentCursor.m_tex, -1.0);
                    auto mouseX = static_cast<float>(Application::g_pApp->GetMouseX());
                    auto mouseY = static_cast<float>(Application::g_pApp->GetMouseY());
                    renderer.AbsToRel(mouseX, mouseY);
                    float const left = mouseX - m_currentCursor.m_spot.x;
                    float const top = mouseY - m_currentCursor.m_spot.y;
                    Application::g_pApp->PutSpriteRel(left, top, m_currentCursor.m_sz.x + left,
                                                      m_currentCursor.m_sz.y + top, 0xFFFFFFFF);
                }
            }
            else if (Application::g_pApp->IsDXCursorEnabled())
            {
                renderer.ShowDXCursor(false);
            }
            renderer.PopBlend();
            renderer.PopZbState();
            renderer.PopLighting();
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
                if (m_wndKbdCapture && m_wndKbdCapture != m_wndMouseOver && m_wndKbdCapture != newWnd &&
                    !m_wndKbdCapture->IsChildOf(newWnd))
                {
                    m_wndKbdCapture->OnMouseOut();
                }
                newWnd->OnMouseIn();
                m_wndMouseOver = newWnd;
            }
        }
    }  // namespace ui
}  // namespace m3d

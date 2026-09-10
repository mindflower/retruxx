#include <ui/tabwnd.h>

#include <core/aiparam.h>
#include <core/kernel.h>
#include <ui/frame.h>
#include <ui/glyphbutton.h>
#include <ui/ui_srv.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(TabWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(TabWnd);

        namespace
        {
            // Tab buttons are given ids starting here, so the button that fired a
            // notification identifies its tab by id - BTN_ID_BASE.
            unsigned const BTN_ID_BASE = 500;

            // CallParentNotify messages the tab strip raises around a selection
            // change; both carry {oldIdx, newIdx, selectionType} as an id list.
            unsigned const MSG_AFTER_CHANGE_CUR_SEL = 5;
            unsigned const MSG_BEFORE_CHANGE_CUR_SEL = 6;
        }  // namespace

        // ===================================================================
        //  TabItemInfo
        // ===================================================================

        TabItemInfo::TabItemInfo() : m_image(), m_imageDisabled(), m_data(0), m_bCenterX(false), m_bCenterY(false)
        {
            // RVA 0x6FBD60 - empty caption and description, both icons invalid.
        }

        TabItemInfo::TabItemInfo(
            CStr const& caption, CStr const& diz, rend::TexHandle image, rend::TexHandle imageDisabled, int data) :
            m_caption(caption),
            m_diz(diz),
            m_image(image),
            m_imageDisabled(imageDisabled),
            m_data(data),
            m_bCenterX(false),
            m_bCenterY(false)
        {
            // RVA 0x6FBDA0
        }

        TabItemInfo::TabItemInfo(TabItemInfo const& rhs) :
            m_caption(rhs.m_caption),
            m_diz(rhs.m_diz),
            m_image(rhs.m_image),
            m_imageDisabled(rhs.m_imageDisabled),
            m_data(rhs.m_data),
            m_bCenterX(rhs.m_bCenterX),
            m_bCenterY(rhs.m_bCenterY)
        {
        }

        TabItemInfo& TabItemInfo::operator=(TabItemInfo const& rhs)
        {
            // RVA 0x6FB740
            m_caption = rhs.m_caption;
            m_diz = rhs.m_diz;
            m_image = rhs.m_image;
            m_imageDisabled = rhs.m_imageDisabled;
            m_bCenterX = rhs.m_bCenterX;
            m_bCenterY = rhs.m_bCenterY;
            return *this;
        }

        void TabItemInfo::Set(
            CStr const& caption, CStr const& diz, rend::TexHandle image, rend::TexHandle imageDisabled, int data)
        {
            // RVA 0x6FB8E0 - note the two centring flags are deliberately left
            // alone, unlike operator= which carries them across.
            m_caption = caption;
            m_diz = diz;
            m_image = image;
            m_imageDisabled = imageDisabled;
            m_data = data;
        }

        // ===================================================================
        //  TabItem
        // ===================================================================

        TabItem::TabItem(ref_ptr<Wnd> workWnd, TabItemInfo const& info, int idx) :
            m_idx(idx),
            m_workWnd(workWnd),
            m_info(info)
        {
        }

        TabItem::TabItem(TabItem const& rhs) : m_idx(rhs.m_idx), m_workWnd(rhs.m_workWnd), m_info(rhs.m_info)
        {
        }

        TabItem::~TabItem()
        {
            // RVA 0x6FB780 - a tab that is going away detaches its work window
            // from the tab control, but only if that is still its parent.
            if (m_workWnd)
            {
                auto* parent = RT_DYNCAST(m_workWnd->GetParent(), Wnd);
                if (parent && parent->IsKindOf(&TabWnd::m_classTabWnd))
                {
                    parent->RemoveChild(m_workWnd.get());
                }
            }
        }

        ref_ptr<Wnd> TabItem::GetWorkWnd() const
        {
            // RVA 0x6FB830 - hands out a counted reference to the tab's work
            // window.
            return m_workWnd;
        }

        void TabItem::SetWorkWnd(ref_ptr<Wnd> workWnd)
        {
            m_workWnd = workWnd;
        }

        TabItemInfo const& TabItem::GetInfo() const
        {
            // RVA 0x6FAD40
            return m_info;
        }

        void TabItem::SetInfo(TabItemInfo const& info)
        {
            // RVA 0x6FB8A0 - copies every field including the centring flags.
            m_info.m_caption = info.m_caption;
            m_info.m_diz = info.m_diz;
            m_info.m_image = info.m_image;
            m_info.m_imageDisabled = info.m_imageDisabled;
            m_info.m_data = info.m_data;
            m_info.m_bCenterX = info.m_bCenterX;
            m_info.m_bCenterY = info.m_bCenterY;
        }

        // ===================================================================
        //  TabButtonInfo
        // ===================================================================

        TabButtonInfo::TabButtonInfo()
        {
            // RVA 0x6FAD50. NOTE: m_alignment is deliberately not initialised
            // here - the shipped ctor writes the five floats and m_drawStyle and
            // stops, so a default-constructed TabButtonInfo carries a garbage
            // alignment until SetNormal / SetIzvrat sets one.
            m_minWidth = 0.0f;
            m_maxWidth = 0.0f;
            m_height = 0.0f;
            m_spaceBetweenButtons = 0.0f;
            m_glyphSz = 0.0f;
            m_drawStyle = DRAWSTYLE_NORMAL;
        }

        void TabButtonInfo::SetNormal(float minW, float maxW, float h, float space, Alignment alignment, float glyphSz)
        {
            // RVA 0x6FAD80
            m_minWidth = minW;
            m_maxWidth = maxW;
            m_height = h;
            m_spaceBetweenButtons = space;
            m_glyphSz = glyphSz;
            m_drawStyle = DRAWSTYLE_NORMAL;
            m_alignment = alignment;
        }

        void TabButtonInfo::SetIzvrat(float w, float h, float space, Alignment alignment)
        {
            // RVA 0x6FADD0 - a fixed-width strip: min and max width are the same
            // and there is no glyph scaling.
            m_minWidth = w;
            m_maxWidth = w;
            m_height = h;
            m_spaceBetweenButtons = space;
            m_glyphSz = 0.0f;
            m_drawStyle = DRAWSTYLE_IZVRAT;
            m_alignment = alignment;
        }

        // ===================================================================
        //  TabWnd
        // ===================================================================

        TabWnd::AuxInfo::AuxInfo()
        {
            // RVA 0x6FACB0
            m_dizSpace = 5.0f;
            m_clientEdge = 10.0f;
            m_btnClientEdge = 5.0f;
        }

        Class* TabWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Class* TabWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(TabWnd);
        }

        Object* TabWnd::CreateObject()
        {
            return new TabWnd;
        }

        Object* TabWnd::Clone()
        {
            // RVA 0x6FCAD0 - runs the copy ctor, which starts an empty tab strip
            // rather than duplicating the tabs.
            return new TabWnd(*this);
        }

        TabWnd::TabWnd() : m_curSel(-1)
        {
            // RVA 0x6FCFC0 - the strip picks up the theme's button metrics.
            TabButtonInfo info;
            info.m_minWidth = m_gfx->GetTabButtonMinWidth();
            info.m_maxWidth = m_gfx->GetTabButtonMaxWidth();
            info.m_height = m_gfx->GetTabButtonHeight();
            info.m_spaceBetweenButtons = m_gfx->GetTabButtonSpace();
            info.m_drawStyle = TabButtonInfo::DRAWSTYLE_NORMAL;
            info.m_alignment = TabButtonInfo::ALIGNMENT_LEFT;
            info.m_glyphSz = m_gfx->GetGlyphHeight();
            SetTabButtonInfo(info);
        }

        TabWnd::TabWnd(TabWnd const&) : m_curSel(-1)
        {
            // RVA 0x6FC860 - copies nothing: the tab and button vectors start
            // empty and m_buttonInfo is left at its default-constructed state
            // rather than being taken from rhs or from the theme.
            m_buttonInfo = TabButtonInfo();
        }

        TabWnd::~TabWnd()
        {
            // RVA 0x6FCB00
            RemoveAllItems();
        }

        int TabWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            // RVA 0x6FACE0 - the strip has to be able to notify its parent,
            // which is how OnBefore/OnAfterChangeCurSel reach the owning window.
            if (!CreateWnd(caption, style, rc, id))
            {
                return 0;
            }
            m_style |= WS_SEND_NOTIFY_MESSAGES;
            return 1;
        }

        // ---- items ---------------------------------------------------------

        int TabWnd::GetItemsCount() const
        {
            // RVA 0x6FB2F0
            return static_cast<int>(m_items.size());
        }

        bool TabWnd::IsItemIdValid(int idx) const
        {
            // RVA 0x6FB490
            return idx >= 0 && idx < static_cast<int>(m_items.size());
        }

        TabItem const* TabWnd::GetItem(int idx) const
        {
            // RVA 0x6FBB40
            return IsItemIdValid(idx) ? m_items[idx] : nullptr;
        }

        int TabWnd::GetCurSel() const
        {
            // RVA 0x6FAD20
            return m_curSel;
        }

        TabButtonInfo const& TabWnd::GetTabButtonInfo() const
        {
            // RVA 0x6FAD30
            return m_buttonInfo;
        }

        int TabWnd::AddItem(ref_ptr<Wnd> workWnd, TabItemInfo const& info)
        {
            // RVA 0x6FD840 - returns the new tab's index, or -1 on failure. The
            // first tab added also becomes the selected one.
            if (!workWnd)
            {
                return -1;
            }

            int const idx = static_cast<int>(m_items.size());
            auto* item = new TabItem(workWnd, info, idx);
            m_items.push_back(item);

            if (!AddButton(idx))
            {
                delete item;
                if (!m_items.empty())
                {
                    m_items.pop_back();
                }
                return -1;
            }

            AdjustWorkWindowRect(workWnd.get(), info);
            if (idx == 0)
            {
                _SetCurSel(0, BY_PROGRAM);
            }
            return idx;
        }

        void TabWnd::RemoveAllItems()
        {
            // RVA 0x6FC900
            for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
            {
                delete m_items[i];
                m_items[i] = nullptr;
            }
            m_items.clear();

            for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
            {
                if (m_buttons[i])
                {
                    m_buttons[i]->DecRef();
                }
                m_buttons[i] = nullptr;
            }
            m_buttons.clear();
            m_curSel = -1;
        }

        // ---- selection -----------------------------------------------------

        int TabWnd::SetCurSel(int idx)
        {
            // RVA 0x6FD830
            return _SetCurSel(idx, BY_PROGRAM);
        }

        int TabWnd::_SetCurSel(int idx, SelectionType selectionType)
        {
            // RVA 0x6FD520 - swaps which tab's work window is parented in, and
            // returns the index that was selected before. A no-op selection
            // returns the current index without raising any notification.
            int const oldSel = m_curSel;
            if (!IsItemIdValid(idx) || m_curSel == idx)
            {
                return oldSel;
            }

            OnBeforeChangeCurSel(idx, selectionType);

            ref_ptr<Wnd> newWnd = m_items[idx]->GetWorkWnd();
            Wnd* oldWnd = nullptr;
            if (IsItemIdValid(m_curSel))
            {
                ref_ptr<Wnd> cur = m_items[m_curSel]->GetWorkWnd();
                oldWnd = cur.get();
                if (oldWnd != newWnd.get())
                {
                    RemoveChild(oldWnd);
                }
            }
            if (newWnd.get() != oldWnd)
            {
                AddChild(newWnd.get());
            }
            m_curSel = idx;

            OnAfterChangeCurSel(oldSel, selectionType);
            return oldSel;
        }

        void TabWnd::OnBeforeChangeCurSel(int newIdx, SelectionType selectionType)
        {
            // RVA 0x6FD0E0 - {oldIdx, newIdx, selectionType}, sent immediately.
            std::vector<int> params;
            params.push_back(m_curSel);
            params.push_back(newIdx);
            params.push_back(static_cast<int>(selectionType));
            CallParentNotify(MSG_BEFORE_CHANGE_CUR_SEL, AIParam(params), 1);
        }

        void TabWnd::OnAfterChangeCurSel(int oldIdx, SelectionType selectionType)
        {
            // RVA 0x6FD1F0 - {oldIdx, newIdx, selectionType}, queued.
            std::vector<int> params;
            params.push_back(oldIdx);
            params.push_back(m_curSel);
            params.push_back(static_cast<int>(selectionType));
            CallParentNotify(MSG_AFTER_CHANGE_CUR_SEL, AIParam(params), 0);
        }

        // ---- buttons -------------------------------------------------------

        int TabWnd::AddButton(int idx)
        {
            // RVA 0x6FCBE0 - one glyph button per tab, id'd BTN_ID_BASE + idx and
            // stripped of both activation styles so the strip never takes focus.
            if (!IsItemIdValid(idx))
            {
                return 0;
            }
            auto* btn = RT_DYNCAST(M3D_KERNEL->New("GlyphButton"), GlyphButton);
            if (!btn)
            {
                return 0;
            }

            TabItemInfo const& info = m_items[idx]->GetInfo();
            btn->CreateGlyphButton(
                0,
                BoundsBase<float>(0.0f, 0.0f, 0.0f, 0.0f),
                GlyphButton::GBTYPE_USER,
                info.m_caption,
                BTN_ID_BASE + idx,
                info.m_image,
                info.m_imageDisabled);
            btn->SetStyle(btn->GetStyle() & ~static_cast<unsigned>(WS_ACTIVATION_CAPTURES_FOCUS | WS_ACTIVATABLE));

            m_buttons.push_back(btn);
            AddChild(btn);
            return 1;
        }

        PointBase<float> TabWnd::RecalcButtonSize(int idx)
        {
            // RVA 0x6FC540 - a normal button is measured against its own caption
            // and then clamped into [minWidth, maxWidth]; an "izvrat" strip uses
            // the fixed width straight from the button info.
            PointBase<float> result(0.0f, 0.0f);
            if (!IsItemIdValid(idx))
            {
                return result;
            }

            float width = 0.0f;
            if (m_buttonInfo.m_drawStyle == TabButtonInfo::DRAWSTYLE_NORMAL)
            {
                GlyphButton* btn = m_buttons[idx];
                // The measurement must not include the frame, so WS_NOFRAME is
                // forced on for the duration of AdjustBoundsToFit.
                unsigned const style = btn->GetStyle();
                btn->SetStyle(style & ~static_cast<unsigned>(WS_NOFRAME));
                btn->SetPane(m_paneName);
                btn->AdjustBoundsToFit(m_buttonInfo.m_maxWidth);
                btn->SetStyle(style);

                width = btn->GetBounds().width;
                if (m_buttonInfo.m_minWidth > width)
                {
                    width = m_buttonInfo.m_minWidth;
                }
                if (width > m_buttonInfo.m_maxWidth)
                {
                    width = m_buttonInfo.m_maxWidth;
                }
            }
            else if (m_buttonInfo.m_drawStyle == TabButtonInfo::DRAWSTYLE_IZVRAT)
            {
                width = m_buttonInfo.m_minWidth;
            }
            else
            {
                return result;
            }

            result.x = width;
            result.y = m_buttonInfo.m_height;
            return result;
        }

        PointBase<float> TabWnd::RecalcButtonPos(int idx)
        {
            // RVA 0x6FC3D0 - buttons are laid end to end from whichever side the
            // alignment names, starting inside the pane's rounded corner.
            PointBase<float> result(0.0f, 0.0f);
            if (!IsItemIdValid(idx))
            {
                return result;
            }

            float cornerSz = 0.0f;
            if ((m_paneFlags & 2) != 0)
            {
                if (Pane* pane = m_gfx->GetPane(m_paneName))
                {
                    if (Frame* frame = pane->m_frame[0])
                    {
                        cornerSz = static_cast<float>(frame->m_cornerRoundSize);
                    }
                }
            }

            if (m_buttonInfo.m_alignment == TabButtonInfo::ALIGNMENT_RIGHT)
            {
                // Measured from the right: sum this button and everything after
                // it, then subtract from the strip width.
                float run = cornerSz;
                for (int i = 0; i <= idx; ++i)
                {
                    run += m_buttons[i]->GetBounds().width;
                    if (i != idx)
                    {
                        run += m_buttonInfo.m_spaceBetweenButtons;
                    }
                }
                result.x = m_bounds.width - run;
            }
            else
            {
                float run = cornerSz;
                for (int i = 0; i < idx; ++i)
                {
                    run += m_buttons[i]->GetBounds().width + m_buttonInfo.m_spaceBetweenButtons;
                }
                result.x = run;
            }
            result.y = 0.0f;
            return result;
        }

        void TabWnd::SetTabButtonInfo(TabButtonInfo const& info)
        {
            // RVA 0x6FCDE0 - re-lays the whole strip, then re-applies the client
            // edges so the top edge picks up the new button height.
            m_buttonInfo = info;

            for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
            {
                m_buttons[i]->SetGlyphHeight(info.m_glyphSz);
            }
            for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
            {
                PointBase<float> const sz = RecalcButtonSize(i);
                BoundsBase<float> b = m_buttons[i]->GetBounds();
                b.width = sz.x;
                b.height = sz.y;
                m_buttons[i]->SetBounds(b, true);
            }
            for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
            {
                PointBase<float> const pos = RecalcButtonPos(i);
                BoundsBase<float> b = m_buttons[i]->GetBounds();
                b.x0 = pos.x;
                b.y0 = pos.y;
                m_buttons[i]->SetBounds(b, true);
            }

            SetClientEdges(_GetClientEdges());
        }

        // ---- client area ---------------------------------------------------

        float TabWnd::_GetTopEdgeDiff() const
        {
            // RVA 0x6FB310 - how much room the strip steals off the top of the
            // client area. A normal strip also reserves a line for the selected
            // tab's description; an "izvrat" one only gives back the frame bar.
            float edgeDiff = m_buttonInfo.m_height;
            if (m_buttonInfo.m_drawStyle != TabButtonInfo::DRAWSTYLE_NORMAL)
            {
                if ((m_paneFlags & 2) != 0)
                {
                    if (Pane* pane = m_gfx->GetPane(m_paneName))
                    {
                        if (Frame* frame = pane->m_frame[0])
                        {
                            return edgeDiff - static_cast<float>(frame->m_barUsedWidth);
                        }
                    }
                }
                return edgeDiff;
            }
            // "A" stands in for a line of text at the current font.
            float const lineH = m_gfx->MeasureText(CStr("A"), m_defFont, TW_NOWRAP, m_bounds.width).y;
            return m_aif.m_dizSpace * 2.0f + lineH + edgeDiff;
        }

        std::vector<float, std::allocator<float>> TabWnd::_GetClientEdges() const
        {
            // RVA 0x6FCB70 - the edges as the caller last set them, i.e. with the
            // strip's own top reservation taken back off.
            std::vector<float> edges = m_clientEdges;
            if (edges[1] != 0.0f)
            {
                edges[1] -= _GetTopEdgeDiff();
            }
            return edges;
        }

        void TabWnd::SetClientEdges(float left, float top, float right, float bottom)
        {
            // RVA 0x6FBB80 - the strip pushes the client area down by whatever it
            // occupies. NOTE: unlike Wnd::SetClientEdges this writes m_clientEdges
            // straight through without re-laying the children.
            float const topEdgeDiff = _GetTopEdgeDiff();
            m_clientEdges[0] = left;
            m_clientEdges[1] = topEdgeDiff + top;
            m_clientEdges[2] = right;
            m_clientEdges[3] = bottom;
        }

        void TabWnd::SetClientEdges(std::vector<float, std::allocator<float>> const& clientEdges)
        {
            // RVA 0x6FBBE0 - forwards to the four-float form, virtually.
            SetClientEdges(clientEdges[0], clientEdges[1], clientEdges[2], clientEdges[3]);
        }

        void TabWnd::AdjustWorkWindowRect(Wnd* w, TabItemInfo const& info) const
        {
            // RVA 0x6FB400 - centring is per tab and opt-in. NOTE: the horizontal
            // case centres against the whole window width while the vertical one
            // uses the client area, so a framed tab window is not centred
            // symmetrically; as shipped.
            if (!w)
            {
                return;
            }
            BoundsBase<float> workB = w->GetBounds();
            BoundsBase<float> const clientB = GetClientBounds();
            if (info.m_bCenterX)
            {
                workB.x0 = (m_bounds.width - workB.width) * 0.5f;
            }
            if (info.m_bCenterY)
            {
                workB.y0 = (clientB.height - workB.height) * 0.5f + clientB.y0;
            }
            w->SetBounds(workB, true);
        }

        // ---- painting ------------------------------------------------------

        void TabWnd::DrawWndText(DrawInfo const& di)
        {
            // RVA 0x6FBC00 - the selected tab's description is drawn just under
            // the buttons; an "izvrat" strip has no description line at all.
            if (m_buttonInfo.m_drawStyle != TabButtonInfo::DRAWSTYLE_NORMAL || !IsItemIdValid(m_curSel) ||
                !m_items[m_curSel])
            {
                return;
            }
            CStr const diz = m_items[m_curSel]->GetInfo().m_diz;
            if (diz.empty())
            {
                return;
            }

            PointBase<float> const at(GetClientBounds().x0, m_aif.m_dizSpace + m_buttonInfo.m_height);

            // The description sits in the non-client band, so it is drawn against
            // the original (unclipped-to-client) rect.
            DrawInfo ddi(di);
            ddi.m_clientRect = di.m_originalRect;
            ddi.m_clientClippedRect = di.m_clippedRect;
            m_gfx->AddText(ddi, at, diz, m_defFont, TW_NOWRAP, TF_LEFT);
        }

        void TabWnd::OnNcPaint(DrawInfo const& di, unsigned clr)
        {
            // RVA 0x6FD2F0
            BoundsBase<float> const b(0.0f, 0.0f, GetBounds().width, GetBounds().height);

            retruxx::vector<BoundsBase<float>> btnRects;
            for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
            {
                btnRects.push_back(m_buttons[i]->GetBounds());
            }

            if (m_buttonInfo.m_drawStyle == TabButtonInfo::DRAWSTYLE_IZVRAT)
            {
                // Each tab draws as its own glyph; the selected one uses the
                // enabled texture and the rest the disabled one.
                retruxx::vector<rend::TexHandle> btnImages;
                for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
                {
                    btnImages.push_back(
                        i == m_curSel ? m_buttons[i]->GetGlyphTexture() : m_buttons[i]->GetGlyphTextureDisabled());
                }
                m_gfx->AddTabWndPaneIzvrat(
                    di, b, m_curClr, m_buttonInfo, btnRects, btnImages, m_curSel, m_paneFlags, m_paneName, m_bgFlags);
            }
            else
            {
                // NOTE: the normal strip is drawn with the colour it was handed,
                // while the izvrat one above ignores it in favour of m_curClr.
                m_gfx->AddTabWndPaneNormal(
                    di, b, clr, m_buttonInfo, btnRects, m_curSel, m_paneFlags, m_paneName, m_bgFlags);
            }
        }

        // ---- input ---------------------------------------------------------

        int TabWnd::OnWndNotify(Wnd* from, unsigned idFrom, unsigned message, AIParam const& data)
        {
            // RVA 0x6FD670
            if (message == 1 && idFrom >= BTN_ID_BASE && idFrom < BTN_ID_BASE + m_items.size())
            {
                _SetCurSel(static_cast<int>(idFrom - BTN_ID_BASE), BY_MOUSE);
                return 1;
            }

            // A notification from the visible tab's own work window is always
            // passed up; anything else only if the style asks for it.
            bool fromCurWorkWnd = false;
            if (IsItemIdValid(m_curSel) && m_items[m_curSel])
            {
                ref_ptr<Wnd> curWnd = m_items[m_curSel]->GetWorkWnd();
                fromCurWorkWnd = from == curWnd.get();
            }
            if (fromCurWorkWnd)
            {
                return ReflectChildNotifyToParent(from, idFrom, message, data);
            }
            if ((m_style & 0x100000) != 0)
            {
                ReflectChildNotifyToParent(from, idFrom, message, data);
            }
            return 0;
        }

        int TabWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
        {
            // RVA 0x6FD7B0 - Tab steps forward through the tabs, Shift+Tab back.
            // NOTE: the shipped code tests the shift state out of `key` rather
            // than out of `state`, so it reads the modifier from the wrong
            // argument; kept as shipped.
            if (Wnd::OnKey(key, scanCode, state))
            {
                return 1;
            }
            if (!state || static_cast<unsigned char>(key) != 3)
            {
                return 0;
            }

            int next = -1;
            if ((key & 0x3000) != 0)
            {
                if (m_curSel > 0)
                {
                    next = m_curSel - 1;
                }
            }
            else if (m_curSel < static_cast<int>(m_items.size()) - 1)
            {
                next = m_curSel + 1;
            }
            if (next != -1)
            {
                _SetCurSel(next, BY_KB);
            }
            return 0;
        }
    }  // namespace ui
}  // namespace m3d

#include <ui/button.h>
#include <ui/frame.h>
#include <ui/scroll.h>
#include <ui/ui_srv.h>
#include <ui/wndstation.h>
#include <core/aiparam.h>

namespace m3d
{
	namespace ui
	{
        RT_CLASS_EXPORTS_BEGIN(ScrollWnd)
		RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(ScrollWnd);

        Object* ScrollWnd::CreateObject()
        {
            return new ScrollWnd;
        }

        Class* ScrollWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Object* ScrollWnd::Clone()
        {
            return new ScrollWnd(*this);
        }

        float ScrollWnd::GetCurPos() const
        {
            return m_curPos * m_maxPos;
        }

        void ScrollWnd::SetCurPos(float p)
        {
            if (m_maxPos <= 0.001)
            {
                m_curPos = 0.0;
            }
            else
            {
                m_curPos = p / m_maxPos;
            }
            if (m_curPos < 0.0)
            {
                m_curPos = 0.0;
            }
            else if (m_curPos > 1.0)
            {
                m_curPos = 1.0;
            }
        }

        void ScrollWnd::EnableWindow(bool bEnable)
        {
            if (bEnable)
                m_style &= 0xFFFFFFFD;
            else
                m_style |= 2u;
            if (m_btn0)
                m_btn0->EnableWindow(bEnable);
            if (m_btn1)
                m_btn1->EnableWindow(bEnable);
        }

        int ScrollWnd::Create(BoundsBase<float> const& rect, int vertical)
        {
            if (CreateWnd({}, m_style, rect, 0) == 0)
            {
                return 0;
            }
            m_vertical = vertical;

            m_btn0 = new ButtonWnd;
            BoundsBase<float> btnRect{0.0, 0.0, 0.0, 0.0};
            m_btn0->Create({}, 0, btnRect, 256);
            AddChild(m_btn0);

            m_btn1 = new ButtonWnd;
            m_btn1->Create({}, 0, btnRect, 0x101);
            AddChild(m_btn1);
            SetScrollPane(m_scrollPaneName);
            return 1;
        }

        int ScrollWnd::Create(CStr const&, unsigned, BoundsBase<float> const& rc, unsigned)
        {
            return Create(rc, 1);
        }

        ScrollWnd::~ScrollWnd()
        {
        }

        float ScrollWnd::GetMaxPos() const
        {
            return m_maxPos;
        }

        void ScrollWnd::SetScrollPane(CStr const& scrollPaneName)
        {
            Wnd::SetScrollPane(scrollPaneName);
            if (m_vertical)
            {
                if (auto pane = GetGfxServer()->GetScrollPane(m_scrollPaneName))
                {
                    if (m_btn0)
                    {
                        m_btn0->SetPane(pane->m_prevButtonPaneName);
                    }
                    if (m_btn1)
                    {
                        m_btn1->SetPane(pane->m_nextButtonPaneName);
                    }
                    RecalcLayot();
                }
            }
        }

        void ScrollWnd::SetScrollRect(float szX, float szY)
        {
            // RVA 0x6FA3C0 - the scrollable overhang is whatever the content
            // sticks out past the bar; content that fits resets the position.
            float const barSz = m_vertical ? GetBounds().height : GetBounds().width;
            float const contentSz = m_vertical ? szY : szX;

            if (barSz < contentSz)
            {
                m_maxPos = contentSz - barSz;
            }
            else
            {
                m_maxPos = 0.0;
                m_curPos = 0.0;
            }

            if (auto* pane = GetGfxServer()->GetScrollPane(m_scrollPaneName))
            {
                m_thumbSz = m_vertical ? pane->m_thumbSize.y : pane->m_thumbSize.x;
            }
            else
            {
                m_thumbSz = 30.0;
            }
        }

        Class* ScrollWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(ScrollWnd);
        }

        void ScrollWnd::SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin)
        {
            m_bounds = rect;
            if (bUpdateBaseOrigin)
            {
                m_baseOrigin.x = m_bounds.x0;
                m_baseOrigin.y = m_bounds.y0;
            }
            RecalcLayot();
        }

        void ScrollWnd::ShowWindow(bool bShow)
        {
            if (bShow)
                this->m_style |= 0x200u;
            else
                this->m_style &= 0xFDu;
            if (m_btn0)
                m_btn0->ShowWindow(bShow);
            if (m_btn1)
                m_btn1->ShowWindow(bShow);
        }

        int ScrollWnd::OnPaint(DrawInfo const& di)
        {
            auto pane = GetGfxServer()->GetScrollPane(m_scrollPaneName);
            if (!pane)
            {
                return 0;
            }
            auto bodyRect = GetBodyRect();
            bodyRect.y0 -= pane->m_space;
            bodyRect.height += pane->m_space * 2.0;

            unsigned clr = 0;
            PaneFlagBg bgFlag = PANE_FLAG_BG_OUT;
            if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
            {
                auto barPane = GetGfxServer()->GetPane(pane->m_barPaneName);
                if (barPane && (barPane->m_frame[3] || barPane->m_bg[3]))
                {
                    clr = this->m_curClr;
                    bgFlag = PANE_FLAG_BG_DISABLE;
                }
                else
                {
                    clr = 3;
                    bgFlag = PANE_FLAG_BG_OUT;
                }
            }
            GetGfxServer()->AddFlatAxialPane0(di, bodyRect, clr, 7, pane->m_barPaneName, bgFlag);

            auto const thumbRect = GetThumbRect();
            if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
            {
                return 1;
            }
            if (m_vertical)
            {
                if (thumbRect.height > bodyRect.height)
                {
                    return 1;
                }
                GetGfxServer()->AddImagedRectGeneral(di, thumbRect, clr, pane->m_thumbTex, 0.0, 0.0, 1.0, 1.0);
            }
            if (thumbRect.width <= bodyRect.width)
            {
                GetGfxServer()->AddImagedRectGeneral(di, thumbRect, clr, pane->m_thumbTex, 0.0, 0.0, 1.0, 1.0);
            }
            return 1;
        }

        void ScrollWnd::RecalcLayot()
        {
            // RVA 0x6FA480 - parks the two stepper buttons at the ends of the bar.
            // NOTE: only the vertical layout is handled; a horizontal scroll bar
            // leaves its buttons wherever they were created.
            if (!m_vertical)
            {
                return;
            }
            auto* pane = GetGfxServer()->GetScrollPane(m_scrollPaneName);
            if (!pane)
            {
                return;
            }

            float const btnX0 = (pane->GetWidth() - pane->m_btnSize.x) * 0.5f;
            float const btnW = pane->m_btnSize.x;

            if (m_btn0)
            {
                BoundsBase<float> bounds;
                bounds.x0 = btnX0;
                bounds.y0 = 0.0f;
                bounds.width = btnW;
                bounds.height = pane->m_btnSize.y;
                m_btn0->SetBounds(bounds, true);
            }
            if (m_btn1)
            {
                float const btn1Y0 = m_bounds.height - pane->m_btnSize.y;
                BoundsBase<float> bounds;
                bounds.x0 = btnX0;
                bounds.y0 = btn1Y0;
                bounds.width = btnW;
                bounds.height = m_bounds.height - btn1Y0;
                m_btn1->SetBounds(bounds, true);
            }
        }

        int ScrollWnd::OnMouseMove(PointBase<float> const& pt0, PointBase<float> const& deltas)
        {
            Wnd::OnMouseMove(pt0, deltas);

            if (this != GetStation()->GetCapture())
            {
                m_tracking = false;
            }
            if (!m_tracking)
            {
                return 1;
            }

            BoundsBase<float> const bodyB = GetBodyRect();

            if (deltas.x == 0.0f && deltas.y == 0.0f)
            {
                BoundsBase<float> const thumbB = GetThumbRect();
                m_hitPosInThumb = m_vertical ? (pt0.y - thumbB.y0) : (pt0.x - thumbB.x0);
                float const maxHit = m_thumbSz - 1.0f;
                if (m_hitPosInThumb < 0.0f)
                {
                    m_hitPosInThumb = 0.0f;
                }
                if (m_hitPosInThumb > maxHit)
                {
                    m_hitPosInThumb = maxHit;
                }
            }

            float const oldPos = m_curPos;
            if (m_vertical)
            {
                m_curPos = ((pt0.y - bodyB.y0) - m_hitPosInThumb) / (bodyB.height - m_thumbSz);
            }
            else
            {
                m_curPos = ((pt0.x - bodyB.x0) - m_hitPosInThumb) / (bodyB.width - m_thumbSz);
            }

            if (m_curPos < 0.0f)
            {
                m_curPos = 0.0f;
            }
            else if (m_curPos > 1.0f)
            {
                m_curPos = 1.0f;
            }

            if (oldPos != m_curPos)
            {
                CallParentNotify(5, {}, false);
            }
            return 1;
        }

        ScrollWnd::ScrollWnd()
        {
            m_persistant = false;
            m_style = 0x140260;
        }

        ScrollWnd::ScrollWnd(ScrollWnd const& sw) : Wnd(sw)
        {
        }

        int ScrollWnd::OnWndNotify(Wnd* from, unsigned idFrom, unsigned message, AIParam const& data)
        {
            if ((m_style & WS_REFLECT_NOTIFY_MESSAGES_TO_PARENT) != 0)
            {
                ReflectChildNotifyToParent(from, idFrom, message, data);
            }
            if (message != 1)
            {
                return 1;
            }

            float const oldPos = m_curPos;
            float const boundsDim = m_vertical ? GetBounds().height : GetBounds().width;
            float const step = boundsDim / m_maxPos;

            if (idFrom == 256)
            {
                m_curPos -= step;
            }
            else if (idFrom == 257)
            {
                m_curPos += step;
            }

            if (m_curPos < 0.0f)
            {
                m_curPos = 0.0f;
            }
            else if (m_curPos > 1.0f)
            {
                m_curPos = 1.0f;
            }

            if (oldPos != m_curPos)
            {
                CallParentNotify(5, {}, true);
            }
            return 1;
        }

        BoundsBase<float> ScrollWnd::GetThumbRect() const
        {
            auto pane = GetGfxServer()->GetScrollPane(m_scrollPaneName);
            if (pane)
            {
                auto bounds = GetBodyRect();
                if (m_vertical)
                {
                    bounds.y0 = ((bounds.height - m_thumbSz) * m_curPos) + bounds.y0;
                    bounds.height = m_thumbSz;
                    bounds.x0 = (pane->GetWidth() - pane->m_thumbSize.x) * 0.5;
                    bounds.width = pane->m_thumbSize.x;
                }
                else
                {
                    bounds.x0 = ((bounds.width - m_thumbSz) * m_curPos) + bounds.x0;
                    bounds.width = m_thumbSz;
                    bounds.y0 = (pane->GetWidth() - pane->m_thumbSize.y) * 0.5;
                    bounds.height = pane->m_thumbSize.y;
                }
                return bounds;
            }
            return { 0.0, 0.0 };
        }

        BoundsBase<float> ScrollWnd::GetBodyRect() const
        {
            auto pane = GetGfxServer()->GetScrollPane(m_scrollPaneName);
            if (pane)
            {
                auto bounds = GetBounds();
                if (m_vertical)
                {
                    bounds.y0 = pane->m_btnSize.y;
                    bounds.height -= (bounds.y0 * 2.0);
                    bounds.x0 = (pane->GetWidth() - pane->m_barWidth) * 0.5;
                    bounds.width = pane->m_barWidth;
                }
                else
                {
                    bounds.x0 = pane->m_btnSize.x;
                    bounds.width -= (bounds.x0 * 2.0);
                    bounds.y0 = (pane->GetWidth() - pane->m_barWidth) * 0.5;
                    bounds.height = pane->m_barWidth;
                }
                return bounds;
            }
            return {0.0, 0.0};
        }

        int ScrollWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
        {
            m_tracking = state != 0;
            GetStation()->CaptureMouse(state != 0 ? this : nullptr);
            PointBase<float> const noDelta{0.0f, 0.0f};
            OnMouseMove(at, noDelta);
            return 1;
        }
	}
}

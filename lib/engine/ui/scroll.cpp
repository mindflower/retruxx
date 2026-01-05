#include <ui/button.h>
#include <ui/frame.h>
#include <ui/scroll.h>
#include <ui/ui_srv.h>

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
            RETRUXX_NOT_IMPLEMENTED;
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

        int ScrollWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        ScrollWnd::~ScrollWnd()
        {
        }

        float ScrollWnd::GetMaxPos() const
        {
            RETRUXX_NOT_IMPLEMENTED;
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
            //TODO: check this!! and refactor
            float v4; // xmm1_4
            float v6; // xmm0_4
            m3d::ui::ScrollPane* v7; // eax
            char v8[16]; // [esp+4h] [ebp-10h] BYREF

            if (this->m_vertical)
            {
                v4 = GetBounds().height;
            }
            else
            {
                v4 = GetBounds().width;
                szY = szX;
            }
            if (v4 < szY)
            {
                v6 = szY - v4;
                if (v6 != m_maxPos)
                    this->m_maxPos = v6;
            }
            else
            {
                v6 = 0.0;
                this->m_curPos = 0.0;
            }
            this->m_maxPos = v6;
            v7 = GetGfxServer()->GetScrollPane(m_scrollPaneName);
            if (v7)
            {
                if (this->m_vertical)
                    this->m_thumbSz = v7->m_thumbSize.y;
                else
                    this->m_thumbSz = v7->m_thumbSize.x;
            }
            else
            {
                this->m_thumbSz = 30.0;
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
            //TODO: check this
            if (m_vertical)
            {
                auto pane = GetGfxServer()->GetScrollPane(m_scrollPaneName);
                if (pane)
                {
                    auto btn1Y1 = pane->GetWidth();
                    float v4 = (btn1Y1 - pane->m_btnSize.x) * 0.5;
                    auto v5 = m_bounds.height - pane->m_btnSize.y;
                    auto v6 = m_bounds.height;
                    auto v7 = pane->m_btnSize.x + v4;
                    auto btnX0 = v4;
                    auto btnX1 = v7;
                    if (m_btn0)
                    {
                        BoundsBase<float> bounds;
                        bounds.x0 = v4;
                        bounds.y0 = 0.0;
                        bounds.width = v7 - v4;
                        bounds.height = pane->m_btnSize.y;
                        m_btn0->SetBounds(bounds, true);
                        v7 = btnX1;
                        v4 = btnX0;
                    }
                    if (m_btn1)
                    {
                        BoundsBase<float> bounds;
                        bounds.x0 = v4;
                        bounds.y0 = v5;
                        bounds.width = v7 - v4;
                        bounds.height = v6 - v5;
                        m_btn1->SetBounds(bounds, true);
                    }
                }
            }
        }

        int ScrollWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        ScrollWnd::ScrollWnd()
        {
            m_persistant = false;
            m_style = 0x140260;
        }

        ScrollWnd::ScrollWnd(ScrollWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ScrollWnd::OnWndNotify(Wnd*, unsigned, unsigned, AIParam const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
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

        int ScrollWnd::OnMouseButton0(unsigned, PointBase<float> const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
	}
}

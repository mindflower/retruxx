#include <ui/slider.h>
#include <ui/ui_srv.h>
#include <core/aiparam.h>

#include "m3dapp.h"

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(SliderWnd)
    	RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(SliderWnd);

        Object* SliderWnd::CreateObject()
        {
            return new SliderWnd;
        }

        Class* SliderWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        int SliderWnd::GetMax() const
        {
            return m_max;
        }

        Object* SliderWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int SliderWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            auto st = style;
            if (!style)
            {
                st = m_style;
            }
            auto result = CreateWnd(caption, st, rc, id);
            if (result)
            {
                m_style |= 4u;
                SetMinMax(0, 10);
                m_notchWidth = GetGfxServer()->GetSliderNotchWidthRatio() * rc.height;
                result = 1;
            }
            return result;
        }

        int SliderWnd::Create(float, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        SliderWnd::~SliderWnd()
        {
            throw std::logic_error("Not implemented");
        }

        int SliderWnd::GetMin() const
        {
            return m_min;
        }

        Class* SliderWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(SliderWnd);
        }

        void SliderWnd::SetMinMax(int min, int max)
        {
            //TODO: refactor
            int v3; // eax
            int v4; // esi

            v3 = this->m_cur;
            v4 = max;
            if (max < min)
                v4 = min;
            if (v3 < min)
                v3 = min;
            this->m_min = min;
            if (v3 > v4)
                v3 = v4;
            this->m_max = v4;
            SetNotch(v3);
        }

        int SliderWnd::GetNotch() const
        {
            return m_cur;
        }

        void SliderWnd::SetNotch(int n)
        {
            //TODO: check and refactor
            int v2; // eax
            bool v3; // zf

            v2 = n;
            if (n < this->m_min)
                v2 = this->m_min;
            if (v2 > this->m_max)
                v2 = this->m_max;
            v3 = (this->m_style & 0x40000) == 0;
            this->m_cur = v2;
            if (!v3)
            {
                CallParentNotify(5u, {}, false);
            }
        }

        int SliderWnd::OnPaint(DrawInfo const& di)
        {
            auto const leftTex = GetGfxServer()->GetTexture(TEX_SLIDER_LEFT);
            auto w = 0;
            auto h = 0;
            M3D_APP->m_renderer->GetDims(leftTex, w, h);
            float sideW = w;
            float sideH = h;
            M3D_APP->m_renderer->AbsToRel(sideW, sideH);
            auto const bounds = GetBounds();

            BoundsBase<float> rect;
            rect.x0 = 0.0;
            rect.y0 = 0.0;
            rect.height = bounds.height;
            rect.width = sideW;
            unsigned clr = m_curClr;
            if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
                clr = 3;
            GetGfxServer()->AddImagedRect(di, rect, clr, leftTex);

            rect.x0 = bounds.width - sideW;
            rect.y0 = 0.0;
            rect.height = bounds.height;
            rect.width = sideW;
            auto const rightTex = GetGfxServer()->GetTexture(TEX_SLIDER_RIGHT);
            GetGfxServer()->AddImagedRect(di, rect, clr, rightTex);

            rect.x0 = sideW;
            rect.y0 = 0.0;
            rect.height = bounds.height;
            rect.width = bounds.width - (sideW * 2.0);
            auto const bodyTex = GetGfxServer()->GetTexture(TEX_SLIDER_BODY);
            GetGfxServer()->AddImagedRectGeneral(di, rect, clr, bodyTex, 0.0, 0.0, rect.width / bounds.width, 1.0);

            //TODO: check this
            rect.x0 = (((bounds.width - m_notchWidth) * ((m_cur - m_min) / (m_max - m_min))) + (m_notchWidth * 0.5)) - (this->m_notchWidth * 0.5);
            rect.y0 = 0.0;
            rect.height = bounds.height;
            rect.width = m_notchWidth;
            auto const thumbTex = GetGfxServer()->GetTexture(TEX_SLIDER_THUMB);
            GetGfxServer()->AddImagedRect(di, rect, clr, thumbTex);

            return 1;
        }

        int SliderWnd::OnMouseMove(PointBase<float> const& at, PointBase<float> const& deltas)
        {
            if (this != GetStation()->GetCapture())
            {
                m_tracking = false;
            }
            if (!m_tracking)
            {
                return 1;
            }
            auto cur = (at.x - (m_notchWidth * 0.5)) / (GetBounds().width - m_notchWidth);
            if (cur < 0.0)
            {
                cur = 0.0;
            }
            else if (cur > 1.0)
            {
                cur = 1.0;
            }
            SetNotch(m_min + ((m_max - m_min) * cur));
            return 1;
        }

        BoundsBase<float> SliderWnd::GetBodyRect() const
        {
            throw std::logic_error("Not implemented");
        }

        int SliderWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
        {
            m_tracking = state != 0;
            GetStation()->CaptureMouse(state != 0 ? this : nullptr);
            OnMouseMove(at, { 0.0, 0.0 });
            return 1;
        }

        SliderWnd::SliderWnd()
        {
            m_style = 274944;
        }

        SliderWnd::SliderWnd(SliderWnd const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}

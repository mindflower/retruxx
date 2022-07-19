#include <ui/slider.h>
#include <ui/ui_srv.h>
#include <core/aiparam.h>

namespace m3d
{
    namespace ui
    {
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
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
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

            v3 = m_cur;
            v4 = max;
            if (max < min)
                v4 = min;
            if (m_cur < min)
                m_cur = min;
            m_min = min;
            if (v3 > v4)
                v3 = v4;
            m_max = v4;
            SetNotch(v3);
        }

        int SliderWnd::GetNotch() const
        {
            throw std::logic_error("Not implemented");
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

        int SliderWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        int SliderWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> SliderWnd::GetBodyRect() const
        {
            throw std::logic_error("Not implemented");
        }

        int SliderWnd::OnMouseButton0(unsigned, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
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

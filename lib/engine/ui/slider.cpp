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
            return new SliderWnd(*this);
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
                // RVA 0x6B2730 does BYTE2(m_style) |= 4, i.e. |= 0x40000. Without
                // it SetNotch never notifies the parent, since it gates on that
                // very bit.
                m_style |= WS_SEND_NOTIFY_MESSAGES;
                SetMinMax(0, 10);
                m_notchWidth = GetGfxServer()->GetSliderNotchWidthRatio() * rc.height;
                result = 1;
            }
            return result;
        }

        int SliderWnd::Create(float width, unsigned id)
        {
            BoundsBase<float> rc{0.0f, 0.0f, width, static_cast<float>(GetGfxServer()->GetSliderHeight())};
            return Create(CStr(), 0, rc, id);
        }

        SliderWnd::~SliderWnd()
        {
            // No owned resources; the base Wnd destructor does the work.
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
            // RVA 0x6B2880 - an inverted range collapses onto its minimum, and the
            // current notch is dragged back into the new range.
            auto hi = max < min ? min : max;
            auto cur = m_cur < min ? min : m_cur;
            m_min = min;
            if (cur > hi)
            {
                cur = hi;
            }
            m_max = hi;
            SetNotch(cur);
        }

        int SliderWnd::GetNotch() const
        {
            return m_cur;
        }

        void SliderWnd::SetNotch(int n)
        {
            // RVA 0x6B2CE0
            auto cur = n;
            if (cur < m_min)
            {
                cur = m_min;
            }
            if (cur > m_max)
            {
                cur = m_max;
            }
            bool const bNotify = (m_style & WS_SEND_NOTIFY_MESSAGES) != 0;
            m_cur = cur;
            if (bNotify)
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
            // NOTE: the u1 texture coordinate really is the strip width over the
            // window *height* - the shipped code divides by the wrong extent, so a
            // slider that is not square repeats its body texture oddly.
            GetGfxServer()->AddImagedRectGeneral(di, rect, clr, bodyTex, 0.0, 0.0, rect.width / bounds.height, 1.0);

            rect.x0 = (bounds.width - m_notchWidth) * (static_cast<float>(m_cur - m_min) / (m_max - m_min));
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
            auto const bounds = GetBounds();
            BoundsBase<float> result;
            result.x0 = m_notchWidth * 0.5f;
            result.y0 = 0.0f;
            result.width = bounds.width - m_notchWidth;
            result.height = bounds.height;
            return result;
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

        SliderWnd::SliderWnd(SliderWnd const& sw)
            : Wnd(sw)
        {
            // RVA 0x6B25F0 - the shipped copy constructor chains to the Wnd one and
            // copies none of the slider's own state, so a cloned slider starts with
            // an indeterminate range, notch and notch width.
        }
    }
}

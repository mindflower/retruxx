#include <core/aiparam.h>
#include <math/vector2.h>
#include <ui/ui.h>
#include <ui/wndstation.h>

namespace m3d
{
    namespace ui
    {
        int Wnd::OnActivate(bool on)
        {
            if (!on || (m_style & 0x4000) == 0)
            {
                return 1;
            }
            auto* child = GetNextActivatableChild(nullptr, 0);
            if (child == nullptr)
            {
                return 1;
            }
            GetStation()->Activate(child);
            return 1;
        }

        int Wnd::OnMouseDblClick(PointBase<float> const& firstClickPt, PointBase<float> const& secondClickPt)
        {
            if ((m_style & 0x40000) != 0)
            {
                AIParam const data(CVector2(secondClickPt.x, secondClickPt.y));
                CallParentNotify(4, data, false);
            }
            if ((m_style & 0x20) == 0 || !GetParent())
            {
                return 1;
            }
            auto const first = ToParent(firstClickPt);
            auto const second = ToParent(secondClickPt);
            auto* wndParent = dynamic_cast<Wnd*>(GetParent());
            wndParent->OnMouseDblClick(first, second);
            return 1;
        }

        int Wnd::OnMouseOut()
        {
            m_mouseOver = false;
            m_mouseDown = 0;
            RemoveTooltip();
            if ((m_style & 0x40000) == 0)
            {
                return 1;
            }
            if (GetParent())
            {
                auto* wndParent = dynamic_cast<Wnd*>(GetParent());
                GetStation()->AddNotifyForWnd(this, wndParent, 8, {}, (m_style & 0x400000) != 0);
            }
            return 1;
        }

        int Wnd::OnPaint(DrawInfo const& clipToIt)
        {
            return 0;
        }

        int Wnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
        {
            return 0;
        }

        int Wnd::OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas)
        {
            if ((m_style & 0x20) !=0 && GetParent())
            {
                auto const res = ToParent(pt);
                auto* wndParent = dynamic_cast<Wnd*>(GetParent());
                wndParent->OnMouseMove(res, deltas);
            }
            if (m_dragMode == DRAG_MOVE)
            {
                DoDragMove0(pt);
            }
            return 1;
        }
    }
}

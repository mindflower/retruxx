#include <config.h>
#include <m3dapp.h>
#include <core/aiparam.h>
#include <core/kernel.h>
#include <math/vector2.h>
#include <ui/ui.h>
#include <ui/wndstation.h>

namespace m3d
{
    namespace ui
    {
        GfxServer* Wnd::GetGfxServer()
        {
            return m_gfx;
        }

        void Wnd::GameDataSetDirty()
        {
            m_gameDataFlags |= 4;
        }

        int Wnd::GameDataSetup()
        {
            return 1;
        }

        void Wnd::SetDefaultFont(int uiFont)
        {
            if (Application::g_pApp->IsTextHieroglyphic(m_caption) && g_Kernel->GetEngineCfg().m_ui_forceHieroglyphicFont.GetB())
            {
                m_defFont = GetGfxServer()->m_hieroglyphicFontId;
            }
            else
            {
                m_defFont = uiFont;
            }
        }

        void Wnd::SetDefaultFont(CStr const& name, float height, FontType type, FontParams params)
        {
            if (Application::g_pApp->IsTextHieroglyphic(m_caption) && g_Kernel->GetEngineCfg().m_ui_forceHieroglyphicFont.GetB())
            {
                m_defFont = GetGfxServer()->m_hieroglyphicFontId;
            }
            else
            {
                m_defFont = GetGfxServer()->GetFontId(name, height, type, params);
            }
        }

        unsigned Wnd::GetStyle() const
        {
            return m_style;
        }

        int Wnd::GetCursor(Cursor& cur)
        {
            return GetStation()->GetDefaultCursor(cur);
        }

        WndStation* Wnd::GetStation() const
        {
            return m_wndStation;
        }

        bool Wnd::IsPtInBounds(PointBase<float> const& pt) const
        {
            //TODO: check this
            auto const result = ToScreen(PointBase<float>{});
            if (pt.x < result.x || (((m_bounds.width + result.x) - result.x) + result.x) <= pt.x)
            {
                return false;
            }
            if (pt.y >= result.y && (((m_bounds.height + result.y) - result.y) + result.y) > pt.y)
            {
                return true;
            }
            return false;
        }

        int Wnd::SetProperty(unsigned propId, void* prop)
        {
            if (Object::SetProperty(propId, prop))
            {
                return 1;
            }
            if (propId != 0x4000)
            {
                return 0;
            }
            //TODO: check this
            m_toolTipText = static_cast<char*>(prop);
            return 1;
        }

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

        void Wnd::OnPaintOverChildren(DrawInfo const& clipToIt)
        {
        }

        int Wnd::OnWndNotify(Wnd* from, unsigned idFrom, unsigned message, AIParam const& data)
        {
            if ((GetStyle() & 0x100000) != 0)
            {
                ReflectChildNotifyToParent(from, idFrom, message, data);
            }
            return 0;
        }

        void Wnd::OnDisplayChanged()
        {
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

        int Wnd::CallParentNotify(unsigned msg, AIParam const& data, bool urgent)
        {
            auto* wndParent = dynamic_cast<Wnd*>(GetParent());
            if (!wndParent)
            {
                return 0;
            }
            if ((GetStyle() & 0x400000) != 0)
            {
                urgent = true;
            }
            GetStation()->AddNotifyForWnd(this, wndParent, msg, data, urgent);
            return 0;
        }

        void Wnd::DoDragMove0(PointBase<float> const& pt)
        {
            if (this == GetStation()->GetCapture())
            {
                DoDragMove(pt);
            }
            else
            {
                DoDragMove(pt);
                GetStation()->CaptureMouse(nullptr);
                m_bounds.x0 = m_dragStartPt.x;
                m_bounds.y0 = m_dragStartPt.y;
                m_dragMode = DRAG_NONE;
            }
        }
    }
}

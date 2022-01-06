#include <config.h>
#include <m3dapp.h>
#include <stdexcept>
#include <core/aiparam.h>
#include <core/kernel.h>
#include <math/vector2.h>
#include <ui/ui.h>
#include <ui/wndstation.h>

namespace m3d
{
    namespace ui
    {
        Wnd::AnimationInfo::~AnimationInfo()
        {
            throw std::logic_error("Not implemented");
        }

        Wnd::AnimationInfo::AnimationType Wnd::AnimationInfo::Str2AnimationType(CStr const&) const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::AnimationInfo::Invalidate()
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::AnimationInfo::IsValid() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::AnimationInfo::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        CStr Wnd::AnimationInfo::AnimationType2Str(AnimationType) const
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::AnimationInfo::CanAnimate() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::AnimationInfo::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::AnimationInfo::SetupDefaultOnHide()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::AnimationInfo::SetupDefaultOnShow()
        {
            throw std::logic_error("Not implemented");
        }

        Class* Wnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        Object* Wnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GetUniqueId() const
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::IsVisible() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::ShowWindow(bool)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned Wnd::GetTextColor() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetScrollPane(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetCursorShow(bool)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GetGuiId() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GetDefaultFont() const
        {
            throw std::logic_error("Not implemented");
        }

        unsigned Wnd::GetTextColorDisabled() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnBeforeAddToWndStation()
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnAfterAddToWndStation()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetGuiId(int)
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> Wnd::ToParent(BoundsBase<float> const&) const
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Wnd::ToParent(PointBase<float> const&) const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetTextColorDisabled(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetTextColor(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::GetCursorShow() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnAfterRemoveFromWndStation()
        {
            throw std::logic_error("Not implemented");
        }

        Wnd::~Wnd()
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle Wnd::GetBackground() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetGameDataFlags(int)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Wnd::GetOrigin() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::StartAnimation(AnimationInfo const&, bool)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned Wnd::GetColor() const
        {
            throw std::logic_error("Not implemented");
        }

        ScrollWnd* Wnd::GetScrollVWnd()
        {
            throw std::logic_error("Not implemented");
        }

        CStr Wnd::GetText() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetPane(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned Wnd::GetInt() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetStyle(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GameDataSave(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        Object* Wnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::RemoveChildForce(Object*)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::SetBackground(rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::SetBackground(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GetGameDataFlags()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetOrigin(PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetColor(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::SetText(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetInt(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::EnableWindow(bool)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::EnableOnShowAnimation(bool)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::AddChild(Object*)
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::IsEnabled() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::EnableOnHideAnimation(bool)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned Wnd::GetId() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetPaneFlags(int)
        {
            throw std::logic_error("Not implemented");
        }

        CStr const& Wnd::GetScrollPaneName() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetOnShowAnimationImmediate(bool)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetBounds(BoundsBase<float> const&, bool)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::Centralize()
        {
            throw std::logic_error("Not implemented");
        }

        Class* Wnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        Wnd::AnimationInfo const& Wnd::GetCurrentAnimation() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GameDataLoad(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetId(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::StopAnimation(bool)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Wnd::ToWindow(PointBase<float> const&) const
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> Wnd::ToWindow(BoundsBase<float> const&) const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::AdjustForWndTextToFit(unsigned, float)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GetPaneFlags() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetOnHideAnimationImmediate(bool)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Wnd::ToScreen(PointBase<float> const&) const
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> Wnd::ToScreen(BoundsBase<float> const&) const
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> Wnd::GetBounds() const
        {
            throw std::logic_error("Not implemented");
        }

        CStr Wnd::GetPaneName() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GameDataUpdate(void*, int)
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> Wnd::GetClientBounds() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnBeforeRemoveFromWndStation()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetFormatMode(TextFormatFlags)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd::AnimationInfo const& Wnd::GetOnHideAnimation() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetClientEdges(std::vector<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetClientEdges(float, float, float, float)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::AdjustToFitChildren()
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> const& Wnd::GetBaseOrigin() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetOnShowAnimation(AnimationInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GameDataClear(bool)
        {
            throw std::logic_error("Not implemented");
        }

        TextWrapFlags Wnd::GetWrapMode() const
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::Valid() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::RemoveChild(Object*)
        {
            throw std::logic_error("Not implemented");
        }

        TextFormatFlags Wnd::GetFormatMode() const
        {
            throw std::logic_error("Not implemented");
        }

        float Wnd::GetFrameWidth() const
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::IsAnimatingNow() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GetPropertiesList(std::set<unsigned>&) const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetWrapMode(TextWrapFlags)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd::AnimationInfo const& Wnd::GetOnShowAnimation() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GetProperty(unsigned, void*) const
        {
            throw std::logic_error("Not implemented");
        }

        std::vector<float> const& Wnd::GetClientEdges() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetOnHideAnimation(AnimationInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetBaseOrigin(PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::RemoveTooltip()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::DrawWndText(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::ProcessAnimation(int, int)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd* Wnd::CreateTooltipWnd()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::OnEndAnimation(bool)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnObtainingFocus()
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnLoosingFocus()
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::CreateWnd(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Wnd::Wnd()
        {
            throw std::logic_error("Not implemented");
        }

        Wnd::Wnd(Wnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Wnd::GetOriginPoint() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::DrawNonClient(DrawInfo const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::StopAnimationMoveSound()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::OnNcPaint(DrawInfo const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::Unregister()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::Register()
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::DestroyWnd()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::FinishDragMove(int, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::StartDragMove(PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

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

        int Wnd::OnTick(int, int)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }

        int Wnd::ReflectChildNotifyToParent(Wnd*, unsigned, unsigned, AIParam const&)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnMouseIn()
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
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

        int Wnd::OnMouseClick(PointBase<float> const&)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }

        void Wnd::OnPaintOverChildren(DrawInfo const& clipToIt)
        {
        }

        int Wnd::OnMouseButton2(unsigned int, PointBase<float> const&)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnMouseButton1(unsigned, PointBase<float> const&)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnWndNotify(Wnd* from, unsigned idFrom, unsigned message, AIParam const& data)
        {
            if ((GetStyle() & 0x100000) != 0)
            {
                ReflectChildNotifyToParent(from, idFrom, message, data);
            }
            return 0;
        }

        int Wnd::OnMouseWheel(int, PointBase<float> const&)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }

        void Wnd::OnDisplayChanged()
        {
        }

        Wnd* Wnd::GetNextActivatableChild(Wnd*, int)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
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

        void Wnd::DoDragMove(PointBase<float> const&)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }
    }
}

#include <config.h>
#include <m3dapp.h>
#include <core/clazz.h>
#include <stdexcept>
#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <math/vector2.h>
#include <server/utils.h>
#include <ui/frame.h>
#include <ui/scroll.h>
#include <ui/ui.h>
#include <ui/wndstation.h>
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(Wnd)
    	RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(Wnd);

        Wnd::AnimationInfo::~AnimationInfo()
        {
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
            return RT_CLASS_LOCAL(Object);
        }

        Object* Wnd::CreateObject()
        {
            return new Wnd;
        }

        int Wnd::GetUniqueId() const
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::IsVisible() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::ShowWindow(bool show)
        {
            if (show)
                this->m_style |= 2u;
            else
                this->m_style &= 0xFDu;
        }

        unsigned Wnd::GetTextColor() const
        {
            return m_textColor;
        }

        void Wnd::SetScrollPane(CStr const& scrollPaneName)
        {
            if (!scrollPaneName.empty())
            {
                m_scrollPaneName = scrollPaneName;
            }
            if (m_scrollVWnd)
            {
                m_scrollVWnd->SetScrollPane(m_scrollPaneName);
            }
            if (m_scrollHWnd)
            {
                m_scrollHWnd->SetScrollPane(m_scrollPaneName);
            }
        }

        void Wnd::SetCursorShow(bool state)
        {
            m_showCursor = state;
        }

        int Wnd::GetGuiId() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GetDefaultFont() const
        {
            return m_defFont;
        }

        unsigned Wnd::GetTextColorDisabled() const
        {
            return m_textColorDisabled;
        }

        int Wnd::OnBeforeAddToWndStation()
        {
            auto res = 1;
            for (auto wnd = dynamic_cast<Wnd*>(GetFirstChild_()); wnd; wnd = dynamic_cast<Wnd*>(wnd->GetNextSibling_()))
            {
                assert(wnd->IsKindOf(RT_CLASS_LOCAL(Wnd)));
                res &= wnd->OnBeforeAddToWndStation();
            }
            return res;
        }

        int Wnd::OnAfterAddToWndStation()
        {
            auto res = 1;
            for (auto wnd = dynamic_cast<Wnd*>(GetFirstChild_()); wnd; wnd = dynamic_cast<Wnd*>(wnd->GetNextSibling_()))
            {
                assert(wnd->IsKindOf(RT_CLASS_LOCAL(Wnd)));
                res &= wnd->OnAfterAddToWndStation();
            }
            if (IsAnimatingNow() && m_currentAnimation.m_purpose == AnimationInfo::PURPOSE_SHOW
                || !m_wndStation->IsAnimationEnabled() || !m_onShowAnimation.CanAnimate())
            {
                return res;
            }
            auto isAnim = IsAnimatingNow();
            StartAnimation(m_onShowAnimation, isAnim);
            return res;
        }

        void Wnd::SetGuiId(int)
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> Wnd::ToParent(BoundsBase<float> const&) const
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Wnd::ToParent(PointBase<float> const& pt) const
        {
            auto res = ToScreen(pt);
            auto parentWnd = dynamic_cast<Wnd*>(GetParent());
            if (parentWnd)
            {
                auto parentToScreenRes = parentWnd->ToScreen(PointBase<float>{0.0, 0.0});
                res.x = res.x - parentToScreenRes.x;
                res.y = res.y - parentToScreenRes.y;
                return res;
            }
            return res;
        }

        void Wnd::SetTextColorDisabled(unsigned textColor)
        {
            m_textColorDisabled = textColor;
            char tmp[128] = { 0 };
            auto clr = GetGfxServer()->GetColor(textColor);
            sprintf(tmp, "%08x", clr);
            m_strTextColorDisabled = CStr("@") + tmp;
        }

        void Wnd::SetTextColor(unsigned textColor)
        {
            m_textColor = textColor;
            char tmp[128] = { 0 };
            auto clr = GetGfxServer()->GetColor(textColor);
            sprintf(tmp, "%08x", clr);
            m_strTextColor = CStr("@") + tmp;
        }

        int Wnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        bool Wnd::GetCursorShow() const
        {
            return m_showCursor;
        }

        int Wnd::OnAfterRemoveFromWndStation()
        {
            auto res = 1;
            for (auto child = GetFirstChild_(); child; child = child->GetNextSibling_())
            {
                assert(child->IsKindOf(RT_CLASS_LOCAL(Wnd)));
                res &= dynamic_cast<Wnd*>(child)->OnAfterRemoveFromWndStation();
            }
            StopAnimationMoveSound();
            return res;
        }

        Wnd::~Wnd()
        {
            DestroyWnd();
        }

        rend::TexHandle Wnd::GetBackground() const
        {
            return m_bgTexture;
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
            return m_curClr;
        }

        ScrollWnd* Wnd::GetScrollVWnd()
        {
            throw std::logic_error("Not implemented");
        }

        CStr Wnd::GetText() const
        {
            return m_caption;
        }

        void Wnd::SetPane(CStr const& name)
        {
            if (!name.empty())
            {
                m_paneName = name;
            }
        }

        int Wnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            m_bounds = strToBounds(xmlNode->GetAttribute("org"));
            CStr strBaseOrigin = xmlNode->GetAttribute("baseOrigin");
            if (!strBaseOrigin.empty())
            {
                m_baseOrigin = strToPoint(strBaseOrigin);
            }
            else
            {
                m_baseOrigin.x = m_bounds.x0;
                m_baseOrigin.y = m_bounds.y0;
            }
            SafeUintAttrib(m_style, xmlNode, "style");
            SafeIntAttrib(m_activationOrder, xmlNode, "order");
            SafeUintAttrib(m_id, xmlNode, "id");

            CStr caption;
            CStr tip;
            SafeStrAttrib(caption, xmlNode, "caption");
            SafeStrAttrib(tip, xmlNode, "tip");
            SetText(GetStation()->InitializeStringUsingIds(caption));
            m_toolTipText = GetStation()->InitializeStringUsingIds(tip);
            SafeStrAttrib(m_bgTextureName, xmlNode, "backimage");
            SafeStrAttrib(m_paneName, xmlNode, "paneName");
            SafeIntAttrib(m_paneFlags, xmlNode, "paneFlags");
            SafeEnumAttrib(m_textWrap, xmlNode, "wrap");
            SafeEnumAttrib(m_textFormat, xmlNode, "format");
            SafeIntAttrib(m_defFont, xmlNode, "font");

            std::vector<float> vecClientEdges;
            CStr strClientEdges;
            SafeStrAttrib(strClientEdges, xmlNode, "clientEdges");
            ai::StrToFloatVector(strClientEdges, vecClientEdges);
            if (vecClientEdges.size() == 4)
            {
                SetClientEdges(vecClientEdges[0], vecClientEdges[1], vecClientEdges[2], vecClientEdges[3]);
            }

            SafeStrAttrib(m_scrollPaneName, xmlNode, "scrollPaneName");
            SafeClrAttrib(m_curClr, xmlNode, "wndColor");

            auto textColor = GetGfxServer()->GetColor(0);
            SafeClrAttrib(textColor, xmlNode, "textColor");
            SetTextColor(textColor);
            textColor = GetGfxServer()->GetColor(4);
            SafeClrAttrib(textColor, xmlNode, "textColorDisabled");
            SetTextColorDisabled(textColor);

            Create(m_caption, m_style, m_bounds, m_id);

            //TODO: check this
            if (!m_bgTextureName.empty())
            {
                SetBackground(m_bgTextureName);
            }

            ref_ptr animationsNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            xmlNode->GetFirstChild_(animationsNode, "Animations");
            if (!animationsNode->IsEmpty())
            {
                ref_ptr onShowAnimationNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                animationsNode->GetFirstChild_(onShowAnimationNode, "AnimationOnShow");
                if (!onShowAnimationNode->IsEmpty())
                {
                    AnimationInfo animationInfo;
                    animationInfo.ReadFromXmlNode(xmlFile, onShowAnimationNode);
                    SetOnShowAnimation(animationInfo);
                }

                ref_ptr onHideAnimationNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                animationsNode->GetFirstChild_(onHideAnimationNode, "AnimationOnHide");
                if (!onHideAnimationNode->IsEmpty())
                {
                    AnimationInfo animationInfo;
                    animationInfo.ReadFromXmlNode(xmlFile, onHideAnimationNode);
                    SetOnHideAnimation(animationInfo);
                }
            }
            return Object::ReadFromXmlNode(xmlFile, xmlNode);
        }

        unsigned Wnd::GetInt() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetStyle(unsigned style)
        {
            m_style = style;
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

        int Wnd::SetBackground(rend::TexHandle bgTex)
        {
            m_bgTextureName = "";
            if (m_bgTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_bgTexture);
            }
            m_bgTexture = bgTex;
            if (!m_bgTexture.IsValid())
            {
                return 0;
            }
            Application::g_pApp->m_renderer->ReferenceTexture(m_bgTexture);
            return 1;
        }

        int Wnd::SetBackground(CStr const& bgTextureName)
        {
            m_bgTextureName = bgTextureName;
            if (m_bgTexture.IsValid())
            {
                Application::g_pApp->m_renderer->ReleaseTexture(m_bgTexture);
            }
            if (!m_bgTextureName.empty())
            {
                m_bgTexture = Application::g_pApp->m_renderer->AddTexture(m_bgTextureName, 4);
            }
            else
            {
                m_bgTexture.SetInvalid();
            }
            return m_bgTexture.IsValid();
        }

        int Wnd::GetGameDataFlags()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetOrigin(PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetColor(unsigned color)
        {
            m_curClr = color;
        }

        int Wnd::SetText(CStr const& caption)
        {
            m_caption = caption;
            if (!Application::g_pApp->IsTextHieroglyphic(m_caption))
            {
                return 1;
            }
            if (g_Kernel->GetEngineCfg().m_ui_forceHieroglyphicFont.GetB())
            {
                m_defFont = m_gfx->m_hieroglyphicFontId;
            }
            return 1;
        }

        void Wnd::SetInt(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::EnableWindow(bool bEnable)
        {
            if (bEnable)
                this->m_style &= 0xFFFFFFFD;
            else
                this->m_style |= 2u;
        }

        void Wnd::EnableOnShowAnimation(bool)
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::AddChild(Object* w)
        {
            if (!w->IsKindOf(RT_CLASS_LOCAL(Wnd)))
            {
                return 0;
            }
            auto wnd = reinterpret_cast<Wnd*>(w);
            if (wnd->m_bSuspendedUnlink)
            {
                wnd->m_bSuspendedUnlink = false;
                std::vector<Object*> stack;
                stack.push_back(w);
                throw std::logic_error("Not implemented");
            }
            else
            {
                auto res = 0;
                if (this == m_wndStation || IsChildOf(m_wndStation))
                {
                    res = 1;
                    if (!(wnd->OnBeforeAddToWndStation() & 1))
                    {
                        return 0;
                    }
                }
                else
                {
                    res = 0;
                }
                Object::AddChild(w);
                m_wndStation->OnAddWnd(this, wnd);
                if (res)
                {
                    wnd->OnAfterAddToWndStation();
                    return 1;
                }
            }
            return 1;
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
            return m_id;
        }

        int Wnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            return CreateWnd(caption, style, rc, id);
        }

        void Wnd::SetPaneFlags(int flags)
        {
            m_paneFlags = flags;
        }

        CStr const& Wnd::GetScrollPaneName() const
        {
            return m_scrollPaneName;
        }

        void Wnd::SetOnShowAnimationImmediate(bool)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin)
        {
            m_bounds = rect;
            if (bUpdateBaseOrigin)
            {
                m_baseOrigin.x = m_bounds.x0;
                m_baseOrigin.y = m_bounds.y0;
            }
        }

        void Wnd::Centralize()
        {
            throw std::logic_error("Not implemented");
        }

        Class* Wnd::GetClass() const
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Wnd::AnimationInfo const& Wnd::GetCurrentAnimation() const
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::GameDataLoad(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetId(unsigned id)
        {
            m_id = id;
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
            return m_paneFlags;
        }

        void Wnd::SetOnHideAnimationImmediate(bool)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Wnd::ToScreen(PointBase<float> const& pt) const
        {
            //TODO: check this!!!
            PointBase<float> res;
            for (auto obj = this; obj; obj = dynamic_cast<const Wnd*>(obj->GetParent()))
            {
                res.x += obj->m_bounds.x0;
                res.y += obj->m_bounds.y0;
            }
            return res;
        }

        BoundsBase<float> Wnd::ToScreen(BoundsBase<float> const& b) const
        {
            //TODO: check this!
            PointBase<float> pt;
            pt.x = b.x0;
            pt.y = b.y0;
            auto tl = ToScreen(pt);

            BoundsBase<float> result;
            result.x0 = tl.x;
            result.y0 = tl.y;
            result.width = (b.width + tl.x) - tl.x;
            result.height = (b.height + tl.y) - tl.y;
            return result;
        }

        BoundsBase<float> Wnd::GetBounds() const
        {
            return m_bounds;
        }

        CStr Wnd::GetPaneName() const
        {
            return m_paneName;
        }

        int Wnd::GameDataUpdate(void*, int)
        {
            throw std::logic_error("Not implemented");
        }

        BoundsBase<float> Wnd::GetClientBounds() const
        {
            //TODO: check and refactor
            auto barWidth = GetFrameWidth();
            auto v4 = m_clientEdges[0] + (float)(0.0 - (float)(0.0 - barWidth));
            auto v5 = m_clientEdges[1] + (float)(0.0 - (float)(0.0 - barWidth));
            auto v6 = (float)((float)((float)(0.0 - barWidth) * 2.0) + m_bounds.width) - (float)(m_clientEdges[0] + m_clientEdges[2]);
            auto v7 = (float)((float)((float)(0.0 - barWidth) * 2.0) + m_bounds.height) - (float)(m_clientEdges[1] + m_clientEdges[3]);
            if (v6 < 0.0)
            {
                v4 = (float)(this->m_bounds.width * 0.5) + this->m_bounds.x0;
                v6 = 0.0;
            }
            if (v7 < 0.0)
            {
                v5 = (float)(this->m_bounds.height * 0.5) + this->m_bounds.y0;
                v7 = 0.0;
            }
            
            BoundsBase<float> res{0.0, 0.0, 0.0, 0.0};
            res.x0 = v4;
            res.y0 = v5;
            res.width = v6;
            res.height = v7;
            return res;
        }

        int Wnd::OnBeforeRemoveFromWndStation()
        {
            auto res = 1;
            for (auto child = GetFirstChild_(); child; child = child->GetNextSibling_())
            {
                M3D_ASSERT(child->IsKindOf(RT_CLASS_LOCAL(Wnd)));
                auto wnd = dynamic_cast<Wnd*>(child);
                res &= wnd->OnBeforeAddToWndStation();
            }
            if (m_bSuspendedUnlink)
            {
                if (res)
                {
                    return !IsAnimatingNow();
                }
                return 0;
            }
            if (IsAnimatingNow() && m_currentAnimation.m_purpose == AnimationInfo::PURPOSE_HIDE)
            {
                return 0;
            }
            if (m_bSuspendedParentUnlink
                || !GetStation()->IsAnimationEnabled()
                || !m_onHideAnimation.CanAnimate())
            {
                return res;
            }
            if (!res)
                return 0;
            return StartAnimation(this->m_onHideAnimation, IsAnimatingNow()) == 0;
        }

        void Wnd::SetFormatMode(TextFormatFlags format)
        {
            m_textFormat = format;
        }

        Wnd::AnimationInfo const& Wnd::GetOnHideAnimation() const
        {
            return m_onHideAnimation;
        }

        void Wnd::SetClientEdges(std::vector<float> const& edges)
        {
            m_clientEdges = edges;
        }

        void Wnd::SetClientEdges(float left, float top, float right, float bottom)
        {
            m_clientEdges[0] = left;
            m_clientEdges[1] = top;
            m_clientEdges[2] = right;
            m_clientEdges[3] = bottom;
        }

        void Wnd::AdjustToFitChildren()
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> const& Wnd::GetBaseOrigin() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetOnShowAnimation(AnimationInfo const& info)
        {
            m_onShowAnimation = info;
            m_onShowAnimation.m_purpose = AnimationInfo::PURPOSE_SHOW;
        }

        int Wnd::GameDataClear(bool)
        {
            throw std::logic_error("Not implemented");
        }

        TextWrapFlags Wnd::GetWrapMode() const
        {
            return m_textWrap;
        }

        bool Wnd::Valid() const
        {
            return m_created == 1;
        }

        int Wnd::RemoveChild(Object* w)
        {
            //TODO: check this
            auto wnd = dynamic_cast<Wnd*>(w);
            if (this != GetStation() && !wnd->IsChildOf(GetStation()))
            {
                wnd->m_bSuspendedUnlink = false;
                wnd->m_bSuspendedParentUnlink = false;
                //TODO: recreate vector logic (idk for what)
                //std::vector<Object*> stack;
                //stack.push_back(wnd);
                GetStation()->OnRemoveWnd(this, wnd);
                UnlinkChild(wnd);
                if (wnd)
                {
                    wnd->OnAfterRemoveFromWndStation();
                }
                return 1;
            }
            if ((wnd->OnBeforeRemoveFromWndStation() & 1) != 0)
            {
                wnd->m_bSuspendedUnlink = false;
                wnd->m_bSuspendedParentUnlink = false;
                //TODO: recreate vector logic (idk for what)
                //std::vector<Object*> stack;
                //stack.push_back(wnd);
                GetStation()->OnRemoveWnd(this, wnd);
                UnlinkChild(wnd);
                if (wnd)
                {
                    wnd->OnAfterRemoveFromWndStation();
                }
                return 1;
            }
            wnd->m_bSuspendedUnlink = true;
            return 0;
        }

        TextFormatFlags Wnd::GetFormatMode() const
        {
            return m_textFormat;
        }

        float Wnd::GetFrameWidth() const
        {
            auto pane = GetGfxServer()->GetPane(m_paneName);
            if (pane)
            {
                auto frame = pane->m_frame[0];
                if ((m_style & 0x40) == 0
                    && (m_paneFlags & 2) != 0
                    && !m_bgTexture.IsValid()
                    && frame)
                {
                    return frame->m_barUsedWidth;
                }
            }
            return 0.0;
        }

        bool Wnd::IsAnimatingNow() const
        {
            return IsChildOf(GetStation()) &&
                m_currentAnimation.m_animationType != AnimationInfo::ANIMATIONTYPE_INVALID &&
                m_currentAnimation.m_bEnabled;
        }

        int Wnd::GetPropertiesList(std::set<unsigned>&) const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::SetWrapMode(TextWrapFlags wrap)
        {
            m_textWrap = wrap;
        }

        Wnd::AnimationInfo const& Wnd::GetOnShowAnimation() const
        {
            return m_onShowAnimation;
        }

        int Wnd::GetProperty(unsigned propId, void* prop) const
        {
            if (Object::GetProperty(propId, prop))
            {
                return 1;
            }
            if (propId != 0x4000)
            {
                return 0;
            }
            if (prop)
            {
                *static_cast<CStr*>(prop) = m_toolTipText;
            }
            return 1;
        }

        std::vector<float> const& Wnd::GetClientEdges() const
        {
            return m_clientEdges;
        }

        void Wnd::SetOnHideAnimation(AnimationInfo const& info)
        {
            m_onHideAnimation = info;
            m_onHideAnimation.m_purpose = AnimationInfo::PURPOSE_HIDE;
        }

        void Wnd::SetBaseOrigin(PointBase<float> const& baseOrigin)
        {
            m_baseOrigin = baseOrigin;
        }

        void Wnd::RemoveTooltip()
        {
            if (m_toolTipWnd && this == GetStation()->m_wndForTooltip)
            {
                if (GetStation()->IsDirectChild(m_toolTipWnd))
                {
                    GetStation()->RemoveChild(m_toolTipWnd);
                }
                delete m_toolTipWnd;
                GetStation()->m_wndForTooltip = nullptr;
            }
            m_toolTipTimeOut = -1;
        }

        void Wnd::DrawWndText(DrawInfo const& di)
        {
            //TODO: implement Wnd::DrawWndText
            if (!m_caption.empty())
            {
                //throw std::logic_error("Not implemented");
            }
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
            m_gotFocus = true;
            return 1;
        }

        int Wnd::OnLoosingFocus()
        {
            throw std::logic_error("Not implemented");
        }

        int Wnd::CreateWnd(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            m_created = true;
            SetText(m_wndStation->InitializeStringUsingIds(caption));
            SetBounds(rc, true);
            SetId(id);
            if (style)
            {
                SetStyle(style);
            }
            Register();
            return 1;
        }

        Wnd::Wnd() :
            m_bounds(0.0, 0.0, 0.0, 0.0),
            m_clientEdges(4, 0.0)
        {
        }

        Wnd::Wnd(Wnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Wnd::GetOriginPoint() const
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::DrawNonClient(DrawInfo const& di, unsigned clr)
        {
            OnNcPaint(di, clr);
        }

        void Wnd::StopAnimationMoveSound()
        {
            if (m_animationSoundMoveChannelId != -1 && Application::g_pApp->m_sound)
            {
                Application::g_pApp->m_sound->StopChannel(m_animationSoundMoveChannelId);
                m_animationSoundMoveChannelId = -1;
            }
        }

        void Wnd::OnNcPaint(DrawInfo const& di, unsigned clr)
        {
            //TODO: check this and refactor
            BoundsBase<float> v4; // eax
            float v5; // xmm1_4
            float v6; // xmm3_4
            m3d::ui::Wnd* v7; // ecx
            BoundsBase<float> v8; // eax
            BoundsBase<float> v9; // eax
            float v10; // xmm0_4
            float v11; // xmm1_4
            int v12; // eax
            int v13[2]; // [esp+8h] [ebp-94h] BYREF
            float v14; // [esp+10h] [ebp-8Ch]
            float v15; // [esp+14h] [ebp-88h]
            BoundsBase<float> b; // [esp+18h] [ebp-84h] BYREF
            BoundsBase<float> rect; // [esp+28h] [ebp-74h] BYREF
            BoundsBase<float> bb; // [esp+38h] [ebp-64h] BYREF
            char v19[16]; // [esp+48h] [ebp-54h] BYREF

            if ((this->m_style & 0x8000) != 0)
            {
                v4 = this->GetBounds();
                v5 = v4.width;
                v6 = v4.height;
                rect.x0 = v5;
                rect.width = (v5 + 15.0) - v5;
                rect.y0 = 15.0;
                rect.height = (v6 + 15.0) - 15.0;
                bb.x0 = 15.0;
                bb.y0 = v6;
                bb.width = v5 - 15.0;
                bb.height = (v6 + 15.0) - v6;


                m3d::ui::DrawInfo new_di(di);
                v7 = dynamic_cast<Wnd*>(this->GetParent());
                if (v7)
                {
                    v8 = v7->GetBounds();
                    v13[0] = 0;
                    v13[1] = 0;
                    v14 = v8.width;
                    v15 = v8.height;
                    new_di.m_clippedRect.x0 = 0.0;
                    new_di.m_clippedRect.y0 = 0.0;
                    new_di.m_clippedRect.width = v14;
                    new_di.m_clippedRect.height = v15;
                }
                GetGfxServer()->AddFlatAxialQuad(new_di, rect, 0x80000000);
                GetGfxServer()->AddFlatAxialQuad(new_di, bb, 0x80000000);
            }
            v9 = this->GetBounds();
            v10 = v9.height;
            v11 = v9.width;
            b.x0 = 0.0;
            b.y0 = 0.0;
            b.width = v11;
            b.height = v10;
            if (!m_bgTexture.IsValid())
                GetGfxServer()->AddFlatAxialPane0(di,
                    b,
                    clr,
                    this->m_paneFlags,
                    this->m_paneName,
                    this->m_bgFlags);
            else
                GetGfxServer()->AddImagedRect(di, b, clr, m_bgTexture);
        }

        void Wnd::Unregister()
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::Register()
        {
            if (m_wndStation)
            {
                m_wndStation->RegisterWnd(this);
            }
        }

        int Wnd::DestroyWnd()
        {
            //TODO: check this and refactor
            auto v2 = GetParent();
            if (v2)
                v2->RemoveChild(this);
            RemoveAllChildren();
            if (this->m_toolTipWnd)
                RemoveTooltip();
            if (this->m_bgTexture.IsValid())
                m3d::Application::g_pApp->m_renderer->ReleaseTexture(this->m_bgTexture);
            StopAnimationMoveSound();
            if (m3d::ui::Wnd::m_wndStation)
                m_wndStation->UnregisterWnd(this);
            this->m_created = 0;
            return 1;
        }

        void Wnd::FinishDragMove(int, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void Wnd::StartDragMove(PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        int LoadExistingDialog(Wnd* destWnd, CStr const& name)
        {
            CStr err;
            ref_ptr xmlFile = ReadXmlFile(name.c_str(), &err);
            if (xmlFile)
            {
                ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                xmlFile->GetFirstChild_(node, "Prefabs");
                node->GetFirstChild_(node, "Node");
                auto attr = node->GetAttribute("class");
                if (attr)
                {
                    if (!destWnd->IsKindOf(attr))
                    {
                        M3D_LOG_INFO("LoadExistingDialog (" + name + "): warning! classes mismatches, existing: " + CStr(attr));
                    }
                    destWnd->ReadFromXmlNode(xmlFile, node);
                }
                return 1;
            }
            M3D_LOG_INFO("LoadDialog: " + err);
            return 0;
        }

        BoundsBase<float> strToBounds(CStr const& s)
        {
            BoundsBase<float> res{0.0, 0.0, 0.0, 0.0};
            if (!s.empty())
            {
                sscanf(s.c_str(), "%f %f %f %f", &res.x0, &res.y0, &res.width, &res.height);
            }
            return res;
        }

        PointBase<float> strToPoint(CStr const& s)
        {
            PointBase<float> res{ 0.0, 0.0 };
            if (!s.empty())
            {
                sscanf(s.c_str(), "%f %f", &res.x, &res.y);
            }
            return res;
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
            m_toolTipText = *static_cast<CStr*>(prop);
            return 1;
        }

        int Wnd::OnTick(int curTime, int deltaTime)
        {
            if ((m_style & 0x20000) != 0)
            {
                GetStation()->CheckForMouseClick(this, false, nullptr);
            }
            if (!m_toolTipText.empty())
            {
	            if (m_toolTipTimeOut > 0)
	            {
                    m_toolTipTimeOut = m_toolTipTimeOut - deltaTime;
                    if (m_toolTipTimeOut < 0)
                    {
                        m_toolTipTimeOut = 0;
                    }
	            }
                if (m_toolTipWnd)
                {
	                if (!m_toolTipTimeOut)
	                {
                        Application::g_pApp->EnqueueMessage(45, reinterpret_cast<int>(this), 0, 0, 0, {}, {});
                        m_toolTipTimeOut = -1;
	                }
                }
                else if (!m_toolTipTimeOut)
                {
                    m_toolTipWnd = CreateTooltipWnd();
                    Application::g_pApp->EnqueueMessage(44, reinterpret_cast<int>(this), 0, 0, 0, {}, {});
                    m_toolTipTimeOut = 3000;
                }
            }
            if (m_currentAnimation.m_animationType != AnimationInfo::ANIMATIONTYPE_INVALID && m_currentAnimation.m_bEnabled)
            {
                ProcessAnimation(curTime, deltaTime);
            }
            return 1;
        }

        int Wnd::ReflectChildNotifyToParent(Wnd*, unsigned, unsigned, AIParam const&)
        {
            //TODO: ...
            throw std::logic_error("Not implemented");
        }

        int Wnd::OnMouseIn()
        {
            m_mouseOver = true;
            if (!m_toolTipText.empty() && GetParent())
            {
                m_toolTipTimeOut = 500;
            }
            if ((m_style & 0x40000) == 0)
                return 1;

            auto wnd = dynamic_cast<Wnd*>(GetParent());
            if (wnd)
            {
                GetStation()->AddNotifyForWnd(this, wnd, 7, {}, (m_style & 0x400000) != 0);
            }
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
            //TODO: check and refactor this
            if ((m_style & 0x40000) != 0)
            {
                if ((m_style & 0x20000) != 0)
                {
                    if (!state)
                    {
                    LABEL_6:
                        m_mouseDown &= 0xFEu;
                        goto LABEL_7;
                    }
                }
                else if (!state)
                {
                    if ((m_mouseDown &1) != 0)
                    {
                        AIParam const param{CVector2{at.x, at.y}};
                        CallParentNotify(1u, param, false);
                    }
                    goto LABEL_6;
                }
                m_mouseDown |= 1u;
            }
        LABEL_7:
            auto parentWnd = dynamic_cast<Wnd*>(GetParent());
            if ((m_style & 0x20) != 0 && parentWnd)
            {
                auto const att = ToParent(at);
                parentWnd->OnMouseButton0(state, att);
            }
            auto const bounds = GetBounds();
            if (at.x < 0.0 || bounds.width <= at.x || at.y < 0.0 || bounds.height <= at.y)
            {
                return 0;
            }
            if ((m_style & 8) != 0)
            {
                if (!state || m_dragMode)
                {
                    if (m_dragMode == DRAG_MOVE)
                    {
                        DoDragMove(at);
                        GetStation()->CaptureMouse(nullptr);
                        m_bounds.x0 = m_dragCurPt.x;
                        m_bounds.y0 = m_dragCurPt.y;
                        m_dragMode = DRAG_NONE;
                    }
                }
                else
                {
                    StartDragMove(at);
                }
            }
            if ((m_style & 0x2000) != 0)
            {
                GetStation()->Activate(this);
            }
            RemoveTooltip();
            return 1;
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

        int Wnd::OnPaint(DrawInfo const& di)
        {
            auto v3 = this->m_style;
            if ((v3 & 0x40) == 0)
            {
                auto v4 = 0;
                if ((v3 & 2) != 0 || (v3 & 0x80000) != 0)
                    v4 = 3;
                else
                    v4 = this->m_curClr;
                this->OnNcPaint(di, v4);
            }
            this->DrawWndText(di);
            return 1;
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

        RT_CLASS_EXPORTS_BEGIN(ModalWnd)
    	RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(ModalWnd);

        Class* ModalWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Object* ModalWnd::CreateObject()
        {
            return new ModalWnd;
        }

        int ModalWnd::DoModal()
        {
            throw std::logic_error("Not implemented");
        }

        int ModalWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            return Wnd::Create(caption, style, rc, id) != 0;
        }

        int ModalWnd::CanClose()
        {
            throw std::logic_error("Not implemented");
        }

        int ModalWnd::IsModal()
        {
            throw std::logic_error("Not implemented");
        }

        Class* ModalWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(ModalWnd);
        }

        Wnd* ModalWnd::GetDlgItem(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Object* ModalWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        ModalWnd::~ModalWnd()
        {
            if (GetStation()->IsModal(this))
            {
                GetStation()->EndModal(this, 0);
            }
        }

        int ModalWnd::OnInitDlgItem(Wnd*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void ModalWnd::OnCloseModal(int)
        {
        }

        int ModalWnd::OnInitModal()
        {
            return 1;
        }

        int ModalWnd::OnPaint(DrawInfo const& clipToIt)
        {
            if ((m_style & 0x40) == 0)
            {
                auto color = m_curClr;
                if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
                {
                    color = 3;
                }
                OnNcPaint(clipToIt, color);
            }
            DrawWndText(clipToIt);
            //TODO: check this
            //if (this->m_curControl)
            //    this->m_curControl->GetBounds(this->m_curControl, &rc);
            return 1;
        }

        int ModalWnd::CloseModal(int)
        {
            throw std::logic_error("Not implemented");
        }

        int ModalWnd::OnKey(unsigned short, unsigned char, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int ModalWnd::OnWndNotify(Wnd* from, unsigned idFrom, unsigned msg, AIParam const& data)
        {
            if ((m_style & 0x100000) != 0)
            {
                auto wnd = dynamic_cast<Wnd*>(GetParent());
                if (wnd)
                {
                    wnd->OnWndNotify(from, idFrom, msg, data);
                }
            }
            if (msg != 1 || !idFrom || idFrom > 3)
            {
                return 0;
            }
            if (GetStation()->IsModal(this))
            {
                CloseModal(idFrom);
            }
            return 1;
        }

        ModalWnd::ModalWnd(ModalWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        ModalWnd::ModalWnd()
        {
            m_style |= 0x40;
        }
    }
}

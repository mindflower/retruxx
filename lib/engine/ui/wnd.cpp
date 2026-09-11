#include <config.h>
#include <m3dapp.h>
#include <core/clazz.h>
#include <stdexcept>
#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <core/log.h>
#include "core/ini.h"
#include <core/ref_ptr.h>
#include <math/vector2.h>
#include <server/utils.h>
#include <core/console/cvar.h>
#include <ui/cursor.h>
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

        namespace
        {
            struct _AnimationType2Str
            {
                /* 0x0000 */ ui::Wnd::AnimationInfo::AnimationType m_type;
                /* 0x0004 */ char const* m_name;
            }; /* size: 0x0008 */

            _AnimationType2Str l_animationType2Str[17] = {
                {Wnd::AnimationInfo::ANIMATIONTYPE_USER, "USER"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_LEFT, "TO_LEFT"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFT, "TO_BEYOND_LEFT"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_RIGHT, "TO_RIGHT"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHT, "TO_BEYOND_RIGHT"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_TOP, "TO_TOP"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BEYOND_TOP, "TO_BEYOND_TOP"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BOTTOM, "TO_BOTTOM"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BEYOND_BOTTOM, "TO_BEYOND_BOTTOM"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_LEFTTOP, "TO_LEFTTOP"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFTTOP, "TO_BEYOND_LEFTTOP"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_LEFTBOTTOM, "TO_LEFTBOTTOM"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFTBOTTOM, "TO_BEYOND_LEFTBOTTOM"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_RIGHTTOP, "TO_RIGHTTOP"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHTTOP, "TO_BEYOND_RIGHTTOP"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_RIGHTBOTTOM, "TO_RIGHTBOTTOM"},
                {Wnd::AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHTBOTTOM, "TO_BEYOND_RIGHTBOTTOM"},
            };
        }  // namespace

        Wnd::AnimationInfo::AnimationInfo()
        {
            m_bEnabled = 1;
            m_startPt.x = 0.0;
            m_startPt.y = 0.0;
            m_endPt.x = 0.0;
            m_endPt.y = 0.0;
            m_animationType = ANIMATIONTYPE_INVALID;
            m_startSpeed = 0.0;
            m_acceleration = 0.0;
            m_curSpeed = 0.0;
            m_delayTime = 0;
            m_startTime = 0;
            m_purpose = PURPOSE_UNKNOWN;
            m_bImmediate = 0;
            m_bSoundMoveEnabled = 0;
            m_bSoundStopEnabled = 0;
        }

        Wnd::AnimationInfo::~AnimationInfo()
        {
        }

        Wnd::AnimationInfo::AnimationType Wnd::AnimationInfo::Str2AnimationType(CStr const& str) const
        {
            for (auto const& anim : l_animationType2Str)
            {
                if (str == anim.m_name)
                {
                    return anim.m_type;
                }
            }
            return ANIMATIONTYPE_INVALID;
        }

        void Wnd::AnimationInfo::Invalidate()
        {
            m_animationType = ANIMATIONTYPE_INVALID;
            m_purpose = PURPOSE_UNKNOWN;
        }

        bool Wnd::AnimationInfo::IsValid() const
        {
            return m_animationType != ANIMATIONTYPE_INVALID;
        }

        int Wnd::AnimationInfo::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
        {
            if (!file || !node)
            {
                return 0;
            }

            SafeBoolAttrib(m_bEnabled, node, "animationEnabled");

            CStr strAnimationType;
            SafeStrAttrib(strAnimationType, node, "animationType");
            m_animationType = Str2AnimationType(strAnimationType);

            if (m_animationType == ANIMATIONTYPE_INVALID)
            {
                return 0;
            }

            if (m_animationType == ANIMATIONTYPE_USER)
            {
                CVector2 vStartPt;
                CVector2 vEndPt;
                SafeVector2Attrib(vStartPt, node, "animationStartPt");
                SafeVector2Attrib(vEndPt, node, "animationEndPt");
                m_startPt.x = vStartPt.x;
                m_startPt.y = vStartPt.y;
                m_endPt.x = vEndPt.x;
                m_endPt.y = vEndPt.y;
            }

            SafeFloatAttrib(m_startSpeed, node, "animationSpeed");
            SafeFloatAttrib(m_acceleration, node, "animationAccel");
            SafeUintAttrib(m_delayTime, node, "animationDelayTime");
            SafeBoolAttrib(m_bImmediate, node, "Immediate");
            SafeStrAttrib(m_soundMoveName, node, "soundMoveName");
            SafeStrAttrib(m_soundStopName, node, "soundStopName");
            SafeBoolAttrib(m_bSoundMoveEnabled, node, "soundMoveEnabled");
            SafeBoolAttrib(m_bSoundStopEnabled, node, "soundStopEnabled");

            return 1;
        }

        CStr Wnd::AnimationInfo::AnimationType2Str(AnimationType animationType) const
        {
            for (auto const& anim : l_animationType2Str)
            {
                if (anim.m_type == animationType)
                {
                    return anim.m_name ? CStr(anim.m_name) : CStr();
                }
            }
            return CStr();
        }

        bool Wnd::AnimationInfo::CanAnimate() const
        {
            return m_animationType != ANIMATIONTYPE_INVALID && m_bEnabled;
        }

        int Wnd::AnimationInfo::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
        {
            if (!file || !node || m_animationType == ANIMATIONTYPE_INVALID)
            {
                return 0;
            }
            node->SetAttribute("animationEnabled", CStr(static_cast<int>(m_bEnabled)).c_str());
            node->SetAttribute("animationType", AnimationType2Str(m_animationType).c_str());
            if (m_animationType == ANIMATIONTYPE_USER)
            {
                node->SetAttribute("animationStartPt", (CStr(m_startPt.x) + CStr(" ") + CStr(m_startPt.y)).c_str());
                node->SetAttribute("animationEndPt", (CStr(m_endPt.x) + CStr(" ") + CStr(m_endPt.y)).c_str());
            }
            node->SetAttribute("animationSpeed", CStr(m_startSpeed).c_str());
            node->SetAttribute("animationAccel", CStr(m_acceleration).c_str());
            node->SetAttribute("animationDelayTime", CStr(m_delayTime).c_str());
            node->SetAttribute("Immediate", CStr(static_cast<int>(m_bImmediate)).c_str());
            node->SetAttribute("soundMoveName", m_soundMoveName.c_str());
            node->SetAttribute("soundStopName", m_soundStopName.c_str());
            node->SetAttribute("soundMoveEnabled", CStr(static_cast<int>(m_bSoundMoveEnabled)).c_str());
            node->SetAttribute("soundStopEnabled", CStr(static_cast<int>(m_bSoundStopEnabled)).c_str());
            return 1;
        }

        void Wnd::AnimationInfo::SetupDefaultOnHide()
        {
            auto const& cfg = g_Kernel->GetEngineCfg();
            m_bEnabled = 1;
            m_startPt.x = 0.0;
            m_startPt.y = 0.0;
            m_endPt.x = 0.0;
            m_endPt.y = 0.0;
            m_animationType = Str2AnimationType(CStr(cfg.m_ui_defaultWndAnimationHideType.GetS()));
            m_startSpeed = cfg.m_ui_defaultWndAnimationHideSpeed.GetF();
            m_acceleration = cfg.m_ui_defaultWndAnimationHideAccel.GetF();
            m_delayTime = 0;
            m_purpose = PURPOSE_HIDE;
            m_bImmediate = 0;
        }

        void Wnd::AnimationInfo::SetupDefaultOnShow()
        {
            auto const& cfg = g_Kernel->GetEngineCfg();
            m_bEnabled = 1;
            m_startPt.x = 0.0;
            m_startPt.y = 0.0;
            m_endPt.x = 0.0;
            m_endPt.y = 0.0;
            m_animationType = Str2AnimationType(CStr(cfg.m_ui_defaultWndAnimationShowType.GetS()));
            m_startSpeed = cfg.m_ui_defaultWndAnimationShowSpeed.GetF();
            m_acceleration = cfg.m_ui_defaultWndAnimationShowAccel.GetF();
            m_curSpeed = 0.0;
            m_delayTime = 0;
            m_purpose = PURPOSE_SHOW;
            m_bImmediate = 0;
        }

        void Wnd::AnimationInfo::Setup(
            PointBase<float> const& startPt,
            PointBase<float> const& endPt,
            float startSpeed,
            float acceleration,
            unsigned int delayTime)
        {
            // Inlined at every call site in the original; reconstructed from the field
            // semantics used by StartAnimation / ProcessAnimation.
            m_bEnabled = 1;
            m_startPt = startPt;
            m_endPt = endPt;
            m_animationType = ANIMATIONTYPE_USER;
            m_startSpeed = startSpeed;
            m_acceleration = acceleration;
            m_curSpeed = startSpeed;
            m_delayTime = delayTime;
            m_bImmediate = 0;
        }

        void Wnd::AnimationInfo::Setup(
            AnimationType animationType,
            float startSpeed,
            float acceleration,
            unsigned int delayTime)
        {
            // Inlined at every call site in the original; reconstructed from the field
            // semantics used by StartAnimation / ProcessAnimation.
            m_bEnabled = 1;
            m_animationType = animationType;
            m_startSpeed = startSpeed;
            m_acceleration = acceleration;
            m_curSpeed = startSpeed;
            m_delayTime = delayTime;
            m_bImmediate = 0;
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
            return m_uniqueId;
        }

        bool Wnd::IsVisible() const
        {
            return (GetStyle() & WS_IS_VISIBLE) != 0;
        }

        void Wnd::ShowWindow(bool show)
        {
            if (show)
            {
                m_style |= WS_IS_VISIBLE;
            }
            else
            {
                m_style &= ~WS_IS_VISIBLE;
            }
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
            return m_guiId;
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
            int res = 1;
            for (auto* wnd = RT_DYNCAST(GetFirstChild(), Wnd); wnd != nullptr;
                 wnd = RT_DYNCAST(wnd->GetNextSibling(), Wnd))
            {
                res &= wnd->OnBeforeAddToWndStation();
            }
            return res;
        }

        int Wnd::OnAfterAddToWndStation()
        {
            int res = 1;
            for (auto* wnd = RT_DYNCAST(GetFirstChild(), Wnd); wnd != nullptr;
                 wnd = RT_DYNCAST(wnd->GetNextSibling(), Wnd))
            {
                res &= wnd->OnAfterAddToWndStation();
            }
            if ((!IsAnimatingNow() || m_currentAnimation.m_purpose != AnimationInfo::PURPOSE_SHOW) &&
                GetStation()->IsAnimationEnabled() && m_onShowAnimation.CanAnimate())
            {
                Wnd::StartAnimation(m_onShowAnimation, IsAnimatingNow());
            }
            return res;
        }

        void Wnd::SetGuiId(int guiId)
        {
            m_guiId = guiId;
        }

        BoundsBase<float> Wnd::ToParent(BoundsBase<float> const& b) const
        {
            PointBase<float> tl = ToParent(PointBase<float>{b.x0, b.y0});
            PointBase<float> br = ToParent(PointBase<float>{b.x0 + b.width, b.y0 + b.height});

            BoundsBase<float> res;
            res.x0 = tl.x;
            res.y0 = tl.y;
            res.width = br.x - tl.x;
            res.height = br.y - tl.y;
            return res;
        }

        PointBase<float> Wnd::ToParent(PointBase<float> const& pt) const
        {
            PointBase<float> res = ToScreen(pt);
            if (auto* parentWnd = RT_DYNCAST(GetParent(), Wnd))
            {
                PointBase<float> parentToScreenRes = parentWnd->ToScreen(PointBase<float>{0.0, 0.0});
                res.x = res.x - parentToScreenRes.x;
                res.y = res.y - parentToScreenRes.y;
                return res;
            }
            return res;
        }

        void Wnd::SetTextColorDisabled(unsigned textColor)
        {
            m_textColorDisabled = textColor;

            char tmp[128] = {0};
            unsigned clr = GetGfxServer()->GetColor(textColor);
            sprintf(tmp, "%08x", clr);

            m_strTextColorDisabled = CStr("@") + tmp;
        }

        void Wnd::SetTextColor(unsigned textColor)
        {
            m_textColor = textColor;

            char tmp[128] = {0};
            unsigned clr = GetGfxServer()->GetColor(textColor);
            sprintf(tmp, "%08x", clr);

            m_strTextColor = CStr("@") + tmp;
        }

        int Wnd::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
        {
            if (!Object::WriteToXmlNode(file, writeTo))
            {
                return 0;
            }

            writeTo->SetAttribute(
                "org",
                CStr::format_("%.3f %.3f %.3f %.3f", m_bounds.x0, m_bounds.y0, m_bounds.width, m_bounds.height)
                    .c_str());
            writeTo->SetAttribute("style", CStr(m_style).c_str());
            writeTo->SetAttribute("order", CStr(m_activationOrder).c_str());
            writeTo->SetAttribute("id", CStr(m_id).c_str());
            writeTo->SetAttribute("caption", m_caption.c_str());
            writeTo->SetAttribute("tip", m_toolTipText.c_str());
            writeTo->SetAttribute("backimage", m_bgTextureName.c_str());
            writeTo->SetAttribute("paneName", m_paneName.c_str());
            writeTo->SetAttribute("scrollPaneName", m_scrollPaneName.c_str());
            writeTo->SetAttribute("paneFlags", CStr(m_paneFlags).c_str());
            writeTo->SetAttribute("wrap", CStr(static_cast<int>(m_textWrap)).c_str());
            writeTo->SetAttribute("format", CStr(static_cast<int>(m_textFormat)).c_str());
            writeTo->SetAttribute("font", CStr(m_defFont).c_str());
            writeTo->SetAttribute("clientEdges", ai::FloatVectorToStr(m_clientEdges).c_str());

            CStr clr;
            clr.format("%08x", m_curClr);
            writeTo->SetAttribute("wndColor", clr.c_str());
            CStr textClr;
            textClr.format("%08x", m_textColor);
            writeTo->SetAttribute("textColor", textClr.c_str());
            CStr textClrDisabled;
            textClrDisabled.format("%08x", m_textColorDisabled);
            writeTo->SetAttribute("textColorDisabled", textClrDisabled.c_str());

            ref_ptr<cmn::XmlNode> animationsNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "Animations");
            writeTo->AddChild(animationsNode);
            if (m_onShowAnimation.IsValid())
            {
                ref_ptr<cmn::XmlNode> showNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "AnimationOnShow");
                animationsNode->AddChild(showNode);
                m_onShowAnimation.WriteToXmlNode(file, showNode);
            }
            if (m_onHideAnimation.IsValid())
            {
                ref_ptr<cmn::XmlNode> hideNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "AnimationOnHide");
                animationsNode->AddChild(hideNode);
                m_onHideAnimation.WriteToXmlNode(file, hideNode);
            }
            return 1;
        }

        bool Wnd::GetCursorShow() const
        {
            return m_showCursor;
        }

        int Wnd::OnAfterRemoveFromWndStation()
        {
            auto res = 1;
            for (auto child = GetFirstChild(); child; child = child->GetNextSibling())
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

        void Wnd::SetGameDataFlags(int flags)
        {
            m_gameDataFlags = flags;
        }

        PointBase<float> Wnd::GetOrigin() const
        {
            return PointBase<float>{m_bounds.x0, m_bounds.y0};
        }

        int Wnd::StartAnimation(AnimationInfo const& animationInfo, bool interpolateWithPrevious)
        {
            // TODO: generated code Wnd::StartAnimation
            // Create a copy of the current animation before modifying it
            AnimationInfo prevAnimation = m_currentAnimation;

            // Copy the new animation info
            m_currentAnimation = animationInfo;

            // Check if current animation is valid and enabled
            if (m_currentAnimation.m_animationType != AnimationInfo::ANIMATIONTYPE_INVALID &&
                m_currentAnimation.m_bEnabled)
            {
                // Get the station this window belongs to
                WndStation* station = GetStation();

                // Check if window is a child of the station
                if (!m3d::Object::IsChildOf(station))
                {
                    // Invalid animation - window not attached to station
                    m_currentAnimation.m_animationType = AnimationInfo::ANIMATIONTYPE_INVALID;
                    m_currentAnimation.m_purpose = AnimationInfo::PURPOSE_UNKNOWN;
                    return false;
                }
            }
            else
            {
                // Animation is invalid or disabled
                return false;
            }

            if (m_currentAnimation.m_bImmediate)
            {
                // Immediate animation - jump to end position
                m_bounds.x0 = m_currentAnimation.m_endPt.x;
                m_bounds.y0 = m_currentAnimation.m_endPt.y;
                OnEndAnimation(false);
            }
            else
            {
                // Animated transition
                auto* parent = RT_DYNCAST(GetParent(), Wnd);
                if (!parent)
                    return false;

                // Initialize animation parameters
                m_currentAnimation.m_curSpeed = m_currentAnimation.m_startSpeed;
                m_currentAnimation.m_startTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();

                // Get parent bounds for relative positioning
                BoundsBase<float> parentB = parent->GetBounds();

                // Handle different animation types
                switch (m_currentAnimation.m_animationType)
                {
                case AnimationInfo::ANIMATIONTYPE_USER:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_LEFT:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = parentB.width;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    m_currentAnimation.m_endPt.x = m_baseOrigin.x;
                    m_currentAnimation.m_endPt.y = m_baseOrigin.y;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFT:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_baseOrigin.x;
                        m_currentAnimation.m_startPt.y = m_baseOrigin.y;
                    }
                    m_currentAnimation.m_endPt.x = 0.0f - m_bounds.width;
                    m_currentAnimation.m_endPt.y = m_bounds.y0;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_RIGHT:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = 0.0f - m_bounds.width;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    m_currentAnimation.m_endPt.x = m_baseOrigin.x;
                    m_currentAnimation.m_endPt.y = m_baseOrigin.y;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHT:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_baseOrigin.x;
                        m_currentAnimation.m_startPt.y = m_baseOrigin.y;
                    }
                    m_currentAnimation.m_endPt.x = parentB.width;
                    m_currentAnimation.m_endPt.y = m_bounds.y0;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_TOP:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = parentB.height;
                    }
                    m_currentAnimation.m_endPt.x = m_baseOrigin.x;
                    m_currentAnimation.m_endPt.y = m_baseOrigin.y;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_TOP:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_baseOrigin.x;
                        m_currentAnimation.m_startPt.y = m_baseOrigin.y;
                    }
                    m_currentAnimation.m_endPt.x = m_bounds.x0;
                    m_currentAnimation.m_endPt.y = 0.0f - m_bounds.height;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BOTTOM:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = 0.0f - m_bounds.height;
                    }
                    m_currentAnimation.m_endPt.x = m_baseOrigin.x;
                    m_currentAnimation.m_endPt.y = m_baseOrigin.y;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_BOTTOM:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_baseOrigin.x;
                        m_currentAnimation.m_startPt.y = m_baseOrigin.y;
                    }
                    m_currentAnimation.m_endPt.x = m_bounds.x0;
                    m_currentAnimation.m_endPt.y = parentB.height;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_LEFTTOP:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = parentB.width;
                        m_currentAnimation.m_startPt.y = parentB.height;
                    }
                    m_currentAnimation.m_endPt.x = m_baseOrigin.x;
                    m_currentAnimation.m_endPt.y = m_baseOrigin.y;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFTTOP:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_baseOrigin.x;
                        m_currentAnimation.m_startPt.y = m_baseOrigin.y;
                    }
                    m_currentAnimation.m_endPt.x = 0.0f - m_bounds.width;
                    m_currentAnimation.m_endPt.y = 0.0f - m_bounds.height;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_LEFTBOTTOM:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = parentB.width;
                        m_currentAnimation.m_startPt.y = 0.0f - m_bounds.height;
                    }
                    m_currentAnimation.m_endPt.x = m_baseOrigin.x;
                    m_currentAnimation.m_endPt.y = m_baseOrigin.y;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFTBOTTOM:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_baseOrigin.x;
                        m_currentAnimation.m_startPt.y = m_baseOrigin.y;
                    }
                    m_currentAnimation.m_endPt.x = 0.0f - m_bounds.width;
                    m_currentAnimation.m_endPt.y = parentB.height;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_RIGHTTOP:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = 0.0f - m_bounds.width;
                        m_currentAnimation.m_startPt.y = parentB.height;
                    }
                    m_currentAnimation.m_endPt.x = m_baseOrigin.x;
                    m_currentAnimation.m_endPt.y = m_baseOrigin.y;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHTTOP:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_baseOrigin.x;
                        m_currentAnimation.m_startPt.y = m_baseOrigin.y;
                    }
                    m_currentAnimation.m_endPt.x = parentB.width;
                    m_currentAnimation.m_endPt.y = 0.0f - m_bounds.height;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_RIGHTBOTTOM:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = 0.0f - m_bounds.width;
                        m_currentAnimation.m_startPt.y = 0.0f - m_bounds.height;
                    }
                    m_currentAnimation.m_endPt.x = m_baseOrigin.x;
                    m_currentAnimation.m_endPt.y = m_baseOrigin.y;
                    break;

                case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHTBOTTOM:
                    if (interpolateWithPrevious)
                    {
                        m_currentAnimation.m_startPt.x = m_bounds.x0;
                        m_currentAnimation.m_startPt.y = m_bounds.y0;
                    }
                    else
                    {
                        m_currentAnimation.m_startPt.x = m_baseOrigin.x;
                        m_currentAnimation.m_startPt.y = m_baseOrigin.y;
                    }
                    m_currentAnimation.m_endPt.x = parentB.width;
                    m_currentAnimation.m_endPt.y = parentB.height;
                    break;

                default:
                    break;
                }

                // Set current position to animation start point
                m_bounds.x0 = m_currentAnimation.m_startPt.x;
                m_bounds.y0 = m_currentAnimation.m_startPt.y;

                // Handle animation sound
                StopAnimationMoveSound();
                if (m_currentAnimation.m_bSoundMoveEnabled)
                {
                    CStr soundName;
                    CStr const* pSoundName = nullptr;

                    if (m_currentAnimation.m_soundMoveName.empty())
                    {
                        soundName = "CONTROL_SOUND_ANIMATION_MOVE_DEFAULT";
                        pSoundName = &soundName;
                    }
                    else
                    {
                        pSoundName = &m_currentAnimation.m_soundMoveName;
                    }

                    m_animationSoundMoveChannelId = m_gfx->PlayControlSound(*pSoundName, 0);
                }
            }

            return true;
        }

        unsigned Wnd::GetColor() const
        {
            return m_curClr;
        }

        ScrollWnd* Wnd::GetScrollVWnd()
        {
            return m_scrollVWnd;
        }

        ScrollWnd* Wnd::GetScrollHWnd()
        {
            return m_scrollHWnd;
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

            retruxx::vector<float> vecClientEdges;
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
            xmlNode->GetFirstChild(animationsNode, "Animations");
            if (!animationsNode->IsEmpty())
            {
                ref_ptr onShowAnimationNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                animationsNode->GetFirstChild(onShowAnimationNode, "AnimationOnShow");
                if (!onShowAnimationNode->IsEmpty())
                {
                    AnimationInfo animationInfo;
                    animationInfo.ReadFromXmlNode(xmlFile, onShowAnimationNode);
                    SetOnShowAnimation(animationInfo);
                }

                ref_ptr onHideAnimationNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                animationsNode->GetFirstChild(onHideAnimationNode, "AnimationOnHide");
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
            return m_int;
        }

        void Wnd::SetStyle(unsigned style)
        {
            m_style = style;
        }

        int Wnd::GameDataSave(cmn::XmlFile*, cmn::XmlNode*)
        {
            return 1;
        }

        Object* Wnd::Clone()
        {
            return new Wnd(*this);
        }

        int Wnd::RemoveChildForce(Object* obj)
        {
            // TODO: generated code
            auto* w = dynamic_cast<Wnd*>(obj);
            // Reset suspension flags
            w->m_bSuspendedUnlink = false;
            w->m_bSuspendedParentUnlink = false;

            // Create a stack for depth-first traversal of child hierarchy
            std::vector<ui::Wnd*> childStack;
            childStack.push_back(w);

            // Process all children in depth-first order
            while (!childStack.empty())
            {
                // Get the next window from stack
                ui::Wnd* currentWnd = childStack.back();
                childStack.pop_back();

                // Traverse all siblings of current window
                ui::Wnd* child = dynamic_cast<Wnd*>(currentWnd->GetFirstChild());
                while (child != nullptr)
                {
                    // Reset parent unlink suspension flag
                    child->m_bSuspendedParentUnlink = false;

                    // If this child has children, add it to stack for processing
                    if (child->GetFirstChild() != nullptr)
                    {
                        childStack.push_back(child);
                    }

                    // Move to next sibling
                    child = dynamic_cast<Wnd*>(child->GetNextSibling());
                }
            }

            // Notify window station about removal
            m_wndStation->OnRemoveWnd(this, w);

            // Actually unlink the child from parent
            UnlinkChild(w);

            // If this window is part of the window station hierarchy,
            // notify the removed window about being removed from station
            if (this == m_wndStation || IsChildOf(m_wndStation))
            {
                w->OnAfterRemoveFromWndStation();
            }

            return 1;
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
            return m_gameDataFlags;
        }

        void Wnd::SetOrigin(PointBase<float> const& pt)
        {
            m_bounds.x0 = pt.x;
            m_bounds.y0 = pt.y;
        }

        m3d::ui::PaneFlagBg Wnd::GetBgFlags() const
        {
            return m_bgFlags;
        }

        void Wnd::SetBgFlags(m3d::ui::PaneFlagBg flags)
        {
            m_bgFlags = flags;
        }

        Wnd* Wnd::CaptureMouse()
        {
            return GetStation()->CaptureMouse(this);
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

        void Wnd::SetInt(unsigned ii)
        {
            m_int = ii;
        }

        void Wnd::EnableWindow(bool bEnable)
        {
            if (bEnable)
                m_style &= ~WS_DISABLE;
            else
                m_style |= WS_DISABLE;
        }

        void Wnd::EnableOnShowAnimation(bool bEnable)
        {
            m_onShowAnimation.m_bEnabled = bEnable;
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
                retruxx::vector<Object*> stack;
                stack.push_back(w);
                while (!stack.empty())
                {
                    Object* cur = stack.back();
                    stack.pop_back();
                    for (auto* child = RT_DYNCAST(cur->GetFirstChild(), Wnd); child;
                         child = RT_DYNCAST(child->GetNextSibling(), Wnd))
                    {
                        child->m_bSuspendedParentUnlink = false;
                        if (child->GetFirstChild())
                        {
                            stack.push_back(child);
                        }
                    }
                }
                if (m_wndStation->IsAnimationEnabled() && wnd->GetOnShowAnimation().CanAnimate())
                {
                    wnd->StartAnimation(wnd->GetOnShowAnimation(), true);
                }
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
            return (m_style & WS_DISABLE) == 0;
        }

        void Wnd::GrayWindow(bool bGray)
        {
            if (bGray)
                m_style |= WS_GRAYED;
            else
                m_style &= ~WS_GRAYED;
        }

        bool Wnd::IsGrayed() const
        {
            return (m_style & WS_GRAYED) != 0;
        }

        void Wnd::EnableOnHideAnimation(bool bEnable)
        {
            m_onHideAnimation.m_bEnabled = bEnable;
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

        void Wnd::SetOnShowAnimationImmediate(bool bImmediate)
        {
            m_onShowAnimation.m_bImmediate = bImmediate;
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
            auto* parentWnd = RT_DYNCAST(GetParent(), Wnd);
            if (!parentWnd)
            {
                return;
            }
            auto parentB = parentWnd->GetBounds();
            auto self = GetBounds();
            self.x0 = (parentB.width - self.width) * 0.5f;
            self.y0 = (parentB.height - self.height) * 0.5f;
            SetBounds(self, true);
        }

        Class* Wnd::GetClass() const
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Wnd::AnimationInfo const& Wnd::GetCurrentAnimation() const
        {
            return m_currentAnimation;
        }

        int Wnd::GameDataLoad(cmn::XmlFile*, cmn::XmlNode*)
        {
            return 1;
        }

        void Wnd::SetId(unsigned id)
        {
            m_id = id;
        }

        void Wnd::StopAnimation(bool returnToBaseOrigin)
        {
            if (returnToBaseOrigin)
            {
                m_bounds.x0 = m_baseOrigin.x;
                m_bounds.y0 = m_baseOrigin.y;
            }
            OnEndAnimation(true);
        }

        PointBase<float> Wnd::ToWindow(PointBase<float> const& pt) const
        {
            PointBase<float> tl = ToScreen(PointBase<float>{0.0, 0.0});
            return PointBase<float>{pt.x - tl.x, pt.y - tl.y};
        }

        BoundsBase<float> Wnd::ToWindow(BoundsBase<float> const& b) const
        {
            PointBase<float> tl = ToScreen(PointBase<float>{0.0, 0.0});
            float x0 = b.x0 - tl.x;
            float y0 = b.y0 - tl.y;
            PointBase<float> br = ToScreen(PointBase<float>{0.0, 0.0});

            BoundsBase<float> res;
            res.x0 = x0;
            res.y0 = y0;
            res.width = ((b.x0 + b.width) - br.x) - x0;
            res.height = ((b.y0 + b.height) - br.y) - y0;
            return res;
        }

        void Wnd::AdjustForWndTextToFit(unsigned uiFont, float maxWidth)
        {
            float x = 0.0f;
            float y = 0.0f;
            if (!m_caption.empty())
            {
                auto sz = GetGfxServer()->MeasureText(m_caption, static_cast<int>(uiFont), m_textWrap, maxWidth);
                x = sz.x;
                y = sz.y;
            }
            m_bounds.width = x + 5.0f;
            m_bounds.height = y + 5.0f;
        }

        int Wnd::GetPaneFlags() const
        {
            return m_paneFlags;
        }

        void Wnd::SetOnHideAnimationImmediate(bool bImmediate)
        {
            m_onHideAnimation.m_bImmediate = bImmediate;
        }

        PointBase<float> Wnd::ToScreen(PointBase<float> const& pt) const
        {
            //TODO: check this!!!
            PointBase<float> res = pt;
            for (auto obj = this; obj; obj = dynamic_cast<Wnd const*>(obj->GetParent()))
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
            return 1;
        }

        BoundsBase<float> Wnd::GetClientBounds() const
        {
            // RVA 0x677F50 - the client area is the window inset by its frame on
            // every side and then by the four client edges.
            auto const barWidth = GetFrameWidth();

            float x0 = m_clientEdges[0] + barWidth;
            float y0 = m_clientEdges[1] + barWidth;
            float width = m_bounds.width - 2.0f * barWidth - (m_clientEdges[0] + m_clientEdges[2]);
            float height = m_bounds.height - 2.0f * barWidth - (m_clientEdges[1] + m_clientEdges[3]);

            // A window too small for its own insets collapses onto its centre.
            // NOTE: those two fallbacks are in absolute coordinates while the
            // normal path is relative to the window; that is how it shipped.
            if (width < 0.0)
            {
                x0 = m_bounds.x0 + m_bounds.width * 0.5f;
                width = 0.0;
            }
            if (height < 0.0)
            {
                y0 = m_bounds.y0 + m_bounds.height * 0.5f;
                height = 0.0;
            }

            // NOTE: BoundsBase's four-argument constructor takes corners, not
            // extents, so the fields are set individually here.
            BoundsBase<float> res;
            res.x0 = x0;
            res.y0 = y0;
            res.width = width;
            res.height = height;
            return res;
        }


        int Wnd::OnBeforeRemoveFromWndStation()
        {
            int res = 1;
            for (auto child = GetFirstChild(); child; child = child->GetNextSibling())
            {
                M3D_ASSERT(child->IsKindOf(RT_CLASS_LOCAL(Wnd)));
                auto wnd = dynamic_cast<Wnd*>(child);
                res &= wnd->OnBeforeRemoveFromWndStation();
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
            if (m_bSuspendedParentUnlink || !GetStation()->IsAnimationEnabled() || !m_onHideAnimation.CanAnimate())
            {
                return res;
            }
            if (!res)
            {
                return 0;
            }
            return StartAnimation(m_onHideAnimation, IsAnimatingNow()) == 0;
        }

        void Wnd::SetFormatMode(TextFormatFlags format)
        {
            m_textFormat = format;
        }

        Wnd::AnimationInfo const& Wnd::GetOnHideAnimation() const
        {
            return m_onHideAnimation;
        }

        void Wnd::SetClientEdges(retruxx::vector<float> const& edges)
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
            // RVA 0x676E50 - grow the bounds to cover every child plus a corner
            // margin. The tail (bounds.width = w + cornerSz, likewise height) and
            // the zeroed accumulators are unambiguous in the disassembly.
            //
            // NOTE: the shipped loop also writes back into each child and calls
            // the child's SetBounds. IDA mislabels that block's stack slots by a
            // whole BoundsBase and every candidate reading is nonsensical, so it
            // has not been reproduced - this pass only measures the children.
            float const cornerSz = static_cast<float>(GetGfxServer()->GetCornerSz());
            float w = 0.0f;
            float h = 0.0f;
            for (auto* child = RT_DYNCAST(GetFirstChild(), Wnd); child;
                 child = RT_DYNCAST(child->GetNextSibling(), Wnd))
            {
                auto b = child->GetBounds();
                if (b.x0 + b.width > w)
                {
                    w = b.x0 + b.width;
                }
                if (b.y0 + b.height > h)
                {
                    h = b.y0 + b.height;
                }
            }
            auto bounds = GetBounds();
            bounds.width = w + cornerSz;
            bounds.height = h + cornerSz;
            SetBounds(bounds, true);
        }

        PointBase<float> const& Wnd::GetBaseOrigin() const
        {
            return m_baseOrigin;
        }

        void Wnd::SetOnShowAnimation(AnimationInfo const& info)
        {
            m_onShowAnimation = info;
            m_onShowAnimation.m_purpose = AnimationInfo::PURPOSE_SHOW;
        }

        int Wnd::GameDataClear(bool)
        {
            return 1;
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
            // TODO: generated code
            ui::Wnd* targetWnd = (ui::Wnd*)w;
            bool canRemove = false;

            // Check if we can remove from WndStation
            if (this != GetStation() && !IsChildOf(GetStation()))
            {
                canRemove = true;
            }
            else
            {
                // Call virtual function to check if removal is allowed
                if ((targetWnd->OnBeforeRemoveFromWndStation() & 1) != 0)
                {
                    canRemove = true;
                }
                else
                {
                    // Removal not allowed - suspend unlinking for this window and its children
                    targetWnd->m_bSuspendedUnlink = true;

                    std::vector<ui::Wnd*> stack;
                    stack.push_back(targetWnd);

                    // Traverse children and mark them as suspended
                    while (!stack.empty())
                    {
                        ui::Wnd* current = stack.back();
                        stack.pop_back();

                        // Process all children
                        ui::Wnd* child = (ui::Wnd*)current->GetFirstChild();
                        while (child != nullptr)
                        {
                            child->m_bSuspendedParentUnlink = true;

                            // If child has children, add to stack
                            if (child->GetFirstChild() != nullptr)
                            {
                                stack.push_back(child);
                            }

                            child = (ui::Wnd*)child->GetNextSibling();
                        }
                    }
                    return 0;
                }
            }

            // Proceed with removal
            if (canRemove)
            {
                targetWnd->m_bSuspendedUnlink = false;
                targetWnd->m_bSuspendedParentUnlink = false;

                std::vector<ui::Wnd*> stack;
                stack.push_back(targetWnd);

                // Traverse and process all descendants
                while (!stack.empty())
                {
                    ui::Wnd* current = stack.back();
                    stack.pop_back();

                    // Process all children of current node
                    ui::Wnd* child = (ui::Wnd*)current->GetFirstChild();
                    while (child != nullptr)
                    {
                        // Clear some flag (based on the original BYTE1(i[10].m_name.m_charPtr) = 0)
                        // This appears to be resetting a flag on the child
                        child->m_bSuspendedUnlink = false;  // Simplified interpretation

                        // If child has children, add to stack
                        if (child->GetFirstChild() != nullptr)
                        {
                            stack.push_back(child);
                        }

                        child = (ui::Wnd*)child->GetNextSibling();
                    }
                }

                // Notify WndStation and perform actual unlinking
                GetStation()->OnRemoveWnd(this, targetWnd);
                Object::UnlinkChild(targetWnd);

                // Call post-removal callback if needed
                if (canRemove)
                {
                    targetWnd->OnAfterRemoveFromWndStation();
                }

                return 1;
            }

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
                if ((m_style & 0x40) == 0 && (m_paneFlags & 2) != 0 && !m_bgTexture.IsValid() && frame)
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

        int Wnd::GetPropertiesList(retruxx::set<unsigned>& properties) const
        {
            // The original does not emit a distinct Wnd override - the vtable slot
            // resolves to Object::GetPropertiesList.
            return Object::GetPropertiesList(properties);
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

        retruxx::vector<float> const& Wnd::GetClientEdges() const
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
            if (!m_caption.empty())
            {
                auto textColor = m_strTextColor;
                if ((m_style & 2) != 0 || (m_style & 0x80000) != 0)
                {
                    textColor = m_strTextColorDisabled;
                }
                auto measureText = GetGfxServer()->MeasureText(m_caption, m_defFont, m_textWrap, di.m_clientRect.width);
                auto origin = measureText;
                switch (m_textFormat)
                {
                case TF_CENTER:
                {
                    origin.x = di.m_clientRect.width * 0.5;
                    if (origin.x < 0.0)
                    {
                        origin.x = 0.0;
                    }
                    if (origin.x > di.m_clientRect.width)
                    {
                        origin.x = di.m_clientRect.width;
                    }
                    break;
                }
                case TF_LEFT:
                case TF_FULL:
                {
                    if ((m_style & 0x400) != 0)
                    {
                        origin.x = (di.m_clientRect.width - measureText.x) * 0.5;
                        if (origin.x < 0.0)
                        {
                            origin.x = 0.0;
                        }
                        if (origin.x > di.m_clientRect.width)
                        {
                            origin.x = di.m_clientRect.width;
                        }
                        break;
                    }
                    origin.x = 0.0;
                    break;
                }
                case TF_RIGHT:
                {
                    if ((m_style & 0x400) != 0)
                    {
                        origin.x = (di.m_clientRect.width + measureText.x) * 0.5;
                        if (origin.x < 0.0)
                        {
                            origin.x = 0.0;
                        }
                        if (origin.x > di.m_clientRect.width)
                        {
                            origin.x = di.m_clientRect.width;
                        }
                    }
                    else
                    {
                        origin.x = di.m_clientRect.width;
                    }
                    break;
                }
                default:
                    break;
                }
                if ((m_style & 0x800) != 0)
                    origin.y = (di.m_clientRect.height - measureText.y) * 0.5;
                else
                    origin.y = 0.0;
                GetGfxServer()->AddText(di, origin, textColor + m_caption, m_defFont, m_textWrap, m_textFormat);
            }
        }

        int Wnd::ProcessAnimation(int curTime, int deltaTime)
        {
            // TODO: generated code Wnd::ProcessAnimation
            // Check if animation is valid and enabled
            if (m_currentAnimation.m_animationType == AnimationInfo::ANIMATIONTYPE_INVALID ||
                !m_currentAnimation.m_bEnabled)
            {
                return false;
            }

            // Check if delay time has passed
            if (curTime - m_currentAnimation.m_startTime >= m_currentAnimation.m_delayTime)
            {
                // Get animation points
                float const endX = m_currentAnimation.m_endPt.x;
                float const endY = m_currentAnimation.m_endPt.y;
                float const startX = m_currentAnimation.m_startPt.x;
                float const startY = m_currentAnimation.m_startPt.y;

                // Calculate current position delta
                float const deltaX = endX - m_bounds.x0;
                float const deltaY = endY - m_bounds.y0;

                // Update speed with acceleration
                float accelerationEffect = (m_currentAnimation.m_acceleration * static_cast<float>(deltaTime)) * 0.001f;
                m_currentAnimation.m_curSpeed += accelerationEffect;

                // Calculate movement for this frame
                float const speedFactor = (m_currentAnimation.m_curSpeed * static_cast<float>(deltaTime)) * 0.001f;

                // Normalize movement vector
                float distance = sqrtf(deltaX * deltaX + deltaY * deltaY + 1.1920929e-7f);
                if (distance < 1.0e-12f)
                {
                    // Already at destination
                    m_bounds.x0 = endX;
                    m_bounds.y0 = endY;
                    OnEndAnimation(false);
                    return true;
                }

                float invDistance = 1.0f / distance;

                // Calculate new position
                float newX = m_bounds.x0 + (deltaX * invDistance) * speedFactor;
                float newY = m_bounds.y0 + (deltaY * invDistance) * speedFactor;

                // Check if animation should end
                if (startX == endX && startY == endY)
                {
                    // Start and end points are the same
                    m_bounds.x0 = endX;
                    m_bounds.y0 = endY;
                    OnEndAnimation(false);
                    return true;
                }

                // Calculate vector from new position to end point
                float toEndX = endX - newX;
                float toEndY = endY - newY;

                // Calculate normalized direction vector to end point
                float toEndDistance = sqrtf(toEndX * toEndX + toEndY * toEndY + 1.1920929e-7f);
                float invToEndDistance = 1.0f / toEndDistance;
                float normToEndX = toEndX * invToEndDistance;
                float normToEndY = toEndY * invToEndDistance;

                // Calculate normalized direction vector from start to end
                float startToEndX = endX - startX;
                float startToEndY = endY - startY;
                float startToEndDistance = sqrtf(startToEndX * startToEndX + startToEndY * startToEndY + 1.1920929e-7f);
                float invStartToEndDistance = 1.0f / startToEndDistance;
                float normStartToEndX = startToEndX * invStartToEndDistance;
                float normStartToEndY = startToEndY * invStartToEndDistance;

                // Check if we've passed the end point or are very close to it
                float directionDifference = (normStartToEndX - normToEndX) * (normStartToEndX - normToEndX) +
                    (normStartToEndY - normToEndY) * (normStartToEndY - normToEndY);

                bool shouldEndAnimation = false;

                if (directionDifference > 0.001f)
                {
                    // Direction has changed significantly (passed the end point)
                    shouldEndAnimation = true;
                }
                else if (normToEndX * normToEndX + normToEndY * normToEndY <= 0.001f)
                {
                    // Very close to the end point
                    shouldEndAnimation = true;
                }

                if (shouldEndAnimation)
                {
                    // Snap to end point and finish animation
                    m_bounds.x0 = endX;
                    m_bounds.y0 = endY;
                    OnEndAnimation(false);
                }
                else
                {
                    // Update to interpolated position
                    m_bounds.x0 = newX;
                    m_bounds.y0 = newY;
                }
            }

            return true;
        }

        Wnd* Wnd::CreateTooltipWnd()
        {
            auto* wnd = new Wnd;
            wnd->Create(m_toolTipText, 0xF00u, BoundsBase<float>{}, 0);
            wnd->m_textWrap = TW_WORD_WRAP;
            wnd->SetDefaultFont(2);
            wnd->SetClientEdges(12.0f, 8.0f, 12.0f, 8.0f);
            wnd->SetPane("PaneTooltip");
            wnd->SetTextColor(0xFF404040u);
            wnd->SetFormatMode(TF_CENTER);

            auto textSz = GetGfxServer()->MeasureText(m_toolTipText, wnd->m_defFont, wnd->m_textWrap, 300.0f);
            float frameW = wnd->GetFrameWidth();

            BoundsBase<float> b;
            b.width = (frameW + 12.0f) * 2.0f + textSz.x;
            b.height = (frameW + 8.0f) * 2.0f + textSz.y;

            PointBase<float> org{static_cast<float>(M3D_APP->GetMouseX()), static_cast<float>(M3D_APP->GetMouseY())};
            M3D_RENDERER->AbsToRel(org.x, org.y);
            b.x0 = org.x - b.width * 0.5f;
            b.y0 = org.y;

            Cursor cur;
            float y0 = b.y0;
            if (GetCursor(cur))
            {
                y0 = cur.m_sz.y + b.y0;
            }

            float dx = 0.0f;
            float dy = 0.0f;
            if (b.width + b.x0 > 1024.0f)
                dx = 1024.0f - (b.width + b.x0);
            if (b.height + y0 > 768.0f)
                dy = 768.0f - (b.height + y0);
            if (b.x0 < 0.0f)
                dx = 0.0f - b.x0;
            if (y0 < 0.0f)
                dy = 0.0f - y0;
            b.x0 += dx;
            b.y0 = y0 + dy;

            wnd->SetBounds(b, true);
            return wnd;
        }

        void Wnd::OnEndAnimation(bool bUrgent)
        {
            if (m_currentAnimation.m_animationType != AnimationInfo::ANIMATIONTYPE_INVALID)
            {
                if (m_animationSoundMoveChannelId != -1)
                {
                    if (M3D_APP->m_sound)
                    {
                        M3D_APP->m_sound->StopChannel(m_animationSoundMoveChannelId);
                    }
                    m_animationSoundMoveChannelId = -1;
                }
                if (m_currentAnimation.m_bSoundStopEnabled)
                {
                    CStr stopName = m_currentAnimation.m_soundStopName;
                    if (stopName.empty())
                    {
                        stopName = "CONTROL_SOUND_ANIMATION_STOP_DEFAULT";
                    }

                    bool bLooped = false;
                    GetGfxServer()->PlayControlSound(stopName, &bLooped);
                }
                m_currentAnimation.m_animationType = AnimationInfo::ANIMATIONTYPE_INVALID;
                m_currentAnimation.m_purpose = AnimationInfo::PURPOSE_UNKNOWN;
                if (bUrgent)
                {
                    M3D_APP->ImmediateMessage(42, (int)this, 0, 0, 0, {}, {});
                }
                else
                {
                    M3D_APP->EnqueueMessage(42, (int)this, 0, 0, 0, {}, {});
                }
            }
        }

        int Wnd::OnObtainingFocus()
        {
            m_gotFocus = true;
            return 1;
        }

        int Wnd::OnLoosingFocus()
        {
            m_gotFocus = 0;
            m_mouseDown = 0;
            return 1;
        }

        int Wnd::CreateWnd(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            m_created = true;
            SetText(m_wndStation->InitializeStringUsingIds(caption));
            // RVA 0x611BF0 writes the bounds straight into the members rather
            // than calling the virtual SetBounds, so a derived class's layout
            // override does not run while the window is still half-built.
            m_bounds = rc;
            m_baseOrigin.x = rc.x0;
            m_baseOrigin.y = rc.y0;
            SetId(id);
            if (style)
            {
                SetStyle(style);
            }
            Register();
            return 1;
        }

        Wnd::Wnd() : m_bounds(0.0, 0.0, 0.0, 0.0), m_clientEdges(4, 0.0)
        {
        }

        Wnd::Wnd(Wnd const&)
        {
            // Matches the original: the copy ctor does not copy any Wnd state - it
            // chains to Object's copy ctor and leaves every Wnd member at its
            // default-constructed value (empty strings/vectors, invalid textures,
            // default AnimationInfo). Only used via Clone().
        }

        PointBase<float> Wnd::GetOriginPoint() const
        {
            PointBase<float> res{0.0, 0.0};
            if (m_scrollVWnd)
            {
                res.y = m_scrollVWnd->GetCurPos();
            }
            if (m_scrollHWnd)
            {
                res.x = m_scrollHWnd->GetCurPos();
            }
            return res;
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
            BoundsBase<float> v4;    // eax
            float v5;                // xmm1_4
            float v6;                // xmm3_4
            ui::Wnd* v7;             // ecx
            BoundsBase<float> v8;    // eax
            BoundsBase<float> v9;    // eax
            float v10;               // xmm0_4
            float v11;               // xmm1_4
            int v12;                 // eax
            int v13[2];              // [esp+8h] [ebp-94h] BYREF
            float v14;               // [esp+10h] [ebp-8Ch]
            float v15;               // [esp+14h] [ebp-88h]
            BoundsBase<float> b;     // [esp+18h] [ebp-84h] BYREF
            BoundsBase<float> rect;  // [esp+28h] [ebp-74h] BYREF
            BoundsBase<float> bb;    // [esp+38h] [ebp-64h] BYREF
            char v19[16];            // [esp+48h] [ebp-54h] BYREF

            if ((m_style & 0x8000) != 0)
            {
                v4 = GetBounds();
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

                ui::DrawInfo new_di(di);
                v7 = dynamic_cast<Wnd*>(GetParent());
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
            v9 = GetBounds();
            v10 = v9.height;
            v11 = v9.width;
            b.x0 = 0.0;
            b.y0 = 0.0;
            b.width = v11;
            b.height = v10;
            if (!m_bgTexture.IsValid())
                GetGfxServer()->AddFlatAxialPane0(di, b, clr, m_paneFlags, m_paneName, m_bgFlags);
            else
                GetGfxServer()->AddImagedRect(di, b, clr, m_bgTexture);
        }

        void Wnd::Unregister()
        {
            if (m_wndStation)
            {
                m_wndStation->UnregisterWnd(this);
            }
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
            if (m_toolTipWnd)
                RemoveTooltip();
            if (m_bgTexture.IsValid())
                M3D_RENDERER->ReleaseTexture(m_bgTexture);
            StopAnimationMoveSound();
            if (ui::Wnd::m_wndStation)
                m_wndStation->UnregisterWnd(this);
            m_created = 0;
            return 1;
        }

        void Wnd::FinishDragMove(int accept, PointBase<float> const& pt)
        {
            DoDragMove(pt);
            GetStation()->CaptureMouse(nullptr);
            if (accept)
            {
                m_bounds.x0 = m_dragCurPt.x;
                m_bounds.y0 = m_dragCurPt.y;
            }
            else
            {
                m_bounds.x0 = m_dragStartPt.x;
                m_bounds.y0 = m_dragStartPt.y;
            }
            m_dragMode = DRAG_NONE;
        }

        void Wnd::StartDragMove(PointBase<float> const& pt)
        {
            GetStation()->CaptureMouse(this);
            m_dragMode = DRAG_MOVE;
            m_dragStartPt = ToParent(pt);
            m_dragStartPtLocal = pt;
        }

        int LoadExistingDialog(Wnd* destWnd, CStr const& name)
        {
            CStr err;
            ref_ptr xmlFile = ReadXmlFile(name.c_str(), &err);
            if (xmlFile)
            {
                ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                xmlFile->GetFirstChild(node, "Prefabs");
                node->GetFirstChild(node, "Node");
                auto attr = node->GetAttribute("class");
                if (attr)
                {
                    if (!destWnd->IsKindOf(attr))
                    {
                        M3D_LOG_INFO(
                            "LoadExistingDialog (" + name + "): warning! classes mismatches, existing: " + CStr(attr));
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
            PointBase<float> res{0.0, 0.0};
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
            if (Application::g_pApp->IsTextHieroglyphic(m_caption) &&
                g_Kernel->GetEngineCfg().m_ui_forceHieroglyphicFont.GetB())
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
            if (Application::g_pApp->IsTextHieroglyphic(m_caption) &&
                g_Kernel->GetEngineCfg().m_ui_forceHieroglyphicFont.GetB())
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

        int Wnd::IsPtInBounds(PointBase<float> const& pt) const
        {
            // RVA 0x481D10 - the point is in screen space, so the window's own
            // origin is mapped over first.
            auto const org = ToScreen(PointBase<float>{});
            if (pt.x < org.x || pt.x >= org.x + m_bounds.width)
            {
                return false;
            }
            return pt.y >= org.y && pt.y < org.y + m_bounds.height;
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
            if (m_currentAnimation.m_animationType != AnimationInfo::ANIMATIONTYPE_INVALID &&
                m_currentAnimation.m_bEnabled)
            {
                ProcessAnimation(curTime, deltaTime);
            }
            return 1;
        }

        int Wnd::ReflectChildNotifyToParent(Wnd* from, unsigned id, unsigned msg, AIParam const& data)
        {
            if (auto* parentWnd = RT_DYNCAST(GetParent(), Wnd))
            {
                parentWnd->OnWndNotify(from, id, msg, data);
            }
            return 0;
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

        int Wnd::OnMouseClick(PointBase<float> const& pt)
        {
            if ((m_style & WS_SEND_NOTIFY_MESSAGES) != 0)
            {
                AIParam const data(CVector2(pt.x, pt.y));
                CallParentNotify(1u, data, false);
            }
            if ((m_style & WS_REFLECT_MS_AND_KEYS_TO_PARENT) != 0 && GetParent())
            {
                // NOTE: the original computes ToParent(pt) but forwards the original
                // window-space point to the parent.
                auto* parentWnd = RT_DYNCAST(GetParent(), Wnd);
                parentWnd->OnMouseClick(pt);
            }
            return 1;
        }

        void Wnd::OnPaintOverChildren(DrawInfo const& clipToIt)
        {
        }

        int Wnd::OnMouseButton2(unsigned state, PointBase<float> const& at)
        {
            if ((m_style & WS_SEND_NOTIFY_MESSAGES) != 0)
            {
                if (state)
                {
                    m_mouseDown |= 4u;
                }
                else
                {
                    if ((m_mouseDown & 4) != 0)
                    {
                        AIParam param(CVector2(at.x, at.y));
                        CallParentNotify(3u, param, false);
                    }
                    m_mouseDown &= ~4u;
                }
            }
            if ((m_style & WS_REFLECT_MS_AND_KEYS_TO_PARENT) != 0 && GetParent())
            {
                auto* parentWnd = RT_DYNCAST(GetParent(), Wnd);
                parentWnd->OnMouseButton2(state, ToParent(at));
            }
            return 1;
        }

        int Wnd::OnMouseButton1(unsigned state, PointBase<float> const& at)
        {
            if ((m_style & WS_SEND_NOTIFY_MESSAGES) != 0)
            {
                if (state)
                {
                    m_mouseDown |= 2u;
                }
                else
                {
                    if ((m_mouseDown & 2) != 0)
                    {
                        AIParam param(CVector2(at.x, at.y));
                        CallParentNotify(2u, param, 0);
                    }
                    m_mouseDown &= ~2u;
                }
            }
            if ((m_style & WS_REFLECT_MS_AND_KEYS_TO_PARENT) != 0 && GetParent())
            {
                auto* parentWnd = RT_DYNCAST(GetParent(), Wnd);
                parentWnd->OnMouseButton1(state, ToParent(at));
            }
            return 1;
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
                    if ((m_mouseDown & 1) != 0)
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

        int Wnd::OnMouseWheel(int ticks, PointBase<float> const& at)
        {
            if ((m_style & WS_REFLECT_MS_AND_KEYS_TO_PARENT) != 0 && GetParent())
            {
                auto* parentWnd = RT_DYNCAST(GetParent(), Wnd);
                parentWnd->OnMouseWheel(ticks, ToParent(at));
            }
            return 1;
        }

        void Wnd::OnDisplayChanged()
        {
        }

        Wnd* Wnd::GetNextActivatableChild(Wnd* first, int back)
        {
            //TODO: check this and refactor
            auto result = (Wnd*)GetFirstChild();
            auto v4 = 0;
            auto v5 = result;
            for (auto maxOrder = 0; v5; v5 = (Wnd*)v5->GetNextSibling())
            {
                if ((v5->m_style & 0x2000) != 0)
                {
                    auto v6 = &v5->m_activationOrder;
                    if (v4 >= v5->m_activationOrder)
                        v6 = &maxOrder;
                    v4 = *v6;
                    maxOrder = *v6;
                }
            }
            auto v7 = 0;
            if (first)
            {
                v7 = first->m_activationOrder + 2 * (back != 1) - 1;
                if (v7 < 0)
                {
                    v7 = v4;
                    goto LABEL_13;
                }
            }
            if (v7 > v4)
                v7 = 0;
        LABEL_13:
            while (result)
            {
                if ((result->m_style & 0x2000) != 0 && result->m_activationOrder == v7)
                    break;
                result = (Wnd*)result->GetNextSibling();
            }
            return result;
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
            auto v3 = m_style;
            if ((v3 & 0x40) == 0)
            {
                auto v4 = 0;
                if ((v3 & 2) != 0 || (v3 & 0x80000) != 0)
                    v4 = 3;
                else
                    v4 = m_curClr;
                OnNcPaint(di, v4);
            }
            DrawWndText(di);
            return 1;
        }

        int Wnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
        {
            return 0;
        }

        int Wnd::OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas)
        {
            if ((m_style & 0x20) != 0 && GetParent())
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

        void Wnd::DoDragMove(PointBase<float> const& pt)
        {
            PointBase<float> local{pt.x - m_dragStartPtLocal.x, pt.y - m_dragStartPtLocal.y};
            PointBase<float> curPt = ToParent(local);

            float x;
            float y;
            if ((m_style & WS_ALWAYS_INSIDE) != 0 && GetParent())
            {
                auto* parentWnd = RT_DYNCAST(GetParent(), Wnd);
                auto pb = parentWnd->GetBounds();
                x = curPt.x;
                y = curPt.y;
                if (pb.x0 > curPt.x)
                {
                    x = pb.x0;
                }
                if (m_bounds.width + curPt.x > pb.width + pb.x0)
                {
                    x -= (m_bounds.width + curPt.x) - (pb.width + pb.x0);
                }
                if (pb.y0 > curPt.y)
                {
                    y = pb.y0;
                }
                if (m_bounds.height + curPt.y > pb.height + pb.y0)
                {
                    y -= (m_bounds.height + curPt.y) - (pb.height + pb.y0);
                }
            }
            else
            {
                x = curPt.x;
                y = curPt.y;
            }

            m_dragCurPtLocal = pt;
            m_dragCurPt.x = x;
            m_dragCurPt.y = y;
            m_bounds.x0 = m_dragCurPt.x;
            m_bounds.y0 = m_dragCurPt.y;
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
            return GetStation()->DoModal(this);
        }

        int ModalWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            return Wnd::Create(caption, style, rc, id) != 0;
        }

        int ModalWnd::CanClose()
        {
            return 1;
        }

        int ModalWnd::IsModal()
        {
            return GetStation()->IsModal(this);
        }

        Class* ModalWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(ModalWnd);
        }

        Wnd* ModalWnd::GetDlgItem(unsigned id)
        {
            for (auto* child = RT_DYNCAST(GetFirstChild(), Wnd); child;
                 child = RT_DYNCAST(child->GetNextSibling(), Wnd))
            {
                if (child->GetId() == id)
                {
                    return child;
                }
            }
            return nullptr;
        }

        Object* ModalWnd::Clone()
        {
            return new ModalWnd(*this);
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
            return 1;
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
            //if (m_curControl)
            //    m_curControl->GetBounds(m_curControl, &rc);
            return 1;
        }

        int ModalWnd::CloseModal(int val)
        {
            M3D_APP->EnqueueMessage(39, reinterpret_cast<int>(this), val, 0, 0, {}, {});
            return 1;
        }

        int ModalWnd::OnKey(unsigned short key, unsigned char, unsigned state)
        {
            // TODO: generated code
            // Handle style 0x4000 when key is pressed (state == 1)
            if ((m_style & WS_ACTIVATION_REFLECT_TO_CHILDREN) != 0 && state == 1)
            {
                unsigned char keyLow = static_cast<unsigned char>(key & 0xFF);
                unsigned char keyHigh = static_cast<unsigned char>((key >> 8) & 0xFF);

                // Mask out certain bits from the high byte (likely modifier bits)
                keyHigh &= 0xCF;  // Clear specific bits

                // Reconstruct the key with masked high byte
                unsigned short maskedKey = static_cast<unsigned short>(keyLow | (keyHigh << 8));

                // Check if it's the Tab key (key == 3)
                if (maskedKey == 3)
                {
                    WndStation* station = GetStation();
                    Wnd* activeWnd = station->GetActive();

                    if (activeWnd && activeWnd->IsChildOf(this))
                    {
                        Wnd* nextActivatable = GetNextActivatableChild(
                            activeWnd,
                            (key & 0x3000) != 0  // Shift key check
                        );

                        if (nextActivatable && activeWnd != nextActivatable)
                        {
                            station->Activate(nextActivatable);
                        }
                    }
                    return 1;  // Key handled
                }
            }
            else if (state == 0)
            {
                // Key released - proceed to other checks
            }
            else
            {
                // State is not 0 or 1, or style doesn't match
            }

            // Handle key code 4 (likely Enter/OK button)
            if (key == 4)
            {
                auto* firstChild = RT_DYNCAST(GetFirstChild(), Wnd);

                // Look for a child with style 0x10000 (likely a default button)
                while (firstChild)
                {
                    if ((firstChild->GetStyle() & 0x10000) != 0)
                    {
                        // Found default button - simulate space key press on it
                        WndStation* station = GetStation();
                        Wnd* targetWnd = static_cast<Wnd*>(firstChild);
                        station->PulseKeyForWindow(targetWnd, 0x20u, 0x39u);
                        return 1;  // Key handled
                    }
                    firstChild = RT_DYNCAST(firstChild->GetNextSibling(), Wnd);
                }
            }

            // Handle Escape key (key == 1) for modal window
            WndStation* station = GetStation();
            if (station->IsModal(this) && state != 0 && key == 1)
            {
                CloseModal(3);  // Close with result code 3 (cancelled)
                return 1;       // Key handled
            }

            return 0;  // Key not handled
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

        ModalWnd::ModalWnd(ModalWnd const& other) : Wnd(other)
        {
        }

        ModalWnd::ModalWnd()
        {
            m_style |= 0x40;
        }
    }  // namespace ui
}  // namespace m3d

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
            // RVA 0x678590 - "TO_x" animations slide the window in from beyond the
            // x edge of its parent to its base origin, "TO_BEYOND_x" ones slide it
            // from the base origin out past that edge. With interpolateWithPrevious
            // the slide starts from wherever the window is now.
            // (The shipped code also copies the previous animation into a local it
            // never reads.)
            m_currentAnimation = animationInfo;
            AnimationInfo& anim = m_currentAnimation;

            if (anim.m_animationType == AnimationInfo::ANIMATIONTYPE_INVALID || !anim.m_bEnabled)
            {
                return false;
            }
            if (!IsChildOf(GetStation()))
            {
                anim.m_animationType = AnimationInfo::ANIMATIONTYPE_INVALID;
                anim.m_purpose = AnimationInfo::PURPOSE_UNKNOWN;
                return false;
            }

            if (anim.m_bImmediate)
            {
                m_bounds.x0 = anim.m_endPt.x;
                m_bounds.y0 = anim.m_endPt.y;
                OnEndAnimation(false);
                return true;
            }

            auto* const parent = static_cast<Wnd*>(GetParent());
            if (!parent)
            {
                return false;
            }
            anim.m_curSpeed = anim.m_startSpeed;
            anim.m_startTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
            BoundsBase<float> const parentB = parent->GetBounds();

            // Positions just beyond each edge of the parent.
            float const beyondLeft = 0.0f - m_bounds.width;
            float const beyondTop = 0.0f - m_bounds.height;
            float const beyondRight = parentB.width;
            float const beyondBottom = parentB.height;
            float const x0 = m_bounds.x0;
            float const y0 = m_bounds.y0;
            PointBase<float> const cur{x0, y0};

            // Slide in from "from" to the base origin.
            auto const slideIn = [&](float fromX, float fromY) {
                anim.m_startPt = interpolateWithPrevious ? cur : PointBase<float>{fromX, fromY};
                anim.m_endPt = m_baseOrigin;
            };
            // Slide out from the base origin to "to".
            auto const slideOut = [&](float toX, float toY) {
                anim.m_startPt = interpolateWithPrevious ? cur : m_baseOrigin;
                anim.m_endPt = PointBase<float>{toX, toY};
            };

            switch (anim.m_animationType)
            {
            case AnimationInfo::ANIMATIONTYPE_USER:
                if (interpolateWithPrevious)
                {
                    anim.m_startPt = cur;
                }
                break;
            case AnimationInfo::ANIMATIONTYPE_TO_LEFT: slideIn(beyondRight, y0); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFT: slideOut(beyondLeft, y0); break;
            case AnimationInfo::ANIMATIONTYPE_TO_RIGHT: slideIn(beyondLeft, y0); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHT: slideOut(beyondRight, y0); break;
            case AnimationInfo::ANIMATIONTYPE_TO_TOP: slideIn(x0, beyondBottom); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_TOP: slideOut(x0, beyondTop); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BOTTOM: slideIn(x0, beyondTop); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_BOTTOM: slideOut(x0, beyondBottom); break;
            case AnimationInfo::ANIMATIONTYPE_TO_LEFTTOP: slideIn(beyondRight, beyondBottom); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFTTOP: slideOut(beyondLeft, beyondTop); break;
            case AnimationInfo::ANIMATIONTYPE_TO_LEFTBOTTOM: slideIn(beyondRight, beyondTop); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_LEFTBOTTOM: slideOut(beyondLeft, beyondBottom); break;
            case AnimationInfo::ANIMATIONTYPE_TO_RIGHTTOP: slideIn(beyondLeft, beyondBottom); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHTTOP: slideOut(beyondRight, beyondTop); break;
            case AnimationInfo::ANIMATIONTYPE_TO_RIGHTBOTTOM: slideIn(beyondLeft, beyondTop); break;
            case AnimationInfo::ANIMATIONTYPE_TO_BEYOND_RIGHTBOTTOM: slideOut(beyondRight, beyondBottom); break;
            default: break;
            }

            m_bounds.x0 = anim.m_startPt.x;
            m_bounds.y0 = anim.m_startPt.y;

            StopAnimationMoveSound();
            if (anim.m_bSoundMoveEnabled)
            {
                CStr const soundName =
                    anim.m_soundMoveName.empty() ? CStr("CONTROL_SOUND_ANIMATION_MOVE_DEFAULT") : anim.m_soundMoveName;
                m_animationSoundMoveChannelId = m_gfx->PlayControlSound(soundName, nullptr);
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
            // RVA 0x6751C0
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

            // Integer attributes are parsed with atoi and left untouched when absent.
            int value = 0;
            if (SafeIntAttrib(value, xmlNode, "style"))
            {
                m_style = value;
            }
            SafeIntAttrib(m_activationOrder, xmlNode, "order");
            if (SafeIntAttrib(value, xmlNode, "id"))
            {
                m_id = value;
            }

            CStr caption;
            CStr tip;
            SafeStrAttrib(caption, xmlNode, "caption");
            SafeStrAttrib(tip, xmlNode, "tip");
            SetText(GetStation()->InitializeStringUsingIds(caption));
            m_toolTipText = GetStation()->InitializeStringUsingIds(tip);
            bool const hasBackImage = SafeStrAttrib(m_bgTextureName, xmlNode, "backimage") != 0;
            SafeStrAttrib(m_paneName, xmlNode, "paneName");

            // Pane flags and the font are always set (through the virtual setters),
            // falling back to 0 when the attribute is absent.
            int paneFlags = 0;
            SafeIntAttrib(paneFlags, xmlNode, "paneFlags");
            SetPaneFlags(paneFlags);
            SafeEnumAttrib(m_textWrap, xmlNode, "wrap");
            SafeEnumAttrib(m_textFormat, xmlNode, "format");
            int font = 0;
            SafeIntAttrib(font, xmlNode, "font");
            SetDefaultFont(font);

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

            // Keyed on the attribute being present, not on the name being non-empty:
            // backimage="" releases the current background.
            if (hasBackImage)
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

        std::uintptr_t Wnd::GetInt() const
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
            // RVA 0xA12590 - RemoveChild without the veto.
            // Sets m_bSuspendedParentUnlink on every window below root (not on root itself). The
            // shipped code treats every child as a Wnd without checking.
            auto const setSuspendedParentUnlinkOnSubtree = [](Wnd* root, bool value)
            {
                std::vector<Object*> stack;
                stack.push_back(root);
                while (!stack.empty())
                {
                    Object* current = stack.back();
                    stack.pop_back();
                    for (Object* child = current->GetFirstChild(); child; child = child->GetNextSibling())
                    {
                        static_cast<Wnd*>(child)->m_bSuspendedParentUnlink = value;
                        if (child->GetFirstChild())
                        {
                            stack.push_back(child);
                        }
                    }
                }
            };

            Wnd* const w = static_cast<Wnd*>(obj);
            w->m_bSuspendedUnlink = false;
            w->m_bSuspendedParentUnlink = false;
            setSuspendedParentUnlinkOnSubtree(w, false);
            m_wndStation->OnRemoveWnd(this, w);
            UnlinkChild(w);
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
                Application::g_pApp->m_renderer->SetTextureParameter(m_bgTexture, rend::TM_WRAP_S, 3u);
                Application::g_pApp->m_renderer->SetTextureParameter(m_bgTexture, rend::TM_WRAP_T, 3u);
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

        void Wnd::SetInt(std::uintptr_t ii)
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
            // RVA 0x41C420 - adds the origins of this window and all its ancestors.
            // The shipped code treats every ancestor as a Wnd without checking.
            PointBase<float> res = pt;
            for (Object const* obj = this; obj; obj = obj->GetParent())
            {
                res.x += static_cast<Wnd const*>(obj)->m_bounds.x0;
                res.y += static_cast<Wnd const*>(obj)->m_bounds.y0;
            }
            return res;
        }

        BoundsBase<float> Wnd::ToScreen(BoundsBase<float> const& b) const
        {
            // RVA 0x444F60 - the extents are rebuilt from the mapped corners, with the
            // matching float rounding.
            PointBase<float> const tl = ToScreen(PointBase<float>{b.x0, b.y0});
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
            // RVA 0xA12370 - a window in the station may veto its removal (typically to play its
            // hide animation first). It then stays linked, marked as suspended, and its subtree
            // is marked as having a suspended parent; the station unlinks it later.

            // Sets m_bSuspendedParentUnlink on every window below root (not on root itself). The
            // shipped code treats every child as a Wnd without checking.
            auto const setSuspendedParentUnlinkOnSubtree = [](Wnd* root, bool value)
            {
                std::vector<Object*> stack;
                stack.push_back(root);
                while (!stack.empty())
                {
                    Object* current = stack.back();
                    stack.pop_back();
                    for (Object* child = current->GetFirstChild(); child; child = child->GetNextSibling())
                    {
                        static_cast<Wnd*>(child)->m_bSuspendedParentUnlink = value;
                        if (child->GetFirstChild())
                        {
                            stack.push_back(child);
                        }
                    }
                }
            };

            Wnd* const targetWnd = static_cast<Wnd*>(w);
            bool const inStation = this == m_wndStation || IsChildOf(m_wndStation);
            if (inStation && (targetWnd->OnBeforeRemoveFromWndStation() & 1) == 0)
            {
                targetWnd->m_bSuspendedUnlink = true;
                setSuspendedParentUnlinkOnSubtree(targetWnd, true);
                return 0;
            }

            targetWnd->m_bSuspendedUnlink = false;
            targetWnd->m_bSuspendedParentUnlink = false;
            setSuspendedParentUnlinkOnSubtree(targetWnd, false);
            m_wndStation->OnRemoveWnd(this, targetWnd);
            UnlinkChild(targetWnd);
            if (inStation)
            {
                targetWnd->OnAfterRemoveFromWndStation();
            }
            return 1;
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
            // RVA 0x678CF0 - moves the window towards the animation's end point at
            // the current (accelerating) speed, and snaps it there once it has
            // overshot or is close enough.
            //
            // The shipped code works on 3D vectors with z = 0 and normalises them as
            // v / sqrt(|v|^2 + FLT_EPSILON). The z terms always vanish, so they are
            // left out here, but the order of the remaining additions is kept.
            // NOTE: those sums and square roots run on the x87 FPU, whose precision
            // depends on the control word the renderer leaves behind; this float
            // version can differ from them in the last bit.
            AnimationInfo& anim = m_currentAnimation;
            if (anim.m_animationType == AnimationInfo::ANIMATIONTYPE_INVALID || !anim.m_bEnabled)
            {
                return false;
            }
            if (curTime - anim.m_startTime < anim.m_delayTime)
            {
                return true;
            }

            float constexpr eps = 1.1920929e-7f;
            PointBase<float> const endPt = anim.m_endPt;
            PointBase<float> const startPt = anim.m_startPt;

            float const toEndX = endPt.x - m_bounds.x0;
            float const toEndY = endPt.y - m_bounds.y0;
            anim.m_curSpeed += (anim.m_acceleration * static_cast<float>(deltaTime)) * 0.001f;
            float const step = (anim.m_curSpeed * static_cast<float>(deltaTime)) * 0.001f;
            float const invLen = 1.0f / sqrtf(toEndY * toEndY + toEndX * toEndX + eps);
            PointBase<float> curPt{(invLen * toEndX) * step + m_bounds.x0, (toEndY * invLen) * step + m_bounds.y0};

            // NOTE: a zero-length animation ends here, but the shipped code does not
            // return: it carries on, and the check below always snaps it to the end
            // point (the second OnEndAnimation call is then a no-op).
            if (startPt.x == endPt.x && startPt.y == endPt.y)
            {
                OnEndAnimation(false);
            }

            // Direction from the new position to the end point...
            float const restX = endPt.x - curPt.x;
            float const restY = endPt.y - curPt.y;
            float const invRest = 1.0f / sqrtf(restY * restY + restX * restX + eps);
            float const restDirX = invRest * restX;
            float const restDirY = restY * invRest;

            // ...against the direction of the whole path.
            float const pathX = endPt.x - startPt.x;
            float const pathY = endPt.y - startPt.y;
            float const invPath = 1.0f / sqrtf(pathY * pathY + pathX * pathX + eps);
            float const diffY = pathY * invPath - restDirY;
            float const diffX = invPath * pathX - restDirX;

            bool const overshot = diffY * diffY + diffX * diffX > 0.001;
            bool const arrived = restDirY * restDirY + restDirX * restDirX <= 0.001;
            if (overshot || arrived)
            {
                curPt = endPt;
                OnEndAnimation(false);
            }
            m_bounds.x0 = curPt.x;
            m_bounds.y0 = curPt.y;
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
                    M3D_APP->ImmediateMessage(42, this, 0, 0, 0, {}, {});
                }
                else
                {
                    M3D_APP->EnqueueMessage(42, this, 0, 0, 0, {}, {});
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
            // RVA 0x6771F0 - an optional drop shadow (a strip down the right edge and
            // one along the bottom, offset by 15), then the background: the image
            // when one is set, otherwise the pane.
            float constexpr shadowOffset = 15.0f;
            // The binary tests bit 0x8000; the WS_DROPSHADOW enumerator reads 0xffff8000
            // (sign-extended), so it cannot be used as a mask here.
            if ((m_style & 0x8000) != 0)
            {
                BoundsBase<float> const bounds = GetBounds();
                float const w = bounds.width;
                float const h = bounds.height;
                // NOTE: the shipped code builds these from corners, so the widths and
                // heights carry that rounding; the right strip also starts at y = 15
                // but is only h tall (its height is (h + 15) - 15).
                BoundsBase<float> rightStrip;
                rightStrip.x0 = w;
                rightStrip.y0 = shadowOffset;
                rightStrip.width = (w + shadowOffset) - w;
                rightStrip.height = (h + shadowOffset) - shadowOffset;
                BoundsBase<float> bottomStrip;
                bottomStrip.x0 = shadowOffset;
                bottomStrip.y0 = h;
                bottomStrip.width = w - shadowOffset;
                bottomStrip.height = (h + shadowOffset) - h;

                // The shadow falls outside the window, so it is clipped to the
                // parent's extents instead.
                DrawInfo shadowDi(di);
                if (auto* const parent = static_cast<Wnd*>(GetParent()))
                {
                    shadowDi.m_clippedRect = parent->GetBounds().SizeRect();
                }
                GetGfxServer()->AddFlatAxialQuad(shadowDi, rightStrip, 0x80000000);
                GetGfxServer()->AddFlatAxialQuad(shadowDi, bottomStrip, 0x80000000);
            }

            BoundsBase<float> const b = GetBounds().SizeRect();
            if (!m_bgTexture.IsValid())
            {
                GetGfxServer()->AddFlatAxialPane0(di, b, clr, m_paneFlags, m_paneName, m_bgFlags);
            }
            else
            {
                GetGfxServer()->AddImagedRect(di, b, clr, m_bgTexture);
            }
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
            // RVA 0x611930
            if (Object* const parent = GetParent())
            {
                parent->RemoveChild(this);
            }
            RemoveAllChildren();
            if (m_toolTipWnd)
            {
                RemoveTooltip();
            }
            if (m_bgTexture.IsValid())
            {
                M3D_RENDERER->ReleaseTexture(m_bgTexture);
            }
            StopAnimationMoveSound();
            Unregister();
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
            // RVA 0x676D10 - property 0x4000 is the tooltip text.
            if (Object::SetProperty(propId, prop))
            {
                return 1;
            }
            if (propId != 0x4000)
            {
                return 0;
            }
            // NOTE: deliberate deviation. The shipped code takes a char const* here
            // (m_toolTipText = CStr(static_cast<char const*>(prop))); this codebase's
            // callers pass a CStr* instead, so the convention is kept as it is.
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
                        Application::g_pApp->EnqueueMessage(45, this, 0, 0, 0, {}, {});
                        m_toolTipTimeOut = -1;
                    }
                }
                else if (!m_toolTipTimeOut)
                {
                    m_toolTipWnd = CreateTooltipWnd();
                    Application::g_pApp->EnqueueMessage(44, this, 0, 0, 0, {}, {});
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
            // RVA 0x6781D0 - tracks the pressed state for notifying windows (a
            // release after a press is a click, unless the window waits for
            // double clicks, when the click comes from the station instead),
            // reflects to the parent, then handles drag-moving and activation.
            if ((m_style & WS_SEND_NOTIFY_MESSAGES) != 0)
            {
                if (state)
                {
                    m_mouseDown |= 1;
                }
                else
                {
                    if ((m_style & WS_DBLCLICK_REACT) == 0 && (m_mouseDown & 1) != 0)
                    {
                        AIParam const param{CVector2{at.x, at.y}};
                        CallParentNotify(1, param, false);
                    }
                    m_mouseDown &= ~1;
                }
            }

            if ((m_style & WS_REFLECT_MS_AND_KEYS_TO_PARENT) != 0 && GetParent())
            {
                // The shipped code treats the parent as a Wnd without checking.
                static_cast<Wnd*>(GetParent())->OnMouseButton0(state, ToParent(at));
            }

            BoundsBase<float> const bounds = GetBounds();
            if (at.x < 0.0 || bounds.width <= at.x || at.y < 0.0 || bounds.height <= at.y)
            {
                return 0;
            }
            if ((m_style & WS_MOVABLE) != 0)
            {
                if (state && !m_dragMode)
                {
                    StartDragMove(at);
                }
                else if (m_dragMode == DRAG_MOVE)
                {
                    FinishDragMove(true, at);
                }
            }
            if ((m_style & WS_ACTIVATABLE) != 0)
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
            // RVA 0x6769D0 - the activatable child whose activation order follows
            // (or, with back == 1, precedes) that of "first", wrapping around at
            // both ends; with no "first", the one with order 0.
            int maxOrder = 0;
            for (Object* child = GetFirstChild(); child; child = child->GetNextSibling())
            {
                auto* const wnd = static_cast<Wnd*>(child);
                if ((wnd->m_style & WS_ACTIVATABLE) != 0 && wnd->m_activationOrder > maxOrder)
                {
                    maxOrder = wnd->m_activationOrder;
                }
            }

            int order = 0;
            if (first)
            {
                order = first->m_activationOrder + (back == 1 ? -1 : 1);
            }
            if (order < 0)
            {
                order = maxOrder;
            }
            else if (order > maxOrder)
            {
                order = 0;
            }

            for (Object* child = GetFirstChild(); child; child = child->GetNextSibling())
            {
                auto* const wnd = static_cast<Wnd*>(child);
                if ((wnd->m_style & WS_ACTIVATABLE) != 0 && wnd->m_activationOrder == order)
                {
                    return wnd;
                }
            }
            return nullptr;
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
            // RVA 0x676CA0
            if ((m_style & WS_NOFRAME) == 0)
            {
                unsigned const color = (m_style & WS_DISABLE) != 0 || (m_style & WS_GRAYED) != 0 ? 3 : m_curClr;
                OnNcPaint(clipToIt, color);
            }
            DrawWndText(clipToIt);
            if (m_curControl)
            {
                // NOTE: the shipped code fetches the current control's bounds here
                // and never uses them (a leftover); the virtual call is kept.
                m_curControl->GetBounds();
            }
            return 1;
        }

        int ModalWnd::CloseModal(int val)
        {
            M3D_APP->EnqueueMessage(39, this, val, 0, 0, {}, {});
            return 1;
        }

        int ModalWnd::OnKey(unsigned short key, unsigned char, unsigned state)
        {
            // RVA 0x676BA0 - Tab (key 3, with or without the 0x3000 modifier bits)
            // cycles the active child, Enter (key 4) presses the default child with
            // a synthetic space, and Esc (key 1) closes a running modal with 3.
            if ((m_style & WS_ACTIVATION_REFLECT_TO_CHILDREN) != 0 && state == 1 && (key & ~0x3000) == 3)
            {
                Wnd* const active = GetStation()->GetActive();
                if (active && active->IsChildOf(this))
                {
                    Wnd* const next = GetNextActivatableChild(active, (key & 0x3000) != 0);
                    if (next && next != active)
                    {
                        GetStation()->Activate(next);
                    }
                }
                return 1;
            }

            // Only presses reach the default child, never releases.
            if (state && key == 4)
            {
                for (Object* child = GetFirstChild(); child; child = child->GetNextSibling())
                {
                    auto* const wnd = static_cast<Wnd*>(child);
                    if ((wnd->m_style & WS_DEFAULT) != 0)
                    {
                        GetStation()->PulseKeyForWindow(wnd, 0x20, 0x39);
                        return 1;
                    }
                }
            }

            if (GetStation()->IsModal(this) && state && key == 1)
            {
                CloseModal(3);
                return 1;
            }
            return 0;
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

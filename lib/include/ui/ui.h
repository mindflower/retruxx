#pragma once
#include "retruxx/common.h"
#include <math/point2d.h>
#include <core/stringm3d.h>
#include <math/bounds2d.h>
#include <renderer/i_renderer.h>
#include <core/clazz.h>
#include "font.h"
#include <ui/msgbox.h>

namespace m3d
{
    class AIParam;
}

enum
{
    PROP_WND_TOOLTIP = 16384,
};

namespace m3d
{
    enum TextFormatFlags
    {
        TF_CENTER = 0x0,
        TF_LEFT = 0x1,
        TF_RIGHT = 0x2,
        TF_FULL = 0x3,

    };
    enum TextWrapFlags
    {
        TW_NOWRAP = 0x0,
        TW_CHAR_WRAP = 0x1,
        TW_WORD_WRAP = 0x2,
    };

    namespace ui
    {
        class DrawInfo;
        class Cursor;
        class WndStation;
        class GfxServer;
        class ScrollWnd;

        enum PaneFlagBg
        {
            PANE_FLAG_BG_OUT = 0x0,
            PANE_FLAG_BG_OVER = 0x2,
            PANE_FLAG_BG_DOWN = 0x1,
            PANE_FLAG_BG_DISABLE = 0x3,
        };

        enum Edges
        {
            LEFT = 0x0,
            TOP = 0x1,
            RIGHT = 0x2,
            BOTTOM = 0x3,
        };

        enum MbFlags
        {
            MBX_OK = 0x1,
            MBX_YES_NO = 0x2,
            MBX_YES_NO_CANCEL = 0x3,
            MBX_BUTTON_FLAGS = 0x3,
            MBX_CENTRALIZED = 0x8,
            MBX_SOUNDLOOP = 0x10,
        };

        enum
        {
            WS_EMPTY = 0,
            WS_USE_DEFAULT = 0,
            WS_NODRAW = 1,
            WS_DISABLE = 2,
            WS_RESIZABLE = 4,
            WS_MOVABLE = 8,
            WS_ALWAYS_INSIDE = 16,
            WS_REFLECT_MS_AND_KEYS_TO_PARENT = 32,
            WS_NOFRAME = 64,
            WS_EDIT_NUMERIC = 128,
            WS_TRANSPARENT = 256,
            WS_IS_VISIBLE = 512,
            WS_TEXT_CENTERED_X = 1024,
            WS_TEXT_CENTERED_Y = 2048,
            WS_TEXT_CENTERED = 3072,
            WS_ACTIVATION_CAPTURES_FOCUS = 4096,
            WS_ACTIVATABLE = 8192,
            WS_ACTIVATION_REFLECT_TO_CHILDREN = 16384,
            WS_DROPSHADOW = 0xffff8000,
            WS_DEFAULT = 0x10000,
            WS_DBLCLICK_REACT = 0x20000,
            WS_SEND_NOTIFY_MESSAGES = 0x40000,
            WS_GRAYED = 0x80000,
            WS_REFLECT_NOTIFY_MESSAGES_TO_PARENT = 0x100000,
            WS_EDIT_QUICK_ERASE_ON_ACTIVATION = 0x200000,
            WS_NOTIFY_MESSAGES_FORCE_IMMEDIATE = 0x400000,
        };

        class Wnd : public m3d::Object
        {
            friend class WndStation;
            friend class ModalWnd;
            friend class TabWnd;
            friend MbRetCodes RunMsgBoxDlg(CStr const&, CStr const&, unsigned int, bool);

        protected:
            Wnd();
            Wnd(m3d::ui::Wnd const& wnd);

        public:
            virtual ~Wnd() override;
            virtual m3d::Object* Clone() override;
            static m3d::Object* CreateObject();
            static m3d::Class* GetBaseClass();
            virtual m3d::Class* GetClass() const override;
            static m3d::Class m_classWnd;

        private:
            int m_created = 0;
            static inline m3d::ui::WndStation* m_wndStation;

        public:
            bool Valid() const;

            enum DragMode
            {
                DRAG_NONE = 0,
                DRAG_MOVE = 1,
                DRAG_SIZE = 2,
                DRAG_DROP = 3,
            };

        private:
            m3d::ui::Wnd::DragMode m_dragMode = DragMode::DRAG_NONE;
            PointBase<float> m_dragStartPtLocal;
            PointBase<float> m_dragCurPtLocal;
            PointBase<float> m_dragStartPt;
            PointBase<float> m_dragCurPt;
            void StartDragMove(PointBase<float> const& pt);
            void FinishDragMove(int accept, PointBase<float> const& pt);
            void DoDragMove0(PointBase<float> const& pt);
            void DoDragMove(PointBase<float> const& pt);

        public:
            virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
            virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override;
            virtual int SetProperty(unsigned int propId, void* property) override;
            virtual int GetProperty(unsigned int propId, void* property) const override;
            virtual int GetPropertiesList(retruxx::set<unsigned int>& properties) const override;

        protected:
            CStr m_toolTipText;
            int m_toolTipTimeOut = -1;
            m3d::ui::Wnd* m_toolTipWnd = nullptr;
            m3d::ui::ScrollWnd* m_scrollHWnd = nullptr;
            m3d::ui::ScrollWnd* m_scrollVWnd = nullptr;
            void RemoveTooltip();
            m3d::ui::Wnd* CreateTooltipWnd();
            PointBase<float> GetOriginPoint() const;

        public:
            int m_activationOrder = -1;

        protected:
            CStr m_caption;
            unsigned int m_style = 512;
            BoundsBase<float> m_bounds;
            retruxx::vector<float> m_clientEdges;
            unsigned int m_curClr = 0;
            unsigned int m_textColor = -1;
            unsigned int m_textColorDisabled = -8421505;
            CStr m_strTextColor = "@ffffffff";
            CStr m_strTextColorDisabled = "@ff7f7f7f";
            bool m_gotFocus = false;
            bool m_mouseOver = false;
            unsigned int m_id = 0;
            CStr m_bgTextureName;
            m3d::rend::TexHandle m_bgTexture;
            CStr m_paneName = "defaultWnd";
            int m_paneFlags = 7;
            m3d::ui::PaneFlagBg m_bgFlags = PaneFlagBg::PANE_FLAG_BG_OUT;
            unsigned char m_mouseDown = 0;

        public:
            int IsPtInBounds(PointBase<float> const& pt) const;
            PointBase<float> ToScreen(PointBase<float> const& pt) const;
            BoundsBase<float> ToScreen(BoundsBase<float> const& b) const;
            PointBase<float> ToWindow(PointBase<float> const& pt) const;
            BoundsBase<float> ToWindow(BoundsBase<float> const& b) const;
            PointBase<float> ToParent(PointBase<float> const& pt) const;
            BoundsBase<float> ToParent(BoundsBase<float> const& b) const;
            unsigned int GetStyle() const;
            void SetStyle(unsigned int st);
            virtual void SetPane(CStr const& name);
            CStr GetPaneName() const;
            virtual void SetPaneFlags(int flags);
            int GetPaneFlags() const;
            void SetBgFlags(m3d::ui::PaneFlagBg);
            m3d::ui::PaneFlagBg GetBgFlags() const;
            void SetOrigin(PointBase<float> const& pt);
            PointBase<float> GetOrigin() const;
            unsigned int GetId() const;
            void SetId(unsigned int id);

        protected:
            static inline m3d::ui::GfxServer* m_gfx;

        public:
            static m3d::ui::GfxServer* GetGfxServer();

        protected:
            m3d::ui::Wnd* CaptureMouse();
            int CreateWnd(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id);

        public:
            virtual int Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id);

        private:
            int DestroyWnd();

        public:
            m3d::ui::WndStation* GetStation() const;
            virtual int SetText(CStr const& caption);
            virtual CStr GetText() const;
            virtual BoundsBase<float> GetBounds() const;
            virtual BoundsBase<float> GetClientBounds() const;
            virtual void SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin);
            virtual void SetClientEdges(retruxx::vector<float, retruxx::allocator<float>> const& clientEdges);
            virtual void SetClientEdges(float left, float top, float right, float bottom);
            retruxx::vector<float, retruxx::allocator<float>> const& GetClientEdges() const;
            virtual int AddChild(m3d::Object* w) override;
            virtual int RemoveChild(m3d::Object* w) override;
            virtual int RemoveChildForce(m3d::Object* w);
            virtual int OnBeforeAddToWndStation();
            virtual int OnAfterAddToWndStation();
            virtual int OnBeforeRemoveFromWndStation();
            virtual int OnAfterRemoveFromWndStation();
            virtual int SetBackground(m3d::rend::TexHandle bgTex);
            virtual int SetBackground(CStr const& bgTextureName);
            m3d::rend::TexHandle GetBackground() const;

        protected:
            int CallParentNotify(unsigned int msg, m3d::AIParam const& data, bool urgent);
            int ReflectChildNotifyToParent(
                m3d::ui::Wnd* from,
                unsigned int id,
                unsigned int msg,
                m3d::AIParam const& data);
            void DrawNonClient(m3d::ui::DrawInfo const& di, unsigned int clr);
            virtual void OnNcPaint(m3d::ui::DrawInfo const& di, unsigned int clr);
            virtual void DrawWndText(m3d::ui::DrawInfo const& di);
            virtual int OnPaint(m3d::ui::DrawInfo const& di);
            virtual void OnPaintOverChildren(m3d::ui::DrawInfo const& clipToIt);
            virtual int OnTick(int curTime, int deltaTime);
            virtual void OnDisplayChanged();
            virtual int OnMouseIn();
            virtual int OnMouseOut();
            virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas);
            virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at);
            virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at);
            virtual int OnMouseButton2(unsigned int state, PointBase<float> const& at);
            virtual int OnMouseDblClick(PointBase<float> const& firstClickPt, PointBase<float> const& secondClickPt);
            virtual int OnMouseClick(PointBase<float> const& pt);
            virtual int OnMouseWheel(int ticks, PointBase<float> const& at);
            virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state);
            virtual int OnLoosingFocus();
            virtual int OnObtainingFocus();
            virtual int
                OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data);
            virtual int OnActivate(bool on);
            m3d::ui::Wnd* GetNextActivatableChild(m3d::ui::Wnd* first, int back);

        private:
            bool m_showCursor = true;

        public:
            virtual void SetCursorShow(bool state);
            virtual bool GetCursorShow() const;
            virtual int GetCursor(m3d::ui::Cursor& cur);

        protected:
            // retruxx adaptation: the window's user value; the shipped build was unsigned int and the
            // weapon group buttons keep a window pointer in it, so it is a machine word here.
            std::uintptr_t m_int;

        public:
            void SetInt(std::uintptr_t ii);
            std::uintptr_t GetInt() const;
            bool IsVisible() const;
            virtual void ShowWindow(bool show);
            virtual void EnableWindow(bool bEnable);
            bool IsEnabled() const;
            void GrayWindow(bool);
            bool IsGrayed() const;

        protected:
            bool m_modalAttachedToStation;
            m3d::TextWrapFlags m_textWrap = TextWrapFlags::TW_WORD_WRAP;
            m3d::TextFormatFlags m_textFormat = TextFormatFlags::TF_LEFT;

        public:
            void SetWrapMode(m3d::TextWrapFlags wrap);
            m3d::TextWrapFlags GetWrapMode() const;
            virtual void SetFormatMode(m3d::TextFormatFlags format);
            m3d::TextFormatFlags GetFormatMode() const;
            void AdjustForWndTextToFit(unsigned int uiFont, float maxWidth);
            void AdjustToFitChildren();
            void Centralize();

        protected:
            int m_defFont = 0;

        public:
            virtual void
                SetDefaultFont(CStr const& name, float height, m3d::ui::FontType type, m3d::ui::FontParams params);
            virtual void SetDefaultFont(int uiFont);
            int GetDefaultFont() const;
            unsigned int GetTextColor() const;
            virtual void SetTextColor(unsigned int textColor);
            unsigned int GetTextColorDisabled() const;
            virtual void SetTextColorDisabled(unsigned int textColor);
            unsigned int GetColor() const;
            virtual void SetColor(unsigned int color);
            m3d::ui::ScrollWnd* GetScrollVWnd();
            m3d::ui::ScrollWnd* GetScrollHWnd();
            virtual void SetScrollPane(CStr const& scrollPaneName);
            CStr const& GetScrollPaneName() const;
            virtual float GetFrameWidth() const;

        protected:
            int m_gameDataFlags = 0;
            CStr m_scrollPaneName = "Scroll1";
            int m_guiId = -1;

        public:
            virtual int GameDataSetup();
            virtual int GameDataClear(bool beforeContinuousLevel);
            virtual int GameDataUpdate(void* data, int dataType);
            virtual int GameDataSave(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
            virtual int GameDataLoad(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
            virtual void GameDataSetDirty();
            int GetGameDataFlags();
            void SetGameDataFlags(int flags);
            void SetGuiId(int guiId);
            int GetGuiId() const;

        private:
            void Register();
            void Unregister();
            int m_uniqueId = -1;

        public:
            int GetUniqueId() const;

            struct AnimationInfo
            {
                enum AnimationType
                {
                    ANIMATIONTYPE_USER = 0,
                    ANIMATIONTYPE_TO_LEFT = 1,
                    ANIMATIONTYPE_TO_BEYOND_LEFT = 2,
                    ANIMATIONTYPE_TO_RIGHT = 3,
                    ANIMATIONTYPE_TO_BEYOND_RIGHT = 4,
                    ANIMATIONTYPE_TO_TOP = 5,
                    ANIMATIONTYPE_TO_BEYOND_TOP = 6,
                    ANIMATIONTYPE_TO_BOTTOM = 7,
                    ANIMATIONTYPE_TO_BEYOND_BOTTOM = 8,
                    ANIMATIONTYPE_TO_LEFTTOP = 9,
                    ANIMATIONTYPE_TO_BEYOND_LEFTTOP = 10,
                    ANIMATIONTYPE_TO_LEFTBOTTOM = 11,
                    ANIMATIONTYPE_TO_BEYOND_LEFTBOTTOM = 12,
                    ANIMATIONTYPE_TO_RIGHTTOP = 13,
                    ANIMATIONTYPE_TO_BEYOND_RIGHTTOP = 14,
                    ANIMATIONTYPE_TO_RIGHTBOTTOM = 15,
                    ANIMATIONTYPE_TO_BEYOND_RIGHTBOTTOM = 16,
                    ANIMATIONTYPE_INVALID = 17,
                };

                enum Purpose
                {
                    PURPOSE_HIDE = 0,
                    PURPOSE_SHOW = 1,
                    PURPOSE_UNKNOWN = 2,
                };

                AnimationInfo();
                ~AnimationInfo();
                void Setup(PointBase<float> const&, PointBase<float> const&, float, float, unsigned int);
                void Setup(m3d::ui::Wnd::AnimationInfo::AnimationType, float, float, unsigned int);
                void SetupDefaultOnShow();
                void SetupDefaultOnHide();
                bool IsValid() const;
                void Invalidate();
                bool CanAnimate() const;
                int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
                int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
                m3d::ui::Wnd::AnimationInfo::AnimationType Str2AnimationType(CStr const& str) const;
                CStr AnimationType2Str(m3d::ui::Wnd::AnimationInfo::AnimationType animationType) const;
                bool m_bEnabled;
                bool m_bImmediate;
                PointBase<float> m_startPt;
                PointBase<float> m_endPt;
                m3d::ui::Wnd::AnimationInfo::AnimationType m_animationType;
                float m_startSpeed;
                float m_acceleration;
                float m_curSpeed;
                unsigned int m_delayTime;
                unsigned int m_startTime;
                m3d::ui::Wnd::AnimationInfo::Purpose m_purpose;
                CStr m_soundMoveName;
                CStr m_soundStopName;
                bool m_bSoundMoveEnabled;
                bool m_bSoundStopEnabled;
            };

        public:
            void SetOnShowAnimation(m3d::ui::Wnd::AnimationInfo const& animationInfo);
            void SetOnHideAnimation(m3d::ui::Wnd::AnimationInfo const& animationInfo);
            m3d::ui::Wnd::AnimationInfo const& GetOnShowAnimation() const;
            m3d::ui::Wnd::AnimationInfo const& GetOnHideAnimation() const;
            m3d::ui::Wnd::AnimationInfo const& GetCurrentAnimation() const;
            void EnableOnShowAnimation(bool bEnable);
            void EnableOnHideAnimation(bool bEnable);
            void SetOnShowAnimationImmediate(bool bImmediate);
            void SetOnHideAnimationImmediate(bool bImmediate);
            int StartAnimation(m3d::ui::Wnd::AnimationInfo const& animationInfo, bool interpolateWithPrevious);
            void StopAnimation(bool returnToBaseOrigin);
            bool IsAnimatingNow() const;
            void SetBaseOrigin(PointBase<float> const& baseOrigin);
            PointBase<float> const& GetBaseOrigin() const;

        protected:
            m3d::ui::Wnd::AnimationInfo m_onShowAnimation;
            m3d::ui::Wnd::AnimationInfo m_onHideAnimation;
            m3d::ui::Wnd::AnimationInfo m_currentAnimation;
            bool m_bSuspendedUnlink = false;
            bool m_bSuspendedParentUnlink = false;
            PointBase<float> m_baseOrigin;
            int m_animationSoundMoveChannelId = -1;
            int ProcessAnimation(int curTime, int deltaTime);
            void OnEndAnimation(bool bUrgent);
            void StopAnimationMoveSound();
        };

        class DrawInfo
        {
        public:
            Wnd* m_wndDest;
            BoundsBase<float> m_originalRect;
            BoundsBase<float> m_clippedRect;
            BoundsBase<float> m_clientRect;
            BoundsBase<float> m_clientClippedRect;
        };

        int LoadExistingDialog(Wnd*, CStr const&);
        BoundsBase<float> strToBounds(CStr const&);
        PointBase<float> strToPoint(CStr const&);
    }  // namespace ui
}  // namespace m3d

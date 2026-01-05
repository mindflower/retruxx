#pragma once
#include "retruxx/common.h"
#include <math/point2d.h>
#include <core/stringm3d.h>
#include <math/bounds2d.h>
#include <renderer/i_renderer.h>
#include <core/clazz.h>
#include "font.h"

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

        protected:
            Wnd();
            Wnd(m3d::ui::Wnd const& wnd);

        public:
            virtual ~Wnd() override /* 0x00 */;
            virtual m3d::Object* Clone() override /* 0x04 */;
            static m3d::Object* CreateObject();
            static m3d::Class* GetBaseClass();
            virtual m3d::Class* GetClass() const override /* 0x34 */;
            static m3d::Class m_classWnd;

        private:
            /* 0x0034 */ int m_created = 0;
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
            /* 0x0038 */ m3d::ui::Wnd::DragMode m_dragMode = DragMode::DRAG_NONE;
            /* 0x003c */ PointBase<float> m_dragStartPtLocal;
            /* 0x0044 */ PointBase<float> m_dragCurPtLocal;
            /* 0x004c */ PointBase<float> m_dragStartPt;
            /* 0x0054 */ PointBase<float> m_dragCurPt;
            void StartDragMove(PointBase<float> const& pt);
            void FinishDragMove(int accept, PointBase<float> const& pt);
            void DoDragMove0(PointBase<float> const& pt);
            void DoDragMove(PointBase<float> const& pt);

        public:
            virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x08 */;
            virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override /* 0x10 */;
            virtual int SetProperty(unsigned int propId, void* property) override /* 0x14 */;
            virtual int GetProperty(unsigned int propId, void* property) const override /* 0x18 */;
            virtual int GetPropertiesList(retruxx::set<unsigned int>& properties) const override /* 0x1c */;

        protected:
            /* 0x005c */ CStr m_toolTipText;
            /* 0x0068 */ int m_toolTipTimeOut = -1;
            /* 0x006c */ m3d::ui::Wnd* m_toolTipWnd = nullptr;
            /* 0x0070 */ m3d::ui::ScrollWnd* m_scrollHWnd = nullptr;
            /* 0x0074 */ m3d::ui::ScrollWnd* m_scrollVWnd = nullptr;
            void RemoveTooltip();
            m3d::ui::Wnd* CreateTooltipWnd();
            PointBase<float> GetOriginPoint() const;

        public:
            /* 0x0078 */ int m_activationOrder = -1;

        protected:
            /* 0x007c */ CStr m_caption;
            /* 0x0088 */ unsigned int m_style = 512;
            /* 0x008c */ BoundsBase<float> m_bounds;
            /* 0x009c */ retruxx::vector<float> m_clientEdges;
            /* 0x00ac */ unsigned int m_curClr = 0;
            /* 0x00b0 */ unsigned int m_textColor = -1;
            /* 0x00b4 */ unsigned int m_textColorDisabled = -8421505;
            /* 0x00b8 */ CStr m_strTextColor = "@ffffffff";
            /* 0x00c4 */ CStr m_strTextColorDisabled = "@ff7f7f7f";
            /* 0x00d0 */ bool m_gotFocus = false;
            /* 0x00d1 */ bool m_mouseOver = false;
            /* 0x00d2 */ char Padding_22[2];
            /* 0x00d4 */ unsigned int m_id = 0;
            /* 0x00d8 */ CStr m_bgTextureName;
            /* 0x00e4 */ m3d::rend::TexHandle m_bgTexture;
            /* 0x00e8 */ CStr m_paneName = "defaultWnd";
            /* 0x00f4 */ int m_paneFlags = 7;
            /* 0x00f8 */ m3d::ui::PaneFlagBg m_bgFlags = PaneFlagBg::PANE_FLAG_BG_OUT;
            /* 0x00fc */ unsigned char m_mouseDown = 0;

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
            virtual void SetPane(CStr const& name) /* 0x3c */;
            CStr GetPaneName() const;
            virtual void SetPaneFlags(int flags) /* 0x40 */;
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
            static m3d::ui::GfxServer* __fastcall GetGfxServer();

        protected:
            m3d::ui::Wnd* CaptureMouse();
            int CreateWnd(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id);

        public:
            virtual int Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id) /* 0x44 */;

        private:
            int DestroyWnd();

        public:
            m3d::ui::WndStation* GetStation() const;
            virtual int SetText(CStr const& caption) /* 0x48 */;
            virtual CStr GetText() const /* 0x4c */;
            virtual BoundsBase<float> GetBounds() const /* 0x50 */;
            virtual BoundsBase<float> GetClientBounds() const /* 0x54 */;
            virtual void SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin) /* 0x58 */;
            virtual void SetClientEdges(retruxx::vector<float, retruxx::allocator<float>> const& clientEdges) /* 0x60 */;
            virtual void SetClientEdges(float left, float top, float right, float bottom) /* 0x60 */;
            retruxx::vector<float, retruxx::allocator<float>> const& GetClientEdges() const;
            virtual int AddChild(m3d::Object* w) override /* 0x20 */;
            virtual int RemoveChild(m3d::Object* w) override /* 0x24 */;
            virtual int RemoveChildForce(m3d::Object* w) /* 0x64 */;
            virtual int OnBeforeAddToWndStation() /* 0x68 */;
            virtual int OnAfterAddToWndStation() /* 0x6c */;
            virtual int OnBeforeRemoveFromWndStation() /* 0x70 */;
            virtual int OnAfterRemoveFromWndStation() /* 0x74 */;
            virtual int SetBackground(m3d::rend::TexHandle bgTex) /* 0x7c */;
            virtual int SetBackground(CStr const& bgTextureName) /* 0x7c */;
            m3d::rend::TexHandle GetBackground() const;

        protected:
            int CallParentNotify(unsigned int msg, m3d::AIParam const& data, bool urgent);
            int ReflectChildNotifyToParent(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data);
            void DrawNonClient(m3d::ui::DrawInfo const& di, unsigned int clr);
            virtual void OnNcPaint(m3d::ui::DrawInfo const& di, unsigned int clr) /* 0x80 */;
            virtual void DrawWndText(m3d::ui::DrawInfo const& di) /* 0x84 */;
            virtual int OnPaint(m3d::ui::DrawInfo const& di) /* 0x88 */;
            virtual void OnPaintOverChildren(m3d::ui::DrawInfo const& clipToIt) /* 0x8c */;
            virtual int OnTick(int curTime, int deltaTime) /* 0x90 */;
            virtual void OnDisplayChanged() /* 0x94 */;
            virtual int OnMouseIn() /* 0x98 */;
            virtual int OnMouseOut() /* 0x9c */;
            virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas) /* 0xa0 */;
            virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) /* 0xa4 */;
            virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at) /* 0xa8 */;
            virtual int OnMouseButton2(unsigned int state, PointBase<float> const& at) /* 0xac */;
            virtual int OnMouseDblClick(PointBase<float> const& firstClickPt, PointBase<float> const& secondClickPt) /* 0xb0 */;
            virtual int OnMouseClick(PointBase<float> const& pt) /* 0xb4 */;
            virtual int OnMouseWheel(int ticks, PointBase<float> const& at) /* 0xb8 */;
            virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) /* 0xbc */;
            virtual int OnLoosingFocus() /* 0xc0 */;
            virtual int OnObtainingFocus() /* 0xc4 */;
            virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data) /* 0xc8 */;
            virtual int OnActivate(bool on) /* 0xcc */;
            m3d::ui::Wnd* GetNextActivatableChild(m3d::ui::Wnd* first, int back);

        private:
            /* 0x00fd */ bool m_showCursor = true;

        public:
            virtual void SetCursorShow(bool state) /* 0xd0 */;
            virtual bool GetCursorShow() const /* 0xd4 */;
            virtual int GetCursor(m3d::ui::Cursor& cur) /* 0xd8 */;
            /* 0x00fe */ char Padding_23[2];

        protected:
            /* 0x0100 */ unsigned int m_int;

        public:
            void SetInt(unsigned int ii);
            unsigned int GetInt() const;
            bool IsVisible() const;
            virtual void ShowWindow(bool show) /* 0xdc */;
            virtual void EnableWindow(bool bEnable) /* 0xe0 */;
            bool IsEnabled() const;
            void GrayWindow(bool);
            bool IsGrayed() const;

        protected:
            /* 0x0104 */ bool m_modalAttachedToStation;
            /* 0x0105 */ char Padding_24[3];
            /* 0x0108 */ m3d::TextWrapFlags m_textWrap = TextWrapFlags::TW_WORD_WRAP;
            /* 0x010c */ m3d::TextFormatFlags m_textFormat = TextFormatFlags::TF_LEFT;

        public:
            void SetWrapMode(m3d::TextWrapFlags wrap);
            m3d::TextWrapFlags GetWrapMode() const;
            virtual void SetFormatMode(m3d::TextFormatFlags format) /* 0xe4 */;
            m3d::TextFormatFlags GetFormatMode() const;
            void AdjustForWndTextToFit(unsigned int uiFont, float maxWidth);
            void AdjustToFitChildren();
            void Centralize();

        protected:
            /* 0x0110 */ int m_defFont = 0;

        public:
            virtual void SetDefaultFont(CStr const& name, float height, m3d::ui::FontType type, m3d::ui::FontParams params) /* 0xec */;
            virtual void SetDefaultFont(int uiFont) /* 0xec */;
            int GetDefaultFont() const;
            unsigned int GetTextColor() const;
            virtual void SetTextColor(unsigned int textColor) /* 0xf0 */;
            unsigned int GetTextColorDisabled() const;
            virtual void SetTextColorDisabled(unsigned int textColor) /* 0xf4 */;
            unsigned int GetColor() const;
            virtual void SetColor(unsigned int color) /* 0xf8 */;
            m3d::ui::ScrollWnd* GetScrollVWnd();
            m3d::ui::ScrollWnd* GetScrollHWnd();
            virtual void SetScrollPane(CStr const& scrollPaneName) /* 0xfc */;
            CStr const& GetScrollPaneName() const;
            virtual float GetFrameWidth() const /* 0x100 */;

        protected:
            /* 0x0114 */ int m_gameDataFlags = 0;
            /* 0x0118 */ CStr m_scrollPaneName = "Scroll1";
            /* 0x0124 */ int m_guiId = -1;

        public:
            virtual int GameDataSetup() /* 0x104 */;
            virtual int GameDataClear(bool beforeContinuousLevel) /* 0x108 */;
            virtual int GameDataUpdate(void* data, int dataType) /* 0x10c */;
            virtual int GameDataSave(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) /* 0x110 */;
            virtual int GameDataLoad(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) /* 0x114 */;
            virtual void GameDataSetDirty() /* 0x118 */;
            int GetGameDataFlags();
            void SetGameDataFlags(int flags);
            void SetGuiId(int guiId);
            int GetGuiId() const;

        private:
            void Register();
            void Unregister();
            /* 0x0128 */ int m_uniqueId = -1;

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
                /* 0x0000 */ bool m_bEnabled;
                /* 0x0001 */ bool m_bImmediate;
                /* 0x0002 */ char Padding_20[2];
                /* 0x0004 */ PointBase<float> m_startPt;
                /* 0x000c */ PointBase<float> m_endPt;
                /* 0x0014 */ m3d::ui::Wnd::AnimationInfo::AnimationType m_animationType;
                /* 0x0018 */ float m_startSpeed;
                /* 0x001c */ float m_acceleration;
                /* 0x0020 */ float m_curSpeed;
                /* 0x0024 */ unsigned int m_delayTime;
                /* 0x0028 */ unsigned int m_startTime;
                /* 0x002c */ m3d::ui::Wnd::AnimationInfo::Purpose m_purpose;
                /* 0x0030 */ CStr m_soundMoveName;
                /* 0x003c */ CStr m_soundStopName;
                /* 0x0048 */ bool m_bSoundMoveEnabled;
                /* 0x0049 */ bool m_bSoundStopEnabled;
            }; /* size: 0x004c */

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
            /* 0x012c */ m3d::ui::Wnd::AnimationInfo m_onShowAnimation;
            /* 0x0178 */ m3d::ui::Wnd::AnimationInfo m_onHideAnimation;
            /* 0x01c4 */ m3d::ui::Wnd::AnimationInfo m_currentAnimation;
            /* 0x0210 */ bool m_bSuspendedUnlink = false;
            /* 0x0211 */ bool m_bSuspendedParentUnlink = false;
            /* 0x0212 */ char Padding_25[2];
            /* 0x0214 */ PointBase<float> m_baseOrigin;
            /* 0x021c */ int m_animationSoundMoveChannelId = -1;
            int ProcessAnimation(int curTime, int deltaTime);
            void OnEndAnimation(bool bUrgent);
            void StopAnimationMoveSound();
        }; /* size: 0x0220 */

        static_assert(sizeof(Wnd) == 0x0220);

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

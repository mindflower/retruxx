#pragma once
#include <vector>
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

    enum PaneFlagBg
    {
        PANE_FLAG_BG_OUT = 0x0,
        PANE_FLAG_BG_OVER = 0x2,
        PANE_FLAG_BG_DOWN = 0x1,
        PANE_FLAG_BG_DISABLE = 0x3,
    };

    namespace ui
    {
        class DrawInfo;
        class Cursor;
        class WndStation;
        class GfxServer;
        class ScrollWnd;

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

        class Wnd : public Object
        {
            friend class WndStation;
        public:
            enum DragMode
            {
                DRAG_NONE = 0x0,
                DRAG_MOVE = 0x1,
                DRAG_SIZE = 0x2,
                DRAG_DROP = 0x3,
            };

            class AnimationInfo
            {
            public:
                enum AnimationType
                {
                    ANIMATIONTYPE_USER = 0x0,
                    ANIMATIONTYPE_TO_LEFT = 0x1,
                    ANIMATIONTYPE_TO_BEYOND_LEFT = 0x2,
                    ANIMATIONTYPE_TO_RIGHT = 0x3,
                    ANIMATIONTYPE_TO_BEYOND_RIGHT = 0x4,
                    ANIMATIONTYPE_TO_TOP = 0x5,
                    ANIMATIONTYPE_TO_BEYOND_TOP = 0x6,
                    ANIMATIONTYPE_TO_BOTTOM = 0x7,
                    ANIMATIONTYPE_TO_BEYOND_BOTTOM = 0x8,
                    ANIMATIONTYPE_TO_LEFTTOP = 0x9,
                    ANIMATIONTYPE_TO_BEYOND_LEFTTOP = 0xA,
                    ANIMATIONTYPE_TO_LEFTBOTTOM = 0xB,
                    ANIMATIONTYPE_TO_BEYOND_LEFTBOTTOM = 0xC,
                    ANIMATIONTYPE_TO_RIGHTTOP = 0xD,
                    ANIMATIONTYPE_TO_BEYOND_RIGHTTOP = 0xE,
                    ANIMATIONTYPE_TO_RIGHTBOTTOM = 0xF,
                    ANIMATIONTYPE_TO_BEYOND_RIGHTBOTTOM = 0x10,
                    ANIMATIONTYPE_INVALID = 0x11,
                };

                enum Purpose
                {
                    PURPOSE_HIDE = 0x0,
                    PURPOSE_SHOW = 0x1,
                    PURPOSE_UNKNOWN = 0x2,
                };

            public:
                ~AnimationInfo();
                AnimationType Str2AnimationType(CStr const&) const;
                void Invalidate();
                //AnimationInfo(AnimationInfo const&);
                bool IsValid() const;
                int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
                CStr AnimationType2Str(AnimationType) const;
                bool CanAnimate() const;
                int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
                void SetupDefaultOnHide();
                void SetupDefaultOnShow();

            private:
                bool m_bEnabled;
                bool m_bImmediate;
                PointBase<float> m_startPt;
                PointBase<float> m_endPt;
                AnimationType m_animationType;
                float m_startSpeed;
                float m_acceleration;
                float m_curSpeed;
                unsigned int m_delayTime;
                unsigned int m_startTime;
                Purpose m_purpose;
                CStr m_soundMoveName;
                CStr m_soundStopName;
                bool m_bSoundMoveEnabled;
                bool m_bSoundStopEnabled;
            };

        public:
            static Class* GetBaseClass();
            static Object* CreateObject();
            static GfxServer* GetGfxServer();

        public:
            static inline WndStation* m_wndStation = nullptr;
            static inline GfxServer* m_gfx = nullptr;

        public:
            int GetUniqueId() const;
            bool IsVisible() const;
            virtual void ShowWindow(bool);
            unsigned int GetTextColor() const;
            virtual void SetScrollPane(CStr const&);
            virtual void SetCursorShow(bool);
            virtual void GameDataSetDirty();
            int GetGuiId() const;
            int GetDefaultFont() const;
            unsigned int GetTextColorDisabled() const;
            virtual int OnBeforeAddToWndStation();
            virtual int OnAfterAddToWndStation();
            virtual int GameDataSetup();
            virtual void SetDefaultFont(int uiFont);
            virtual void SetDefaultFont(CStr const& name, float height, FontType type, FontParams params);
            void SetGuiId(int);
            BoundsBase<float> ToParent(BoundsBase<float> const&) const;
            PointBase<float> ToParent(PointBase<float> const&) const;
            virtual void SetTextColorDisabled(unsigned int);
            virtual void SetTextColor(unsigned int);
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            virtual bool GetCursorShow() const;
            virtual int OnAfterRemoveFromWndStation();
            virtual ~Wnd();
            rend::TexHandle GetBackground() const;
            void SetGameDataFlags(int);
            PointBase<float> GetOrigin() const;
            int StartAnimation(AnimationInfo const&, bool);
            unsigned int GetStyle() const;
            virtual int GetCursor(Cursor& cur);
            unsigned int GetColor() const;
            ScrollWnd* GetScrollVWnd();
            virtual CStr GetText() const;
            virtual void SetPane(CStr const&);
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            unsigned int GetInt() const;
            void SetStyle(unsigned int);
            virtual int GameDataSave(cmn::XmlFile*, cmn::XmlNode*);
            virtual Object* Clone();
            virtual int RemoveChildForce(Object*);
            virtual int SetBackground(rend::TexHandle);
            virtual int SetBackground(CStr const&);
            int GetGameDataFlags();
            void SetOrigin(PointBase<float> const&);
            virtual void SetColor(unsigned int);
            WndStation* GetStation() const;
            virtual int SetText(CStr const&);
            void SetInt(unsigned int);
            virtual void EnableWindow(bool);
            void EnableOnShowAnimation(bool);
            virtual int AddChild(Object*);
            bool IsEnabled() const;
            void EnableOnHideAnimation(bool);
            unsigned int GetId() const;
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            virtual void SetPaneFlags(int);
            CStr const& GetScrollPaneName() const;
            void SetOnShowAnimationImmediate(bool);
            virtual void SetBounds(BoundsBase<float> const&, bool);
            void Centralize();
            virtual Class* GetClass() const;
            AnimationInfo const& GetCurrentAnimation() const;
            virtual int GameDataLoad(cmn::XmlFile*, cmn::XmlNode*);
            void SetId(unsigned int);
            void StopAnimation(bool);
            PointBase<float> ToWindow(PointBase<float> const&) const;
            BoundsBase<float> ToWindow(BoundsBase<float> const&) const;
            void AdjustForWndTextToFit(unsigned int, float);
            int GetPaneFlags() const;
            void SetOnHideAnimationImmediate(bool);
            PointBase<float> ToScreen(PointBase<float> const&) const;
            BoundsBase<float> ToScreen(BoundsBase<float> const&) const;
            virtual BoundsBase<float> GetBounds() const;
            CStr GetPaneName() const;
            virtual int GameDataUpdate(void*, int);
            virtual BoundsBase<float> GetClientBounds() const;
            virtual int OnBeforeRemoveFromWndStation();
            virtual void SetFormatMode(TextFormatFlags);
            AnimationInfo const& GetOnHideAnimation() const;
            virtual void SetClientEdges(std::vector<float> const&);
            virtual void SetClientEdges(float, float, float, float);
            bool IsPtInBounds(PointBase<float> const& pt) const;
            void AdjustToFitChildren();
            PointBase<float> const& GetBaseOrigin() const;
            void SetOnShowAnimation(AnimationInfo const&);
            virtual int GameDataClear(bool);
            TextWrapFlags GetWrapMode() const;
            virtual int SetProperty(unsigned int propId, void* prop);
            bool Valid() const;
            virtual int RemoveChild(Object*);
            TextFormatFlags GetFormatMode() const;
            virtual float GetFrameWidth() const;
            bool IsAnimatingNow() const;
            virtual int GetPropertiesList(std::set<unsigned int>&) const;
            void SetWrapMode(TextWrapFlags);
            AnimationInfo const& GetOnShowAnimation() const;
            virtual int GetProperty(unsigned int, void*) const;
            std::vector<float> const& GetClientEdges() const;
            void SetOnHideAnimation(AnimationInfo const&);
            void SetBaseOrigin(PointBase<float> const&);

        protected:
            void RemoveTooltip();
            virtual void DrawWndText(DrawInfo const&);
            virtual int OnTick(int, int);
            int ProcessAnimation(int, int);
            int ReflectChildNotifyToParent(Wnd*, unsigned int, unsigned int, AIParam const&);
            Wnd* CreateTooltipWnd();
            void OnEndAnimation(bool);
            virtual int OnObtainingFocus();
            virtual int OnLoosingFocus();
            virtual int OnMouseIn();
            virtual int OnActivate(bool on);
            virtual int OnMouseClick(PointBase<float> const&);
            virtual void OnPaintOverChildren(DrawInfo const& clipToIt);
            virtual int OnMouseButton2(unsigned int, PointBase<float> const&);
            int CreateWnd(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            virtual int OnMouseButton1(unsigned int, PointBase<float> const&);
            virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at);
            Wnd();
            Wnd(Wnd const&);
            virtual int OnWndNotify(Wnd* from, unsigned int idFrom, unsigned int message, AIParam const& data);
            PointBase<float> GetOriginPoint() const;
            void DrawNonClient(DrawInfo const&, unsigned int);
            virtual int OnMouseWheel(int, PointBase<float> const&);
            virtual void OnDisplayChanged();
            void StopAnimationMoveSound();
            Wnd* GetNextActivatableChild(Wnd*, int);
            virtual int OnMouseDblClick(PointBase<float> const& firstClickPt, PointBase<float> const& secondClickPt);
            virtual int OnMouseOut();
            virtual void OnNcPaint(DrawInfo const&, unsigned int);
            virtual int OnPaint(DrawInfo const& clipToIt);
            virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state);
            virtual int OnMouseMove(PointBase<float> const& pt, PointBase<float> const& deltas);
            int CallParentNotify(unsigned int msg, AIParam const& data, bool urgent);

        private:
            void Unregister();
            void Register();
            void DoDragMove0(PointBase<float> const& pt);
            void DoDragMove(PointBase<float> const&);
            int DestroyWnd();
            void FinishDragMove(int, PointBase<float> const&);
            void StartDragMove(PointBase<float> const&);

        public:
            RT_CLASS_INLINE_DECLARE(Wnd);

        private:
            int m_created = 0;
            DragMode m_dragMode = DragMode::DRAG_NONE;
            PointBase<float> m_dragStartPtLocal;
            PointBase<float> m_dragCurPtLocal;
            PointBase<float> m_dragStartPt;
            PointBase<float> m_dragCurPt;
            CStr m_toolTipText;
            Wnd* m_toolTipWnd = nullptr;
            int m_toolTipTimeOut = -1;
            ScrollWnd* m_scrollHWnd = nullptr;
            ScrollWnd* m_scrollVWnd = nullptr;
            int m_activationOrder = -1;
            CStr m_caption;
            unsigned int m_style = 512;                     //TODO: magic number
            BoundsBase<float> m_bounds;
            std::vector<float> m_clientEdges;
            unsigned int m_curClr = 0;
            unsigned int m_textColor = -1;
            unsigned int m_textColorDisabled = -8421505;    //TODO: magic number
            CStr m_strTextColor = "@ffffffff";
            CStr m_strTextColorDisabled = "@ff7f7f7f";
            bool m_gotFocus = 0;
            bool m_mouseOver = 0;
            unsigned int m_id = 0;
            CStr m_bgTextureName;
            rend::TexHandle m_bgTexture;
            CStr m_paneName = "defaultWnd";
            int m_paneFlags = 7;                            //TODO: magic number
            PaneFlagBg m_bgFlags = PaneFlagBg::PANE_FLAG_BG_OUT;
            unsigned __int8 m_mouseDown = 0;
            bool m_showCursor = true;
            unsigned int m_int;
            bool m_modalAttachedToStation;
            TextWrapFlags m_textWrap = TextWrapFlags::TW_WORD_WRAP;
            TextFormatFlags m_textFormat = TextFormatFlags::TF_LEFT;
            int m_defFont = 0;
            int m_gameDataFlags = 0;
            CStr m_scrollPaneName = "Scroll1";
            int m_guiId = -1;
            int m_uniqueId = -1;
            AnimationInfo m_onShowAnimation;
            AnimationInfo m_onHideAnimation;
            AnimationInfo m_currentAnimation;
            bool m_bSuspendedUnlink = false;
            bool m_bSuspendedParentUnlink = false;
            PointBase<float> m_baseOrigin;
            int m_animationSoundMoveChannelId = -1;
        };

        class DrawInfo
        {
        private:
            Wnd* m_wndDest;
            BoundsBase<float> m_originalRect;
            BoundsBase<float> m_clippedRect;
            BoundsBase<float> m_clientRect;
            BoundsBase<float> m_clientClippedRect;
        };

        int LoadExistingDialog(Wnd*, CStr const&);
        BoundsBase<float> strToBounds(CStr const&);
        PointBase<float> strToPoint(CStr const&);
    }
}

#pragma once

namespace ui
{
    namespace m3d
    {
        class ModalWnd :  public Wnd
        {
        public:
            static Class * __fastcall GetBaseClass();
            int DoModal();
            virtual int Create(class CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int);
            virtual int CanClose();
            int IsModal();
            virtual Class * GetClass() const ;
            static Object * __fastcall CreateObject();
            Wnd * GetDlgItem(unsigned int);
            virtual Object * Clone();
            virtual ~ModalWnd();
        protected:
            virtual int OnInitDlgItem(Wnd *,unsigned int);
            virtual void OnCloseModal(int);
            virtual int OnInitModal();
            virtual int OnPaint(DrawInfo const &);
            virtual int CloseModal(int);
            virtual int OnKey(unsigned short,unsigned char,unsigned int);
            virtual int OnWndNotify(Wnd *,unsigned int,unsigned int,AIParam const &);
            ModalWnd(class ModalWnd const &);
            ModalWnd();
        private:
            Wnd *m_curControl;
        };
    }
}

namespace ui
{
    namespace m3d
    {
        class WndStation :  public Wnd
        {
        public:
            int DispatchMouse(Event const &);
            bool IsWndAlive(Wnd const *,int) const ;
            static Class * __fastcall GetBaseClass();
            Wnd * CaptureFocus(Wnd *);
            int Done();
            ModalWnd * GetTopModal();
            int DispatchPaint(Wnd *,struct BoundsBase<float> const &);
            int CheckForMouseClick(Wnd *,bool,class PointBase<float> const *);
            Wnd * GetWndForMousePoint(Wnd *,class PointBase<float> const &,bool);
            int DoModal(ModalWnd *);
            int PulseKeyForWindow(Wnd *,unsigned short,unsigned char);
            Cursor const & GetCurrentCursor() const ;
            Wnd * GetCapture() const ;
            Wnd * GetWndMouseOver();
            void EnableAnimation(bool);
            int Activate(Wnd *);
            int OnAddWnd(Wnd *,Wnd *);
            virtual Wnd * CaptureMouse(Wnd *);
            class CStr GetStringByStringId0(class CStr const &);
            int GetStringByStringId(class CStr &,class CStr const &);
            bool HasChildModalRunning();
            int AddNotifyForWnd(Wnd *,Wnd *,unsigned int,AIParam const &,bool);
            int CheckForMouseDblClick(Wnd *,class PointBase<float> const &,unsigned int,class PointBase<float> &);
            virtual int GetDefaultCursor(Cursor &);
            int LoadStrings(class CStr const &);
            void CloseAllModalWithCancelRet();
            virtual int Create(class CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int);
            int Create(class CStr const &,class CStr const &);
            virtual Class * GetRtClass() const ;
            void StopAllAnimations();
            Wnd * GetFocus() const ;
            int ProcessEvent(Event const &);
            Wnd * GetWndByUniqueId(int) const ;
            void EndModal(ModalWnd *,unsigned int);
            Wnd * GetActive() const ;
            int Repaint();
            bool IsModal(ModalWnd *);
            void OnEndAnimation(Wnd *);
            int FlushGfx(rend::IRenderer *);
            bool IsAnimationEnabled() const ;
            class CStr InitializeStringUsingIds(class CStr const &);
            virtual ~WndStation();
            int OnRemoveWnd(Wnd *,Wnd *);
        protected:
            WndStation();
        private:
            int DispatchJoystick(Event const &);
            void CreateDefaultStrings();
            void OnCloseComboBox(ComboBoxWnd *);
            void OnOpenComboBox(ComboBoxWnd *);
            void ForEachChild(Wnd *,void (Wnd::*)());
            Wnd * ModalOverride(Wnd *);
            void RegisterWnd(Wnd *);
            static GfxServer * __fastcall getGfxServer();
            void RemoveCurrentTooltip();
            void UpdateOnMouseInOut(Wnd *);
            void UnregisterWnd(Wnd *);
            int DispatchKey(Event const &);
            Wnd *m_wndMouseOver;
            Wnd *m_wndMouseCapture;
            Wnd *m_wndKbdCapture;
            Wnd *m_wndActive;
            Wnd *m_wndForTooltip;
            Wnd *m_wndCandidateForDblClick;
            ComboBoxWnd *m_wndOpenedComboBox;
            std::vector<ModalWnd *> m_wndModalStack;
            unsigned int m_wndModalRetVal;
            Cursor *m_curDefault;
            Cursor m_currentCursor;
            bool m_bAnimationEnabled;
            PointBase<float> m_prevMouseCoord;
            CStrHash<CStr> m_strings;
            CIntHash<int> m_allWindows;
            CIntHash<Wnd *> m_allWindowsById;
            int m_nextUniqueId;
        };
    }
}

namespace ui
{
    namespace m3d
    {
        class Wnd :  public Object
        {
        public:
            int GetUniqueId() const ;
            bool IsVisible() const ;
            virtual void ShowWindow(bool);
            static Class * __fastcall GetBaseClass();
            unsigned int GetTextColor() const ;
            virtual void SetScrollPane(class CStr const &);
            virtual void SetCursorShow(bool);
            virtual void GameDataSetDirty();
            int GetGuiId() const ;
            int GetDefaultFont() const ;
            unsigned int GetTextColorDisabled() const ;
            virtual int OnBeforeAddToWndStation();
            static Object * __fastcall CreateObject();
            virtual int OnAfterAddToWndStation();
            virtual int GameDataSetup();
            virtual void SetDefaultFont(int);
            virtual void SetDefaultFont(class CStr const &,float,FontType,union FontParams);
            void SetGuiId(int);
            struct BoundsBase<float> ToParent(struct BoundsBase<float> const &) const ;
            class PointBase<float> ToParent(class PointBase<float> const &) const ;
            virtual void SetTextColorDisabled(unsigned int);
            virtual void SetTextColor(unsigned int);
            virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            virtual bool GetCursorShow() const ;
            virtual int OnAfterRemoveFromWndStation();
            virtual ~Wnd();
            rend::TexHandle GetBackground() const ;
            void SetGameDataFlags(int);
            class PointBase<float> GetOrigin() const ;
            int StartAnimation(struct AnimationInfo const &,bool);
            unsigned int GetStyle() const ;
            virtual int GetCursor(Cursor &);
            unsigned int GetColor() const ;
            ScrollWnd * GetScrollVWnd();
            virtual class CStr GetText() const ;
            virtual void SetPane(class CStr const &);
            virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            unsigned int GetInt() const ;
            void SetStyle(unsigned int);
            virtual int GameDataSave(cmn::XmlFile *,cmn::XmlNode *);
            virtual Object * Clone();
            virtual int RemoveChildForce(Object *);
            virtual int SetBackground(rend::TexHandle);
            virtual int SetBackground(class CStr const &);
            int GetGameDataFlags();
            void SetOrigin(class PointBase<float> const &);
            virtual void SetColor(unsigned int);
            class WndStation * GetStation() const ;
            virtual int SetText(class CStr const &);
            void SetInt(unsigned int);
            virtual void EnableWindow(bool);
            void EnableOnShowAnimation(bool);
            virtual int AddChild(Object *);
            bool IsEnabled() const ;
            void EnableOnHideAnimation(bool);
            unsigned int GetId() const ;
            virtual int Create(class CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int);
            virtual void SetPaneFlags(int);
            class CStr const & GetScrollPaneName() const ;
            void SetOnShowAnimationImmediate(bool);
            virtual void SetBounds(struct BoundsBase<float> const &,bool);
            void Centralize();
            virtual Class * GetClass() const ;
            struct AnimationInfo const & GetCurrentAnimation() const ;
            virtual int GameDataLoad(cmn::XmlFile *,cmn::XmlNode *);
            void SetId(unsigned int);
            void StopAnimation(bool);
            class PointBase<float> ToWindow(class PointBase<float> const &) const ;
            struct BoundsBase<float> ToWindow(struct BoundsBase<float> const &) const ;
            void AdjustForWndTextToFit(unsigned int,float);
            int GetPaneFlags() const ;
            void SetOnHideAnimationImmediate(bool);
            class PointBase<float> ToScreen(class PointBase<float> const &) const ;
            struct BoundsBase<float> ToScreen(struct BoundsBase<float> const &) const ;
            virtual struct BoundsBase<float> GetBounds() const ;
            class CStr GetPaneName() const ;
            virtual int GameDataUpdate(void *,int);
            virtual struct BoundsBase<float> GetClientBounds() const ;
            virtual int OnBeforeRemoveFromWndStation();
            virtual void SetFormatMode(TextFormatFlags);
            struct AnimationInfo const & GetOnHideAnimation() const ;
            virtual void SetClientEdges(class std::vector<float,class std::allocator<float> > const &);
            virtual void SetClientEdges(float,float,float,float);
            int IsPtInBounds(class PointBase<float> const &) const ;
            void AdjustToFitChildren();
            class PointBase<float> const & GetBaseOrigin() const ;
            void SetOnShowAnimation(struct AnimationInfo const &);
            virtual int GameDataClear(bool);
            TextWrapFlags GetWrapMode() const ;
            virtual int SetProperty(unsigned int,void *);
            static GfxServer * __fastcall GetGfxServer();
            bool Valid() const ;
            virtual int RemoveChild(Object *);
            TextFormatFlags GetFormatMode() const ;
            virtual float GetFrameWidth() const ;
            bool IsAnimatingNow() const ;
            virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
            void SetWrapMode(TextWrapFlags);
            struct AnimationInfo const & GetOnShowAnimation() const ;
            virtual int GetProperty(unsigned int,void *) const ;
            class std::vector<float,class std::allocator<float> > const & GetClientEdges() const ;
            void SetOnHideAnimation(struct AnimationInfo const &);
            void SetBaseOrigin(class PointBase<float> const &);
        protected:
            void RemoveTooltip();
            virtual void DrawWndText(DrawInfo const &);
            virtual int OnTick(int,int);
            int ProcessAnimation(int,int);
            int ReflectChildNotifyToParent(class Wnd *,unsigned int,unsigned int,AIParam const &);
            class Wnd * CreateTooltipWnd();
            void OnEndAnimation(bool);
            virtual int OnObtainingFocus();
            virtual int OnLoosingFocus();
            virtual int OnMouseIn();
            virtual int OnActivate(bool);
            virtual int OnMouseClick(class PointBase<float> const &);
            virtual void OnPaintOverChildren(DrawInfo const &);
            virtual int OnMouseButton2(unsigned int,class PointBase<float> const &);
            int CreateWnd(class CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int);
            virtual int OnMouseButton1(unsigned int,class PointBase<float> const &);
            virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
            Wnd();
            Wnd(class Wnd const &);
            virtual int OnWndNotify(class Wnd *,unsigned int,unsigned int,AIParam const &);
            class PointBase<float> GetOriginPoint() const ;
            void DrawNonClient(DrawInfo const &,unsigned int);
            virtual int OnMouseWheel(int,class PointBase<float> const &);
            virtual void OnDisplayChanged();
            void StopAnimationMoveSound();
            class Wnd * GetNextActivatableChild(class Wnd *,int);
            virtual int OnMouseDblClick(class PointBase<float> const &,class PointBase<float> const &);
            virtual int OnMouseOut();
            virtual void OnNcPaint(DrawInfo const &,unsigned int);
            virtual int OnPaint(DrawInfo const &);
            virtual int OnKey(unsigned short,unsigned char,unsigned int);
            virtual int OnMouseMove(class PointBase<float> const &,class PointBase<float> const &);
            int CallParentNotify(unsigned int,AIParam const &,bool);
        private:
            void Unregister();
            void WndStation::ForEachChild(class Wnd *,void (*)());
            void Register();
            void DoDragMove0(class PointBase<float> const &);
            void DoDragMove(class PointBase<float> const &);
            int DestroyWnd();
            void FinishDragMove(int,class PointBase<float> const &);
            void StartDragMove(class PointBase<float> const &);
            int m_created;
            Wnd::DragMode m_dragMode;
            PointBase<float> m_dragStartPtLocal;
            PointBase<float> m_dragCurPtLocal;
            PointBase<float> m_dragStartPt;
            PointBase<float> m_dragCurPt;
            CStr m_toolTipText;
            int m_toolTipTimeOut;
            Wnd *m_toolTipWnd;
            ScrollWnd *m_scrollHWnd;
            ScrollWnd *m_scrollVWnd;
            int m_activationOrder;
            CStr m_caption;
            unsigned int m_style;
            BoundsBase<float> m_bounds;
            std::vector<float> m_clientEdges;
            unsigned int m_curClr;
            unsigned int m_textColor;
            unsigned int m_textColorDisabled;
            CStr m_strTextColor;
            CStr m_strTextColorDisabled;
            bool m_gotFocus;
            bool m_mouseOver;
            unsigned int m_id;
            CStr m_bgTextureName;
            rend::TexHandle m_bgTexture;
            CStr m_paneName;
            int m_paneFlags;
            PaneFlagBg m_bgFlags;
            unsigned __int8 m_mouseDown;
            bool m_showCursor;
            unsigned int m_int;
            bool m_modalAttachedToStation;
            TextWrapFlags m_textWrap;
            TextFormatFlags m_textFormat;
            int m_defFont;
            int m_gameDataFlags;
            CStr m_scrollPaneName;
            int m_guiId;
            int m_uniqueId;
            Wnd::AnimationInfo m_onShowAnimation;
            Wnd::AnimationInfo m_onHideAnimation;
            Wnd::AnimationInfo m_currentAnimation;
            bool m_bSuspendedUnlink;
            bool m_bSuspendedParentUnlink;
            PointBase<float> m_baseOrigin;
            int m_animationSoundMoveChannelId;
        };
    }
}

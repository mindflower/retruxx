#pragma once

namespace m3d
{
    namespace ui
    {
        class SliderWnd : public Wnd
        {
        public:
            int GetMax() const;
            virtual Object* Clone();
            virtual int Create(CStr const&, unsigned int, struct BoundsBase<float> const&, unsigned int);
            virtual int Create(float, unsigned int);
            virtual ~SliderWnd();
            int GetMin() const;
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual Class* GetClass() const;
            virtual void SetMinMax(int, int);
            int GetNotch() const;
            virtual void SetNotch(int);
        protected:
            virtual int OnPaint(DrawInfo const&);
            virtual int OnMouseMove(class PointBase<float> const&, class PointBase<float> const&);
            struct BoundsBase<float> GetBodyRect() const;
            virtual int OnMouseButton0(unsigned int, class PointBase<float> const&);
            SliderWnd();
            SliderWnd(class SliderWnd const&);
        private:
            int m_min;
            int m_max;
            int m_cur;
            bool m_tracking;
            float m_notchWidth;
        };

        class LineWnd : public Wnd
        {
        public:
            virtual Class* GetClass() const;
            virtual int Create(CStr const&, unsigned int, struct BoundsBase<float> const&, unsigned int);
            int Create(struct BoundsBase<float> const&, unsigned int, enum LineWndDirection);
            virtual ~LineWnd();
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            static Object* __fastcall CreateObject();
            void SetDirection(enum LineWndDirection);
            static Class* __fastcall GetBaseClass();
            virtual Object* Clone();
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        protected:
            virtual void SetPaneFlags(int);
            virtual void OnNcPaint(DrawInfo const&, unsigned int);
            virtual int OnPaint(DrawInfo const&);
            LineWnd(class LineWnd const&);
            LineWnd();
        private:
            LineWndDirection m_direction;
        };

        class ModalWnd : public Wnd
        {
        public:
            static Class* __fastcall GetBaseClass();
            int DoModal();
            virtual int Create(CStr const&, unsigned int, struct BoundsBase<float> const&, unsigned int);
            virtual int CanClose();
            int IsModal();
            virtual Class* GetClass() const;
            static Object* __fastcall CreateObject();
            Wnd* GetDlgItem(unsigned int);
            virtual Object* Clone();
            virtual ~ModalWnd();
        protected:
            virtual int OnInitDlgItem(Wnd*, unsigned int);
            virtual void OnCloseModal(int);
            virtual int OnInitModal();
            virtual int OnPaint(DrawInfo const&);
            virtual int CloseModal(int);
            virtual int OnKey(unsigned short, unsigned char, unsigned int);
            virtual int OnWndNotify(Wnd*, unsigned int, unsigned int, AIParam const&);
            ModalWnd(class ModalWnd const&);
            ModalWnd();
        private:
            Wnd* m_curControl;
        };

        class EditWnd : public Wnd
        {
        public:
            int IncrementCursorPosition();
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            void DeleteSymbol(int);
            void SetQuickEraseState(bool);
            virtual Object* Clone();
            int GetMaxCursorPos() const;
            virtual int SetText(CStr const&);
            static Class* __fastcall GetBaseClass();
            int GetMinCursorPos() const;
            virtual ~EditWnd();
            static Object* __fastcall CreateObject();
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            int CursorPos2StrPos(int) const;
            int SetCursorPosition(int);
            int DecrementCursorPosition();
            bool IsInQuickEraseState() const;
            int GetCursorPosition() const;
            virtual int Create(CStr const&, unsigned int, struct BoundsBase<float> const&, unsigned int);
            CStr GetVisibleText() const;
            void SetCursorColor(unsigned int);
            virtual Class* GetClass() const;
            unsigned int GetCursorColor() const;
        protected:
            virtual int OnActivate(bool);
            virtual int OnMouseButton0(unsigned int, class PointBase<float> const&);
            EditWnd();
            EditWnd(class EditWnd const&);
            virtual int OnPaint(DrawInfo const&);
            virtual int OnKey(unsigned short, unsigned char, unsigned int);
        private:
            bool m_bQuickEraseState;
            int m_cursorPosition;
            unsigned int m_cursorColor;
            CStr m_strCursorColor;
        };

        class ImageWnd : public Wnd
        {
        public:
            int SetImage(CStr const&);
            int SetImage(rend::TexHandle);
            virtual Class* GetClass() const;
            static Object* __fastcall CreateObject();
            virtual ~ImageWnd();
            rend::TexHandle GetImage() const;
            virtual Object* Clone();
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            static Class* __fastcall GetBaseClass();
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            virtual int CreateImageWnd(struct BoundsBase<float> const&, CStr const&);
            virtual int CreateImageWnd(struct BoundsBase<float> const&, rend::TexHandle);
            virtual int Create(CStr const&, unsigned int, struct BoundsBase<float> const&, unsigned int);
            virtual int OnPaint(DrawInfo const&);
        protected:
            ImageWnd();
            ImageWnd(class ImageWnd const&);
        private:
            CStr m_textureName;
            rend::TexHandle m_texture;
            bool m_bDrawPane;
        };

        class ScrollWnd : public Wnd
        {
        public:
            virtual Object* Clone();
            float GetCurPos() const;
            void SetCurPos(float);
            virtual void EnableWindow(bool);
            int Create(struct BoundsBase<float> const&, int);
            virtual int Create(CStr const&, unsigned int, struct BoundsBase<float> const&, unsigned int);
            virtual ~ScrollWnd();
            float GetMaxPos() const;
            static Object* __fastcall CreateObject();
            virtual void SetScrollPane(CStr const&);
            static Class* __fastcall GetBaseClass();
            void SetScrollRect(float, float);
            virtual Class* GetClass() const;
            virtual void SetBounds(struct BoundsBase<float> const&, bool);
            virtual void ShowWindow(bool);
        protected:
            virtual int OnPaint(DrawInfo const&);
            void RecalcLayot();
            virtual int OnMouseMove(class PointBase<float> const&, class PointBase<float> const&);
            ScrollWnd();
            ScrollWnd(class ScrollWnd const&);
            virtual int OnWndNotify(Wnd*, unsigned int, unsigned int, AIParam const&);
            struct BoundsBase<float> GetThumbRect() const;
            struct BoundsBase<float> GetBodyRect() const;
            virtual int OnMouseButton0(unsigned int, class PointBase<float> const&);
        private:
            int m_vertical;
            bool m_tracking;
            float m_maxPos;
            float m_curPos;
            float m_thumbSz;
            float m_hitPosInThumb;
            ButtonWnd* m_btn0;
            ButtonWnd* m_btn1;
        };

        class SgNodeArrayWnd :  public ImageWnd
        {
        public:
            virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            struct CVector & Translation();
            void AddSgNode(SgNode *,struct CMatrix const &,bool);
            static Object * __fastcall CreateObject();
            struct Quaternion & Rotation();
            virtual Class * GetClass() const ;
            static Class * __fastcall GetBaseClass();
            virtual ~SgNodeArrayWnd();
            void SetClassesToRender(class std::vector<Class *,class std::allocator<Class *> > const &);
            void ClearSgNodes();
            virtual int OnPaint(DrawInfo const &);
            virtual Object * Clone();
            rend::TexHandle GetTargetTexture() const ;
            virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            virtual int CreateSgNodeArrayWnd(CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int,rend::TexHandle);
            virtual int CreateSgNodeArrayWnd(rend::TexHandle,unsigned int,struct BoundsBase<float> const &,unsigned int,rend::TexHandle);
            int SetTargetTexture(rend::TexHandle);
        protected:
            virtual int CreateImageWnd(struct BoundsBase<float> const &,rend::TexHandle);
            virtual int CreateImageWnd(struct BoundsBase<float> const &,CStr const &);
            SgNodeArrayWnd();
            SgNodeArrayWnd(class SgNodeArrayWnd const &);
        private:
            rend::TexHandle m_renderTexture;
            std::vector<SgNode *> m_nodes;
            std::vector<CMatrix> m_xForms;
            std::vector<bool> m_withChilds;
            std::vector<Class *> m_classesToRender;
            CVector m_Scale;
            CVector m_Translation;
            Quaternion m_Rotation;
        };

        class MenuWnd : public ModalWnd
        {
        public:
            class Item
            {
            public:
                Item(CStr const&, int, class Item*, class Item*);
            protected:
            private:
                Item* m_next;
                Item* m_child;
                Item* m_parent;
                CStr m_strId;
                CStr m_caption;
                int m_cmd;
                BoundsBase<float> m_drawRect;
            };

        public:
            virtual Class* GetClass() const;
            void SetMenu(class Item*);
            virtual int OnMouseMove(class PointBase<float> const&, class PointBase<float> const&);
            virtual int OnInitModal();
            virtual ~MenuWnd();
            virtual int OnPaint(DrawInfo const&);
            virtual int OnMouseOut();
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual int OnMouseButton0(unsigned int, class PointBase<float> const&);
            virtual Object* Clone();
            virtual int CanClose();
        protected:
            void UpdateItems(class Item*);
            void RecalcLayout();
            MenuWnd(class MenuWnd const&);
            MenuWnd();
            int StorePathToItem(class std::list<class Item*, class std::allocator<class Item*> >&, class Item*, class Item*);
        private:
            std::allocator<Item*>::allocator<Item*>();
            std::allocator<Item*>::allocator<Item*>();
            bool m_isHoldDown;
            std::list<MenuWnd::Item*> m_menuItemsStack;
            MenuWnd::Item* m_itemUnderMouse;
            MenuWnd::Item* m_menu;
        };

        class StringsListBoxWnd : public ListBoxWnd<CStr>
        {
        public:
            virtual Object* Clone();
            virtual ~StringsListBoxWnd();
            static Class* __fastcall GetBaseClass();
            static Object* __fastcall CreateObject();
            virtual Class* GetClass() const;
        protected:
            virtual int RenderItem(int, class PointBase<float> const&, DrawInfo const&);
            virtual int MeasureItem(int, struct BoundsBase<float>&) const;
            virtual int DeleteItem(int);
            StringsListBoxWnd();
            StringsListBoxWnd(class StringsListBoxWnd const&);
            virtual int CompareItem(int, int);
        private:
        };

        class FormattedStringsListBoxWnd : public ListBoxWnd<FormattedLine>
        {
        public:
            virtual ~FormattedStringsListBoxWnd();
            static Object* __fastcall CreateObject();
            virtual Class* GetClass() const;
            virtual void SetTextColor(unsigned int);
            static Class* __fastcall GetBaseClass();
            virtual Object* Clone();
            virtual void SetFormatMode(TextFormatFlags);
        protected:
            virtual int CompareItem(int, int);
            virtual int MeasureItem(int, struct BoundsBase<float>&) const;
            virtual int DeleteItem(int);
            virtual int RenderItem(int, class PointBase<float> const&, DrawInfo const&);
            FormattedStringsListBoxWnd();
            FormattedStringsListBoxWnd(class FormattedStringsListBoxWnd const&);
        private:
        };
    }
}


namespace ui
{
    namespace m3d
    {
        class CheckWnd :  public ButtonWnd
        {
        public:
            virtual Class * GetClass() const ;
            static Object * __fastcall CreateObject();
            void SetCheck(int);
            virtual ~CheckWnd();
            CStr const & GetCheckedPaneName() const ;
            virtual Object * Clone();
            int GetCheck() const ;
            void SetUncheckedPane(CStr const &);
            virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            CStr const & GetUncheckedPaneName() const ;
            void SetCheckedPane(CStr const &);
            static Class * __fastcall GetBaseClass();
            virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            virtual int Create(CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int);
        protected:
            virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
            virtual void DrawWndText(DrawInfo const &);
            struct BoundsBase<float> GetIcoBounds() const ;
            virtual void OnNcPaint(DrawInfo const &,unsigned int);
            CheckWnd();
            CheckWnd(class CheckWnd const &);
        private:
            int m_isChecked;
            CStr m_checkedPaneName;
            CStr m_uncheckedPaneName;
        };
    }
}


namespace ui
{
    namespace m3d
    {
        class GlyphButton :  public ButtonWnd
        {
        public:
            void SetGlyphHeight(float);
            virtual Class * GetClass() const ;
            float GetGlyphHeight();
            void AdjustBoundsToFit(float);
            int SetGlyphTexture(CStr const &,CStr const &);
            void SetGlyphTexture(rend::TexHandle,rend::TexHandle);
            virtual ~GlyphButton();
            rend::TexHandle GetGlyphTexture() const ;
            rend::TexHandle GetGlyphTextureDisabled() const ;
            virtual int CreateGlyphButton(struct BoundsBase<float> const &,enum GlyphButtonType);
            virtual int CreateGlyphButton(unsigned int,struct BoundsBase<float> const &,enum GlyphButtonType,CStr const &,unsigned int,CStr,CStr);
            virtual int CreateGlyphButton(unsigned int,struct BoundsBase<float> const &,enum GlyphButtonType,CStr const &,unsigned int,rend::TexHandle,rend::TexHandle);
            static Object * __fastcall CreateObject();
            static Class * __fastcall GetBaseClass();
            virtual Object * Clone();
        protected:
            virtual int OnPaint(DrawInfo const &);
            GlyphButton(class GlyphButton const &);
            GlyphButton();
        private:
            rend::TexHandle m_glyphTexture;
            rend::TexHandle m_glyphTextureDisabled;
            float m_glyphHeight;
            float m_space;
        };
    }
}

namespace ui
{
    namespace m3d
    {
        class TextBoxWnd :  public FormattedStringsListBoxWnd
        {
        public:
            virtual int SetText(CStr const &);
            static Class * __fastcall GetBaseClass();
            virtual CStr GetText() const ;
            virtual Object * Clone();
            static Object * __fastcall CreateObject();
            int SetVScrollToPos(unsigned int);
            virtual Class * GetClass() const ;
            virtual int Create(CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int);
            int Create(struct BoundsBase<float> const &,unsigned int,int);
            virtual ~TextBoxWnd();
        protected:
            void RecalcLayout();
            virtual int OnPaint(DrawInfo const &);
            TextBoxWnd(class TextBoxWnd const &);
            TextBoxWnd();
        private:
        };
    }
}

namespace ui
{
    namespace m3d
    {
        class ModelWnd :  public ImageWnd
        {
        public:
            virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            int SetTargetTexture(rend::TexHandle);
            AnimInfo * Animation();
            static rend::TexHandle __fastcall CreateTextureAsRenderTarget(class PointBase<int> const &);
            struct CVector & Scale();
            static Class * __fastcall GetBaseClass();
            rend::TexHandle GetTargetTexture() const ;
            virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            virtual int OnPaint(DrawInfo const &);
            struct Quaternion & Rotation();
            virtual Class * GetClass() const ;
            static Object * __fastcall CreateObject();
            int SetModel(AnimatedModel *);
            virtual ~ModelWnd();
            void SetCfgNum(unsigned int);
            virtual Object * Clone();
            unsigned int & SkinNum();
            AnimatedModel * GetModel();
            struct CVector & Translation();
            static class PointBase<int> __fastcall GetFitTargetTextureSize(struct BoundsBase<float> const &);
            virtual int CreateModelWnd(rend::TexHandle,unsigned int,struct BoundsBase<float> const &,unsigned int,rend::TexHandle);
            virtual int CreateModelWnd(CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int,rend::TexHandle);
        protected:
            virtual int CreateImageWnd(struct BoundsBase<float> const &,CStr const &);
            virtual int CreateImageWnd(struct BoundsBase<float> const &,rend::TexHandle);
            ModelWnd(class ModelWnd const &);
            ModelWnd();
        private:
            rend::TexHandle m_renderTexture;
            AnimatedModel *m_Model;
            AnimInfo *m_Animation;
            Configuration m_cfg;
            unsigned int m_SkinNum;
            CVector m_Scale;
            CVector m_Translation;
            Quaternion m_Rotation;
            unsigned int m_LastTimeCalled;
        };
    }
}

namespace ui
{
    namespace m3d
    {
        class ModelArrayWnd :  public ImageWnd
        {
        public:
            rend::TexHandle GetTargetTexture() const ;
            int SetTargetTexture(rend::TexHandle);
            virtual Object * Clone();
            virtual int OnPaint(DrawInfo const &);
            virtual int CreateModelArrayWnd(rend::TexHandle,unsigned int,struct BoundsBase<float> const &,unsigned int,rend::TexHandle);
            virtual int CreateModelArrayWnd(CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int,rend::TexHandle);
            void ClearModels();
            virtual ~ModelArrayWnd();
            int GetSkinNum(int) const ;
            void SetSkinNum(int,int);
            static Object * __fastcall CreateObject();
            static Class * __fastcall GetBaseClass();
            void AddModel(AnimatedModel *,struct CMatrix const &);
            virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            void SetCfgNum(unsigned int,unsigned int);
            virtual Class * GetClass() const ;
            virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        protected:
            ModelArrayWnd();
            ModelArrayWnd(class ModelArrayWnd const &);
            virtual int CreateImageWnd(struct BoundsBase<float> const &,rend::TexHandle);
            virtual int CreateImageWnd(struct BoundsBase<float> const &,CStr const &);
        private:
            rend::TexHandle m_renderTexture;
            std::vector<AnimatedModel *> m_models;
            std::vector<AnimInfo *> m_animations;
            std::vector<CMatrix> m_xForms;
            std::vector<Configuration> m_cfgs;
            std::vector<unsigned int> m_SkinNums;
            CVector m_Scale;
            CVector m_Translation;
            Quaternion m_Rotation;
        };
    }
}


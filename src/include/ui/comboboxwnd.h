#pragma once
#include "font.h"
#include "ui.h"

namespace m3d
{
    class AIParam;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class StringsListBoxWnd;

        class ComboBoxWnd :  public Wnd
        {
        public:
            class AuxInfo
            {
            public:
                AuxInfo();

            private:
                float m_space;
            };

            enum State
            {
                STATE_CLOSE = 0x0,
                STATE_OPEN = 0x1,
            };

        public:
            virtual int GetCurSel() const;
            virtual int GetItemData(int) const;
            virtual BoundsBase<float> GetListBounds() const;
            virtual void SetCurSel(int);
            virtual void SetItemData(int,int);
            unsigned int GetComboStyle() const;
            virtual void Close();
            virtual struct BoundsBase<float> GetFullBounds() const;
            virtual void Open();
            virtual int GetCount() const;
            virtual float GetSelTextFixedHeight() const;
            virtual Class* GetClass() const;
            void SetComboStyle(unsigned int);
            virtual ~ComboBoxWnd();
            virtual void SetBounds(struct BoundsBase<float> const &,bool);
            virtual void SetSelTextFixedHeight(float);
            virtual int Create(CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int);
            virtual int Create(unsigned int,struct BoundsBase<float> const &,int,unsigned int,float,float);
            void SetToggleButtonPane(CStr const &,CStr const &);
            virtual CStr GetItem(int) const ;
            virtual void SetListMaxHeight(float);
            virtual void SetColor(unsigned int);
            virtual int SetText(CStr const &);
            virtual Object * Clone();
            virtual int AddItem(CStr const &);
            virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            virtual void SetPane(CStr const &);
            virtual CStr GetText() const ;
            virtual void SetItem(int,CStr const &);
            float GetMaxListHeight() const ;
            virtual struct BoundsBase<float> GetSelTextBounds() const ;
            virtual struct BoundsBase<float> GetFullMaxBounds() const ;
            virtual void SetTextColor(unsigned int);
            virtual unsigned int GetDrawFlags() const ;
            virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
            virtual void SetTextColorDisabled(unsigned int);
            virtual void SetDefaultFont(CStr const &,float,FontType,union FontParams);
            virtual void SetDefaultFont(int);
            virtual int RemoveItem(int);
            virtual struct BoundsBase<float> GetListMaxBounds() const ;
            static Object * CreateObject();
            virtual void SetDrawFlags(unsigned int);
            virtual int InsertItem(CStr const &,int);
            virtual void SetScrollPane(CStr const &);
            static Class * GetBaseClass();
            virtual int ItemFromPoint(class PointBase<float> const &);
            virtual int RemoveAllItems();
            bool IsOpen() const ;

        protected:
            ComboBoxWnd(ComboBoxWnd const &);
            ComboBoxWnd();
            void UpdateToggleButtonPane();
            virtual void SetState(State,bool);
            virtual void RecalcLayot();
            virtual void SelectItem();
            virtual void ToggleState();
            virtual void RecalcSelfBounds();
            virtual int OnWndNotify(Wnd *,unsigned int,unsigned int,AIParam const &);
            virtual void RecalcListBounds();
            virtual int OnMouseButton0(unsigned int,class PointBase<float> const &);
            virtual int OnAfterRemoveFromWndStation();
            virtual int OnAfterAddToWndStation();

        public:
            RT_CLASS_DECLARE(ComboBoxWnd);

        private:
            StringsListBoxWnd *m_wndStringList;
            ButtonWnd *m_btnToggle;
            Wnd *m_wndSelText;
            AuxInfo m_aif;
            State m_state;
            float m_maxListH;
            float m_selTextFixedH;
            unsigned int m_comboStyle;
            CStr m_toggleButtonOpenPaneName;
            CStr m_toggleButtonClosePaneName;
        };
    }
}

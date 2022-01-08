#pragma once
#include "ui.h"
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd : public Wnd
        {
        public:
            RT_CLASS_DECLARE(ButtonWnd);

        public:
			static Object* CreateObject();
			static Class* GetBaseClass();

        public:
			virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
			rend::TexHandle GetImageDown() const;
			virtual Class* GetClass() const;
			void SetRegular();
			int SetImaged(rend::TexHandle, rend::TexHandle, rend::TexHandle, rend::TexHandle);
			rend::TexHandle GetImageRegular() const;
			virtual Object* Clone();
			bool IsImaged() const;
			virtual ~ButtonWnd();
			virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
			rend::TexHandle GetImageDisabled() const;
			rend::TexHandle GetImageIn() const;
			virtual float GetFrameWidth() const;
			virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);

        protected:
			virtual int OnMouseOut();
			virtual int OnLoosingFocus();
			virtual int OnMouseIn();
		    ButtonWnd(ButtonWnd const&);
			void ReleaseTextures();
			virtual void OnNcPaint(DrawInfo const&, unsigned int);
			virtual int OnPaint(DrawInfo const&);
			virtual int OnMouseButton0(unsigned int, PointBase<float> const&);
			virtual int OnKey(unsigned short, unsigned char, unsigned int);
			ButtonWnd();
			virtual int OnObtainingFocus();

        private:
            int m_isImaged;
            int m_isInside;
            rend::TexHandle m_image;
            rend::TexHandle m_imageMouseIn;
            rend::TexHandle m_imageMouseDown;
            rend::TexHandle m_imageDisabled;
            bool m_isSounded;
        };

        class CheckWnd : public ButtonWnd
        {
        public:
            RT_CLASS_DECLARE(CheckWnd);

        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            virtual Class* GetClass() const;
            void SetCheck(int);
            virtual ~CheckWnd();
            CStr const& GetCheckedPaneName() const;
            virtual Object* Clone();
            int GetCheck() const;
            void SetUncheckedPane(CStr const&);
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            CStr const& GetUncheckedPaneName() const;
            void SetCheckedPane(CStr const&);
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);

        protected:
            virtual int OnMouseButton0(unsigned int, PointBase<float> const&);
            virtual void DrawWndText(DrawInfo const&);
            BoundsBase<float> GetIcoBounds() const;
            virtual void OnNcPaint(DrawInfo const&, unsigned int);
            CheckWnd();
            CheckWnd(class CheckWnd const&);

        private:
            int m_isChecked;
            CStr m_checkedPaneName;
            CStr m_uncheckedPaneName;
        };
    }
}

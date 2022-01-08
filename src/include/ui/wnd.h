#pragma once
#include "ui.h"

namespace m3d
{
    namespace ui
    {
        class ModalWnd : public Wnd
        {
        public:
            RT_CLASS_DECLARE(ModalWnd);

        public:
            static Class* GetBaseClass();
            static Object* CreateObject();

        public:
            int DoModal();
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            virtual int CanClose();
            int IsModal();
            virtual Class* GetClass() const;
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
            ModalWnd(ModalWnd const&);
            ModalWnd();

        private:
            Wnd* m_curControl;
        };
    }
}

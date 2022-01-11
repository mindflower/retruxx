#pragma once
#include "listbox.h"

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd : public FormattedStringsListBoxWnd
        {
        public:
            static Class* GetBaseClass();
            static Object* CreateObject();

        public:
            virtual int SetText(CStr const&);
            virtual CStr GetText() const;
            virtual Object* Clone();
            int SetVScrollToPos(unsigned int);
            virtual Class* GetClass() const;
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            int Create(BoundsBase<float> const&, unsigned int, int);
            virtual ~TextBoxWnd();

        protected:
            void RecalcLayout();
            virtual int OnPaint(DrawInfo const&);
            TextBoxWnd(TextBoxWnd const&);
            TextBoxWnd();

        public:
            RT_CLASS_INLINE_DECLARE(TextBoxWnd);
        };
    }
}

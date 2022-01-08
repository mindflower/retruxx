#pragma once
#include "uilistboxwnd.h"
#include "ui_srv.h"

namespace m3d
{
    namespace ui
    {
        class StringsListBoxWnd : public ListBoxWnd<CStr>
        {
        public:
            static Class* GetBaseClass();
            static Object* CreateObject();

        public:
            virtual Object* Clone();
            virtual ~StringsListBoxWnd();
            virtual Class* GetClass() const;

        protected:
            virtual int RenderItem(int, PointBase<float> const&, DrawInfo const&);
            virtual int MeasureItem(int, BoundsBase<float>&) const;
            virtual int DeleteItem(int);
            StringsListBoxWnd();
            StringsListBoxWnd(StringsListBoxWnd const&);
            virtual int CompareItem(int, int);

        public:
            RT_CLASS_DECLARE(StringsListBoxWnd);
        };

        class FormattedStringsListBoxWnd : public ListBoxWnd<FormattedLine>
        {
        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            virtual ~FormattedStringsListBoxWnd();
            virtual Class* GetClass() const;
            virtual void SetTextColor(unsigned int);
            virtual Object* Clone();
            virtual void SetFormatMode(TextFormatFlags);

        protected:
            virtual int CompareItem(int, int);
            virtual int MeasureItem(int, BoundsBase<float>&) const;
            virtual int DeleteItem(int);
            virtual int RenderItem(int, PointBase<float> const&, DrawInfo const&);
            FormattedStringsListBoxWnd();
            FormattedStringsListBoxWnd(FormattedStringsListBoxWnd const&);

        public:
            RT_CLASS_DECLARE(FormattedStringsListBoxWnd);
        };
    }
}

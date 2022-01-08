#pragma once
#include "ui.h"

namespace m3d
{
    namespace ui
    {
        enum LineWndDirection
        {
            LINEWND_HORIZONTAL = 0x0,
            LINEWND_VERTICAL = 0x1,
        };

        class LineWnd : public Wnd
        {
        public:
            RT_CLASS_DECLARE(LineWnd);

        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            virtual Class* GetClass() const;
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            int Create(BoundsBase<float> const&, unsigned int, LineWndDirection);
            virtual ~LineWnd();
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            void SetDirection(LineWndDirection);
            virtual Object* Clone();
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);

        protected:
            virtual void SetPaneFlags(int);
            virtual void OnNcPaint(DrawInfo const&, unsigned int);
            virtual int OnPaint(DrawInfo const&);
            LineWnd(LineWnd const&);
            LineWnd();

        private:
            LineWndDirection m_direction;
        };
    }
}

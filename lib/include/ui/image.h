#pragma once
#include "ui.h"
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd : public Wnd
        {
        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            int SetImage(CStr const&);
            int SetImage(rend::TexHandle);
            virtual Class* GetClass() const;
            virtual ~ImageWnd();
            rend::TexHandle GetImage() const;
            virtual Object* Clone();
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            virtual int CreateImageWnd(BoundsBase<float> const&, CStr const&);
            virtual int CreateImageWnd(BoundsBase<float> const&, rend::TexHandle);
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            virtual int OnPaint(DrawInfo const&);

        protected:
            ImageWnd();
            ImageWnd(ImageWnd const&);

        public:
            RT_CLASS_DECLARE(ImageWnd);

        protected:
            /* 0x0220 */ CStr m_textureName;
            /* 0x022c */ rend::TexHandle m_texture;
            /* 0x0230 */ bool m_bDrawPane = false;
        };
    }
}

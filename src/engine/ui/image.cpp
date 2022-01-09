#include <ui/image.h>

namespace m3d
{
    namespace ui
    {
        Object* ImageWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        Class* ImageWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::SetImage(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::SetImage(rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        Class* ImageWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        ImageWnd::~ImageWnd()
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle ImageWnd::GetImage() const
        {
            throw std::logic_error("Not implemented");
        }

        Object* ImageWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::CreateImageWnd(BoundsBase<float> const&, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::CreateImageWnd(BoundsBase<float> const&, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int ImageWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        ImageWnd::ImageWnd()
        {
            throw std::logic_error("Not implemented");
        }

        ImageWnd::ImageWnd(ImageWnd const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}

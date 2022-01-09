#include <ui/linewnd.h>

namespace m3d
{
    namespace ui
    {
        Object* LineWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        Class* LineWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        Class* LineWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        int LineWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int LineWnd::Create(BoundsBase<float> const&, unsigned, LineWndDirection)
        {
            throw std::logic_error("Not implemented");
        }

        LineWnd::~LineWnd()
        {
            throw std::logic_error("Not implemented");
        }

        int LineWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void LineWnd::SetDirection(LineWndDirection)
        {
            throw std::logic_error("Not implemented");
        }

        Object* LineWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int LineWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void LineWnd::SetPaneFlags(int)
        {
            throw std::logic_error("Not implemented");
        }

        void LineWnd::OnNcPaint(DrawInfo const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int LineWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        LineWnd::LineWnd(LineWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        LineWnd::LineWnd()
        {
            throw std::logic_error("Not implemented");
        }
    }
}

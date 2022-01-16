#include <ui/frame.h>

namespace m3d
{
    namespace ui
    {
        Frame::~Frame()
        {
            throw std::logic_error("Not implemented");
        }

        void Frame::ReleaseTextures()
        {
            throw std::logic_error("Not implemented");
        }

        Frame::Frame()
        {
            throw std::logic_error("Not implemented");
        }

        int Frame::ReadFromXmlNode(cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        Pane::~Pane()
        {
            throw std::logic_error("Not implemented");
        }

        Pane::Pane()
        {
            throw std::logic_error("Not implemented");
        }

        int Pane::ReadFromXmlNode(cmn::XmlNode*, std::vector<BackGround*> const&, std::vector<Frame*> const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}

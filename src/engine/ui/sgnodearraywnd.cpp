#include <ui/sgnodearraywnd.h>
#include <math/matrix.h>

namespace m3d
{
    namespace ui
    {
        Object* SgNodeArrayWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        Class* SgNodeArrayWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        int SgNodeArrayWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        CVector& SgNodeArrayWnd::Translation()
        {
            throw std::logic_error("Not implemented");
        }

        void SgNodeArrayWnd::AddSgNode(SgNode*, CMatrix const&, bool)
        {
            throw std::logic_error("Not implemented");
        }

        Quaternion& SgNodeArrayWnd::Rotation()
        {
            throw std::logic_error("Not implemented");
        }

        Class* SgNodeArrayWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        SgNodeArrayWnd::~SgNodeArrayWnd()
        {
            throw std::logic_error("Not implemented");
        }

        void SgNodeArrayWnd::SetClassesToRender(std::vector<Class*> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void SgNodeArrayWnd::ClearSgNodes()
        {
            throw std::logic_error("Not implemented");
        }

        int SgNodeArrayWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        Object* SgNodeArrayWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle SgNodeArrayWnd::GetTargetTexture() const
        {
            throw std::logic_error("Not implemented");
        }

        int SgNodeArrayWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        int SgNodeArrayWnd::CreateSgNodeArrayWnd(CStr const&, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int SgNodeArrayWnd::CreateSgNodeArrayWnd(rend::TexHandle, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int SgNodeArrayWnd::SetTargetTexture(rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int SgNodeArrayWnd::CreateImageWnd(BoundsBase<float> const&, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int SgNodeArrayWnd::CreateImageWnd(BoundsBase<float> const&, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        SgNodeArrayWnd::SgNodeArrayWnd()
        {
            throw std::logic_error("Not implemented");
        }

        SgNodeArrayWnd::SgNodeArrayWnd(SgNodeArrayWnd const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}

#include <ui/sgnodearraywnd.h>
#include <math/matrix.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(SgNodeArrayWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(SgNodeArrayWnd);

        Object* SgNodeArrayWnd::CreateObject()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* SgNodeArrayWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ImageWnd);
        }

        int SgNodeArrayWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        CVector& SgNodeArrayWnd::Translation()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void SgNodeArrayWnd::AddSgNode(SgNode*, CMatrix const&, bool)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Quaternion& SgNodeArrayWnd::Rotation()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* SgNodeArrayWnd::GetClass() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        SgNodeArrayWnd::~SgNodeArrayWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void SgNodeArrayWnd::SetClassesToRender(std::vector<Class*> const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void SgNodeArrayWnd::ClearSgNodes()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int SgNodeArrayWnd::OnPaint(DrawInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* SgNodeArrayWnd::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        rend::TexHandle SgNodeArrayWnd::GetTargetTexture() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int SgNodeArrayWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int SgNodeArrayWnd::CreateSgNodeArrayWnd(CStr const&, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int SgNodeArrayWnd::CreateSgNodeArrayWnd(rend::TexHandle, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int SgNodeArrayWnd::SetTargetTexture(rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int SgNodeArrayWnd::CreateImageWnd(BoundsBase<float> const&, rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int SgNodeArrayWnd::CreateImageWnd(BoundsBase<float> const&, CStr const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        SgNodeArrayWnd::SgNodeArrayWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        SgNodeArrayWnd::SgNodeArrayWnd(SgNodeArrayWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}

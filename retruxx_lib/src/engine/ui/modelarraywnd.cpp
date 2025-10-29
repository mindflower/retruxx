#include <ui/modelarraywnd.h>
#include <skelmodel.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(ModelArrayWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(ModelArrayWnd);

        Object* ModelArrayWnd::CreateObject()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* ModelArrayWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ImageWnd);
        }

        rend::TexHandle ModelArrayWnd::GetTargetTexture() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::SetTargetTexture(rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* ModelArrayWnd::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::OnPaint(DrawInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::CreateModelArrayWnd(rend::TexHandle, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::CreateModelArrayWnd(CStr const&, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void ModelArrayWnd::ClearModels()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        ModelArrayWnd::~ModelArrayWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::GetSkinNum(int) const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void ModelArrayWnd::SetSkinNum(int, int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void ModelArrayWnd::AddModel(AnimatedModel*, CMatrix const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void ModelArrayWnd::SetCfgNum(unsigned, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* ModelArrayWnd::GetClass() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        ModelArrayWnd::ModelArrayWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        ModelArrayWnd::ModelArrayWnd(ModelArrayWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::CreateImageWnd(BoundsBase<float> const&, rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int ModelArrayWnd::CreateImageWnd(BoundsBase<float> const&, CStr const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}

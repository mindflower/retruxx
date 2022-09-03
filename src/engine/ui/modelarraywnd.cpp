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
            throw std::logic_error("Not implemented");
        }

        Class* ModelArrayWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle ModelArrayWnd::GetTargetTexture() const
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::SetTargetTexture(rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        Object* ModelArrayWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::CreateModelArrayWnd(rend::TexHandle, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::CreateModelArrayWnd(CStr const&, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        void ModelArrayWnd::ClearModels()
        {
            throw std::logic_error("Not implemented");
        }

        ModelArrayWnd::~ModelArrayWnd()
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::GetSkinNum(int) const
        {
            throw std::logic_error("Not implemented");
        }

        void ModelArrayWnd::SetSkinNum(int, int)
        {
            throw std::logic_error("Not implemented");
        }

        void ModelArrayWnd::AddModel(AnimatedModel*, CMatrix const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void ModelArrayWnd::SetCfgNum(unsigned, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Class* ModelArrayWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        ModelArrayWnd::ModelArrayWnd()
        {
            throw std::logic_error("Not implemented");
        }

        ModelArrayWnd::ModelArrayWnd(ModelArrayWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::CreateImageWnd(BoundsBase<float> const&, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelArrayWnd::CreateImageWnd(BoundsBase<float> const&, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}

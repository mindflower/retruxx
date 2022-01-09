#include <ui/modelwnd.h>

namespace m3d
{
    namespace ui
    {
        Class* ModelWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        Object* ModelWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle ModelWnd::CreateTextureAsRenderTarget(PointBase<int> const&)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<int> ModelWnd::GetFitTargetTextureSize(BoundsBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::SetTargetTexture(rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        AnimInfo* ModelWnd::Animation()
        {
            throw std::logic_error("Not implemented");
        }

        CVector& ModelWnd::Scale()
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle ModelWnd::GetTargetTexture() const
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        Quaternion& ModelWnd::Rotation()
        {
            throw std::logic_error("Not implemented");
        }

        Class* ModelWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::SetModel(AnimatedModel*)
        {
            throw std::logic_error("Not implemented");
        }

        ModelWnd::~ModelWnd()
        {
            throw std::logic_error("Not implemented");
        }

        void ModelWnd::SetCfgNum(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Object* ModelWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned& ModelWnd::SkinNum()
        {
            throw std::logic_error("Not implemented");
        }

        AnimatedModel* ModelWnd::GetModel()
        {
            throw std::logic_error("Not implemented");
        }

        CVector& ModelWnd::Translation()
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::CreateModelWnd(rend::TexHandle, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::CreateModelWnd(CStr const&, unsigned, BoundsBase<float> const&, unsigned, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::CreateImageWnd(BoundsBase<float> const&, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int ModelWnd::CreateImageWnd(BoundsBase<float> const&, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        ModelWnd::ModelWnd(ModelWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        ModelWnd::ModelWnd()
        {
            throw std::logic_error("Not implemented");
        }
    }
}

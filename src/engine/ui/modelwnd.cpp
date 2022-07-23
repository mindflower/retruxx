#include <ui/modelwnd.h>

#include "m3dapp.h"
#include "core/kernel.h"
#include "core/timer.h"

namespace m3d
{
    namespace ui
    {
        RT_CLASS_DEFINE(ModelWnd);

        Class* ModelWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ImageWnd);
        }

        Object* ModelWnd::CreateObject()
        {
            return new ModelWnd;
        }

        rend::TexHandle ModelWnd::CreateTextureAsRenderTarget(PointBase<int> const&)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<int> ModelWnd::GetFitTargetTextureSize(BoundsBase<float> const& wndBounds)
        {
            //TODO: check this and refactor
            PointBase<int> result; // eax
            float v3; // xmm0_4
            int* v4; // ecx
            int v5; // edx
            bool v6; // cc
            char v7; // cl
            int v8; // edx
            int v9; // [esp+4h] [ebp-8h] BYREF
            int v10; // [esp+8h] [ebp-4h] BYREF

            v9 = wndBounds.height;
            v3 = wndBounds.width;
            v10 = v9;
            v9 = v3;
            v4 = &v10;
            if (v9 >= v10)
                v4 = &v9;
            v5 = *v4;
            v6 = *v4 < 4;
            v7 = 2;
            if (!v6)
            {
                do
                    ++v7;
                while (1 << v7 <= v5);
            }
            v8 = 1 << v7;
            if (1 << v7 >= 32)
            {
                if (v8 > 512)
                    v8 = 512;
                result.x = v8;
                result.y = v8;
            }
            else
            {
                result.x = 32;
                result.y = 32;
            }
            return result;
        }

        int ModelWnd::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
        {
            auto result = ImageWnd::ReadFromXmlNode(file, node);
            if (result)
            {
                //CVector rot;
                //CMatrix rotMatr;
                //m3d::SafeVectorAttrib(rot, node, "ModelRotation");
            	//rotMatr.rotYPR(rot.y, rot.x, rot.z);
                //m_Rotation.FromMatrix(rotMatr);
                //TODO: check this!!!
                m3d::SafeQuaternionAttrib(m_Rotation, node, "ModelRotation");
                m3d::SafeVectorAttrib(m_Translation, node, "ModelTranslation");
                m3d::SafeVectorAttrib(m_Scale, node, "ModelScale");

                //TODO: check this!!!!!Q
                m_renderTexture = m3d::Application::g_pApp->m_renderer->GetBufferedTargetTexture(GetFitTargetTextureSize(m_bounds).x);
                m3d::Application::g_pApp->m_renderer->ReferenceTexture(m_renderTexture);
                result = 1;
            }
            return result;
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
            return RT_CLASS_LOCAL(ModelWnd);
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
            m_LastTimeCalled = m3d::g_Kernel->GetTimer().GetCurTimeUnscaled();
        }
    }
}

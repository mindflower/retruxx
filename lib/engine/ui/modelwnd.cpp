#include <ui/modelwnd.h>

#include "m3dapp.h"
#include "core/kernel.h"
#include "core/timer.h"
#include "core/ini.h"
#include <math/matrix.h>
#include <math/quaternion.h>
#include <ui/ui_srv.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(ModelWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(ModelWnd);

        Class* ModelWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ImageWnd);
        }

        Object* ModelWnd::CreateObject()
        {
            return new ModelWnd;
        }

        rend::TexHandle ModelWnd::CreateTextureAsRenderTarget(PointBase<int> const& texSz)
        {
            // RVA 0x707D50
            return M3D_RENDERER->GetBufferedTargetTexture(texSz.x);
        }

        PointBase<int> ModelWnd::GetFitTargetTextureSize(BoundsBase<float> const& wndBounds)
        {
            //TODO: check this and refactor
            PointBase<int> result;  // eax
            float v3;               // xmm0_4
            int* v4;                // ecx
            int v5;                 // edx
            bool v6;                // cc
            char v7;                // cl
            int v8;                 // edx
            int v9;                 // [esp+4h] [ebp-8h] BYREF
            int v10;                // [esp+8h] [ebp-4h] BYREF

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
                // RVA 0x708650: "ModelRotation" is stored as a yaw/pitch/roll vector
                // (the format WriteToXmlNode emits and the shipped game data uses),
                // not as a quaternion.
                CVector rot;
                m3d::SafeVectorAttrib(rot, node, "ModelRotation");
                CMatrix rotMatr;
                rotMatr.rotYPR(rot.y, rot.x, rot.z);
                m_Rotation.FromMatrix(rotMatr);
                m3d::SafeVectorAttrib(m_Translation, node, "ModelTranslation");
                m3d::SafeVectorAttrib(m_Scale, node, "ModelScale");

                auto const targetTex = M3D_RENDERER->GetBufferedTargetTexture(GetFitTargetTextureSize(m_bounds).x);
                M3D_RENDERER->ReleaseTexture(m_renderTexture);
                m_renderTexture = targetTex;
                M3D_RENDERER->ReferenceTexture(m_renderTexture);
                result = 1;
            }
            return result;
        }

        int ModelWnd::SetTargetTexture(rend::TexHandle targetTex)
        {
            // RVA 0x7085F0
            M3D_RENDERER->ReleaseTexture(m_renderTexture);
            m_renderTexture = targetTex;
            M3D_RENDERER->ReferenceTexture(targetTex);
            return 1;
        }

        AnimInfo* ModelWnd::Animation()
        {
            return m_Animation;
        }

        CVector& ModelWnd::Scale()
        {
            return m_Scale;
        }

        rend::TexHandle ModelWnd::GetTargetTexture() const
        {
            return m_renderTexture;
        }

        int ModelWnd::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
        {
            auto result = ImageWnd::WriteToXmlNode(file, writeTo);
            if (result)
            {
                // RVA 0x7080A0: emit the rotation as a yaw/pitch/roll vector.
                CMatrix const rotMatr = m_Rotation.ToMatrix();
                CVector ypr;
                rotMatr.getYPR(ypr.y, ypr.x, ypr.z);
                writeTo->SetAttribute("ModelRotation", CStr(ypr).c_str());
                writeTo->SetAttribute("ModelTranslation", CStr(m_Translation).c_str());
                writeTo->SetAttribute("ModelScale", CStr(m_Scale).c_str());
                result = 1;
            }
            return result;
        }

        int ModelWnd::OnPaint(DrawInfo const& di)
        {
            // RVA 0x7089C0
            auto const clientB = GetClientBounds();
            if (m_texture.IsValid())
            {
                GetGfxServer()->AddImagedRect(di, clientB, m_curClr, m_texture);
            }
            if (m_paneFlags)
            {
                unsigned const clr = ((m_style & 2) != 0 || (m_style & 0x80000) != 0) ? 3u : m_curClr;
                auto const bounds = GetBounds();
                BoundsBase<float> paneRect;
                paneRect.x0 = 0.0f;
                paneRect.y0 = 0.0f;
                paneRect.width = bounds.width;
                paneRect.height = bounds.height;
                GetGfxServer()->AddFlatAxialPane0(di, paneRect, clr, m_paneFlags, m_paneName, m_bgFlags);
            }

            // TODO(RVA 0x7089C0): the 3D model preview. When m_Model is set and
            // IRenderer::RenderToTexStart(m_renderTexture, true) succeeds the shipped
            // code clears the target (M3DCLEAR_CZ), sets one white directional light,
            // pushes a perspective projection (cot(PI/8) on _11/_22, near/far ~1.001),
            // advances m_Animation by (curTimeUnscaled - m_LastTimeCalled) and calls
            // m_Model->Update(m_Animation, false, &m_cfg), clamps the skin index
            // against m_Model->m_loadSkins / m_Skins, composes
            // scale * quat(m_Rotation) * translate(m_Translation) and calls
            // m_Model->Render(matr, m_Animation, m_cfg, skinNum), pops the render
            // state and blits m_renderTexture into clientB via
            // GfxServer::AddImagedRectGeneral with aspect-corrected UVs. Left unported:
            // it inlines a dozen unnamed IRenderer state calls plus a full CMatrix
            // multiply chain. The 2D parts above keep the window drawing.
            return 1;
        }

        Quaternion& ModelWnd::Rotation()
        {
            return m_Rotation;
        }

        Class* ModelWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(ModelWnd);
        }

        int ModelWnd::SetModel(AnimatedModel* Mdl)
        {
            // RVA 0x70A140
            delete m_Animation;
            m_Model = Mdl;
            m_Animation = new AnimInfo();
            m_Animation->CreateFor(m_Model);
            if (m_Model)
            {
                m_cfg.m_num = 0;
                m_Model->FromCfgNum(m_cfg);
                m_Model->CalculateMeshes(m_cfg);
            }
            return 1;
        }

        ModelWnd::~ModelWnd()
        {
            delete m_Animation;
            M3D_RENDERER->ReleaseTexture(m_renderTexture);
        }

        unsigned ModelWnd::GetCfgNum()
        {
            return m_cfg.m_num;
        }

        void ModelWnd::SetCfgNum(unsigned cfgNum)
        {
            // RVA 0x707D70
            if (m_Model)
            {
                m_cfg.m_num = cfgNum;
                m_Model->FromCfgNum(m_cfg);
                m_Model->CalculateMeshes(m_cfg);
            }
        }

        Object* ModelWnd::Clone()
        {
            // RVA 0x70A060 allocates a ModelWnd and runs the copy ctor (which asserts
            // false) before returning null - ModelWnd is effectively non-cloneable.
            return new ModelWnd(*this);
        }

        unsigned& ModelWnd::SkinNum()
        {
            // RVA 0x514850
            return m_SkinNum;
        }

        AnimatedModel* ModelWnd::GetModel()
        {
            return m_Model;
        }

        CVector& ModelWnd::Translation()
        {
            return m_Translation;
        }

        int ModelWnd::CreateModelWnd(
            rend::TexHandle imageTex,
            unsigned style,
            BoundsBase<float> const& rc,
            unsigned id,
            rend::TexHandle targetTex)
        {
            if (!style)
            {
                style = 832;
            }
            if (Wnd::Create({}, style, rc, id) == 0)
            {
                return 0;
            }
            m_texture = imageTex;
            M3D_RENDERER->ReferenceTexture(m_texture);
            if (m_texture.IsValid())
            {
                m_renderTexture = targetTex;
                M3D_RENDERER->ReferenceTexture(m_renderTexture);
            }
            else
            {
                m_renderTexture = M3D_RENDERER->GetBufferedTargetTexture(GetFitTargetTextureSize(GetBounds()).x);
                if ((style & 0x80000000) != 0)
                {
                    return 0;
                }
            }
            return 1;
        }

        int ModelWnd::CreateModelWnd(
            CStr const& imageFile,
            unsigned style,
            BoundsBase<float> const& rc,
            unsigned id,
            rend::TexHandle targetTex)
        {
            // RVA 0x708420: load the named texture, forward to the TexHandle overload,
            // then drop the extra reference the load took.
            m_textureName = imageFile;
            rend::TexHandle tex = M3D_RENDERER->AddTexture(imageFile, 4);
            int const res = CreateModelWnd(tex, style, rc, id, targetTex);
            M3D_RENDERER->ReleaseTexture(tex);
            return res;
        }

        int ModelWnd::CreateImageWnd(BoundsBase<float> const& rc, CStr const& textureName)
        {
            // RVA 0x707D40 (thunk)
            return ImageWnd::CreateImageWnd(rc, textureName);
        }

        int ModelWnd::CreateImageWnd(BoundsBase<float> const& rc, rend::TexHandle tex)
        {
            // RVA 0x7085D0 (thunk)
            return ImageWnd::CreateImageWnd(rc, tex);
        }

        ModelWnd::ModelWnd(ModelWnd const& rhs) : ImageWnd(rhs)
        {
            // RVA 0x709FF0 builds the ImageWnd base, resets m_renderTexture / m_cfg,
            // then asserts false: ModelWnd has no working copy constructor.
            RETRUXX_NOT_IMPLEMENTED;
        }

        ModelWnd::ModelWnd() :
            m_Model(nullptr),
            m_Animation(nullptr),
            m_SkinNum(0),
            m_Scale(1.0f, 1.0f, 1.0f),
            m_Translation(0.0f, 0.0f, 0.0f),
            m_Rotation(0.0f, 0.0f, 0.0f, 1.0f)
        {
            // RVA 0x709EE0: m_renderTexture stays invalid and m_cfg stays empty via
            // their own default ctors; seed the frame-time baseline OnPaint deltas
            // against.
            m_LastTimeCalled = m3d::g_Kernel->GetTimer().GetCurTimeUnscaled();
        }
    }  // namespace ui
}  // namespace m3d

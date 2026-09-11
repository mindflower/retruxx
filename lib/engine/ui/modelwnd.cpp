#include <ui/modelwnd.h>

#include <cmath>

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
            // RVA 0x708010 - the render target is square and a power of two, just
            // large enough to cover the longer side of the window, clamped to
            // [32, 512].
            int const w = static_cast<int>(wndBounds.width);
            int const h = static_cast<int>(wndBounds.height);
            int const maxSide = w >= h ? w : h;

            int shift = 2;
            if (maxSide >= 4)
            {
                do
                {
                    ++shift;
                } while ((1 << shift) <= maxSide);
            }

            int size = 1 << shift;
            if (size < 32)
            {
                size = 32;
            }
            else if (size > 512)
            {
                size = 512;
            }

            PointBase<int> result;
            result.x = size;
            result.y = size;
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

            // The 3D model preview: render the animated model into m_renderTexture
            // and blit it back over the client rect.
            if (!m_Model || !M3D_RENDERER->RenderToTexStart(m_renderTexture, true))
            {
                return 1;
            }

            M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, 0);

            // One directional light aimed down (-1, -1, -1). NOTE: the shipped code
            // leaves the light's colour fields uninitialised; only these are set.
            CVector const lightDir(-1.0f, -1.0f, -1.0f);
            rend::LightSource light;
            light.m_type = rend::M3DLIGHT_DIRECTIONAL;
            light.m_origin = lightDir;
            light.m_direction = lightDir;
            light.m_range = 1000.0f;
            M3D_RENDERER->LightSet(0, light);
            M3D_RENDERER->SetLighting(true, false);
            M3D_RENDERER->LightEnable(0, 1);

            // 45-degree vertical FOV, square aspect, near/far planes at 1 and ~1000.
            CMatrix matProj;
            matProj.zero();
            float const projScale = static_cast<float>(1.0 / std::tan(0.3926990926265717));  // cot(PI/8)
            matProj._11 = projScale;
            matProj._22 = projScale;
            matProj._33 = 1.001001f;
            matProj._34 = 1.0f;
            matProj._43 = -1.001001f;
            M3D_RENDERER->MatPushProj();
            M3D_RENDERER->MatSetProj(matProj);

            CMatrix world;
            world.identity();
            M3D_RENDERER->MatSet(world);

            // Isolate the preview from the scene's render state; each of these is
            // undone by the matching Pop* after the model is drawn.
            M3D_RENDERER->PushFog(false);
            M3D_RENDERER->PushZbState(rend::ZB_ENABLE);
            M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
            M3D_RENDERER->SetAlphaTest(1);
            M3D_RENDERER->PushBlend(rend::BM_NONE);
            for (int stage = 0; stage < 8; ++stage)
            {
                M3D_RENDERER->TgDisable(stage);
            }

            // Advance the animation by the unscaled wall-clock delta since last paint.
            unsigned const nowUnscaled = m3d::g_Kernel->GetTimer().GetCurTimeUnscaled();
            m_Animation->MoveFrame(nowUnscaled - m_LastTimeCalled);
            m_LastTimeCalled = nowUnscaled;
            m_Model->Update(m_Animation, false, &m_cfg);

            // Resolve the requested skin against what the model actually has loaded.
            int skinNum = static_cast<int>(m_SkinNum);
            LoadSkins const& loaded = m_Model->GetLoadedSkins();
            if (!loaded.loadAllSkins)
            {
                auto const& loadSkins = loaded.loadSkins;
                if (!loadSkins.empty() && loadSkins.find(skinNum) == loadSkins.end())
                {
                    skinNum = *loadSkins.begin();
                }
            }
            else
            {
                int const lastSkin = static_cast<int>(m_Model->GetNumSkins()) - 1;
                if (static_cast<int>(m_SkinNum) < 0)
                {
                    m_SkinNum = 0;
                    skinNum = 0;
                }
                if (static_cast<int>(m_SkinNum) > lastSkin)
                {
                    skinNum = lastSkin;
                }
            }

            // Model transform, applied left to right to a row vector:
            //   yaw(180) * scale(m_Scale) * quat(m_Rotation) * translate(m_Translation)
            CMatrix flipY;
            flipY.identity();
            flipY._11 = -1.0f;  // cos(PI)
            flipY._33 = -1.0f;

            CMatrix scale;
            scale.identity();
            scale._11 = m_Scale.x;
            scale._22 = m_Scale.y;
            scale._33 = m_Scale.z;

            CMatrix translate;
            translate.identity();
            translate._41 = m_Translation.x;
            translate._42 = m_Translation.y;
            translate._43 = m_Translation.z;

            CMatrix const modelMatr = flipY * scale * m_Rotation.ToMatrix() * translate;
            m_Model->Render(modelMatr, m_Animation, m_cfg, static_cast<unsigned>(skinNum));

            M3D_RENDERER->PopZbState();
            M3D_RENDERER->PopFog();
            M3D_RENDERER->MatPopProj();
            M3D_RENDERER->PopCull();
            M3D_RENDERER->PopBlend();
            M3D_RENDERER->RenderToTexFinish();

            if (m_renderTexture.IsValid())
            {
                // Centre the square render target inside the client rect.
                float u0 = 0.0f;
                float v0 = 0.0f;
                if (clientB.width <= clientB.height)
                {
                    u0 = (1.0f - clientB.width / clientB.height) * 0.5f;
                }
                else
                {
                    v0 = (1.0f - clientB.height / clientB.width) * 0.5f;
                }
                GetGfxServer()->AddImagedRectGeneral(
                    di, clientB, m_curClr, m_renderTexture, u0, v0, 1.0f - u0, 1.0f - v0);
            }
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

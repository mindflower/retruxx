#include <ui/modelarraywnd.h>
#include <skelmodel.h>
#include <cmath>
#include <m3dapp.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <math/matrix.h>
#include <ui/modelwnd.h>
#include <ui/ui_srv.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(ModelArrayWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(ModelArrayWnd);

        Object* ModelArrayWnd::CreateObject()
        {
            // RVA 0x798210
            return new ModelArrayWnd();
        }

        Class* ModelArrayWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ImageWnd);
        }

        rend::TexHandle ModelArrayWnd::GetTargetTexture() const
        {
            // RVA 0x794AD0
            return m_renderTexture;
        }

        int ModelArrayWnd::SetTargetTexture(rend::TexHandle targetTex)
        {
            // RVA 0x794A80
            M3D_RENDERER->ReleaseTexture(m_renderTexture);
            m_renderTexture = targetTex;
            M3D_RENDERER->ReferenceTexture(targetTex);
            return 1;
        }

        Object* ModelArrayWnd::Clone()
        {
            // RVA 0x7981E0 - the copy constructor refuses, so this never returns a window.
            return new ModelArrayWnd(*this);
        }

        int ModelArrayWnd::OnPaint(DrawInfo const& di)
        {
            // RVA 0x7955C0 - draws every model, each with its own transform on top of the window's shared one, into
            // the render texture and shows it over the client rect. NOTE: unlike ModelWnd the animations are not
            // advanced, so the models keep the pose they were added in.
            BoundsBase<float> const clientB = GetClientBounds();
            if (m_texture.IsValid())
            {
                m_gfx->AddImagedRect(di, clientB, m_curClr, m_texture);
            }
            if (m_paneFlags)
            {
                unsigned const clr = ((m_style & 2) != 0 || (m_style & 0x80000) != 0) ? 3u : m_curClr;
                BoundsBase<float> const bounds = GetBounds();
                BoundsBase<float> paneRect;
                paneRect.x0 = 0.0f;
                paneRect.y0 = 0.0f;
                paneRect.width = bounds.width;
                paneRect.height = bounds.height;
                m_gfx->AddFlatAxialPane0(di, paneRect, clr, m_paneFlags, m_paneName, m_bgFlags);
            }

            if (m_models.empty() || !M3D_RENDERER->RenderToTexStart(m_renderTexture, true))
            {
                return 1;
            }

            auto* const renderer = M3D_RENDERER;
            renderer->ClearViewport(rend::M3DCLEAR_CZ, 0);
            renderer->MatGet();
            CMatrix identity;
            identity.zero();
            identity._11 = 1.0f;
            identity._22 = 1.0f;
            identity._33 = 1.0f;
            identity._44 = 1.0f;
            renderer->MatSet(identity);
            renderer->PushFog(false);
            renderer->PushZbState(rend::ZB_ENABLE);
            renderer->PushCull(rend::M3DCULL_CCW);
            renderer->SetAlphaTest(1);
            renderer->PushBlend(rend::BM_NONE);
            for (int stage = 0; stage < 8; ++stage)
            {
                renderer->TgDisable(stage);
            }

            // 45-degree vertical FOV, square aspect, near/far planes at 1 and ~1000.
            CMatrix matProj;
            matProj.zero();
            float const projScale = static_cast<float>(1.0 / std::tan(0.3926990926265717));
            matProj._11 = projScale;
            matProj._22 = projScale;
            matProj._33 = 1.001001f;
            matProj._34 = 1.0f;
            matProj._43 = -1.001001f;
            renderer->MatPushProj();
            renderer->MatSetProj(matProj);

            // Shared transform, applied left to right to a row vector:
            //   yaw(180) * scale(m_Scale) * quat(m_Rotation) * translate(m_Translation)
            float const s = static_cast<float>(std::sin(3.141592741012573));
            float const c = static_cast<float>(std::cos(3.141592741012573));
            CMatrix yaw;
            yaw.identity();
            yaw._11 = c;
            yaw._13 = -s;
            yaw._31 = s;
            yaw._33 = c;
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
            CMatrix const matr = yaw * scale * m_Rotation.ToMatrix() * translate;

            // One directional light aimed down (-1, -1, -1). NOTE: its colour fields are left unset.
            CVector const lightDir(-1.0f, -1.0f, -1.0f);
            rend::LightSource light;
            light.m_type = rend::M3DLIGHT_DIRECTIONAL;
            light.m_direction = lightDir;
            light.m_origin = lightDir;
            light.m_range = 1000.0f;
            renderer->LightSet(0, light);
            renderer->SetLighting(true, false);
            renderer->LightEnable(0, 1);

            for (size_t i = 0; i < m_models.size(); ++i)
            {
                AnimatedModel* const model = m_models[i];
                // Resolve the requested skin against what the model actually has loaded.
                int skinNum = static_cast<int>(m_SkinNums[i]);
                LoadSkins const& loaded = model->GetLoadedSkins();
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
                    int const lastSkin = static_cast<int>(model->GetNumSkins()) - 1;
                    if (skinNum < 0)
                    {
                        skinNum = 0;
                    }
                    if (skinNum > lastSkin)
                    {
                        skinNum = lastSkin;
                    }
                }
                model->Render(m_xForms[i] * matr, m_animations[i], m_cfgs[i], static_cast<unsigned>(skinNum));
            }

            renderer->PopZbState();
            renderer->PopFog();
            renderer->MatPopProj();
            renderer->PopCull();
            renderer->PopBlend();
            renderer->RenderToTexFinish();

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
                m_gfx->AddImagedRectGeneral(di, clientB, m_curClr, m_renderTexture, u0, v0, 1.0f - u0, 1.0f - v0);
            }
            return 1;
        }

        int ModelArrayWnd::CreateModelArrayWnd(rend::TexHandle imageTex, unsigned style, BoundsBase<float> const& rc,
            unsigned id, rend::TexHandle targetTex)
        {
            // RVA 0x794970 - without a target texture one sized for the window is made.
            if (!style)
            {
                style = 832;
            }
            if (!Wnd::Create(CStr(""), style, rc, id))
            {
                return 0;
            }
            m_texture = imageTex;
            M3D_RENDERER->ReferenceTexture(m_texture);
            if (targetTex.IsValid())
            {
                m_renderTexture = targetTex;
                M3D_RENDERER->ReferenceTexture(m_renderTexture);
                return 1;
            }
            m_renderTexture = ModelWnd::CreateTextureAsRenderTarget(ModelWnd::GetFitTargetTextureSize(m_bounds));
            return m_renderTexture.IsValid() ? 1 : 0;
        }

        int ModelArrayWnd::CreateModelArrayWnd(CStr const& imageFile, unsigned style, BoundsBase<float> const& rc,
            unsigned id, rend::TexHandle targetTex)
        {
            // RVA 0x7948C0
            m_textureName = imageFile;
            rend::TexHandle tex = M3D_RENDERER->AddTexture(CStr(imageFile.c_str()), 4);
            int const res = CreateModelArrayWnd(tex, style, rc, id, targetTex);
            M3D_RENDERER->ReleaseTexture(tex);
            return res;
        }

        void ModelArrayWnd::ClearModels()
        {
            // RVA 0x798010
            m_xForms.clear();
            m_models.clear();
            for (auto& anim : m_animations)
            {
                delete anim;
                anim = nullptr;
            }
            m_animations.clear();
            m_cfgs.clear();
            m_SkinNums.clear();
        }

        ModelArrayWnd::~ModelArrayWnd()
        {
            // RVA 0x797E00
            for (auto& anim : m_animations)
            {
                delete anim;
                anim = nullptr;
            }
            M3D_RENDERER->ReleaseTexture(m_renderTexture);
        }

        int ModelArrayWnd::GetSkinNum(int modelNum) const
        {
            // RVA 0x794F40
            if (modelNum < 0 || modelNum >= static_cast<int>(m_SkinNums.size()))
            {
                return -1;
            }
            return m_SkinNums[modelNum];
        }

        void ModelArrayWnd::SetSkinNum(int skinNum, int modelNum)
        {
            // RVA 0x794F80
            if (modelNum >= 0 && modelNum < static_cast<int>(m_SkinNums.size()))
            {
                m_SkinNums[modelNum] = skinNum;
            }
        }

        void ModelArrayWnd::AddModel(AnimatedModel* model, CMatrix const& xForm)
        {
            // RVA 0x798260 - the model is posed once, standing, with configuration 0 and skin 0.
            m_models.push_back(model);
            m_animations.push_back(new AnimInfo());
            m_animations.back()->CreateFor(model);
            m_animations.back()->SetAnimation(AT_STAND1);
            m_models.back()->Update(m_animations.back(), false, nullptr);
            m_xForms.push_back(xForm);
            m_cfgs.push_back(Configuration());
            SetCfgNum(static_cast<unsigned>(m_cfgs.size()) - 1, 0);
            m_SkinNums.push_back(0);
        }

        int ModelArrayWnd::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
        {
            // RVA 0x794AE0 - the rotation is stored as yaw/pitch/roll (in y, x, z) and a new render target is made for
            // the window's size.
            if (!ImageWnd::ReadFromXmlNode(file, node))
            {
                return 0;
            }
            CVector rot(0.0f, 0.0f, 0.0f);
            SafeVectorAttrib(rot, node, "ModelRotation");
            CMatrix rotMatr;
            rotMatr.rotYPR(rot.y, rot.x, rot.z);
            m_Rotation.FromMatrix(rotMatr);
            SafeVectorAttrib(m_Translation, node, "ModelTranslation");
            SafeVectorAttrib(m_Scale, node, "ModelScale");

            rend::TexHandle tex = ModelWnd::CreateTextureAsRenderTarget(ModelWnd::GetFitTargetTextureSize(m_bounds));
            M3D_RENDERER->ReleaseTexture(m_renderTexture);
            m_renderTexture = tex;
            M3D_RENDERER->ReferenceTexture(tex);
            M3D_RENDERER->ReleaseTexture(tex);
            return 1;
        }

        void ModelArrayWnd::SetCfgNum(unsigned modelNum, unsigned cfgNum)
        {
            // RVA 0x796F80
            if (modelNum < m_models.size())
            {
                m_cfgs[modelNum].m_num = cfgNum;
                m_models[modelNum]->FromCfgNum(m_cfgs[modelNum]);
                m_models[modelNum]->CalculateMeshes(m_cfgs[modelNum]);
            }
        }

        Class* ModelArrayWnd::GetClass() const
        {
            // RVA 0x793BB0
            return RT_CLASS_LOCAL(ModelArrayWnd);
        }

        int ModelArrayWnd::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
        {
            // RVA 0x794260
            if (!ImageWnd::WriteToXmlNode(file, writeTo))
            {
                return 0;
            }
            CVector ypr;
            m_Rotation.ToMatrix().getYPR(ypr.y, ypr.x, ypr.z);
            writeTo->SetAttribute("ModelRotation", CStr(ypr).c_str());
            writeTo->SetAttribute("ModelTranslation", CStr(m_Translation).c_str());
            writeTo->SetAttribute("ModelScale", CStr(m_Scale).c_str());
            return 1;
        }

        ModelArrayWnd::ModelArrayWnd()
        {
            // RVA 0x797CF0
            m_Scale = CVector(1.0f, 1.0f, 1.0f);
            m_Translation = CVector(0.0f, 0.0f, 0.0f);
            m_Rotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }

        ModelArrayWnd::ModelArrayWnd(ModelArrayWnd const& rhs) : ImageWnd(rhs)
        {
            // RVA 0x797F70 - model windows cannot be copied.
            SYS_ERROR("false");
        }

        int ModelArrayWnd::CreateImageWnd(BoundsBase<float> const& rc, rend::TexHandle tex)
        {
            // RVA 0x794A60
            return ImageWnd::CreateImageWnd(rc, tex);
        }

        int ModelArrayWnd::CreateImageWnd(BoundsBase<float> const& rc, CStr const& textureName)
        {
            // RVA 0x793BC0
            return ImageWnd::CreateImageWnd(rc, textureName);
        }
    }
}

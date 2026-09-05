#include "scene/nodes/sgnodeanimatedmodel.h"

#include <ui/sgnodearraywnd.h>
#include <ui/modelwnd.h>
#include <ui/ui_srv.h>
#include <math/matrix.h>
#include <scene/scenegraph.h>
#include <core/kernel.h>
#include <core/ini.h>
#include <m3dapp.h>

#include <cmath>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(SgNodeArrayWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(SgNodeArrayWnd);

        Object* SgNodeArrayWnd::CreateObject()
        {
            return new SgNodeArrayWnd;
        }

        Class* SgNodeArrayWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ImageWnd);
        }

        Class* SgNodeArrayWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(SgNodeArrayWnd);
        }

        Object* SgNodeArrayWnd::Clone()
        {
            // RVA 0x701370 allocates a SgNodeArrayWnd and runs the copy ctor (which
            // asserts false) before returning null - SgNodeArrayWnd is effectively
            // non-cloneable.
            return new SgNodeArrayWnd(*this);
        }

        int SgNodeArrayWnd::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
        {
            auto result = ImageWnd::WriteToXmlNode(file, writeTo);
            if (result)
            {
                // RVA 0x6FDBB0: emit the rotation as a yaw/pitch/roll vector, the
                // same format ReadFromXmlNode expects.
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

        int SgNodeArrayWnd::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
        {
            auto result = ImageWnd::ReadFromXmlNode(file, node);
            if (result)
            {
                // RVA 0x6FE230: "ModelRotation" is stored as a yaw/pitch/roll vector,
                // not as a quaternion.
                CVector rot;
                m3d::SafeVectorAttrib(rot, node, "ModelRotation");
                CMatrix rotMatr;
                rotMatr.rotYPR(rot.y, rot.x, rot.z);
                m_Rotation.FromMatrix(rotMatr);
                m3d::SafeVectorAttrib(m_Translation, node, "ModelTranslation");
                m3d::SafeVectorAttrib(m_Scale, node, "ModelScale");

                // Size the render target to the window, then drop the extra
                // reference the create took - SetTargetTexture holds its own.
                auto targetTex = ModelWnd::CreateTextureAsRenderTarget(ModelWnd::GetFitTargetTextureSize(m_bounds));
                SetTargetTexture(targetTex);
                M3D_RENDERER->ReleaseTexture(targetTex);
                result = 1;
            }
            return result;
        }

        int SgNodeArrayWnd::CreateSgNodeArrayWnd(
            CStr const& imageFile,
            unsigned style,
            BoundsBase<float> const& rc,
            unsigned id,
            rend::TexHandle targetTex)
        {
            // RVA 0x6FE010: load the named texture, forward to the TexHandle
            // overload, then drop the extra reference the load took.
            m_textureName = imageFile;
            rend::TexHandle tex = M3D_RENDERER->AddTexture(imageFile, 4);
            int const res = CreateSgNodeArrayWnd(tex, style, rc, id, targetTex);
            M3D_RENDERER->ReleaseTexture(tex);
            return res;
        }

        int SgNodeArrayWnd::CreateSgNodeArrayWnd(
            rend::TexHandle imageTex,
            unsigned style,
            BoundsBase<float> const& rc,
            unsigned id,
            rend::TexHandle targetTex)
        {
            // RVA 0x6FE0C0
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
            if (targetTex.IsValid())
            {
                m_renderTexture = targetTex;
                M3D_RENDERER->ReferenceTexture(m_renderTexture);
            }
            else
            {
                // No target supplied - allocate one sized to fit this window.
                m_renderTexture = ModelWnd::CreateTextureAsRenderTarget(ModelWnd::GetFitTargetTextureSize(m_bounds));
                if (!m_renderTexture.IsValid())
                {
                    return 0;
                }
            }
            return 1;
        }

        int SgNodeArrayWnd::SetTargetTexture(rend::TexHandle targetTex)
        {
            // RVA 0x6FE1D0
            M3D_RENDERER->ReleaseTexture(m_renderTexture);
            m_renderTexture = targetTex;
            M3D_RENDERER->ReferenceTexture(targetTex);
            return 1;
        }

        rend::TexHandle SgNodeArrayWnd::GetTargetTexture() const
        {
            return m_renderTexture;
        }

        int SgNodeArrayWnd::CreateImageWnd(BoundsBase<float> const& rc, rend::TexHandle tex)
        {
            // RVA 0x6FE1B0 (thunk)
            return ImageWnd::CreateImageWnd(rc, tex);
        }

        int SgNodeArrayWnd::CreateImageWnd(BoundsBase<float> const& rc, CStr const& textureName)
        {
            // RVA 0x6FD960 (thunk)
            return ImageWnd::CreateImageWnd(rc, textureName);
        }

        void SgNodeArrayWnd::AddSgNode(SgNode* node, CMatrix const& xForm, bool withChilds)
        {
            // RVA 0x7014D0 - the three vectors are kept index-parallel.
            m_nodes.push_back(node);
            m_xForms.push_back(xForm);
            m_withChilds.push_back(withChilds);
        }

        void SgNodeArrayWnd::ClearSgNodes()
        {
            // RVA 0x701280
            m_xForms.clear();
            m_nodes.clear();
            m_withChilds.clear();
        }

        void SgNodeArrayWnd::SetClassesToRender(std::vector<Class*> const& classes)
        {
            m_classesToRender = classes;
        }

        CVector& SgNodeArrayWnd::Translation()
        {
            return m_Translation;
        }

        Quaternion& SgNodeArrayWnd::Rotation()
        {
            return m_Rotation;
        }

        CVector& SgNodeArrayWnd::Scale()
        {
            return m_Scale;
        }

        int SgNodeArrayWnd::OnPaint(DrawInfo const& di)
        {
            // RVA 0x6FE910
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

            // Render the collected scene-graph nodes into m_renderTexture and blit
            // the result back over the client rect.
            if (m_nodes.empty() || !M3D_RENDERER->RenderToTexStart(m_renderTexture, true))
            {
                return 1;
            }

            M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, 0);

            CMatrix world;
            world.identity();
            M3D_RENDERER->MatSet(world);

            // Isolate the preview from the scene's render state; both are undone by
            // the matching Pop* once the nodes are drawn.
            M3D_RENDERER->PushFog(false);
            M3D_RENDERER->PushZbState(rend::ZB_ENABLE);

            // 45-degree vertical FOV, square aspect, near/far planes at 1 and ~1000.
            CMatrix matProj;
            matProj.zero();
            float const projScale = static_cast<float>(1.0 / std::tan(0.3926990926265717));  // cot(PI/8)
            matProj._11 = projScale;
            matProj._22 = projScale;
            matProj._33 = 1.001001f;
            matProj._34 = 1.0f;
            matProj._43 = -1.001001f;
            M3D_RENDERER->MatSetProj(matProj);

            // Window transform, applied left to right to a row vector:
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

            CMatrix const wndMatr = flipY * scale * m_Rotation.ToMatrix() * translate;

            // One directional light aimed down (-1, -1, -1). NOTE: as in ModelWnd the
            // shipped code leaves the light's colour fields uninitialised.
            CVector const lightDir(-1.0f, -1.0f, -1.0f);
            rend::LightSource light;
            light.m_type = rend::M3DLIGHT_DIRECTIONAL;
            light.m_origin = lightDir;
            light.m_direction = lightDir;
            light.m_range = 1000.0f;
            M3D_RENDERER->LightSet(0, light);
            M3D_RENDERER->SetLighting(true, false);
            M3D_RENDERER->LightEnable(0, 1);

            // NOTE: every node is drawn through the graph of the *first* one, so all
            // nodes added to this window are assumed to share a scene graph.
            SceneGraph* sg = m_nodes.front()->GetGraph();

            for (unsigned classIdx = 0; classIdx < m_classesToRender.size(); ++classIdx)
            {
                Class* const wantClass = m_classesToRender[classIdx];
                for (unsigned nodeIdx = 0; nodeIdx < m_nodes.size(); ++nodeIdx)
                {
                    SgNode* const node = m_nodes[nodeIdx];
                    CMatrix const nodeMatr = m_xForms[nodeIdx] * wndMatr;
                    if (node->IsKindOf(wantClass))
                    {
                        sg->RenderNode(node, nodeMatr, false);
                    }

                    if (!m_withChilds[nodeIdx])
                    {
                        continue;
                    }

                    // Depth-first walk of the node's descendants. Each one is placed
                    // relative to the node we were handed, so the subtree keeps its
                    // shape regardless of where the node sits in the world.
                    CMatrix const toNodeSpace = node->m_currentXForm.getInverseRotTranslate();
                    std::vector<Object*> stack;
                    stack.push_back(node);
                    while (!stack.empty())
                    {
                        Object* const cur = stack.back();
                        stack.pop_back();
                        for (Object* it = cur->GetFirstChild(); it; it = it->GetNextSibling())
                        {
                            auto* const child = static_cast<SgNode*>(it);
                            if (child->IsKindOf(wantClass))
                            {
                                CMatrix const childMatr = child->m_currentXForm * toNodeSpace * nodeMatr;
                                sg->RenderNode(child, childMatr, false);
                            }
                            // Only descend into children that have children of their own.
                            if (child->GetFirstChild())
                            {
                                stack.push_back(child);
                            }
                        }
                    }
                }
            }

            M3D_RENDERER->PopZbState();
            M3D_RENDERER->PopFog();
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

        SgNodeArrayWnd::SgNodeArrayWnd()
        {
            // RVA 0x700FB0
            m_Scale.x = 1.0f;
            m_Scale.y = 1.0f;
            m_Scale.z = 1.0f;
            m_Translation.x = 0.0f;
            m_Translation.y = 0.0f;
            m_Translation.z = 0.0f;
            m_Rotation.x = 0.0f;
            m_Rotation.y = 0.0f;
            m_Rotation.z = 0.0f;
            m_Rotation.w = 1.0f;

            m_classesToRender.push_back(RT_CLASS_LOCAL(SgAnimatedModelNode));
        }

        SgNodeArrayWnd::SgNodeArrayWnd(SgNodeArrayWnd const& rhs) : ImageWnd(rhs)
        {
            // RVA 0x7011F0 builds the ImageWnd base and resets the node/xform members,
            // then asserts false: SgNodeArrayWnd has no working copy constructor.
            M3D_ASSERT(false);
        }

        SgNodeArrayWnd::~SgNodeArrayWnd()
        {
            // RVA 0x701110 - the member containers clean themselves up.
            M3D_RENDERER->ReleaseTexture(m_renderTexture);
        }
    }  // namespace ui
}  // namespace m3d

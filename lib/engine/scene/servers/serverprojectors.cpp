#include <m3dapp.h>
#include <config.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/profilerstack.h>
#include <core/ref_ptr.h>
#include <core/timer.h>
#include <scene/servers/serverprojectors.h>
#include <cmath>
#include <clipper.h>
#include <client.h>
#include <landscape.h>
#include <level.h>
#include <world.h>
#include <scene/scenegraph.h>
#include "engine/landscape/roads/roadmanager.h"

namespace
{
    // The void* every DataServer::Model carries is, for this server, the
    // projector's description as read from its .xml.
    struct ProjectorProto
    {
        /* 0x0000 */ int m_radius = 0;
        /* 0x0004 */ m3d::rend::TexHandle m_texture;
    };

    // Property id: read a projector's radius back as a float.
    int const PROJECTOR_RADIUS = 10241;

    // What SgProjectorNode hands to RenderItem: its world transform (the projector looks along the Y axis) and
    // radius.
    struct ProjectorRenderInfo
    {
        /* 0x0000 */ CMatrix m_xform;
        /* 0x0040 */ float m_radius;
    };

    float const VISCELL_EDGE_LENGTH_16 = 128.0f;
}

struct ProjectorStats
{
    /* 0x0000 */ int numProjToRender;
    /* 0x0004 */ int numCellsRendered;
    /* 0x0008 */ int numModelsRendered;
    /* 0x000c */ int curFrame;

    void Zero()
    {
        // RVA 0x75EE00 - NOTE: curFrame is deliberately left alone; it tracks
        // which frame the counters were last shown for, not a count itself.
        this->numModelsRendered = 0;
        this->numProjToRender = 0;
        this->numCellsRendered = 0;
    }

    ProjectorStats()
    {
        this->curFrame = 0;
        this->numModelsRendered = 0;
        this->numProjToRender = 0;
        this->numCellsRendered = 0;
    }
}; /* size: 0x0010 */

namespace m3d
{
    ProjectorsServer::~ProjectorsServer()
    {
        // RVA 0x760C20 - the shared attenuation ramp outlives the individual
        // projectors, so it goes after them.
        ProjectorsServer::Release();
        M3D_RENDERER->ReleaseTexture(m_attenuationTex);
    }

    int ProjectorsServer::GetItemProperty(int id, int prop, void* dest)
    {
        // RVA 0x75F220 - the radius is stored as an int but handed out as a
        // float, since that is what the scene nodes work in.
        if (prop != PROJECTOR_RADIUS)
        {
            return DataServer::GetItemProperty(id, prop, dest);
        }

        auto const* projector = static_cast<ProjectorProto const*>(m_models[id].m_ptr);
        *static_cast<float*>(dest) = static_cast<float>(projector->m_radius);
        return 1;
    }

    int ProjectorsServer::SetItemProperty(int id, int prop, void* src)
    {
        // RVA 0x75EE30 - nothing projector specific to write.
        return DataServer::SetItemProperty(id, prop, src);
    }

    int ProjectorsServer::SaveAllLoadedEntities(char const*)
    {
        // RVA 0x75EE80 - projectors are never written back out.
        return 1;
    }

    int ProjectorsServer::Init()
    {
        m_valid = true;
        return 1;
    }

    ProjectorsServer::ProjectorsServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("projectors", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
        m_attenuationTex = Application::g_pApp->m_renderer->AddTexture("data\\textures\\attenuation_256.tga", 4);
        Application::g_pApp->m_renderer->SetTextureParameter(m_attenuationTex, rend::TM_TEX_FILTER, 1);
        Application::g_pApp->m_renderer->SetTextureParameter(m_attenuationTex, rend::TM_WRAP_S, 3);
        Application::g_pApp->m_renderer->SetTextureParameter(m_attenuationTex, rend::TM_WRAP_T, 3);

        m_texBiasMat._12 = 0.0;
        m_texBiasMat._13 = 0.0;
        m_texBiasMat._14 = 0.0;
        m_texBiasMat._21 = 0.0;
        m_texBiasMat._23 = 0.0;
        m_texBiasMat._24 = 0.0;
        m_texBiasMat._31 = 0.0;
        m_texBiasMat._32 = 0.0;
        m_texBiasMat._34 = 0.0;
        m_texBiasMat._44 = 1.0;
        m_texBiasMat._11 = 0.5;
        m_texBiasMat._33 = 0.5;
        m_texBiasMat._41 = 0.5;
        m_texBiasMat._42 = 0.5;
        m_texBiasMat._43 = 0.5;
        m_texBiasMat._22 = -0.5;

        memset(&m_texProjMat, 0, sizeof(m_texProjMat));
        m_texProjMat._34 = 1.0;
        m_texProjMat._33 = 1.001001;
        m_texProjMat._43 = -1.001001;
        m_texProjMat._11 = 1.0 / tan(0.5235987901687622);
        m_texProjMat._22 = m_texProjMat._11;

        m_texProjBiasMat._11 = (((m_texBiasMat._31 * m_texProjMat._13) + (m_texBiasMat._21 * m_texProjMat._12))
            + (m_texBiasMat._11 * m_texProjMat._11))
            + (m_texProjMat._14 * m_texBiasMat._41);
        m_texProjBiasMat._12 = (((m_texBiasMat._42 * m_texProjMat._14) + (m_texBiasMat._12 * m_texProjMat._11)) + (m_texBiasMat._22 * m_texProjMat._12))
            + (m_texBiasMat._32 * m_texProjMat._13);
        m_texProjBiasMat._13 = (((m_texBiasMat._13 * m_texProjMat._11) + (m_texProjMat._14 * m_texBiasMat._43))
            + (m_texBiasMat._23 * m_texProjMat._12))
            + (m_texBiasMat._33 * m_texProjMat._13);
        m_texProjBiasMat._14 = (((m_texBiasMat._34 * m_texProjMat._13) + (m_texBiasMat._44 * m_texProjMat._14)) + (m_texProjMat._11 * m_texBiasMat._14))
            + (m_texBiasMat._24 * m_texProjMat._12);
        m_texProjBiasMat._21 = (((m_texBiasMat._31 * m_texProjMat._23) + (m_texBiasMat._11 * m_texProjMat._21)) + (m_texProjMat._24 * m_texBiasMat._41))
            + (m_texBiasMat._21 * m_texProjMat._22);
        m_texProjBiasMat._22 = (((m_texBiasMat._12 * m_texProjMat._21) + (m_texProjMat._22 * m_texBiasMat._22)) + (m_texProjMat._24 * m_texBiasMat._42))
            + (m_texProjMat._23 * m_texBiasMat._32);
        m_texProjBiasMat._23 = (((m_texBiasMat._13 * m_texProjMat._21) + (m_texProjMat._24 * m_texBiasMat._43)) + (m_texProjMat._23 * m_texBiasMat._33))
            + (m_texProjMat._22 * m_texBiasMat._23);
        m_texProjBiasMat._24 = (((m_texBiasMat._44 * m_texProjMat._24) + (m_texBiasMat._34 * m_texProjMat._23)) + (m_texProjMat._22 * m_texBiasMat._24))
            + (m_texBiasMat._14 * m_texProjMat._21);
        m_texProjBiasMat._31 = (((m_texBiasMat._31 * m_texProjMat._33) + (m_texBiasMat._11 * m_texProjMat._31)) + (m_texProjMat._34 * m_texBiasMat._41))
            + (m_texProjMat._32 * m_texBiasMat._21);
        m_texProjBiasMat._32 = (((m_texProjMat._32 * m_texBiasMat._22) + (m_texBiasMat._12 * m_texProjMat._31))
            + (m_texProjMat._34 * m_texBiasMat._42))
            + (m_texProjMat._33 * m_texBiasMat._32);
        m_texProjBiasMat._33 = (((m_texProjMat._34 * m_texBiasMat._43) + (m_texProjMat._33 * m_texBiasMat._33)) + (m_texBiasMat._13 * m_texProjMat._31))
            + (m_texProjMat._32 * m_texBiasMat._23);
        m_texProjBiasMat._34 = (((m_texBiasMat._44 * m_texProjMat._34) + (m_texBiasMat._34 * m_texProjMat._33)) + (m_texProjMat._32 * m_texBiasMat._24))
            + (m_texBiasMat._14 * m_texProjMat._31);
        m_texProjBiasMat._41 = (((m_texBiasMat._31 * m_texProjMat._43) + (m_texBiasMat._11 * m_texProjMat._41)) + (m_texProjMat._44 * m_texBiasMat._41))
            + (m_texProjMat._42 * m_texBiasMat._21);
        m_texProjBiasMat._42 = (((m_texProjMat._42 * m_texBiasMat._22) + (m_texBiasMat._12 * m_texProjMat._41)) + (m_texProjMat._44 * m_texBiasMat._42))
            + (m_texProjMat._43 * m_texBiasMat._32);
        m_texProjBiasMat._43 = (((m_texProjMat._43 * m_texBiasMat._33) + (m_texProjMat._44 * m_texBiasMat._43)) + (m_texBiasMat._13 * m_texProjMat._41))
            + (m_texProjMat._42 * m_texBiasMat._23);
        m_texProjBiasMat._44 = (((m_texBiasMat._44 * m_texProjMat._44) + (m_texBiasMat._34 * m_texProjMat._43)) + (m_texProjMat._42 * m_texBiasMat._24))
            + (m_texBiasMat._14 * m_texProjMat._41);

        memset(&m_attenMat, 0, sizeof(m_attenMat));
        m_attenMat._31 = 0.0099999998;
        m_attenMat._41 = 0.039999999;
    }

    int ProjectorsServer::Release()
    {
        // RVA 0x75F5F0
        m_valid = false;

        for (auto& model : m_models)
        {
            auto* projector = static_cast<ProjectorProto*>(model.m_ptr);
            M3D_RENDERER->ReleaseTexture(projector->m_texture);
            delete projector;
        }
        m_models.clear();

        return 1;
    }

    int ProjectorsServer::AddItem(char const* params, char const* id)
    {
        // RVA 0x75F6B0
        int const existing = GetItemByName(id, false);
        if (existing != -1)
        {
            return existing;
        }

        Proto proto;
        int paramsPos;
        ParseProto(params, &proto, &paramsPos);
        if (proto != PROTO_FILE)
        {
            M3D_LOG_INFO("Protocol is not supported: " + CStr(proto));
            return -1;
        }

        char const* fileName = params + paramsPos;

        CStr errorStr;
        ref_ptr<cmn::XmlFile> xmlFile = ReadXmlFile(fileName, &errorStr);
        if (!xmlFile)
        {
            M3D_LOG_INFO("ProjectorsServer:: cannot load " + CStr(fileName) + " err: " + errorStr);
            // NOTE: the shipped code returns 0 here, not -1, so a projector
            // whose file is missing silently aliases whichever projector is
            // first in the table instead of being reported as a failure.
            return 0;
        }

        ref_ptr<cmn::XmlNode> node = xmlFile->CreateNode();
        xmlFile->GetFirstChild(node, "Projector");
        if (node->IsEmpty())
        {
            M3D_LOG_INFO("ProjectorsServer:: projector descriptor missing in " + CStr(fileName));
            return -1;
        }

        char const* texName = node->GetAttribute("Texture");
        int const radius = strToInt(CStr(node->GetAttribute("Radius")));

        auto* projector = new ProjectorProto();
        projector->m_radius = radius;
        projector->m_texture = M3D_RENDERER->AddTexture(CStr(texName), 0);

        // Clamped at the edges so the cone does not tile outside its footprint.
        M3D_RENDERER->SetTextureParameter(projector->m_texture, rend::TM_WRAP_S, 3);
        M3D_RENDERER->SetTextureParameter(projector->m_texture, rend::TM_WRAP_T, 3);

        m_models.push_back(Model(projector, fileName, fileName, id));
        return m_models.size() - 1;
    }

    void ProjectorsServer::RenderItem(int id, void* params)
    {
        // RVA 0x75FB60 - the three negative ids bracket a batch of
        // projectors: -2 sets the render state up, -3 tears it down and -4 is a
        // no-op marker.
        static ProjectorStats stats;
        m_profiler->StartCountdown();
        switch (id)
        {
        case -2:
        {
            overlayStart();
            M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
            M3D_RENDERER->SetAlphaTest(0);
            M3D_RENDERER->PushLighting(false);
            M3D_RENDERER->PushBlend(rend::BM_DCOLOR_1);
            M3D_RENDERER->PushFog(false);
            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
            M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
            M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
            M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);

            // The counters are accumulated by the per-projector branch and
            // drawn (and reset) once per frame here.
            int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
            if (M3D_ENGINE_CFG.m_g_showProjectorsStats.GetB() && stats.curFrame != curFrame)
            {
                M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
                M3D_APP->SetFont(CStr("Lucida Console"), 10.0f, 1, M3D_APP->m_codePage.CodePage);

                CStr statStr;
                statStr.format("%-12s %8d", "numProjectors", stats.numProjToRender);
                M3D_APP->DrawTextRel(724.0f, 284.0f, 0xFF888888, statStr, 0, -1);
                statStr.format("%-12s %8d", "numProjCells", stats.numCellsRendered);
                M3D_APP->DrawTextRel(724.0f, 296.0f, 0xFF888888, statStr, 0, -1);
                statStr.format("%-12s %8d", "numProjModels", stats.numModelsRendered);
                M3D_APP->DrawTextRel(724.0f, 308.0f, 0xFF888888, statStr, 0, -1);

                M3D_RENDERER->PopZbState();

                stats.Zero();
                stats.curFrame = curFrame;
            }
            break;
        }
        case -3:
        {
            overlayStop();
            M3D_RENDERER->PopCull();
            M3D_RENDERER->SetAlphaTest(0);
            M3D_RENDERER->PopLighting();
            M3D_RENDERER->PopBlend();
            M3D_RENDERER->PopFog();
            // NOTE: the teardown re-applies the same stage states the setup
            // did rather than restoring anything, so it leaves stage 0 on
            // MODULATE/TEXTURE for whatever draws next.
            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
            M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
            M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
            M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
            break;
        }
        case -4:
        {
            break;
        }
        default:
        {
            // One projector: the landscape cells, roads and animated models inside its 60-degree frustum are drawn
            // again with its texture projected onto them.
            auto const* const ri = static_cast<ProjectorRenderInfo const*>(params);
            CVector const org(ri->m_xform._41, ri->m_xform._42, ri->m_xform._43);
            CVector const dir(ri->m_xform._21, ri->m_xform._22, ri->m_xform._23);
            CVector const target(dir.x + org.x, dir.y + org.y, dir.z + org.z);
            CMatrix projViewMatrix;
            projViewMatrix.lookAtLH(org, target, CVector(0.0f, 1.0f, 0.0f));
            CMatrix const projWorldMat = projViewMatrix * m_texProjBiasMat;

            auto* const proto = static_cast<ProjectorProto*>(m_models[id].m_ptr);
            auto* const renderer = M3D_RENDERER;
            rend::TexHandle tex = proto->m_texture;
            renderer->SetTexture(0, tex, -1.0);
            float const projRadius = static_cast<float>(proto->m_radius);
            CClipper projectorFrusta;
            projectorFrusta.createScreenFrustums(org, projViewMatrix, 1.0471976f, 1.0471976f, 0.1f, projRadius);

            CWorld& world = pClient->GetWorld();
            SceneGraph& graph = world.m_sceneGraph;
            Landscape& ls = world.m_landscape;
            rend::IEffect* const lsShader = graph.m_lsProjectorShader;
            rend::IEffect* const roadShader = graph.m_roadProjectorShader;
            retruxx::vector<unsigned int> roadCells;
            retruxx::set<SgNode*> projNodes;

            renderer->TgSetTransformMode(0, rend::TG_PROJ_PS11);
            renderer->SingleLayerStencilStart();
            lsShader->SetMatrix(rend::IEffect::User_float4x4_param, projWorldMat);
            roadShader->SetMatrix(rend::IEffect::User_float4x4_param, projWorldMat);
            lsShader->SetVector3(rend::IEffect::User_float4_param, org);
            {
                CVector const n = dir.getNormalized();
                lsShader->SetVector3(rend::IEffect::User_float3_param, CVector(0.0f - n.x, 0.0f - n.y, 0.0f - n.z));
            }
            lsShader->SetFloat(rend::IEffect::User_float_param, projRadius);
            roadShader->SetVector3(rend::IEffect::User_float4_param, org);
            {
                CVector const n = dir.getNormalized();
                roadShader->SetVector3(rend::IEffect::User_float3_param, CVector(0.0f - n.x, 0.0f - n.y, 0.0f - n.z));
            }
            roadShader->SetFloat(rend::IEffect::User_float_param, projRadius);

            // Fade distance and weather-dependent strength, shared with the object and tree shaders.
            float const fadeDist = static_cast<float>(
                (static_cast<double>(M3D_ENGINE_CFG.m_g_projectorsFarDist.GetI()) - 0.5) * VISCELL_EDGE_LENGTH_16);
            lsShader->SetFloat(rend::IEffect::User_float_param2, fadeDist);
            roadShader->SetFloat(rend::IEffect::User_float_param2, fadeDist);
            unsigned char const specular = static_cast<unsigned char>(world.GetWeatherSpecularColor());
            float const strength =
                static_cast<float>(1.3 - static_cast<double>(static_cast<int>(specular) * 0.0039215689f));
            lsShader->SetFloat(rend::IEffect::User_float_param3, strength);
            roadShader->SetFloat(rend::IEffect::User_float_param3, strength);
            graph.m_objProjectorShader->SetFloat(rend::IEffect::User_float_param2, fadeDist);
            graph.m_treeProjectorShader->SetFloat(rend::IEffect::User_float_param2, fadeDist);
            graph.m_objProjectorShader->SetFloat(rend::IEffect::User_float_param3, strength);
            graph.m_treeProjectorShader->SetFloat(rend::IEffect::User_float_param3, strength);

            graph.m_sortedCellsEndRadius = M3D_ENGINE_CFG.m_g_projectorsFarDist.GetI();
            graph.m_sortedCellsCurCell = 0;
            graph.m_sortedCellsCurRadius = 0;
            int x;
            int z;
            int v;
            int radius;
            while (graph.SortedCellsFetch(x, z, v, radius))
            {
                if (!v)
                {
                    continue;
                }
                int const landSize = ls.m_owner->m_level->land_size;
                float const cellRadius = VISCELL_EDGE_LENGTH_16 * 0.70700002f;
                Landscape::CellParams const& cell = ls.m_cellParams[x + z * landSize];
                CVector const cellOrg((static_cast<float>(x) + 0.5f) * VISCELL_EDGE_LENGTH_16,
                    (cell.m_h1 + cell.m_h0) * 0.5f, (static_cast<float>(z) + 0.5f) * VISCELL_EDGE_LENGTH_16);
                if (projectorFrusta.testSphere(cellOrg, cellRadius))
                {
                    ls.drawCellOverlayedShader(x, z, lsShader);
                    roadCells.push_back(x + (z << 16));
                    graph.CollectNodesProjector(projNodes, x, z, projectorFrusta);
                    ++stats.numCellsRendered;
                }
            }

            renderer->PushCull(rend::M3DCULL_CCW);
            RoadInFrustumTest roadTest(&projectorFrusta);
            world.m_roadManager.RenderRoads(roadCells, RRT_FOR_PROJECTOR, &roadTest, false);
            renderer->SingleLayerStencilFinish();

            if (!projNodes.empty())
            {
                retruxx::vector<SgNode*> nodesVector(projNodes.begin(), projNodes.end());
                RenderNodeInfo rni;
                rni.rnt = RNT_FOR_PROJECTOR;
                rni.isCullInverted = false;
                rni.isPrimaryRender = false;
                rni.isUseImpostors = true;
                float const invLen = 1.0f / std::sqrt(dir.x * dir.x + dir.z * dir.z + dir.y * dir.y + 0.00000011920929f);
                rni.projDir = CVector(0.0f - invLen * dir.x, 0.0f - dir.y * invLen, 0.0f - dir.z * invLen);
                rni.projOrg = org;
                rni.projTansform = projWorldMat;
                graph.m_objProjectorShader->SetFloat(rend::IEffect::User_float_param, projRadius);
                graph.m_treeProjectorShader->SetFloat(rend::IEffect::User_float_param, projRadius);
                renderer->TgSetTcSource(1, rend::TC_FROM_VERTEX, 1);
                renderer->SetStageState(1, rend::BM_COLOR, rend::TS_PREV);
                renderer->SetStageState(1, rend::BM_ALPHA, rend::TS_TEXTURE);
                renderer->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestWorld.GetI());
                M3D_APP->GetAnimatedModelsServer().RenderNodeSet(
                    nodesVector.data(), static_cast<unsigned>(nodesVector.size()), rni);
                renderer->SetAlphaTest(0);
                renderer->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
                renderer->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
            }
            renderer->PopCull();
            renderer->TgSetTransformMode(0, rend::TG_DISABLE);
            renderer->TgDisable(1);
            stats.numModelsRendered += static_cast<int>(projNodes.size());
            ++stats.numProjToRender;
            break;
        }
        }
        m_profiler->EndCountdown();
    }

    int ProjectorsServer::RemoveItem(int)
    {
        // RVA 0x75EE20 - projector prototypes live for the whole level, so
        // there is nothing to take out one at a time.
        return 1;
    }

    void ProjectorsServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>& itemslist)
    {
        // RVA 0x75EF10
        for (unsigned i = 0; i < itemslist.size(); ++i)
        {
            if (m_fnLoadCallback)
            {
                // NOTE: integer arithmetic, so the reported progress steps in
                // whole percent and stays at 0 until 1/100th of the list is in.
                m_fnLoadCallback(i * 100 / itemslist.size(), m_fnLoadCallbackData);
            }

            CStr const fn = itemslist[i].m_filename;
            CStr const id = itemslist[i].m_id;
            if (AddItem(fn.c_str(), id.c_str()) == -1)
            {
                // NOTE: the shipped message says "DataServer", not
                // "ProjectorsServer" - a copy-paste in the original.
                M3D_LOG_INFO("DataServer: cannot read " + fn + " id = " + id);
            }
        }
    }
}

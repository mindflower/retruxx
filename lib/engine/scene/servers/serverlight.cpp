#include <flares.h>
#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serverlight.h>
#include <core/log.h>
#include <core/ini.h>
#include <core/ref_ptr.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <config.h>
#include <client.h>
#include <world.h>
#include <level.h>

int statsInited = 0;

struct PointLightStats
{
    /* 0x0000 */ int numLightsToRender = 0;
    /* 0x0004 */ int numLightCellsRendered = 0;
    /* 0x0008 */ int numLightModelsRendered = 0;
    /* 0x000c */ int curFrame = 0;

    void Zero()
    {
        int numLightsToRender = 0;
        int numLightCellsRendered = 0;
        int numLightModelsRendered = 0;
    }

}; /* size: 0x0010 */

namespace m3d
{
    int LightsServer::SaveAllLoadedEntities(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LightsServer::Init()
    {
        m_valid = true;
        CFlare::Init();
        return 1;
    }

    int LightsServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LightsServer::AddItem(char const* params, char const* id)
    {
        // RVA 0x76C220
        auto item = GetItemByName(id, false);
        if (item == -1)
        {
            int protoPos = 0;
            DataServer::Proto proto;
            ParseProto(params, &proto, &protoPos);
            if (proto == PROTO_FILE)
            {
                auto const filename = &params[protoPos];
                CStr err;
                if (ref_ptr xmlFile = m3d::ReadXmlFile(&params[protoPos], &err))
                {
                    ref_ptr xmlNode = xmlFile->CreateNode();
                    xmlFile->GetFirstChild(xmlNode, "Light");
                    if (xmlNode->IsEmpty())
                    {
                        M3D_LOG_INFO("LightsServer:: light descriptor missing in " + CStr(filename));
                        return -1;
                    }

                    // Attributes missing from the descriptor keep these defaults.
                    int radius = 10;
                    CVector color(1.0f, 1.0f, 1.0f);
                    float ttl = -1.0f;

                    SafeIntAttrib(radius, xmlNode, "Radius");
                    SafeVectorAttrib(color, xmlNode, "Color");
                    SafeFloatAttrib(ttl, xmlNode, "Ttl");

                    auto pointModel = new PointLightModel();
                    pointModel->m_radius = radius;
                    pointModel->m_color = color;
                    pointModel->m_ttl = ttl;

                    auto model = DataServer::Model(pointModel, filename, filename, id);
                    m_models.push_back(std::move(model));
                    return m_models.size() - 1;
                }

                // NOTE: a light file that cannot be read yields item 0 instead of -1.
                M3D_LOG_INFO("LightServer:: cannot load " + CStr(filename) + " err: " + err);
                return 0;
            }
            else
            {
                M3D_LOG_INFO("protocol is not supported");
                return -1;
            }
        }
        return item;
    }

    LightsServer::~LightsServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void LightsServer::RenderItem(int id, void* params)
    {
        // RVA 0x76C690 - a point light brightens the landscape cells, roads and models inside its radius.
        static PointLightStats stats_0;
        m_profiler->StartCountdown();

        auto* const renderer = M3D_RENDERER;
        switch (id)
        {
        case -2:
        {
            overlayStart();
            renderer->PushCull(rend::M3DCULL_CCW);
            renderer->SetAlphaTest(0);
            renderer->PushLighting(false);
            renderer->PushBlend(rend::BM_DCOLOR_1);
            renderer->PushFog(false);
            renderer->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
            renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
            renderer->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
            renderer->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);

            // Once a frame, print what the previous frame's lights cost and start counting afresh.
            if (M3D_ENGINE_CFG.m_g_showProjectorsStats.GetB() && stats_0.curFrame != M3D_KERNEL->GetTimer().GetCurFrame())
            {
                renderer->PushZbState(rend::ZB_DISABLE);
                M3D_APP->SetFont(CStr("Lucida Console"), 10.0f, 1, M3D_APP->m_codePage.CodePage);
                CStr statStr;
                statStr.format("%-12s %8d", "numLights", stats_0.numLightsToRender);
                M3D_APP->DrawTextRel(724.0f, 320.0f, 0xFF888888, statStr, 0, -1);
                statStr.format("%-12s %8d", "numLightCells", stats_0.numLightCellsRendered);
                M3D_APP->DrawTextRel(724.0f, 332.0f, 0xFF888888, statStr, 0, -1);
                statStr.format("%-12s %8d", "numLightModels", stats_0.numLightModelsRendered);
                M3D_APP->DrawTextRel(724.0f, 344.0f, 0xFF888888, statStr, 0, -1);
                renderer->PopZbState();
                stats_0.numLightModelsRendered = 0;
                stats_0.numLightsToRender = 0;
                stats_0.numLightCellsRendered = 0;
                stats_0.curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
            }
            break;
        }
        case -3:
            overlayStop();
            renderer->PopCull();
            renderer->SetAlphaTest(0);
            renderer->PopLighting();
            renderer->PopBlend();
            renderer->PopFog();
            renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
            renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
            renderer->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
            renderer->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
            break;
        case -4:
            break;
        default:
        {
            float const VISCELL_EDGE_LENGTH = 128.0f;

            int const farDist = M3D_ENGINE_CFG.m_g_projectorsFarDist.GetI();
            renderer->SingleLayerStencilStart();

            auto const* const model = static_cast<PointLightModel const*>(m_models[id].m_ptr);
            CWorld& world = pClient->GetWorld();
            rend::IEffect* const lsShader = world.GetGraph().m_lsLightShader;
            rend::IEffect* const roadShader = world.GetGraph().m_roadLightShader;
            Landscape& ls = world.GetLandscape();
            RoadManager& roadManager = world.GetRoadManager();
            SceneGraph& graph = world.GetGraph();

            retruxx::vector<unsigned int> roadCells;
            retruxx::set<SgNode*> lightNodes;

            // The light's position is the translation row of its world matrix.
            auto const* const transform = static_cast<CMatrix const*>(params);
            CVector const pos(transform->_41, transform->_42, transform->_43);
            float const radius = static_cast<float>(model->m_radius);
            CVector const& color = model->m_color;

            lsShader->SetVector3(rend::IEffect::User_float4_param, pos);
            lsShader->SetFloat(rend::IEffect::User_float_param, radius);
            lsShader->SetVector3(rend::IEffect::User_float3_param, color);
            roadShader->SetVector3(rend::IEffect::User_float4_param, pos);
            roadShader->SetFloat(rend::IEffect::User_float_param, radius);
            roadShader->SetVector3(rend::IEffect::User_float3_param, color);

            float const fadeDist = (static_cast<float>(M3D_ENGINE_CFG.m_g_projectorsFarDist.GetI()) - 0.5f) * VISCELL_EDGE_LENGTH;
            lsShader->SetFloat(rend::IEffect::User_float_param2, fadeDist);
            roadShader->SetFloat(rend::IEffect::User_float_param2, fadeDist);
            graph.m_objectLightShader->SetFloat(rend::IEffect::User_float_param2, fadeDist);
            graph.m_treeLightShader->SetFloat(rend::IEffect::User_float_param2, fadeDist);

            // Every visible cell whose centre lies within the light radius (plus half a cell diagonal) is lit.
            graph.SortedCellsStartFetching(0, farDist);
            int x = 0;
            int z = 0;
            int vis = 0;
            int cellRadius = 0;
            while (graph.SortedCellsFetch(x, z, vis, cellRadius))
            {
                if (!vis)
                {
                    continue;
                }
                Landscape::CellParams const& cell = ls.m_cellParams[x + z * ls.m_owner->m_level->land_size];
                float const dx = pos.x - (static_cast<float>(x) + 0.5f) * VISCELL_EDGE_LENGTH;
                float const dy = pos.y - (cell.m_h1 + cell.m_h0) * 0.5f;
                float const dz = pos.z - (static_cast<float>(z) + 0.5f) * VISCELL_EDGE_LENGTH;
                // The shipped code squares and sums on the x87 stack, hence the double precision.
                double const dist = sqrt(static_cast<double>(dz) * dz + static_cast<double>(dy) * dy + static_cast<double>(dx) * dx);
                if (static_cast<double>(VISCELL_EDGE_LENGTH) * 0.70700002f + radius > dist)
                {
                    ls.drawCellOverlayedShader(x, z, lsShader);
                    roadCells.push_back(x + (z << 16));
                    graph.CollectNodesLight(lightNodes, x, z, pos, radius);
                    ++stats_0.numLightCellsRendered;
                }
            }

            renderer->PushCull(rend::M3DCULL_CCW);
            RoadInRadius3dTest const roadTest(pos, radius);
            roadManager.RenderRoads(roadCells, RRT_FOR_POINTLIGHT, &roadTest, false);
            renderer->SingleLayerStencilFinish();

            if (!lightNodes.empty())
            {
                retruxx::vector<SgNode*> nodesVector;
                for (SgNode* node : lightNodes)
                {
                    nodesVector.push_back(node);
                }

                RenderNodeInfo rni;
                rni.projOrg = pos;
                rni.rnt = RNT_FOR_POINTLIGHT;
                rni.isCullInverted = false;
                rni.isPrimaryRender = false;
                rni.isUseImpostors = true;

                graph.m_objectLightShader->SetFloat(rend::IEffect::User_float_param, radius);
                graph.m_treeLightShader->SetFloat(rend::IEffect::User_float_param, radius);
                graph.m_objectLightShader->SetVector3(rend::IEffect::User_float3_param, color);
                graph.m_treeLightShader->SetVector3(rend::IEffect::User_float3_param, color);
                renderer->TgSetTcSource(0, rend::TC_FROM_VERTEX, 0);
                renderer->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
                renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
                renderer->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestWorld.GetI());
                M3D_APP->m_serverAnimatedModels->RenderNodeSet(
                    nodesVector.empty() ? nullptr : nodesVector.data(), static_cast<unsigned int>(nodesVector.size()), rni);
                renderer->SetAlphaTest(0);
                renderer->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
                renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
            }

            renderer->PopCull();
            stats_0.numLightModelsRendered += static_cast<int>(lightNodes.size());
            ++stats_0.numLightsToRender;
            break;
        }
        }

        m_profiler->EndCountdown();
    }

    int LightsServer::RemoveItem(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LightsServer::GetItemProperty(int id, int prop, void* dest)
    {
        // TODO: check this
        if (prop != 8449)
            return m3d::DataServer::GetItemProperty(id, prop, dest);
        *(float*)dest = ((PointLightModel*)m_models[id].m_ptr)->m_radius;
        return 1;
    }

    LightsServer::LightsServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("point lights", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
    }

    int LightsServer::SetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void LightsServer::AddItemsList(retruxx::vector<ServerItem>& list)
    {
        auto size = list.size();
        for (int i = 0; i < size; ++i)
        {
            if (m_fnLoadCallback)
            {
                m_fnLoadCallback(i / size, m_fnLoadCallbackData);
            }
            if (AddItem(list[i].m_filename.c_str(), list[i].m_id.c_str()) == -1)
            {
                M3D_LOG_INFO("LightServer: cannot read " + list[i].m_filename);
            }
        }
    }
}  // namespace m3d

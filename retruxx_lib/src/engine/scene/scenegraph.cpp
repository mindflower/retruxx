#define NOMINMAX
#include <stdexcept>
#include <scene/scenegraph.h>
#include <algorithm>

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/timer.h"
#include "world.h"
#include "level.h"
#include "scene/nodes/sgnodeanimatedmodel.h"
#include "scene/nodes/sgnodedecals.h"
#include "scene/nodes/sgnodegameunit.h"
#include "scene/nodes/sgnodelines.h"
#include "scene/nodes/sgnodeparticles.h"
#include "scene/nodes/sgnodepointlightsource.h"
#include "scene/nodes/sgnodeprojector.h"
#include "scene/nodes/sgnodesound.h"
#include "scene/nodes/sgnodesprite.h"
#include "scene/nodes/sgnodestaticmodel.h"
#include "scene/servers/dataserver.h"


namespace
{
    float const VISCELL_EDGE_LENGTH_6 = 128.0;

    CVector camOrg;
    float transparentRadius = 0.0;
    bool inTransparencyRadius = false;
}

namespace m3d
{
    float IsNodeTransparent::getTransparentRadius()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool IsNodeTransparent::setPermanentTransparency(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool IsNodeTransparent::test(SgNode*, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::LinkNode(SgNode* toLink)
    {
        // TODO: generated code
        toLink->m_forGraph = new GraphItemsForSgNode;

        PointBase<int> p0;
        PointBase<int> p1;
        toLink->GetVisCellBounds(p0, p1);
        toLink->m_forGraph->m_cellsCoveredPoint0 = p0;
        toLink->m_forGraph->m_cellsCoveredPoint1 = p1;

        if (p0.x <= p1.x) {
            int y = p0.y;
            int v5 = p1.y;

            for (int x = p0.x; x <= p1.x; x++) {
                int j = y;
                if (y <= v5) {
                    do {
                        int cellIndex = x + (j << 6);
                        m3d::SceneGraph::CellItems& v7 = m_cellItems[cellIndex];
                        v7.m_nodesLinkedDirect.AddObject(toLink);
                        j++;
                    } while (j <= v5);
                }
            }
        }

        auto v13 = m_owner->m_level->land_size - 1;
        int modelCastShadow = 0;
        toLink->GetServerItemProperty(0, &modelCastShadow);
        if (modelCastShadow) {
            int v35 = 0;
            toLink->GetProperty(8721u, &v35);

            if (v35) {
                auto v15 = toLink->m_currentWorldOrigin.y;
                auto x = toLink->m_currentWorldOrigin.x;
                auto z = toLink->m_currentWorldOrigin.z;

                Aabb box = toLink->m_boundingBox;

                float v23 = (float)((float)(v15 + box.m_box[4]) - (float)(v15 + box.m_box[1])) * 1.3f;
                float v24 = v23 + (z + box.m_box[5]);

                int v25 = static_cast<int>((1.0f / VISCELL_EDGE_LENGTH_6) * ((z + box.m_box[2]) - v23));
                int v26 = static_cast<int>((1.0f / VISCELL_EDGE_LENGTH_6) * ((x + box.m_box[0]) - v23));
                int v27 = static_cast<int>((1.0f / VISCELL_EDGE_LENGTH_6) * (v23 + (x + box.m_box[3])));
                int v28 = static_cast<int>((1.0f / VISCELL_EDGE_LENGTH_6) * v24);

                int x0 = v26;
                int z0 = v25;
                int x1 = v27;
                int z1 = v28;

                // Clamp values to valid range
                x0 = std::clamp(v26, 0, v13);
                x1 = std::clamp(v27, 0, v13);
                z0 = std::clamp(v25, 0, v13);
                z1 = std::clamp(v28, 0, v13);

                for (int xCoord = x0; xCoord <= x1; xCoord++) {
                    for (int zCoord = z0; zCoord <= z1; zCoord++) {
                        int cellIndex = 64 * zCoord + xCoord;
                        m3d::SceneGraph::CellItems& cell = m_cellItems[cellIndex];

                        // Insert into shadowing set
                        auto result = cell.m_nodesShadowingDirect.insert(toLink);

                        // Store the cell identifier (combining x and z coordinates)
                        unsigned int cellId = (xCoord & 0xFFFF) | ((zCoord & 0xFFFF) << 16);
                        toLink->m_forGraph->m_cellsShadowCovered.push_back(cellId);
                    }
                }
            }
        }
    }

    void SceneGraph::SortedCellsPrepare()
    {
        m_cellsPrepared = true;
        memset(m_sortedCellsX, 0xFF, sizeof(m_sortedCellsX));
        auto const org = M3D_RENDERER->MatGetOrgInv();

        auto const curX = (1.0 / VISCELL_EDGE_LENGTH_6) * org.x;
        auto const curZ = (1.0 / VISCELL_EDGE_LENGTH_6) * org.z;
        int SortedCellsTops[256] = { 0 };
        const auto sds = 50176 / sizeof(m_cellItems[0]);
        //TODO: check this and refactor
        auto ls = m_owner->m_level->GetLandSize();
        auto x = 0;
        if (ls > 0)
        {
            auto v3 = curX;
            auto v10 = curX;
            auto cellItem = m_cellItems;
            while (true)
            {
                auto v4 = curZ;
                auto tempCellItem = cellItem;
                auto v6 = 0;
                auto v16 = v3 * v3;
                do
                {
                    tempCellItem->m_bVisibleInCurrentFrame = false;
                    auto v7 = static_cast<unsigned>(floor(sqrt((v4 * v4) + v16)));
                    if (v7 <= 0x64)
                    {
                        auto idx = SortedCellsTops[v7] + v7 * (6 * v7 + 2);
                        //TODO: check this!!!!
                        //SceneGraph* v9 = (char*)this + idx;
                        m_sortedCellsX[idx] = x;
                        m_sortedCellsY[idx] = v6;
                        SortedCellsTops[v7] = SortedCellsTops[v7] + 1;
                    }
                    ++v6;
                    tempCellItem += 64;
                    --v4;
                } while (v6 < ls);
                --v10;
                ++cellItem;
                if (++x >= ls)
                {
                    break;
                }
                v3 = v10;
            }
        }
      // throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::UnlinkAndDeleteAll()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::UnlinkThinkNode(SgNode* toThink)
    {
        m_thinkList.erase(toThink);
    }

    float SceneGraph::GetAlphaForNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::UpdateThinkNodes()
    {
        auto curTime = M3D_KERNEL->GetTimer().GetFrameStartTime();
        for (auto& think : m_thinkList)
        {
            auto nextThinkTime = think->m_nextThinkTime;
            if (nextThinkTime > 0 && nextThinkTime < curTime)
            {
                auto prevThinkTime = think->m_prevThinkTime;
                auto delta = curTime - prevThinkTime;
                if (delta == 1)
                {
                    delta = 100;
                }
                think->Think(delta, curTime);
                think->m_prevThinkTime = curTime;
            }
        }
    }

    void SceneGraph::DumpRenderingNodesInfoForClass(const Class*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNode* SceneGraph::GetRootNode()
    {
        return &m_rootNode;
    }

    SgNode const* SceneGraph::GetRootNode() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::RelinkNode(SgNode* toRelink, bool bForceRelink)
    {
        if (bForceRelink || toRelink->VisCellBoundsChanged())
        {
            m3d::SceneGraph::UnlinkNode(toRelink);
            m3d::SceneGraph::LinkNode(toRelink);
        }
    }

    void SceneGraph::CollectNodesLight(retruxx::set<SgNode*>&, unsigned, unsigned, CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::InsertInRemoveIfFree(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SceneGraph::SortedCellsStartFetching(int radius0, int radius1)
    {
        this->m_sortedCellsCurRadius = radius0;
        this->m_sortedCellsCurCell = 0;
        this->m_sortedCellsEndRadius = radius1;
        return this->m_cellsPrepared;
    }

    void SceneGraph::RefreshObjectsInRect(int, int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SceneGraph::SortedCellsFetch(int& cellX, int& cellY, int& vis, int& radius)
    {
        // TODO: generated code
        radius = m_sortedCellsCurRadius;
        int v5 = m_sortedCellsCurRadius * (6 * m_sortedCellsCurRadius + 2);

        if (m_sortedCellsX[v5 + m_sortedCellsCurCell] >= 0)
        {
            cellX = m_sortedCellsX[v5 + m_sortedCellsCurCell];
            cellY = m_sortedCellsY[v5 + m_sortedCellsCurCell];
            vis = (m_enableVisSpaceMask & m_enableMap[256 * (cellY) + cellX]) != 0;
            ++m_sortedCellsCurCell;
            return 1;
        }
        else
        {
            int endRadius = m_sortedCellsEndRadius;

            while (true)
            {
                ++m_sortedCellsCurRadius;
                m_sortedCellsCurCell = 0;

                if (m_sortedCellsCurRadius >= endRadius)
                    return 0;

                v5 = m_sortedCellsCurRadius * (6 * m_sortedCellsCurRadius + 2);

                if (m_sortedCellsX[v5 + m_sortedCellsCurCell] >= 0)
                {
                    cellX = m_sortedCellsX[v5 + m_sortedCellsCurCell];
                    cellY = m_sortedCellsY[v5 + m_sortedCellsCurCell];
                    vis = (m_enableVisSpaceMask & m_enableMap[256 * (cellY) + cellX]) != 0;
                    ++m_sortedCellsCurCell;
                    return 1;
                }
            }
        }
    }

    int SceneGraph::SortedCellsFetch(int&, int&, int&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::LightSwitchOffAllLights()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::UnlinkNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNode* SceneGraph::GetNodeByName(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SceneGraph::IsLinkedNode(SgNode* toCheck)
    {
        auto* forGraph = toCheck->m_forGraph;
        return forGraph
            && forGraph->m_cellsCoveredPoint0.x <= forGraph->m_cellsCoveredPoint1.x
            && forGraph->m_cellsCoveredPoint0.y <= forGraph->m_cellsCoveredPoint1.y;
    }

    bool SceneGraph::IsCellVisible(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::DeleteFromRemoveIfFree(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNode* SceneGraph::TraceLine(CVector&, CVector const&, CVector const&, retruxx::set<Class*> const&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::LightSetupLightsForNode(SgNode* node)
    {
        if (!node || !m_owner) return;

        // Transform the sun direction by the node's inverse transpose (for normal transformation)
        const CVector& sunDir = m_owner->GetSun(0.0);
        const CMatrix& transform = node->m_currentXForm;

        // Calculate the transformed light direction (applying the node's rotation)
        // This appears to be transforming the sun direction by the upper 3x3 of the matrix
        float transformedX = -(sunDir.x * transform._11 + sunDir.y * transform._12 + sunDir.z * transform._13);
        float transformedY = -(sunDir.x * transform._21 + sunDir.y * transform._22 + sunDir.z * transform._23);
        float transformedZ = -(sunDir.x * transform._31 + sunDir.y * transform._32 + sunDir.z * transform._33);

        // Setup the directional light source
        rend::LightSource light;
        light.m_type = rend::M3DLIGHT_DIRECTIONAL;
        light.m_direction.x = transformedX;
        light.m_direction.y = transformedY;
        light.m_direction.z = transformedZ;
        light.m_origin.x = transformedX;
        light.m_origin.y = transformedY;
        light.m_origin.z = transformedZ;
        light.m_range = 1000.0f;

        // Set diffuse color from weather system
        uint32_t weatherDiffuse = m_owner->GetWeatherDiffuseColor();
        light.m_diffuse = rend::Colorf(weatherDiffuse);

        // Set ambient color from weather system
        uint32_t weatherAmbient = m_owner->GetWeatherAmbientColor();
        light.m_ambient = rend::Colorf(weatherAmbient);

        // Apply the light to the renderer
        M3D_RENDERER->LightSet(0, light);
        M3D_RENDERER->LightEnable(0, true);
    }

    namespace
    {
        void CheckNodeValidity(m3d::SgNode *node, const char *debugStr)
        {
            throw retruxx::logic_error("Not implemented");
        }
    }

    void SceneGraph::Update()
    {
        const auto frameStartTime = M3D_KERNEL->GetTimer().GetFrameStartTime();
        UpdateAllXForms();
        auto ttlIt = m_ttledList.begin();
        while (ttlIt != m_ttledList.end())
        {
            auto* node = *ttlIt;
            if (node->m_ttl <= 0 || node->m_ttl >= frameStartTime)
            {
                ++ttlIt;
            }
            else
            {
                ttlIt = m_ttledList.erase(ttlIt);
                RemoveNodeExceptRemoveIfFree(node);
            }
        }

        // TODO: generated code

        // Process nodes marked for removal if free
        m_bIsPurgingRemoveIfFree = true;

        auto removeIt = m_RemoveIfFreeList.begin();
        while (removeIt != m_RemoveIfFreeList.end())
        {
            SgNode* currentNode = *removeIt;
            bool needToRemove = true;

            // Check if all children are free (eligible for removal)
            std::vector<SgNode*> stack;
            stack.push_back(currentNode);

            while (!stack.empty())
            {
                SgNode* current = stack.back();
                stack.pop_back();

                // Check all children of this node
                SgNode* child = (SgNode*)current->GetFirstChild();
                while (child != nullptr)
                {
                    if (!child->IsFree())
                    {
                        needToRemove = false;
                    }

                    if (child->GetFirstChild() != nullptr)
                    {
                        stack.push_back(child);
                    }

                    child = (SgNode*)child->GetNextSibling();
                }
            }

            if (needToRemove)
            {
                // Remove the node from various lists and clean up
                auto nextIt = m_RemoveIfFreeList.erase(removeIt);

                // If node is linked in the spatial partitioning, unlink it
                GraphItemsForSgNode* graphItems = currentNode->m_forGraph;
                if (graphItems &&
                    graphItems->m_cellsCoveredPoint0.x <= graphItems->m_cellsCoveredPoint1.x &&
                    graphItems->m_cellsCoveredPoint0.y <= graphItems->m_cellsCoveredPoint1.y)
                {
                    UnlinkNode(currentNode);
                }

                // Remove from parent if it has one
                if (currentNode->GetParent() != nullptr)
                {
                    currentNode->GetParent()->RemoveChild(currentNode);
                }

                // Remove from transformation update list
                DeleteFromUpdateXFormList(currentNode);

                // Validate node before deletion
                CheckNodeValidity(currentNode, "Check Two");

                // Delete the node
                delete currentNode;

                removeIt = nextIt;
            }
            else
            {
                // Move to next node in the list
                ++removeIt;
            }
        }

        m_bIsPurgingRemoveIfFree = false;
    }

    void SceneGraph::RenderDebugForNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::RenderContouredNodes()
    {
        // TODO: implement SceneGraph::RenderContouredNodes
        // throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::UpdateAllXForms()
    {
        for (auto& node : m_updateXFormList)
        {
            node->UpdateXForm(true, false);
        }
        m_updateXFormList.clear();
    }

    void SceneGraph::DeleteFromTtlList(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SceneGraph::IsCellEnabled(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::RenderNode(SgNode*, CMatrix const&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::Render(SgRenderFlags flags)
    {
        const auto frameStart = M3D_KERNEL->GetTimer().GetFrameStartTime();
        const auto lastFrameTime = M3D_KERNEL->GetTimer().GetLastFrameTime();
        if (flags == SGRF_LOW_DETAIL || flags < SGRF_SHADOWS)
        {
            retruxx::vector<int> effectiveClasses;
            if (flags == SGRF_LOW_DETAIL)
            {
                if (M3D_ENGINE_CFG.m_ssRender.GetB())
                {
                    effectiveClasses.push_back(m3d::SgStaticModelNode::m_classSgStaticModelNode.m_index);
                }

                if (M3D_ENGINE_CFG.m_dsRender.GetB())
                {
                    effectiveClasses.push_back(m3d::SgAnimatedModelNode::m_classSgAnimatedModelNode.m_index);
                    effectiveClasses.push_back(m3d::SgGameUnitNode::m_classSgGameUnitNode.m_index);
                }
            }
            else if (flags)
            {
                if (M3D_ENGINE_CFG.m_esRender.GetB())
                {
                    effectiveClasses.push_back(m3d::SgParticlesNode::m_classSgParticlesNode.m_index);
                    effectiveClasses.push_back(m3d::SgProjectorNode::m_classSgProjectorNode.m_index);
                    effectiveClasses.push_back(m3d::SgPointLightSourceNode::m_classSgPointLightSourceNode.m_index);
                }
                M3D_APP->GetAnimatedModelsServer().RenderTransparents(m_transparentNodes, m_numTransparentNodes);
            }
            else
            {
                if (M3D_ENGINE_CFG.m_ssRender.GetB())
                {
                    effectiveClasses.push_back(m3d::SgStaticModelNode::m_classSgStaticModelNode.m_index);
                }

                if (M3D_ENGINE_CFG.m_esRender.GetB())
                {
                    effectiveClasses.push_back(m3d::SgSpriteNode::m_classSgSpriteNode.m_index);
                    effectiveClasses.push_back(m3d::SgLinesNode::m_classSgLinesNode.m_index);
                    effectiveClasses.push_back(m3d::SgParticlesOpaqueNode::m_classSgParticlesOpaqueNode.m_index);
                }

                if (M3D_ENGINE_CFG.m_dsRender.GetB())
                {
                    effectiveClasses.push_back(m3d::SgGameUnitNode::m_classSgGameUnitNode.m_index);
                    effectiveClasses.push_back(m3d::SgAnimatedModelNode::m_classSgAnimatedModelNode.m_index);
                    effectiveClasses.push_back(m3d::SgDecalsNode::m_classSgDecalsNode.m_index);
                }

                if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
                {
                    effectiveClasses.push_back(m3d::SgSoundSourceNode::m_classSgSoundSourceNode.m_index);
                }
            }

            for (const auto clsIdx : effectiveClasses)
            {
                if (m_visNumSlots[clsIdx])
                {
                    auto effIdx = clsIdx * 2000;
                    if (auto* server = m_visSlots[effIdx]->GetServer())
                    {
                        if (server == &M3D_APP->GetAnimatedModelsServer() || server == &M3D_APP->GetParticlesServer())
                        {
                            RenderNodeInfo rni;
                            rni.rnt = RNT_SIMPLE;
                            rni.isCullInverted = flags == SGRF_LOW_DETAIL;
                            rni.isUseImpostors = true;
                            rni.isPrimaryRender = flags != SGRF_LOW_DETAIL;
                            // TODO: check this
                            server->RenderNodeSet(&m_visSlots[effIdx], m_visNumSlots[clsIdx], rni);
                        }
                        else
                        {
                            // TODO: check this
                            bool renderStart = false;
                            server->RenderItem(-2, 0);
                            renderStart = true;

                            for (int i = 0; i < m_visNumSlots[clsIdx]; i++)
                            {
                                if (renderStart)
                                {
                                    server->RenderItem(-3, 0);
                                }
                                server->RenderItem(-2, 0);
                                renderStart = true;
                                m_visSlots[effIdx]->Render(NRF_DEFAULT, nullptr, lastFrameTime, frameStart);
                            }
                            if (renderStart)
                            {
                                server->RenderItem(-3, 0);
                            }

                        }
                    }
                }
            }
        }
        else if (flags == SGRF_SHADOWS)
        {
            DrawShadows();
        }
    }

    void SceneGraph::GetNodeNamesHierarchy(SgNode*, retruxx::vector<CStr>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SceneGraph::SceneGraph()
    {
        this->m_noModelCull = 0;
        this->m_bIsInUnlinkAndDeleteAll = 0;
        this->m_bIsPurgingRemoveIfFree = 0;
        this->m_easyRelink = 0;
        this->m_roadProjectorShader = M3D_RENDERER->NewEffect( "data/shaders/roadProjector.fx", true);
        M3D_ASSERT(m_roadProjectorShader);
        this->m_roadProjectorShader->SetDefaultTechnique(true);
        this->m_lsProjectorShader = M3D_RENDERER->NewEffect("data/shaders/lsProjector.fx", true);
        M3D_ASSERT(m_lsProjectorShader);
        this->m_lsProjectorShader->SetDefaultTechnique(true);
        this->m_objProjectorShader = M3D_RENDERER->NewEffect("data/shaders/objectProjector.fx", true);
        M3D_ASSERT(m_objProjectorShader);
        this->m_objProjectorShader->SetDefaultTechnique(true);
        this->m_treeProjectorShader = M3D_RENDERER->NewEffect("data/shaders/treeProjector.fx", true);
        M3D_ASSERT(m_treeProjectorShader);
        this->m_treeProjectorShader->SetDefaultTechnique(true);
        this->m_lsLightShader = M3D_RENDERER->NewEffect("data/shaders/lsLight.fx", true);
        M3D_ASSERT(m_lsLightShader);
        this->m_lsLightShader->SetDefaultTechnique(true);
        this->m_roadLightShader = M3D_RENDERER->NewEffect("data/shaders/roadLight.fx", true);
        M3D_ASSERT(m_roadLightShader);
        this->m_roadLightShader->SetDefaultTechnique(true);
        this->m_objectLightShader = M3D_RENDERER->NewEffect("data/shaders/objectlight.fx", true);
        M3D_ASSERT(m_objectLightShader);
        this->m_objectLightShader->SetDefaultTechnique(true);
        this->m_treeLightShader = M3D_RENDERER->NewEffect("data/shaders/treeLight.fx", true);
        M3D_ASSERT(m_treeLightShader);
        this->m_treeLightShader->SetDefaultTechnique(true);
        this->m_roadSpriteShader = M3D_RENDERER->NewEffect("data/shaders/roadSprite.fx", true);
        M3D_ASSERT(m_roadSpriteShader);
        this->m_roadSpriteShader->SetDefaultTechnique(true);
        this->m_texShadow = M3D_RENDERER->AddDynamicTexture(
            "$TexShadow",
            g_Kernel->GetEngineCfg().m_lgtShadowTexSz.GetI(),
            g_Kernel->GetEngineCfg().m_lgtShadowTexSz.GetI(), 6);
        this->m_detTexShadow = M3D_RENDERER->AddDynamicTexture(
            "$DetTexShadow",
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(),
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(), 6);
        this->m_texBlurShadow = M3D_RENDERER->AddDynamicTexture(
            "$TexBlurShadow",
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(),
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(), 6);
        M3D_RENDERER->SetTextureParameter(
            this->m_texBlurShadow,
            rend::TM_TEX_FILTER, 5u);
        this->m_lsShadowShader = M3D_RENDERER->NewEffect("data/shaders/lsShadows.fx", true);
        M3D_ASSERT(m_lsShadowShader);
        this->m_lsShadowShader->SetDefaultTechnique(true);
        this->m_roadShadowShader = M3D_RENDERER->NewEffect("data/shaders/roadShadows.fx", true);
        M3D_ASSERT(m_roadShadowShader);
        this->m_roadShadowShader->SetDefaultTechnique(true);
        this->m_lsDetailShadowShader = M3D_RENDERER->NewEffect("data/shaders/lsDetailedShadows.fx", true);
        M3D_ASSERT(m_lsDetailShadowShader);
        this->m_lsDetailShadowShader->SetDefaultTechnique(true);
        this->m_roadDetailShadowShader = M3D_RENDERER->NewEffect("data/shaders/roadDetailedShadows.fx", true);
        M3D_ASSERT(m_roadDetailShadowShader);
        this->m_roadDetailShadowShader->SetDefaultTechnique(true);
        this->m_shadowShader = M3D_RENDERER->NewEffect("data/shaders/shadow.fx", true);
        M3D_ASSERT(m_shadowShader);
        this->m_shadowShader->SetDefaultTechnique(true);
        this->m_blurShadowShader = M3D_RENDERER->NewEffect("data/shaders/blurShadow.fx", true);
        M3D_ASSERT(m_blurShadowShader);
        this->m_blurShadowShader->SetDefaultTechnique(true);
        this->m_grassShadowVs = M3D_RENDERER->NewHlslShader("data/shaders/grassShadows.vs", "GrassVS", rend::IHlslShader::VS_1_1);
        M3D_ASSERT(m_grassShadowVs);
        this->m_grassShadowPs = M3D_RENDERER->NewHlslShader("data/shaders/grassShadows.ps", "GrassPS", rend::IHlslShader::PS_1_1);
        M3D_ASSERT(m_grassShadowPs);
        this->m_contourShader = M3D_RENDERER->NewEffect("data/shaders/contour.fx", true);
        M3D_ASSERT(m_contourShader);
        this->m_contourShader->SetDefaultTechnique(true);
        this->m_rootNode.m_isRootNode = 1;
        this->m_rootNode.UpdateXForm(false , true);
        memset(this->m_enableMap, 0, sizeof(this->m_enableMap));
        this->m_enableVisSpaceMask = 1;

        //TODO: add this fields
        this->m_visSlots = new SgNode*[128000];
        this->m_visNumSlots = new int[64];
        this->m_visSlotsUnderwater = new SgNode*[128000];
        this->m_visNumSlotsUnderwater = new int[64];
        this->m_transparentNodes = new SgNode*[500];
        this->m_numTransparentNodes = 0;
        this->m_transparencyTest = new IsNodeTransparent;

        memset(this->m_visNumSlots, 0, 64 * sizeof(int));
        memset(this->m_visSlots, 0, 128000 * sizeof(SgNode*));
        memset(this->m_visNumSlotsUnderwater, 0, 64 * sizeof(int));
        memset(this->m_visSlotsUnderwater, 0, 28000 * sizeof(SgNode*));
        memset(this->m_transparentNodes, 0, 500 * sizeof(SgNode*));
        this->m_cellsPrepared = 0;
    }

    void SceneGraph::UpdateVis(bool newFrame, const CClipper& frusta, bool primary)
    {
        if (newFrame)
        {
            SortedCellsPrepare();
            memset(m_enableMap, 0, sizeof(m_enableMap));
        }
        else
        {
            memset(this->m_visNumSlots, 0, 0x100u);
            m_numTransparentNodes = 0;
            camOrg = M3D_RENDERER->MatGetOrgInv();
            transparentRadius = m_transparencyTest->getTransparentRadius();

            const auto curFrame = M3D_KERNEL->GetTimer().GetCurFrame()
            ;
            float lsViewDistanceDivider = M3D_ENGINE_CFG.m_lsTransitionDevider.GetF();
            lsViewDistanceDivider = ((lsViewDistanceDivider * 8.0) + 4.0);
            if (lsViewDistanceDivider >= 4)
            {
                if (lsViewDistanceDivider > 12)
                    lsViewDistanceDivider = 12;
            }
            else
            {
                lsViewDistanceDivider = 4;
            }

            SortedCellsStartFetching(0, lsViewDistanceDivider);

            int x, y, vis, radius;
            while (SortedCellsFetch(x, y, vis, radius))
            {
                inTransparencyRadius = false;
                if (primary)
                {
                    inTransparencyRadius = (((VISCELL_EDGE_LENGTH_6 * 0.70700002)
                                             + transparentRadius)
                                             * ((VISCELL_EDGE_LENGTH_6 * 0.70700002)
                                                + transparentRadius)) > ((((((y + 0.5) * VISCELL_EDGE_LENGTH_6) - camOrg.z) * (((y + 0.5) * VISCELL_EDGE_LENGTH_6) - camOrg.z)) + ((camOrg.y - camOrg.y) * (camOrg.y - camOrg.y))) + ((((x + 0.5) * VISCELL_EDGE_LENGTH_6) - camOrg.x) * (((x + 0.5) * VISCELL_EDGE_LENGTH_6) - camOrg.x)));
                }

                auto idx = x + (y << 6);
                m_cellItems[idx].m_bVisibleInCurrentFrame = true;
                auto* objects = m_cellItems[idx].m_nodesLinkedDirect.GetObjects();
                for (int i = 0 ; i < 64; ++i)
                {
                    for (auto& obj : objects[i])
                    {
                        AddNodeAndItsChildrenToRender(RT_DYNCAST(obj, SgNode), frusta, curFrame);
                    }
                }
            }

            int v12 = 0;
            for (int i = 0; i < 64; ++i)
            {
                auto v14 = v12 + this->m_visNumSlots[i];
                for (int j = v12; j < v14; ++j)
                {
                    auto v16 = this->m_visSlots[j];
                    v16->m_isWaitingForRender = 0;
                }
                v12 += 2000;
            }
            for (int  k = 0; k < this->m_numTransparentNodes; ++k)
            {
                auto v18 = this->m_transparentNodes[k];
                v18->m_isWaitingForRender = 0;
            }
        }
    }

    void SceneGraph::DeleteFromContourList(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNode* SceneGraph::GetNodeByNamesHierarchy(retruxx::vector<CStr> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::InsertInUpdateXFormList(SgNode* toInsert)
    {
        m_updateXFormList.insert(toInsert);
    }

    void SceneGraph::SetVisMask(unsigned char vis)
    {
        this->m_enableVisSpaceMask = vis;
    }

    bool SceneGraph::IsInUnlinkAndDeleteAll() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::DeleteAllTtledNodes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::SetModelForceNoCull(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ObjectsContainer const& SceneGraph::GetCellObjs(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::CollectNodesProjector(retruxx::set<SgNode*>&, unsigned, unsigned, CClipper const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::EnableVisibleCells(CClipper& frusta, unsigned or)
    {
        float v5 = m_owner->m_level->GetLandSize() * VISCELL_EDGE_LENGTH_6;
        //TODO: check this
        float box[6];
        box[2] = 0.0;
        box[5] = v5;
        box[0] = 0.0;
        box[3] = v5;
        m_owner->GetLandscape().getMinMaxHeightForBox(box, 0.0);
        frusta.enableAll();
        enableVisibleCells_r(frusta, box, or );
        //throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::RemoveNode(SgNode*&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::DeleteAllRemoveIfFreeNodes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::DumpToFile(CStr const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::LightSetupSunForWorld()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::UpdateTexShadowSizes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::SetOwner(CWorld* world)
    {
        m_owner = world;
    }

    SceneGraph::~SceneGraph()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::SetTransparencyTest(IsNodeTransparent* t)
    {
        if (t)
        {
            delete m_transparencyTest;
            m_transparencyTest = t;
        }
    }

    void SceneGraph::InsertInContourList(SgNode*, unsigned, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::CheckNodeIsNotInAnyList(SgNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::InsertInTtlList(SgNode*, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::LinkThinkNode(SgNode* toThink)
    {
        m_thinkList.insert(toThink);
    }

    void SceneGraph::DeleteFromUpdateXFormList(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::GetCellsStatistic(retruxx::vector<CellInfo>*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SceneGraph::CellItems& SceneGraph::GetCellItems(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SceneGraph::CellItems const& SceneGraph::GetCellItems(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::CollectShadowingNodesStencil(retruxx::set<SgNode*>&, Class*, int, int, unsigned, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::CollectShadowingNodes(retruxx::set<SgNode*>&, Class*, int, int, int, unsigned, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SceneGraph::AddOneNodeToRender(SgNode* n, CClipper const& frusta, int curFrame)
    {
        auto cls = n->GetClass();
        if (this->m_visNumSlots[cls->m_index] >= 1999)
            return 0;

        if (IS_KIND_OF(n, SgSoundSourceNode))
        {
            int prop = 0;
            n->GetProperty(4354, &prop);
            if (!prop)
            {
                prop = 750;
            }

            auto origin = M3D_RENDERER->GetViewOrigin();
            auto v12 = origin.z - n->m_currentWorldOrigin.z;
            auto v13 = origin.y - n->m_currentWorldOrigin.y;
            if ((((v12 * v12) + (v13 * v13)) + ((origin.x - n->m_currentWorldOrigin.x) * (origin.x - n->m_currentWorldOrigin.x))) <= (prop * prop))
            {
                n->m_isWaitingForRender = true;
                *(&this->m_visSlots[2000 * cls->m_index] + this->m_visNumSlots[cls->m_index]++) = n;
                n->m_frameVisible = curFrame;
                return 1;
            }
            return 0;
        }

        auto& orgForSphere = n->m_originWorldAbsForSphere;
        auto na = n->m_boundingRadius;
        if (!frusta.testSphere(orgForSphere, na*2.0))
        {
            return 0;
        }

        n->m_frameVisible2 = curFrame;
        if (!frusta.testSphere(orgForSphere, na))
        {
            return 0;
        }

        if (IsTransparent(n))
        {
            n->m_frameTransparent = curFrame;
            n->m_frameVisible = curFrame;
            n->m_isWaitingForRender = 1;
            this->m_transparentNodes[this->m_numTransparentNodes++] = n;
            return 1;
        }

        n->m_isWaitingForRender = true;
        *(&this->m_visSlots[2000 * cls->m_index] + this->m_visNumSlots[cls->m_index]++) = n;
        n->m_frameVisible = curFrame;
        return 1;
    }

    void SceneGraph::RemoveNodeExceptRemoveIfFree(SgNode*&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SceneGraph::getYOfs(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::EnsureEverythingIsUnlinked() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::DrawShadows()
    {
        // TODO: implement SceneGraph::DrawShadow
        //throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::enableCellsSetRect(int* rc, unsigned orValue, unsigned andValue)
    {
        // TODO: check and refactor this
        auto v4 = rc[1];
        if (v4 < rc[3])
        {
            auto v5 = rc[2];
            auto v6 = &this->m_enableMap[256 * v4];
            do
            {
                for (int i = *rc; i < v5; v5 = rc[2])
                {
                    auto v8 = v6[i++] & andValue;
                    v6[i - 1] = orValue | v8;
                }
                ++v4;
                v6 += 256;
            } while (v4 < rc[3]);
        }
    }

    void SceneGraph::enableCellsSetRect(float* rc, unsigned v0, unsigned v1)
    {
        //TODO: check and refactor this
        int rrc[4] = {0};

        auto v4 = rc[3] * (float)(1.0 / VISCELL_EDGE_LENGTH_6);
        rrc[0] = (int)(float)(*rc * (float)(1.0 / VISCELL_EDGE_LENGTH_6));
        auto v5 = (int)v4;
        auto v6 = rc[2] * (float)(1.0 / VISCELL_EDGE_LENGTH_6);
        rrc[2] = v5;
        auto v7 = (int)v6;
        auto v8 = rc[5] * (float)(1.0 / VISCELL_EDGE_LENGTH_6);
        rrc[1] = v7;
        auto land_size = this->m_owner->m_level->land_size;
        rrc[3] = (int)v8;
        if (rrc[0] > land_size)
            rrc[0] = land_size;
        if (v7 > land_size)
            rrc[1] = land_size;
        if (v5 > land_size)
            rrc[2] = land_size;
        if ((int)v8 > land_size)
            rrc[3] = land_size;
        enableCellsSetRect(rrc, v0, v1);
    }

    SgNode* SceneGraph::TraceLineThruCellNodesForClass(float&, int, int, CVector const&, CVector const&, Class*, retruxx::set<SgNode*>&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SceneGraph::AddNodeAndItsChildrenToRender(SgNode* n, CClipper const& frusta, int curFrame)
    {
        if (n->m_isWaitingForRender)
            return 0;

        auto prop = AddOneNodeToRender(n, frusta, curFrame);
        if (!prop)
        {
            n->GetProperty(4355, &prop);
        }
        if (prop)
        {
            retruxx::vector<m3d::Object*> stack;
            stack.push_back(n);

            // Depth-first traversal
            while (!stack.empty())
            {
                // Pop the last node from stack
                m3d::Object* currentNode = stack.back();
                stack.pop_back();

                // Process all children of current node
                m3d::SgNode* child = dynamic_cast<m3d::SgNode*>(currentNode->GetFirstChild());

                // TODO: check this
                while (child != nullptr)
                {
                    // Process collision for this child node
                    AddOneNodeToRender(child, frusta, curFrame);

                    // If child has children of its own, push to stack for processing
                    if (child->GetFirstChild() != nullptr)
                    {
                        stack.push_back(child);
                    }

                    // Move to next sibling
                    child = dynamic_cast<m3d::SgNode*>(child->GetNextSibling());
                }
            }
        }
        return 1;
    }

    void SceneGraph::DrawStencilShadows()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SceneGraph::IsTransparent(SgNode* n)
    {
        if (n->m_transparencyType == TT_NONE)
            return 0;

        if (n->m_transparencyType == TT_PERMANENT)
            return this->m_transparencyTest->setPermanentTransparency(n);

        if (!inTransparencyRadius )
            return 0;

        auto na = (((n->m_originWorldAbsForSphere.z - camOrg.z)
                    * (n->m_originWorldAbsForSphere.z - camOrg.z))
                   + ((n->m_originWorldAbsForSphere.x - camOrg.x)
                      * (n->m_originWorldAbsForSphere.x - camOrg.x)))
            + ((n->m_originWorldAbsForSphere.y - camOrg.y)
               * (n->m_originWorldAbsForSphere.y - camOrg.y));

        if (na >= (transparentRadius * transparentRadius))
            return 0;

        auto v5 = sqrt(na) / transparentRadius;
        return this->m_transparencyTest->test(n, v5);
    }

    void SceneGraph::enableVisibleCells_r(CClipper& frusta, float* box, unsigned int orFlags)
    {
        // TODO: generated code
        const float VISCELL_EDGE_LENGTH = 6.0f;
        const float MAX_LAND_SIZE = static_cast<float>(m_owner->m_level->land_size) * VISCELL_EDGE_LENGTH;

        // Test bounding box against frustum
        CVector ofs(0, 0, 0);
        int testResult = frusta.testBBox(tbFullTest, box, ofs);

        // Handle fully outside frustum
        if (testResult == 0) {
            enableCellsSetRect(box, 0, ~orFlags);
            return;
        }

        // Handle fully inside frustum
        if (testResult == 2) {
            enableCellsSetRect(box, orFlags, 0xFFFFFFFF);
            return;
        }

        // Partially visible case
        unsigned int clipSave = frusta.m_enabled;
        ofs = CVector(0, 0, 0);
        frusta.enableUpdateFromBox(box, ofs);

        // Clamp box to maximum landscape size
        box[3] = std::min(box[3], MAX_LAND_SIZE);
        box[5] = std::min(box[5], MAX_LAND_SIZE);

        // Calculate box dimensions
        float width = box[3] - box[0];
        float depth = box[5] - box[2];

        // Check if we should split the box
        if (width > VISCELL_EDGE_LENGTH || depth > VISCELL_EDGE_LENGTH) {
            // Calculate number of subdivisions
            float xCells = std::ceil((width * 0.5f) / VISCELL_EDGE_LENGTH);
            float zCells = std::ceil((depth * 0.5f) / VISCELL_EDGE_LENGTH);

            // Calculate sub-box dimensions
            float subWidth = VISCELL_EDGE_LENGTH * xCells;
            float subDepth = VISCELL_EDGE_LENGTH * zCells;

            // Create four sub-boxes
            float newBoxes[4][6] = {
                // Bottom-left sub-box
                { box[0],           box[1], box[2],
                  box[0] + subWidth, box[4], box[2] + subDepth },

                  // Bottom-right sub-box
                  { box[0] + subWidth, box[1], box[2],
                    box[0] + subWidth * 2, box[4], box[2] + subDepth },

                    // Top-right sub-box
                    { box[0] + subWidth, box[1], box[2] + subDepth,
                      box[0] + subWidth * 2, box[4], box[2] + subDepth * 2 },

                      // Top-left sub-box
                      { box[0],           box[1], box[2] + subDepth,
                        box[0] + subWidth, box[4], box[2] + subDepth * 2 }
            };

            // Process each sub-box
            for (auto& newBox : newBoxes) {
                // Clamp to landscape boundaries
                newBox[3] = std::min(newBox[3], MAX_LAND_SIZE);
                newBox[5] = std::min(newBox[5], MAX_LAND_SIZE);

                // Get height range for this box
                m_owner->GetLandscape().getMinMaxHeightForBox(newBox, 0.0f);

                // Recurse into sub-box
                enableVisibleCells_r(frusta, newBox, orFlags);
            }
        }
        else {
            // Box is small enough, mark cells
            enableCellsSetRect(box, orFlags, 0xFFFFFFFF);
        }

        // Restore original frustum state
        frusta.m_enabled = clipSave;
    }

    void ObjectsContainer::AddObject(m3d::Object* obj)
    {
        auto clz = obj->GetClass();
        m_objectsByClassIdx[clz->m_index].push_back(obj);
    }

    void ObjectsContainer::RemoveObject(m3d::Object* obj)
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>>* ObjectsContainer::GetObjectsByClass(m3d::Class* cl)
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>>* ObjectsContainer::GetObjects() const
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>>* ObjectsContainer::GetObjects()
    {
        return m_objectsByClassIdx;
    }

    bool ObjectsContainer::empty() const
    {
        throw std::logic_error("Not implemented");
    }
}

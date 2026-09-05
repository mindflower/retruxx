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
#include "core/log.h"
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

#include "server/relationship.h"
#include "server/objects/physicbodies/physicbody.h"
#include "server/objects/vehicle.h"

namespace
{
    float const VISCELL_EDGE_LENGTH_6 = 128.0;

    CVector camOrg;
    float transparentRadius = 0.0;
    bool inTransparencyRadius = false;
}  // namespace

namespace
{
    void CheckNodeValidity(m3d::SgNode* node, char const* debugStr)
    {
        // TODO: implement CheckNodeValidity
        // RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::SgNode* GetNodeByNameNode(CStr const& name, m3d::SgNode* node)
    {
        // RVA 0x2356C0
        if (!node)
        {
            return nullptr;
        }
        if (name == node->GetName())
        {
            return node;
        }
        for (auto* child = dynamic_cast<m3d::SgNode*>(node->GetFirstChild()); child != nullptr;
             child = dynamic_cast<m3d::SgNode*>(child->GetNextSibling()))
        {
            if (auto* found = GetNodeByNameNode(name, child))
            {
                return found;
            }
        }
        return nullptr;
    }

    bool isSpheresTouches(CVector const& c0, float r0, CVector const& c1, float r1)
    {
        // RVA 0x36B9C0
        return r0 + r1 > (c1 - c0).length();
    }

    // Visits every game-unit / animated-model node linked directly in a cell and
    // each node beneath it. `visitRoot` returns false to skip a top-level node's
    // subtree. Shared by CollectNodesProjector and CollectNodesLight, which
    // differ only in how they range-test a node.
    template<typename VisitRoot, typename VisitChild>
    void ForEachCellNodeTree(m3d::ObjectsContainer& container, VisitRoot const& visitRoot, VisitChild const& visitChild)
    {
        m3d::Class* const classes[] = {
            &m3d::SgGameUnitNode::m_classSgGameUnitNode, &m3d::SgAnimatedModelNode::m_classSgAnimatedModelNode};

        for (m3d::Class* cls : classes)
        {
            auto* list = container.GetObjectsByClass(cls);
            if (!list)
            {
                continue;
            }
            for (m3d::Object* obj : *list)
            {
                auto* root = static_cast<m3d::SgNode*>(obj);
                if (!visitRoot(root))
                {
                    continue;
                }

                std::vector<m3d::Object*> stack;
                stack.push_back(root);
                while (!stack.empty())
                {
                    m3d::Object* current = stack.back();
                    stack.pop_back();

                    auto* child = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
                    while (child)
                    {
                        visitChild(child);
                        if (child->GetFirstChild())
                        {
                            stack.push_back(child);
                        }
                        child = dynamic_cast<m3d::SgNode*>(child->GetNextSibling());
                    }
                }
            }
        }
    }

    // Adds one node's mesh count and triangle count to the running totals.
    void AddNodesTris(unsigned int& trisStats, unsigned short& meshStats, m3d::SgNode* node)
    {
        // RVA 0x239440
        m3d::Configuration* cfg = nullptr;
        node->GetProperty(m3d::PROP_DM_CFG, &cfg);
        if (!cfg)
        {
            return;
        }
        meshStats += static_cast<unsigned short>(cfg->m_meshes.size());
        for (auto const* mesh : cfg->m_meshes)
        {
            trisStats += mesh->m_numDrawIndices / 3;
        }
    }

    // Applies AddNodesTris to `node` and every node below it.
    void AddNodesTrisRecursive(unsigned int& trisStats, unsigned short& meshStats, m3d::SgNode* node)
    {
        AddNodesTris(trisStats, meshStats, node);

        std::vector<m3d::Object*> stack;
        stack.push_back(node);
        while (!stack.empty())
        {
            m3d::Object* current = stack.back();
            stack.pop_back();

            auto* child = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
            while (child)
            {
                AddNodesTris(trisStats, meshStats, child);
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
                child = dynamic_cast<m3d::SgNode*>(child->GetNextSibling());
            }
        }
    }

    void DumpToFileNode(FILE* fOut, m3d::SgNode const* node)
    {
        // RVA 0x2355D0
        if (!node)
        {
            return;
        }
        fprintf(
            fOut,
            "%s pos(%s) rot(%s) \n",
            node->GetName(),
            CStr(node->GetOrigin()).c_str(),
            CStr(node->GetRotation()).c_str());
        for (auto const* child = dynamic_cast<m3d::SgNode const*>(node->GetFirstChild()); child != nullptr;
             child = dynamic_cast<m3d::SgNode const*>(child->GetNextSibling()))
        {
            DumpToFileNode(fOut, child);
        }
    }
}  // namespace

namespace m3d
{
    float IsNodeTransparent::getTransparentRadius()
    {
        return M3D_ENGINE_CFG.m_g_transparentRadius.GetF();
    }

    bool IsNodeTransparent::setPermanentTransparency(SgNode*)
    {
        return false;
    }

    bool IsNodeTransparent::test(SgNode*, float)
    {
        return true;
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

        if (p0.x <= p1.x)
        {
            int y = p0.y;
            int v5 = p1.y;

            for (int x = p0.x; x <= p1.x; x++)
            {
                int j = y;
                if (y <= v5)
                {
                    do
                    {
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
        if (modelCastShadow)
        {
            int v35 = 0;
            toLink->GetProperty(8721u, &v35);

            if (v35)
            {
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

                for (int xCoord = x0; xCoord <= x1; xCoord++)
                {
                    for (int zCoord = z0; zCoord <= z1; zCoord++)
                    {
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
        this->m_cellsPrepared = true;

        // Initialize sorted cells with invalid values (-1)
        memset(this->m_sortedCellsX, 0xFF, sizeof(this->m_sortedCellsX));
        memset(this->m_sortedCellsY, 0xFF, sizeof(this->m_sortedCellsY));

        // Get the origin position from the renderer
        CVector origin = Application::g_pApp->m_renderer->MatGetOrgInv();

        // Calculate current grid position based on origin
        float gridScale = 1.0f / VISCELL_EDGE_LENGTH_6;
        int currentX = static_cast<int>(gridScale * origin.x);
        int currentZ = static_cast<int>(gridScale * origin.z);

        // Initialize sorted cells tops (tracking how many cells are at each distance)
        int SortedCellsTops[256] = {0};

        int levelSize = this->m_owner->m_level->land_size;

        // Process each cell in the grid
        for (int x = 0; x < levelSize; ++x)
        {
            int relX = currentX - x;  // Calculate relative X position

            for (int z = 0; z < levelSize; ++z)
            {
                // Cells are addressed on a fixed 64-wide stride, not on land_size.
                this->m_cellItems[64 * z + x].m_bVisibleInCurrentFrame = false;

                int relZ = currentZ - z;  // Calculate relative Z position

                // Calculate distance from origin
                float distanceSquared = static_cast<float>(relX * relX + relZ * relZ);
                unsigned int distance = static_cast<unsigned int>(floor(sqrt(distanceSquared)));

                // Only process cells within reasonable distance (100 units)
                if (distance <= 100)
                {
                    // Get the current top index for this distance
                    int topIndex = SortedCellsTops[distance];

                    // Calculate the base index for this distance bucket
                    // Each distance bucket can hold up to (6 * distance + 2) cells
                    int baseIndex = distance * (6 * distance + 2);

                    // Store the cell coordinates in sorted arrays
                    this->m_sortedCellsX[baseIndex + topIndex] = static_cast<char>(x);
                    this->m_sortedCellsY[baseIndex + topIndex] = static_cast<char>(z);

                    // Update the top index for this distance
                    SortedCellsTops[distance] = topIndex + 1;
                }
            }
        }
    }

    void SceneGraph::UnlinkAndDeleteAll()
    {
        m_bIsInUnlinkAndDeleteAll = true;
        for (auto* node = dynamic_cast<m3d::SgNode*>(m_rootNode.GetFirstChild()); node != nullptr;
             node = dynamic_cast<m3d::SgNode*>(node->GetNextSibling()))
        {
            UnlinkNode(node);
        }

        m_rootNode.RemoveAllChildren();

        m_thinkList.clear();
        m_ttledList.clear();
        m_RemoveIfFreeList.clear();
        m_contourList.clear();
        m_updateXFormList.clear();

        m_bIsInUnlinkAndDeleteAll = false;
    }

    void SceneGraph::UnlinkThinkNode(SgNode* toThink)
    {
        m_thinkList.erase(toThink);
    }

    float SceneGraph::GetAlphaForNode(SgNode* node)
    {
        // TODO: generated code SceneGraph::GetAlphaForNode
        // Calculate alpha steps based on engine configuration
        int const alphaSteps = 256 / (M3D_ENGINE_CFG.m_alphaTestWorld.GetI() + 1);
        float const alphaStep = 1.0f / static_cast<float>(alphaSteps);

        float maxAlpha = 1.0f;
        float currentAlpha = 1.0f;

        // Get timer for TTL calculation
        auto const& timer = M3D_KERNEL->GetTimer();

        // Handle TTL (time to live) - fade in effect for newly spawned nodes
        if (node->m_ttl > 0)
        {
            int timeSinceSpawn = node->m_ttl - timer.GetFrameStartTime();
            if (timeSinceSpawn >= 0 && timeSinceSpawn < 1500)
            {
                maxAlpha = static_cast<float>(timeSinceSpawn) * 0.00066666666f;
            }
        }

        // Reduce alpha for small on-screen objects if model culling is enabled
        if (node->m_onScreenSize < 20.0f && !m_noModelCull)
        {
            currentAlpha = node->m_onScreenSize * 0.051282052f;
        }

        // Handle static model nodes with server-side transparency
        if (IS_KIND_OF(node, SgStaticModelNode))
        {
            float transparency = 0.0f;
            node->GetServerItemProperty(2, &transparency);

            if (transparency != 0.0f)
            {
                // Calculate distance from camera to node
                auto renderer = Application::g_pApp->m_renderer;
                CVector cameraPos = renderer->MatGetOrgInv();

                CVector delta = cameraPos - node->m_originWorldAbsForSphere;
                float distance = delta.length() - 64.0f - node->m_boundingRadius;

                if (distance < 0.0f)
                {
                    distance = 0.0f;
                }

                // Apply distance-based alpha fade
                if (distance <= 64.0f)
                {
                    float distanceFactor = distance * 0.015625f;  // distance / 64
                    currentAlpha = (maxAlpha - alphaStep) * (distanceFactor * distanceFactor) + alphaStep;
                }
            }
        }

        // Return the minimum of max alpha and current alpha
        return std::min(maxAlpha, currentAlpha);
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

    void SceneGraph::DumpRenderingNodesInfoForClass(Class const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgNode* SceneGraph::GetRootNode()
    {
        return &m_rootNode;
    }

    SgNode const* SceneGraph::GetRootNode() const
    {
        return &m_rootNode;
    }

    void SceneGraph::RelinkNode(SgNode* toRelink, bool bForceRelink)
    {
        if (bForceRelink || toRelink->VisCellBoundsChanged())
        {
            UnlinkNode(toRelink);
            LinkNode(toRelink);
        }
    }

    void SceneGraph::CollectNodesLight(
        retruxx::set<SgNode*>& nodes,
        unsigned x,
        unsigned z,
        CVector const& lightPos,
        float lightRadius)
    {
        unsigned const landSize = m_owner->m_level->land_size;
        if (x >= landSize || z >= landSize || (m_enableMap[256 * z + x] & m_enableVisSpaceMask) == 0)
        {
            return;
        }

        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        auto inRange = [&lightPos, lightRadius](SgNode* n)
        {
            return isSpheresTouches(n->m_originWorldAbsForSphere, n->m_boundingRadius, lightPos, lightRadius);
        };

        ForEachCellNodeTree(
            m_cellItems[64 * z + x].m_nodesLinkedDirect,
            [&](SgNode* n)
            {
                if (n->m_frameVisible != curFrame || !inRange(n))
                {
                    return false;
                }
                if (IS_KIND_OF(n, SgAnimatedModelNode) && n->m_frameTransparent != curFrame)
                {
                    nodes.insert(n);
                }
                return true;
            },
            [&](SgNode* n)
            {
                if (IS_KIND_OF(n, SgAnimatedModelNode) && n->m_frameVisible == curFrame && inRange(n) &&
                    n->m_frameTransparent != curFrame)
                {
                    nodes.insert(n);
                }
            });
    }

    void SceneGraph::InsertInRemoveIfFree(SgNode* toInsert)
    {
        // TODO: check this
        if (toInsert)
        {
            CheckNodeValidity(toInsert, "Check from InsertInRemoveIfFree");
            if (m_bIsPurgingRemoveIfFree)
            {
                M3D_LOG_WARN(
                    "Warning: inserting node in RemoveIfFree when it is being purged! node name = '" +
                    CStr(toInsert->GetName()) + "', class = '" + CStr(toInsert->GetClassNameA()));
            }

            toInsert->RemoveImmediateAfterParent(false);
            m_RemoveIfFreeList.insert(toInsert);
            toInsert->m_persistant = 0;
            toInsert->m_isInRemoveIfFree = 1;
            toInsert->m_isRemoveIfFree = 1;

            std::vector<m3d::Object*> stack;
            stack.push_back(toInsert);

            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process all children of current node
                m3d::SgNode* childNode = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
                while (childNode)
                {
                    childNode->m_isRemoveIfFree = true;

                    // If child has children, add to stack for processing
                    if (childNode->GetFirstChild())
                    {
                        stack.push_back(childNode);
                    }

                    childNode = dynamic_cast<m3d::SgNode*>(childNode->GetNextSibling());
                }
            }
        }
    }

    bool SceneGraph::SortedCellsStartFetching(int radius0, int radius1)
    {
        this->m_sortedCellsCurRadius = radius0;
        this->m_sortedCellsCurCell = 0;
        this->m_sortedCellsEndRadius = radius1;
        return this->m_cellsPrepared;
    }

    void SceneGraph::RefreshObjectsInRect(int cx0, int cy0, int cx1, int cy1)
    {
        if (cx1 < 0 || cy1 < 0)
        {
            return;
        }
        int const landSize = m_owner->m_level->land_size;
        if (cx0 >= landSize || cy0 >= landSize)
        {
            return;
        }

        int const maxIdx = landSize - 1;
        cx0 = std::clamp(cx0, 0, maxIdx);
        cx1 = std::min(cx1, maxIdx);
        cy0 = std::clamp(cy0, 0, maxIdx);
        cy1 = std::min(cy1, maxIdx);

        for (int z = cy0; z <= cy1; ++z)
        {
            for (int x = cx0; x <= cx1; ++x)
            {
                auto* lists = m_cellItems[64 * z + x].m_nodesLinkedDirect.GetObjects();
                for (int cls = 0; cls < 64; ++cls)
                {
                    for (m3d::Object* obj : lists[cls])
                    {
                        auto* node = static_cast<SgNode*>(obj);
                        node->m_isXFormDirty = 7;
                        node->UpdateXForm(false, true);
                    }
                }
            }
        }
    }

    int SceneGraph::SortedCellsFetch(int& cellX, int& cellY, int& vis, int& radius)
    {
        radius = this->m_sortedCellsCurRadius;
        auto v5 = this->m_sortedCellsCurRadius * (6 * this->m_sortedCellsCurRadius + 2);
        auto result = 1;
        if (this->m_sortedCellsX[v5 + this->m_sortedCellsCurCell] >= 0)
        {
            cellX = this->m_sortedCellsX[v5 + this->m_sortedCellsCurCell];
            auto v9 = this->m_sortedCellsY[v5 + this->m_sortedCellsCurCell];
            cellY = v9;
            vis = (this->m_enableVisSpaceMask & this->m_enableMap[256 * v9 + cellX]) != 0;
            ++this->m_sortedCellsCurCell;
        }
        else
        {
            auto sortedCellsEndRadius = this->m_sortedCellsEndRadius;
            while (1)
            {
                auto v8 = ++this->m_sortedCellsCurRadius;
                this->m_sortedCellsCurCell = 0;
                if (v8 >= sortedCellsEndRadius)
                    return 0;
                v5 = v8 * (6 * v8 + 2);
                if (this->m_sortedCellsX[v5 + this->m_sortedCellsCurCell] >= 0)
                {
                    cellX = this->m_sortedCellsX[v5 + this->m_sortedCellsCurCell];
                    auto v9 = this->m_sortedCellsY[v5 + this->m_sortedCellsCurCell];
                    cellY = v9;
                    vis = (this->m_enableVisSpaceMask & this->m_enableMap[256 * v9 + cellX]) != 0;
                    ++this->m_sortedCellsCurCell;
                    return 1;
                }
            }
        }
        return result;
    }

    int SceneGraph::SortedCellsFetch(int& cellX, int& cellY, int& vis)
    {
        int radius = 0;
        return SortedCellsFetch(cellX, cellY, vis, radius);
    }

    void SceneGraph::LightSwitchOffAllLights()
    {
        auto* renderer = Application::g_pApp->m_renderer;
        for (int i = 0; i < renderer->GetMaxLights(); ++i)
        {
            renderer->LightEnable(i, 0);
        }
    }

    void SceneGraph::UnlinkNode(SgNode* toUnlink)
    {
        // TODO: generated code
        if (!toUnlink || !toUnlink->m_forGraph)
        {
            return;
        }

        m3d::GraphItemsForSgNode* graphItems = toUnlink->m_forGraph;
        toUnlink->m_forGraph = nullptr;

        // Process all cells covered by this node
        for (int x = graphItems->m_cellsCoveredPoint0.x; x <= graphItems->m_cellsCoveredPoint1.x; ++x)
        {
            for (int y = graphItems->m_cellsCoveredPoint0.y; y <= graphItems->m_cellsCoveredPoint1.y; ++y)
            {
                CellItems& cellItems = m_cellItems[64 * y + x];

                // Remove node from direct links in this cell
                cellItems.m_nodesLinkedDirect.RemoveObject(toUnlink);

                // Determine if this is the last cell (for cleanup purposes)
                bool isLastCell = (x == graphItems->m_cellsCoveredPoint1.x && y == graphItems->m_cellsCoveredPoint1.y);

                // Unlink collision geometries from landscape
                m_owner->GetLandscape().UnlinkNodeCollisionGeomsFromCell(toUnlink, x, y, isLastCell);

                // Process child nodes recursively using a stack
                std::vector<m3d::Object*> stack;
                stack.push_back(toUnlink);

                while (!stack.empty())
                {
                    m3d::Object* current = stack.back();
                    stack.pop_back();

                    // Process all children of current node
                    m3d::SgNode* childNode = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
                    while (childNode)
                    {
                        // Unlink child's collision geometries
                        m_owner->GetLandscape().UnlinkNodeCollisionGeomsFromCell(childNode, x, y, isLastCell);

                        // If child has children, add to stack for processing
                        if (childNode->GetFirstChild())
                        {
                            stack.push_back(childNode);
                        }

                        childNode = dynamic_cast<m3d::SgNode*>(childNode->GetNextSibling());
                    }
                }
            }
        }

        // Remove from shadow coverage
        for (auto it = graphItems->m_cellsShadowCovered.begin(); it != graphItems->m_cellsShadowCovered.end(); ++it)
        {
            uint32_t cellKey = *it;
            int cellX = cellKey & 0xFFFF;
            int cellY = (cellKey >> 16) & 0xFFFF;

            m3d::SceneGraph::CellItems& cellItems = m_cellItems[64 * cellY + cellX];

            // Remove node from shadowing direct set
            auto shadowIt = cellItems.m_nodesShadowingDirect.find(toUnlink);
            if (shadowIt != cellItems.m_nodesShadowingDirect.end())
            {
                cellItems.m_nodesShadowingDirect.erase(shadowIt);
            }
        }

        // Clean up graph items
        graphItems->m_cellsCoveredPoint0 = {0, 0};
        graphItems->m_cellsCoveredPoint1 = {-1, -1};

        graphItems->m_cellsShadowCovered.clear();

        // Free the graph items memory
        delete graphItems;
    }

    SgNode* SceneGraph::GetNodeByName(CStr const& name)
    {
        return GetNodeByNameNode(name, &m_rootNode);
    }

    bool SceneGraph::IsLinkedNode(SgNode* toCheck)
    {
        auto* forGraph = toCheck->m_forGraph;
        return forGraph && forGraph->m_cellsCoveredPoint0.x <= forGraph->m_cellsCoveredPoint1.x &&
            forGraph->m_cellsCoveredPoint0.y <= forGraph->m_cellsCoveredPoint1.y;
    }

    bool SceneGraph::IsCellVisible(int x, int z) const
    {
        return m_cellItems[64 * z + x].m_bVisibleInCurrentFrame;
    }
    m3d::rend::IEffect* SceneGraph::GetRoadProjectorShader()
    {
        return m_roadProjectorShader;
    }
    m3d::rend::IEffect* SceneGraph::GetLsProjectorShader()
    {
        return m_lsProjectorShader;
    }
    m3d::rend::IEffect* SceneGraph::GetObjProjectorShader(m3d::rend::IEffect* objShader)
    {
        if (objShader && objShader->GetTechniqueDesc(0).name == "TreeTech")
        {
            return m_treeProjectorShader;
        }
        return m_objProjectorShader;
    }
    m3d::rend::IEffect* SceneGraph::GetObjProjectorShader()
    {
        return m_objProjectorShader;
    }
    m3d::rend::IEffect* SceneGraph::GetTreeProjectorShader()
    {
        return m_treeProjectorShader;
    }
    m3d::rend::IEffect* SceneGraph::GetLsLightShader()
    {
        return m_lsLightShader;
    }
    m3d::rend::IEffect* SceneGraph::GetRoadLightShader()
    {
        return m_roadLightShader;
    }
    m3d::rend::IEffect* SceneGraph::GetObjectLightShader(m3d::rend::IEffect* objShader)
    {
        if (objShader && objShader->GetTechniqueDesc(0).name == "TreeTech")
        {
            return m_treeLightShader;
        }
        return m_objectLightShader;
    }
    m3d::rend::IEffect* SceneGraph::GetObjectLightShader()
    {
        return m_objectLightShader;
    }
    m3d::rend::IEffect* SceneGraph::GetTreeLightShader()
    {
        return m_treeLightShader;
    }
    m3d::rend::IEffect* SceneGraph::GetRoadSpriteShader()
    {
        return m_roadSpriteShader;
    }
    m3d::rend::IEffect* SceneGraph::GetShadowShader()
    {
        return m_shadowShader;
    }
    m3d::rend::IEffect* SceneGraph::GetRoadShadowShader()
    {
        return m_roadShadowShader;
    }
    m3d::rend::IEffect* SceneGraph::GetLsDetShadowShader()
    {
        // NOTE: the shipped build never emits this accessor (every caller
        // inlines it), so the member is inferred from the naming symmetry with
        // GetRoadDetShadowShader / m_roadDetailShadowShader.
        return m_lsDetailShadowShader;
    }
    m3d::rend::IEffect* SceneGraph::GetRoadDetShadowShader()
    {
        return m_roadDetailShadowShader;
    }
    m3d::rend::IEffect* SceneGraph::GetContourShader()
    {
        return m_contourShader;
    }

    void SceneGraph::DeleteFromRemoveIfFree(SgNode* toDelete)
    {
        m_RemoveIfFreeList.erase(toDelete);
        toDelete->m_isInRemoveIfFree = 0;
        toDelete->m_isRemoveIfFree = 0;

        std::vector<m3d::Object*> stack;
        stack.push_back(toDelete);

        while (!stack.empty())
        {
            m3d::Object* current = stack.back();
            stack.pop_back();

            auto* childNode = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
            while (childNode)
            {
                childNode->m_isRemoveIfFree = 0;
                if (childNode->GetFirstChild())
                {
                    stack.push_back(childNode);
                }
                childNode = dynamic_cast<m3d::SgNode*>(childNode->GetNextSibling());
            }
        }
    }

    SgNode* SceneGraph::TraceLine(CVector&, CVector const&, CVector const&, retruxx::set<Class*> const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SceneGraph::LightSetupLightsForNode(SgNode* node)
    {
        // TODO: generated code SceneGraph::LightSetupLightsForNode
        if (!node || !m_owner)
            return;

        // Transform the sun direction by the node's inverse transpose (for normal transformation)
        CVector const& sunDir = m_owner->GetSun(0.0);
        CMatrix const& transform = node->m_currentXForm;

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

    void SceneGraph::Update()
    {
        auto const frameStartTime = M3D_KERNEL->GetTimer().GetFrameStartTime();
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
                if (graphItems && graphItems->m_cellsCoveredPoint0.x <= graphItems->m_cellsCoveredPoint1.x &&
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

                // TODO: DecRef
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SceneGraph::RenderContouredNodes()
    {
        if (m_contourList.empty())
        {
            return;
        }

        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        DataServer& serverAnimatedModels = M3D_APP->GetAnimatedModelsServer();

        std::vector<SgNode*> nodesToRender;
        for (SgNode* node : m_contourList)
        {
            if (node->m_frameVisible == curFrame && node->GetServer() == &serverAnimatedModels)
            {
                nodesToRender.push_back(node);
            }
        }

        if (nodesToRender.empty())
        {
            return;
        }

        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
        M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
        M3D_RENDERER->SetAlphaTest(0);
        M3D_RENDERER->PushLighting(false);
        M3D_RENDERER->PushBlend(rend::BM_0_1);
        M3D_RENDERER->PushFog(false);

        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);

        M3D_RENDERER->SingleLayerStencilStart();
        {
            RenderNodeInfo rni;
            rni.rnt = RNT_FOR_SHADOW;
            rni.isUseImpostors = true;
            serverAnimatedModels.RenderNodeSet(&nodesToRender.front(), nodesToRender.size(), rni);
        }

        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopLighting();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopFog();

        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);

        M3D_RENDERER->PushZbState(rend::ZB_ENABLE);
        M3D_RENDERER->PushCull(rend::Cull::M3DCULL_NONE);
        M3D_RENDERER->SetAlphaTest(0);
        M3D_RENDERER->PushLighting(false);
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->PushFog(false);

        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_TFACTOR);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TFACTOR);
        M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);

        {
            RenderNodeInfo rni;
            rni.rnt = RNT_FOR_CONTOUR;
            rni.isUseImpostors = true;
            serverAnimatedModels.RenderNodeSet(&nodesToRender.front(), nodesToRender.size(), rni);
        }

        M3D_RENDERER->SingleLayerStencilFinish();

        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopLighting();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopFog();

        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
    }

    void SceneGraph::UpdateAllXForms()
    {
        for (auto& node : m_updateXFormList)
        {
            node->UpdateXForm(true, false);
        }
        m_updateXFormList.clear();
    }

    void SceneGraph::DeleteFromTtlList(SgNode* toDelete)
    {
        m_ttledList.erase(toDelete);
    }

    int SceneGraph::IsCellEnabled(int x, int y)
    {
        return (this->m_enableVisSpaceMask & this->m_enableMap[256 * y + x]) != 0;
    }

    void SceneGraph::RenderNode(SgNode* n, CMatrix const& curMatr, bool fullInv)
    {
        DataServer* server = n->GetServer();
        if (!server)
        {
            return;
        }

        CMatrix vv;
        if (fullInv)
        {
            CMatrix invMat = n->m_currentXForm.getInverse();
            vv = invMat * curMatr;
        }
        else
        {
            CVector x, y, z;
            n->m_currentXForm.GetInvBasis(x, y, z);

            CMatrix invMat;
            invMat.identity();
            invMat._11 = x.x;
            invMat._12 = x.y;
            invMat._13 = x.z;
            invMat._21 = y.x;
            invMat._22 = y.y;
            invMat._23 = y.z;
            invMat._31 = z.x;
            invMat._32 = z.y;
            invMat._33 = z.z;

            CVector pos = n->m_currentXForm.getOrg();
            invMat._41 = -(pos.x * x.x + pos.y * y.x + pos.z * z.x);
            invMat._42 = -(pos.x * x.y + pos.y * y.y + pos.z * z.y);
            invMat._43 = -(pos.x * x.z + pos.y * y.z + pos.z * z.z);
            invMat._44 = 1.0f;

            vv = invMat * curMatr;
        }

        M3D_RENDERER->MatPush(vv);

        if (IS_KIND_OF(n, SgAnimatedModelNode) || IS_KIND_OF(n, SgParticlesNode))
        {
            SgNode* nodes[1] = {n};
            RenderNodeInfo rni;
            rni.isUseImpostors = true;
            server->RenderNodeSet(nodes, 1, rni);
        }
        else
        {
            server->RenderItem(-2, nullptr);

            uint32_t curTime = M3D_KERNEL->GetTimer().GetCurTime();

            n->Render(NRF_DEFAULT, nullptr, 10, curTime);

            server->RenderItem(-3, nullptr);
        }

        M3D_RENDERER->MatPop(false);
    }

    void SceneGraph::Render(SgRenderFlags flags)
    {
        auto const frameStart = M3D_KERNEL->GetTimer().GetFrameStartTime();
        auto const lastFrameTime = M3D_KERNEL->GetTimer().GetLastFrameTime();
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

            for (auto const clsIdx : effectiveClasses)
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

                            auto slots = &m_visSlots[effIdx];

                            for (int i = 0; i < m_visNumSlots[clsIdx]; i++)
                            {
                                if (renderStart)
                                {
                                    server->RenderItem(-3, 0);
                                }
                                server->RenderItem(-2, 0);
                                renderStart = true;
                                slots[i]->Render(NRF_DEFAULT, nullptr, lastFrameTime, frameStart);
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

    void SceneGraph::GetNodeNamesHierarchy(SgNode* node, retruxx::vector<CStr>& hierarchy)
    {
        hierarchy.clear();
        for (m3d::Object* cur = node; cur && cur != &m_rootNode; cur = cur->GetParent())
        {
            hierarchy.insert(hierarchy.begin(), CStr(cur->GetName()));
        }
    }

    SceneGraph::SceneGraph()
    {
        this->m_noModelCull = 0;
        this->m_bIsInUnlinkAndDeleteAll = 0;
        this->m_bIsPurgingRemoveIfFree = 0;
        this->m_easyRelink = 0;
        this->m_roadProjectorShader = M3D_RENDERER->NewEffect("data/shaders/roadProjector.fx", true);
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
            g_Kernel->GetEngineCfg().m_lgtShadowTexSz.GetI(),
            6);
        this->m_detTexShadow = M3D_RENDERER->AddDynamicTexture(
            "$DetTexShadow",
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(),
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(),
            6);
        this->m_texBlurShadow = M3D_RENDERER->AddDynamicTexture(
            "$TexBlurShadow",
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(),
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(),
            6);
        M3D_RENDERER->SetTextureParameter(this->m_texBlurShadow, rend::TM_TEX_FILTER, 5u);
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
        this->m_grassShadowVs =
            M3D_RENDERER->NewHlslShader("data/shaders/grassShadows.vs", "GrassVS", rend::IHlslShader::VS_1_1);
        M3D_ASSERT(m_grassShadowVs);
        this->m_grassShadowPs =
            M3D_RENDERER->NewHlslShader("data/shaders/grassShadows.ps", "GrassPS", rend::IHlslShader::PS_1_1);
        M3D_ASSERT(m_grassShadowPs);
        this->m_contourShader = M3D_RENDERER->NewEffect("data/shaders/contour.fx", true);
        M3D_ASSERT(m_contourShader);
        this->m_contourShader->SetDefaultTechnique(true);
        this->m_rootNode.m_isRootNode = 1;
        this->m_rootNode.UpdateXForm(false, true);
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

    void SceneGraph::UpdateVis(bool newFrame, CClipper const& frusta, bool primary)
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

            auto const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
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
                    inTransparencyRadius = (((VISCELL_EDGE_LENGTH_6 * 0.70700002) + transparentRadius) *
                                            ((VISCELL_EDGE_LENGTH_6 * 0.70700002) + transparentRadius)) >
                        ((((((y + 0.5) * VISCELL_EDGE_LENGTH_6) - camOrg.z) *
                           (((y + 0.5) * VISCELL_EDGE_LENGTH_6) - camOrg.z)) +
                          ((camOrg.y - camOrg.y) * (camOrg.y - camOrg.y))) +
                         ((((x + 0.5) * VISCELL_EDGE_LENGTH_6) - camOrg.x) *
                          (((x + 0.5) * VISCELL_EDGE_LENGTH_6) - camOrg.x)));
                }

                auto idx = x + (y << 6);
                m_cellItems[idx].m_bVisibleInCurrentFrame = true;
                auto* objects = m_cellItems[idx].m_nodesLinkedDirect.GetObjects();
                for (int i = 0; i < 64; ++i)
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
            for (int k = 0; k < this->m_numTransparentNodes; ++k)
            {
                auto v18 = this->m_transparentNodes[k];
                v18->m_isWaitingForRender = 0;
            }
        }
    }

    void SceneGraph::DeleteFromContourList(SgNode* toDelete)
    {
        if (toDelete)
        {
            m_contourList.erase(toDelete);
            toDelete->m_isContoured = false;
        }
    }

    SgNode* SceneGraph::GetNodeByNamesHierarchy(retruxx::vector<CStr> const& hierarchy)
    {
        SgNode* node = &m_rootNode;
        for (size_t i = 0; i < hierarchy.size() && node; ++i)
        {
            auto* child = dynamic_cast<SgNode*>(node->GetFirstChild());
            while (child && !(CStr(child->GetName()) == hierarchy[i]))
            {
                child = dynamic_cast<SgNode*>(child->GetNextSibling());
            }
            node = child;
        }
        return node;
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
        return m_bIsInUnlinkAndDeleteAll;
    }

    void SceneGraph::DeleteAllTtledNodes()
    {
        // TODO: check this
        for (auto& ttl : m_ttledList)
        {
            if (ttl)
            {
                m_RemoveIfFreeList.erase(ttl);
                if (IsLinkedNode(ttl))
                {
                    UnlinkNode(ttl);
                }
                if (auto* parent = ttl->GetParent())
                {
                    parent->RemoveChild(ttl);
                }
                DeleteFromUpdateXFormList(ttl);
                CheckNodeValidity(ttl, "Check Two");
                delete ttl;
            }
        }

        m_ttledList.clear();
    }

    void SceneGraph::SetModelForceNoCull(bool t)
    {
        m_noModelCull = t;
    }

    ObjectsContainer const& SceneGraph::GetCellObjs(int x, int z) const
    {
        return m_cellItems[64 * z + x].m_nodesLinkedDirect;
    }

    void SceneGraph::CollectNodesProjector(
        retruxx::set<SgNode*>& nodes,
        unsigned x,
        unsigned z,
        CClipper const& projectorFrusta)
    {
        unsigned const landSize = m_owner->m_level->land_size;
        if (x >= landSize || z >= landSize || (m_enableMap[256 * z + x] & m_enableVisSpaceMask) == 0)
        {
            return;
        }

        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        auto inRange = [&projectorFrusta](SgNode* n)
        {
            return projectorFrusta.testSphere(n->m_originWorldAbsForSphere, n->m_boundingRadius) != 0;
        };

        ForEachCellNodeTree(
            m_cellItems[64 * z + x].m_nodesLinkedDirect,
            [&](SgNode* n)
            {
                if (n->m_frameVisible != curFrame || !inRange(n))
                {
                    return false;
                }
                if (IS_KIND_OF(n, SgAnimatedModelNode) && n->m_frameTransparent != curFrame)
                {
                    nodes.insert(n);
                }
                return true;
            },
            [&](SgNode* n)
            {
                if (IS_KIND_OF(n, SgAnimatedModelNode) && n->m_frameVisible == curFrame && inRange(n) &&
                    n->m_frameTransparent != curFrame)
                {
                    nodes.insert(n);
                }
            });
    }

    void SceneGraph::EnableVisibleCells(CClipper& frusta, unsigned or)
    {
        float v5 = (float)m_owner->m_level->land_size * VISCELL_EDGE_LENGTH_6;
        //TODO: check this
        float box[6] = {0};
        box[2] = 0.0;
        box[5] = v5;
        box[0] = 0.0;
        box[3] = v5;
        m_owner->GetLandscape().getMinMaxHeightForBox(box, 0.0);
        frusta.enableAll();
        enableVisibleCells_r(frusta, box, or);
        //RETRUXX_NOT_IMPLEMENTED;
    }

    void SceneGraph::RemoveNode(SgNode*& toRemove)
    {
        if (toRemove)
        {
            m_RemoveIfFreeList.erase(toRemove);
            RemoveNodeExceptRemoveIfFree(toRemove);
        }
    }

    void SceneGraph::DeleteAllRemoveIfFreeNodes()
    {
        for (auto* node : m_RemoveIfFreeList)
        {
            RemoveNodeExceptRemoveIfFree(node);
        }
        m_RemoveIfFreeList.clear();
    }

    void SceneGraph::DumpToFile(CStr const& filename) const
    {
        FILE* fOut = fopen(filename.c_str(), "w");
        if (fOut)
        {
            DumpToFileNode(fOut, &m_rootNode);
            fclose(fOut);
        }
    }

    void SceneGraph::LightSetupSunForWorld()
    {
        m3d::rend::LightSource ls;
        ls.m_type = rend::M3DLIGHT_DIRECTIONAL;
        ls.m_direction.x = 0.0 - m_owner->GetSun(0.0).x;
        ls.m_direction.y = 0.0 - m_owner->GetSun(0.0).y;
        ls.m_direction.z = 0.0 - m_owner->GetSun(0.0).z;
        ls.m_origin = ls.m_direction;
        ls.m_range = 1000.0;
        ls.m_diffuse = m_owner->GetWeatherDiffuseColor();
        ls.m_ambient = m_owner->GetWeatherAmbientColor();

        M3D_RENDERER->LightSet(0, ls);
        M3D_RENDERER->LightEnable(0, 1);
    }

    void SceneGraph::UpdateTexShadowSizes()
    {
        auto* renderer = Application::g_pApp->m_renderer;

        int oldSzX = 0;
        int oldSzY = 0;
        renderer->GetDims(m_texShadow, oldSzX, oldSzY);
        int const lgtSz = M3D_ENGINE_CFG.m_lgtShadowTexSz.GetI();
        if (lgtSz != oldSzX)
        {
            renderer->ReleaseTexture(m_texShadow);
            m_texShadow = renderer->AddDynamicTexture("$TexShadow", lgtSz, lgtSz, 6);
        }

        renderer->GetDims(m_detTexShadow, oldSzX, oldSzY);
        int const detSz = M3D_ENGINE_CFG.m_detShadowTexSz.GetI();
        if (detSz != oldSzX)
        {
            renderer->ReleaseTexture(m_detTexShadow);
            m_detTexShadow = renderer->AddDynamicTexture("$DetTexShadow", detSz, detSz, 6);
            renderer->ReleaseTexture(m_texBlurShadow);
            m_texBlurShadow = renderer->AddDynamicTexture("$TexBlurShadow", detSz, detSz, 6);
        }
    }

    void SceneGraph::SetOwner(CWorld* world)
    {
        m_owner = world;
    }

    SceneGraph::~SceneGraph()
    {
        UnlinkAndDeleteAll();

        delete[] m_visSlots;
        m_visSlots = nullptr;
        delete[] m_visNumSlots;
        m_visNumSlots = nullptr;
        delete[] m_visSlotsUnderwater;
        m_visSlotsUnderwater = nullptr;
        delete[] m_visNumSlotsUnderwater;
        m_visNumSlotsUnderwater = nullptr;
        delete[] m_transparentNodes;
        m_transparentNodes = nullptr;

        auto release = [](auto*& resource)
        {
            if (resource)
            {
                resource->Release();
                resource = nullptr;
            }
        };

        release(m_lsProjectorShader);
        release(m_roadProjectorShader);
        release(m_objProjectorShader);
        release(m_treeProjectorShader);
        release(m_lsLightShader);
        release(m_roadLightShader);
        release(m_objectLightShader);
        release(m_treeLightShader);
        release(m_roadSpriteShader);

        auto* renderer = Application::g_pApp->m_renderer;
        renderer->ReleaseTexture(m_texShadow);
        renderer->ReleaseTexture(m_detTexShadow);
        renderer->ReleaseTexture(m_texBlurShadow);
        for (auto& tex : m_texShadows)
        {
            if (tex.IsValid())
            {
                renderer->ReleaseTexture(tex);
            }
        }

        release(m_lsShadowShader);
        release(m_blurShadowShader);
        release(m_roadShadowShader);
        release(m_lsDetailShadowShader);
        release(m_roadDetailShadowShader);
        release(m_shadowShader);
        release(m_grassShadowVs);
        release(m_grassShadowPs);
        release(m_contourShader);

        delete m_transparencyTest;
        m_transparencyTest = nullptr;
    }

    void SceneGraph::SetTransparencyTest(IsNodeTransparent* t)
    {
        if (t)
        {
            delete m_transparencyTest;
            m_transparencyTest = t;
        }
    }

    void SceneGraph::InsertInContourList(SgNode* toInsert, unsigned color, float width)
    {
        if (toInsert)
        {
            m_contourList.insert(toInsert);
            toInsert->m_isContoured = true;
            toInsert->m_contourColor = color;
            toInsert->m_contourWidth = width;
        }
    }

    void SceneGraph::CheckNodeIsNotInAnyList(SgNode* sgNode) const
    {
        auto describe = [sgNode](char const* what)
        {
            M3D_LOG_ERR(CStr(what) + CStr(sgNode->GetName()) + "', class = '" + CStr(sgNode->GetClassNameA()));
        };

        if (m_ttledList.find(sgNode) != m_ttledList.end())
        {
            describe("Error: node is in TtledList, name = '");
        }
        if (m_RemoveIfFreeList.find(sgNode) != m_RemoveIfFreeList.end())
        {
            describe("Error: node is in RemoveIfFreeList, name = '");
        }
        if (m_updateXFormList.find(sgNode) != m_updateXFormList.end())
        {
            describe("Error: node is in UpdateXFormList, name = '");
        }
    }

    void SceneGraph::InsertInTtlList(SgNode* toInsert, int frameToDie)
    {
        toInsert->m_ttl = frameToDie;
        m_ttledList.insert(toInsert);
    }

    void SceneGraph::LinkThinkNode(SgNode* toThink)
    {
        m_thinkList.insert(toThink);
    }

    void SceneGraph::DeleteFromUpdateXFormList(SgNode* toDelete)
    {
        // TODO: check this
        m_updateXFormList.erase(toDelete);

        // Process child nodes recursively using a stack
        std::vector<m3d::Object*> stack;
        stack.push_back(toDelete);

        while (!stack.empty())
        {
            m3d::Object* current = stack.back();
            stack.pop_back();

            // Process all children of current node
            m3d::SgNode* childNode = dynamic_cast<m3d::SgNode*>(current->GetFirstChild());
            while (childNode)
            {
                m_updateXFormList.erase(childNode);

                // If child has children, add to stack for processing
                if (childNode->GetFirstChild())
                {
                    stack.push_back(childNode);
                }

                childNode = dynamic_cast<m3d::SgNode*>(childNode->GetNextSibling());
            }
        }
    }

    void SceneGraph::GetCellsStatistic(retruxx::vector<CellInfo>* cellStats)
    {
        int const sizeInCells = m_owner->m_level->land_size;
        for (int x = 0; x < sizeInCells; ++x)
        {
            for (int z = 0; z < sizeInCells; ++z)
            {
                CellInfo& info = (*cellStats)[x * sizeInCells + z];
                info.trisCount = 0;
                info.objCount = 0;
                info.meshesCount = 0;

                CellItems& cell = m_cellItems[64 * z + x];
                cell.m_bVisibleInCurrentFrame = true;
                auto* lists = cell.m_nodesLinkedDirect.GetObjects();

                auto& animModels = lists[SgAnimatedModelNode::m_classSgAnimatedModelNode.m_index];
                info.animModelCount = static_cast<unsigned short>(animModels.size());
                for (m3d::Object* obj : animModels)
                {
                    AddNodesTrisRecursive(info.trisCount, info.meshesCount, static_cast<SgNode*>(obj));
                }

                auto& gameUnits = lists[SgGameUnitNode::m_classSgGameUnitNode.m_index];
                info.animModelCount += static_cast<unsigned short>(gameUnits.size());
                for (m3d::Object* obj : gameUnits)
                {
                    AddNodesTrisRecursive(info.trisCount, info.meshesCount, static_cast<SgNode*>(obj));
                }

                for (int cls = 0; cls < 64; ++cls)
                {
                    info.objCount += static_cast<unsigned short>(lists[cls].size());
                }
            }
        }
    }

    SceneGraph::CellItems& SceneGraph::GetCellItems(int x, int z)
    {
        return m_cellItems[64 * z + x];
    }

    SceneGraph::CellItems const& SceneGraph::GetCellItems(int x, int z) const
    {
        return m_cellItems[64 * z + x];
    }

    void SceneGraph::CollectShadowingNodesStencil(
        retruxx::set<SgNode*>& nodes,
        Class* clazz,
        int x,
        int z,
        unsigned ls,
        int /*curFrame*/)
    {
        if (static_cast<unsigned>(x) >= ls || static_cast<unsigned>(z) >= ls ||
            (m_enableMap[256 * z + x] & m_enableVisSpaceMask) == 0)
        {
            return;
        }

        auto collect = [&nodes, clazz](SgNode* node)
        {
            if (node->GetClass() != clazz)
            {
                return;
            }
            int shadowing = 0;
            node->GetServerItemProperty(6, &shadowing);
            if (shadowing)
            {
                nodes.insert(node);
            }
        };

        for (m3d::Object* obj : m_cellItems[64 * z + x].m_nodesShadowingDirect)
        {
            auto* root = static_cast<SgNode*>(obj);
            collect(root);

            std::vector<m3d::Object*> stack;
            stack.push_back(root);
            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                auto* child = dynamic_cast<SgNode*>(current->GetFirstChild());
                while (child)
                {
                    collect(child);
                    if (child->GetFirstChild())
                    {
                        stack.push_back(child);
                    }
                    child = dynamic_cast<SgNode*>(child->GetNextSibling());
                }
            }
        }
    }

    void SceneGraph::CollectShadowingNodes(
        retruxx::set<SgNode*>& nodes,
        Class* clazz,
        int x,
        int z,
        int size,
        unsigned ls,
        int curFrame)
    {
        for (int cx = x; cx < x + size; ++cx)
        {
            for (int cz = z; cz < z + size; ++cz)
            {
                if (static_cast<unsigned>(cx) >= ls || static_cast<unsigned>(cz) >= ls ||
                    (m_enableVisSpaceMask & m_enableMap[256 * cz + cx]) == 0)
                {
                    continue;
                }

                for (m3d::Object* obj : m_cellItems[64 * cz + cx].m_nodesShadowingDirect)
                {
                    auto* root = static_cast<SgNode*>(obj);
                    if (root->m_frameVisible2 != curFrame)
                    {
                        continue;
                    }
                    if (root->GetClass() == clazz)
                    {
                        nodes.insert(root);
                    }

                    std::vector<m3d::Object*> stack;
                    stack.push_back(root);
                    while (!stack.empty())
                    {
                        m3d::Object* current = stack.back();
                        stack.pop_back();

                        auto* child = dynamic_cast<SgNode*>(current->GetFirstChild());
                        while (child)
                        {
                            if (child->GetClass() == clazz)
                            {
                                nodes.insert(child);
                            }
                            if (child->GetFirstChild())
                            {
                                stack.push_back(child);
                            }
                            child = dynamic_cast<SgNode*>(child->GetNextSibling());
                        }
                    }
                }
            }
        }
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
            if ((((v12 * v12) + (v13 * v13)) +
                 ((origin.x - n->m_currentWorldOrigin.x) * (origin.x - n->m_currentWorldOrigin.x))) <= (prop * prop))
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
        if (!frusta.testSphere(orgForSphere, na * 2.0))
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

    void SceneGraph::RemoveNodeExceptRemoveIfFree(SgNode*& toRemove)
    {
        auto forGraph = toRemove->m_forGraph;
        if (forGraph && forGraph->m_cellsCoveredPoint0.x <= forGraph->m_cellsCoveredPoint1.x &&
            forGraph->m_cellsCoveredPoint0.y <= forGraph->m_cellsCoveredPoint1.y)
        {
            UnlinkNode(toRemove);
        }

        auto parent = toRemove->GetParent();
        if (parent)
            parent->RemoveChild(toRemove);

        DeleteFromUpdateXFormList(toRemove);
        CheckNodeValidity(toRemove, "Check Two");

        // TODO: check this
        // TODO: DecRef
        toRemove->DecRef();
        toRemove = nullptr;
    }

    int SceneGraph::getYOfs(int y)
    {
        return y * (6 * y + 2);
    }

    void SceneGraph::EnsureEverythingIsUnlinked() const
    {
        // The shipped release build compiles this to an empty body - it is a
        // debug-only consistency check.
    }

    void SceneGraph::DrawShadows()
    {
        // TODO: implement SceneGraph::DrawShadow
        //RETRUXX_NOT_IMPLEMENTED;
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

    SgNode* SceneGraph::TraceLineThruCellNodesForClass(
        float& tt,
        int cx,
        int cz,
        CVector const& v0,
        CVector const& dir,
        Class* wantClazz,
        retruxx::set<SgNode*>& dontCheckTwice,
        unsigned traceMode)
    {
        if (wantClazz->m_index >= 64)
        {
            return nullptr;
        }

        SgNode* toRet = nullptr;
        float bestT = 100000.0f;

        auto& objs = m_cellItems[64 * cz + cx].m_nodesLinkedDirect.GetObjects()[wantClazz->m_index];
        for (m3d::Object* obj : objs)
        {
            auto* node = static_cast<SgNode*>(obj);
            if (dontCheckTwice.find(node) != dontCheckTwice.end())
            {
                continue;
            }
            dontCheckTwice.insert(node);

            // traceMode bit 1 skips allies, bit 2 skips enemies - both relative
            // to whoever the player is currently driving.
            if ((traceMode & 6) != 0)
            {
                ai::Vehicle* playerVehicle = m_owner->GetVehicleControlledByPlayer();
                if (playerVehicle)
                {
                    ai::PhysicBody* physicBody = nullptr;
                    node->GetProperty(PROP_NODE_PHYSICBODY, &physicBody);
                    if (physicBody)
                    {
                        ai::eTolerance const tolerance =
                            ai::theRelationship->CheckTolerance(playerVehicle->GetBelong(), physicBody->GetBelong());
                        bool const allyOk = (traceMode & 4) == 0 || tolerance < ai::RS_ALLY;
                        bool const enemyOk = (traceMode & 2) == 0 || tolerance > ai::RS_ENEMY;
                        if (!allyOk || !enemyOk)
                        {
                            continue;
                        }
                    }
                }
            }

            SgNode* exactHitNode = nullptr;
            float const t = node->IntersectRay(v0, dir, exactHitNode, wantClazz);
            if (t >= 0.0f && bestT > t)
            {
                bestT = t;
                toRet = exactHitNode;
            }
        }

        if (bestT != 100000.0f)
        {
            tt = bestT;
        }
        return toRet;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool SceneGraph::IsTransparent(SgNode* n)
    {
        if (n->m_transparencyType == TT_NONE)
            return 0;

        if (n->m_transparencyType == TT_PERMANENT)
            return this->m_transparencyTest->setPermanentTransparency(n);

        if (!inTransparencyRadius)
            return 0;

        auto na = (((n->m_originWorldAbsForSphere.z - camOrg.z) * (n->m_originWorldAbsForSphere.z - camOrg.z)) +
                   ((n->m_originWorldAbsForSphere.x - camOrg.x) * (n->m_originWorldAbsForSphere.x - camOrg.x))) +
            ((n->m_originWorldAbsForSphere.y - camOrg.y) * (n->m_originWorldAbsForSphere.y - camOrg.y));

        if (na >= (transparentRadius * transparentRadius))
            return 0;

        auto v5 = sqrt(na) / transparentRadius;
        return this->m_transparencyTest->test(n, v5);
    }

    void SceneGraph::enableVisibleCells_r(CClipper& frusta, float* box, unsigned int orFlags)
    {
        // TODO: generated code
        float const VISCELL_EDGE_LENGTH = 128.0f;
        float const MAX_LAND_SIZE = m_owner->m_level->land_size * VISCELL_EDGE_LENGTH;

        // Test bounding box against frustum
        CVector ofs(0, 0, 0);
        int testResult = frusta.testBBox(tbFullTest, box, ofs);

        // Handle fully outside frustum
        if (testResult == 0)
        {
            enableCellsSetRect(box, 0, ~orFlags);
            return;
        }

        // Handle fully inside frustum
        if (testResult == 2)
        {
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
        if (width > VISCELL_EDGE_LENGTH && depth > VISCELL_EDGE_LENGTH)
        {
            // Calculate number of subdivisions
            float xCells = std::ceil((width * 0.5f) / VISCELL_EDGE_LENGTH);
            float zCells = std::ceil((depth * 0.5f) / VISCELL_EDGE_LENGTH);

            // Calculate sub-box dimensions
            float subWidth = VISCELL_EDGE_LENGTH * xCells;
            float subDepth = VISCELL_EDGE_LENGTH * zCells;

            // Create four sub-boxes
            float newBoxes[4][6] = {
                // Bottom-left sub-box
                {box[0], box[1], box[2], box[0] + subWidth, box[4], box[2] + subDepth},

                // Bottom-right sub-box
                {box[0] + subWidth, box[1], box[2], box[0] + subWidth * 2, box[4], box[2] + subDepth},

                // Top-right sub-box
                {box[0] + subWidth, box[1], box[2] + subDepth, box[0] + subWidth * 2, box[4], box[2] + subDepth * 2},

                // Top-left sub-box
                {box[0], box[1], box[2] + subDepth, box[0] + subWidth, box[4], box[2] + subDepth * 2}};

            // Process each sub-box
            for (auto& newBox : newBoxes)
            {
                // Clamp to landscape boundaries
                newBox[3] = std::min(newBox[3], MAX_LAND_SIZE);
                newBox[5] = std::min(newBox[5], MAX_LAND_SIZE);

                // Get height range for this box
                m_owner->GetLandscape().getMinMaxHeightForBox(newBox, 0.0f);

                // Recurse into sub-box
                enableVisibleCells_r(frusta, newBox, orFlags);
            }
        }
        else
        {
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
        // TODO: check this
        auto cls = obj->GetClass();
        auto& objs = m_objectsByClassIdx[cls->m_index];
        auto it = std::find(objs.begin(), objs.end(), obj);
        if (it != objs.end())
        {
            objs.erase(it);

            //delete obj;
        }
        else
        {
            M3D_LOG_INFO("Warning, object not found: " + CStr(obj->GetName()));
        }
    }

    retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>>* ObjectsContainer::GetObjectsByClass(m3d::Class* cl)
    {
        if (cl->m_index >= 64)
        {
            return nullptr;
        }
        return &m_objectsByClassIdx[cl->m_index];
    }

    retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>> const* ObjectsContainer::GetObjects() const
    {
        return m_objectsByClassIdx;
    }

    retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>>* ObjectsContainer::GetObjects()
    {
        return m_objectsByClassIdx;
    }

    bool ObjectsContainer::empty() const
    {
        // NOTE: the shipped build inlines this everywhere, so no standalone
        // body survives to transcribe; reconstructed as the obvious meaning.
        for (auto const& objs : m_objectsByClassIdx)
        {
            if (!objs.empty())
            {
                return false;
            }
        }
        return true;
    }
}  // namespace m3d

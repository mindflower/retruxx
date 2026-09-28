#define NOMINMAX
#include <stdexcept>
#include <scene/scenegraph.h>
#include <algorithm>
#include <cmath>
#include "landscape.h"
#include "math/coremath.h"

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "core/timer.h"
#include "world.h"
#include "level.h"
#include "client.h"
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

    // Shadows never start fading closer than this, however short the fog is.
    float const FADE_START = 128.0;

    // The shadow texture packs one cell per colour channel.
    int const CELLS_PER_SHADOW_TEXTURE = 3;
    unsigned const COLOR_MASK[CELLS_PER_SHADOW_TEXTURE] = {0x0000FFu, 0x00FF00u, 0xFF0000u};

    struct ShadowStats
    {
        // RVA 0x8A1D10
        /* 0x0000 */ int numToTexDIPs = 0;
        /* 0x0004 */ int numPutDIPs = 0;
        /* 0x0008 */ int numToTexPolys = 0;
        /* 0x000c */ int numPutPolys = 0;
        /* 0x0010 */ int numCells = 0;
    };

    // RVA 0x8A2360 - builds the world -> shadow texture transform for one cell
    // batch: move the batch centre to the origin, scale it into the [-0.5, 0.5]
    // box, shift it into [0, 1] and finally move the world z into the texture v
    // coordinate. The y row of the scale is deliberately zero, so the height of
    // a point never reaches the texture coordinates.
    CMatrix CalcLinearTransform(float sc, float tx, float tz)
    {
        CMatrix shift;
        shift.zero();
        shift._11 = 1.0f;
        shift._22 = 1.0f;
        shift._33 = 1.0f;
        shift._44 = 1.0f;
        shift._41 = -tx;
        shift._43 = -tz;

        CMatrix scale;
        scale.zero();
        scale._11 = sc;
        scale._33 = sc;
        scale._44 = 1.0f;

        CMatrix shiftHalf1;
        shiftHalf1.zero();
        shiftHalf1._11 = 1.0f;
        shiftHalf1._22 = 1.0f;
        shiftHalf1._33 = 1.0f;
        shiftHalf1._44 = 1.0f;
        shiftHalf1._41 = 0.5f;
        shiftHalf1._43 = 0.5f;

        // Swaps the y and z columns.
        CMatrix change;
        change.zero();
        change._11 = 1.0f;
        change._23 = 1.0f;
        change._32 = 1.0f;
        change._44 = 1.0f;

        return shift * scale * shiftHalf1 * change;
    }

    struct SortCells
    {
        // RVA 0x8A2270 / 0x8A22A0 - orders cells back to front, measuring from
        // the camera to the centre of each cell. The y term is the camera
        // height and is identical on both sides, so it only shifts both
        // distances by the same amount.
        explicit SortCells(CVector const& p) : pos(p)
        {
        }

        bool operator()(retruxx::pair<int, int> const& a, retruxx::pair<int, int> const& b) const
        {
            float const ax = (static_cast<float>(a.first) + 0.5f) * VISCELL_EDGE_LENGTH_6 - pos.x;
            float const az = (static_cast<float>(a.second) + 0.5f) * VISCELL_EDGE_LENGTH_6 - pos.z;
            float const bx = (static_cast<float>(b.first) + 0.5f) * VISCELL_EDGE_LENGTH_6 - pos.x;
            float const bz = (static_cast<float>(b.second) + 0.5f) * VISCELL_EDGE_LENGTH_6 - pos.z;
            float const y = -pos.y;
            return (az * az + y * y) + ax * ax > (bz * bz + y * y) + bx * bx;
        }

        /* 0x0000 */ CVector pos;
    };

    CVector camOrg;
    float transparentRadius = 0.0;
    bool inTransparencyRadius = false;
}  // namespace

int clipLineToBox(CVector* v, float* box);

namespace
{
    // The shipped MemoryManager::Malloc puts the block size and 0xDEADBEEF in front of every allocation and
    // 0xFEEBDAED behind it, and CheckNodeValidity reads those words around a node. This build allocates nodes with
    // the C runtime instead, so the words are not there and the check is switched off.
    constexpr bool NODES_HAVE_GUARD_WORDS = false;

    // Read by RenderDebugForNode but never assigned, so every label says 0.
    int g_nodeNum = 0;

    void DumpNodeInfo(m3d::SgNode* node)
    {
        // RVA 0x635FE0 - logs the node and each of its ancestors.
        while (node)
        {
            {
                M3D_LOG_INFO(CStr("Node class = '") + CStr(node->GetClassNameA()) + CStr("'"));
            }
            {
                M3D_LOG_INFO(CStr("Node name = '") + CStr(node->GetName()) + CStr("'"));
            }
            m3d::DataServer* const server = node->GetServer();
            if (!server)
            {
                M3D_LOG_INFO("Node server is NULL");
            }
            else
            {
                int serverHandle = -1;
                node->GetProperty(m3d::PROP_NODE_HANDLE, &serverHandle);
                if (serverHandle == -1)
                {
                    M3D_LOG_INFO("Node has invalid handle");
                }
                else
                {
                    CStr name;
                    CStr fileName;
                    server->GetItemProperty(serverHandle, m3d::PROP_MODEL_NAME, &name);
                    server->GetItemProperty(serverHandle, m3d::PROP_MODEL_FILENAME, &fileName);
                    M3D_LOG_INFO(CStr("Node model: '") + name + CStr("' (") + fileName + CStr(")"));
                }
            }

            m3d::Object* const parent = node->GetParent();
            if (!parent)
            {
                break;
            }
            if (!parent->IsKindOf(&m3d::SgNode::m_classSgNode))
            {
                M3D_LOG_INFO("Node parent is not SgNode!");
                return;
            }
            {
                M3D_LOG_INFO("\nNode parent:");
            }
            node = static_cast<m3d::SgNode*>(parent);
        }
    }

    void CheckNodeValidity(m3d::SgNode* node, char const* debugStr)
    {
        // RVA 0x6368C0 - checks the allocator's guard words around the node and stops in the debugger when either
        // is damaged.
        if (!NODES_HAVE_GUARD_WORDS)
        {
            return;
        }

        int const* const header = reinterpret_cast<int const*>(node);
        bool valid = true;
        if (static_cast<unsigned>(header[-1]) != 0xDEADBEEF)
        {
            M3D_LOG_ERR(CStr("Error: node prefix is invalid: ") + CStr(header[-1]));
            valid = false;
        }
        int const size = header[-2];
        int const* const postfix = reinterpret_cast<int const*>(reinterpret_cast<char const*>(node) + size);
        if (valid)
        {
            if (static_cast<unsigned>(*postfix) == 0xFEEBDAED)
            {
                return;
            }
            M3D_LOG_ERR(CStr("Error: node postfix is invalid: ") + CStr(*postfix));
        }
        {
            M3D_LOG_INFO(CStr("Debug string: '") + CStr(debugStr) + CStr("'"));
        }
        {
            M3D_LOG_INFO(CStr("Node size = ") + CStr(size));
        }
        DumpNodeInfo(node);
        __debugbreak();
    }

    // Line points outside the landscape (one cell's margin in, up to 2000 high) are cut off.
    int clipLineToLanscapeRect(CVector* v, int x0, int z0, int x1, int z1)
    {
        // RVA 0x88D8A0
        CVector vv[3];
        vv[0] = v[0];
        vv[1] = v[1];
        float box[6];
        box[1] = 0.0f;
        box[4] = 2000.0f;
        box[0] = static_cast<float>(x0) * VISCELL_EDGE_LENGTH_6 + 1.0f;
        box[2] = static_cast<float>(z0) * VISCELL_EDGE_LENGTH_6 + 1.0f;
        box[3] = static_cast<float>(x1) * VISCELL_EDGE_LENGTH_6 - 1.0f;
        box[5] = static_cast<float>(z1) * VISCELL_EDGE_LENGTH_6 - 1.0f;
        int const result = clipLineToBox(vv, box);
        v[0] = vv[0];
        v[1] = vv[1];
        return result;
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
        // RVA 0x639C50
        GraphItemsForSgNode* const gi = new GraphItemsForSgNode;
        toLink->m_forGraph = gi;

        PointBase<int> p0;
        PointBase<int> p1;
        toLink->GetVisCellBounds(p0, p1);
        gi->m_cellsCoveredPoint0 = p0;
        gi->m_cellsCoveredPoint1 = p1;

        for (int x = p0.x; x <= p1.x; ++x)
        {
            for (int z = p0.y; z <= p1.y; ++z)
            {
                m_cellItems[64 * z + x].m_nodesLinkedDirect.AddObject(toLink);
            }
        }

        int const maxIdx = m_owner->m_level->land_size - 1;
        int modelCastShadow = 0;
        toLink->GetServerItemProperty(PROP_MODEL_CAST_SHADOW, &modelCastShadow);
        if (!modelCastShadow)
        {
            return;
        }
        bool nodeCastShadow = false;
        toLink->GetProperty(PROP_DM_CAST_SHADOW, &nodeCastShadow);
        if (!nodeCastShadow)
        {
            return;
        }

        // The shadow can fall anywhere around the node, so the box is grown on all sides by 1.3 of its height.
        CVector const& org = toLink->m_currentWorldOrigin;
        Aabb const box = toLink->m_boundingBox;
        float const margin = ((org.y + box.m_box[4]) - (org.y + box.m_box[1])) * 1.3f;
        float const invCell = 1.0f / VISCELL_EDGE_LENGTH_6;
        int const x0 = std::clamp(static_cast<int>(invCell * ((org.x + box.m_box[0]) - margin)), 0, maxIdx);
        int const x1 = std::clamp(static_cast<int>(invCell * (margin + (org.x + box.m_box[3]))), 0, maxIdx);
        int const z0 = std::clamp(static_cast<int>(invCell * ((org.z + box.m_box[2]) - margin)), 0, maxIdx);
        int const z1 = std::clamp(static_cast<int>(invCell * (margin + (org.z + box.m_box[5]))), 0, maxIdx);

        for (int x = x0; x <= x1; ++x)
        {
            for (int z = z0; z <= z1; ++z)
            {
                m_cellItems[64 * z + x].m_nodesShadowingDirect.insert(toLink);
                // The cell is remembered as (z << 16) + x for UnlinkNode.
                gi->m_cellsShadowCovered.push_back(x + (z << 16));
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
        // RVA 0x6353E0
        // Alpha test quantises alpha to this many steps; nothing may go below the first one.
        int const alphaSteps = 256 / (M3D_ENGINE_CFG.m_alphaTestWorld.GetI() + 1);
        float const alphaStep = 1.0f / static_cast<float>(alphaSteps);

        // Nodes fade out over the last 1.5 s of their time to live.
        float maxAlpha = 1.0f;
        if (node->m_ttl > 0)
        {
            int const timeLeft = node->m_ttl - M3D_KERNEL->GetTimer().GetFrameStartTime();
            if (timeLeft < 1500)
            {
                maxAlpha = static_cast<float>(std::max(timeLeft, 0)) * 0.00066666666f;
            }
        }

        // Small on-screen objects fade out (1 / 19.5 per unit of size).
        float alpha = node->m_onScreenSize;
        if (alpha < 20.0f && !m_noModelCull)
        {
            alpha *= 0.051282052f;
        }

        // Transparent static models (trees) fade near the camera, down to alphaStep.
        if (IS_KIND_OF(node, SgStaticModelNode))
        {
            int transparency = 0;
            node->GetServerItemProperty(PROP_MODEL_TRANS, &transparency);
            if (transparency)
            {
                CVector const d = M3D_RENDERER->MatGetOrgInv() - node->m_originWorldAbsForSphere;
                float distance = std::sqrt(d.z * d.z + d.y * d.y + d.x * d.x) - 64.0f - node->m_boundingRadius;
                if (distance < 0.0f)
                {
                    distance = 0.0f;
                }
                if (distance <= 64.0f)
                {
                    float const k = distance * 0.015625f;
                    alpha = (maxAlpha - alphaStep) * k * k + alphaStep;
                }
            }
        }

        // NOTE: alpha starts as the raw on-screen size, not 1, so a node that is not faded by size is simply
        // capped by maxAlpha (and with m_noModelCull a node under one unit keeps its size as alpha).
        return maxAlpha > alpha ? alpha : maxAlpha;
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

    void SceneGraph::DumpRenderingNodesInfoForClass(Class const* nodeClass)
    {
        // RVA 0x635860 - logs the models of the nodes of one class gathered for rendering this frame.
        if (!nodeClass)
        {
            return;
        }
        SgNode** const slots = &m_visSlots[2000 * nodeClass->m_index];
        int const numNodes = m_visNumSlots[nodeClass->m_index];
        if (!numNodes)
        {
            return;
        }
        DataServer* const server = slots[0]->GetServer();
        if (!server)
        {
            return;
        }
        for (int i = 0; i < numNodes; ++i)
        {
            CStr logStr = CStr(i) + CStr(": ");
            int serverHandle = -1;
            slots[i]->GetProperty(PROP_NODE_HANDLE, &serverHandle);
            if (serverHandle == -1)
            {
                logStr += CStr("Invalid node");
            }
            else
            {
                CStr name;
                CStr fileName;
                server->GetItemProperty(serverHandle, PROP_MODEL_NAME, &name);
                server->GetItemProperty(serverHandle, PROP_MODEL_FILENAME, &fileName);
                logStr += name + CStr(" (") + fileName + CStr(")");
            }
            M3D_LOG_INFO(logStr);
        }
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
        // RVA 0x63B050
        if (!toInsert)
        {
            return;
        }
        CheckNodeValidity(toInsert, "Check from InsertInRemoveIfFree");
        if (m_bIsPurgingRemoveIfFree)
        {
            M3D_LOG_WARN(
                CStr("Warning: inserting node in RemoveIfFree when it is being purged! node name = '") +
                CStr(toInsert->GetName()) + CStr("', class = '") + CStr(toInsert->GetClassNameA()) + CStr("'"));
        }

        toInsert->RemoveImmediateAfterParent(false);
        m_RemoveIfFreeList.insert(toInsert);
        toInsert->m_persistant = 0;
        toInsert->m_isInRemoveIfFree = true;
        toInsert->m_isRemoveIfFree = true;

        // The whole subtree becomes remove-if-free, but only the node itself sits in the list.
        std::vector<Object*> stack;
        stack.push_back(toInsert);
        while (!stack.empty())
        {
            Object* const current = stack.back();
            stack.pop_back();
            for (auto* child = static_cast<SgNode*>(current->GetFirstChild()); child;
                 child = static_cast<SgNode*>(child->GetNextSibling()))
            {
                child->m_isRemoveIfFree = true;
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
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
        // RVA 0x63A070
        // NOTE: the original has no null checks and dereferences m_forGraph unconditionally; the guard only
        // turns that crash into a no-op.
        if (!toUnlink || !toUnlink->m_forGraph)
        {
            return;
        }

        GraphItemsForSgNode* const gi = toUnlink->m_forGraph;
        toUnlink->m_forGraph = nullptr;

        Landscape& landscape = m_owner->GetLandscape();
        for (int x = gi->m_cellsCoveredPoint0.x; x <= gi->m_cellsCoveredPoint1.x; ++x)
        {
            for (int z = gi->m_cellsCoveredPoint0.y; z <= gi->m_cellsCoveredPoint1.y; ++z)
            {
                m_cellItems[64 * z + x].m_nodesLinkedDirect.RemoveObject(toUnlink);

                // The landscape frees its per-node collision list together with the last covered cell.
                bool const isLastCell = x == gi->m_cellsCoveredPoint1.x && z == gi->m_cellsCoveredPoint1.y;
                landscape.UnlinkNodeCollisionGeomsFromCell(toUnlink, x, z, isLastCell);

                // The whole subtree's collision geoms are unlinked as well (the node itself only once, above).
                std::vector<Object*> stack;
                stack.push_back(toUnlink);
                while (!stack.empty())
                {
                    Object* const current = stack.back();
                    stack.pop_back();
                    for (auto* child = static_cast<SgNode*>(current->GetFirstChild()); child;
                         child = static_cast<SgNode*>(child->GetNextSibling()))
                    {
                        landscape.UnlinkNodeCollisionGeomsFromCell(child, x, z, isLastCell);
                        if (child->GetFirstChild())
                        {
                            stack.push_back(child);
                        }
                    }
                }
            }
        }

        // Cells were remembered by LinkNode as (z << 16) + x.
        for (unsigned int const cell : gi->m_cellsShadowCovered)
        {
            m_cellItems[64 * (cell >> 16) + (cell & 0xFFFF)].m_nodesShadowingDirect.erase(toUnlink);
        }

        gi->m_cellsCoveredPoint0 = {0, 0};
        gi->m_cellsCoveredPoint1 = {-1, -1};
        gi->m_cellsShadowCovered.clear();
        delete gi;
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

    SgNode* SceneGraph::TraceLine(
        CVector& hit, CVector const& start, CVector const& finish, retruxx::set<Class*> const& cl0, unsigned traceMode)
    {
        // RVA 0x88DE90 (traceline.cpp) - walks the visibility cells under the segment and returns the nearest node
        // of the wanted classes it crosses (the landscape counts as a node). A set holding nullptr means the
        // landscape, static and animated models and game units.
        CVector v[2] = {start, finish};
        int const landSize = m_owner->m_level->land_size;
        if (!clipLineToLanscapeRect(v, 0, 0, landSize, landSize))
        {
            return nullptr;
        }

        retruxx::set<Class*> classes;
        if (cl0.find(nullptr) == cl0.end())
        {
            classes = cl0;
        }
        else
        {
            classes.insert(RT_CLASS_LOCAL(Landscape));
            classes.insert(RT_CLASS_LOCAL(SgStaticModelNode));
            classes.insert(RT_CLASS_LOCAL(SgAnimatedModelNode));
            classes.insert(RT_CLASS_LOCAL(SgGameUnitNode));
        }

        CVector const delta(finish.x - start.x, finish.y - start.y, finish.z - start.z);
        SgNode* hitNodes[64];
        float hitDistances[64];
        hitNodes[(RT_CLASS_LOCAL(Landscape))->m_index] = &m_owner->m_landscape;
        for (float& d : hitDistances)
        {
            d = -1.0f;
        }
        float const lineLength = std::sqrt(delta.x * delta.x + delta.z * delta.z + delta.y * delta.y);
        float const invLength =
            1.0f / std::sqrt(delta.z * delta.z + delta.y * delta.y + delta.x * delta.x + 0.00000011920929f);
        CVector const dir(invLength * delta.x, delta.y * invLength, delta.z * invLength);

        retruxx::set<SgNode*> dontCheckTwice;
        int const x0 = static_cast<int>(v[0].x);
        int const z0 = static_cast<int>(v[0].z);
        CBrezLine cellLine;
        cellLine.start(x0, z0, static_cast<int>(v[1].x), static_cast<int>(v[1].z));
        int const ssz = static_cast<int>(static_cast<float>(m_owner->m_level->land_size) * VISCELL_EDGE_LENGTH_6);
        if (x0 < 0 || z0 < 0 || x0 >= ssz || z0 >= ssz)
        {
            // NOTE: the "dst" part of the message prints the start point again, and "ssz = " follows the z0 value
            // without a space.
            M3D_LOG_INFO(CStr("TRACELINE ERROR: x0 = ") + CStr(x0) + CStr(" z0 = ") + CStr(z0) + CStr("ssz = ") +
                CStr(ssz) + CStr(" src = ") + CStr(start.x) + CStr(",") + CStr(start.y) + CStr(",") + CStr(start.z) +
                CStr(" dst = ") + CStr(start.x) + CStr(",") + CStr(start.y) + CStr(",") + CStr(start.z));
            return nullptr;
        }

        float prevCellX = -1.0f;
        float prevCellZ = -1.0f;
        float const scale = 1.0f / VISCELL_EDGE_LENGTH_6;
        int px = 0;
        int pz = 0;
        if (!cellLine.step(px, pz))
        {
            return nullptr;
        }
        for (;;)
        {
            float const cellX = static_cast<float>(std::floor(static_cast<double>(px) * scale));
            float const cellZ = static_cast<float>(std::floor(static_cast<double>(pz) * scale));
            if ((prevCellX != cellX || prevCellZ != cellZ) && cellX >= 0.0f && cellZ >= 0.0f && cellX < 64.0f &&
                cellZ < 64.0f)
            {
                prevCellX = cellX;
                prevCellZ = cellZ;
                int const cx = static_cast<int>(cellX);
                int const cz = static_cast<int>(cellZ);
                for (Class* const cls : classes)
                {
                    int const index = cls->m_index;
                    if (cls == RT_CLASS_LOCAL(Landscape))
                    {
                        m_owner->m_landscape.traceLineThruCellLs(
                            hitDistances[index], cx, cz, start, dir, (traceMode & 1) != 0);
                    }
                    else
                    {
                        hitNodes[index] = TraceLineThruCellNodesForClass(
                            hitDistances[index], cx, cz, start, dir, cls, dontCheckTwice, traceMode);
                    }
                }

                // Hits beyond the end of the segment do not count.
                bool found = false;
                for (Class* const cls : classes)
                {
                    float& distance = hitDistances[cls->m_index];
                    if (distance >= 0.0f)
                    {
                        if (distance <= lineLength)
                        {
                            found = true;
                        }
                        else
                        {
                            distance = -1.0f;
                        }
                    }
                }
                if (found)
                {
                    break;
                }
            }
            if (!cellLine.step(px, pz))
            {
                return nullptr;
            }
        }

        float min = 999999.0f;
        SgNode* nearest = nullptr;
        for (Class* const cls : classes)
        {
            int const index = cls->m_index;
            if (hitDistances[index] >= 0.0f && min > hitDistances[index])
            {
                nearest = hitNodes[index];
                min = hitDistances[index];
            }
        }
        hit.x = dir.x * min + start.x;
        hit.y = dir.y * min + start.y;
        hit.z = dir.z * min + start.z;
        return nearest;
    }

    void SceneGraph::LightSetupLightsForNode(SgNode* node)
    {
        // RVA 0x7AF6B0 - a single directional sun light, with the direction brought into the node's local space.
        CVector const& sun = m_owner->GetSun(0.0f);
        CMatrix const& xf = node->m_currentXForm;
        // NOTE: the sums are kept in the binary's order (z, y, then x) so the float rounding matches.
        CVector const dir(
            0.0f - ((xf._13 * sun.z + xf._12 * sun.y) + sun.x * xf._11),
            0.0f - ((xf._23 * sun.z + xf._22 * sun.y) + xf._21 * sun.x),
            0.0f - ((xf._33 * sun.z + xf._32 * sun.y) + xf._31 * sun.x));

        rend::LightSource light;
        light.m_type = rend::M3DLIGHT_DIRECTIONAL;
        light.m_direction = dir;
        light.m_origin = dir;
        light.m_range = 1000.0f;
        light.m_diffuse = rend::Colorf(m_owner->GetWeatherDiffuseColor());
        light.m_ambient = rend::Colorf(m_owner->GetWeatherAmbientColor());

        M3D_RENDERER->LightSet(0, light);
        M3D_RENDERER->LightEnable(0, true);
    }

    void SceneGraph::Update()
    {
        // RVA 0x63E780
        auto const frameStartTime = M3D_KERNEL->GetTimer().GetFrameStartTime();
        UpdateAllXForms();

        // Nodes whose time to live has run out are removed right away.
        for (auto ttlIt = m_ttledList.begin(); ttlIt != m_ttledList.end();)
        {
            SgNode* node = *ttlIt;
            if (node->m_ttl <= 0 || node->m_ttl >= frameStartTime)
            {
                ++ttlIt;
                continue;
            }
            ttlIt = m_ttledList.erase(ttlIt);
            m_RemoveIfFreeList.erase(node);
            RemoveNodeExceptRemoveIfFree(node);
        }

        // A remove-if-free node is deleted once every node of its subtree reports it is free.
        m_bIsPurgingRemoveIfFree = true;
        for (auto removeIt = m_RemoveIfFreeList.begin(); removeIt != m_RemoveIfFreeList.end();)
        {
            SgNode* const node = *removeIt;
            // NOTE: the node itself is never asked, only its descendants.
            bool needToRemove = true;
            std::vector<Object*> stack;
            stack.push_back(node);
            while (!stack.empty())
            {
                Object* const current = stack.back();
                stack.pop_back();
                for (auto* child = static_cast<SgNode*>(current->GetFirstChild()); child;
                     child = static_cast<SgNode*>(child->GetNextSibling()))
                {
                    if (!child->IsFree())
                    {
                        needToRemove = false;
                    }
                    if (child->GetFirstChild())
                    {
                        stack.push_back(child);
                    }
                }
            }

            if (!needToRemove)
            {
                ++removeIt;
                continue;
            }

            removeIt = m_RemoveIfFreeList.erase(removeIt);
            if (IsLinkedNode(node))
            {
                UnlinkNode(node);
            }
            if (Object* parent = node->GetParent())
            {
                parent->RemoveChild(node);
            }
            DeleteFromUpdateXFormList(node);
            CheckNodeValidity(node, "Check Two");
            // The binary calls the virtual deleting destructor directly, not DecRef.
            delete node;
        }
        m_bIsPurgingRemoveIfFree = false;
    }

    void SceneGraph::RenderDebugForNode(SgNode* n)
    {
        // RVA 0x634B90 - labels the node with its class above its origin. NOTE: the label is lifted along Y by half
        // the box's Z extent.
        auto* const renderer = M3D_RENDERER;
        CVector const org(n->m_currentWorldOrigin.x,
            n->m_currentWorldOrigin.y + (n->m_boundingBox.m_box[5] - n->m_boundingBox.m_box[2]) * 0.5f,
            n->m_currentWorldOrigin.z);
        CVector const screen = renderer->Project(org - renderer->MatGetOrgInv());
        M3D_APP->SetFont(CStr("Tahoma"), 11.0f, 0, M3D_APP->m_codePage.CodePage);
        renderer->PushFog(false);
        renderer->PushBlend(rend::BM_NONE);
        renderer->PushZbState(rend::ZB_DISABLE);
        CStr const strText = CStr(n->GetClassNameA()) + CStr(" ") + CStr(g_nodeNum);
        M3D_APP->DrawTextAbs(screen.x, screen.y, 0xFFFFFFFF, strText, 0, -1);
        renderer->PopZbState();
        renderer->PopBlend();
        renderer->PopFog();
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
            CMatrix invMat = n->m_currentXForm.getInverseRotTranslate();
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
        // RVA 0x63A850
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
                int const numNodes = m_visNumSlots[clsIdx];
                if (!numNodes)
                {
                    continue;
                }
                SgNode** const slots = &m_visSlots[2000 * clsIdx];
                DataServer* const server = slots[0]->GetServer();
                if (!server)
                {
                    continue;
                }
                if (server == &M3D_APP->GetAnimatedModelsServer() || server == &M3D_APP->GetParticlesServer())
                {
                    RenderNodeInfo rni;
                    rni.rnt = RNT_SIMPLE;
                    rni.isCullInverted = flags == SGRF_LOW_DETAIL;
                    rni.isUseImpostors = true;
                    rni.isPrimaryRender = flags != SGRF_LOW_DETAIL;
                    server->RenderNodeSet(slots, numNodes, rni);
                    continue;
                }

                // NOTE: the binary opens the batch with -2 and then, for every node, closes the previous
                // state with -3 and opens it again with -2, so the render state is set up once per node.
                server->RenderItem(-2, nullptr);
                for (int i = 0; i < numNodes; ++i)
                {
                    server->RenderItem(-3, nullptr);
                    server->RenderItem(-2, nullptr);
                    slots[i]->Render(NRF_DEFAULT, nullptr, lastFrameTime, frameStart);
                }
                server->RenderItem(-3, nullptr);
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
        // RVA 0x63C140
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

        // Per-class visibility slots: 64 node classes, up to 2000 nodes each.
        this->m_visSlots = new SgNode*[64 * 2000];
        this->m_visNumSlots = new int[64];
        this->m_visSlotsUnderwater = new SgNode*[64 * 2000];
        this->m_visNumSlotsUnderwater = new int[64];
        this->m_transparentNodes = new SgNode*[500];
        this->m_numTransparentNodes = 0;
        this->m_transparencyTest = new IsNodeTransparent;

        memset(this->m_visNumSlots, 0, 64 * sizeof(int));
        memset(this->m_visSlots, 0, 64 * 2000 * sizeof(SgNode*));
        memset(this->m_visNumSlotsUnderwater, 0, 64 * sizeof(int));
        memset(this->m_visSlotsUnderwater, 0, 64 * 2000 * sizeof(SgNode*));
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
        // RVA 0x63E640
        for (SgNode* ttl : m_ttledList)
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
        // RVA 0x6375B0
        float v5 = (float)m_owner->m_level->land_size * VISCELL_EDGE_LENGTH_6;
        float box[6] = {0};
        box[2] = 0.0;
        box[5] = v5;
        box[0] = 0.0;
        box[3] = v5;
        m_owner->GetLandscape().getMinMaxHeightForBox(box, 0.0);
        frusta.enableAll();
        enableVisibleCells_r(frusta, box, or);
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
        // RVA 0x63B480 - drops the node and its whole subtree from the pending transform updates.
        m_updateXFormList.erase(toDelete);
        std::vector<Object*> stack;
        stack.push_back(toDelete);
        while (!stack.empty())
        {
            Object* const current = stack.back();
            stack.pop_back();
            for (auto* child = static_cast<SgNode*>(current->GetFirstChild()); child;
                 child = static_cast<SgNode*>(child->GetNextSibling()))
            {
                m_updateXFormList.erase(child);
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
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
        auto const curSlot = this->m_visNumSlots[cls->m_index]++;
        *(&this->m_visSlots[2000 * cls->m_index] + curSlot) = n;
        n->m_frameVisible = curFrame;
        return 1;
    }

    void SceneGraph::RemoveNodeExceptRemoveIfFree(SgNode*& toRemove)
    {
        // RVA 0x63B9F0
        if (IsLinkedNode(toRemove))
        {
            UnlinkNode(toRemove);
        }
        if (Object* parent = toRemove->GetParent())
        {
            parent->RemoveChild(toRemove);
        }
        DeleteFromUpdateXFormList(toRemove);
        CheckNodeValidity(toRemove, "Check Two");
        // The binary calls the virtual deleting destructor directly, whatever the reference count.
        delete toRemove;
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

    void SceneGraph::DrawDetailedShadows(
        CVector const& pos,
        float radius,
        rend::TexHandle tex,
        retruxx::vector<Class*> const& classesToRender)
    {
        // RVA 0x8A8950 - everything close to the camera gets a second, much
        // sharper shadow pass of its own: one texture covering a single disc of
        // the world instead of one channel per landscape cell.
        auto const& cfg = M3D_ENGINE_CFG;

        int const maxIdx = pClient->GetWorld().m_level->land_size - 1;
        int x0 = static_cast<int>((pos.x - radius) * (1.0f / VISCELL_EDGE_LENGTH_6));
        int z0 = static_cast<int>((pos.z - radius) * (1.0f / VISCELL_EDGE_LENGTH_6));
        int x1 = static_cast<int>((pos.x + radius) * (1.0f / VISCELL_EDGE_LENGTH_6));
        int z1 = static_cast<int>((pos.z + radius) * (1.0f / VISCELL_EDGE_LENGTH_6));
        x0 = std::clamp(x0, 0, maxIdx);
        z0 = std::clamp(z0, 0, maxIdx);
        x1 = std::clamp(x1, 0, maxIdx);
        z1 = std::clamp(z1, 0, maxIdx);

        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        auto& lsc = m_owner->GetLandscape();
        unsigned const ls = pClient->GetWorld().m_level->land_size;

        if (!M3D_RENDERER->RenderToTexStart(tex, false))
        {
            return;
        }

        M3D_RENDERER->ClearViewport(rend::M3DCLEAR_C, 0);
        M3D_RENDERER->PushBlend(rend::BM_1_1);
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
        M3D_RENDERER->SetAlphaTest(cfg.m_g_shadowAlphaTest.GetI());
        M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
        M3D_RENDERER->PushFog(false);
        M3D_RENDERER->PushLighting(true);
        LightSwitchOffAllLights();
        M3D_RENDERER->PushAmbient();
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
        M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);

        CVector4 const light(m_owner->m_sunDir, 0.0f);

        CMatrix matProj;
        matProj.zero();
        matProj._11 = 2.0f / (radius * 2.0f);
        matProj._22 = 2.0f / (radius * -2.0f);
        matProj._33 = 0.000099999997f;
        matProj._43 = -0.000099999997f;
        matProj._44 = 1.0f;
        M3D_RENDERER->MatSetProj(matProj);

        float scale = radius * 2.0f;

        CVector at;
        at.x = pos.x;
        at.y = 0.0f;
        at.z = pos.z;
        CVector eye;
        eye.x = pos.x;
        eye.y = 5000.0f;
        eye.z = pos.z;
        CVector up;
        up.x = 0.0f;
        up.y = 0.0f;
        up.z = 1.0f;

        CMatrix matView;
        matView.lookAtLH(eye, at, up);
        M3D_RENDERER->MatSet(matView);
        // The detailed shadow lives entirely in the blue channel.
        M3D_RENDERER->SetAmbient(255u, false);

        for (int x = x0; x <= x1; ++x)
        {
            for (int z = z0; z <= z1; ++z)
            {
                for (size_t j = 0; j < classesToRender.size(); ++j)
                {
                    retruxx::set<SgNode*> nodes;
                    CollectShadowingNodes(nodes, classesToRender[j], x, z, 1, ls, curFrame);
                    if (nodes.empty())
                    {
                        continue;
                    }
                    auto* server = (*nodes.begin())->GetServer();
                    if (server != &M3D_APP->GetAnimatedModelsServer())
                    {
                        continue;
                    }

                    retruxx::vector<SgNode*> nodesVector;
                    retruxx::vector<CMatrix> matrVector;

                    CVector normal;
                    normal.x = 0.0f;
                    normal.y = 1.0f;
                    normal.z = 0.0f;
                    CPlane projectPlane;

                    for (SgNode* node : nodes)
                    {
                        // Only what actually reaches into the detailed disc is
                        // worth the sharper pass.
                        float const dx = node->m_originWorldAbsForSphere.x - pos.x;
                        float const dz = node->m_originWorldAbsForSphere.z - pos.z;
                        if (sqrtf(dx * dx + dz * dz) >= node->m_boundingRadius + node->m_boundingRadius + radius)
                        {
                            continue;
                        }

                        nodesVector.push_back(node);
                        matrVector.push_back(node->m_currentXForm);

                        CVector org;
                        org.x = node->m_currentWorldOrigin.x;
                        org.z = node->m_currentWorldOrigin.z;
                        org.y = lsc.GetHeight(org.x, org.z, -1, true);
                        projectPlane.fromPointNormal(org, normal);

                        CMatrix shadowMatr;
                        shadowMatr.shadow(light, projectPlane);
                        node->m_currentXForm = node->m_currentXForm * shadowMatr;
                    }

                    if (!nodesVector.empty())
                    {
                        RenderNodeInfo rni;
                        rni.rnt = RNT_FOR_SHADOW;
                        rni.isUseImpostors = true;
                        server->RenderNodeSet(&nodesVector.front(), nodesVector.size(), rni);
                    }

                    for (size_t k = 0; k < nodesVector.size(); ++k)
                    {
                        nodesVector[k]->m_currentXForm = matrVector[k];
                    }
                }
            }
        }

        M3D_RENDERER->PopAmbient();
        M3D_RENDERER->PopLighting();
        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopCull();
        M3D_RENDERER->SetAlphaTest(0);
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopZbState();

        if (cfg.m_g_shadowBlur.GetB())
        {
            // Blur in place: copy the render target aside and run the blur
            // shader back over it.
            M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
            M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
            M3D_RENDERER->PushFog(false);
            M3D_RENDERER->PushBlend(rend::BM_NONE);
            M3D_RENDERER->SetAlphaTest(0);
            M3D_RENDERER->TexCopy(m_texBlurShadow, tex);
            m_blurShadowShader->SetTexture(rend::IEffect::DiffMap0, &m_texBlurShadow);
            m_blurShadowShader->SetFloat(
                rend::IEffect::User_float_param, cfg.m_g_shadowBlurCoeff.GetF() * 0.000099999997f);
            M3D_RENDERER->DrawFullScreenQuad(m_blurShadowShader);
            M3D_RENDERER->PopBlend();
            M3D_RENDERER->PopFog();
            M3D_RENDERER->PopCull();
            M3D_RENDERER->PopZbState();
        }

        M3D_RENDERER->RenderToTexFinish();

        // Second half: modulate the landscape and the roads under the disc with
        // the texture that was just rendered.
        overlayStart();
        M3D_RENDERER->PushBlend(rend::BM_DCOLOR_0);
        M3D_RENDERER->SetAlphaTest(10);
        M3D_RENDERER->PushFog(false);
        M3D_RENDERER->SingleLayerStencilStart();

        m_lsDetailShadowShader->SetTexture(rend::IEffect::DiffMap0, &tex);
        scale = 1.0f / scale;
        M3D_RENDERER->MatGetOrgInv();

        retruxx::vector<unsigned> roadCells;

        CVector4 fac;
        fac.x = 0.0f;
        fac.y = 0.0f;
        fac.z = pClient->GetWorld().GetWeatherManager().GetShadowTransparencyFromWeather() * 0.0078125f;
        fac.w = 0.0f;
        m_lsDetailShadowShader->SetVector4(rend::IEffect::User_float4_param, fac);
        m_roadDetailShadowShader->SetVector4(rend::IEffect::User_float4_param, fac);

        CMatrix const resMatr = CalcLinearTransform(scale, pos.x, pos.z);
        m_lsDetailShadowShader->SetMatrix(rend::IEffect::User_float4x4_param, resMatr);
        m_roadDetailShadowShader->SetMatrix(rend::IEffect::User_float4x4_param, resMatr);

        for (int x = x0; x <= x1; ++x)
        {
            for (int z = z0; z <= z1; ++z)
            {
                if (static_cast<unsigned>(x) < ls && static_cast<unsigned>(z) < ls &&
                    (m_enableVisSpaceMask & m_enableMap[256 * z + x]) != 0)
                {
                    roadCells.push_back(x + (z << 16));
                }
            }
        }

        if (!roadCells.empty())
        {
            M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
            m_roadDetailShadowShader->SetTexture(rend::IEffect::DiffMap0, &tex);
            RoadInRadius2dTest const roadTest(pos, radius);
            pClient->GetWorld().GetRoadManager().RenderRoads(roadCells, RRT_FOR_DETAILED_SHADOW, &roadTest, false);
            M3D_RENDERER->PopCull();
        }

        M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
        for (int x = x0; x <= x1; ++x)
        {
            for (int z = z0; z <= z1; ++z)
            {
                if (static_cast<unsigned>(x) < ls && static_cast<unsigned>(z) < ls &&
                    (m_enableVisSpaceMask & m_enableMap[256 * z + x]) != 0)
                {
                    m_owner->GetLandscape().drawCellOverlayedShader(x, z, m_lsDetailShadowShader);
                }
            }
        }
        M3D_RENDERER->PopCull();

        M3D_RENDERER->PopFog();
        M3D_RENDERER->SingleLayerStencilFinish();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopZFunc();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopZBiasSlopeScale();
        M3D_RENDERER->PopZBias();
    }

    void SceneGraph::DrawShadowsToTexture(
        int* cis,
        int cnt,
        int size,
        rend::TexHandle tex,
        retruxx::vector<Class*> const& classesToRender,
        CVector& pos,
        float radius)
    {
        // RVA 0x8A7B00 - renders the flattened silhouette of everything standing
        // in the given cells into one channel each of the shared shadow texture.
        (void)pos;
        (void)radius;

        auto const& cfg = M3D_ENGINE_CFG;
        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        auto& lsc = m_owner->GetLandscape();
        unsigned const ls = pClient->GetWorld().m_level->land_size;

        M3D_RENDERER->SetWhiteTexture(0);
        M3D_RENDERER->SetWhiteTexture(1);
        M3D_RENDERER->SetWhiteTexture(2);
        M3D_RENDERER->SetWhiteTexture(3);

        if (!M3D_RENDERER->RenderToTexStart(tex, false))
        {
            return;
        }

        M3D_RENDERER->ClearViewport(rend::M3DCLEAR_C, 0);
        M3D_RENDERER->PushBlend(rend::BM_1_1);
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
        M3D_RENDERER->SetAlphaTest(cfg.m_g_shadowAlphaTest.GetI());
        M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
        M3D_RENDERER->PushFog(false);
        M3D_RENDERER->PushLighting(true);
        LightSwitchOffAllLights();
        M3D_RENDERER->PushAmbient();
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_DIFFUSE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
        M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);

        CVector4 const light(m_owner->m_sunDir, 0.0f);

        // A top down orthographic projection exactly one cell batch wide. The
        // depth range is enormous and almost flat - nothing is z tested here.
        CMatrix matProj;
        matProj.zero();
        matProj._11 = 2.0f / (static_cast<float>(size) * VISCELL_EDGE_LENGTH_6);
        matProj._22 = 2.0f / (static_cast<float>(-size) * VISCELL_EDGE_LENGTH_6);
        matProj._33 = 0.000099999997f;
        matProj._43 = -0.000099999997f;
        matProj._44 = 1.0f;
        M3D_RENDERER->MatSetProj(matProj);

        // One texel of margin all round, so filtering never bleeds between the
        // cell batches that share the texture.
        rend::Viewport viewPort;
        M3D_RENDERER->GetDims(tex, viewPort.m_width, viewPort.m_height);
        viewPort.m_width -= 2;
        viewPort.m_height -= 2;
        viewPort.m_x0 = 1;
        viewPort.m_y0 = 1;
        viewPort.m_zMin = 0.0f;
        viewPort.m_zMax = 1.0f;
        M3D_RENDERER->SetViewport(viewPort);

        if (cfg.m_testShadowActualRenderToTexture.GetB())
        {
            float const half = static_cast<float>(size) * 0.5f;
            for (int i = 0; i < cnt; ++i)
            {
                int const x = cis[2 * i];
                int const z = cis[2 * i + 1];

                CVector lookAt;
                lookAt.x = (static_cast<float>(x) + half) * VISCELL_EDGE_LENGTH_6;
                lookAt.y = 0.0f;
                lookAt.z = (static_cast<float>(z) + half) * VISCELL_EDGE_LENGTH_6;
                CVector lookFrom;
                lookFrom.x = lookAt.x;
                lookFrom.y = 5000.0f;
                lookFrom.z = lookAt.z;
                CVector up;
                up.x = 0.0f;
                up.y = 0.0f;
                up.z = 1.0f;

                CMatrix matView;
                matView.lookAtLH(lookFrom, lookAt, up);
                M3D_RENDERER->MatSet(matView);
                // Each cell of the batch owns one colour channel.
                M3D_RENDERER->SetAmbient(COLOR_MASK[i], false);

                for (size_t j = 0; j < classesToRender.size(); ++j)
                {
                    retruxx::set<SgNode*> nodes;
                    CollectShadowingNodes(nodes, classesToRender[j], x, z, size, ls, curFrame);
                    if (nodes.empty())
                    {
                        continue;
                    }
                    auto* server = (*nodes.begin())->GetServer();
                    if (server != &M3D_APP->GetAnimatedModelsServer())
                    {
                        continue;
                    }

                    retruxx::vector<SgNode*> nodesVector;
                    retruxx::vector<CMatrix> matrVector;

                    CVector normal;
                    normal.x = 0.0f;
                    normal.y = 1.0f;
                    normal.z = 0.0f;
                    CPlane projectPlane;

                    for (SgNode* node : nodes)
                    {
                        nodesVector.push_back(node);
                        matrVector.push_back(node->m_currentXForm);

                        // Flatten the node onto the ground under its own origin.
                        CVector org;
                        org.x = node->m_currentWorldOrigin.x;
                        org.z = node->m_currentWorldOrigin.z;
                        org.y = lsc.GetHeight(org.x, org.z, -1, true);
                        projectPlane.fromPointNormal(org, normal);

                        CMatrix shadowMatr;
                        shadowMatr.shadow(light, projectPlane);
                        node->m_currentXForm = node->m_currentXForm * shadowMatr;
                    }

                    if (!nodesVector.empty())
                    {
                        RenderNodeInfo rni;
                        rni.rnt = RNT_FOR_SHADOW;
                        rni.isUseImpostors = true;
                        server->RenderNodeSet(&nodesVector.front(), nodesVector.size(), rni);
                    }

                    for (size_t k = 0; k < nodesVector.size(); ++k)
                    {
                        nodesVector[k]->m_currentXForm = matrVector[k];
                    }
                }
            }
        }

        M3D_RENDERER->PopAmbient();
        M3D_RENDERER->PopLighting();
        M3D_RENDERER->SetAlphaTest(0);
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopFog();
        M3D_RENDERER->RenderToTexFinish();
    }

    void SceneGraph::PutShadowTextureToGrass(int* cis, int cnt, float fade0, float fade1, int size, rend::TexHandle tex)
    {
        // RVA 0x8A2EE0 - the grass is drawn after the landscape has already been
        // shadowed, so it gets its own pass through the same shadow texture.
        auto const& cfg = M3D_ENGINE_CFG;
        if (cfg.m_g_grassDrawDist.GetF() < 10.0f)
        {
            return;
        }

        unsigned const ls = pClient->GetWorld().m_level->land_size;
        auto& lsc = pClient->GetWorld().GetLandscape();

        m_grassShadowVs->Apply();
        m_grassShadowPs->Apply();

        overlayStart();
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
        M3D_RENDERER->SetAlphaTest(cfg.m_g_grassAlphatest.GetI());
        M3D_RENDERER->PushFog(false);

        CMatrix const viewMatrix = M3D_RENDERER->MatGet();
        CMatrix const viewProjMatrix = viewMatrix * M3D_RENDERER->MatGetProj();
        m_grassShadowVs->SetMatrix(m_grassShadowVs->GetParamHandleByName("mViewProj"), viewProjMatrix);

        CVector const viewPos = viewMatrix.getOrgInv();
        m_grassShadowVs->SetVector3(m_grassShadowVs->GetParamHandleByName("ViewPos"), viewPos);
        m_grassShadowVs->SetFloat(m_grassShadowVs->GetParamHandleByName("drawDist"), cfg.m_g_grassDrawDist.GetF());

        M3D_RENDERER->SetTexture(2, tex, -1.0);
        M3D_RENDERER->SetTextureParameter(tex, rend::TM_WRAP_S, 3u);
        M3D_RENDERER->SetTextureParameter(tex, rend::TM_WRAP_T, 3u);

        m_grassShadowVs->SetFloat(m_grassShadowVs->GetParamHandleByName("FadeStart"), fade0);
        m_grassShadowVs->SetFloat(m_grassShadowVs->GetParamHandleByName("FadeEnd"), fade1);

        CVector lightmapScale;
        lightmapScale.x = 1.0f / (static_cast<float>(m_owner->m_level->land_size) * VISCELL_EDGE_LENGTH_6);
        lightmapScale.y = -lightmapScale.x;
        lightmapScale.z = 0.0f;
        m_grassShadowVs->SetVector3(m_grassShadowVs->GetParamHandleByName("lightmapScale"), lightmapScale);

        int w = 0;
        int h = 0;
        M3D_RENDERER->GetDims(tex, w, h);
        float const sc = (1.0f - 4.0f / static_cast<float>(w)) / (static_cast<float>(size) * VISCELL_EDGE_LENGTH_6);
        M3D_RENDERER->MatGetOrgInv();

        float const half = static_cast<float>(size) * 0.5f;
        for (int i = 0; i < cnt; ++i)
        {
            unsigned const cellX = cis[2 * i];
            unsigned const cellZ = cis[2 * i + 1];

            float const transparency =
                pClient->GetWorld().GetWeatherManager().GetShadowTransparencyFromWeather() * 0.0078125f;
            unsigned const mask = COLOR_MASK[i];

            CVector4 fac;
            fac.x = (mask & 0xFF0000u) != 0 ? transparency : 0.0f;
            fac.y = (mask & 0x00FF00u) != 0 ? transparency : 0.0f;
            fac.z = (mask & 0x0000FFu) != 0 ? transparency : 0.0f;
            fac.w = 0.0f;
            m_grassShadowPs->SetVector4(m_grassShadowPs->GetParamHandleByName("TFactor"), fac);

            CMatrix const resMatr = CalcLinearTransform(
                sc,
                (static_cast<float>(static_cast<int>(cellX)) + half) * VISCELL_EDGE_LENGTH_6,
                (static_cast<float>(static_cast<int>(cellZ)) + half) * VISCELL_EDGE_LENGTH_6);
            m_grassShadowVs->SetMatrix(m_grassShadowVs->GetParamHandleByName("textureTransform"), resMatr);

            for (unsigned x = cellX; x < cellX + size; ++x)
            {
                for (unsigned z = cellZ; z < cellZ + size; ++z)
                {
                    if (x >= ls || z >= ls || (m_enableVisSpaceMask & m_enableMap[256 * z + x]) == 0)
                    {
                        continue;
                    }

                    unsigned numVisibleInstances = 0;
                    lsc.CollectGrassCell(x, z, numVisibleInstances, visGrassInstances, visModelsForGrassInstances);
                    lsc.RenderGrass(
                        numVisibleInstances, visGrassInstances, visModelsForGrassInstances, Landscape::RGT_FOR_SHADOW);
                }
            }
        }

        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopZFunc();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopZBiasSlopeScale();
        M3D_RENDERER->PopZBias();
        M3D_RENDERER->SetWhiteTexture(1);
    }

    void SceneGraph::PutShadowTextureToLandscapeAndRoad(
        int* cis,
        int cnt,
        float fade0,
        float fade1,
        int size,
        rend::TexHandle tex)
    {
        // RVA 0x8A7180 - modulates the already rendered landscape and road with
        // the shadow texture rendered by DrawShadowsToTexture.
        unsigned const ls = pClient->GetWorld().m_level->land_size;

        overlayStart();
        M3D_RENDERER->PushBlend(rend::BM_DCOLOR_0);
        M3D_RENDERER->SetAlphaTest(10);
        M3D_RENDERER->PushFog(false);

        m_roadShadowShader->SetTexture(rend::IEffect::DiffMap0, &tex);
        m_lsShadowShader->SetTexture(rend::IEffect::DiffMap0, &tex);
        m_roadShadowShader->SetFloat(rend::IEffect::User_float_param, fade0);
        m_roadShadowShader->SetFloat(rend::IEffect::User_float_param2, fade1);
        m_lsShadowShader->SetFloat(rend::IEffect::User_float_param, fade0);
        m_lsShadowShader->SetFloat(rend::IEffect::User_float_param2, fade1);

        int w = 0;
        int h = 0;
        M3D_RENDERER->GetDims(tex, w, h);
        // The outermost texel row is left out so that bilinear filtering never
        // samples across the edge of a cell.
        float const scale = (1.0f - 4.0f / static_cast<float>(w)) / (static_cast<float>(size) * VISCELL_EDGE_LENGTH_6);
        M3D_RENDERER->MatGetOrgInv();

        retruxx::vector<unsigned> oneCell(1u, 0u);

        float const half = static_cast<float>(size) * 0.5f;
        for (int i = 0; i < cnt; ++i)
        {
            unsigned const cellX = cis[2 * i];
            unsigned const cellZ = cis[2 * i + 1];

            float const transparency =
                pClient->GetWorld().GetWeatherManager().GetShadowTransparencyFromWeather() * 0.0078125f;
            unsigned const mask = COLOR_MASK[i];

            CVector4 fac;
            fac.x = (mask & 0xFF0000u) != 0 ? transparency : 0.0f;
            fac.y = (mask & 0x00FF00u) != 0 ? transparency : 0.0f;
            fac.z = (mask & 0x0000FFu) != 0 ? transparency : 0.0f;
            fac.w = 0.0f;
            m_roadShadowShader->SetVector4(rend::IEffect::User_float4_param, fac);
            m_lsShadowShader->SetVector4(rend::IEffect::User_float4_param, fac);

            CMatrix const resMatr = CalcLinearTransform(
                scale,
                (static_cast<float>(static_cast<int>(cellX)) + half) * VISCELL_EDGE_LENGTH_6,
                (static_cast<float>(static_cast<int>(cellZ)) + half) * VISCELL_EDGE_LENGTH_6);
            m_roadShadowShader->SetMatrix(rend::IEffect::User_float4x4_param, resMatr);
            m_lsShadowShader->SetMatrix(rend::IEffect::User_float4x4_param, resMatr);

            for (unsigned x = cellX; x < cellX + size; ++x)
            {
                for (unsigned z = cellZ; z < cellZ + size; ++z)
                {
                    if (x >= ls || z >= ls || (m_enableVisSpaceMask & m_enableMap[256 * z + x]) == 0)
                    {
                        continue;
                    }

                    // The stencil keeps the road and the landscape underneath it
                    // from being darkened twice.
                    M3D_RENDERER->SingleLayerStencilStart();
                    M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
                    oneCell.back() = x + (z << 16);
                    pClient->GetWorld().GetRoadManager().RenderRoads(oneCell, RRT_FOR_SHADOW, nullptr, false);
                    m_owner->GetLandscape().drawCellOverlayedShader(x, z, m_lsShadowShader);
                    M3D_RENDERER->PopCull();
                    M3D_RENDERER->SingleLayerStencilFinish();
                }
            }
        }

        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopZFunc();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopZBiasSlopeScale();
        M3D_RENDERER->PopZBias();
    }

    void SceneGraph::DrawShadows()
    {
        // RVA 0x8AA1E0
        auto const& cfg = M3D_ENGINE_CFG;
        if (!cfg.m_dsShadows.GetB() || !pClient->GetWorld().GetWeatherManager().GetShadowVisibilityFromWeather())
        {
            return;
        }

        ShadowStats shadowStats;

        // The shadows fade out well before the fog does, and never reach
        // further than the configured cell radius.
        float fadeStart = 0.0f;
        float fadeEnd = 0.0f;
        m_owner->GetLandscape().GetFogStartAndEnd(fadeStart, fadeEnd);
        fadeStart = fadeStart * 0.75f;
        fadeEnd = fadeEnd * 0.75f;
        if (FADE_START <= fadeStart)
        {
            fadeStart = FADE_START;
        }
        float const farDist = (static_cast<float>(cfg.m_g_shadowFarDist.GetI()) - 0.5f) * VISCELL_EDGE_LENGTH_6;
        if (farDist <= fadeEnd)
        {
            fadeEnd = farDist;
        }

        retruxx::vector<Class*> classes;
        classes.push_back(RT_CLASS_LOCAL(SgGameUnitNode));
        classes.push_back(RT_CLASS_LOCAL(SgAnimatedModelNode));

        for (int tg = 0; tg < 8; ++tg)
        {
            M3D_RENDERER->TgDisable(tg);
        }

        // The detailed shadow map is centred a little in front of the camera
        // rather than on it, so the whole radius stays in view.
        float const radius = cfg.m_g_shadowDetailRadius.GetF();
        CVector r;
        CVector u;
        CVector f;
        M3D_RENDERER->MatGetBasis(r, u, f);
        float const invLen = 1.0f / sqrtf(f.x * f.x + f.z * f.z + f.y * f.y + 1.1920929e-7f);
        float const ahead = radius + 1.0f;
        CVector const camOrgInv = M3D_RENDERER->MatGetOrgInv();
        CVector pos;
        pos.x = camOrgInv.x + f.x * invLen * ahead;
        pos.y = camOrgInv.y + f.y * invLen * ahead;
        pos.z = camOrgInv.z + f.z * invLen * ahead;

        m_lsDetailShadowShader->SetVector3(rend::IEffect::User_float3_param, pos);
        m_lsDetailShadowShader->SetFloat(rend::IEffect::User_float_param3, radius);
        m_roadDetailShadowShader->SetVector3(rend::IEffect::User_float3_param, pos);
        m_roadDetailShadowShader->SetFloat(rend::IEffect::User_float_param3, radius);
        m_lsShadowShader->SetVector3(rend::IEffect::User_float3_param, pos);
        m_lsShadowShader->SetFloat(rend::IEffect::User_float_param3, radius);
        m_roadShadowShader->SetVector3(rend::IEffect::User_float3_param, pos);
        m_roadShadowShader->SetFloat(rend::IEffect::User_float_param3, radius);

        DrawDetailedShadows(pos, radius, m_detTexShadow, classes);

        // Collect every visible cell within the shadow radius. SortedCellsFetch
        // walks outwards ring by ring, so pushing to the front leaves the list
        // roughly far-to-near before the exact sort below.
        retruxx::deque<retruxx::pair<int, int>> cls;
        m_sortedCellsEndRadius = cfg.m_g_shadowFarDist.GetI();
        m_sortedCellsCurCell = 0;
        m_sortedCellsCurRadius = 0;
        {
            int cellX = 0;
            int cellY = 0;
            int vis = 0;
            int rad = 0;
            while (SortedCellsFetch(cellX, cellY, vis, rad))
            {
                if (vis)
                {
                    cls.push_front(retruxx::pair<int, int>(cellX, cellY));
                }
            }
        }

        // Only the camera's ground position is used; the height is explicitly
        // zeroed, which is what makes the y term of the comparator dead.
        CVector const camOrg = M3D_RENDERER->MatGetOrgInv();
        CVector camPos;
        camPos.x = camOrg.x;
        camPos.y = 0.0f;
        camPos.z = camOrg.z;
        std::sort(cls.begin(), cls.end(), SortCells(camPos));

        pClient->GetWorld().GetLandscape().RenderGrass(cls);

        // Cells are shadowed three at a time - one per colour channel of the
        // shared shadow texture.
        size_t off = 0;
        while (off != cls.size())
        {
            int cnt = static_cast<int>(cls.size() - off);
            if (cnt >= CELLS_PER_SHADOW_TEXTURE)
            {
                cnt = CELLS_PER_SHADOW_TEXTURE;
            }

            int cis[2 * CELLS_PER_SHADOW_TEXTURE];
            for (int k = 0; k < cnt; ++k)
            {
                cis[2 * k] = cls[off + k].first;
                cis[2 * k + 1] = cls[off + k].second;
            }
            off += cnt;

            rend::RenderStats rsBefore{};
            rend::RenderStats rsAfter{};

            M3D_RENDERER->GetStats(rsBefore);
            if (cfg.m_testShadowRenderToTexture.GetB())
            {
                DrawShadowsToTexture(cis, cnt, 1, m_texShadow, classes, pos, radius);
            }
            M3D_RENDERER->GetStats(rsAfter);
            shadowStats.numToTexPolys += rsAfter.polyCount - rsBefore.polyCount;
            shadowStats.numToTexDIPs += rsAfter.DIPs - rsBefore.DIPs;
            shadowStats.numCells += cnt;

            M3D_RENDERER->GetStats(rsBefore);
            if (cfg.m_testShadowRenderToLandscape.GetB())
            {
                PutShadowTextureToLandscapeAndRoad(cis, cnt, fadeStart, fadeEnd, 1, m_texShadow);
            }
            if (cfg.m_testShadowRenderToGrass.GetB())
            {
                PutShadowTextureToGrass(cis, cnt, fadeStart, fadeEnd, 1, m_texShadow);
            }
            M3D_RENDERER->GetStats(rsAfter);
            shadowStats.numPutPolys += rsAfter.polyCount - rsBefore.polyCount;
            shadowStats.numPutDIPs += rsAfter.DIPs - rsBefore.DIPs;
        }

        if (cfg.m_g_showShadowsStats.GetB())
        {
            M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
            M3D_APP->SetFont("Lucida Console", 10.0f, 1u, M3D_APP->m_codePage.CodePage);

            CStr statStr;
            statStr.format("%-12s %8d", "shadowPutDIPS", shadowStats.numPutDIPs);
            M3D_APP->DrawTextRel(724.0f, 224.0f, 0xFF888888u, statStr, 0, -1);
            statStr.format("%-12s %8d", "shadowPutPolys", shadowStats.numPutPolys);
            M3D_APP->DrawTextRel(724.0f, 236.0f, 0xFF888888u, statStr, 0, -1);
            statStr.format("%-12s %8d", "shadowToTexDIPs", shadowStats.numToTexDIPs);
            M3D_APP->DrawTextRel(724.0f, 248.0f, 0xFF888888u, statStr, 0, -1);
            statStr.format("%-12s %8d", "shadowToTexPolys", shadowStats.numToTexPolys);
            M3D_APP->DrawTextRel(724.0f, 260.0f, 0xFF888888u, statStr, 0, -1);
            statStr.format("%-12s %8d", "shadowCells", shadowStats.numCells);
            M3D_APP->DrawTextRel(724.0f, 272.0f, 0xFF888888u, statStr, 0, -1);

            M3D_RENDERER->PopZbState();
        }
    }

    void SceneGraph::enableCellsSetRect(int* rc, unsigned orValue, unsigned andValue)
    {
        // RVA 0x633D90 - rc is {x0, z0, x1, z1} with exclusive upper bounds.
        for (int z = rc[1]; z < rc[3]; ++z)
        {
            unsigned char* const row = &m_enableMap[256 * z];
            for (int x = rc[0]; x < rc[2]; ++x)
            {
                row[x] = static_cast<unsigned char>((row[x] & andValue) | orValue);
            }
        }
    }

    void SceneGraph::enableCellsSetRect(float* rc, unsigned v0, unsigned v1)
    {
        // RVA 0x633DF0 - rc is an Aabb {minX, minY, minZ, maxX, maxY, maxZ} in world units.
        // NOTE: the cell rectangle is clamped to land_size from above only, never to 0 from below.
        float const invCell = 1.0f / VISCELL_EDGE_LENGTH_6;
        int const landSize = m_owner->m_level->land_size;
        int rrc[4] = {
            std::min(static_cast<int>(rc[0] * invCell), landSize),
            std::min(static_cast<int>(rc[2] * invCell), landSize),
            std::min(static_cast<int>(rc[3] * invCell), landSize),
            std::min(static_cast<int>(rc[5] * invCell), landSize),
        };
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
        // RVA 0x63A470
        if (n->m_isWaitingForRender)
        {
            return 0;
        }

        // An invisible node still lets its subtree through when it has a sound child, which must keep playing.
        int visible = AddOneNodeToRender(n, frusta, curFrame);
        if (!visible)
        {
            n->GetProperty(PROP_NODE_HAVE_CHILDSOUND, &visible);
        }
        if (!visible)
        {
            return 1;
        }

        std::vector<Object*> stack;
        stack.push_back(n);
        while (!stack.empty())
        {
            Object* const current = stack.back();
            stack.pop_back();
            for (auto* child = static_cast<SgNode*>(current->GetFirstChild()); child;
                 child = static_cast<SgNode*>(child->GetNextSibling()))
            {
                AddOneNodeToRender(child, frusta, curFrame);
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }
        }
        return 1;
    }

    void SceneGraph::DrawStencilShadows()
    {
        // RVA 0x8A9ED0 - hands the (up to 100) nearest animated models and game units within the shadow distance to
        // the stencil shadow renderer. NOTE: the fade distances are worked out but never used.
        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame() - 1;
        float fogStart;
        float fogEnd;
        m_owner->GetLandscape().GetFogStartAndEnd(fogStart, fogEnd);
        fogStart *= 0.75f;
        fogEnd *= 0.75f;
        fogStart = FADE_START <= fogStart ? FADE_START : fogStart;
        float const farDist = static_cast<float>(M3D_ENGINE_CFG.m_g_shadowFarDist.GetI()) * VISCELL_EDGE_LENGTH_6;
        fogEnd = farDist <= fogEnd ? farDist : fogEnd;
        unsigned const ls = pClient->GetWorld().m_level->land_size;
        if (!M3D_ENGINE_CFG.m_g_stencilShadows.GetB())
        {
            return;
        }

        retruxx::vector<Class*> classes;
        classes.push_back(RT_CLASS_LOCAL(SgAnimatedModelNode));
        classes.push_back(RT_CLASS_LOCAL(SgGameUnitNode));
        retruxx::set<SgNode*> nodes;
        m_sortedCellsEndRadius = M3D_ENGINE_CFG.m_g_shadowFarDist.GetI();
        m_sortedCellsCurCell = 0;
        m_sortedCellsCurRadius = 0;
        int x;
        int z;
        int v;
        int radius;
        while (SortedCellsFetch(x, z, v, radius))
        {
            if (v)
            {
                CollectShadowingNodesStencil(nodes, classes[0], x, z, ls, curFrame);
                CollectShadowingNodesStencil(nodes, classes[1], x, z, ls, curFrame);
            }
        }

        int numNodes = static_cast<int>(nodes.size());
        if (numNodes)
        {
            retruxx::vector<SgNode*> shadowNodes(nodes.begin(), nodes.end());
            CVector const cameraPos = M3D_RENDERER->MatGetOrgInv();
            // ShadowNodesSortPred: nearest to the camera first.
            std::sort(shadowNodes.begin(), shadowNodes.end(), [&cameraPos](SgNode const* node1, SgNode const* node2) {
                float const d1z = node1->m_currentWorldOrigin.z - cameraPos.z;
                float const d1y = node1->m_currentWorldOrigin.y - cameraPos.y;
                float const d1x = node1->m_currentWorldOrigin.x - cameraPos.x;
                float const d2y = node2->m_currentWorldOrigin.y - cameraPos.y;
                float const d2z = node2->m_currentWorldOrigin.z - cameraPos.z;
                float const d2x = node2->m_currentWorldOrigin.x - cameraPos.x;
                return d2z * d2z + d2y * d2y + d2x * d2x > d1z * d1z + d1y * d1y + d1x * d1x;
            });
            if (numNodes > 100)
            {
                numNodes = 100;
            }
            M3D_APP->GetAnimatedModelsServer().RenderShadowVolumesSet(shadowNodes.data(), numNodes);
        }
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
        // RVA 0x635060 - quadtree walk over the landscape: cells in fully visible boxes get orFlags, cells in
        // invisible boxes lose it, and partially visible boxes are split in four until they are one cell wide.
        int const test = frusta.testBBox(tbFullTest, box, CVector(0.0f, 0.0f, 0.0f));
        if (test == 0)
        {
            enableCellsSetRect(box, 0, ~orFlags);
            return;
        }
        if (test == 2)
        {
            enableCellsSetRect(box, orFlags, 0xFFFFFFFF);
            return;
        }

        unsigned int const clipSave = frusta.m_enabled;
        frusta.enableUpdateFromBox(box, CVector(0.0f, 0.0f, 0.0f));

        float const maxCoord = static_cast<float>(m_owner->m_level->land_size) * VISCELL_EDGE_LENGTH_6;
        box[3] = std::min(box[3], maxCoord);
        box[5] = std::min(box[5], maxCoord);
        float const width = box[3] - box[0];
        float const depth = box[5] - box[2];
        if (VISCELL_EDGE_LENGTH_6 >= width || VISCELL_EDGE_LENGTH_6 >= depth)
        {
            // NOTE: this path returns without restoring frusta.m_enabled, which enableUpdateFromBox changed above.
            enableCellsSetRect(box, orFlags, 0xFFFFFFFF);
            return;
        }

        // Each half is rounded up to whole cells.
        float const invCell = 1.0f / VISCELL_EDGE_LENGTH_6;
        float const halfX = VISCELL_EDGE_LENGTH_6 * std::ceil(width * 0.5f * invCell);
        float const halfZ = VISCELL_EDGE_LENGTH_6 * std::ceil(depth * 0.5f * invCell);
        Landscape& landscape = m_owner->GetLandscape();
        // The quadrants in the binary's order; y (min/max height) is filled in by getMinMaxHeightForBox.
        float const quadrants[4][2] = {
            {box[0], box[2]},
            {box[0] + halfX, box[2]},
            {box[0] + halfX, box[2] + halfZ},
            {box[0], box[2] + halfZ},
        };
        float newBox[6] = {};
        for (auto const& q : quadrants)
        {
            newBox[0] = q[0];
            newBox[2] = q[1];
            newBox[3] = std::min(q[0] + halfX, maxCoord);
            newBox[5] = std::min(q[1] + halfZ, maxCoord);
            landscape.getMinMaxHeightForBox(newBox, 0.0f);
            enableVisibleCells_r(frusta, newBox, orFlags);
        }
        frusta.m_enabled = clipSave;
    }

    void ObjectsContainer::AddObject(m3d::Object* obj)
    {
        auto clz = obj->GetClass();
        m_objectsByClassIdx[clz->m_index].push_back(obj);
    }

    void ObjectsContainer::RemoveObject(m3d::Object* obj)
    {
        // RVA 0x639680 - unlinks the first occurrence; the object itself is not freed.
        auto& objs = m_objectsByClassIdx[obj->GetClass()->m_index];
        auto const it = std::find(objs.begin(), objs.end(), obj);
        if (it == objs.end())
        {
            M3D_LOG_INFO(CStr("Warning, object not found: ") + CStr(obj->GetName()));
            return;
        }
        objs.erase(it);
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

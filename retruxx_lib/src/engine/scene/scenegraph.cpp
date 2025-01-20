#include <stdexcept>
#include <scene/scenegraph.h>

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "world.h"
#include "level.h"

namespace
{
    float const VISCELL_EDGE_LENGTH_6 = 128.0;
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

    void SceneGraph::LinkNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
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

    void SceneGraph::UnlinkThinkNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float SceneGraph::GetAlphaForNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::UpdateThinkNodes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::DumpRenderingNodesInfoForClass(const Class*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNode* SceneGraph::GetRootNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNode const* SceneGraph::GetRootNode() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::RelinkNode(SgNode*, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::CollectNodesLight(retruxx::set<SgNode*>&, unsigned, unsigned, CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::InsertInRemoveIfFree(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SceneGraph::SortedCellsStartFetching(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::RefreshObjectsInRect(int, int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SceneGraph::SortedCellsFetch(int&, int&, int&, int&)
    {
        throw retruxx::logic_error("Not implemented");
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

    bool SceneGraph::IsLinkedNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
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

    void SceneGraph::LightSetupLightsForNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::Update()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::RenderDebugForNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::RenderContouredNodes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::UpdateAllXForms()
    {
        throw retruxx::logic_error("Not implemented");
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

    void SceneGraph::Render(SgRenderFlags)
    {
        throw retruxx::logic_error("Not implemented");
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
        //this->m_visSlots = m3d::g_Kernel->g_mar.AllocMem(512000, 0, 0);
        //this->m_visNumSlots = m3d::g_Kernel->g_mar.AllocMem(256, 0, 0);
        //this->m_visSlotsUnderwater = m3d::g_Kernel->g_mar.AllocMem(512000, 0, 0);
        //this->m_visNumSlotsUnderwater = m3d::g_Kernel->g_mar.AllocMem(256, 0, 0);
        //this->m_transparentNodes = m3d::g_Kernel->g_mar.AllocMem(2000, 0, 0);
        this->m_numTransparentNodes = 0;
        this->m_transparencyTest = new IsNodeTransparent;
        //memset(this->m_visNumSlots, 0, 0x100u);
        //memset(this->m_visSlots, 0, 0x7D000u);
        //memset(this->m_visNumSlotsUnderwater, 0, 0x100u);
        //memset(this->m_visSlotsUnderwater, 0, 0x7D000u);
        //memset(this->m_transparentNodes, 0, 0x7D0u);
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
            throw retruxx::logic_error("Not implemented");
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

    void SceneGraph::InsertInUpdateXFormList(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::SetVisMask(unsigned char)
    {
        throw retruxx::logic_error("Not implemented");
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

    void SceneGraph::SetTransparencyTest(IsNodeTransparent*)
    {
        throw retruxx::logic_error("Not implemented");
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

    void SceneGraph::LinkThinkNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
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

    int SceneGraph::AddOneNodeToRender(SgNode*, CClipper const&, int)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::enableCellsSetRect(int*, unsigned, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::enableCellsSetRect(float*, unsigned, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNode* SceneGraph::TraceLineThruCellNodesForClass(float&, int, int, CVector const&, CVector const&, Class*, retruxx::set<SgNode*>&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SceneGraph::AddNodeAndItsChildrenToRender(SgNode*, CClipper const&, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::DrawStencilShadows()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SceneGraph::IsTransparent(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SceneGraph::enableVisibleCells_r(CClipper&, float*, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }
}

#include <stdexcept>
#include <scene/scenegraph.h>

#include "config.h"
#include "m3dapp.h"
#include "core/kernel.h"

namespace m3d
{
    float IsNodeTransparent::getTransparentRadius()
    {
        throw std::logic_error("Not implemented");
    }

    bool IsNodeTransparent::setPermanentTransparency(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    bool IsNodeTransparent::test(SgNode*, float)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::LinkNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::SortedCellsPrepare()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::UnlinkAndDeleteAll()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::UnlinkThinkNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    float SceneGraph::GetAlphaForNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::UpdateThinkNodes()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DumpRenderingNodesInfoForClass(const Class*)
    {
        throw std::logic_error("Not implemented");
    }

    SgNode* SceneGraph::GetRootNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgNode const* SceneGraph::GetRootNode() const
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::RelinkNode(SgNode*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::CollectNodesLight(std::set<SgNode*>&, unsigned, unsigned, CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::InsertInRemoveIfFree(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    bool SceneGraph::SortedCellsStartFetching(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::RefreshObjectsInRect(int, int, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int SceneGraph::SortedCellsFetch(int&, int&, int&, int&)
    {
        throw std::logic_error("Not implemented");
    }

    int SceneGraph::SortedCellsFetch(int&, int&, int&)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::LightSwitchOffAllLights()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::UnlinkNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    SgNode* SceneGraph::GetNodeByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool SceneGraph::IsLinkedNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    bool SceneGraph::IsCellVisible(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DeleteFromRemoveIfFree(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    SgNode* SceneGraph::TraceLine(CVector&, CVector const&, CVector const&, std::set<Class*> const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::LightSetupLightsForNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::Update()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::RenderDebugForNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::RenderContouredNodes()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::UpdateAllXForms()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DeleteFromTtlList(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int SceneGraph::IsCellEnabled(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::RenderNode(SgNode*, CMatrix const&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::Render(SgRenderFlags)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::GetNodeNamesHierarchy(SgNode*, std::vector<CStr>&)
    {
        throw std::logic_error("Not implemented");
    }

    SceneGraph::SceneGraph()
    {
        this->m_noModelCull = 0;
        this->m_bIsInUnlinkAndDeleteAll = 0;
        this->m_bIsPurgingRemoveIfFree = 0;
        this->m_easyRelink = 0;
        this->m_roadProjectorShader = m3d::Application::g_pApp->m_renderer->NewEffect( "data/shaders/roadProjector.fx", true);
        M3D_ASSERT(m_roadProjectorShader);
        this->m_roadProjectorShader->SetDefaultTechnique(true);
        this->m_lsProjectorShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/lsProjector.fx", true);
        M3D_ASSERT(m_lsProjectorShader);
        this->m_lsProjectorShader->SetDefaultTechnique(true);
        this->m_objProjectorShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/objectProjector.fx", true);
        M3D_ASSERT(m_objProjectorShader);
        this->m_objProjectorShader->SetDefaultTechnique(true);
        this->m_treeProjectorShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/treeProjector.fx", true);
        M3D_ASSERT(m_treeProjectorShader);
        this->m_treeProjectorShader->SetDefaultTechnique(true);
        this->m_lsLightShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/lsLight.fx", true);
        M3D_ASSERT(m_lsLightShader);
        this->m_lsLightShader->SetDefaultTechnique(true);
        this->m_roadLightShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/roadLight.fx", true);
        M3D_ASSERT(m_roadLightShader);
        this->m_roadLightShader->SetDefaultTechnique(true);
        this->m_objectLightShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/objectlight.fx", true);
        M3D_ASSERT(m_objectLightShader);
        this->m_objectLightShader->SetDefaultTechnique(true);
        this->m_treeLightShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/treeLight.fx", true);
        M3D_ASSERT(m_treeLightShader);
        this->m_treeLightShader->SetDefaultTechnique(true);
        this->m_roadSpriteShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/roadSprite.fx", true);
        M3D_ASSERT(m_roadSpriteShader);
        this->m_roadSpriteShader->SetDefaultTechnique(true);
        this->m_texShadow = m3d::Application::g_pApp->m_renderer->AddDynamicTexture(
            "$TexShadow",
            g_Kernel->GetEngineCfg().m_lgtShadowTexSz.GetI(),
            g_Kernel->GetEngineCfg().m_lgtShadowTexSz.GetI(), 6);
        this->m_detTexShadow = m3d::Application::g_pApp->m_renderer->AddDynamicTexture(
            "$DetTexShadow",
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(),
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(), 6);
        this->m_texBlurShadow = m3d::Application::g_pApp->m_renderer->AddDynamicTexture(
            "$TexBlurShadow",
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(),
            g_Kernel->GetEngineCfg().m_detShadowTexSz.GetI(), 6);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(
            this->m_texBlurShadow,
            rend::TM_TEX_FILTER, 5u);
        this->m_lsShadowShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/lsShadows.fx", true);
        M3D_ASSERT(m_lsShadowShader);
        this->m_lsShadowShader->SetDefaultTechnique(true);
        this->m_roadShadowShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/roadShadows.fx", true);
        M3D_ASSERT(m_roadShadowShader);
        this->m_roadShadowShader->SetDefaultTechnique(true);
        this->m_lsDetailShadowShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/lsDetailedShadows.fx", true);
        M3D_ASSERT(m_lsDetailShadowShader);
        this->m_lsDetailShadowShader->SetDefaultTechnique(true);
        this->m_roadDetailShadowShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/roadDetailedShadows.fx", true);
        M3D_ASSERT(m_roadDetailShadowShader);
        this->m_roadDetailShadowShader->SetDefaultTechnique(true);
        this->m_shadowShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/shadow.fx", true);
        M3D_ASSERT(m_shadowShader);
        this->m_shadowShader->SetDefaultTechnique(true);
        this->m_blurShadowShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/blurShadow.fx", true);
        M3D_ASSERT(m_blurShadowShader);
        this->m_blurShadowShader->SetDefaultTechnique(true);
        this->m_grassShadowVs = m3d::Application::g_pApp->m_renderer->NewHlslShader("data/shaders/grassShadows.vs", "GrassVS", rend::IHlslShader::VS_1_1);
        M3D_ASSERT(m_grassShadowVs);
        this->m_grassShadowPs = m3d::Application::g_pApp->m_renderer->NewHlslShader("data/shaders/grassShadows.ps", "GrassPS", rend::IHlslShader::PS_1_1);
        M3D_ASSERT(m_grassShadowPs);
        this->m_contourShader = m3d::Application::g_pApp->m_renderer->NewEffect("data/shaders/contour.fx", true);
        M3D_ASSERT(m_contourShader);
        this->m_contourShader->SetDefaultTechnique(true);
        this->m_rootNode.m_isRootNode = 1;
        //TODO: important! UpdateXForm
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

    void SceneGraph::UpdateVis(bool, CClipper const&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DeleteFromContourList(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    SgNode* SceneGraph::GetNodeByNamesHierarchy(std::vector<CStr> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::InsertInUpdateXFormList(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::SetVisMask(unsigned char)
    {
        throw std::logic_error("Not implemented");
    }

    bool SceneGraph::IsInUnlinkAndDeleteAll() const
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DeleteAllTtledNodes()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::SetModelForceNoCull(bool)
    {
        throw std::logic_error("Not implemented");
    }

    ObjectsContainer const& SceneGraph::GetCellObjs(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::CollectNodesProjector(std::set<SgNode*>&, unsigned, unsigned, CClipper const&)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::EnableVisibleCells(CClipper&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::RemoveNode(SgNode*&)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DeleteAllRemoveIfFreeNodes()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DumpToFile(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::LightSetupSunForWorld()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::UpdateTexShadowSizes()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::SetOwner(CWorld* world)
    {
        m_owner = world;
    }

    SceneGraph::~SceneGraph()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::SetTransparencyTest(IsNodeTransparent*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::InsertInContourList(SgNode*, unsigned, float)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::CheckNodeIsNotInAnyList(SgNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::InsertInTtlList(SgNode*, int)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::LinkThinkNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DeleteFromUpdateXFormList(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::GetCellsStatistic(std::vector<CellInfo>*)
    {
        throw std::logic_error("Not implemented");
    }

    SceneGraph::CellItems& SceneGraph::GetCellItems(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    SceneGraph::CellItems const& SceneGraph::GetCellItems(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::CollectShadowingNodesStencil(std::set<SgNode*>&, Class*, int, int, unsigned, int)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::CollectShadowingNodes(std::set<SgNode*>&, Class*, int, int, int, unsigned, int)
    {
        throw std::logic_error("Not implemented");
    }

    int SceneGraph::AddOneNodeToRender(SgNode*, CClipper const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::RemoveNodeExceptRemoveIfFree(SgNode*&)
    {
        throw std::logic_error("Not implemented");
    }

    int SceneGraph::getYOfs(int)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::EnsureEverythingIsUnlinked() const
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DrawShadows()
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::enableCellsSetRect(int*, unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::enableCellsSetRect(float*, unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    SgNode* SceneGraph::TraceLineThruCellNodesForClass(float&, int, int, CVector const&, CVector const&, Class*, std::set<SgNode*>&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int SceneGraph::AddNodeAndItsChildrenToRender(SgNode*, CClipper const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::DrawStencilShadows()
    {
        throw std::logic_error("Not implemented");
    }

    bool SceneGraph::IsTransparent(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void SceneGraph::enableVisibleCells_r(CClipper&, float*, unsigned)
    {
        throw std::logic_error("Not implemented");
    }
}

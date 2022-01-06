#include <stdexcept>
#include <scene/scenegraph.h>

namespace m3d
{
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
        throw std::logic_error("Not implemented");
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

    void SceneGraph::SetOwner(CWorld*)
    {
        throw std::logic_error("Not implemented");
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

#pragma once
#include "nodes/sgnode.h"
#include <list>
#include <set>
#include <vector>
#include <math/point2d.h>
#include <renderer/i_renderer.h>

namespace ai
{
    class CompositeObj;
    class JointedObj;
    class PhysicBody;
}  // namespace ai

class CClipper;

namespace m3d
{
    class CWorld;

    enum SgRenderFlags
    {
        SGRF_DEFAULT_OPAQUE = 0x0,
        SGRF_DEFAULT_TRANS = 0x1,
        SGRF_SHADOWS = 0x2,
        SGRF_LOW_DETAIL = 0x3,
        SGRF_OVERLAYS = 0x4,
        SGRF_REFRACTION = 0x5,
    };

    class IsNodeTransparent
    {
    public:
        virtual bool test(m3d::SgNode* node, float distCoeff);
        virtual float getTransparentRadius();
        virtual bool setPermanentTransparency(m3d::SgNode* node);
    };

    class ObjectsContainer
    {
    private:
        retruxx::list<m3d::Object*> m_objectsByClassIdx[64];

    public:
        void AddObject(m3d::Object* obj);
        void RemoveObject(m3d::Object* obj);
        retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>>* GetObjectsByClass(m3d::Class* cl);
        retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>> const* GetObjects() const;
        retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*>>* GetObjects();
        bool empty() const;
    };

    struct GraphItemsForSgNode
    {
        PointBase<int> m_cellsCoveredPoint0;
        PointBase<int> m_cellsCoveredPoint1;
        retruxx::vector<unsigned int, retruxx::allocator<unsigned int>> m_cellsShadowCovered;

        bool bSomeCellsCovered() const;
        void ClearCellsCovered();
    };

    class SceneGraph
    {
        friend class CWorld;
        // ai::CompositeObj takes its node off the think list once the pieces drive it.
        friend class ai::CompositeObj;
        friend class ai::JointedObj;
        // PhysicBody::SetAnimationStopped moves nodes in and out of the think list.
        friend class ai::PhysicBody;
        friend class LightsServer;
        // ProjectorsServer::RenderItem walks the sorted cells and sets up the projector shaders.
        friend class ProjectorsServer;

    public:
        SceneGraph(m3d::SceneGraph const&);
        SceneGraph();
        ~SceneGraph();
        void InsertInTtlList(m3d::SgNode* toInsert, int frameToDie);
        void DeleteAllTtledNodes();
        void DeleteFromTtlList(m3d::SgNode* toDelete);
        void LinkNode(m3d::SgNode* toLink);
        void InsertInRemoveIfFree(m3d::SgNode* toInsert);
        void DeleteFromRemoveIfFree(m3d::SgNode* toDelete);
        void DeleteAllRemoveIfFreeNodes();
        void InsertInContourList(m3d::SgNode* toInsert, unsigned int color, float width);
        void DeleteFromContourList(m3d::SgNode* toDelete);
        void RelinkNode(m3d::SgNode* toRelink, bool bForceRelink);
        void UnlinkNode(m3d::SgNode* toUnlink);
        void UnlinkAndDeleteAll();
        bool IsLinkedNode(m3d::SgNode* toCheck);
        void RemoveNode(m3d::SgNode*& toRemove);
        void LinkThinkNode(m3d::SgNode* toThink);
        void UnlinkThinkNode(m3d::SgNode* toThink);
        void InsertInUpdateXFormList(m3d::SgNode* toInsert);
        void DeleteFromUpdateXFormList(m3d::SgNode* toDelete);
        void CheckNodeIsNotInAnyList(m3d::SgNode* sgNode) const;
        void UpdateThinkNodes();
        void UpdateAllXForms();
        void Update();
        void UpdateVis(bool newFrame, CClipper const& frusta, bool primary);
        void SortNodesForRender(int);
        void Render(m3d::SgRenderFlags flags);
        void RenderDebugForNode(m3d::SgNode* n);
        void RenderNode(m3d::SgNode* n, CMatrix const& curMatr, bool fullInv);
        void RenderContouredNodes();
        void SetOwner(m3d::CWorld* w);
        m3d::CWorld* GetOwner();
        m3d::SgNode* TraceLine(
            CVector& hit,
            CVector const& start,
            CVector const& finish,
            retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const& cl0,
            unsigned int traceMode);
        int TraceRect(
            retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>& dest,
            CVector2 const& v0,
            CVector2 const& v1,
            retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const&
                affectClasses);
        void SetModelForceNoCull(bool t);
        void LightSwitchOffAllLights();
        void LightSetupLightsForNode(m3d::SgNode* node);
        void LightSetupSunForWorld();
        m3d::SgNode const* GetRootNode() const;
        m3d::SgNode* GetRootNode();
        m3d::SgNode* GetNodeByNamesHierarchy(retruxx::vector<CStr, retruxx::allocator<CStr>> const& hierarchy);
        void GetNodeNamesHierarchy(m3d::SgNode* node, retruxx::vector<CStr, retruxx::allocator<CStr>>& hierarchy);
        int IsCellEnabled(int x, int y);
        void SortedCellsPrepare();
        bool SortedCellsStartFetching(int radius0, int radius1);
        int SortedCellsFetch(int& cellX, int& cellY, int& vis, int& radius);
        int SortedCellsFetch(int& cellX, int& cellY, int& vis);
        void EnableVisibleCells(CClipper& frusta, unsigned int or);
        void SetVisMask(unsigned char vis);
        void RefreshObjectsInRect(int cx0, int cy0, int cx1, int cy1);
        float GetAlphaForNode(m3d::SgNode* n);
        void SetTransparencyTest(m3d::IsNodeTransparent* t);
        void DumpToFile(CStr const& filename) const;
        m3d::SgNode* GetNodeByName(CStr const& name);
        m3d::ObjectsContainer const& GetCellObjs(int x, int z) const;
        bool IsCellVisible(int x, int z) const;
        m3d::rend::IEffect* GetRoadProjectorShader();
        m3d::rend::IEffect* GetLsProjectorShader();
        m3d::rend::IEffect* GetObjProjectorShader(m3d::rend::IEffect* objShader);
        m3d::rend::IEffect* GetObjProjectorShader();
        m3d::rend::IEffect* GetTreeProjectorShader();
        void CollectNodesProjector(
            retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>>& nodes,
            unsigned int x,
            unsigned int z,
            CClipper const& projectorFrusta);
        m3d::rend::IEffect* GetLsLightShader();
        m3d::rend::IEffect* GetRoadLightShader();
        m3d::rend::IEffect* GetObjectLightShader(m3d::rend::IEffect* objShader);
        m3d::rend::IEffect* GetObjectLightShader();
        m3d::rend::IEffect* GetTreeLightShader();
        void CollectNodesLight(
            retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>>& nodes,
            unsigned int x,
            unsigned int z,
            CVector const& lightPos,
            float lightRadius);
        m3d::rend::IEffect* GetRoadSpriteShader();
        void UpdateTexShadowSizes();
        m3d::rend::IEffect* GetShadowShader();
        m3d::rend::IEffect* GetRoadShadowShader();
        m3d::rend::IEffect* GetLsDetShadowShader();
        m3d::rend::IEffect* GetRoadDetShadowShader();
        m3d::rend::IEffect* GetContourShader();
        bool IsInUnlinkAndDeleteAll() const;

        struct CellInfo
        {
            unsigned int trisCount;
            unsigned short objCount;
            unsigned short animModelCount;
            unsigned short meshesCount;
        };

    public:
        void GetCellsStatistic(
            retruxx::vector<m3d::SceneGraph::CellInfo, retruxx::allocator<m3d::SceneGraph::CellInfo>>* cellStats);
        void DumpRenderingNodesInfoForClass(m3d::Class const* nodeClass);

        struct CellItems
        {
            m3d::ObjectsContainer m_nodesLinkedDirect;
            retruxx::set<m3d::Object*, retruxx::less<m3d::Object*>, retruxx::allocator<m3d::Object*>>
                m_nodesShadowingDirect;
            bool m_bVisibleInCurrentFrame = false;
        };

    private:
        m3d::SgNode** m_visSlots;
        int* m_visNumSlots;
        m3d::SgNode** m_visSlotsUnderwater;
        int* m_visNumSlotsUnderwater;
        m3d::SgNode** m_transparentNodes;
        int m_numTransparentNodes;
        retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>> m_thinkList;
        retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>> m_ttledList;
        retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>> m_RemoveIfFreeList;
        retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>> m_contourList;
        retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>> m_updateXFormList;
        m3d::SceneGraph::CellItems& GetCellItems(int x, int z);
        m3d::SceneGraph::CellItems const& GetCellItems(int x, int z) const;
        int AddOneNodeToRender(m3d::SgNode* n, CClipper const& frusta, int curFrame);
        int AddNodeAndItsChildrenToRender(m3d::SgNode* n, CClipper const& frusta, int curFrame);
        void RemoveNodeExceptRemoveIfFree(m3d::SgNode*& toRemove);

        static int const NUM_CELL_ITEMS;
        m3d::SceneGraph::CellItems m_cellItems[4096];
        bool m_easyRelink;
        bool m_cellsPrepared;
        m3d::SgNode m_rootNode;
        m3d::SgNode m_landscapeNode;
        m3d::CWorld* m_owner;
        m3d::rend::IEffect* m_lsProjectorShader;
        m3d::rend::IEffect* m_roadProjectorShader;
        m3d::rend::IEffect* m_objProjectorShader;
        m3d::rend::IEffect* m_treeProjectorShader;
        m3d::rend::IEffect* m_roadLightShader;
        m3d::rend::IEffect* m_lsLightShader;
        m3d::rend::IEffect* m_objectLightShader;
        m3d::rend::IEffect* m_treeLightShader;
        m3d::rend::IEffect* m_roadSpriteShader;
        m3d::rend::TexHandle m_texShadows[20];
        m3d::rend::TexHandle m_texShadow;
        m3d::rend::TexHandle m_detTexShadow;
        m3d::rend::TexHandle m_texBlurShadow;
        m3d::rend::IEffect* m_shadowShader;
        m3d::rend::IEffect* m_blurShadowShader;
        m3d::rend::IEffect* m_lsShadowShader;
        m3d::rend::IEffect* m_roadShadowShader;
        m3d::rend::IEffect* m_lsDetailShadowShader;
        m3d::rend::IEffect* m_roadDetailShadowShader;
        m3d::rend::IHlslShader* m_grassShadowVs;
        m3d::rend::IHlslShader* m_grassShadowPs;

        void DrawShadows();
        void DrawDetailedShadows(
            CVector const& pos,
            float radius,
            m3d::rend::TexHandle tex,
            retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*>> const& classesToRender);
        void DrawShadowsToTexture(
            int* cis,
            int cnt,
            int size,
            m3d::rend::TexHandle tex,
            retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*>> const& classesToRender,
            CVector& pos,
            float radius);
        void PutShadowTextureToLandscapeAndRoad(
            int* cis,
            int cnt,
            float fade0,
            float fade1,
            int size,
            m3d::rend::TexHandle tex);
        void PutShadowTextureToGrass(int* cis, int cnt, float fade0, float fade1, int size, m3d::rend::TexHandle tex);
        void CollectShadowingNodes(
            retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>>& nodes,
            m3d::Class* clazz,
            int x,
            int z,
            int size,
            unsigned int ls,
            int curFrame);
        bool IsThereSomethingToShadow(
            m3d::SceneGraph::CellItems&,
            retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*>> const&);
        bool IsTransparent(m3d::SgNode* n);
        void DrawStencilShadows();
        void CollectShadowingNodesStencil(
            retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>>& nodes,
            m3d::Class* clazz,
            int x,
            int z,
            unsigned int ls,
            int curFrame);
        m3d::rend::IEffect* m_contourShader;
        bool m_noModelCull;
        bool m_bIsInUnlinkAndDeleteAll;
        bool m_bIsPurgingRemoveIfFree;
        m3d::IsNodeTransparent* m_transparencyTest;
        m3d::SgNode* TraceLineThruCellNodesForClass(
            float& tt,
            int cx,
            int cz,
            CVector const& v0,
            CVector const& dir,
            m3d::Class* wantClazz,
            retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*>>& dontCheckTwice,
            unsigned int traceMode);
        unsigned char m_enableMap[65536];
        unsigned char m_enableVisSpaceMask;
        char m_sortedCellsX[240000];
        char m_sortedCellsY[240000];
        int m_sortedCellsCurRadius;
        int m_sortedCellsEndRadius;
        int m_sortedCellsCurCell;
        void enableVisibleCells_r(CClipper& frusta, float* box, unsigned int or);
        void enableCellsSetRect(float* rc, unsigned int v0, unsigned int v1);
        void enableCellsSetRect(int* rc, unsigned int or, unsigned int and);
        void enableVisibleCellsFilterOutBackfaces(CMatrix const&, m3d::rend::Cull, unsigned int);
        int getYOfs(int y);
        void EnsureEverythingIsUnlinked() const;
    };
}  // namespace m3d

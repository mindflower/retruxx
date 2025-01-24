#pragma once
#include "nodes/sgnode.h"
#include <list>
#include <set>
#include <vector>
#include <math/point2d.h>
#include <renderer/i_renderer.h>

struct CClipper;

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
        virtual bool test(m3d::SgNode* node, float distCoeff) /* 0x00 */;
        virtual float getTransparentRadius() /* 0x04 */;
        virtual bool setPermanentTransparency(m3d::SgNode* node) /* 0x08 */;
    }; /* size: 0x0004 */

    class ObjectsContainer
    {
    private:
        /* 0x0000 */ retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*> > m_objectsByClassIdx[64];

    public:
        void AddObject(m3d::Object* obj);
        void RemoveObject(m3d::Object* obj);
        retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*> >* GetObjectsByClass(m3d::Class* cl);
        const retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*> >* GetObjects() const;
        retruxx::list<m3d::Object*, retruxx::allocator<m3d::Object*> >* GetObjects();
        bool empty() const;
    }; /* size: 0x0300 */

    static_assert(sizeof(ObjectsContainer) == 0x0300);

    struct GraphItemsForSgNode
    {
        /* 0x0000 */ PointBase<int> m_cellsCoveredPoint0;
        /* 0x0008 */ PointBase<int> m_cellsCoveredPoint1;
        /* 0x0010 */ retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > m_cellsShadowCovered;

        bool bSomeCellsCovered() const;
        void ClearCellsCovered();
    }; /* size: 0x0020 */

    static_assert(sizeof(GraphItemsForSgNode) == 0x0020);

    class SceneGraph
    {
    public:
        SceneGraph(const m3d::SceneGraph&);
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
        void UpdateVis(bool newFrame, const CClipper& frusta, bool primary);
        void SortNodesForRender(int);
        void Render(m3d::SgRenderFlags flags);
        void RenderDebugForNode(m3d::SgNode* n);
        void RenderNode(m3d::SgNode* n, const CMatrix& curMatr, bool fullInv);
        void RenderContouredNodes();
        void SetOwner(m3d::CWorld* w);
        m3d::CWorld* GetOwner();
        m3d::SgNode* TraceLine(CVector& hit, const CVector& start, const CVector& finish, const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> >& cl0, unsigned int traceMode);
        int TraceRect(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*> >& dest, const CVector2& v0, const CVector2& v1, const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> >& affectClasses);
        void SetModelForceNoCull(bool t);
        void LightSwitchOffAllLights();
        void LightSetupLightsForNode(m3d::SgNode* node);
        void LightSetupSunForWorld();
        const m3d::SgNode* GetRootNode() const;
        m3d::SgNode* GetRootNode();
        m3d::SgNode* GetNodeByNamesHierarchy(const retruxx::vector<CStr, retruxx::allocator<CStr> >& hierarchy);
        void GetNodeNamesHierarchy(m3d::SgNode* node, retruxx::vector<CStr, retruxx::allocator<CStr> >& hierarchy);
        int IsCellEnabled(int x, int y);
        void SortedCellsPrepare();
        bool SortedCellsStartFetching(int radius0, int radius1);
        int SortedCellsFetch(int& cellX, int& cellY, int& vis, int& radius);
        int SortedCellsFetch(int& cellX, int& cellY, int& vis);
        void EnableVisibleCells(CClipper& frusta, unsigned int or );
        void SetVisMask(unsigned char vis);
        void RefreshObjectsInRect(int cx0, int cy0, int cx1, int cy1);
        float GetAlphaForNode(m3d::SgNode* n);
        void SetTransparencyTest(m3d::IsNodeTransparent* t);
        void DumpToFile(const CStr& filename) const;
        m3d::SgNode* GetNodeByName(const CStr& name);
        const m3d::ObjectsContainer& GetCellObjs(int x, int z) const;
        bool IsCellVisible(int x, int z) const;
        m3d::rend::IEffect* GetRoadProjectorShader();
        m3d::rend::IEffect* GetLsProjectorShader();
        m3d::rend::IEffect* GetObjProjectorShader(m3d::rend::IEffect* objShader);
        m3d::rend::IEffect* GetObjProjectorShader();
        m3d::rend::IEffect* GetTreeProjectorShader();
        void CollectNodesProjector(retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> >& nodes, unsigned int x, unsigned int z, const CClipper& projectorFrusta);
        m3d::rend::IEffect* GetLsLightShader();
        m3d::rend::IEffect* GetRoadLightShader();
        m3d::rend::IEffect* GetObjectLightShader(m3d::rend::IEffect* objShader);
        m3d::rend::IEffect* GetObjectLightShader();
        m3d::rend::IEffect* GetTreeLightShader();
        void CollectNodesLight(retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> >& nodes, unsigned int x, unsigned int z, const CVector& lightPos, float lightRadius);
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
            /* 0x0000 */ unsigned int trisCount;
            /* 0x0004 */ unsigned short objCount;
            /* 0x0006 */ unsigned short animModelCount;
            /* 0x0008 */ unsigned short meshesCount;
        }; /* size: 0x000c */

    public:
        void GetCellsStatistic(retruxx::vector<m3d::SceneGraph::CellInfo, retruxx::allocator<m3d::SceneGraph::CellInfo> >* cellStats);
        void DumpRenderingNodesInfoForClass(const m3d::Class* nodeClass);

        struct CellItems
        {
            /* 0x0000 */ m3d::ObjectsContainer m_nodesLinkedDirect;
            /* 0x0300 */ retruxx::set<m3d::Object*, retruxx::less<m3d::Object*>, retruxx::allocator<m3d::Object*> > m_nodesShadowingDirect;
            /* 0x030c */ bool m_bVisibleInCurrentFrame = false;
        }; /* size: 0x0310 */

    private:
        /* 0x0000 */ m3d::SgNode** m_visSlots;
        /* 0x0004 */ int* m_visNumSlots;
        /* 0x0008 */ m3d::SgNode** m_visSlotsUnderwater;
        /* 0x000c */ int* m_visNumSlotsUnderwater;
        /* 0x0010 */ m3d::SgNode** m_transparentNodes;
        /* 0x0014 */ int m_numTransparentNodes;
        /* 0x0018 */ retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> > m_thinkList;
        /* 0x0024 */ retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> > m_ttledList;
        /* 0x0030 */ retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> > m_RemoveIfFreeList;
        /* 0x003c */ retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> > m_contourList;
        /* 0x0048 */ retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> > m_updateXFormList;
        m3d::SceneGraph::CellItems& GetCellItems(int x, int z);
        const m3d::SceneGraph::CellItems& GetCellItems(int x, int z) const;
        int AddOneNodeToRender(m3d::SgNode* n, const CClipper& frusta, int curFrame);
        int AddNodeAndItsChildrenToRender(m3d::SgNode* n, const CClipper& frusta, int curFrame);
        void RemoveNodeExceptRemoveIfFree(m3d::SgNode*& toRemove);
        static const int NUM_CELL_ITEMS;
        /* 0x0054 */ m3d::SceneGraph::CellItems m_cellItems[4096];
        /* 0x310054 */ bool m_easyRelink;
        /* 0x310055 */ bool m_cellsPrepared;
        /* 0x310056 */ char Padding_163[2];
        /* 0x310058 */ m3d::SgNode m_rootNode;
        /* 0x31022c */ m3d::SgNode m_landscapeNode;
        /* 0x310400 */ m3d::CWorld* m_owner;
        /* 0x310404 */ m3d::rend::IEffect* m_lsProjectorShader;
        /* 0x310408 */ m3d::rend::IEffect* m_roadProjectorShader;
        /* 0x31040c */ m3d::rend::IEffect* m_objProjectorShader;
        /* 0x310410 */ m3d::rend::IEffect* m_treeProjectorShader;
        /* 0x310414 */ m3d::rend::IEffect* m_roadLightShader;
        /* 0x310418 */ m3d::rend::IEffect* m_lsLightShader;
        /* 0x31041c */ m3d::rend::IEffect* m_objectLightShader;
        /* 0x310420 */ m3d::rend::IEffect* m_treeLightShader;
        /* 0x310424 */ m3d::rend::IEffect* m_roadSpriteShader;
        /* 0x310428 */ m3d::rend::TexHandle m_texShadows[20];
        /* 0x310478 */ m3d::rend::TexHandle m_texShadow;
        /* 0x31047c */ m3d::rend::TexHandle m_detTexShadow;
        /* 0x310480 */ m3d::rend::TexHandle m_texBlurShadow;
        /* 0x310484 */ m3d::rend::IEffect* m_shadowShader;
        /* 0x310488 */ m3d::rend::IEffect* m_blurShadowShader;
        /* 0x31048c */ m3d::rend::IEffect* m_lsShadowShader;
        /* 0x310490 */ m3d::rend::IEffect* m_roadShadowShader;
        /* 0x310494 */ m3d::rend::IEffect* m_lsDetailShadowShader;
        /* 0x310498 */ m3d::rend::IEffect* m_roadDetailShadowShader;
        /* 0x31049c */ m3d::rend::IHlslShader* m_grassShadowVs;
        /* 0x3104a0 */ m3d::rend::IHlslShader* m_grassShadowPs;
        void DrawShadows();
        void DrawDetailedShadows(const CVector& pos, float radius, m3d::rend::TexHandle tex, const retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*> >& classesToRender);
        void DrawShadowsToTexture(int* cis, int cnt, int size, m3d::rend::TexHandle tex, const retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*> >& classesToRender, CVector& pos, float radius);
        void PutShadowTextureToLandscapeAndRoad(int* cis, int cnt, float fade0, float fade1, int size, m3d::rend::TexHandle tex);
        void PutShadowTextureToGrass(int* cis, int cnt, float fade0, float fade1, int size, m3d::rend::TexHandle tex);
        void CollectShadowingNodes(retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> >& nodes, m3d::Class* clazz, int x, int z, int size, unsigned int ls, int curFrame);
        bool IsThereSomethingToShadow(m3d::SceneGraph::CellItems&, const retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*> >&);
        bool IsTransparent(m3d::SgNode* n);
        void DrawStencilShadows();
        void CollectShadowingNodesStencil(retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> >& nodes, m3d::Class* clazz, int x, int z, unsigned int ls, int curFrame);
        /* 0x3104a4 */ m3d::rend::IEffect* m_contourShader;
        /* 0x3104a8 */ bool m_noModelCull;
        /* 0x3104a9 */ bool m_bIsInUnlinkAndDeleteAll;
        /* 0x3104aa */ bool m_bIsPurgingRemoveIfFree;
        /* 0x3104ab */ char Padding_164;
        /* 0x3104ac */ m3d::IsNodeTransparent* m_transparencyTest;
        m3d::SgNode* TraceLineThruCellNodesForClass(float& tt, int cx, int cz, const CVector& v0, const CVector& dir, m3d::Class* wantClazz, retruxx::set<m3d::SgNode*, retruxx::less<m3d::SgNode*>, retruxx::allocator<m3d::SgNode*> >& dontCheckTwice, unsigned int traceMode);
        /* 0x3104b0 */ unsigned char m_enableMap[65536];
        /* 0x3204b0 */ unsigned char m_enableVisSpaceMask;
        /* 0x3204b1 */ char Padding_165[79];
        /* 0x320500 */ char m_sortedCellsX[240000];
        /* 0x35ae80 */ char m_sortedCellsY[240000];
        /* 0x395800 */ int m_sortedCellsCurRadius;
        /* 0x395804 */ int m_sortedCellsEndRadius;
        /* 0x395808 */ int m_sortedCellsCurCell;
        /* 0x39580c */ char Padding_166[116];
        void enableVisibleCells_r(CClipper& frusta, float* box, unsigned int or );
        void enableCellsSetRect(float* rc, unsigned int v0, unsigned int v1);
        void enableCellsSetRect(int* rc, unsigned int or , unsigned int and);
        void enableVisibleCellsFilterOutBackfaces(const CMatrix&, m3d::rend::Cull, unsigned int);
        int getYOfs(int y);
        void EnsureEverythingIsUnlinked() const;
    }; /* size: 0x395880 */

    static_assert(sizeof(SceneGraph) == 0x395880);
}

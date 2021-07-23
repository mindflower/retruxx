#pragma once
#include "nodes/sgnode.h"
#include <list>
#include <set>
#include <vector>
#include <math/point2d.h>

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
        virtual float getTransparentRadius();
        virtual bool setPermanentTransparency(SgNode*);
        virtual bool test(SgNode*, float);

    private:
        //m3d::IsNodeTransparent_vtbl* __vftable /*VFT*/;
    };

    class ObjectsContainer
    {
    public:
        std::list<m3d::Object*>* GetObjectsByClass(m3d::Class*);
        std::list<m3d::Object*> const* GetObjects() const;
        std::list<m3d::Object*>* GetObjects();
        void AddObject(m3d::Object*);
        void RemoveObject(m3d::Object*);

    private:
        std::list<m3d::Object*> m_objectsByClassIdx[64];

    };
    class GraphItemsForSgNode
    {
    public:
        void ClearCellsCovered();
        bool bSomeCellsCovered() const;

    private:
        PointBase<int> m_cellsCoveredPoint0;
        PointBase<int> m_cellsCoveredPoint1;
        std::vector<unsigned int> m_cellsShadowCovered;
    };

    class SceneGraph
    {
    public:
        struct CellInfo
        {
            unsigned int trisCount;
            unsigned __int16 objCount;
            unsigned __int16 animModelCount;
            unsigned __int16 meshesCount;
        };

        class CellItems
        {
        public:
            CellItems();

        private:
            ObjectsContainer m_nodesLinkedDirect;
            std::set<Object*> m_nodesShadowingDirect;
            bool m_bVisibleInCurrentFrame;
        };

    public:
        void LinkNode(SgNode*);
        void SortedCellsPrepare();
        rend::IEffect* GetContourShader();
        void UnlinkAndDeleteAll();
        void UnlinkThinkNode(SgNode*);
        float GetAlphaForNode(SgNode*);
        void UpdateThinkNodes();
        void DumpRenderingNodesInfoForClass(const Class*);
        rend::IEffect* GetObjProjectorShader();
        rend::IEffect* GetObjProjectorShader(rend::IEffect*);
        SgNode* GetRootNode();
        SgNode const* GetRootNode() const;
        void RelinkNode(SgNode*, bool);
        rend::IEffect* GetLsProjectorShader();
        void CollectNodesLight(std::set<SgNode*>&, unsigned int, unsigned int, CVector const&, float);
        void InsertInRemoveIfFree(SgNode*);
        bool SortedCellsStartFetching(int, int);
        void RefreshObjectsInRect(int, int, int, int);
        rend::IEffect* GetRoadDetShadowShader();
        int SortedCellsFetch(int&, int&, int&, int&);
        int SortedCellsFetch(int&, int&, int&);
        rend::IEffect* GetObjectLightShader(rend::IEffect*);
        rend::IEffect* GetObjectLightShader();
        void LightSwitchOffAllLights();
        void UnlinkNode(SgNode*);
        SgNode* GetNodeByName(CStr const&);
        rend::IEffect* GetTreeProjectorShader();
        bool IsLinkedNode(SgNode*);
        bool IsCellVisible(int, int) const;
        void DeleteFromRemoveIfFree(SgNode*);
        SgNode* TraceLine(CVector&, CVector const&, CVector const&, std::set<Class*> const&, unsigned int);
        rend::IEffect* GetRoadLightShader();
        void LightSetupLightsForNode(SgNode*);
        void Update();
        void RenderDebugForNode(SgNode*);
        void RenderContouredNodes();
        void UpdateAllXForms();
        void DeleteFromTtlList(SgNode*);
        rend::IEffect* GetRoadProjectorShader();
        int IsCellEnabled(int, int);
        void RenderNode(SgNode*, CMatrix const&, bool);
        void Render(SgRenderFlags);
        void GetNodeNamesHierarchy(SgNode*, std::vector<CStr>&);
        SceneGraph();
        int TraceRect(std::list<SgNode*>&, CVector2 const&, CVector2 const&, std::set<Class*> const&);
        void UpdateVis(bool, CClipper const&, bool);
        void DeleteFromContourList(SgNode*);
        SgNode* GetNodeByNamesHierarchy(std::vector<CStr> const&);
        void InsertInUpdateXFormList(SgNode*);
        void SetVisMask(unsigned char);
        rend::IEffect* GetRoadSpriteShader();
        rend::IEffect* GetTreeLightShader();
        bool IsInUnlinkAndDeleteAll() const;
        void DeleteAllTtledNodes();
        void SetModelForceNoCull(bool);
        ObjectsContainer const& GetCellObjs(int, int) const;
        void CollectNodesProjector(std::set<SgNode*>&, unsigned int, unsigned int, CClipper const&);
        void EnableVisibleCells(CClipper&, unsigned int);
        void RemoveNode(SgNode*&);
        void DeleteAllRemoveIfFreeNodes();
        rend::IEffect* GetShadowShader();
        void DumpToFile(CStr const&) const;
        void LightSetupSunForWorld();
        void UpdateTexShadowSizes();
        void SetOwner(CWorld*);
        ~SceneGraph();
        void SetTransparencyTest(IsNodeTransparent*);
        rend::IEffect* GetLsLightShader();
        void InsertInContourList(SgNode*, unsigned int, float);
        void CheckNodeIsNotInAnyList(SgNode*) const;
        void InsertInTtlList(SgNode*, int);
        void LinkThinkNode(SgNode*);
        void DeleteFromUpdateXFormList(SgNode*);
        void GetCellsStatistic(std::vector<CellInfo>*);
        rend::IEffect* GetRoadShadowShader();

    private:
        CellItems& GetCellItems(int, int);
        CellItems const& GetCellItems(int, int) const;
        void CollectShadowingNodesStencil(std::set<SgNode*>&, Class*, int, int, unsigned int, int);
        void CollectShadowingNodes(std::set<SgNode*>&, Class*, int, int, int, unsigned int, int);
        void PutShadowTextureToGrass(int*, int, float, float, int, rend::TexHandle);
        int AddOneNodeToRender(SgNode*, CClipper const&, int);
        void RemoveNodeExceptRemoveIfFree(SgNode*&);
        void DrawShadowsToTexture(int*, int, int, rend::TexHandle, std::vector<Class*> const&, CVector&, float);
        void DrawDetailedShadows(CVector const&, float, rend::TexHandle, std::vector<Class*> const&);
        int getYOfs(int);
        void EnsureEverythingIsUnlinked() const;
        void PutShadowTextureToLandscapeAndRoad(int*, int, float, float, int, rend::TexHandle);
        void DrawShadows();
        void enableCellsSetRect(int*, unsigned int, unsigned int);
        void enableCellsSetRect(float*, unsigned int, unsigned int);
        SgNode* TraceLineThruCellNodesForClass(float&, int, int, CVector const&, CVector const&, Class*, std::set<SgNode*>&, unsigned int);
        int AddNodeAndItsChildrenToRender(SgNode*, CClipper const&, int);
        void DrawStencilShadows();
        bool IsTransparent(SgNode*);
        void enableVisibleCells_r(CClipper&, float*, unsigned int);

    private:
        SgNode** m_visSlots;
        int* m_visNumSlots;
        SgNode** m_visSlotsUnderwater;
        int* m_visNumSlotsUnderwater;
        SgNode** m_transparentNodes;
        int m_numTransparentNodes;
        std::set<SgNode*> m_thinkList;
        std::set<SgNode*> m_ttledList;
        std::set<SgNode*> m_RemoveIfFreeList;
        std::set<SgNode*> m_contourList;
        std::set<SgNode*> m_updateXFormList;
        CellItems m_cellItems[4096];
        bool m_easyRelink;
        bool m_cellsPrepared;
        SgNode m_rootNode;
        SgNode m_landscapeNode;
        CWorld* m_owner;
        rend::IEffect* m_lsProjectorShader;
        rend::IEffect* m_roadProjectorShader;
        rend::IEffect* m_objProjectorShader;
        rend::IEffect* m_treeProjectorShader;
        rend::IEffect* m_roadLightShader;
        rend::IEffect* m_lsLightShader;
        rend::IEffect* m_objectLightShader;
        rend::IEffect* m_treeLightShader;
        rend::IEffect* m_roadSpriteShader;
        rend::TexHandle m_texShadows[20];
        rend::TexHandle m_texShadow;
        rend::TexHandle m_detTexShadow;
        rend::TexHandle m_texBlurShadow;
        rend::IEffect* m_shadowShader;
        rend::IEffect* m_blurShadowShader;
        rend::IEffect* m_lsShadowShader;
        rend::IEffect* m_roadShadowShader;
        rend::IEffect* m_lsDetailShadowShader;
        rend::IEffect* m_roadDetailShadowShader;
        rend::IHlslShader* m_grassShadowVs;
        rend::IHlslShader* m_grassShadowPs;
        rend::IEffect* m_contourShader;
        bool m_noModelCull;
        bool m_bIsInUnlinkAndDeleteAll;
        bool m_bIsPurgingRemoveIfFree;
        IsNodeTransparent* m_transparencyTest;
        unsigned __int8 m_enableMap[65536];
        unsigned __int8 m_enableVisSpaceMask;
        char m_sortedCellsX[240000];
        char m_sortedCellsY[240000];
        int m_sortedCellsCurRadius;
        int m_sortedCellsEndRadius;
        int m_sortedCellsCurCell;
    };
}

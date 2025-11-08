#pragma once
#include <core/stringm3d.h>
#include <math/vector.h>
#include <math/vector2.h>
#include "thirdparty/containers.h"

struct CClipper;

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }

    class Landscape;
    class AnimatedModel;
    class RoadNode;

    enum RenderRoadType
    {
        RRT_SIMPLE = 0x0,
        RRT_FOR_SHADOW = 0x1,
        RRT_FOR_PROJECTOR = 0x2,
        RRT_FOR_DETAILED_SHADOW = 0x3,
        RRT_FOR_POINTLIGHT = 0x4,
        RRT_FOR_SPRITE = 0x5,
    };

    class RoadSet
    {
    public:
        void Clear();
        ~RoadSet();
        RoadSet();
        int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);

    private:
        CStr m_name;
        retruxx::vector<AnimatedModel*> m_roadModels[4];
        retruxx::vector<retruxx::vector<retruxx::vector<unsigned int>>> m_boundVerts[4];
        retruxx::vector<retruxx::vector<retruxx::vector<unsigned int>>> m_fakeBoundVerts[4];
        retruxx::vector<retruxx::vector<retruxx::vector<unsigned int>>> m_cliffBorders[4];
        float m_minX[4];
        float m_minZ[4];
        float m_maxX[4];
        float m_maxZ[4];
        float m_sizeZ[4];
        float m_sizeX[4];
        CVector m_scale;
        CStr m_wheeltraceTexName;
        int m_soilType;
    };

    class RoadTestCallBack
    {
    private:
        //m3d::RoadTestCallBack_vtbl* __vftable /*VFT*/;
    };

    class RoadInRadius3dTest : public RoadTestCallBack
    {
    public:
        virtual bool TestRoadNode(RoadNode*) const;
        RoadInRadius3dTest(CVector const&, float);

    private:
        CVector org;
        float radius;

    };
    class RoadInRadius2dTest : public RoadTestCallBack
    {
    public:
        virtual bool TestRoadNode(RoadNode*) const;
        RoadInRadius2dTest(CVector const&, float);

    private:
        CVector org;
        float radius;
    };

    class RoadInFrustumTest : public RoadTestCallBack
    {
    public:
        RoadInFrustumTest(CClipper*);
        virtual bool TestRoadNode(RoadNode*) const;

    private:
        CClipper* frustum;
    };

    class RoadManager
    {
    private:
        /* 0x0000 */ m3d::Landscape* m_owner;
        /* 0x0004 */ retruxx::vector<m3d::RoadNode*, retruxx::allocator<m3d::RoadNode*> >* m_coveredCells;

    public:
        RoadManager(const m3d::RoadManager&);
        RoadManager();
        /* 0x0008 */ m3d::RoadNode* m_roadRoot;
        /* 0x000c */ retruxx::vector<m3d::RoadSet*, retruxx::allocator<m3d::RoadSet*> > m_roadSets;
        void ClearRoadSets();
        void RebuildStructures();
        void RebuildSomeNodes(retruxx::set<m3d::RoadNode*, retruxx::less<m3d::RoadNode*>, retruxx::allocator<m3d::RoadNode*> > nodesToRebuild, bool bNeedToRelink);
        void Init();
        void Release();
        void ReleaseCollision();
        void ReleaseCollisionForRoadNode(m3d::RoadNode* rn);
        void UnlinkRoadNodeCollisionFromCells(m3d::RoadNode* rn);
        int ReadRoadSetConfigFromXmlFile(const char* configName);
        int ReadRoadsFromXmlFile(const char* name);
        int WriteRoadsToXmlFile(const char* filename);
        void SetOwner(m3d::Landscape* owner);
        int GetRoadSetHandleByName(const CStr& name);
        const CStr GetRoadSetNameByHandle(int handle);
        void GetRoadMinMaxXByHandle(int handle, int roadType, float& minx, float& maxx);
        void GetRoadMinMaxZByHandle(int handle, int roadType, float& minz, float& maxz);
        retruxx::vector<m3d::RoadNode*, retruxx::allocator<m3d::RoadNode*> >& GetCoveredCell(unsigned int);
        int RenderRoads(retruxx::vector<unsigned int, retruxx::allocator<unsigned int> >& visList, m3d::RenderRoadType rrt, const m3d::RoadTestCallBack* rnTest, bool bForRoadMap);
        void UpdateVis();

    private:
        void FindFriends();
        CVector2 FindLeftProjection(m3d::RoadNode* rn, float x, float z);
        void RecalcCoveredCells();
        void LinkRoadNodes();
        void CalcNodeData(m3d::RoadNode* rn);
        void LinkToBorder(m3d::RoadNode* rn, unsigned int idx, int link, CVector& res);
        bool GetAdjPoint(m3d::RoadNode* rn, unsigned int idx, int link, CVector& acceptor);
    }; /* size: 0x001c */

    static_assert(sizeof(RoadManager) == 0x001c);
}

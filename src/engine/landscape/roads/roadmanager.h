#pragma once
#include <core/stringm3d.h>

namespace m3d
{
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
        std::vector<AnimatedModel*> m_roadModels[4];
        std::vector<std::vector<std::vector<unsigned int>>> m_boundVerts[4];
        std::vector<std::vector<std::vector<unsigned int>>> m_fakeBoundVerts[4];
        std::vector<std::vector<std::vector<unsigned int>>> m_cliffBorders[4];
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
    public:
        CStr const GetRoadSetNameByHandle(int);
        void UpdateVis();
        void ReleaseCollisionForRoadNode(RoadNode *);
        void RebuildStructures();
        int GetRoadSetHandleByName(CStr const &);
        void RebuildSomeNodes(std::set<RoadNode *>,bool);
        void GetRoadMinMaxZByHandle(int,int,float &,float &);
        RoadManager();
        int WriteRoadsToXmlFile(char const *);
        void GetRoadMinMaxXByHandle(int,int,float &,float &);
        void UnlinkRoadNodeCollisionFromCells(RoadNode *);
        void SetOwner(Landscape *);
        int ReadRoadSetConfigFromXmlFile(char const *);
        int ReadRoadsFromXmlFile(char const *);
        void Release();
        int RenderRoads(std::vector<unsigned int> &,RenderRoadType,RoadTestCallBack const *,bool);
        void Init();
        void ClearRoadSets();
        void ReleaseCollision();

    private:
        void RecalcCoveredCells();
        void LinkToBorder(RoadNode *,unsigned int,int,struct CVector &);
        void LinkRoadNodes();
        struct CVector2 FindLeftProjection(RoadNode *,float,float);
        bool GetAdjPoint(RoadNode *,unsigned int,int,struct CVector &);
        void FindFriends();
        void CalcNodeData(RoadNode *);
        Landscape *m_owner;
        std::vector<RoadNode *> *m_coveredCells;
        RoadNode *m_roadRoot;
        std::vector<RoadSet *> m_roadSets;
    };
}

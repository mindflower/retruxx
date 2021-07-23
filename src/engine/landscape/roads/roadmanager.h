#pragma once

namespace m3d
{
    class RoadInRadius3dTest :  public RoadTestCallBack
    {
    public:
        virtual bool TestRoadNode(RoadNode *) const ;
        RoadInRadius3dTest(struct CVector const &,float);
    protected:
    private:
        CVector org;
        float radius;
    };
}

namespace m3d
{
    class RoadInRadius2dTest :  public RoadTestCallBack
    {
    public:
        virtual bool TestRoadNode(RoadNode *) const ;
        RoadInRadius2dTest(struct CVector const &,float);
    protected:
    private:
        CVector org;
        float radius;
    };
}

namespace m3d
{
    class RoadInFrustumTest :  public RoadTestCallBack
    {
    public:
        RoadInFrustumTest(class CClipper *);
        virtual bool TestRoadNode(RoadNode *) const ;
    protected:
    private:
        CClipper *frustum;
    };
}

namespace m3d
{
    class RoadManager
    {
    public:
        class CStr const GetRoadSetNameByHandle(int);
        void UpdateVis();
        void ReleaseCollisionForRoadNode(RoadNode *);
        void RebuildStructures();
        int GetRoadSetHandleByName(class CStr const &);
        void RebuildSomeNodes(class std::set<RoadNode *,struct std::less<RoadNode *>,class std::allocator<RoadNode *> >,bool);
        void GetRoadMinMaxZByHandle(int,int,float &,float &);
        RoadManager();
        int WriteRoadsToXmlFile(char const *);
        void GetRoadMinMaxXByHandle(int,int,float &,float &);
        void UnlinkRoadNodeCollisionFromCells(RoadNode *);
        void SetOwner(Landscape *);
        int ReadRoadSetConfigFromXmlFile(char const *);
        int ReadRoadsFromXmlFile(char const *);
        void Release();
        int RenderRoads(class std::vector<unsigned int,class std::allocator<unsigned int> > &,RenderRoadType,RoadTestCallBack const *,bool);
        void Init();
        void ClearRoadSets();
        void ReleaseCollision();
    protected:
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

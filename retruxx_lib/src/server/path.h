#pragma once
#include "map.h"

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }
}

namespace ai
{
    enum eSearchResult
    {
        PATH_NOT_ENOUGH_TIME = 0x0,
        PATH_POS_UNREACHABLE = 0x1,
        PATH_OK = 0x2,
    };

    class DHeap
    {
    public:
        bool FindMin(unsigned int*);
        void Insert(unsigned int);
        void SiftUp(unsigned int, unsigned int);
        DHeap(unsigned char, unsigned int, unsigned int*, unsigned int*);
        unsigned int MinChild(unsigned int);
        ~DHeap();
        bool DeleteMin(unsigned int*);
        void SiftDown(unsigned int, unsigned int);

    private:
        unsigned __int8 D;
        unsigned int MaxSize;
        unsigned int Size;
        unsigned int* pKey;
        unsigned int* pHInverse;
        unsigned int* pH;
    };

    class Path
    {
    public:
        void ProceedSearch4(unsigned int);
        void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *,Map *);
        void ProceedSearch5(unsigned int);
        Path(std::vector<CVector2,std::allocator<CVector2> > const &);
        Path();
        bool GetSearchStatus() const ;
        void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void ProceedSearch1(unsigned int);
        void ProceedSearch2(unsigned int);
        void ProceedSearchAStarStraightment(unsigned int);
        void ProceedSearch3(unsigned int);
        void GetItem(unsigned int,float *,float *) const ;
        eSearchResult GetResult() const ;
        void ProceedSearchDijkstraStraightment(unsigned int);
        float GetPathLength(int) const ;
        ~Path();
        void StartSearchInRect(Map *,CVector2 const &,CVector2 const &,CVector2 const &,CVector2 const &,float,unsigned char);
        bool IsStraightPathValid(Map *,CVector2 const &,CVector2 const &,float,unsigned char);
        unsigned int GetSize() const ;
        void StartSearch(Map *,CVector2 const &,CVector2 const &,float,unsigned char);

    private:
        void StartSearchInternal();
        void StraightenPath(bool);
        void IdToMapIndex(unsigned int,MapIndex &) const ;
        void ClearHelpers();
        unsigned int MapIndexToId(MapIndex const &) const ;
        void _Init();
        void FilterPath();
        unsigned int m_size;
        CVector2 *m_pPath;
        bool m_bSearching;
        eSearchResult m_SearchResult;
        unsigned int v;
        unsigned int w;
        Map *m_pMap;
        MapIndex m_startPointIndex;
        MapIndex m_endPointIndex;
        MapIndex m_minIndex;
        MapIndex m_maxIndex;
        unsigned __int8 m_blockedValue;
        int m_radius;
        float m_PathLength;
        DHeap *pHeap;
        unsigned int *pDist;
        unsigned int *pHInverse;
        unsigned __int8 *pFlag;
        int *pParent;
        int *pLength;
        unsigned int m_minDist;
        unsigned int m_idOfClosestPoint;
        CVector2 m_startPoint;
        CVector2 m_endPoint;
    };
}

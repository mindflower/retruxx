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
    private:
        /* 0x0000 */ unsigned int m_size;
        /* 0x0004 */ CVector2* m_pPath;
        /* 0x0008 */ bool m_bSearching;
        /* 0x0009 */ char Padding_120[3];
        /* 0x000c */ ai::eSearchResult m_SearchResult;
        /* 0x0010 */ unsigned int v;
        /* 0x0014 */ unsigned int w;
        /* 0x0018 */ ai::Map* m_pMap;
        ai::MapIndex m_startPointIndex;
        ai::MapIndex m_endPointIndex;
        ai::MapIndex m_minIndex;
        ai::MapIndex m_maxIndex;
        /* 0x003c */ unsigned char m_blockedValue;
        /* 0x003d */ char Padding_121[3];
        /* 0x0040 */ int m_radius;
        /* 0x0044 */ float m_PathLength;
        /* 0x0048 */ ai::DHeap* pHeap;
        /* 0x004c */ unsigned int* pDist;
        /* 0x0050 */ unsigned int* pHInverse;
        /* 0x0054 */ unsigned char* pFlag;
        /* 0x0058 */ long* pParent;
        /* 0x005c */ long* pLength;
        /* 0x0060 */ unsigned int m_minDist;
        /* 0x0064 */ unsigned int m_idOfClosestPoint;

    public:
        inline static int QuantAmount = 0;
        static int QuantMax;
        CVector2 m_startPoint;
        CVector2 m_endPoint;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode, ai::Map* pMap);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
        Path(const ai::Path&);
        Path(const std::vector<CVector2, std::allocator<CVector2> >& path);
        Path();
        ~Path();
        void StartSearch(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint, float radius, unsigned char blockedValue);
        void StartSearchInRect(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint, const CVector2& rectPoint1, const CVector2& rectPoint2, float radius, unsigned char blockedValue);
        void ProceedSearch1(unsigned int msInterval);
        void ProceedSearch2(unsigned int msInterval);
        void ProceedSearch3(unsigned int msInterval);
        void ProceedSearch4(unsigned int msInterval);
        void ProceedSearch5(unsigned int msInterval);
        void ProceedSearchAStarStraightment(unsigned int msInterval);
        void ProceedSearchDijkstraStraightment(unsigned int msInterval);
        float GetPathLength(int PathNum) const;
        bool GetSearchStatus() const;
        ai::eSearchResult GetResult() const;
        unsigned int GetSize() const;
        void GetItem(unsigned int ItemNum, float* PathCoordx, float* PathCoordy) const;
        bool IsStraightPathValid(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint, float radius, unsigned char blockedValue);

    private:
        void _Init();
        void StartSearchInternal();
        void ClearHelpers();
        unsigned int MapIndexToId(const ai::MapIndex& v) const;
        void IdToMapIndex(unsigned int id, ai::MapIndex& res) const;
        void StraightenPath(bool bForDijkstra);
        void FilterPath();
    }; /* size: 0x0078 */
}

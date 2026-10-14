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
        // Team::TeamAIOnPathFind reads the outcome of a finished search directly.
        friend class Team;

    private:
        unsigned int m_size;
        CVector2* m_pPath;
        bool m_bSearching;
        char Padding_120[3];
        ai::eSearchResult m_SearchResult;
        unsigned int v;
        unsigned int w;
        ai::Map* m_pMap;
        ai::MapIndex m_startPointIndex;
        ai::MapIndex m_endPointIndex;
        ai::MapIndex m_minIndex;
        ai::MapIndex m_maxIndex;
        unsigned char m_blockedValue;
        char Padding_121[3];
        int m_radius;
        float m_PathLength;
        ai::DHeap* pHeap;
        unsigned int* pDist;
        unsigned int* pHInverse;
        unsigned char* pFlag;
        long* pParent;
        long* pLength;
        unsigned int m_minDist;
        unsigned int m_idOfClosestPoint;

    public:
        inline static int QuantAmount = 0;
        // The shipped build holds 5 here and never writes it (RVA 0xA039B0).
        inline static int QuantMax = 5;
        CVector2 m_startPoint;
        CVector2 m_endPoint;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode, ai::Map* pMap);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
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
        // Not in the shipped build, which repeats this inline at the end of each search.
        void _BuildPathFromParents(unsigned int endNode, bool fTarget);
        unsigned int MapIndexToId(const ai::MapIndex& v) const;
        void IdToMapIndex(unsigned int id, ai::MapIndex& res) const;
        void StraightenPath(bool bForDijkstra);
        void FilterPath();
    };
}

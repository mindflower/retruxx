#include "path.h"

#include <stdexcept>

#include "retruxx/common.h"

namespace ai
{
    DHeap::~DHeap()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode, ai::Map* pMap)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Path::Path(const std::vector<CVector2, std::allocator<CVector2>>& path)
        : m_startPointIndex(0, 0)
        , m_endPointIndex(0, 0)
        , m_minIndex(0, 0)
        , m_maxIndex(0, 0)
    {
        this->m_size = 0;
        this->m_pPath = 0;
        this->pHeap = 0;
        this->pDist = 0;
        this->pHInverse = 0;
        this->pFlag = 0;
        this->pParent = 0;
        this->pLength = 0;
        this->m_bSearching = 0;
        this->m_blockedValue = -1;

        m_size = path.size();
        m_pPath = new CVector2[m_size];
        for (int i = 0 ; i < m_size ; ++i)
        {
            m_pPath[i] = path[i];
        }
    }

    Path::Path()
        : m_startPointIndex(0, 0)
        , m_endPointIndex(0, 0)
        , m_minIndex(0, 0)
        , m_maxIndex(0, 0)
    {
        this->m_size = 0;
        this->m_pPath = 0;
        this->pHeap = 0;
        this->pDist = 0;
        this->pHInverse = 0;
        this->pFlag = 0;
        this->pParent = 0;
        this->pLength = 0;
        this->m_bSearching = 0;
        this->m_blockedValue = -1;
    }

    Path::~Path()
    {
        delete[] m_pPath;
        ClearHelpers();
    }

    void Path::StartSearch(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint, float radius,
        unsigned char blockedValue)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::StartSearchInRect(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint,
        const CVector2& rectPoint1, const CVector2& rectPoint2, float radius, unsigned char blockedValue)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::ProceedSearch1(unsigned int msInterval)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::ProceedSearch2(unsigned int msInterval)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::ProceedSearch3(unsigned int msInterval)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::ProceedSearch4(unsigned int msInterval)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::ProceedSearch5(unsigned int msInterval)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::ProceedSearchAStarStraightment(unsigned int msInterval)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::ProceedSearchDijkstraStraightment(unsigned int msInterval)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Path::GetPathLength(int PathNum) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Path::GetSearchStatus() const
    {
        return this->m_bSearching;
    }

    ai::eSearchResult Path::GetResult() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int Path::GetSize() const
    {
        return this->m_size;
    }

    void Path::GetItem(unsigned int ItemNum, float* PathCoordx, float* PathCoordy) const
    {
        if (ItemNum < m_size)
        {
            const auto& coord = m_pPath[ItemNum];
            *PathCoordx = coord.x;
            *PathCoordy = coord.y;
        }
    }

    bool Path::IsStraightPathValid(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint, float radius,
        unsigned char blockedValue)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::_Init()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::StartSearchInternal()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::ClearHelpers()
    {
        delete pHeap;
        pHeap = nullptr;

        delete pDist;
        pDist = nullptr;

        delete pHInverse;
        pHInverse = nullptr;

        delete pParent;
        pParent = nullptr;

        delete pFlag;
        pFlag = nullptr;

        delete pLength;
        pLength = nullptr;
    }

    unsigned int Path::MapIndexToId(const ai::MapIndex& v) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::IdToMapIndex(unsigned int id, ai::MapIndex& res) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::StraightenPath(bool bForDijkstra)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Path::FilterPath()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

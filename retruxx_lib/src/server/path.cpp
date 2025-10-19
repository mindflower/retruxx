#include "path.h"

#include <stdexcept>

namespace ai
{
    void Path::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode, ai::Map* pMap)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Path::StartSearchInRect(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint,
        const CVector2& rectPoint1, const CVector2& rectPoint2, float radius, unsigned char blockedValue)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::ProceedSearch1(unsigned int msInterval)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::ProceedSearch2(unsigned int msInterval)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::ProceedSearch3(unsigned int msInterval)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::ProceedSearch4(unsigned int msInterval)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::ProceedSearch5(unsigned int msInterval)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::ProceedSearchAStarStraightment(unsigned int msInterval)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::ProceedSearchDijkstraStraightment(unsigned int msInterval)
    {
        throw std::logic_error("Not implemented");
    }

    float Path::GetPathLength(int PathNum) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Path::GetSearchStatus() const
    {
        throw std::logic_error("Not implemented");
    }

    ai::eSearchResult Path::GetResult() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned int Path::GetSize() const
    {
        throw std::logic_error("Not implemented");
    }

    void Path::GetItem(unsigned int ItemNum, float* PathCoordx, float* PathCoordy) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Path::IsStraightPathValid(ai::Map* pMap, const CVector2& startPoint, const CVector2& endPoint, float radius,
        unsigned char blockedValue)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::_Init()
    {
        throw std::logic_error("Not implemented");
    }

    void Path::StartSearchInternal()
    {
        throw std::logic_error("Not implemented");
    }

    void Path::ClearHelpers()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned int Path::MapIndexToId(const ai::MapIndex& v) const
    {
        throw std::logic_error("Not implemented");
    }

    void Path::IdToMapIndex(unsigned int id, ai::MapIndex& res) const
    {
        throw std::logic_error("Not implemented");
    }

    void Path::StraightenPath(bool bForDijkstra)
    {
        throw std::logic_error("Not implemented");
    }

    void Path::FilterPath()
    {
        throw std::logic_error("Not implemented");
    }
}

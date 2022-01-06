#include "roadmanager.h"
#include <stdexcept>

namespace m3d
{
    CStr const RoadManager::GetRoadSetNameByHandle(int)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::UpdateVis()
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::ReleaseCollisionForRoadNode(RoadNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::RebuildStructures()
    {
        throw std::logic_error("Not implemented");
    }

    int RoadManager::GetRoadSetHandleByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::GetRoadMinMaxZByHandle(int, int, float&, float&)
    {
        throw std::logic_error("Not implemented");
    }

    RoadManager::RoadManager()
    {
        throw std::logic_error("Not implemented");
    }

    int RoadManager::WriteRoadsToXmlFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::GetRoadMinMaxXByHandle(int, int, float&, float&)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::UnlinkRoadNodeCollisionFromCells(RoadNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int RoadManager::ReadRoadSetConfigFromXmlFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int RoadManager::ReadRoadsFromXmlFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::Release()
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::Init()
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::ClearRoadSets()
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::ReleaseCollision()
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::RecalcCoveredCells()
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::LinkToBorder(RoadNode*, unsigned, int, CVector&)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::LinkRoadNodes()
    {
        throw std::logic_error("Not implemented");
    }

    CVector2 RoadManager::FindLeftProjection(RoadNode*, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    bool RoadManager::GetAdjPoint(RoadNode*, unsigned, int, CVector&)
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::FindFriends()
    {
        throw std::logic_error("Not implemented");
    }

    void RoadManager::CalcNodeData(RoadNode*)
    {
        throw std::logic_error("Not implemented");
    }
}

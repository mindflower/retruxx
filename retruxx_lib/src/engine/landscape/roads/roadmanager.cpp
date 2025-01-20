#include "roadmanager.h"
#include <stdexcept>

namespace m3d
{
    CStr const RoadManager::GetRoadSetNameByHandle(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::UpdateVis()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::ReleaseCollisionForRoadNode(RoadNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::RebuildStructures()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int RoadManager::GetRoadSetHandleByName(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::RebuildSomeNodes(retruxx::set<RoadNode*>, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::SetOwner(Landscape* landscape)
    {
        m_owner = landscape;
    }

    int RoadManager::RenderRoads(retruxx::vector<unsigned>&, RenderRoadType, RoadTestCallBack const*, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::GetRoadMinMaxZByHandle(int, int, float&, float&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    RoadManager::RoadManager()
    {
    }

    int RoadManager::WriteRoadsToXmlFile(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::GetRoadMinMaxXByHandle(int, int, float&, float&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::UnlinkRoadNodeCollisionFromCells(RoadNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int RoadManager::ReadRoadSetConfigFromXmlFile(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int RoadManager::ReadRoadsFromXmlFile(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::Init()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::ClearRoadSets()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::ReleaseCollision()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::RecalcCoveredCells()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::LinkToBorder(RoadNode*, unsigned, int, CVector&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::LinkRoadNodes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector2 RoadManager::FindLeftProjection(RoadNode*, float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool RoadManager::GetAdjPoint(RoadNode*, unsigned, int, CVector&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::FindFriends()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::CalcNodeData(RoadNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }
}

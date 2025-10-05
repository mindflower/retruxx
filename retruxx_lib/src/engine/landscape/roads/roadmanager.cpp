#include "roadmanager.h"
#include <stdexcept>
#include <core/kernel.h>
#include <config.h>
#include <landscape.h>
#include <core/timer.h>
#include <world.h>
#include <level.h>
#include "road.h"

namespace m3d
{
    CStr const RoadManager::GetRoadSetNameByHandle(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::UpdateVis()
    {
        // TODO: check and refactor this
        if (this->m_coveredCells)
        {
            auto m_f = M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF();
            auto v4 = (int)(float)((float)(m_f * 8.0) + 4.0);
            if (v4 >= 4)
            {
                if (v4 > 12)
                    v4 = 12;
            }
            else
            {
                v4 = 4;
            }

            auto& v5 = m3d::g_Kernel->GetTimer();
            
            auto v7 = m_owner->m_owner;
            
            auto curFrame = v5.GetCurFrame();

            auto levelsize = v7->m_level->land_size;
            auto* Graph = m_owner->GetGraph();
            Graph->SortedCellsStartFetching(0, v4);

            int x = 0;
            int z = 0;
            int v = 0;
            int radius = 0;
            if (Graph->SortedCellsFetch(x, z, v, radius))
            {
                do
                {
                    if (v)
                    {
                        auto& v10 = this->m_coveredCells[x + levelsize * z];
                        for (int i = 0; i < v10.size(); ++i)
                        {
                            if (m_owner->m_frustumCull.testSphere(v10[i]->m_boundCenter, v10[i]->m_boundRadius))
                            {
                                v10[i]->m_frameVisible = curFrame;
                            }
                        }
                    }
                } while (Graph->SortedCellsFetch(x, z, v, radius));
            }
        }
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
        this->m_owner = 0;
        this->m_coveredCells = 0;
        this->m_roadRoot = 0;
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
        // TODO: implement RoadManager::ReadRoadSetConfigFromXmlFile
        return 0;
        throw retruxx::logic_error("Not implemented");
    }

    int RoadManager::ReadRoadsFromXmlFile(char const*)
    {
        // TODO: implement RoadManager::ReadRoadsFromXmlFile
        return 0;
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RoadManager::Init()
    {
        M3D_ASSERT(m_roadRoot == nullptr);
        m_roadRoot = (m3d::RoadNode*)M3D_KERNEL->New("RoadNode");

        M3D_ASSERT(m_coveredCells == nullptr);
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

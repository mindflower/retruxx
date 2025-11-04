#include "roadmanager.h"
#include <stdexcept>
#include <core/kernel.h>
#include <config.h>
#include <landscape.h>
#include <core/timer.h>
#include <world.h>
#include <level.h>

#include "geomobject.h"
#include "road.h"

namespace m3d
{
    CStr const RoadManager::GetRoadSetNameByHandle(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::UpdateVis()
    {
        // TODO: implement RoadManager::UpdateVis
        return;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::RebuildStructures()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int RoadManager::GetRoadSetHandleByName(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::RebuildSomeNodes(retruxx::set<RoadNode*>, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::SetOwner(Landscape* landscape)
    {
        m_owner = landscape;
    }

    int RoadManager::RenderRoads(retruxx::vector<unsigned>&, RenderRoadType, RoadTestCallBack const*, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::GetRoadMinMaxZByHandle(int, int, float&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RoadSet::~RoadSet()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RoadManager::RoadManager()
    {
        this->m_owner = 0;
        this->m_coveredCells = 0;
        this->m_roadRoot = 0;
    }

    int RoadManager::WriteRoadsToXmlFile(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::GetRoadMinMaxXByHandle(int, int, float&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::UnlinkRoadNodeCollisionFromCells(RoadNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int RoadManager::ReadRoadSetConfigFromXmlFile(char const*)
    {
        // TODO: implement RoadManager::ReadRoadSetConfigFromXmlFile
        return 0;
        RETRUXX_NOT_IMPLEMENTED;
    }

    int RoadManager::ReadRoadsFromXmlFile(char const*)
    {
        // TODO: implement RoadManager::ReadRoadsFromXmlFile
        return 0;
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::Release()
    {
        if (m_roadRoot)
        {
            m_roadRoot->RemoveAllChildren();
        }

        ClearRoadSets();

        delete m_roadRoot;
        m_roadRoot = nullptr;

        delete m_coveredCells;
        m_coveredCells = nullptr;
    }

    void RoadManager::Init()
    {
        M3D_ASSERT(m_roadRoot == nullptr);
        m_roadRoot = (m3d::RoadNode*)M3D_KERNEL->New("RoadNode");

        M3D_ASSERT(m_coveredCells == nullptr);
    }

    void RoadManager::ClearRoadSets()
    {
        for (auto& roadSet : m_roadSets)
        {
            delete roadSet;
        }

        m_roadSets.clear();
    }

    void RoadManager::ReleaseCollision()
    {
       for (auto* child = dynamic_cast<m3d::RoadNode*>(m_roadRoot->GetFirstChild()); child; child = dynamic_cast<m3d::RoadNode*>(child->GetNextSibling()))
       {
           auto& geom = child->m_geomObject;
           if (geom)
           {
               geom->Release();
               delete geom;
               geom = nullptr;
           }
       }
    }

    void RoadManager::RecalcCoveredCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::LinkToBorder(RoadNode*, unsigned, int, CVector&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::LinkRoadNodes()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector2 RoadManager::FindLeftProjection(RoadNode*, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RoadManager::GetAdjPoint(RoadNode*, unsigned, int, CVector&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::FindFriends()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::CalcNodeData(RoadNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

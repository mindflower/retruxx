#include "intersectionmanager.h"
#include <m3dapp.h>
#include <stdexcept>
#include <math/vector.h>

#include "geomobject.h"
#include "landscape.h"
#include "objects/base/objcontainer.h"
#include "objects/physicbodies/geoms/sphereforintersection.h"
#include "server.h"
#include "world.h"
#include "core/kernel.h"
#include "core/log.h"
#include "objects/base/physicobj.h"
#include "objects/physicbodies/geoms/box.h"

namespace ai
{
    namespace
    {
        m3d::DbgCounter* cntIntersectionCalls = nullptr;
        m3d::DbgCounter* cntObjectsChecked = nullptr;
        m3d::DbgCounter* cntIntersectingObjectsChecked = nullptr;
        m3d::DbgCounter* cntObjectsSatisfied = nullptr;

        std::set<class ref_ptr<class ai::Obstacle>, struct std::less<class ref_ptr<class ai::Obstacle> >, class std::allocator<class ref_ptr<class ai::Obstacle> > >* tmpObstacles = nullptr;
        std::set<struct m3d::Class*, struct std::less<struct m3d::Class*>, class std::allocator<struct m3d::Class*> > const* tmpTargetClasses = nullptr;
        bool bPlayerPassCellCollided = false;

        void PushObstacle(ai::Obstacle* pOb)
        {
            if (!pOb)
            {
                return;
            }

            auto owner = pOb->GetOwner();
            if (!owner)
            {
                return;
            }

            auto ownerCls = owner->GetClass();
            auto it = tmpTargetClasses->find(ownerCls);
            if (it != tmpTargetClasses->end())
            {
                tmpObstacles->emplace(pOb);
            }
        }

        void IntersectionCallback(void* data,dxGeom* o1,dxGeom* o2)
        {
            // TODO: implement IntersectionCallback
            if ((dGeomIsSpace(o1) || dGeomIsSpace(o2)) && o1 != o2)
            {
                dSpaceCollide2(o1, o2, data, IntersectionCallback);
            }
            else
            {
                auto v5 = (ai::SphereForIntersection*)dGeomGetData(o1);
                auto v6 = (ai::SphereForIntersection*)dGeomGetData(o2);
            
                ai::Obstacle* id = 0;
                if (v5)
                    id = v5->GetOwner();
                if (v6)
                {
                    auto otherId = v6->GetOwner();
                    PushObstacle(id);
                    PushObstacle(otherId);
                }
                else
                {
                    PushObstacle(id);
                    PushObstacle(0);
                }
            }
        }
    }

    bool IntersectionManager::SpheresIntersect(const CVector&, float, const CVector, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void IntersectionManager::GetIntersectedObjects(
        retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle>>, retruxx::allocator<ref_ptr<ai::
        Obstacle>>>& objIds, const ai::Sphere* pLookSphere,
        const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>>& targetClasses,
        bool bCheckBoxes, bool bCheckPlayerPassmap)
    {
        ai::IntersectionManager::_GetIntersectedObjectsCustom(
            objIds,
            pLookSphere,
            targetClasses,
            IntersectionCallback,
            bCheckBoxes,
            bCheckPlayerPassmap);
    }

    void IntersectionManager::GetIntersectedObjectsByKindOf(
        retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle>>, retruxx::allocator<ref_ptr<ai::
        Obstacle>>>& objIds, const ai::Sphere* pLookSphere,
        const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>>& targetClasses,
        bool bCheckBoxes)
    {
        throw std::logic_error("Not implemented");
    }

    bool IntersectionManager::IsSphereValid(Sphere const*, retruxx::set<m3d::Class*> const&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void IntersectionManager::Registration()
    {
        auto idx = m3d::Application::g_pApp->GetDbgCounterStack().AddCounter("GetIntersectedObjects() calls");
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            cntIntersectionCalls = m3d::Application::g_pApp->GetDbgCounterStack().GetCounter(idx);
        }
        cntIntersectionCalls->SetI(0);

        idx = m3d::Application::g_pApp->GetDbgCounterStack().AddCounter("objects checked for intersection");
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            cntObjectsChecked = m3d::Application::g_pApp->GetDbgCounterStack().GetCounter(idx);
        }
        cntObjectsChecked->SetI(0);

        idx = m3d::Application::g_pApp->GetDbgCounterStack().AddCounter("intersecting objects of them");
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            cntIntersectingObjectsChecked = m3d::Application::g_pApp->GetDbgCounterStack().GetCounter(idx);
        }
        cntIntersectingObjectsChecked->SetI(0);

        idx = m3d::Application::g_pApp->GetDbgCounterStack().AddCounter("objects satisfied intersection");
        if (idx < m3d::Application::g_pApp->GetDbgCounterStack().GetNumCounters())
        {
            cntObjectsSatisfied = m3d::Application::g_pApp->GetDbgCounterStack().GetCounter(idx);
        }
        cntObjectsSatisfied->SetI(0);
    }

    void IntersectionManager::AddLookingSphere(SphereForIntersection* lookingSphere)
    {
        lookingSphere->m_next = ai::IntersectionManager::m_firstLookingSphere;
        lookingSphere->m_prev = 0;
        if (ai::IntersectionManager::m_firstLookingSphere)
            ai::IntersectionManager::m_firstLookingSphere->m_prev = lookingSphere;
        ai::IntersectionManager::m_firstLookingSphere = lookingSphere;
    }

    void IntersectionManager::ClearCounters()
    {
        cntIntersectionCalls->SetI(0);
        cntObjectsChecked->SetI(0);
        cntIntersectingObjectsChecked->SetI(0);
        cntObjectsSatisfied->SetI(0);
    }

    void IntersectionManager::RemoveLookingSphere(SphereForIntersection*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void IntersectionManager::_GetIntersectedObjectsCustom(
        retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle>>, retruxx::allocator<ref_ptr<ai::
        Obstacle>>>& objIds, const ai::Sphere* pLookSphere,
        const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>>& targetClasses,
        void(*nearCallback)(void*, dxGeom*, dxGeom*), bool bCheckBoxes, bool bCheckPlayerPassmap)
    {
        // TODO: generated code
        if (!pLookSphere)
        {
            return;
        }

        tmpObstacles = &objIds;
        tmpTargetClasses = &targetClasses;
        bPlayerPassCellCollided = false;

        // Clear the result set
        objIds.clear();

        // Get sphere properties
        CVector lookCenter = (float*)dGeomGetPosition(pLookSphere->GetGeomId());
        float lookRadius = pLookSphere->GetRadius();

        // Calculate grid cells to check
        ai::Geom::CellAabb cellAabb = pLookSphere->CountCellAabb();

        m3d::Landscape* landscape = &ai::pServer->GetWorld()->GetLandscape();

        // Iterate through grid cells
        for (int x = cellAabb.x0; x <= cellAabb.x1; ++x)
        {
            for (int z = cellAabb.z0; z <= cellAabb.z1; ++z)
            {
                m3d::Landscape::CollisionCellItem* cellItem = landscape->GetCollisionCellItem(x, z);
                if (!cellItem)
                {
                    continue;
                }

                // Check physic objects in this cell
                for (int objId : cellItem->m_physicObjIds)
                {
                    ai::cntObjectsChecked->IncI();
                    m3d::Object* obj = ai::theObjects->GetEntityByObjId(objId);
                    if (obj)
                    {
                        if (obj->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
                        {
                            auto* object = (PhysicObj*)obj;
                            // Check sphere intersection with physic object
                            if (object->m_intersectionObstacle)
                            {
                                if (object->m_intersectionObstacle->bIsEnabled())
                                {
                                    auto objSphere = object->m_intersectionObstacle->GetSphere();
                                    CVector sphereCenter = (float*)dGeomGetPosition(objSphere->GetGeomId());
                                    float sphereRadius = objSphere->GetRadius();

                                    CVector diff;
                                    diff.x = sphereCenter.x - lookCenter.x;
                                    diff.y = sphereCenter.y - lookCenter.y;
                                    diff.z = sphereCenter.z - lookCenter.z;

                                    float distanceSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
                                    float combinedRadius = sphereRadius + lookRadius;

                                    cntIntersectingObjectsChecked->IncI();

                                    if (distanceSq <= (combinedRadius * combinedRadius))
                                    {
                                        cntObjectsSatisfied->IncI();
                                        nearCallback(nullptr, pLookSphere->GetGeomId(), objSphere->m_geomId);
                                    }
                                }
                            }
                        }
                        else
                        {
                            M3D_LOG_ERR("Error: not PhysicObj is linked to collision cell x = " + CStr(x) + ", y = " + CStr(z) + ", id = " + CStr(objId));
                        }
                    }
                    else
                    {
                        M3D_LOG_ERR("Error: NULL object is linked to collision cell x = " + CStr(x) + ", y = " + CStr(z) + ", id = " + CStr(objId));
                    }
                }

                // Check obstacles in this cell
                for (const auto& obstacle : (*cellItem->m_obstacles))
                {
                    cntObjectsChecked->IncI();

                    if (!obstacle)
                    {
                        // Log error: NULL obstacle
                        M3D_LOG_ERR("Error: NULL obstacle is linked to collision cell x = " + CStr(x) + ", y = " + CStr(z));
                        continue;
                    }

                    if (obstacle->bIsEnabled())
                    {
                        auto* obstacleSphere = obstacle->GetSphere();
                        CVector sphereCenter = (float*)dGeomGetPosition(obstacleSphere->GetGeomId());
                        float sphereRadius = obstacleSphere->GetRadius();

                        CVector diff;
                        diff.x = sphereCenter.x - lookCenter.x;
                        diff.y = sphereCenter.y - lookCenter.y;
                        diff.z = sphereCenter.z - lookCenter.z;

                        float distanceSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
                        float combinedRadius = sphereRadius + lookRadius;

                        cntIntersectingObjectsChecked->IncI();

                        if (distanceSq <= (combinedRadius * combinedRadius))
                        {
                            cntObjectsSatisfied->IncI();

                            auto* obstacleBox = obstacle->GetBox();

                            if (!bCheckBoxes || !obstacleBox ||
                                dCollide(pLookSphere->GetGeomId(), obstacleBox->GetGeomId(), 1, nullptr, 104))
                            {
                                nearCallback(nullptr, pLookSphere->GetGeomId(), obstacleSphere->GetGeomId());
                            }
                        }
                    }
                }

                // Check player passmap if requested
                if (bCheckPlayerPassmap)
                {
                    for (auto* geomObject : cellItem->m_geomsList)
                    {
                        if (geomObject->IsKindOf(&m3d::GeomObjectPassCell::m_classGeomObjectPassCell) &&
                            dCollide(pLookSphere->GetGeomId(), geomObject->GetGeom(), 1, nullptr, 104))
                        {
                            bPlayerPassCellCollided = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}

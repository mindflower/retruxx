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

        std::set<
            class ref_ptr<class ai::Obstacle>,
            struct std::less<class ref_ptr<class ai::Obstacle>>,
            class std::allocator<class ref_ptr<class ai::Obstacle>>>* tmpObstacles = nullptr;
        std::set<struct m3d::Class*, struct std::less<struct m3d::Class*>, class std::allocator<struct m3d::Class*>> const*
            tmpTargetClasses = nullptr;
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

            if (tmpTargetClasses->empty())
            {
                tmpObstacles->emplace(pOb);
            }
            else
            {
                auto ownerCls = owner->GetClass();
                auto it = tmpTargetClasses->find(ownerCls);
                if (it != tmpTargetClasses->end())
                {
                    tmpObstacles->emplace(pOb);
                }
            }
        }

        void IntersectionCallback(void* data, dxGeom* o1, dxGeom* o2)
        {
            if ((dGeomIsSpace(o1) || dGeomIsSpace(o2)) && o1 != o2)
            {
                dSpaceCollide2(o1, o2, data, IntersectionCallback);
            }
            else
            {
                auto sphere1 = static_cast<SphereForIntersection*>(dGeomGetData(o1));
                auto sphere2 = static_cast<SphereForIntersection*>(dGeomGetData(o2));

                ai::Obstacle* id = nullptr;
                if (sphere1)
                {
                    id = sphere1->GetOwner();
                }
                if (sphere2)
                {
                    auto otherId = sphere2->GetOwner();
                    PushObstacle(id);
                    PushObstacle(otherId);
                }
                else
                {
                    PushObstacle(id);
                    PushObstacle(nullptr);
                }
            }
        }
    }  // namespace

    bool IntersectionManager::SpheresIntersect(CVector const&, float, CVector const, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void IntersectionManager::GetIntersectedObjects(
        retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle>>, retruxx::allocator<ref_ptr<ai::Obstacle>>>& objIds,
        ai::Sphere const* pLookSphere,
        retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const& targetClasses,
        bool bCheckBoxes,
        bool bCheckPlayerPassmap)
    {
        ai::IntersectionManager::_GetIntersectedObjectsCustom(
            objIds, pLookSphere, targetClasses, IntersectionCallback, bCheckBoxes, bCheckPlayerPassmap);
    }

    void IntersectionManager::GetIntersectedObjectsByKindOf(
        retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle>>, retruxx::allocator<ref_ptr<ai::Obstacle>>>& objIds,
        ai::Sphere const* pLookSphere,
        retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> const& targetClasses,
        bool bCheckBoxes)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool IntersectionManager::IsSphereValid(
        Sphere const* pIntersectionSphere,
        retruxx::set<m3d::Class*> const& targetClasses,
        bool bCheckPlayerPassmap)
    {
        retruxx::set<ref_ptr<ai::Obstacle>> objIds;
        _GetIntersectedObjectsCustom(objIds, pIntersectionSphere, targetClasses, IntersectionCallback, true, bCheckPlayerPassmap);
        return objIds.empty() && (!bCheckPlayerPassmap || !bPlayerPassCellCollided);
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

    void IntersectionManager::RemoveLookingSphere(SphereForIntersection* lookingSphere)
    {
        auto prev = lookingSphere->m_prev;
        if (prev)
            prev->m_next = lookingSphere->m_next;
        else
            m_firstLookingSphere = lookingSphere->m_next;

        auto next = lookingSphere->m_next;
        if (next)
            next->m_prev = lookingSphere->m_prev;
    }

    void IntersectionManager::_GetIntersectedObjectsCustom(
        retruxx::set<ref_ptr<ai::Obstacle>>& objIds,
        ai::Sphere const* pLookSphere,
        retruxx::set<m3d::Class*> const& targetClasses,
        void (*nearCallback)(void*, dxGeom*, dxGeom*),
        bool bCheckBoxes,
        bool bCheckPlayerPassmap)
    {
        // TODO: generated code
        if (!pLookSphere)
        {
            return;
        }

        tmpObstacles = &objIds;
        tmpTargetClasses = &targetClasses;
        bPlayerPassCellCollided = false;

        cntIntersectionCalls->IncI();

        // Clear the result set
        objIds.clear();

        // Get sphere properties
        CVector lookCenter = (float*)dGeomGetPosition(pLookSphere->GetGeomId());
        float const lookRadius = pLookSphere->GetRadius();

        // Calculate grid cells to check
        auto const cellAabb = pLookSphere->CountCellAabb();

        m3d::Landscape& landscape = pServer->GetWorld()->GetLandscape();

        // Iterate through grid cells
        for (int x = cellAabb.x0; x <= cellAabb.x1; ++x)
        {
            for (int z = cellAabb.z0; z <= cellAabb.z1; ++z)
            {
                auto* cellItem = landscape.GetCollisionCellItem(x, z);
                if (!cellItem)
                {
                    continue;
                }

                // Check physic objects in this cell
                for (int objId : cellItem->m_physicObjIds)
                {
                    cntObjectsChecked->IncI();

                    auto* obj = theObjects->GetEntityByObjId(objId);
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
                                    auto* objSphere = object->m_intersectionObstacle->GetSphere();
                                    CVector sphereCenter = (float*)dGeomGetPosition(objSphere->GetGeomId());
                                    float sphereRadius = objSphere->GetRadius();

                                    CVector diff;
                                    diff.x = sphereCenter.x - lookCenter.x;
                                    diff.y = sphereCenter.y - lookCenter.y;
                                    diff.z = sphereCenter.z - lookCenter.z;

                                    float distanceSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
                                    float combinedRadius = sphereRadius + lookRadius;

                                    cntIntersectingObjectsChecked->IncI();

                                    if ((combinedRadius * combinedRadius) > distanceSq)
                                    {
                                        cntObjectsSatisfied->IncI();
                                        nearCallback(nullptr, pLookSphere->GetGeomId(), objSphere->m_geomId);
                                    }
                                }
                            }
                        }
                        else
                        {
                            M3D_LOG_ERR(
                                "Error: not PhysicObj is linked to collision cell x = " + CStr(x) + ", y = " + CStr(z) +
                                ", id = " + CStr(objId));
                        }
                    }
                    else
                    {
                        M3D_LOG_ERR(
                            "Error: NULL object is linked to collision cell x = " + CStr(x) + ", y = " + CStr(z) + ", id = " + CStr(objId));
                    }
                }

                // Check obstacles in this cell
                for (auto const& obstacle : *cellItem->m_obstacles)
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

                        if ((combinedRadius * combinedRadius) > distanceSq)
                        {
                            cntObjectsSatisfied->IncI();

                            auto* obstacleBox = obstacle->GetBox();

                            dContact contact;
                            if (!bCheckBoxes || !obstacleBox ||
                                dCollide(pLookSphere->GetGeomId(), obstacleBox->GetGeomId(), 1, &contact.geom, 104))
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
                        dContact contact;
                        if (geomObject->IsKindOf(&m3d::GeomObjectPassCell::m_classGeomObjectPassCell) &&
                            dCollide(pLookSphere->GetGeomId(), geomObject->GetGeom(), 1, &contact.geom, 104))
                        {
                            bPlayerPassCellCollided = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}  // namespace ai

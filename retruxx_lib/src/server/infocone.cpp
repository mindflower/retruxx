#include "infocone.h"

#include "config.h"
#include "m3dapp.h"
#include "math/matrix.h"
#include "objects/physicbodies/physichelpers.h"

#include <stdexcept>

#include "objects/physicbodies/geoms/ray.h"
#include "objects/physicbodies/geoms/sphere.h"
#include "server.h"
#include "world.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/timer.h"
#include "objects/staticautogun.h"
#include "objects/vehicle.h"
#include "objects/base/globalproperties.h"
#include "objects/monsters/boss02.h"
#include "objects/monsters/boss03.h"
#include "objects/monsters/boss04.h"
#include "objects/monsters/boss04drone.h"
#include "objects/monsters/bossmetalarm.h"
#include "objects/physicbodies/vehiclepart.h"

#include <algorithm>
#include <client.h>
#include <ode/objects.h>
#include "ode/odecpp.h"

namespace ai
{
    InfoCone::InfoCone(ai::InfoCone const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfoCone::InfoCone()
    {
        this->m_vehicleId = -1;
        this->m_cameraPos = CVector(0.0, 0.0, 0.0);
        this->m_lookAt = CVector(0.0, 0.0, 0.0);
        this->m_lookVector = CVector(0.0, 0.0, 0.0);
        this->m_lookDir = CVector(0.0, 0.0, 0.0);
        this->m_lookDistance = 0.0;
        this->m_halfConeCos = 1.0;
        this->m_sphereRadiusCoef = 0.0;
        this->m_sphere = ai::Sphere::CreateObject(0, 1.0, 0);
        this->m_ray = ai::Ray::CreateObject(0, 1.0, 0);
    }

    InfoCone::~InfoCone()
    {
        delete m_sphere;
        delete m_ray;
    }

    bool InfoCone::SetVehicleId(int vehicleId)
    {
        // TODO: generated code InfoCone::SetVehicleId
        // Check if mouse is pointing at something
        if (!M3D_APP->bIsMousePointing())
        {
            return false;
        }

        // Get mouse hit point in world space
        m3d::SgNode* hitNode = nullptr;
        M3D_APP->GetMouseHitPoint(m_lookAt, hitNode);

        // Store current camera position
        m_cameraPos = M3D_APP->m_curCamera.m_worldOrigin;

        CVector hitPoint = m_lookAt;
        bool hasValidHit = (hitNode != nullptr);

        // If no valid hit point, create a point far in front of the camera
        if (!hasValidHit)
        {
            // Get camera rotation matrix
            CMatrix cameraRot;
            cameraRot.rotYPR(M3D_APP->m_curCamera.m_rotYaw, M3D_APP->m_curCamera.m_rotPitch, M3D_APP->m_curCamera.m_rotRoll);

            CVector INITIAL_OBJECTS_DIRECTION = {0.0, 0.0, 1.0};
            // Calculate a point far in the camera's forward direction
            CVector farPoint;
            farPoint.x = (((cameraRot._13 * INITIAL_OBJECTS_DIRECTION.z) + (cameraRot._12 * INITIAL_OBJECTS_DIRECTION.y)) +
                     (cameraRot._11 * INITIAL_OBJECTS_DIRECTION.x)) *
                1000000.0;
            farPoint.y = (((cameraRot._23 * INITIAL_OBJECTS_DIRECTION.z) + (cameraRot._22 * INITIAL_OBJECTS_DIRECTION.y)) +
                     (cameraRot._21 * INITIAL_OBJECTS_DIRECTION.x)) *
                1000000.0;
            farPoint.z = (((cameraRot._33 * INITIAL_OBJECTS_DIRECTION.z) + (cameraRot._32 * INITIAL_OBJECTS_DIRECTION.y)) +
                     (cameraRot._31 * INITIAL_OBJECTS_DIRECTION.x)) *
                1000000.0;

            hitPoint = farPoint;
        }
        else
        {
            // Try to trace to the hit point with a maximum distance
            CVector traceResult;
            if (!TraceTo(hitPoint, traceResult, 5000.0f))
            {
                // If trace fails, use the original hit point
                hitPoint = m_lookAt;
            }
            else
            {
                // Use the traced point
                hitPoint = traceResult;
            }
        }

        // Update the look-at point
        m_lookAt = hitPoint;
        
        // Store vehicle ID
        m_vehicleId = vehicleId;

        // Calculate look vector from camera to hit point
        m_lookVector = m_lookAt - m_cameraPos;

        // Normalize look direction
        m_lookDir = m_lookVector.getNormalized();

        float distance = m_lookVector.length();

        // Store look distance with a small offset
        m_lookDistance = distance + 10.0f;

        // Scale look vector to the actual distance
        m_lookVector = m_lookVector.getNormalized() * m_lookDistance;

        float infoAreaRadius = theGlobProp.m_infoAreaRadius;
        float halfConeRightEdge = 0.0;
        M3D_RENDERER->RelToAbs(infoAreaRadius, halfConeRightEdge);

        // Calculate cone properties for visibility testing
        // Get mouse position
        float mouseX = M3D_APP->GetMouseX() + infoAreaRadius;
        float mouseY = M3D_APP->GetMouseY();

        // Convert mouse position to world space ray
        CVector2 mouseVec2 = CVector2(mouseX, mouseY);
        CVector mouseWorldRay = M3D_RENDERER->Unproject(mouseVec2);

        // Normalize the mouse ray
        mouseWorldRay = mouseWorldRay.getNormalized();

        // Calculate cosine of half-cone angle (dot product between look direction and mouse ray)
        m_halfConeCos = m_lookDir.z * mouseWorldRay.z + (m_lookDir.y * mouseWorldRay.y) + (m_lookDir.x * mouseWorldRay.x);
        m_sphere->SetRadius(0.1f);

        m_sphereRadiusCoef = sqrt(1.0 / (m_halfConeCos * m_halfConeCos) - 1.0);

        return true;
    }

    int InfoCone::GetInfoObjId() const
    {
        // TODO: check this!!
        auto& landscape = pServer->GetWorld()->GetLandscape();
        auto& graph = m3d::pClient->GetWorld().GetGraph();

        int const prevFrame = M3D_KERNEL->GetTimer().GetCurFrame() - 1;
        float const distanceDivider = M3D_ENGINE_CFG.m_lsViewDistanceDivider.GetF();
        float const endRadius = std::clamp(distanceDivider * 8.0 + 4.0, 4.0, 12.0);

        if (!graph.SortedCellsStartFetching(0, endRadius))
        {
            return -1;
        }

        int infoObjId = -1;
        float maxAngleCosFromContact = 0.0f;

        int cellX = 0;
        int cellZ = 0;
        int vis = 0;
        int radius = 0;
        while (graph.SortedCellsFetch(cellX, cellZ, vis, radius))
        {
            if (vis == 0)
            {
                continue;
            }

            auto* collisionItem = landscape.GetCollisionCellItem(cellX, cellZ);
            if (collisionItem == nullptr)
            {
                M3D_LOG_INFO("Warning: null collision cell item, cellX = " + CStr(cellX) + ", cellZ = " + CStr(cellZ));
                continue;
            }

            for (int const objId : collisionItem->m_physicObjIds)
            {
                auto* obj = theObjects->GetEntityByObjId(objId);
                if (auto* vehicle = RT_DYNCAST(obj, Vehicle); vehicle && vehicle->IsTrailer())
                {
                    auto* parent = vehicle->GetParent();
                    if (!parent || !IS_KIND_OF(parent, PhysicObj))
                    {
                        continue;
                    }
                    obj = parent;
                }

                auto* physicObj = RT_DYNCAST(obj, PhysicObj);
                if (physicObj->GetId() == m_vehicleId || !physicObj->GetBody() ||
                    !(IS_KIND_OF(physicObj, Vehicle) || IS_KIND_OF(physicObj, StaticAutoGun) || IS_KIND_OF(physicObj, BossMetalArm) ||
                      IS_KIND_OF(physicObj, Boss02) || IS_KIND_OF(physicObj, Boss03) || IS_KIND_OF(physicObj, Boss04) || IS_KIND_OF(physicObj, Boss04Drone)))
                {
                    continue;
                }
                m3d::SgNode* node = nullptr;
                if (auto* complexPhysObj = RT_DYNCAST(physicObj, ComplexPhysicObj))
                {
                    node = complexPhysObj->begin()->second->m_Node;
                }
                else if (auto* simplePhysObj = RT_DYNCAST(physicObj, SimplePhysicObj))
                {
                    node = simplePhysObj->GetPhysicBody()->m_Node;
                }

                if (!node || node->m_frameVisible != prevFrame)
                {
                    continue;
                }

                // TODO: generated code
                // Check collision with each geometry in the vehicle body
                for (dxGeom* geom = dBodyGetFirstGeom(physicObj->GetBody()->id()); geom; geom = dGeomGetBodyNext(geom))
                {
                    if (!dGeomIsEnabled(geom))
                    {
                        continue;
                    }

                    dxSpace* space = dGeomGetSpace(geom);
                    if (!space || space == ai::gIntersectionSpace)
                    {
                        continue;
                    }

                    // Get geometry position and calculate direction to it
                    float const* geomPos = dGeomGetPosition(geom);
                    CVector objDirection(geomPos[0] - m_cameraPos.x, geomPos[1] - m_cameraPos.y, geomPos[2] - m_cameraPos.z);

                    // Calculate projection onto look vector
                    float projection = (objDirection.x * m_lookVector.x + objDirection.y * m_lookVector.y + objDirection.z * m_lookVector.z) / m_lookDistance;

                    // Check if object is within the view cone
                    if (projection >= 0.0f && projection <= m_lookDistance)
                    {
                        // Calculate sphere position along look direction
                        CVector spherePos(
                            m_cameraPos.x + m_lookDir.x * projection, m_cameraPos.y + m_lookDir.y * projection, m_cameraPos.z + m_lookDir.z * projection);

                        // Set up collision sphere
                        float sphereRadius = projection * m_sphereRadiusCoef;
                        m_sphere->SetPosition(spherePos);
                        m_sphere->SetRadius(sphereRadius);

                        // Check collision between object geometry and our sphere
                        dContact contact;
                        if (dCollide(geom, m_sphere->GetGeomId(), 1, &contact.geom, sizeof(dContact)))
                        {
                            // Calculate intersection point and direction
                            CVector toIntersection(
                                contact.geom.pos[0] - m_cameraPos.x, contact.geom.pos[1] - m_cameraPos.y, contact.geom.pos[2] - m_cameraPos.z);

                            float intersectionDistance = toIntersection.length() - 5.0f;
                            float scaleFactor = (intersectionDistance >= 1.0f) ? intersectionDistance : 1.0f;

                            // Create destination point for line-of-sight check
                            CVector normalizedDir = toIntersection.getNormalized();
                            CVector dst = m_cameraPos + normalizedDir * scaleFactor;

                            // Check if there's clear line of sight
                            CVector hitPoint;
                            if (!TraceTo(dst, hitPoint, 0.0f))
                            {
                                // Calculate angle between look direction and object direction
                                CVector lookDirNormalized = m_lookVector.getNormalized();
                                CVector objDirNormalized = objDirection.getNormalized();
                                float angleCos =
                                    lookDirNormalized.z * objDirNormalized.z + lookDirNormalized.y * objDirNormalized.y +
                                    lookDirNormalized.x * objDirNormalized.x;

                                // Update best candidate if this object has better alignment
                                if (angleCos > maxAngleCosFromContact)
                                {
                                    maxAngleCosFromContact = angleCos;
                                    infoObjId = physicObj->GetId();
                                }
                            }
                        }
                    }
                }
            }
        }
        return infoObjId;
    }

    void InfoCone::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool InfoCone::TraceTo(CVector const& dst, CVector& newDst, float length) const
    {
        // TODO: generated code InfoCone::TraceTo
        // Set ray origin to camera position
        dGeomSetPosition(m_ray->GetGeomId(), m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);

        // Calculate direction vector from camera to destination
        CVector direction = dst - m_cameraPos;

        // Normalize direction vector
        float directionLength = direction.length();
        if (directionLength > 0.0f)
        {
            direction = direction * (1.0f / directionLength);
        }
        else
        {
            // If direction is zero, use a default forward direction
            direction = CVector(0.0f, 0.0f, 1.0f);
            directionLength = 0.0f;
        }

        // Set ray direction
        m_ray->SetDirection(direction);

        // Set ray length
        if (length >= 0.1f)
        {
            // Use specified maximum length
            m_ray->SetLength(length);
        }
        else
        {
            // Use actual distance to destination
            m_ray->SetLength(directionLength);
        }

        // Perform ray trace
        dContact contact;
        bool hit = ai::TraceLine(*m_ray, contact, true, true, true, false, nullptr, true, false);

        if (!hit)
        {
            return false;
        }

        // Store hit point
        newDst.x = contact.geom.pos[0];
        newDst.y = contact.geom.pos[1];
        newDst.z = contact.geom.pos[2];

        return true;
    }
}  // namespace ai

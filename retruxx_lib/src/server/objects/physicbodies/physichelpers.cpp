#define NOMINMAX
#include "physichelpers.h"

#include <ode/objects.h>

#include "geomobject.h"
#include "landscape.h"
#include "physicbody.h"
#include "world.h"
#include "core/kernel.h"
#include "core/log.h"
#include "math/coremath.h"
#include "ode/odecpp.h"
#include "server/server.h"
#include "server/objects/blastwave.h"
#include "server/objects/player.h"
#include "server/objects/town.h"
#include "server/objects/base/shell.h"
#include "server/objects/vehicle.h"
#include <algorithm>

namespace ai
{
    CollisionInfo::CollisionInfo(const ai::CollisionInfo& info)
    {
        this->m_geomType = info.m_geomType;
        this->m_relTranslation = info.m_relTranslation;
        this->m_relRotation = info.m_relRotation;
        this->m_size = info.m_size;
        this->m_radius = info.m_radius;
        this->m_trimeshVertices = info.m_trimeshVertices;
        this->m_trimeshIndices = info.m_trimeshIndices;
    }

    CollisionInfo::CollisionInfo()
    {
        Init();
    }

    void CollisionInfo::Init()
    {
        this->m_geomType = GEOM_TYPE_NONE;
        this->m_relTranslation.x = 0.0;
        this->m_relTranslation.y = 0.0;
        this->m_relTranslation.z = 0.0;
        this->m_relRotation.x = 0.0;
        this->m_relRotation.y = 0.0;
        this->m_relRotation.z = 0.0;
        this->m_relRotation.w = 1.0;
        this->m_size.x = 0.0;
        this->m_size.y = 0.0;
        this->m_size.z = 0.0;
        this->m_radius = 0.0;

        m_trimeshVertices = {};
        m_trimeshIndices = {};
    }

    int ai::RoughSign(float value)
    {
        if (value > 0.000001)
            return 1;
        if (value >= -0.000001)
            return 0;
        return -1;
    }

    bool GetCollisionInfoByServerHandle(int, retruxx::vector<CollisionInfo>&, bool)
    {
        // TODO: impement GetCollisionInfoByServerHandle
        return false;
        throw std::logic_error("Not implemented");
    }

    void CommonGeomMovedCallback(dxGeom* geomId)
    {
        auto Data = (m3d::Object*)dGeomGetData(geomId);
        auto v2 = (ai::PhysicBody*)Data;
        if (Data)
        {
            PhysicObj* Owner = nullptr;
            if (Data->IsKindOf(&ai::PhysicBody::m_classPhysicBody))
            {
                v2->TransferPhysicParamsToSceneGraphNode();
                Owner = v2->GetOwner();
            }
            else
            {
                if (!v2->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
                    return;
                Owner = (ai::PhysicObj*)v2;
            }
            if (Owner)
            {
                if ((Owner->GetFlags() & 2) == 0)
                    Owner->RelinkGeomsToCollisionCells();
            }
        }
    }

    void SetNodeElapsedAnimationTimeInMs(m3d::SgNode*, int)
    {
        throw std::logic_error("Not implemented");
    }

    CVector ProjectVectorOntoPlane(CVector const& normal, CVector const& v)
    {
        auto v3 = 0.0;
        CVector result;
        auto v6 = normal.y * v.y + normal.x * v.x + v.z * normal.z;
        if (fabs(v6) >= 0.001)
        {
            v3 = v6;
        }
        float v7 = normal.x * v3;
        float v8 = v.z - (v3 * normal.z);
        float v9 = v.y - (normal.y * v3);
        result.x = v.x - v7;
        result.y = v9;
        result.z = v8;
        return result;
    }

    int TraceLine(ai::Ray const&, retruxx::vector<ai::Geom*> const&, dContact*)
    {
        throw std::logic_error("Not implemented");
    }

    namespace
    {
        bool IsLittle(dxGeom *)
        {
            throw std::logic_error("Not implemented");
        }
    }

    bool TraceLine(ai::Ray const& ray, dContact& closestContact, bool dontCollideWithDynamic, bool dontCollideWithLittle, bool dontCollideWithPlayer, bool dontCollideWithWater, ai::TraceLineCallback* callback, bool dontCollideWithShells, bool smartCollideWithTowns)
    {
        // TODO: generated code

        // Get ray start position and direction
        CVector start = *dGeomGetPosition(ray.GetGeomId());
        CVector dir = ray.GetDirection();
        float rayLength = ray.GetLength();

        // Calculate ray end point
        CVector end;
        end.x = start.x + dir.x * rayLength;
        end.y = start.y + dir.y * rayLength;
        end.z = start.z + dir.z * rayLength;

        // Get level size for bounds checking
        m3d::Landscape* landscape = &ai::pServer->GetWorld()->GetLandscape();
        int levelSize = static_cast<int>(ai::pServer->GetLevelSize());
        int maxCoord = levelSize - 1;

        // Clamp start coordinates to level bounds
        int startX = static_cast<int>(start.x);
        int startZ = static_cast<int>(start.z);
        startX = std::max(0, std::min(startX, maxCoord));
        startZ = std::max(0, std::min(startZ, maxCoord));

        // Create line for grid traversal
        CBrezLine line;
        line.start(startX, startZ, static_cast<int>(end.x), static_cast<int>(end.z));

        float minDistanceSq = 1.0e30f;
        bool foundContact = false;
        int prevCellX = -1;
        int prevCellZ = -1;

        // Validate cell edge size (debug check)
        int cellEdgeSize = 128.0f;
        M3D_ASSERT(cellEdgeSize == 128);

        // Temporary contact storage
        dContact contacts[8];

        // Get player vehicle for player collision filtering
        ai::Vehicle* playerVehicle = nullptr;
        if (dontCollideWithPlayer && ai::thePlayer)
        {
            playerVehicle = ai::thePlayer->GetVehicle();
        }

        // Traverse through grid cells along the ray
        int cellX = 0;
        int cellZ = 0;
        // TODO: check this
        while (line.step(cellX, cellZ))
        {
            // Check if we're still within level bounds
            if (cellX < 0 || cellX >= levelSize || cellZ < 0 || cellZ >= levelSize)
            {
                break;
            }

            // Convert to collision cell coordinates
            int collisionCellX = cellX >> 7; // Divide by 128
            int collisionCellZ = cellZ >> 7;

            // Only process if we moved to a new collision cell
            if (collisionCellX != prevCellX || collisionCellZ != prevCellZ)
            {
                prevCellX = collisionCellX;
                prevCellZ = collisionCellZ;

                // Get collision cell from landscape
                m3d::Landscape::CollisionCellItem* cellItem =
                    landscape->GetCollisionCellItem(collisionCellX, collisionCellZ);

                if (!cellItem)
                {
                    // Log warning about missing collision cell
                    M3D_LOG_INFO("Warning: null collision cell item, cellX = " +
                                 CStr(collisionCellX) +
                                 ", cellZ = " + CStr(collisionCellZ));
                    continue;
                }

                // Process static geometry in this cell
                for (auto* geomObject : cellItem->m_geomsList)
                {
                    if (!geomObject) continue;

                    // Apply collision filters
                    if (dontCollideWithLittle && IsLittle(geomObject->GetGeom()))
                    {
                        continue;
                    }
                    if (dontCollideWithWater &&
                        geomObject->IsKindOf(&m3d::GeomObjectWater::m_classGeomObjectWater))
                    {
                        continue;
                    }
                    if (geomObject->IsKindOf(&m3d::GeomObjectPassCell::m_classGeomObjectPassCell))
                    {
                        continue;
                    }

                    // Perform collision detection
                    int contactCount = dCollide(ray.GetGeomId(), geomObject->GetGeom(),
                                                8, &contacts[0].geom, sizeof(dContact));

                    // Process contacts
                    for (int i = 0; i < contactCount; i++)
                    {
                        const dContact& contact = contacts[i];
                        CVector contactPos(contact.geom.pos[0], contact.geom.pos[1], contact.geom.pos[2]);

                        // Verify contact is in the current cell
                        int contactCellX = static_cast<int>(contactPos.x) >> 7;
                        int contactCellZ = static_cast<int>(contactPos.z) >> 7;

                        if (contactCellX == collisionCellX && contactCellZ == collisionCellZ)
                        {
                            CVector delta = start;
                            delta -= contactPos;
                            float distanceSq = delta.lengthSq();

                            if (distanceSq < minDistanceSq)
                            {
                                minDistanceSq = distanceSq;
                                closestContact = contact;
                                foundContact = true;
                            }
                        }
                    }
                }

                // Process dynamic objects in this cell
                for (int objId : cellItem->m_physicObjIds)
                {
                    // Check callback filter
                    if (callback && !callback->CollideId(objId))
                    {
                        continue;
                    }

                    auto* object = dynamic_cast<PhysicObj*>(theObjects->GetEntityByObjId(objId));
                    if (!object)
                    {
                        // Log error about invalid object reference
                        M3D_LOG_ERR("Error: NULL object is linked to collision cell x = " +
                                    CStr(collisionCellX) + ", y = " +
                                    CStr(collisionCellZ) + ", id = " +
                                    CStr(objId));
                        continue;
                    }


                    // Apply various filters
                    if (callback && !callback->CollidePhysicObj(object))
                    {
                        continue;
                    }
                    if (!object->GetBody())
                    {
                        continue;
                    }
                    if (dontCollideWithDynamic && (object->GetPhysicState() & 4) != 0)
                    {
                        continue;
                    }
                    if (dontCollideWithShells &&
                        object->IsKindOf(&ai::Shell::m_classShell))
                    {
                        continue;
                    }
                    if ((object->GetPhysicState() & 2) != 0)
                    {
                        continue;
                    }
                    if (dontCollideWithPlayer)
                    {
                        if (object == playerVehicle) continue;
                        if (object->GetParent() == playerVehicle) continue;
                    }
                    if (object->IsKindOf(&ai::BlastWave::m_classBlastWave))
                    {
                        continue;
                    }

                    // Special handling for towns with smart collision
                    if (smartCollideWithTowns && object->IsKindOf(&ai::Town::m_classTown))
                    {
                        auto* protoInfo = dynamic_cast<Town*>(object)->GetPrototypeInfo();
                        unsigned int maxContacts = protoInfo->m_numCollisionLayersBelowVehicle;
                        if (maxContacts > 0)
                        {
                            std::vector<CVector> contactPoints;

                            // Collect all contact points with the town
                            dxGeom* geom = dBodyGetFirstGeom(object->GetBody()->id());
                            while (geom)
                            {
                                if (!IsLittle(geom))
                                {
                                    dxSpace* space = dGeomGetSpace(geom);
                                    if (space && space != ai::gIntersectionSpace && space != dGeomGetSpace(ray.GetGeomId()))
                                    {
                                        int contactCount = dCollide(ray.GetGeomId(), geom, 8, &contacts[0].geom, sizeof(dContact));

                                        for (int i = 0; i < contactCount; i++)
                                        {
                                            CVector contactPos(contacts[i].geom.pos[0], contacts[i].geom.pos[1], contacts[i].geom.pos[2]);
                                            contactPoints.push_back(contactPos);
                                        }
                                    }
                                }
                                geom = dGeomGetBodyNext(geom);
                            }

                            // Sort contacts by distance and use only the farthest ones (town optimization)
                            if (!contactPoints.empty())
                            {
                                std::sort(contactPoints.begin(), contactPoints.end(),
                                          [&](const CVector& a, const CVector& b)
                                {
                                    return (start - a).lengthSq() < (start - b).lengthSq();
                                });

                                // Use only the most distant contacts (town optimization)
                                size_t startIndex = 0;
                                if (contactPoints.size() > maxContacts)
                                {
                                    startIndex = contactPoints.size() - maxContacts;
                                }

                                for (size_t i = startIndex; i < contactPoints.size(); i++) {
                                    CVector delta = start - contactPoints[i];
                                    float distanceSq = delta.lengthSq();

                                    if (distanceSq < minDistanceSq)
                                    {
                                        minDistanceSq = distanceSq;
                                        closestContact.geom.pos[0] = contactPoints[i].x;
                                        closestContact.geom.pos[1] = contactPoints[i].y;
                                        closestContact.geom.pos[2] = contactPoints[i].z;
                                        closestContact.geom.pos[3] = 0.0f;
                                        closestContact.geom.g1 = 0;
                                        closestContact.geom.g2 = 0;
                                        foundContact = true;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // Regular object collision processing
                        dxGeom* geom = dBodyGetFirstGeom(object->GetBody()->id());
                        while (geom) {
                            if (!dontCollideWithLittle || !IsLittle(geom))
                            {
                                dxSpace* space = dGeomGetSpace(geom);
                                if (space && space != ai::gIntersectionSpace && space != dGeomGetSpace(ray.GetGeomId())) {
                                    int contactCount = dCollide(ray.GetGeomId(), geom, 8, &contacts[0].geom, sizeof(dContact));

                                    for (int i = 0; i < contactCount; i++) {
                                        CVector contactPos(contacts[i].geom.pos[0], contacts[i].geom.pos[1], contacts[i].geom.pos[2]);
                                        CVector delta = start - contactPos;
                                        float distanceSq = delta.lengthSq();

                                        if (distanceSq < minDistanceSq) {
                                            minDistanceSq = distanceSq;
                                            closestContact = contacts[i];
                                            closestContact.geom.g1 = ray.GetGeomId();
                                            closestContact.geom.g2 = geom;
                                            foundContact = true;
                                        }
                                    }
                                }
                            }
                            geom = dGeomGetBodyNext(geom);
                        }
                    }
                }

                // Early exit if we found a contact
                if (foundContact)
                {
                    break;
                }
            }
        }

        // Finally, check against terrain geometry
        m3d::GeomObject* terrainGeom = landscape->GetTerrainGeomObject();
        if (terrainGeom)
        {
            int contactCount = dCollide(ray.GetGeomId(), terrainGeom->GetGeom(), 8, &contacts[0].geom, sizeof(dContact));

            for (int i = 0; i < contactCount; i++)
            {
                CVector contactPos(contacts[i].geom.pos[0], contacts[i].geom.pos[1], contacts[i].geom.pos[2]);
                CVector delta = start - contactPos;
                float distanceSq = delta.lengthSq();

                if (distanceSq < minDistanceSq)
                {
                    minDistanceSq = distanceSq;
                    closestContact = contacts[i];
                    foundContact = true;
                }
            }
        }

        return foundContact;
    }
}


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

#include "m3dapp.h"
#include "scene/servers/dataserver.h"

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

    bool GetCollisionInfoByModelName(CStr const&, retruxx::vector<ai::CollisionInfo>&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GetCollisionInfoByServerHandle(int serverHandle, retruxx::vector<CollisionInfo>& collisionInfos, bool bTrimeshAllowed)
    {
        // TODO: generated code
        auto& animatedModelsServer = M3D_APP->GetAnimatedModelsServer();

        // Clear existing collision infos
        collisionInfos.clear();

        m3d::AnimatedModel* model = nullptr;

        // If no valid server handle, return empty
        if (serverHandle != -1)
        {
            // Get the animated model from the server
            animatedModelsServer.GetItemProperty(serverHandle, 16394, &model);

            if (!model)
                return false;

            // Process collision trimesh data if available and allowed
            const auto& collisionTrimesh = model->GetCollisionTrimesh();

            if (!collisionTrimesh.Points.empty() && bTrimeshAllowed)
            {
                ai::CollisionInfo collisionInfo;
                collisionInfo.m_geomType = GEOM_TYPE_TRIMESH;
                collisionInfo.m_numTrimeshVertices = collisionTrimesh.Points.size();
                collisionInfo.m_numTrimeshIndices = 3 * collisionTrimesh.Triangles.size();

                // Allocate and copy vertices
                collisionInfo.m_trimeshVertices = new ref_count_helper<retruxx::vector<CVector>>();
                collisionInfo.m_trimeshVertices->GetObjectA().resize(collisionInfo.m_numTrimeshVertices);

                for (unsigned int i = 0; i < collisionInfo.m_numTrimeshVertices; ++i)
                {
                    collisionInfo.m_trimeshVertices->GetObjectA()[i] = collisionTrimesh.Points[i];
                }

                // Allocate and copy indices
                collisionInfo.m_trimeshIndices = new ref_count_helper<std::vector<int>>();
                collisionInfo.m_trimeshIndices->GetObjectA().resize(collisionInfo.m_numTrimeshIndices);

                unsigned int indexOffset = 0;
                for (unsigned int triIndex = 0; triIndex < collisionTrimesh.Triangles.size(); ++triIndex)
                {
                    for (unsigned int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
                    {
                        collisionInfo.m_trimeshIndices->GetObjectA()[indexOffset++] =
                            collisionTrimesh.Triangles[triIndex].I[vertexIndex];
                    }
                }

                collisionInfos.push_back(std::move(collisionInfo));
            }

            // Process geometry primitives (boxes, spheres, cylinders)
            unsigned int geomCount = model->GetNumGeoms();
            for (unsigned int geomIndex = 0; geomIndex < geomCount; ++geomIndex)
            {
                const auto* geom = model->GetGeom(geomIndex);
                if (!geom)
                    continue;

                ai::CollisionInfo collisionInfo;

                // Set geometry transformation
                collisionInfo.m_relRotation = geom->Rotation;
                collisionInfo.m_relTranslation = geom->Translation;

                // Process based on geometry type
                switch (geom->Type)
                {
                case m3d::BOX:
                    collisionInfo.m_geomType = GEOM_TYPE_BOX;
                    collisionInfo.m_size.x = geom->Sizes.BoxSizes.x;
                    collisionInfo.m_size.y = geom->Sizes.BoxSizes.y;
                    collisionInfo.m_size.z = geom->Sizes.BoxSizes.z;
                    break;

                case m3d::SPHERE:
                    collisionInfo.m_geomType = GEOM_TYPE_SPHERE;
                    collisionInfo.m_radius = geom->Sizes.SphereRadius;
                    break;

                case m3d::CYLINDER:
                    collisionInfo.m_geomType = GEOM_TYPE_CYLINDER;
                    collisionInfo.m_radius = geom->Sizes.CylinderSizes.r;
                    collisionInfo.m_size.y = geom->Sizes.CylinderSizes.y;

                    // Apply 45-degree rotation for cylinder
                    Quaternion rotationAdjust;
                    rotationAdjust.y = sin(0.7853981852531433f); // sin(45°)
                    float cos45 = cos(0.7853981852531433f);      // cos(45°)

                    Quaternion adjustedRotation;
                    adjustedRotation.x = (cos45 * collisionInfo.m_relRotation.x) +
                        (collisionInfo.m_relRotation.w * rotationAdjust.y);
                    adjustedRotation.y = (collisionInfo.m_relRotation.y * cos45) +
                        (collisionInfo.m_relRotation.z * rotationAdjust.y);
                    adjustedRotation.z = (collisionInfo.m_relRotation.z * cos45) -
                        (collisionInfo.m_relRotation.y * rotationAdjust.y);
                    adjustedRotation.w = (collisionInfo.m_relRotation.w * cos45) -
                        (collisionInfo.m_relRotation.x * rotationAdjust.y);

                    collisionInfo.m_relRotation = adjustedRotation;
                    break;
                }

                collisionInfos.push_back(std::move(collisionInfo));
            }
        }

        // If no collision data found, create a default box based on model bounds
        if (collisionInfos.empty())
        {
            ai::CollisionInfo collisionInfo;
            collisionInfo.m_geomType = GEOM_TYPE_BOX;

            if (model)
            {
                // Calculate box size from model bounds
                collisionInfo.m_size.x = model->m_box.m_box[3] - model->m_box.m_box[0];
                collisionInfo.m_size.y = model->m_box.m_box[4] - model->m_box.m_box[1];
                collisionInfo.m_size.z = model->m_box.m_box[5] - model->m_box.m_box[2];

                // Calculate center position
                collisionInfo.m_relTranslation.x = (model->m_box.m_box[3] + model->m_box.m_box[0]) * 0.5f;
                collisionInfo.m_relTranslation.y = (model->m_box.m_box[4] + model->m_box.m_box[1]) * 0.5f;
                collisionInfo.m_relTranslation.z = (model->m_box.m_box[5] + model->m_box.m_box[2]) * 0.5f;
            }
            else
            {
                // Default small box
                collisionInfo.m_size = CVector(0.1f, 0.1f, 0.1f);
            }

            collisionInfos.push_back(std::move(collisionInfo));
        }

        // Validate and fix invalid geometry sizes
        for (auto& collInfo : collisionInfos)
        {
            switch (collInfo.m_geomType)
            {
            case GEOM_TYPE_BOX:
            {
                float boxSizeSq = collInfo.m_size.x * collInfo.m_size.x +
                    collInfo.m_size.y * collInfo.m_size.y +
                    collInfo.m_size.z * collInfo.m_size.z;

                if (boxSizeSq < 0.0001f)
                {
                    // Log warning about zero-sized box
                    M3D_LOG_INFO("Warning: size of box in model is zero. Setting to (0.5, 0.5, 0.5)");
                    collInfo.m_size = CVector(0.5f, 0.5f, 0.5f);
                }
                break;
            }

            case GEOM_TYPE_SPHERE:
                if (collInfo.m_radius < 0.0001f)
                {
                    // Log warning about zero-radius sphere
                    M3D_LOG_INFO("Warning: size of sphere in model is zero. Setting to 0.5");
                    collInfo.m_radius = 0.5f;
                }
                break;

            case GEOM_TYPE_CYLINDER:
            {
                float cylinderSizeSq = collInfo.m_size.x * collInfo.m_size.x +
                    collInfo.m_size.y * collInfo.m_size.y +
                    collInfo.m_size.z * collInfo.m_size.z;

                if (collInfo.m_radius < 0.0001f || cylinderSizeSq < 0.0001f)
                {
                    // Log warning about invalid cylinder sizes
                    M3D_LOG_INFO("Warning: sizes of cylinder in model are invalid. Setting to (0.5, 0.5)");
                    collInfo.m_size = CVector(0.0f, 0.5f, 0.0f);
                    collInfo.m_radius = 0.5f;
                }
                break;
            }
            }
        }

        return !collisionInfos.empty();
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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    namespace
    {
        bool IsLittle(dxGeom *)
        {
            RETRUXX_NOT_IMPLEMENTED;
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

    bool GetValidPosition(CVector const&, float, unsigned char, CVector&, bool, bool, std::set<m3d::Class*> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AnimInfo* GetNodeAnimInfo(m3d::SgNode const* node)
    {
        if (!node)
        {
            return nullptr;
        }

        m3d::AnimInfo* anim = nullptr;
        node->GetProperty(1u, &anim);
        return anim;
    }
}


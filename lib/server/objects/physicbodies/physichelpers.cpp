#define NOMINMAX
#include "physichelpers.h"

#include <ode/objects.h>

#include "geomobject.h"
#include "landscape.h"
#include "level.h"
#include "physicbody.h"
#include "world.h"
#include "core/kernel.h"
#include "core/log.h"
#include "math/coremath.h"
#include "ode/odecpp.h"
#include "server/server.h"
#include "server/map.h"
#include "server/objects/blastwave.h"
#include "server/objects/player.h"
#include "server/objects/town.h"
#include "server/objects/base/shell.h"
#include "server/objects/vehicle.h"
#include "vehiclepart.h"
#include <algorithm>

#include "m3dapp.h"
#include "scene/servers/dataserver.h"
#include <server/intersectionmanager.h>

namespace ai
{
    CollisionInfo::CollisionInfo(ai::CollisionInfo const& info)
    {
        this->m_geomType = info.m_geomType;
        this->m_relTranslation = info.m_relTranslation;
        this->m_relRotation = info.m_relRotation;
        this->m_size = info.m_size;
        this->m_radius = info.m_radius;
        this->m_trimeshVertices = info.m_trimeshVertices;
        this->m_trimeshIndices = info.m_trimeshIndices;
        m_numTrimeshVertices = info.m_numTrimeshVertices;
        m_numTrimeshIndices = info.m_numTrimeshIndices;
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
        m_numTrimeshVertices = 0;
        m_trimeshIndices = {};
        m_numTrimeshIndices = 0;
    }

    int ai::RoughSign(float value)
    {
        if (value > 0.000001)
            return 1;
        if (value >= -0.000001)
            return 0;
        return -1;
    }

    bool GetCollisionInfoByModelName(CStr const& id, retruxx::vector<ai::CollisionInfo>& collisionInfos, bool bTrimeshAllowed)
    {
        int const itemByName = M3D_APP->GetAnimatedModelsServer().GetItemByName(id.c_str(), true);
        return GetCollisionInfoByServerHandle(itemByName, collisionInfos, bTrimeshAllowed);
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
            auto const& collisionTrimesh = model->GetCollisionTrimesh();

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
                        collisionInfo.m_trimeshIndices->GetObjectA()[indexOffset++] = collisionTrimesh.Triangles[triIndex].I[vertexIndex];
                    }
                }

                collisionInfos.push_back(std::move(collisionInfo));
            }

            // Process geometry primitives (boxes, spheres, cylinders)
            unsigned int geomCount = model->GetNumGeoms();
            for (unsigned int geomIndex = 0; geomIndex < geomCount; ++geomIndex)
            {
                auto const* geom = model->GetGeom(geomIndex);
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
                    rotationAdjust.y = sin(0.7853981852531433f);  // sin(45�)
                    float cos45 = cos(0.7853981852531433f);       // cos(45�)

                    Quaternion adjustedRotation;
                    adjustedRotation.x = (cos45 * collisionInfo.m_relRotation.x) + (collisionInfo.m_relRotation.w * rotationAdjust.y);
                    adjustedRotation.y = (collisionInfo.m_relRotation.y * cos45) + (collisionInfo.m_relRotation.z * rotationAdjust.y);
                    adjustedRotation.z = (collisionInfo.m_relRotation.z * cos45) - (collisionInfo.m_relRotation.y * rotationAdjust.y);
                    adjustedRotation.w = (collisionInfo.m_relRotation.w * cos45) - (collisionInfo.m_relRotation.x * rotationAdjust.y);

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
                float boxSizeSq =
                    collInfo.m_size.x * collInfo.m_size.x + collInfo.m_size.y * collInfo.m_size.y + collInfo.m_size.z * collInfo.m_size.z;

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
                float cylinderSizeSq =
                    collInfo.m_size.x * collInfo.m_size.x + collInfo.m_size.y * collInfo.m_size.y + collInfo.m_size.z * collInfo.m_size.z;

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
        bool IsLittle(dxGeom* geom)
        {
            auto v1 = geom;
            if (dGeomGetClass(geom) == 6)
                v1 = dGeomTransformGetGeom(v1);
            auto cls = dGeomGetClass(v1);
            if (!cls)
                return dGeomSphereGetRadius(v1) <= 2.0;
            if (cls != 1)
                return 0;

            float size[4];
            dGeomBoxGetLengths(v1, size);
            int v4 = size[0] <= 2.0;
            if (size[1] <= 2.0)
                ++v4;
            if (size[2] <= 2.0)
                ++v4;
            return v4 >= 2;
        }
    }  // namespace

    bool CollideGeom(
        ai::Geom const& testGeom,
        bool dontCollideWithLittle,
        bool dontCollideWithPlayer,
        bool dontCollideWithWater,
        bool dontCollideWithShells)
    {
        // RVA 0x608E90 - a yes/no overlap test for one geom against the single
        // collision cell it sits in, plus the terrain. Unlike TraceLine it never
        // reports where it hit, so it stops at the first contact.
        float const VISCELL_EDGE_LENGTH = 128.0f;
        float const invCellSize = 1.0f / static_cast<int>(VISCELL_EDGE_LENGTH);

        auto const* geomPos = dGeomGetPosition(testGeom.GetGeomId());
        int const cellX = static_cast<int>(invCellSize * static_cast<float>(geomPos[0]));
        int const cellZ = static_cast<int>(invCellSize * static_cast<float>(geomPos[2]));

        // land_size counts cells, not world units - GetLevelSize() is the latter
        // and must not be used to bound a cell index.
        int const landSize = ai::pServer->GetLevel()->land_size;
        if (cellX < 0 || cellX >= landSize || cellZ < 0 || cellZ >= landSize)
        {
            return false;
        }

        dContact contact;
        auto& landscape = ai::pServer->GetWorld()->GetLandscape();

        if (auto* terrain = landscape.GetTerrainGeomObject())
        {
            if (dCollide(testGeom.GetGeomId(), terrain->GetGeom(), 1, &contact.geom, sizeof(dContact)))
            {
                return true;
            }
        }

        auto* cellItem = landscape.GetCollisionCellItem(cellX, cellZ);
        if (!cellItem)
        {
            M3D_LOG_INFO(
                "Warning: null collision cell item, cellX = " + CStr(cellX) + ", cellZ = " + CStr(cellZ));
            return false;
        }

        for (auto* geomObject : cellItem->m_geomsList)
        {
            if (dontCollideWithLittle && IsLittle(geomObject->GetGeom()))
            {
                continue;
            }
            if (dontCollideWithWater && geomObject->IsKindOf(&m3d::GeomObjectWater::m_classGeomObjectWater))
            {
                continue;
            }
            if (geomObject->IsKindOf(&m3d::GeomObjectPassCell::m_classGeomObjectPassCell))
            {
                continue;
            }
            if (dCollide(testGeom.GetGeomId(), geomObject->GetGeom(), 1, &contact.geom, sizeof(dContact)))
            {
                return true;
            }
        }

        for (int objId : cellItem->m_physicObjIds)
        {
            auto* obj = dynamic_cast<ai::PhysicObj*>(ai::theObjects->GetEntityByObjId(objId));
            if (!obj)
            {
                M3D_LOG_ERR(
                    "Error: NULL object is linked to collision cell x = " + CStr(cellX) + ", y = " + CStr(cellZ) +
                    ", id = " + CStr(objId));
                continue;
            }

            // A body-less object has no geometry worth testing, and a blast wave
            // is a pressure volume rather than something solid.
            if (!obj->GetBody() || obj->IsKindOf(&ai::BlastWave::m_classBlastWave))
            {
                continue;
            }

            if (obj->IsKindOf(&ai::SimplePhysicObj::m_classSimplePhysicObj))
            {
                auto* simple = static_cast<ai::SimplePhysicObj*>(obj);
                // The original indexes the first geom unconditionally; here a body
                // that never got its geometry built would be a null dereference.
                auto* body = simple->GetPhysicBody();
                if (!body || body->m_pGeoms.empty() || !body->m_pGeoms.front())
                {
                    continue;
                }
                dxGeom* geom = body->m_pGeoms.front()->GetGeomId();

                if (dontCollideWithLittle && IsLittle(geom))
                {
                    continue;
                }
                if (dontCollideWithShells && obj->IsKindOf(&ai::Shell::m_classShell))
                {
                    continue;
                }
                if (dontCollideWithPlayer &&
                    static_cast<ai::Vehicle*>(obj->GetParent()) ==
                        ai::gDynamicScene->GetVehicleControlledByPlayer())
                {
                    continue;
                }
                // Geoms parked in the intersection space, or in our own space,
                // are not real obstacles.
                if (dGeomGetSpace(geom) == ai::gIntersectionSpace)
                {
                    continue;
                }
                if (dGeomGetSpace(geom) == dGeomGetSpace(testGeom.GetGeomId()))
                {
                    continue;
                }
                if (dCollide(testGeom.GetGeomId(), geom, 1, &contact.geom, sizeof(dContact)))
                {
                    return true;
                }
            }
            else if (obj->IsKindOf(&ai::ComplexPhysicObj::m_classComplexPhysicObj))
            {
                auto* complex = static_cast<ai::ComplexPhysicObj*>(obj);
                if (dontCollideWithPlayer && complex == ai::gDynamicScene->GetVehicleControlledByPlayer())
                {
                    continue;
                }

                for (auto const& part : complex->m_vehicleParts)
                {
                    if (!part.second || part.second->m_pGeoms.empty() || !part.second->m_pGeoms.front())
                    {
                        continue;
                    }
                    dxGeom* geom = part.second->m_pGeoms.front()->GetGeomId();
                    if (dGeomGetSpace(geom) == ai::gIntersectionSpace)
                    {
                        continue;
                    }
                    if (dGeomGetSpace(geom) == dGeomGetSpace(testGeom.GetGeomId()))
                    {
                        continue;
                    }
                    if (dCollide(testGeom.GetGeomId(), geom, 1, &contact.geom, sizeof(dContact)))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool TraceLine(
        ai::Ray const& ray,
        dContact& closestContact,
        bool dontCollideWithDynamic,
        bool dontCollideWithLittle,
        bool dontCollideWithPlayer,
        bool dontCollideWithWater,
        ai::TraceLineCallback* callback,
        bool dontCollideWithShells,
        bool smartCollideWithTowns)
    {
        // TODO: generated code

        // Get ray start position and direction
        auto* geomPos = dGeomGetPosition(ray.GetGeomId());
        CVector start;
        start.x = geomPos[0];
        start.y = geomPos[1];
        start.z = geomPos[2];

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
            int collisionCellX = cellX >> 7;  // Divide by 128
            int collisionCellZ = cellZ >> 7;

            // Only process if we moved to a new collision cell
            if (collisionCellX != prevCellX || collisionCellZ != prevCellZ)
            {
                prevCellX = collisionCellX;
                prevCellZ = collisionCellZ;

                // Get collision cell from landscape
                m3d::Landscape::CollisionCellItem* cellItem = landscape->GetCollisionCellItem(collisionCellX, collisionCellZ);

                if (!cellItem)
                {
                    // Log warning about missing collision cell
                    M3D_LOG_INFO(
                        "Warning: null collision cell item, cellX = " + CStr(collisionCellX) + ", cellZ = " + CStr(collisionCellZ));
                    continue;
                }

                // Process static geometry in this cell
                for (auto* geomObject : cellItem->m_geomsList)
                {
                    if (!geomObject)
                        continue;

                    // Apply collision filters
                    if (dontCollideWithLittle && IsLittle(geomObject->GetGeom()))
                    {
                        continue;
                    }
                    if (dontCollideWithWater && geomObject->IsKindOf(&m3d::GeomObjectWater::m_classGeomObjectWater))
                    {
                        continue;
                    }
                    if (geomObject->IsKindOf(&m3d::GeomObjectPassCell::m_classGeomObjectPassCell))
                    {
                        continue;
                    }

                    // Perform collision detection
                    int contactCount = dCollide(ray.GetGeomId(), geomObject->GetGeom(), 8, &contacts[0].geom, sizeof(dContact));

                    // Process contacts
                    for (int i = 0; i < contactCount; i++)
                    {
                        dContact const& contact = contacts[i];
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
                        M3D_LOG_ERR(
                            "Error: NULL object is linked to collision cell x = " + CStr(collisionCellX) + ", y = " + CStr(collisionCellZ) +
                            ", id = " + CStr(objId));
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
                    if (dontCollideWithShells && object->IsKindOf(&ai::Shell::m_classShell))
                    {
                        continue;
                    }
                    if ((object->GetPhysicState() & 2) != 0)
                    {
                        continue;
                    }
                    if (dontCollideWithPlayer)
                    {
                        if (object == playerVehicle)
                            continue;
                        if (object->GetParent() == playerVehicle)
                            continue;
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
                                std::sort(
                                    contactPoints.begin(),
                                    contactPoints.end(),
                                    [&](CVector const& a, CVector const& b)
                                    {
                                        return (start - a).lengthSq() < (start - b).lengthSq();
                                    });

                                // Use only the most distant contacts (town optimization)
                                size_t startIndex = 0;
                                if (contactPoints.size() > maxContacts)
                                {
                                    startIndex = contactPoints.size() - maxContacts;
                                }

                                for (size_t i = startIndex; i < contactPoints.size(); i++)
                                {
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
                        while (geom)
                        {
                            if (!dontCollideWithLittle || !IsLittle(geom))
                            {
                                dxSpace* space = dGeomGetSpace(geom);
                                if (space && space != ai::gIntersectionSpace && space != dGeomGetSpace(ray.GetGeomId()))
                                {
                                    int contactCount = dCollide(ray.GetGeomId(), geom, 8, &contacts[0].geom, sizeof(dContact));

                                    for (int i = 0; i < contactCount; i++)
                                    {
                                        CVector contactPos(contacts[i].geom.pos[0], contacts[i].geom.pos[1], contacts[i].geom.pos[2]);
                                        CVector delta = start - contactPos;
                                        float distanceSq = delta.lengthSq();

                                        if (distanceSq < minDistanceSq)
                                        {
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

    bool GetValidPosition(
        CVector const& position,
        float radius,
        unsigned char blockingValue,
        CVector& availablePosition,
        bool bCheckPassMap,
        bool bForPlayerVehicle,
        std::set<m3d::Class*> const& targetClasses)
    {
        // TODO: generated code GetValidPosition
        float minDist = 1.0e20f;

        // Create intersection sphere
        scoped_ptr<ai::SphereForIntersection> sphere = ai::SphereForIntersection::CreateObject(
            radius, ai::SphereForIntersection::SpherePurpose::LOOKING, 0);  // LOOKING constant assumed to be 0

        // Set initial position
        dGeomSetPosition(sphere->GetGeomId(), position.x, position.y, position.z);

        // Check if initial position is valid
        if (!ai::IntersectionManager::IsSphereValid(sphere, targetClasses, bForPlayerVehicle) ||
            (bCheckPassMap && Map::theGlobalMap->IsCircleBlocked(CVector2{position.x, position.z}, radius, blockingValue)))
        {
            // Initial position is invalid, search for a valid one
            float currentRadius = radius;
            float dist = radius;

            if (radius <= 100.0f)
            {
                while (dist <= 100.0f)
                {
                    // Try 8 directions around the circle
                    for (int i = 0; i < 8; ++i)
                    {
                        static CVector const DirOffset[] = {
                            {1.0, 0.0, 0.0},
                            {-1.0, 0.0, 0.0},
                            {0.0, 0.0, 1.0},
                            {0.0, 0.0, -1.0},
                            {0.70709997, 0.0, 0.70709997},
                            {-0.70709997, 0.0, 0.70709997},
                            {0.70709997, 0.0, -0.70709997},
                            {-0.70709997, 0.0, -0.70709997},
                        };
                        auto const& offset = DirOffset[i];

                        // Calculate new position
                        float newX = position.x + offset.x * dist;
                        float newY = position.y + offset.y * dist;
                        float newZ = position.z + offset.z * dist;

                        // Update sphere position
                        dGeomSetPosition(sphere->GetGeomId(), newX, newY, newZ);

                        // Check if this position is better than current best
                        if (minDist > dist && ai::IntersectionManager::IsSphereValid(sphere, targetClasses, bForPlayerVehicle))
                        {
                            if (!bCheckPassMap ||
                                !ai::Map::theGlobalMap->IsCircleBlocked(CVector2{newX, newZ}, radius, blockingValue))
                            {
                                minDist = dist;
                                availablePosition = CVector{newX, newY, newZ};
                            }
                        }
                    }

                    // Increase search radius
                    dist += radius;
                    currentRadius = dist;
                }
            }

            // Check if we found a valid position
            if (minDist >= 1.0e19f)
            {
                return false;
            }
            return true;
        }
        else
        {
            // Initial position is valid
            availablePosition = position;
            return true;
        }
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

    CVector GetRandomDeviatedVector(CVector const& axis, float maxDeviationAngle)
    {
        // TODO: generated code GetRandomDeviatedVector
        // Generate random deviation angles
        float minAngle = 0.0f;
        float maxAngle = maxDeviationAngle;

        // Determine the range for random angle generation
        float* angleRangeStart = (maxDeviationAngle >= 0.0f) ? &minAngle : &maxAngle;
        float* angleRangeEnd = (maxDeviationAngle <= 0.0f) ? &minAngle : &maxAngle;
        float* actualStart = (maxDeviationAngle >= 0.0f) ? &minAngle : &maxAngle;

        // Generate random deviation angle
        float deviationAngle = ((rand() * (*angleRangeEnd - *angleRangeStart)) * 0.000030518509f + *actualStart) * 0.5f;

        // Create quaternion for deviation rotation
        Quaternion quatDeviation;
        quatDeviation.y = sin(deviationAngle);
        quatDeviation.w = cos(deviationAngle);

        // Generate random rotation angle
        float rotationAngle = (rand() * 0.00019175345f) * 0.5f;

        // Create rotation quaternion components
        float sinRot = sin(rotationAngle);
        float cosRot = cos(rotationAngle);

        // Calculate quaternion components for the deviation
        float qx = (quatDeviation.w * 0.0f) + (cosRot * 0.0f) - (sinRot * quatDeviation.y);
        float qy = (cosRot * quatDeviation.y) + (sinRot * 0.0f) + (quatDeviation.w * 0.0f);
        float qz = (sinRot * quatDeviation.w) + (quatDeviation.y * 0.0f) + (cosRot * 0.0f);
        float qw = (cosRot * quatDeviation.w) - (quatDeviation.y * 0.0f) - (sinRot * 0.0f);

        // Calculate intermediate values for matrix construction
        float temp1 = qz * qy;
        float temp2 = qw * qy;
        float temp3 = qw * qz;

        // Build rotation matrix for deviation
        CMatrix deviationMatrix;
        deviationMatrix._11 = 1.0f - ((qz * qz + qy * qy) * 2.0f);
        deviationMatrix._21 = ((qy * qx) - (qw * qz)) * 2.0f;
        deviationMatrix._31 = ((qw * qy) + (qz * qx)) * 2.0f;
        deviationMatrix._12 = ((qw * qz) + (qy * qx)) * 2.0f;
        deviationMatrix._22 = 1.0f - ((qz * qz + qx * qx) * 2.0f);
        deviationMatrix._32 = ((qz * qy) - (qw * qx)) * 2.0f;
        deviationMatrix._13 = ((qz * qx) - (qw * qy)) * 2.0f;
        deviationMatrix._23 = ((qw * qx) + (qz * qy)) * 2.0f;
        deviationMatrix._33 = 1.0f - ((qy * qy + qx * qx) * 2.0f);

        // Set translation components to identity
        deviationMatrix._14 = 0.0f;
        deviationMatrix._24 = 0.0f;
        deviationMatrix._34 = 0.0f;
        deviationMatrix._41 = 0.0f;
        deviationMatrix._42 = 0.0f;
        deviationMatrix._43 = 0.0f;
        deviationMatrix._44 = 1.0f;

        // Apply deviation to initial direction
        CMatrix tempMatrix(deviationMatrix);
        CVector deviatedDir;

        CVector INITIAL_OBJECTS_DIRECTION_36(0.0, 0.0, 1.0);
        deviatedDir.x = (tempMatrix._11 * INITIAL_OBJECTS_DIRECTION_36.x) + (tempMatrix._21 * INITIAL_OBJECTS_DIRECTION_36.y) +
            (tempMatrix._31 * INITIAL_OBJECTS_DIRECTION_36.z);
        deviatedDir.y = (tempMatrix._12 * INITIAL_OBJECTS_DIRECTION_36.x) + (tempMatrix._22 * INITIAL_OBJECTS_DIRECTION_36.y) +
            (tempMatrix._32 * INITIAL_OBJECTS_DIRECTION_36.z);
        deviatedDir.z = (tempMatrix._13 * INITIAL_OBJECTS_DIRECTION_36.x) + (tempMatrix._23 * INITIAL_OBJECTS_DIRECTION_36.y) +
            (tempMatrix._33 * INITIAL_OBJECTS_DIRECTION_36.z);

        // Calculate axis correction if needed
        float crossX = (INITIAL_OBJECTS_DIRECTION_36.y * axis.z) - (INITIAL_OBJECTS_DIRECTION_36.z * axis.y);
        float crossY = (axis.x * INITIAL_OBJECTS_DIRECTION_36.z) - (INITIAL_OBJECTS_DIRECTION_36.x * axis.z);
        float crossZ = (INITIAL_OBJECTS_DIRECTION_36.x * axis.y) - (axis.x * INITIAL_OBJECTS_DIRECTION_36.y);

        float crossLengthSq = (crossX * crossX) + (crossY * crossY) + (crossZ * crossZ);

        // Calculate angle between initial direction and target axis
        float dotProduct =
            INITIAL_OBJECTS_DIRECTION_36.x * axis.x + INITIAL_OBJECTS_DIRECTION_36.y * axis.y + INITIAL_OBJECTS_DIRECTION_36.z * axis.z;
        float angleBetween = atan2(sqrt(crossLengthSq), dotProduct);

        // Apply axis correction if significant misalignment
        if (crossLengthSq > 0.0001f)
        {
            // Normalize cross product
            float invLength = 1.0f / sqrt(crossLengthSq + 1.1920929e-7f);
            float normX = crossX * invLength;
            float normY = crossY * invLength;
            float normZ = crossZ * invLength;

            // Re-normalize to ensure unit length
            float renormalize = 1.0f / sqrt(normX * normX + normY * normY + normZ * normZ + 1.1920929e-7f);
            normX *= renormalize;
            normY *= renormalize;
            normZ *= renormalize;

            // Create correction quaternion
            float halfAngle = angleBetween * 0.5f;
            float sinHalf = sin(halfAngle);
            float cosHalf = cos(halfAngle);

            float qxCorr = sinHalf * normX;
            float qyCorr = sinHalf * normY;
            float qzCorr = sinHalf * normZ;
            float qwCorr = cosHalf;

            // Build correction matrix
            CMatrix correctionMatrix;
            correctionMatrix._11 = 1.0f - ((qzCorr * qzCorr + qyCorr * qyCorr) * 2.0f);
            correctionMatrix._21 = ((qyCorr * qxCorr) - (qwCorr * qzCorr)) * 2.0f;
            correctionMatrix._31 = ((qwCorr * qyCorr) + (qzCorr * qxCorr)) * 2.0f;
            correctionMatrix._12 = ((qwCorr * qzCorr) + (qyCorr * qxCorr)) * 2.0f;
            correctionMatrix._22 = 1.0f - ((qzCorr * qzCorr + qxCorr * qxCorr) * 2.0f);
            correctionMatrix._32 = ((qzCorr * qyCorr) - (qwCorr * qxCorr)) * 2.0f;
            correctionMatrix._13 = ((qzCorr * qxCorr) - (qwCorr * qyCorr)) * 2.0f;
            correctionMatrix._23 = ((qwCorr * qxCorr) + (qzCorr * qyCorr)) * 2.0f;
            correctionMatrix._33 = 1.0f - ((qyCorr * qyCorr + qxCorr * qxCorr) * 2.0f);

            // Set translation components to identity
            correctionMatrix._14 = 0.0f;
            correctionMatrix._24 = 0.0f;
            correctionMatrix._34 = 0.0f;
            correctionMatrix._41 = 0.0f;
            correctionMatrix._42 = 0.0f;
            correctionMatrix._43 = 0.0f;
            correctionMatrix._44 = 1.0f;

            // Apply correction to deviated direction. Every component has to be
            // built from the original vector, so rotate into a fresh one rather
            // than updating in place.
            CMatrix finalMatrix(correctionMatrix);
            CVector const preCorrection = deviatedDir;
            deviatedDir.x = (finalMatrix._11 * preCorrection.x) + (finalMatrix._21 * preCorrection.y) +
                (finalMatrix._31 * preCorrection.z);
            deviatedDir.y = (finalMatrix._12 * preCorrection.x) + (finalMatrix._22 * preCorrection.y) +
                (finalMatrix._32 * preCorrection.z);
            deviatedDir.z = (finalMatrix._13 * preCorrection.x) + (finalMatrix._23 * preCorrection.y) +
                (finalMatrix._33 * preCorrection.z);
        }

        return deviatedDir;
    }
}  // namespace ai

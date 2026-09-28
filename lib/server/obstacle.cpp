#include "obstacle.h"
#include <stdexcept>
#include <math/aabb.h>
#include <math/quaternion.h>
#include <math/vector.h>

#include "m3dapp.h"
#include "skelmodel.h"
#include "math/obb.h"
#include "ode/objects.h"
#include "objects/base/physicobj.h"
#include "objects/base/complexphysicobj.h"
#include "objects/base/simplephysicobj.h"
#include "objects/physicbodies/vehiclepart.h"
#include "objects/physicbodies/geoms/sphereforintersection.h"
#include "landscape.h"
#include "core/kernel.h"
#include "core/timer.h"
#include "ode/odecpp.h"
#include "objects/base/objcontainer.h"
#include "objects/physicbodies/geoms/box.h"
#include "scene/nodes/sgnode.h"
#include "scene/servers/dataserver.h"
#include "server.h"
#include "world.h"

namespace ai
{
    void Obstacle::UnlinkFromOwner()
    {
        this->m_ownerPhysicObjId = -1;
        this->m_ownerSgNode = 0;
    }

    Obstacle::~Obstacle()
    {
        delete m_intersectionSphere;
        delete m_intersectionBox;
    }

    // RVA 0x7BB790
    Quaternion Obstacle::GetRotation() const
    {
        dQuaternion quat;
        dGeomGetQuaternion(m_intersectionSphere->GetGeomId(), quat);
        // ODE keeps w first.
        return Quaternion(quat[1], quat[2], quat[3], quat[0]);
    }

    int Obstacle::DecRef()
    {
        auto res = --this->m_refCount;
        if (m_refCount <= 0)
        {
            delete this;
        }
        return res;
    }

    namespace
    {
        // Shared tail of the SgNode and Obb constructors (inlined in both): a sphere circumscribing the box,
        // centred on it.
        SphereForIntersection* CreateSphereAroundBox(Box const* box, Obstacle* owner)
        {
            CVector const size = box->GetSize();
            float const radius = sqrt(size.x * size.x + size.y * size.y + size.z * size.z) * 0.5f;
            SphereForIntersection* const sphere =
                SphereForIntersection::CreateObject(radius, SphereForIntersection::INTERSECTING, owner);
            float const* const pos = dGeomGetPosition(box->GetGeomId());
            dGeomSetPosition(sphere->GetGeomId(), pos[0], pos[1], pos[2]);
            return sphere;
        }
    }  // namespace

    Obstacle::Obstacle(m3d::SgNode* sgNode)
    {
        // RVA 0x7BB2C0 - an obstacle around the bounding box of the node's animated model, if it has one.
        this->m_refCount = 0;
        this->m_bIsEnabled = 1;
        this->m_intersectionSphere = 0;
        this->m_intersectionBox = 0;
        this->m_ownerPhysicObjId = -1;
        this->m_ownerSgNode = sgNode;

        int modelHandle = -1;
        sgNode->GetProperty(m3d::PROP_NODE_HANDLE, &modelHandle);
        if (modelHandle == -1)
        {
            return;
        }

        m3d::AnimatedModel* mdl = nullptr;
        M3D_APP->GetAnimatedModelsServer().GetItemProperty(modelHandle, 16394, &mdl);
        float const* const box = mdl->m_box.m_box;  // min x, y, z, max x, y, z
        float const centerX = (box[3] + box[0]) * 0.5f;
        float const centerY = (box[4] + box[1]) * 0.5f;
        float const centerZ = (box[5] + box[2]) * 0.5f;
        m_intersectionBox = Box::CreateObject(nullptr, CVector(box[3] - box[0], box[4] - box[1], box[5] - box[2]), nullptr);

        // The box centre, rotated into the node's orientation (the shipped code inlines Quaternion::ToMatrix).
        CMatrix const rot = m_ownerSgNode->GetRotation().ToMatrix();
        CVector const& origin = m_ownerSgNode->GetOriginWorldAbs();
        dGeomSetPosition(
            m_intersectionBox->GetGeomId(),
            origin.x + (rot._31 * centerZ + rot._21 * centerY + rot._11 * centerX),
            origin.y + (rot._32 * centerZ + rot._22 * centerY + rot._12 * centerX),
            origin.z + (rot._33 * centerZ + rot._23 * centerY + rot._13 * centerX));

        Quaternion const& q = m_ownerSgNode->GetRotation();
        float const odeQuat[4] = {q.w, q.x, q.y, q.z};
        dGeomSetQuaternion(m_intersectionBox->GetGeomId(), odeQuat);

        m_intersectionSphere = CreateSphereAroundBox(m_intersectionBox, this);
    }

    Obstacle::Obstacle(Obb const& obb)
    {
        // RVA 0x7BB0E0 - an obstacle for an oriented box, owned by the scene root.
        this->m_ownerSgNode = 0;
        this->m_refCount = 0;
        this->m_intersectionSphere = 0;
        this->m_intersectionBox = 0;
        this->m_bIsEnabled = 1;
        this->m_ownerPhysicObjId = -1;
        this->m_ownerSgNode = ai::pServer->GetWorld()->GetGraph().GetRootNode();

        CMatrix basis;
        basis.zero();
        basis._11 = obb.m_basis[0].x;
        basis._21 = obb.m_basis[0].y;
        basis._31 = obb.m_basis[0].z;
        basis._12 = obb.m_basis[1].x;
        basis._22 = obb.m_basis[1].y;
        basis._32 = obb.m_basis[1].z;
        basis._13 = obb.m_basis[2].x;
        basis._23 = obb.m_basis[2].y;
        basis._33 = obb.m_basis[2].z;

        m_intersectionBox = Box::CreateObject(nullptr, obb.m_max - obb.m_min, nullptr);
        dGeomSetPosition(m_intersectionBox->GetGeomId(), obb.m_origin.x, obb.m_origin.y, obb.m_origin.z);

        Quaternion q;
        q.FromMatrix(basis);
        float const odeQuat[4] = {q.w, q.x, q.y, q.z};
        dGeomSetQuaternion(m_intersectionBox->GetGeomId(), odeQuat);

        m_intersectionSphere = CreateSphereAroundBox(m_intersectionBox, this);
    }

    Obstacle::Obstacle(PhysicObj const* physicObj)
    {
        // RVA 0x7BB080 - an obstacle sphere attached to the object's body.
        this->m_refCount = 0;
        this->m_bIsEnabled = 1;
        this->m_intersectionSphere = 0;
        this->m_intersectionBox = 0;
        this->m_ownerPhysicObjId = -1;
        this->m_ownerSgNode = 0;

        auto const* protoInfo = physicObj->GetPrototypeInfo();
        this->m_intersectionSphere =
            ai::SphereForIntersection::CreateObject(protoInfo->m_intersectionRadius, SphereForIntersection::INTERSECTING, this);
        dGeomSetBody(this->m_intersectionSphere->GetGeomId(), physicObj->GetBody()->id());
        this->m_ownerPhysicObjId = physicObj->GetId();
    }

    Aabb Obstacle::GetAabb() const
    {
        auto aabb = m_intersectionSphere->GetAabb();
        if (m_intersectionBox)
        {
            auto boxAabb = m_intersectionBox->GetAabb();
            aabb.EmbraceBox(boxAabb);
        }
        return aabb;
    }

    m3d::Object* Obstacle::GetOwner() const
    {
        if (m_ownerPhysicObjId == -1)
            return this->m_ownerSgNode;

        return ai::theObjects->GetEntityByObjId(m_ownerPhysicObjId);
    }

    void Obstacle::Disable()
    {
        if (m_intersectionSphere)
        {
            dGeomDisable(m_intersectionSphere->GetGeomId());
        }
        if (m_intersectionBox)
        {
            dGeomDisable(m_intersectionBox->GetGeomId());
        }
        m_bIsEnabled = false;
    }

    CVector Obstacle::GetPosition() const
    {
        auto position = dGeomGetPosition(m_intersectionSphere->GetGeomId());

        CVector result;
        result.x = position[0];
        result.y = position[1];
        result.z = position[2];
        return result;
    }

    int Obstacle::IncRef()
    {
        return ++this->m_refCount;
    }

    void Obstacle::Enable()
    {
        if (m_intersectionSphere)
        {
            dGeomEnable(m_intersectionSphere->GetGeomId());
        }
        if (m_intersectionBox)
        {
            dGeomEnable(m_intersectionBox->GetGeomId());
        }
        m_bIsEnabled = true;
    }

    CVector Obstacle::GetLinearVelocity() const
    {
        auto* obj = RT_DYNCAST(theObjects->GetEntityByObjId(m_ownerPhysicObjId), PhysicObj);
        if (obj)
        {
            return obj->GetLinearVelocity();
        }
        return ZeroVector;
    }

    Box const* Obstacle::GetBox() const
    {
        return this->m_intersectionBox;
    }

    SphereForIntersection const* Obstacle::GetSphere() const
    {
        return this->m_intersectionSphere;
    }

    // RVA 0x7BB960
    void Obstacle::RenderDebugInfo() const
    {
        if (!m_bIsEnabled)
        {
            return;
        }

        // Only obstacles whose owner's model was drawn this frame are shown.
        if (m_ownerPhysicObjId >= 0)
        {
            Obj* const owner = theObjects->GetEntityByObjId(m_ownerPhysicObjId);
            if (owner)
            {
                if (owner->IsKindOf(RT_CLASS_LOCAL(SimplePhysicObj)))
                {
                    m3d::SgNode const* const node = static_cast<SimplePhysicObj*>(owner)->GetPhysicBody()->m_Node;
                    if (node && node->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame())
                    {
                        return;
                    }
                }
                else if (owner->IsKindOf(RT_CLASS_LOCAL(ComplexPhysicObj)))
                {
                    ComplexPhysicObj const* const complexObj = static_cast<ComplexPhysicObj*>(owner);
                    if (!complexObj->m_vehicleParts.empty())
                    {
                        // NOTE: the first part's node is used without a null check.
                        m3d::SgNode const* const node = complexObj->m_vehicleParts.begin()->second->m_Node;
                        if (node->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame())
                        {
                            return;
                        }
                    }
                }
            }
        }

        dGeomID const geom = m_intersectionBox ? m_intersectionBox->GetGeomId() : m_intersectionSphere->GetGeomId();
        pServer->GetWorld()->GetLandscape().DrawGeom(geom);
    }

    bool Obstacle::bIsEnabled() const
    {
        return this->m_bIsEnabled;
    }

    float Obstacle::GetIntersectionRadius() const
    {
        return m_intersectionSphere->GetRadius();
    }

    PhysicObj* Obstacle::GetOwnerPhysicObj() const
    {
        return RT_DYNCAST(theObjects->GetEntityByObjId(m_ownerPhysicObjId), PhysicObj);
    }

    // RVA 0x7BB000
    void Obstacle::_Init()
    {
        m_refCount = 0;
        m_bIsEnabled = true;
        m_intersectionSphere = nullptr;
        m_intersectionBox = nullptr;
        m_ownerPhysicObjId = -1;
        m_ownerSgNode = nullptr;
    }
}  // namespace ai

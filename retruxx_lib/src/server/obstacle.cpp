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

    Quaternion Obstacle::GetRotation() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    Obstacle::Obstacle(m3d::SgNode* sgNode)
    {
        this->m_refCount = 0;
        this->m_bIsEnabled = 1;
        this->m_intersectionSphere = 0;
        this->m_intersectionBox = 0;
        this->m_ownerPhysicObjId = -1;
        this->m_ownerSgNode = sgNode;

        int sh = -1;
        sgNode->GetProperty(4360u, &sh);
        if (sh != -1)
        {
            // TODO: check this and refactor
            m3d::AnimatedModel* mdl = nullptr;
            M3D_APP->GetAnimatedModelsServer().GetItemProperty(sh, 16394, &mdl);

            auto center = (float)(mdl->m_box.m_box[3] + mdl->m_box.m_box[0]) * 0.5;
            auto center_4 = (float)(mdl->m_box.m_box[4] + mdl->m_box.m_box[1]) * 0.5;
            auto center_8 = (float)(mdl->m_box.m_box[5] + mdl->m_box.m_box[2]) * 0.5;

            CVector size;
            size.x = mdl->m_box.m_box[3] - mdl->m_box.m_box[0];
            size.y = mdl->m_box.m_box[4] - mdl->m_box.m_box[1];
            size.z = mdl->m_box.m_box[5] - mdl->m_box.m_box[2];

            m_intersectionBox = Box::CreateObject(nullptr, size, nullptr);

            const auto ownerRotation = m_ownerSgNode->GetRotation();
            CMatrix vv;
            auto z = ownerRotation.z;
            auto y = ownerRotation.y;
            auto v13 = z * ownerRotation.w;
            auto v14 = ownerRotation.x * z;
            auto v15 = ownerRotation.x * ownerRotation.w;
            auto v33 = ownerRotation.x * ownerRotation.x;
            auto v34 = ownerRotation.x * ownerRotation.y;
            auto v27 = ownerRotation.z * ownerRotation.y;
            auto v16 = z * z;
            auto v28 = ownerRotation.y * ownerRotation.w;
            auto v17 = y * y;
            vv._11 = 1.0 - (float)((float)(v16 + v17) * 2.0);
            vv._21 = (float)(v34 - v13) * 2.0;
            vv._31 = (float)(v28 + v14) * 2.0;
            vv._12 = (float)(v13 + v34) * 2.0;
            vv._22 = 1.0 - (float)((float)(v16 + v33) * 2.0);
            vv._33 = 1.0 - (float)((float)(v17 + v33) * 2.0);
            vv._32 = (float)(v27 - v15) * 2.0;
            vv.m[0][2] = ((float)(v14 - v28) * 2.0);
            vv.m[0][3] = 0.0;
            vv.m[1][2] = ((float)(v15 + v27) * 2.0);
            vv.m[1][3] = 0.0;
            memset(&vv.m[2][3], 0, 16);
            vv._44 = 1.0;

            const auto ownerWorldAbs = m_ownerSgNode->GetOriginWorldAbs();
            dGeomSetPosition(
                m_intersectionBox->GetGeomId(),
                ownerWorldAbs.x + (((vv._31 * center_8) + (vv._21 * center_4)) + (vv._11 * center)),
                ownerWorldAbs.y + (((vv._32 * center_8) + (vv._22 * center_4)) + (vv._12 * center)),
                ownerWorldAbs.z + (((vv._33 * center_8) + (vv._23 * center_4)) + (vv._13 * center)));

            auto rot = ownerRotation;
            float quat[4] = {rot.w, rot.x, rot.y, rot.z};
            dGeomSetQuaternion(m_intersectionBox->GetGeomId(), quat);

            auto boxSize = m_intersectionBox->GetSize();
            auto radius = boxSize.length() * 0.5;
            this->m_intersectionSphere = ai::SphereForIntersection::CreateObject(radius, SphereForIntersection::INTERSECTING, this);
            
            auto position = dGeomGetPosition(this->m_intersectionBox->GetGeomId());
            dGeomSetPosition(this->m_intersectionSphere->GetGeomId(), position[0], position[1], position[2]);
        }
    }

    Obstacle::Obstacle(Obb const& obb)
    {
        // TODO: check this and refactor
        this->m_ownerSgNode = 0;
        this->m_refCount = 0;
        this->m_intersectionSphere = 0;
        this->m_intersectionBox = 0;
        this->m_bIsEnabled = 1;
        this->m_ownerPhysicObjId = -1;
        this->m_ownerSgNode = ai::pServer->GetWorld()->GetGraph().GetRootNode();

        CMatrix mat;
        mat.zero();
        mat._11 = obb.m_basis[0].x;
        mat._21 = obb.m_basis[0].y;
        mat._31 = obb.m_basis[0].z;
        mat._12 = obb.m_basis[1].x;
        mat._22 = obb.m_basis[1].y;
        mat._32 = obb.m_basis[1].z;
        mat._13 = obb.m_basis[2].x;
        mat._23 = obb.m_basis[2].y;
        mat._33 = obb.m_basis[2].z;

        CVector size;
        size.x = obb.m_max.x - obb.m_min.x;
        size.y = obb.m_max.y - obb.m_min.y;
        size.z = obb.m_max.z - obb.m_min.z;


        this->m_intersectionBox = Box::CreateObject(0, size, 0);

        dGeomSetPosition(m_intersectionBox->GetGeomId(), obb.m_origin.x, obb.m_origin.y, obb.m_origin.z);

        Quaternion q;
        q.FromMatrix(mat);

        float quat[4];
        quat[0] = q.w;
        quat[1] = q.x;
        quat[2] = q.y;
        quat[3] = q.z;

        dGeomSetQuaternion(m_intersectionBox->GetGeomId(), quat);

        auto boxSize = m_intersectionBox->GetSize();
        auto radius = boxSize.length() * 0.5;
        this->m_intersectionSphere = ai::SphereForIntersection::CreateObject(radius, SphereForIntersection::INTERSECTING, this);

        auto position = dGeomGetPosition(this->m_intersectionBox->GetGeomId());
        dGeomSetPosition(this->m_intersectionSphere->GetGeomId(), position[0], position[1], position[2]);
    }

    Obstacle::Obstacle(PhysicObj const* physicObj)
    {
        this->m_refCount = 0;
        this->m_bIsEnabled = 1;
        this->m_intersectionSphere = 0;
        this->m_intersectionBox = 0;
        this->m_ownerPhysicObjId = -1;
        this->m_ownerSgNode = 0;

        const auto* protoInfo = physicObj->GetPrototypeInfo();
        this->m_intersectionSphere = ai::SphereForIntersection::CreateObject(protoInfo->m_intersectionRadius, SphereForIntersection::INTERSECTING, this);
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Obstacle::GetPosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Obstacle::IncRef()
    {
        return ++this->m_refCount;
    }

    void Obstacle::Enable()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Obstacle::GetLinearVelocity() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Box const* Obstacle::GetBox() const
    {
        return this->m_intersectionBox;
    }

    SphereForIntersection const* Obstacle::GetSphere() const
    {
        return this->m_intersectionSphere;
    }

    void Obstacle::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obstacle::bIsEnabled() const
    {
        return this->m_bIsEnabled;
    }

    float Obstacle::GetIntersectionRadius() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicObj* Obstacle::GetOwnerPhysicObj() const
    {
        return RT_DYNCAST(theObjects->GetEntityByObjId(m_ownerPhysicObjId), PhysicObj);
    }

    void Obstacle::_Init()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

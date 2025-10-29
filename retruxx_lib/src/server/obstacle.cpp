#include "obstacle.h"
#include <stdexcept>
#include <math/aabb.h>
#include <math/quaternion.h>
#include <math/vector.h>

#include "ode/objects.h"
#include "objects/base/physicobj.h"
#include "ode/odecpp.h"
#include "objects/base/objcontainer.h"
#include "objects/physicbodies/geoms/box.h"
#include "scene/nodes/sgnode.h"

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

    Obstacle::Obstacle(m3d::SgNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obstacle::Obstacle(Obb const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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

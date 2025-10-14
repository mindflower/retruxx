#include "obstacle.h"
#include <stdexcept>
#include <math/aabb.h>
#include <math/quaternion.h>
#include <math/vector.h>

#include "ode/objects.h"
#include "objects/base/physicobj.h"
#include "ode/odecpp.h"

namespace ai
{
    void Obstacle::UnlinkFromOwner()
    {
        throw std::logic_error("Not implemented");
    }

    Obstacle::~Obstacle()
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion Obstacle::GetRotation() const
    {
        throw std::logic_error("Not implemented");
    }

    int Obstacle::DecRef()
    {
        throw std::logic_error("Not implemented");
    }

    Obstacle::Obstacle(m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    Obstacle::Obstacle(Obb const&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Obstacle::GetOwner() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obstacle::Disable()
    {
        throw std::logic_error("Not implemented");
    }

    CVector Obstacle::GetPosition() const
    {
        throw std::logic_error("Not implemented");
    }

    int Obstacle::IncRef()
    {
        return ++this->m_refCount;
    }

    void Obstacle::Enable()
    {
        throw std::logic_error("Not implemented");
    }

    CVector Obstacle::GetLinearVelocity() const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    bool Obstacle::bIsEnabled() const
    {
        return this->m_bIsEnabled;
    }

    float Obstacle::GetIntersectionRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    PhysicObj* Obstacle::GetOwnerPhysicObj() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obstacle::_Init()
    {
        throw std::logic_error("Not implemented");
    }
}

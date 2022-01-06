#include "obstacle.h"
#include <stdexcept>
#include <math/aabb.h>
#include <math/quaternion.h>
#include <math/vector.h>

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

    Obstacle::Obstacle(PhysicObj const*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    SphereForIntersection const* Obstacle::GetSphere() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obstacle::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Obstacle::bIsEnabled() const
    {
        throw std::logic_error("Not implemented");
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

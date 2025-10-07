#include "sphereforintersection.h"

#include <stdexcept>

#include "server/intersectionmanager.h"

extern "C"
{
#include <ode/collision.h>
}

namespace ai
{
    SphereForIntersection::~SphereForIntersection()
    {
        throw std::logic_error("Not implemented");
    }

    ai::SphereForIntersection* SphereForIntersection::CreateObject(float radius,
        ai::SphereForIntersection::SpherePurpose purpose, ai::Obstacle* owner)
    {
        throw std::logic_error("Not implemented");
    }

    ai::SphereForIntersection::SpherePurpose SphereForIntersection::GetPurpose() const
    {
        throw std::logic_error("Not implemented");
    }

    void SphereForIntersection::SetTargetClasses(
        const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>>& targetClasses)
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obstacle* SphereForIntersection::GetOwner() const
    {
        throw std::logic_error("Not implemented");
    }

    SphereForIntersection::SphereForIntersection(dxGeom* const geomId, ai::SphereForIntersection::SpherePurpose purpose,
        ai::Obstacle* owner) : Sphere(geomId, nullptr)
    {
        this->m_signature = -238835241;
        this->m_purpose = purpose;
        this->m_owner = owner;
        this->m_prev = 0;
        this->m_next = 0;
        dGeomSetData(GetGeomId(), this);
        if (purpose)
        {
            dGeomSetCategoryBits(geomId, 0);
            dGeomSetCollideBits(geomId, 1u);
        }
        else
        {
            dGeomSetCategoryBits(geomId, 1u);
            dGeomSetCollideBits(geomId, 0);
            ai::IntersectionManager::AddLookingSphere(this);
        }
    }
}

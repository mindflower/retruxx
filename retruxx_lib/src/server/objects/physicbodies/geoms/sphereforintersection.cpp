#include "sphereforintersection.h"

#include <stdexcept>

#include "server/dynamicscene.h"
#include "server/intersectionmanager.h"

extern "C"
{
#include <ode/collision.h>
}

namespace ai
{
    SphereForIntersection::~SphereForIntersection()
    {
        if (m_purpose == LOOKING)
        {
            IntersectionManager::RemoveLookingSphere(this);
        }
    }

    ai::SphereForIntersection* SphereForIntersection::CreateObject(float radius,
        ai::SphereForIntersection::SpherePurpose purpose, ai::Obstacle* owner)
    {
        if (radius < 0.1)
            radius = 0.1;

        auto sphere = dCreateSphere(gIntersectionSpace, radius);
        return new SphereForIntersection(sphere, purpose, owner);
    }

    ai::SphereForIntersection::SpherePurpose SphereForIntersection::GetPurpose() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SphereForIntersection::SetTargetClasses(
        const retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>>& targetClasses)
    {
        m_targetClasses = targetClasses;
    }

    ai::Obstacle* SphereForIntersection::GetOwner() const
    {
        return m_owner;
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

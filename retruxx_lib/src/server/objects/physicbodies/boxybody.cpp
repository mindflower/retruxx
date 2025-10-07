#include "boxybody.h"

#include <stdexcept>

#include "geoms/box.h"
#include "math/vector.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BoxyBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BoxyBody);

    BoxyBody::BoxyBody(std::vector<CollisionInfo, std::allocator<CollisionInfo>> const& collisionInfos, float massValue)
    {
        auto obj = Box::CreateObject(nullptr, collisionInfos.front().m_size, nullptr);
        auto& geom = m_pGeoms.front();
        geom->SetGeom(obj);

        UpdateGeomsByCollisionInfo(collisionInfos);
        auto realSize = collisionInfos.front().m_size;
        if (realSize.x < 0.1)
            realSize.x = 0.1;
        if (realSize.y < 0.1)
            realSize.y = 0.1;
        if (realSize.z < 0.1)
            realSize.z = 0.1;
        dMassSetBoxTotal(&m_mass, massValue, realSize.x, realSize.y, realSize.z);
    }

    m3d::Class* BoxyBody::GetClass() const
    {
        return RT_CLASS_LOCAL(BoxyBody);
    }

    m3d::Object* BoxyBody::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void BoxyBody::SetMass(float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* BoxyBody::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* BoxyBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicBody);
    }

    BoxyBody::~BoxyBody()
    {
        throw std::logic_error("Not implemented");
    }

    CVector BoxyBody::GetSize() const
    {
        throw std::logic_error("Not implemented");
    }

    BoxyBody::BoxyBody()
    {
        throw std::logic_error("Not implemented");
    }

    BoxyBody::BoxyBody(BoxyBody const&)
    {
        throw std::logic_error("Not implemented");
    }

    Box const* BoxyBody::_Box() const
    {
        throw std::logic_error("Not implemented");
    }
}

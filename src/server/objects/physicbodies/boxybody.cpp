#include "boxybody.h"

#include <stdexcept>

#include "math/vector.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BoxyBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BoxyBody);

    BoxyBody::BoxyBody(std::vector<CollisionInfo, std::allocator<CollisionInfo>> const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* BoxyBody::GetClass() const
    {
        throw std::logic_error("Not implemented");
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

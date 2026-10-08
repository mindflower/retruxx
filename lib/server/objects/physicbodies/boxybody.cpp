#include "boxybody.h"

#include <cassert>
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
        // RVA 0x850AB0
        // NOTE: the copy constructor asserts, so this never produces a body.
        return new BoxyBody(*this);
    }

    void BoxyBody::SetMass(float newMassValue)
    {
        // RVA 0x850C90
        CVector const size = _Box()->GetSize();
        dMassSetBoxTotal(&m_mass, newMassValue, size.x, size.y, size.z);
    }

    m3d::Object* BoxyBody::CreateObject()
    {
        // RVA 0x850B00
        // NOTE: the default constructor asserts, so this never produces a body.
        return new BoxyBody();
    }

    m3d::Class* BoxyBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicBody);
    }

    BoxyBody::~BoxyBody() = default;

    CVector BoxyBody::GetSize() const
    {
        // RVA 0x850B90
        return _Box()->GetSize();
    }

    BoxyBody::BoxyBody()
    {
        // RVA 0x850A70 - a boxy body must be built from collision infos.
        assert(0);
    }

    BoxyBody::BoxyBody(BoxyBody const&)
    {
        // RVA 0x850A40 - boxy bodies cannot be copied.
        assert(0);
    }

    Box const* BoxyBody::_Box() const
    {
        // RVA 0x850B80
        // The single geom's transform wraps the box itself.
        return static_cast<Box const*>(static_cast<GeomTransform const*>(m_pGeoms.front())->GetGeom());
    }

    Box* BoxyBody::_Box()
    {
        // Always inlined in the shipped build; the same as the const overload.
        return const_cast<Box*>(static_cast<BoxyBody const*>(this)->_Box());
    }
}

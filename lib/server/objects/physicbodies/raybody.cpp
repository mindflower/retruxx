#include "raybody.h"

#include <cassert>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RayBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RayBody);

    RayBody::RayBody(const ai::RayBodyPrototypeInfo& prototypeInfo) :
        SimplePhysicBody(prototypeInfo)
    {
        // RVA 0x904960
        // Swaps the transform's inner geom for a ray; the mass is a fixed unit sphere.
        Ray* ray = Ray::CreateObject(nullptr, prototypeInfo.m_length, CommonGeomMovedCallback);
        m_pGeoms.front()->SetGeom(ray);
        dMassSetSphereTotal(&m_mass, 1.0f, 1.0f);
    }

    RayBody::RayBody(float length)
    {
        auto* geom = m_pGeoms[0];
        auto* obj = ai::Ray::CreateObject(0, length, ai::CommonGeomMovedCallback);
        m_pGeoms[0]->SetGeom(obj);
        dMassSetSphereTotal(&m_mass, 1.0, 1.0);
    }

    RayBody::RayBody()
    {
        // RVA 0x904770 - a ray body must be built from a prototype or a length.
        assert(0);
    }

    RayBody::RayBody(const ai::RayBody& rhs)
    {
        // RVA 0x904740 - ray bodies cannot be copied.
        assert(0);
    }

    RayBody::~RayBody() = default;

    m3d::Object* RayBody::Clone()
    {
        // RVA 0x9047D0
        // NOTE: the copy constructor asserts, so this never produces a body.
        return new RayBody(*this);
    }

    m3d::Object* RayBody::CreateObject()
    {
        // RVA 0x904820
        // NOTE: the default constructor asserts, so this never produces a body.
        return new RayBody();
    }

    m3d::Class* RayBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicBody);
    }

    m3d::Class* RayBody::GetClass() const
    {
        return RT_CLASS_LOCAL(RayBody);
    }

    float RayBody::GetLength() const
    {
        // Declared in the PDB but never emitted in the shipped build (always inlined).
        return _Ray()->GetLength();
    }

    void RayBody::SetLength(float length)
    {
        // Declared in the PDB but never emitted in the shipped build (always inlined).
        _Ray()->SetLength(length);
    }

    CVector RayBody::GetDirection() const
    {
        // Declared in the PDB but never emitted in the shipped build (always inlined).
        return _Ray()->GetDirection();
    }

    void RayBody::SetDirection(const CVector& direction)
    {
        // Declared in the PDB but never emitted in the shipped build (always inlined).
        _Ray()->SetDirection(direction);
    }

    void RayBody::SetMass(float newMassValue)
    {
        // RVA 0x9047B0
        // A ray has no mass to set.
        assert(0);
    }

    const ai::Ray* RayBody::_Ray() const
    {
        // Declared in the PDB but never emitted in the shipped build (always inlined).
        return static_cast<Ray const*>(static_cast<GeomTransform const*>(m_pGeoms.front())->GetGeom());
    }

    ai::Ray* RayBody::_Ray()
    {
        // Declared in the PDB but never emitted in the shipped build (always inlined).
        return static_cast<Ray*>(m_pGeoms.front()->GetGeom());
    }
}

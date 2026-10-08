#include "sphericbody.h"
#include "physichelpers.h"

#include <cassert>
#include <stdexcept>

#include "geoms/sphere.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SphericBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SphericBody);

    SphericBodyPrototypeInfo::SphericBodyPrototypeInfo() :
        m_radius(1.0f)
    {
        // RVA 0x850FC0
    }

    bool SphericBodyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x850E10
        bool const result = SimplePhysicBodyPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_radius, xmlNode, "Radius");
        }
        return result;
    }

    m3d::Object* SphericBody::Clone()
    {
        // RVA 0x850D70
        // NOTE: the copy constructor asserts, so this never produces a body.
        return new SphericBody(*this);
    }

    m3d::Class* SphericBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicBody);
    }

    void SphericBody::SetMass(float newMassValue)
    {
        // RVA 0x850F80
        dMassSetSphereTotal(&m_mass, newMassValue, _Sphere()->GetRadius());
    }

    SphericBody::SphericBody(std::vector<CollisionInfo, std::allocator<CollisionInfo>> const& collisionInfos, float massValue)
    {
        // RVA 0x850E90 - swaps the transform's inner geom for a sphere of the first collision
        // info's radius (SetGeom deletes the old one and relinks the ODE transform).
        auto* obj = ai::Sphere::CreateObject(0, collisionInfos.front().m_radius, 0);
        m_pGeoms.front()->SetGeom(obj);

        UpdateGeomsByCollisionInfo(collisionInfos);
        dMassSetSphereTotal(&this->m_mass, massValue, collisionInfos.front().m_radius);
    }

    SphericBody::SphericBody(SphericBodyPrototypeInfo const& prototypeInfo) :
        SimplePhysicBody(prototypeInfo)
    {
        // RVA 0x850F10 - swaps the transform's inner geom for a sphere of the prototype's radius.
        // NOTE: the sphere is created without a moved callback.
        Sphere* sphere = Sphere::CreateObject(nullptr, prototypeInfo.m_radius, nullptr);
        m_pGeoms.front()->SetGeom(sphere);
        dMassSetSphereTotal(&m_mass, prototypeInfo.m_massValue, prototypeInfo.m_radius);
    }

    float SphericBody::GetRadius() const
    {
        // RVA 0x5DF080
        return _Sphere()->GetRadius();
    }

    SphericBody::~SphericBody() = default;

    m3d::Class* SphericBody::GetClass() const
    {
        return RT_CLASS_LOCAL(SphericBody);
    }

    m3d::Object* SphericBody::CreateObject()
    {
        // RVA 0x850DC0
        // NOTE: the default constructor asserts, so this never produces a body.
        return new SphericBody();
    }

    SphericBody::SphericBody()
    {
        // RVA 0x850D30 - a spheric body must be built from a prototype or collision infos.
        assert(0);
    }

    SphericBody::SphericBody(SphericBody const&)
    {
        // RVA 0x850D00 - spheric bodies cannot be copied.
        assert(0);
    }

    Sphere* SphericBody::_Sphere()
    {
        // Declared in the PDB but never emitted in the shipped build (always inlined).
        return static_cast<Sphere*>(m_pGeoms.front()->GetGeom());
    }

    Sphere const* SphericBody::_Sphere() const
    {
        // RVA 0x5DC0F0
        return static_cast<Sphere const*>(static_cast<GeomTransform const*>(m_pGeoms.front())->GetGeom());
    }
}

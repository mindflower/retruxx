#include "sphericbody.h"
#include "physichelpers.h"
#
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SphericBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SphericBody);

    SphericBodyPrototypeInfo::SphericBodyPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool SphericBodyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* SphericBody::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* SphericBody::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void SphericBody::SetMass(float)
    {
        throw std::logic_error("Not implemented");
    }

    SphericBody::SphericBody(std::vector<CollisionInfo, std::allocator<CollisionInfo>> const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    SphericBody::SphericBody(SphericBodyPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    float SphericBody::GetRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    SphericBody::~SphericBody()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* SphericBody::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* SphericBody::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    SphericBody::SphericBody()
    {
        throw std::logic_error("Not implemented");
    }

    SphericBody::SphericBody(SphericBody const&)
    {
        throw std::logic_error("Not implemented");
    }

    Sphere const* SphericBody::_Sphere() const
    {
        throw std::logic_error("Not implemented");
    }
}

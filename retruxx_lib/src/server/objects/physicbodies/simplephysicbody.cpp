#include "simplephysicbody.h"

#include <stdexcept>

#include "physichelpers.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SimplePhysicBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SimplePhysicBody);

    bool SimplePhysicBodyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    SimplePhysicBodyPrototypeInfo::SimplePhysicBodyPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    SimplePhysicBody::SimplePhysicBody(ai::SimplePhysicBodyPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    SimplePhysicBody::~SimplePhysicBody()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* SimplePhysicBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(PhysicBody);
    }

    m3d::Class* SimplePhysicBody::GetRtClass() const
    {
        return RT_CLASS_LOCAL(SimplePhysicBody);
    }

    SimplePhysicBody::SimplePhysicBody()
    {
    }
}

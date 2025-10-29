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
        RETRUXX_NOT_IMPLEMENTED;
    }

    SimplePhysicBodyPrototypeInfo::SimplePhysicBodyPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SimplePhysicBody::SimplePhysicBody(ai::SimplePhysicBodyPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SimplePhysicBody::~SimplePhysicBody()
    {
        RETRUXX_NOT_IMPLEMENTED;
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

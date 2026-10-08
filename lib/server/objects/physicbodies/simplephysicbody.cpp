#include "simplephysicbody.h"

#include <stdexcept>

#include "physichelpers.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SimplePhysicBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SimplePhysicBody);

    bool SimplePhysicBodyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x851030 - adds nothing of its own to what the base reads.
        return PhysicBodyPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    SimplePhysicBodyPrototypeInfo::SimplePhysicBodyPrototypeInfo()
    {
        // RVA 0x8510D0 - nothing of its own; the base does all the setup.
    }

    SimplePhysicBody::SimplePhysicBody(ai::SimplePhysicBodyPrototypeInfo const& prototypeInfo)
        : PhysicBody(prototypeInfo)
    {
        // RVA 0x851080 - nothing of its own.
    }

    SimplePhysicBody::~SimplePhysicBody() = default;

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

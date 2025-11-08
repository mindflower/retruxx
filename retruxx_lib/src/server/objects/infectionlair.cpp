#include "infectionlair.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(InfectionLair)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(InfectionLair);

    bool InfectionLairPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::SettlementPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    InfectionLairPrototypeInfo::InfectionLairPrototypeInfo()
    {
    }

    Obj* InfectionLairPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* InfectionLair::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    m3d::Class* InfectionLair::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionLairPrototypeInfo const* InfectionLair::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionLair::InfectionLair(InfectionLairPrototypeInfo const& prototype) : Settlement(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionLair::~InfectionLair()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* InfectionLair::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* InfectionLair::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

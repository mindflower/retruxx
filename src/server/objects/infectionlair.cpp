#include "infectionlair.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(InfectionLair)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(InfectionLair);

    bool InfectionLairPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    InfectionLairPrototypeInfo::InfectionLairPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* InfectionLairPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* InfectionLair::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* InfectionLair::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    InfectionLairPrototypeInfo const* InfectionLair::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    InfectionLair::InfectionLair(InfectionLairPrototypeInfo const& prototype) : Settlement(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    InfectionLair::~InfectionLair()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* InfectionLair::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* InfectionLair::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

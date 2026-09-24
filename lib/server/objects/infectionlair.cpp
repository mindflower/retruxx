#include "infectionlair.h"

#include <stdexcept>

#include "base/prototypemanager.h"
#include "core/kernel.h"

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
        // RVA 0x83AF80
        return new InfectionLair(*this);
    }

    m3d::Class* InfectionLair::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    m3d::Class* InfectionLair::GetClass() const
    {
        // RVA 0x83AF20
        return RT_CLASS_LOCAL(InfectionLair);
    }

    InfectionLairPrototypeInfo const* InfectionLair::GetPrototypeInfo() const
    {
        // RVA 0x83B370 - NOTE: the prototype is cast without a type check.
        return static_cast<InfectionLairPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    InfectionLair::InfectionLair(InfectionLairPrototypeInfo const& prototype) : Settlement(prototype)
    {
        // RVA 0x83AF50 - an InfectionLair is a plain Settlement that InfectionZone looks for by class.
    }

    // RVA 0x83AF70
    InfectionLair::~InfectionLair() = default;

    m3d::Object* InfectionLair::CreateObject()
    {
        // RVA 0x83B1B0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* InfectionLair::Clone()
    {
        // RVA 0x83AFF0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}

#include "lair.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Lair)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Lair);

    LairPrototypeInfo::LairPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* LairPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool LairPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Lair::Lair(LairPrototypeInfo const& prototype) : Settlement(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    int Lair::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    LairPrototypeInfo const* Lair::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Lair::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Lair::CreateChildren()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Lair::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    eGObjPropertySaveStatus Lair::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Lair::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Lair::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Lair::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Lair::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void Lair::Registration()
    {
        m_propertiesMap["MaxAttackers"] = 66;
        m_propertiesMap["MaxDefenders"] = 64;
    }

    void Lair::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Lair::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    bool Lair::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    int Lair::_GetMaxSpawnedVehicles()
    {
        throw std::logic_error("Not implemented");
    }

    Lair::~Lair()
    {
        throw std::logic_error("Not implemented");
    }

    int Lair::_GetNumSpawnedVehicles()
    {
        throw std::logic_error("Not implemented");
    }

    void Lair::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool Lair::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Lair::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void Lair::_OnObjectEntersLocation(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Lair::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Team* Lair::_GetDefendingTeam()
    {
        throw std::logic_error("Not implemented");
    }

    Team* Lair::_GetAttackingTeam()
    {
        throw std::logic_error("Not implemented");
    }
}

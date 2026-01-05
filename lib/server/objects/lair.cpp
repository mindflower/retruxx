#include "lair.h"
#include "team.h"
#include "core/kernel.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Lair)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Lair);

    LairPrototypeInfo::LairPrototypeInfo()
    {
        m_maxAttackers = 1;
        m_maxDefenders = 1;
    }

    Obj* LairPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool LairPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SettlementPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);

            m3d::SafeIntAttrib(m_maxAttackers, xmlNode, "MaxAttackers");
            M3D_ASSERT(m_maxAttackers <= MAX_VEHICLES_IN_TEAM);

            m3d::SafeIntAttrib(m_maxDefenders, xmlNode, "MaxDefenders");
            M3D_ASSERT(m_maxDefenders <= MAX_VEHICLES_IN_TEAM);
        }
        return result;
    }

    Lair::Lair(LairPrototypeInfo const& prototype) : Settlement(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Lair::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    LairPrototypeInfo const* Lair::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Lair::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Lair::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Lair::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    eGObjPropertySaveStatus Lair::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Lair::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Lair::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Lair::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Lair::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Lair::Registration()
    {
        m_propertiesMap["MaxAttackers"] = 66;
        m_propertiesMap["MaxDefenders"] = 64;
    }

    void Lair::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Lair::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Lair::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Lair::_GetMaxSpawnedVehicles()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Lair::~Lair()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Lair::_GetNumSpawnedVehicles()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Lair::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Lair::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Lair::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Lair::_OnObjectEntersLocation(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Lair::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Team* Lair::_GetDefendingTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Team* Lair::_GetAttackingTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

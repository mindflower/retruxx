#include "chassis.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Chassis)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Chassis);

    ChassisPrototypeInfo::ChassisPrototypeInfo()
    {
        this->m_maxHealth = 1.0;
        this->m_maxFuel = 1.0;
    }

    bool ChassisPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(this->m_maxHealth, xmlNode, "MaxHealth");
            m3d::SafeFloatAttrib(this->m_maxFuel, xmlNode, "MaxFuel");
            m3d::SafeStrAttrib(this->m_brakingSoundName, xmlNode, "BrakingSound");
            m3d::SafeStrAttrib(this->m_pneumoSoundName, xmlNode, "PneumoSound");
            m3d::SafeStrAttrib(this->m_gearShiftSoundName, xmlNode, "GearShiftSound");
            return 1;
        }
        return result;
    }

    Obj* ChassisPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Chassis::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Chassis::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Chassis::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    Chassis::Chassis(ChassisPrototypeInfo const& prototype) :
        VehiclePart(prototype),
        m_health(prototype.m_maxHealth, 0.0, prototype.m_maxHealth, 0.0),
        m_fuel(prototype.m_maxFuel, 0.0, prototype.m_maxFuel, 0.0)
    {
        throw retruxx::logic_error("Not implemented");
    }

    NumericInRangeRegenerating<float> const& Chassis::Health() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    NumericInRangeRegenerating<float>& Chassis::Health()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Chassis::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Chassis::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Chassis::Registration()
    {
        m_propertiesMap["Health"] = 26;
        m_propertiesMap["MaxHealth"] = 27;
        m_propertiesMap["Fuel"] = 9;
        m_propertiesMap["MaxFuel"] = 10;
    }

    NumericInRangeRegenerating<float> const& Chassis::Fuel() const
    {
        return this->m_fuel;
    }

    NumericInRangeRegenerating<float>& Chassis::Fuel()
    {
        return this->m_fuel;
    }

    void Chassis::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ChassisPrototypeInfo const* Chassis::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Chassis::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Chassis::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Chassis::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Chassis::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Chassis::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Chassis::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Chassis::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Chassis::~Chassis()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Chassis::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Chassis::_OnHealthValueBeforeApplyModifier(Modifier const&, float&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Chassis::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

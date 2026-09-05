#include "chassis.h"

#include <stdexcept>
#include "base/prototypemanager.h"

#include <core/ini.h>
#include <core/kernel.h>
#include <m3dapp.h>
#include "base/objcontainer.h"
#include "../relationship.h"
#include "../utils.h"
#include "vehicle.h"

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
        return new Chassis(*this);
    }

    bool Chassis::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x7C25A0. NOTE: the shipped code calls GetPrototypeInfo() here and
        // throws the result away; preserved as a comment rather than as a dead call.
        //
        // NOTE: the two current values are assigned raw while the two maximums go
        // through Numeric::set(), so setting Health/Fuel by property deliberately
        // skips the range clamp and the change notifications. Sibling classes such
        // as Barricade do route their current value through set(), so this is a
        // real asymmetry in Chassis rather than a decompilation artefact.
        switch (propertyId)
        {
        case 9:
            m_fuel.value().setUnsafe(newValue.GetAsFloat());
            return 1;
        case 10:
            m_fuel.maxValue().set(newValue.GetAsFloat());
            return 1;
        case 26:
            m_health.value().setUnsafe(newValue.GetAsFloat());
            return 1;
        case 27:
            m_health.maxValue().set(newValue.GetAsFloat());
            return 1;
        default:
            return VehiclePart::SetPropertyById(propertyId, newValue);
        }
    }

    void Chassis::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7C1F70 - only the current values are persisted; the maximums come
        // back from the prototype on load.
        VehiclePart::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("Health", CStr(m_health.value().get()).c_str());
        xmlNode->SetAttribute("Fuel", CStr(m_fuel.value().get()).c_str());
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
        m_health.m_BeforeValueApplyModifier =
            new BeforeApplyModifierFloatCallback(*this, &ai::Chassis::_OnHealthValueBeforeApplyModifier);
    }

    NumericInRangeRegenerating<float> const& Chassis::Health() const
    {
        return m_health;
    }

    NumericInRangeRegenerating<float>& Chassis::Health()
    {
        return m_health;
    }

    int Chassis::GetPropertyId(char const* propertyName) const
    {
        // RVA 0x7C2780
        auto const it = m_propertiesMap.find(propertyName);
        if (it == m_propertiesMap.end())
        {
            return VehiclePart::GetPropertyId(propertyName);
        }
        return it->second;
    }

    eGObjPropertySaveStatus Chassis::GetPropertySaveStatus(int id) const
    {
        // RVA 0x7C2740
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it == m_propertiesSaveStatesMap.end())
        {
            return VehiclePart::GetPropertySaveStatus(id);
        }
        return it->second;
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

    void Chassis::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7C2460 - each attribute defaults to whatever the value already
        // holds, so a save without them leaves the prototype's values in place.
        VehiclePart::LoadRuntimeValues(xmlFile, xmlNode);

        float health = m_health.value().get();
        m3d::SafeFloatAttrib(health, xmlNode, "Health");
        m_health.value().set(health);

        float fuel = m_fuel.value().get();
        m3d::SafeFloatAttrib(fuel, xmlNode, "Fuel");
        m_fuel.value().set(fuel);
    }

    ChassisPrototypeInfo const* Chassis::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), ChassisPrototypeInfo const);
    }

    m3d::Class* Chassis::GetClass() const
    {
        return RT_CLASS_LOCAL(Chassis);
    }

    CStr Chassis::GetPropertyName(int id) const
    {
        // RVA 0x7C2940 - reverse lookup, so a linear scan of the small map.
        for (auto const& prop : m_propertiesMap)
        {
            if (prop.second == id)
            {
                return prop.first;
            }
        }
        return VehiclePart::GetPropertyName(id);
    }

    void Chassis::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x7C28C0
        for (auto const& prop : m_propertiesMap)
        {
            Props.insert(prop.second);
        }
        VehiclePart::GetPropertiesIDs(Props);
    }

    void Chassis::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x7C2840
        for (auto const& prop : m_propertiesMap)
        {
            Props.insert(prop.first);
        }
        VehiclePart::GetPropertiesNames(Props);
    }

    bool Chassis::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7C2B00
        switch (propertyId)
        {
        case 9:
            retVal = m_fuel.value().get();
            return 1;
        case 10:
            retVal = m_fuel.maxValue().get();
            return 1;
        case 26:
            retVal = m_health.value().get();
            return 1;
        case 27:
            retVal = m_health.maxValue().get();
            return 1;
        default:
            return VehiclePart::_GetPropertyInternal(propertyId, retVal);
        }
    }

    bool Chassis::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x7C2BC0 - a factory-fresh chassis is full, so the current value and
        // the maximum share the same default.
        ChassisPrototypeInfo const* prototype = GetPrototypeInfo();
        switch (propertyId)
        {
        case 9:
        case 10:
            retVal = prototype->m_maxFuel;
            return 1;
        case 26:
        case 27:
            retVal = prototype->m_maxHealth;
            return 1;
        default:
            return VehiclePart::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    void Chassis::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x7C29B0 - a save status of 0 is the default and is not recorded, so
        // GetPropertySaveStatus falls through to the base class for those.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    Chassis::~Chassis() = default;

    m3d::Object* Chassis::Clone()
    {
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    bool Chassis::_OnHealthValueBeforeApplyModifier(Modifier const& modifier, float& newHealth)
    {
        // RVA 0x7C2C40. Runs before damage (or repair) lands on the chassis, and
        // is the hook that turns a hit into an "I am under attack" reaction and
        // into a reputation loss when the player is the one shooting.
        PhysicObj* owner = GetOwner();
        if (!owner || !owner->IsKindOf(&Vehicle::m_classVehicle))
        {
            return 0;
        }
        auto* vehicle = static_cast<Vehicle*>(owner);

        if (modifier.m_SenderID != -1)
        {
            Obj* sender = theObjects->GetEntityByObjId(modifier.m_SenderID);
            if (sender && theRelationship->GetTolerance(sender->GetBelong(), GetBelong()) <= 2.0f)
            {
                vehicle->CauseEvent(GE_UNDER_ATTACK, 0.0f, m3d::AIParam(modifier.m_SenderID), m3d::AIParam());
            }
        }

        // God mode swallows the change entirely.
        if (vehicle->getGodMode())
        {
            return 1;
        }

        if (modifier.m_SenderID != -1 && M3D_APP->GetCurGameMode() != 1)
        {
            Obj* sender = theObjects->GetEntityByObjId(modifier.m_SenderID);
            if (sender && sender->IsKindOf(&Vehicle::m_classVehicle))
            {
                auto* senderVehicle = static_cast<Vehicle*>(sender);
                if (senderVehicle->bIsControlledByPlayer() && GetBelong() != senderVehicle->GetBelong())
                {
                    // Only actual damage costs reputation, and it costs in
                    // proportion to the fraction of the chassis destroyed.
                    float const damage = m_health.value().get() - newHealth;
                    if (damage > 0.0f)
                    {
                        DecToleranceWhenDamageFromPlayerInflicted(vehicle, damage / m_health.maxValue().get());
                    }
                }
            }
        }
        return 0;
    }

    m3d::Object* Chassis::CreateObject()
    {
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai

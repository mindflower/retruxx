#include "cabin.h"

#include "base/prototypemanager.h"
#include <server/resourcemanager.h>

#include <algorithm>
#include <core/kernel.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Cabin)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Cabin);

    CabinPrototypeInfo::CabinPrototypeInfo()
    {
        this->m_maxPower = 1.0;
        this->m_maxTorque = 1.0;
        this->m_maxSpeed = 1.0;
        this->m_fuelConsumption = 1.0;
        this->m_control = 50.0;
    }

    ai::Obj* CabinPrototypeInfo::CreateTargetObject() const
    {
        return new Cabin(*this);
    }

    bool CabinPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(this->m_maxPower, xmlNode, "MaxPower");
            m3d::SafeFloatAttrib(this->m_maxTorque, xmlNode, "MaxTorque");
            m3d::SafeFloatAttrib(this->m_maxSpeed, xmlNode, "MaxSpeed");
            m3d::SafeFloatAttrib(this->m_fuelConsumption, xmlNode, "FuelConsumption");
            m3d::SafeStrAttrib(this->m_engineHighSoundName, xmlNode, "EngineHighSound");
            m3d::SafeStrAttrib(this->m_engineLowSoundName, xmlNode, "EngineLowSound");
            m3d::SafeFloatAttrib(this->m_control, xmlNode, "Control");

            this->m_control = std::clamp(this->m_control, 0.0f, 100.0f);
            this->m_maxSpeed = this->m_maxSpeed * 0.27777779;

            ref_ptr gadgetNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(gadgetNode, "GadgetDescription");
            if (!gadgetNode->IsEmpty() && gadgetNode->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
            {
                ref_ptr slotNode = xmlFile->CreateNode();
                for (gadgetNode->GetFirstChild(slotNode, "Slot"); !slotNode->IsEmpty();
                     slotNode->GetNextSibling(slotNode, "Slot"))
                {
                    CStr resourceType;
                    m3d::SafeStrAttrib(resourceType, slotNode, "ResourceType");

                    int maxAmount = -1;
                    m3d::SafeIntAttrib(maxAmount, slotNode, "MaxAmount");

                    // TOOD: check this!!
                    int slot = -1;
                    for (auto const& gadgetSlot : this->m_gadgetSlots)
                    {
                        if (gadgetSlot.second.y > slot)
                        {
                            slot = gadgetSlot.second.y;
                        }
                    }
                    m_gadgetSlots.emplace(resourceType, PointBase<int>(slot + 1, slot + maxAmount));
                }
            }
        }
        return result;
    }

    int CabinPrototypeInfo::GetMaxGadgets(CStr const& gadgetResourceName) const
    {
        // RVA 0x6CC1C0 - a cabin's gadget slots are keyed by the resource they take. The first slot whose
        // resource the asked-for one is a kind of answers for all of them, and how many fit is the width
        // of that slot's range.
        int const gadgetResourceId = theResourceManager->GetResourceId(gadgetResourceName);
        for (auto const& slot : m_gadgetSlots)
        {
            int const slotResourceId = theResourceManager->GetResourceId(slot.first);
            if (theResourceManager->bResourceIsKindOf(gadgetResourceId, slotResourceId))
            {
                return slot.second.y - slot.second.x + 1;
            }
        }
        return 0;
    }

    float Cabin::GetMaxSpeed() const
    {
        return this->m_maxSpeed;
    }

    void Cabin::SetMaxPower(float maxPower)
    {
        // RVA 0x5CB6E0
        m_maxPower = maxPower;
    }

    float Cabin::GetFuelConsumption() const
    {
        return this->m_fuelConsumption;
    }

    float Cabin::GetMaxTorque() const
    {
        return this->m_maxTorque;
    }

    Cabin::Cabin(CabinPrototypeInfo const& prototypeInfo) : VehiclePart(prototypeInfo)
    {
        this->m_maxPower = prototypeInfo.m_maxPower;
        this->m_maxTorque = prototypeInfo.m_maxTorque;
        this->m_maxSpeed = prototypeInfo.m_maxSpeed;
        this->m_fuelConsumption = prototypeInfo.m_fuelConsumption;
        this->m_control = 50.0;
        this->m_maxGadgets = 3;
    }

    void Cabin::SetMaxSpeed(float speed)
    {
        // RVA 0x5CB6B0
        m_maxSpeed = speed;
    }

    float Cabin::GetMaxPower() const
    {
        // RVA 0x453190
        return m_maxPower;
    }

    void Cabin::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x6CC390
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        VehiclePart::GetPropertiesIDs(Props);
    }

    m3d::Class* Cabin::GetClass() const
    {
        return RT_CLASS_LOCAL(Cabin);
    }

    int Cabin::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x6CC2A0
        auto const it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return VehiclePart::GetPropertyId(PropertyName);
    }

    CStr Cabin::GetPropertyName(int id) const
    {
        // RVA 0x6CC410
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return VehiclePart::GetPropertyName(id);
    }

    eGObjPropertySaveStatus Cabin::GetPropertySaveStatus(int id) const
    {
        // RVA 0x6CC260
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return VehiclePart::GetPropertySaveStatus(id);
    }

    float Cabin::GetControl() const
    {
        return m_control;
    }

    void Cabin::SetMaxTorque(float maxTorque)
    {
        // RVA 0x5CB700
        m_maxTorque = maxTorque;
    }

    CabinPrototypeInfo const* Cabin::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), CabinPrototypeInfo const);
    }

    m3d::Class* Cabin::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    bool Cabin::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x6CBD00 - NOTE: MaxPower has no case here, so it is the one cabin property that cannot be
        // set through the property system even though it can be read back.
        switch (propertyId)
        {
            case 22:
                m_maxTorque = newValue.GetAsFloat();
                return true;
            case 23:
                m_maxSpeed = newValue.GetAsFloat();
                return true;
            case 24:
                m_fuelConsumption = newValue.GetAsFloat();
                return true;
            case 25:
                m_control = newValue.GetAsFloat();
                return true;
            default:
                return VehiclePart::SetPropertyById(propertyId, newValue);
        }
    }

    void Cabin::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x6CC310
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        VehiclePart::GetPropertiesNames(Props);
    }

    void Cabin::Registration()
    {
        m_propertiesMap["MaxTorque"] = 22;
        m_propertiesMap["MaxSpeed"] = 23;
        m_propertiesMap["FuelConsumption"] = 24;
        m_propertiesMap["Control"] = 25;
    }

    int Cabin::GetMaxGadgets(CStr const& gadgetResourceName) const
    {
        // RVA 0x6CC240
        return GetPrototypeInfo()->GetMaxGadgets(gadgetResourceName);
    }

    void Cabin::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x6CC840 - SAVE_PROP_NORMAL is the default and is not recorded.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool Cabin::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        switch (propertyId)
        {
        case 22:
            retVal = m_maxTorque;
            return true;
        case 23:
            retVal = m_maxSpeed;
            return true;
        case 24:
            retVal = m_fuelConsumption;
            return true;
        case 25:
            retVal = m_control;
            return true;
        default:
            return ai::VehiclePart::_GetPropertyInternal(propertyId, retVal);
        }
        return false;
    }

    bool Cabin::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        auto prototypeInfo = (ai::CabinPrototypeInfo const*)GetPrototypeInfo();
        switch (propertyId)
        {
        case 22:
            retVal = prototypeInfo->m_maxTorque;
            return true;

        case 23:
            retVal = prototypeInfo->m_maxSpeed;
            return true;

        case 24:
            retVal = prototypeInfo->m_fuelConsumption;
            return true;

        case 25:
            retVal = prototypeInfo->m_control;
            return true;

        default:
            return ai::VehiclePart::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        return false;
    }

    Cabin::~Cabin() = default;

    m3d::Object* Cabin::Clone()
    {
        // RVA 0x6CBDF0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Cabin::CreateObject()
    {
        // RVA 0x6CBFB0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai

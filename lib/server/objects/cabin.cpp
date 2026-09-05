#include "cabin.h"

#include <algorithm>
#include <stdexcept>
#include "base/prototypemanager.h"

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

    int CabinPrototypeInfo::GetMaxGadgets(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Cabin::GetMaxSpeed() const
    {
        return this->m_maxSpeed;
    }

    void Cabin::SetMaxPower(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void Cabin::SetMaxSpeed(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Cabin::GetMaxPower() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cabin::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Cabin::GetClass() const
    {
        return RT_CLASS_LOCAL(Cabin);
    }

    int Cabin::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Cabin::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Cabin::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Cabin::GetControl() const
    {
        return m_control;
    }

    void Cabin::SetMaxTorque(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CabinPrototypeInfo const* Cabin::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), CabinPrototypeInfo const);
    }

    m3d::Class* Cabin::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    bool Cabin::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cabin::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cabin::Registration()
    {
        m_propertiesMap["MaxTorque"] = 22;
        m_propertiesMap["MaxSpeed"] = 23;
        m_propertiesMap["FuelConsumption"] = 24;
        m_propertiesMap["Control"] = 25;
    }

    int Cabin::GetMaxGadgets(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Cabin::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Cabin::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

#include "cabin.h"

#include <algorithm>
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
        throw retruxx::logic_error("Not implemented");
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
                for (gadgetNode->GetFirstChild(slotNode, "Slot"); !slotNode->IsEmpty(); slotNode->GetNextSibling(slotNode, "Slot"))
                {
                    CStr resourceType;
                    m3d::SafeStrAttrib(resourceType, slotNode, "ResourceType");

                    int maxAmount = -1;
                    m3d::SafeIntAttrib(maxAmount, slotNode, "MaxAmount");

                    // TOOD: check this!!
                    int slot = -1;
                    for (const auto& gadgetSlot : this->m_gadgetSlots)
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
        throw retruxx::logic_error("Not implemented");
    }

    float Cabin::GetMaxSpeed() const
    {
        return this->m_maxSpeed;
    }

    void Cabin::SetMaxPower(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Cabin::GetFuelConsumption() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Cabin::GetMaxTorque() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Cabin::Cabin(CabinPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Cabin::SetMaxSpeed(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Cabin::GetMaxPower() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Cabin::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Cabin::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Cabin::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Cabin::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Cabin::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Cabin::GetControl() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Cabin::SetMaxTorque(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CabinPrototypeInfo const* Cabin::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Cabin::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    bool Cabin::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Cabin::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Cabin::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Cabin::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Cabin::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Cabin::~Cabin()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Cabin::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Cabin::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

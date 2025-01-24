#include "cabin.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Cabin)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Cabin);

    CabinPrototypeInfo::CabinPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    ai::Obj* CabinPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool CabinPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CabinPrototypeInfo::GetMaxGadgets(CStr const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Cabin::GetMaxSpeed() const
    {
        throw retruxx::logic_error("Not implemented");
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

#include "cabin.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Cabin)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Cabin);

    CabinPrototypeInfo::CabinPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* CabinPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool CabinPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    int CabinPrototypeInfo::GetMaxGadgets(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    float Cabin::GetMaxSpeed() const
    {
        throw std::logic_error("Not implemented");
    }

    void Cabin::SetMaxPower(float)
    {
        throw std::logic_error("Not implemented");
    }

    float Cabin::GetFuelConsumption() const
    {
        throw std::logic_error("Not implemented");
    }

    float Cabin::GetMaxTorque() const
    {
        throw std::logic_error("Not implemented");
    }

    Cabin::Cabin(CabinPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void Cabin::SetMaxSpeed(float)
    {
        throw std::logic_error("Not implemented");
    }

    float Cabin::GetMaxPower() const
    {
        throw std::logic_error("Not implemented");
    }

    void Cabin::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Cabin::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int Cabin::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Cabin::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Cabin::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    float Cabin::GetControl() const
    {
        throw std::logic_error("Not implemented");
    }

    void Cabin::SetMaxTorque(float)
    {
        throw std::logic_error("Not implemented");
    }

    CabinPrototypeInfo const* Cabin::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Cabin::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    bool Cabin::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Cabin::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Cabin::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    int Cabin::GetMaxGadgets(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Cabin::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool Cabin::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Cabin::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    Cabin::~Cabin()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Cabin::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Cabin::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

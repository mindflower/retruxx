#include "ware.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Ware)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Ware);

    CStr const& WarePrototypeInfo::GetModelName() const
    {
        throw std::logic_error("Not implemented");
    }

    WarePrototypeInfo::WarePrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    int WarePrototypeInfo::GetMinCount() const
    {
        throw std::logic_error("Not implemented");
    }

    float WarePrototypeInfo::GetPriceDispersion() const
    {
        throw std::logic_error("Not implemented");
    }

    int WarePrototypeInfo::GetMaxCount() const
    {
        throw std::logic_error("Not implemented");
    }

    bool WarePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* WarePrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    Ware::Ware(WarePrototypeInfo const& prototype) : Obj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    bool Ware::ApplyModifier(Modifier const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Ware::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    unsigned Ware::GetPrice(IPriceCoeffProvider const*) const
    {
        throw std::logic_error("Not implemented");
    }

    int Ware::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Ware::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    WarePrototypeInfo const* Ware::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Ware::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    NumericInRange<float> const& Ware::Durability() const
    {
        throw std::logic_error("Not implemented");
    }

    NumericInRange<float>& Ware::Durability()
    {
        throw std::logic_error("Not implemented");
    }

    void Ware::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Ware::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void Ware::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void Ware::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Ware::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Ware::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    Ware::~Ware()
    {
        throw std::logic_error("Not implemented");
    }

    bool Ware::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Ware::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Ware::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Ware::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Ware::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

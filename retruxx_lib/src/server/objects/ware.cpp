#include "ware.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Ware)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Ware);

    CStr const& WarePrototypeInfo::GetModelName() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    WarePrototypeInfo::WarePrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int WarePrototypeInfo::GetMinCount() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float WarePrototypeInfo::GetPriceDispersion() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int WarePrototypeInfo::GetMaxCount() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool WarePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* WarePrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Ware::Ware(WarePrototypeInfo const& prototype) :
        Obj(prototype),
        m_durability(prototype.m_maxDurability, 0.0, prototype.m_maxDurability)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Ware::ApplyModifier(Modifier const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Ware::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    unsigned Ware::GetPrice(IPriceCoeffProvider const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Ware::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Ware::CanChildBeAdded(m3d::Class*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    WarePrototypeInfo const* Ware::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Ware::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    NumericInRange<float> const& Ware::Durability() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    NumericInRange<float>& Ware::Durability()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Ware::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Ware::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Ware::Registration()
    {
        m_propertiesMap["Durability"] = 19;
    }

    void Ware::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Ware::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Ware::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Ware::~Ware()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Ware::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Ware::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Ware::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Ware::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Ware::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

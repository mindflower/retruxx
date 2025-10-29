#include "ware.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Ware)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Ware);

    CStr const& WarePrototypeInfo::GetModelName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WarePrototypeInfo::WarePrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WarePrototypeInfo::GetMinCount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float WarePrototypeInfo::GetPriceDispersion() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WarePrototypeInfo::GetMaxCount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool WarePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* WarePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Ware::Ware(WarePrototypeInfo const& prototype) :
        Obj(prototype),
        m_durability(prototype.m_maxDurability, 0.0, prototype.m_maxDurability)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Ware::ApplyModifier(Modifier const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Ware::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    unsigned Ware::GetPrice(IPriceCoeffProvider const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Ware::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Ware::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WarePrototypeInfo const* Ware::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Ware::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NumericInRange<float> const& Ware::Durability() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NumericInRange<float>& Ware::Durability()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Ware::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Ware::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Ware::Registration()
    {
        m_propertiesMap["Durability"] = 19;
    }

    void Ware::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Ware::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Ware::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Ware::~Ware()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Ware::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Ware::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Ware::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Ware::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Ware::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

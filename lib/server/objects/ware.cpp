#include "ware.h"
#include "core/ini.h"
#include "core/kernel.h"

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
        m_maxDurability = 1.0f;
        m_maxItems = 1;
        m_priceDispersion = 0.0f;
        m_minCount = 0;
        m_maxCount = 50;
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

    bool WarePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeUintAttrib(m_maxItems, xmlNode, "MaxItems");
            m3d::SafeFloatAttrib(m_maxDurability, xmlNode, "Durability");
            m3d::SafeFloatAttrib(m_priceDispersion, xmlNode, "PriceDispersion");
            M3D_ASSERT(m_priceDispersion >= 0.0f && m_priceDispersion <= 100.0f);

            m3d::SafeStrAttrib(m_modelName, xmlNode, "ModelFile");
            m3d::SafeIntAttrib(m_minCount, xmlNode, "MinCount");
            m3d::SafeIntAttrib(m_maxCount, xmlNode, "MaxCount");
        }
        return result;
    }

    Obj* WarePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Ware::Ware(WarePrototypeInfo const& prototype) : Obj(prototype), m_durability(prototype.m_maxDurability, 0.0, prototype.m_maxDurability)
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
}  // namespace ai

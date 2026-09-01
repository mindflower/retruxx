#include "ware.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "base/prototypemanager.h"
#include "base/obj.h"
#include <server/modifier.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Ware)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Ware);

    int GetIntPrice(float price)
    {
        return price >= 1.0f ? static_cast<int>(price) : 1;
    }

    float GetDurabilityPriceCoeff(ai::NumericInRange<float> const& durability)
    {
        if (durability.maxValue().get() >= 0.001f)
        {
            return durability.value().get() / durability.maxValue().get();
        }
        return 0.0f;
    }

    CStr const& WarePrototypeInfo::GetModelName() const
    {
        return m_modelName;
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
        return m_minCount;
    }

    float WarePrototypeInfo::GetPriceDispersion() const
    {
        return m_priceDispersion;
    }

    int WarePrototypeInfo::GetMaxCount() const
    {
        return m_maxCount;
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
        return new Ware(*this);
    }

    Ware::Ware(WarePrototypeInfo const& prototypeInfo) :
        Obj(prototypeInfo),
        m_durability(prototypeInfo.m_maxDurability, 0.0, prototypeInfo.m_maxDurability)
    {
        m_maxItems = prototypeInfo.m_maxItems;
    }

    bool Ware::ApplyModifier(Modifier const& modifier)
    {
        if (ai::Obj::ApplyModifier(modifier))
        {
            return true;
        }
        if (modifier.m_PropertyName == "dur")
        {
            m_durability.value().ApplyModifier(modifier, m_durability.maxValue().get());
            return true;
        }
        return false;
    }

    m3d::Class* Ware::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    unsigned Ware::GetPrice(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        auto const* proto = GetPrototypeInfo();
        float const durabilityCoeff = ai::GetDurabilityPriceCoeff(m_durability);
        float const coeff = GetPriceCoeff(priceCoeffProvider) * durabilityCoeff;
        float const price = static_cast<float>(static_cast<double>(proto->GetBasePrice()) * coeff);
        return ai::GetIntPrice(price);
    }

    int Ware::GetPropertyId(char const* propName) const
    {
        auto it = Ware::m_propertiesMap.find(propName);
        if (it != Ware::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::Obj::GetPropertyId(propName);
    }

    bool Ware::CanChildBeAdded(m3d::Class*) const
    {
        return false;
    }

    WarePrototypeInfo const* Ware::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), WarePrototypeInfo const);
    }

    CStr Ware::GetPropertyName(int id) const
    {
        for (auto const& [name, propId] : Ware::m_propertiesMap)
        {
            if (propId == id)
            {
                return name;
            }
        }
        return ai::Obj::GetPropertyName(id);
    }

    NumericInRange<float> const& Ware::Durability() const
    {
        return m_durability;
    }

    NumericInRange<float>& Ware::Durability()
    {
        return m_durability;
    }

    void Ware::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        ai::Obj::GetPropertiesIDs(Props);
        for (auto const& [name, id] : Ware::m_propertiesMap)
        {
            Props.insert(id);
        }
    }

    m3d::Class* Ware::GetClass() const
    {
        return RT_CLASS_LOCAL(Ware);
    }

    void Ware::Registration()
    {
        m_propertiesMap["Durability"] = 19;
    }

    void Ware::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        ai::Obj::GetPropertiesNames(Props);
        for (auto const& [name, id] : Ware::m_propertiesMap)
        {
            Props.insert(name);
        }
    }

    eGObjPropertySaveStatus Ware::GetPropertySaveStatus(int id) const
    {
        auto it = Ware::m_propertiesSaveStatesMap.find(id);
        if (it != Ware::m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return ai::Obj::GetPropertySaveStatus(id);
    }

    bool Ware::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId != 19)
            return Obj::SetPropertyById(propertyId, newValue);

        m_durability.value().set(newValue.GetAsFloat());
        return 1;
    }

    Ware::~Ware() = default;

    bool Ware::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        if (propertyId != 19)
        {
            return ai::Obj::_GetPropertyInternal(propertyId, retVal);
        }
        retVal = m_durability.value().get();
        return true;
    }

    bool Ware::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        if (propertyId != 19)
        {
            return ai::Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        retVal = GetPrototypeInfo() ? GetPrototypeInfo()->m_maxDurability : 0.0f;
        return true;
    }

    void Ware::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    m3d::Object* Ware::CreateObject()
    {
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Ware::Clone()
    {
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

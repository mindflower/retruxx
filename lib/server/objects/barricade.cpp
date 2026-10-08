#include "barricade.h"

#include <stdexcept>
#include "core/kernel.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Barricade)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Barricade);

    BarricadePrototypeInfo::BarricadePrototypeInfo()
    {
        m_probability = 1.0f;
    }

    Obj* BarricadePrototypeInfo::CreateTargetObject() const
    {
        return new Barricade(*this);
    }

    bool BarricadePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::ObjPrefabPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_probability, xmlNode, "Probability");
        }
        return result;
    }

    void BarricadePrototypeInfo::_InternalCopyFrom(PrototypeInfo const& rhs)
    {
        // RVA 0x8423B0
        // NOTE: skips ObjPrefabPrototypeInfo's own copy: the prefab's object infos are copied
        // here directly after the SimplePhysicObj part.
        BarricadePrototypeInfo const& other = static_cast<BarricadePrototypeInfo const&>(rhs);
        SimplePhysicObjPrototypeInfo::operator=(other);
        m_objInfos = other.m_objInfos;
        m_probability = other.m_probability;
    }

    m3d::Class* Barricade::GetClass() const
    {
        return RT_CLASS_LOCAL(Barricade);
    }

    int Barricade::GetPropertyId(char const* propName) const
    {
        auto it = Barricade::m_propertiesMap.find(propName);
        if (it != Barricade::m_propertiesMap.end())
        {
            return it->second;
        }

        return ObjPrefab::GetPropertyId(propName);
    }

    eGObjPropertySaveStatus Barricade::GetPropertySaveStatus(int id) const
    {
        // RVA 0x841B50
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return SimplePhysicObj::GetPropertySaveStatus(id);
    }

    CStr Barricade::GetPropertyName(int id) const
    {
        // RVA 0x841D80
        // A linear search, since the map is keyed by name.
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return SimplePhysicObj::GetPropertyName(id);
    }

    BarricadePrototypeInfo const* Barricade::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), BarricadePrototypeInfo const);
    }

    void Barricade::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        SimplePhysicObj::GetPropertiesIDs(Props);
    }

    void Barricade::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
        if (ai::theObjects->m_SaveType == ObjContainer::eSAVE_TYPES::SAVE_LEVEL)
        {
            if ((float)((float)rand() * 0.000030518509) > m_probability.value().get())
                Remove();
        }
    }

    void Barricade::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        SimplePhysicObj::GetPropertiesNames(Props);
    }

    void Barricade::Registration()
    {
        m_propertiesMap["Probability"] = 67;
    }

    Barricade::Barricade(BarricadePrototypeInfo const& prototype) : ObjPrefab(prototype), m_probability(prototype.m_probability, 0.0, 1.0)
    {
    }

    m3d::Class* Barricade::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ObjPrefab);
    }

    bool Barricade::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId != 67)
            return SimplePhysicObj::SetPropertyById(propertyId, newValue);

        m_probability.value().set(newValue.GetAsFloat());
        return 1;
    }

    void Barricade::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x841F70
        // The default save status is not stored.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool Barricade::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x8422D0
        // NOTE: the default is 1, not the prototype's probability (the prototype is fetched
        // and ignored).
        GetPrototypeInfo();
        if (propertyId != 67)
        {
            return SimplePhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        retVal = 1.0f;
        return true;
    }

    Barricade::~Barricade() = default;

    bool Barricade::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x842270
        if (propertyId != 67)
        {
            return SimplePhysicObj::_GetPropertyInternal(propertyId, retVal);
        }
        retVal = m_probability.value().get();
        return true;
    }

    m3d::Object* Barricade::CreateObject()
    {
        // RVA 0x8415D0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Barricade::Clone()
    {
        // RVA 0x841410
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

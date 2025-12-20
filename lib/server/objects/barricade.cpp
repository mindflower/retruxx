#include "barricade.h"

#include <stdexcept>
#include "base/objcontainer.h"

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

    void BarricadePrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    eGObjPropertySaveStatus Barricade::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Barricade::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BarricadePrototypeInfo const* Barricade::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Barricade::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void Barricade::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void Barricade::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Barricade::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Barricade::~Barricade() = default;

    bool Barricade::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Barricade::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Barricade::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

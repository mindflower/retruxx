#include "barricade.h"

#include <stdexcept>

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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Barricade::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void Barricade::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Barricade::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ObjPrefab);
    }

    bool Barricade::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Barricade::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Barricade::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Barricade::~Barricade()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

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

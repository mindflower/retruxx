#include "lightobj.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(LightObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(LightObj);

    bool LightObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::SgNodeObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    ai::Obj* LightObjPrototypeInfo::CreateTargetObject() const
    {
        return new LightObj(*this);
    }

    LightObjPrototypeInfo::LightObjPrototypeInfo()
    {
        m_bIsUpdating = 0;
    }

    m3d::Class* LightObj::GetClass() const
    {
        return RT_CLASS_LOCAL(LightObj);
    }

    void LightObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void LightObj::Registration()
    {
    }

    LightObj::LightObj(LightObjPrototypeInfo const& prototype) : SgNodeObj(prototype)
    {
    }

    void LightObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool LightObj::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        return SgNodeObj::SetPropertyById(propertyId, newValue);
    }

    eGObjPropertySaveStatus LightObj::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* LightObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNodeObj);
    }

    LightObjPrototypeInfo const* LightObj::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr LightObj::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LightObj::GetPropertyId(char const* propName) const
    {
        auto it = LightObj::m_propertiesMap.find(propName);
        if (it != LightObj::m_propertiesMap.end())
        {
            return it->second;
        }

        return SgNodeObj::GetPropertyId(propName);
    }

    bool LightObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    LightObj::~LightObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool LightObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* LightObj::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* LightObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

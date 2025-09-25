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
        throw retruxx::logic_error("Not implemented");
    }

    LightObjPrototypeInfo::LightObjPrototypeInfo()
    {
        m_bIsUpdating = 0;
    }

    m3d::Class* LightObj::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void LightObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void LightObj::Registration()
    {
    }

    LightObj::LightObj(LightObjPrototypeInfo const& prototype) : SgNodeObj(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void LightObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool LightObj::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus LightObj::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* LightObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNodeObj);
    }

    LightObjPrototypeInfo const* LightObj::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr LightObj::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LightObj::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool LightObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    LightObj::~LightObj()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool LightObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* LightObj::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* LightObj::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

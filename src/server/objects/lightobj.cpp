#include "lightobj.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(LightObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(LightObj);

    bool LightObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* LightObjPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    LightObjPrototypeInfo::LightObjPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* LightObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void LightObj::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void LightObj::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    LightObj::LightObj(LightObjPrototypeInfo const& prototype) : SgNodeObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void LightObj::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool LightObj::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus LightObj::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* LightObj::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    LightObjPrototypeInfo const* LightObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr LightObj::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    int LightObj::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool LightObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    LightObj::~LightObj()
    {
        throw std::logic_error("Not implemented");
    }

    bool LightObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* LightObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* LightObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

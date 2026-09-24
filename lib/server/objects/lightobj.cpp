#include "lightobj.h"

#include <stdexcept>

#include "base/prototypemanager.h"
#include "core/kernel.h"

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
        // RVA 0x82E270
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

    void LightObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x82E860
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        SgNodeObj::GetPropertiesNames(Props);
    }

    void LightObj::Registration()
    {
        // RVA 0x82E200 - LightObj has no properties of its own.
    }

    LightObj::LightObj(LightObjPrototypeInfo const& prototype) : SgNodeObj(prototype)
    {
    }

    void LightObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x82E960
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        SgNodeObj::GetPropertiesIDs(Props);
    }

    bool LightObj::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        return SgNodeObj::SetPropertyById(propertyId, newValue);
    }

    eGObjPropertySaveStatus LightObj::GetPropertySaveStatus(int id) const
    {
        // RVA 0x82E7C0
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return SgNodeObj::GetPropertySaveStatus(id);
    }

    m3d::Class* LightObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNodeObj);
    }

    LightObjPrototypeInfo const* LightObj::GetPrototypeInfo() const
    {
        // RVA 0x82E790 - NOTE: the prototype is cast without a type check.
        return static_cast<LightObjPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CStr LightObj::GetPropertyName(int id) const
    {
        // RVA 0x82EA60 - a linear search, since the map is keyed by name.
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return SgNodeObj::GetPropertyName(id);
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

    bool LightObj::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x82E230
        return SgNodeObj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    // RVA 0x82E1F0
    LightObj::~LightObj() = default;

    bool LightObj::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x82E210
        return SgNodeObj::_GetPropertyInternal(propertyId, retVal);
    }

    m3d::Object* LightObj::CreateObject()
    {
        // RVA 0x82E4A0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* LightObj::Clone()
    {
        // RVA 0x82E2E0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

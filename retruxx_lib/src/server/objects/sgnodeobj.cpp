#include "sgnodeobj.h"

#include <stdexcept>

#include "core/ini.h"

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetPosition)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetPosition)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetRotation)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetRotation)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetSgNode)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetScale)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetScale)
{
    throw retruxx::logic_error("Not implemented");
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SgNodeObj)
        RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, SetPosition, "", "", "")
        RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, GetPosition, "", "", "")
        RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, SetRotation, "", "", "")
        RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, GetRotation, "", "", "")
        RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, SetSgNode, "", "", "")
        RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, SetScale, "", "", "")
        RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, GetScale, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgNodeObj);

    bool SgNodeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(this->m_engineModelName, xmlNode, "ModelFile");
            return 1;
        }
        return result;
    }

    SgNodeObjPrototypeInfo::SgNodeObjPrototypeInfo()
    {
    }

    CStr const& SgNodeObjPrototypeInfo::GetEngineModelName() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ai::Obj* SgNodeObjPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNodeObj::SgNodeObj(SgNodeObjPrototypeInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::SetRotation(Quaternion const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::Registration()
    {
        m_propertiesMap["Pos"] = 4;
        m_propertiesMap["Rot"] = 5;
        m_propertiesMap["ModelFile"] = 44;
        m_propertiesMap["NodeScale"] = 7;
    }

    bool SgNodeObj::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::SetSgNode(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::SetSgNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* SgNodeObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    Quaternion SgNodeObj::GetRotation() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgNodeObj::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNodeObjPrototypeInfo const* SgNodeObj::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::DeleteSgNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus SgNodeObj::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr SgNodeObj::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* SgNodeObj::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::SetPosition(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float SgNodeObj::GetScale() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector SgNodeObj::GetPosition() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::SetScale(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SgNodeObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SgNodeObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNodeObj::_InternalCreateVisualPart()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgNodeObj::~SgNodeObj()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* SgNodeObj::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* SgNodeObj::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

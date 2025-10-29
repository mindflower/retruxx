#include "sgnodeobj.h"

#include <stdexcept>

#include "core/ini.h"

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetPosition)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetPosition)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetRotation)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetRotation)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetSgNode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetScale)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetScale)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* SgNodeObjPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgNodeObj::SgNodeObj(SgNodeObjPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::SetRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::SetSgNode(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::SetSgNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* SgNodeObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    Quaternion SgNodeObj::GetRotation() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgNodeObj::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgNodeObjPrototypeInfo const* SgNodeObj::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::DeleteSgNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus SgNodeObj::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr SgNodeObj::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* SgNodeObj::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::SetPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float SgNodeObj::GetScale() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector SgNodeObj::GetPosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::SetScale(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool SgNodeObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool SgNodeObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgNodeObj::~SgNodeObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* SgNodeObj::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* SgNodeObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

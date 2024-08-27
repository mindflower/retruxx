#include "sgnodeobj.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetPosition)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetPosition)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetRotation)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetRotation)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetSgNode)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetScale)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetScale)
{
    throw std::logic_error("Not implemented");
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

    bool SgNodeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    SgNodeObjPrototypeInfo::SgNodeObjPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& SgNodeObjPrototypeInfo::GetEngineModelName() const
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* SgNodeObjPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    SgNodeObj::SgNodeObj(SgNodeObjPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::SetRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::SetSgNode(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::SetSgNode()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* SgNodeObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    Quaternion SgNodeObj::GetRotation() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgNodeObj::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    SgNodeObjPrototypeInfo const* SgNodeObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::DeleteSgNode()
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus SgNodeObj::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr SgNodeObj::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* SgNodeObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::SetPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    float SgNodeObj::GetScale() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector SgNodeObj::GetPosition() const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::SetScale(float)
    {
        throw std::logic_error("Not implemented");
    }

    bool SgNodeObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool SgNodeObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    void SgNodeObj::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }

    SgNodeObj::~SgNodeObj()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* SgNodeObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* SgNodeObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

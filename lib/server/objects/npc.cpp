#include "npc.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, GetSpokenCount)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, SetSpokenCount)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, HasDynamicQuests)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, HasNotTakenDynamicQuests)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Npc)
    RT_CLASS_EXPORT(Npc, m3d::METHOD, GetSpokenCount, "", "", "")
    RT_CLASS_EXPORT(Npc, m3d::METHOD, SetSpokenCount, "", "", "")
    RT_CLASS_EXPORT(Npc, m3d::METHOD, HasDynamicQuests, "", "", "")
    RT_CLASS_EXPORT(Npc, m3d::METHOD, HasNotTakenDynamicQuests, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Npc);

    NpcPrototypeInfo::NpcPrototypeInfo()
    {
        m_bIsUpdating = false;
    }

    Obj* NpcPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool NpcPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    bool Npc::HasDynamicQuests() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Npc::GetCfgNumber() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Npc::GetSkinNumber() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::SetCurrentNpc(Npc*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::SetSpokenCount(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NpcPrototypeInfo const* Npc::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Npc::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Npc::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Npc* Npc::GetCurrentNpc()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Npc::GetSpokenCount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Npc::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::SetHelloReplyNames(retruxx::vector<CStr, retruxx::allocator<CStr>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Npc::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    CStr const& Npc::GetModelName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Npc::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Npc::HasNotTakenDynamicQuests() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> const& Npc::GetHelloReplyNames() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::Registration()
    {
        m_propertiesMap["ModelName"] = 44;
        m_propertiesMap["skin"] = 45;
        m_propertiesMap["cfg"] = 46;
        m_propertiesMap["helloReplyNames"] = 47;
        m_propertiesMap["NpcType"] = 48;
    }

    eGObjPropertySaveStatus Npc::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Npc::NpcType Npc::GetNpcType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Npc::Npc(NpcPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Npc::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Npc::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Npc::~Npc()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Npc::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Npc::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Npc::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

#include "npc.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, GetSpokenCount)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, SetSpokenCount)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, HasDynamicQuests)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, HasNotTakenDynamicQuests)
{
    throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    Obj* NpcPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool NpcPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Npc::HasDynamicQuests() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Npc::GetCfgNumber() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Npc::GetSkinNumber() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::SetCurrentNpc(Npc*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::SetSpokenCount(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    NpcPrototypeInfo const* Npc::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Npc::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Npc::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Npc* Npc::GetCurrentNpc()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Npc::GetSpokenCount() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Npc::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::SetHelloReplyNames(retruxx::vector<CStr, retruxx::allocator<CStr>> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Npc::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    CStr const& Npc::GetModelName() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Npc::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Npc::HasNotTakenDynamicQuests() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> const& Npc::GetHelloReplyNames() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    Npc::NpcType Npc::GetNpcType() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Npc::Npc(NpcPrototypeInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Npc::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Npc::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Npc::~Npc()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Npc::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Npc::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Npc::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

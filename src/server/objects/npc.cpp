#include "npc.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, GetSpokenCount)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, SetSpokenCount)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, HasDynamicQuests)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, HasNotTakenDynamicQuests)
{
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    Obj* NpcPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool NpcPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Npc::HasDynamicQuests() const
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Npc::GetCfgNumber() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Npc::GetSkinNumber() const
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::SetCurrentNpc(Npc*)
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::SetSpokenCount(int)
    {
        throw std::logic_error("Not implemented");
    }

    NpcPrototypeInfo const* Npc::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Npc::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Npc::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    Npc* Npc::GetCurrentNpc()
    {
        throw std::logic_error("Not implemented");
    }

    int Npc::GetSpokenCount() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Npc::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::SetHelloReplyNames(std::vector<CStr, std::allocator<CStr>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Npc::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    CStr const& Npc::GetModelName() const
    {
        throw std::logic_error("Not implemented");
    }

    int Npc::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Npc::HasNotTakenDynamicQuests() const
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<CStr, std::allocator<CStr>> const& Npc::GetHelloReplyNames() const
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Npc::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    Npc::NpcType Npc::GetNpcType() const
    {
        throw std::logic_error("Not implemented");
    }

    Npc::Npc(NpcPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool Npc::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Npc::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    Npc::~Npc()
    {
        throw std::logic_error("Not implemented");
    }

    void Npc::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Npc::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Npc::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

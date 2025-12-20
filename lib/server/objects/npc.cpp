#include "npc.h"

#include <stdexcept>
#include "base/globalproperties.h"
#include <core/ini.h>
#include <core/aiparam.h>
#include <server/utils.h>

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
    namespace
    {
        struct
        {
            CStr str;
            Npc::NpcType type;
        } l_str2Type[] = {{"BARMAN", Npc::NpcType::NPC_BARMAN}, {"CLIENT", Npc::NpcType::NPC_CLIENT}};

        Npc::NpcType Str2NpcType(CStr const& npcTypeStr)
        {
            for (auto const& [str, type] : l_str2Type)
            {
                if (str == npcTypeStr)
                {
                    return type;
                }
            }
            return Npc::NpcType::NPC_CLIENT;
        }
    }  // namespace

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
        return new Npc(*this);
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

    void Npc::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_spokenCount, xmlNode, "SpokenCount");
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
        return RT_CLASS_LOCAL(Npc);
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

    bool Npc::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        int res = 1;
        switch (propertyId)
        {
        case 44:
            m_ModelName = newValue.GetAsStr();
            break;
        case 45:
            m_SkinNumber = newValue.GetAsID();
            break;
        case 46:
            m_CfgNumber = newValue.GetAsID();
            break;
        case 47:
            ai::StrToStringVector(newValue.GetAsStr(), m_helloReplyNames);
            break;
        case 48:
            m_npcType = Str2NpcType(newValue.GetAsStr());
            break;
        default:
            res = Obj::SetPropertyById(propertyId, newValue);
            break;
        }
        return res;
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

    int Npc::GetPropertyId(char const* propName) const
    {
        auto it = Npc::m_propertiesMap.find(propName);
        if (it != Npc::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::Obj::GetPropertyId(propName);
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
        return m_npcType;
    }

    Npc::Npc(NpcPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_npcType = NPC_CLIENT;
        m_SkinNumber = 0;
        m_CfgNumber = 0;
        m_spokenCount = 0;
        m_ModelName = ai::theGlobProp.m_barmenModelName;
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

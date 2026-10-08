#include "npc.h"

#include <stdexcept>
#include "base/globalproperties.h"
#include <core/ini.h>
#include <core/aiparam.h>
#include <server/utils.h>
#include <core/kernel.h>
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "dynamicquest.h"
#include "town.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, GetSpokenCount)
{
    auto npc = (ai::Npc*)context->asObject(0, "Npc");
    context->pushInt(npc->GetSpokenCount());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, SetSpokenCount)
{
    auto npc = (ai::Npc*)context->asObject(0, "Npc");
    npc->SetSpokenCount(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, HasDynamicQuests)
{
    auto npc = (ai::Npc*)context->asObject(0, "Npc");
    context->pushBool(npc->HasDynamicQuests());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Npc, HasNotTakenDynamicQuests)
{
    auto npc = (ai::Npc*)context->asObject(0, "Npc");
    context->pushBool(npc->HasNotTakenDynamicQuests());
    return 1;
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

        CStr NpcType2Str(Npc::NpcType npcType)
        {
            // RVA 0x7051A0 - an unknown type comes back as an empty string.
            for (auto const& [str, type] : l_str2Type)
            {
                if (type == npcType)
                {
                    return str;
                }
            }
            return CStr();
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
        // RVA 0x7051F0 - only barmen hand out dynamic quests, and they come from the nearest town
        // up the hierarchy.
        if (m_npcType != NPC_BARMAN)
        {
            return false;
        }

        Obj* town = GetParent();
        while (town && !town->IsKindOf(&Town::m_classTown))
        {
            town = town->GetParent();
        }
        if (!town)
        {
            return false;
        }
        return !static_cast<Town*>(town)->GetDynamicQuestIds().empty();
    }

    void Npc::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
    }

    unsigned Npc::GetCfgNumber() const
    {
        // RVA 0x4A05F0
        return m_CfgNumber;
    }

    unsigned Npc::GetSkinNumber() const
    {
        // RVA 0x4A05E0
        return m_SkinNumber;
    }

    void Npc::SetCurrentNpc(Npc* npc)
    {
        M3D_KERNEL->UnRegisterGlobal("g_CurrentNpc");
        theCurrentNpc = npc;
        if (npc)
        {
            M3D_KERNEL->RegisterGlobal(npc, "g_CurrentNpc");
        }
    }

    void Npc::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_spokenCount, xmlNode, "SpokenCount");
    }

    void Npc::SetSpokenCount(int spokenCount)
    {
        // RVA 0x704B30
        m_spokenCount = spokenCount;
    }

    NpcPrototypeInfo const* Npc::GetPrototypeInfo() const
    {
        // RVA 0x7052B0 - NOTE: the prototype is cast without a type check.
        return static_cast<NpcPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* Npc::GetClass() const
    {
        return RT_CLASS_LOCAL(Npc);
    }

    CStr Npc::GetPropertyName(int id) const
    {
        // RVA 0x705650 - a linear search, since the map is keyed by name.
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    void Npc::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x7055D0
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    void Npc::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x705550
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    Npc* Npc::GetCurrentNpc()
    {
        // RVA 0x704AC0
        return theCurrentNpc;
    }

    int Npc::GetSpokenCount() const
    {
        return m_spokenCount;
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

    void Npc::SetHelloReplyNames(retruxx::vector<CStr, retruxx::allocator<CStr>> const& replyNames)
    {
        // NOTE: declared in the PDB but never instantiated in the shipped binary, so this body does
        // not come from it.
        m_helloReplyNames = replyNames;
    }

    void Npc::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x704B50
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
    }

    m3d::Class* Npc::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    CStr const& Npc::GetModelName() const
    {
        // RVA 0x4A05D0
        return m_ModelName;
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
        // RVA 0x7052E0 - NOTE: unlike HasDynamicQuests, this keeps climbing past the first town and
        // looks at every town up the hierarchy.
        if (m_npcType != NPC_BARMAN)
        {
            return false;
        }

        for (Obj* parent = GetParent(); parent; parent = parent->GetParent())
        {
            if (!parent->IsKindOf(&Town::m_classTown))
            {
                continue;
            }

            for (int questId : static_cast<Town*>(parent)->GetDynamicQuestIds())
            {
                Obj* quest = theObjects->GetEntityByObjId(questId);
                if (quest && quest->IsKindOf(&DynamicQuest::m_classDynamicQuest) &&
                    static_cast<DynamicQuest*>(quest)->GetQuestStatus() == DynamicQuest::STATUS_NOT_TAKEN)
                {
                    return true;
                }
            }
        }
        return false;
    }

    retruxx::vector<CStr> const& Npc::GetHelloReplyNames() const
    {
        return m_helloReplyNames;
    }

    void Npc::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x704C40
        Obj::SaveToXML(xmlFile, xmlNode);
        xmlNode->SetAttribute("SpokenCount", CStr(m_spokenCount).c_str());
    }

    void Npc::Registration()
    {
        m_propertiesMap["ModelName"] = 44;
        m_propertiesMap["skin"] = 45;
        m_propertiesMap["cfg"] = 46;
        m_propertiesMap["helloReplyNames"] = 47;
        m_propertiesMap["NpcType"] = 48;
    }

    eGObjPropertySaveStatus Npc::GetPropertySaveStatus(int id) const
    {
        // RVA 0x705510
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertySaveStatus(id);
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

    void Npc::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x705740 - the default save status is not stored.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool Npc::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x705940 - NOTE: skin and cfg are stored as numbers but read back as strings.
        switch (propertyId)
        {
        case 44:
            retVal = m_ModelName;
            return true;
        case 45:
            retVal = CStr(m_SkinNumber);
            return true;
        case 46:
            retVal = CStr(m_CfgNumber);
            return true;
        case 47:
            retVal = ai::StringVectorToStr(m_helloReplyNames);
            return true;
        case 48:
            retVal = NpcType2Str(m_npcType);
            return true;
        default:
            return Obj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    bool Npc::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x705A40 - NOTE: the default model is "mask1", not the barman model the constructor
        // assigns.
        switch (propertyId)
        {
        case 44:
            retVal = CStr("mask1");
            return true;
        case 45:
            retVal = CStr("0");
            return true;
        case 46:
            retVal = CStr("0");
            return true;
        case 47:
            retVal = CStr();
            return true;
        case 48:
            retVal = NpcType2Str(NPC_CLIENT);
            return true;
        default:
            return Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    Npc::~Npc() = default;

    void Npc::_InternalPostLoad()
    {
    }

    m3d::Object* Npc::CreateObject()
    {
        // RVA 0x704FE0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Npc::Clone()
    {
        // RVA 0x704E20
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

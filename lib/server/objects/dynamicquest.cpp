#include "dynamicquest.h"
#include <stdexcept>

#include "base/prototypemanager.h"
#include "core/ini.h"
#include "player.h"
#include "town.h"
#include "server/processmanager.h"

#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <m3dapp.h>

RT_CLASS_EXPORT_METHOD_DEFINE(DynamicQuest, Take)
{
    auto* quest = (ai::DynamicQuest*)context->asObject(0, "DynamicQuest");
    quest->Take();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DynamicQuest, GetReward)
{
    auto* quest = (ai::DynamicQuest*)context->asObject(0, "DynamicQuest");
    context->pushInt(quest->GetReward());
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuest)
        RT_CLASS_EXPORT(DynamicQuest, m3d::METHOD, Take, "", "", "")
        RT_CLASS_EXPORT(DynamicQuest, m3d::METHOD, GetReward, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuest);

    namespace
    {
        struct QuestStatusToStr
        {
            CStr m_str;
            DynamicQuest::QuestStatus m_status;
        };

        QuestStatusToStr const questStatusToStrMap[] = {
            {CStr("NOT_TAKEN"), DynamicQuest::STATUS_NOT_TAKEN},
            {CStr("PROCESSING"), DynamicQuest::STATUS_PROCESSING},
            {CStr("COMPLETE"), DynamicQuest::STATUS_COMPLETE},
            {CStr("FAILED"), DynamicQuest::STATUS_FAILED},
            {CStr("FORGOTTEN"), DynamicQuest::STATUS_FORGOTTEN},
        };

        DynamicQuest::QuestStatus Str2QuestStatus(CStr const& str)
        {
            // RVA 0x706220 - an unreadable status is treated as a quest nobody remembers any more.
            for (auto const& entry : questStatusToStrMap)
            {
                if (entry.m_str == str)
                {
                    return entry.m_status;
                }
            }
            M3D_LOG_ERR("Error: undefined dynamic quest status: " + str);
            return DynamicQuest::STATUS_FORGOTTEN;
        }

        CStr QuestStatus2Str(DynamicQuest::QuestStatus questStatus)
        {
            // RVA 0x706340
            for (auto const& entry : questStatusToStrMap)
            {
                if (entry.m_status == questStatus)
                {
                    return entry.m_str;
                }
            }
            // NOTE: an unknown status stringifies as "0", not as the empty string.
            return CStr(0);
        }
    }  // namespace

    DynamicQuestPrototypeInfo::DynamicQuestPrototypeInfo()
    {
        m_minReward = 0;
    }

    bool DynamicQuestPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeIntAttrib(m_minReward, xmlNode, "MinReward");
            return 1;
        }
        return result;
    }

    int DynamicQuest::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x706AE0
        auto const it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertyId(PropertyName);
    }

    bool DynamicQuest::CanChildBeAdded(m3d::Class*) const
    {
        // RVA 0x705D30 - a dynamic quest never has children of its own.
        return false;
    }

    void DynamicQuest::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x705DB0
        Obj::LoadRuntimeValues(xmlFile, xmlNode);

        ref_ptr takeGameTimeNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(takeGameTimeNode, "TakeGameTime");
        if (!takeGameTimeNode->IsEmpty())
        {
            m_takeGameTime.LoadFromXML(xmlFile, takeGameTimeNode);
        }

        if (!xmlNode->IsEmpty())
        {
            char const* const hirerObjId = xmlNode->GetAttribute("HirerObjId");
            if (hirerObjId)
            {
                m_hirerObjId = atoi(hirerObjId);
            }
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const targetObjId = xmlNode->GetAttribute("TargetObjId");
            if (targetObjId)
            {
                m_targetObjId = atoi(targetObjId);
            }
        }
    }

    DynamicQuest::DynamicQuest(DynamicQuestPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        // RVA 0x705CB0
        m_hirerObjId = -1;
        m_targetObjId = -1;
        m_questStatus = STATUS_NOT_TAKEN;
        m_reward = 0;
        m_fadingMsgIdOnComplete = FM_DYNAMIC_QUEST_COMPLETED;
        m_bShowMessageForAddMoney = true;
    }

    DynamicQuestPrototypeInfo const* DynamicQuest::GetPrototypeInfo() const
    {
        // RVA 0x706B50
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), DynamicQuestPrototypeInfo const);
    }

    CStr DynamicQuest::GetPropertyName(int id) const
    {
        // RVA 0x706D20
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    GameTime const& DynamicQuest::GetTakeGameTime() const
    {
        // RVA 0x519C50
        return m_takeGameTime;
    }

    CStr const& DynamicQuest::GetTargetName() const
    {
        // RVA 0x571960
        return m_targetName;
    }

    int DynamicQuest::GetHirerObjId() const
    {
        // RVA 0x6EA4B0
        return m_hirerObjId;
    }

    void DynamicQuest::Take()
    {
        // RVA 0x707CE0
        _SetStatus(STATUS_PROCESSING);
    }

    void DynamicQuest::PassToAnotherMap()
    {
        // RVA 0x707C10 - a quest still in progress is left behind when the player changes map.
        if (m_questStatus == STATUS_PROCESSING)
        {
            _SetStatus(STATUS_FORGOTTEN);
        }
    }

    m3d::Class* DynamicQuest::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    eGObjPropertySaveStatus DynamicQuest::GetPropertySaveStatus(int id) const
    {
        // RVA 0x706BE0
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertySaveStatus(id);
    }

    void DynamicQuest::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x705BD0
        Obj::LoadFromXML(xmlFile, xmlNode);
    }

    int DynamicQuest::GetReward() const
    {
        // RVA 0x581F80
        return m_reward;
    }

    CStr const& DynamicQuest::GetHirerName() const
    {
        // RVA 0x571950
        return m_hirerName;
    }

    bool DynamicQuest::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x706650
        switch (propertyId)
        {
            case 57:
                m_targetName = newValue.GetAsStr();
                return true;
            case 58:
                m_hirerName = newValue.GetAsStr();
                return true;
            case 59:
                m_questStatus = Str2QuestStatus(newValue.GetAsStr());
                return true;
            case 60:
                m_reward = newValue.GetAsID();
                return true;
            default:
                return Obj::SetPropertyById(propertyId, newValue);
        }
    }

    int DynamicQuest::OnEvent(Event const& evn)
    {
        // RVA 0x706D90
        int const result = Obj::OnEvent(evn);
        if (evn.m_eventId == GE_RELATION_CHANGED)
        {
            _OnRelationChanged(evn);
            return 1;
        }
        return result;
    }

    void DynamicQuest::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x706390
        Obj::SaveRuntimeValues(xmlFile, xmlNode);

        ref_ptr takeGameTimeNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "TakeGameTime");
        xmlNode->AddChild(takeGameTimeNode);
        m_takeGameTime.SaveToXML(xmlFile, takeGameTimeNode);

        xmlNode->SetAttribute("HirerObjId", CStr(m_hirerObjId).c_str());
        xmlNode->SetAttribute("TargetObjId", CStr(m_targetObjId).c_str());
    }

    void DynamicQuest::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x705BE0
        Obj::SaveToXML(xmlFile, xmlNode);
    }

    void DynamicQuest::UpdateBeforeShowInConversation()
    {
        // RVA 0x705BF0 - only the quests whose price depends on the player override this.
    }

    void DynamicQuest::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x706CA0
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    m3d::Class* DynamicQuest::GetClass() const
    {
        // RVA 0x705BA0
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    int DynamicQuest::GetTargetObjId() const
    {
        // RVA 0x550A70
        return m_targetObjId;
    }

    void DynamicQuest::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x706C20
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    void DynamicQuest::Registration()
    {
        m_propertiesMap["PrototypeId"] = 2;
        m_propertiesSaveStatesMap[2] = SAVE_PROP_NEVER;
        m_propertiesMap["Belong"] = 0;
        m_propertiesSaveStatesMap[0] = SAVE_PROP_NEVER;
        m_propertiesMap["targetName"] = 57;
        m_propertiesMap["hirerName"] = 58;
        m_propertiesMap["status"] = 59;
        m_propertiesMap["reward"] = 60;
    }

    DynamicQuest::QuestStatus DynamicQuest::GetQuestStatus() const
    {
        // RVA 0x497720
        return m_questStatus;
    }

    DynamicQuest::~DynamicQuest()
    {
        // RVA 0x705D40 - the members clean themselves up.
    }

    void DynamicQuest::_OnHirerBecamesEnemyWithPlayer()
    {
        // RVA 0x707C30 - a quest in progress is failed when the person who set it turns hostile.
        if (m_questStatus == STATUS_PROCESSING)
        {
            M3D_APP->EnqueueMessage(
                66564, FM_DYNAMIC_QUEST_FAILED_BECAUSE_HIRER_BECAMES_ENEMY, GetId(), 0, 0, {}, {});
            _SetStatus(STATUS_FAILED);
        }
    }

    bool DynamicQuest::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x707A40
        switch (propertyId)
        {
            case 57:
                retVal = m_targetName;
                return true;
            case 58:
                retVal = m_hirerName;
                return true;
            case 59:
                retVal = QuestStatus2Str(m_questStatus);
                return true;
            case 60:
                retVal = m_reward;
                return true;
            default:
                return Obj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    void DynamicQuest::_SetStatus(QuestStatus newStatus)
    {
        // RVA 0x707140 - the one place a quest's status changes. Only a quest that has not been settled
        // yet can move on, so completing or failing one is final.
        QuestStatus const oldStatus = m_questStatus;
        if (oldStatus == newStatus || oldStatus > STATUS_PROCESSING)
        {
            return;
        }
        m_questStatus = newStatus;
        M3D_LOG_INFO("Dynamic quest id " + CStr(GetId()) + " status changed to " + QuestStatus2Str(m_questStatus));

        switch (m_questStatus)
        {
            case STATUS_PROCESSING:
                _OnTake();
                if (thePlayer)
                {
                    theProcessManager->PostMessageA(
                        GE_DYNAMIC_QUEST_TAKEN, thePlayer->GetId(), GetId(), 0.0, {}, {}, 0);
                }
                break;

            case STATUS_COMPLETE:
            {
                if (!thePlayer)
                {
                    break;
                }
                thePlayer->AddMoney(m_reward);
                m3d::AIParam param(static_cast<int>(GE_RELATION_CHANGED));
                theProcessManager->PostMessageA(
                    GE_UNSUBSCRIBE, thePlayer->GetId(), GetId(), 0.0, param, {}, 1);
                theProcessManager->PostMessageA(
                    GE_DYNAMIC_QUEST_COMPLETE, thePlayer->GetId(), GetId(), 0.0, {}, {}, 0);

                if (m_bShowMessageForAddMoney)
                {
                    if (m_reward > 0)
                    {
                        M3D_APP->ImmediateMessage(66563, FM_PLAYER_ADD_MONEY, m_reward, 0, 0, {}, {});
                    }
                    else if (m_reward < 0)
                    {
                        M3D_APP->ImmediateMessage(66563, FM_PLAYER_GIVE_MONEY, -m_reward, 0, 0, {}, {});
                    }
                }
                M3D_APP->ImmediateMessage(66564, m_fadingMsgIdOnComplete, GetId(), 0, 0, {}, {});
                break;
            }

            case STATUS_FAILED:
            {
                if (!thePlayer)
                {
                    break;
                }
                m3d::AIParam param(static_cast<int>(GE_RELATION_CHANGED));
                theProcessManager->PostMessageA(
                    GE_UNSUBSCRIBE, thePlayer->GetId(), GetId(), 0.0, param, {}, 1);
                if (oldStatus == STATUS_PROCESSING)
                {
                    theProcessManager->PostMessageA(
                        GE_DYNAMIC_QUEST_FAILED, thePlayer->GetId(), GetId(), 0.0, {}, {}, 0);
                }
                break;
            }

            case STATUS_FORGOTTEN:
            {
                if (thePlayer)
                {
                    m3d::AIParam param(static_cast<int>(GE_RELATION_CHANGED));
                    theProcessManager->PostMessageA(
                        GE_UNSUBSCRIBE, thePlayer->GetId(), GetId(), 0.0, param, {}, 1);
                }
                Remove();
                if (thePlayer && oldStatus == STATUS_PROCESSING)
                {
                    theProcessManager->PostMessageA(
                        GE_DYNAMIC_QUEST_FORGOTTEN, thePlayer->GetId(), GetId(), 0.0, {}, {}, 0);
                }
                M3D_LOG_INFO("Dynamic quest id " + CStr(GetId()) + " removed");
                break;
            }

            default:
                break;
        }

        // Whatever happened, the town that set the quest may want to put up a new one.
        Obj* const hirer = theObjects->GetEntityByObjId(m_hirerObjId);
        if (hirer && hirer->IsKindOf(&ai::Town::m_classTown))
        {
            ((Town*)hirer)->GenerateNewDynamicQuestIfNeeded();
        }
        M3D_APP->ImmediateMessage(66547, GetId(), m_hirerObjId, 0, 0, {}, {});
    }

    void DynamicQuest::_OnCreate()
    {
        // RVA 0x706490 - every quest type has to say what it listens to.
        SYS_ERROR("!\"pure virtual\"");
    }

    void DynamicQuest::_OnTake()
    {
        // RVA 0x707070 - remember when the quest was taken and start watching the player's relations, so
        // that the quest can be failed if its hirer turns hostile.
        m_takeGameTime = theObjects->getGameTime();
        if (thePlayer)
        {
            m3d::AIParam param(static_cast<int>(GE_RELATION_CHANGED));
            theProcessManager->PostMessageA(GE_SUBSCRIBE, thePlayer->GetId(), GetId(), 0.0, param, {}, 1);
        }
    }

    void DynamicQuest::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x706DC0 - SAVE_PROP_NORMAL is the default and is not recorded.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    void DynamicQuest::_InternalPostLoad()
    {
        // RVA 0x706FD0
        _OnCreate();
        M3D_APP->EnqueueMessage(66547, GetId(), m_hirerObjId, 0, 0, {}, {});
    }

    bool DynamicQuest::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x707B00
        switch (propertyId)
        {
            case 57:
                retVal = CStr("");
                return true;
            case 58:
                retVal = CStr("");
                return true;
            case 59:
                retVal = QuestStatus2Str(STATUS_NOT_TAKEN);
                return true;
            case 60:
                retVal = 0;
                return true;
            default:
                // NOTE: the fallback goes to _GetPropertyInternal, not to the base class' default.
                return Obj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    m3d::Object* DynamicQuest::Clone()
    {
        // RVA 0x705EA0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void DynamicQuest::_OnObjectEntersLocation(Event const&)
    {
        // RVA 0x706920 - each quest type handles arrivals itself; this one is never reached.
        SYS_ERROR("!\"obsolete\"");
    }

    m3d::Object* DynamicQuest::CreateObject()
    {
        // RVA 0x706060
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void DynamicQuest::_OnObjectDie(Event const&)
    {
        // RVA 0x706760 - each quest type handles deaths itself; this one is never reached.
        SYS_ERROR("!\"obsolete\"");
    }

    void DynamicQuest::_OnRelationChanged(Event const& evn)
    {
        // RVA 0x706B80 - only a change involving the hirer's own clan matters.
        Obj* const hirer = theObjects->GetEntityByObjId(m_hirerObjId);
        if (hirer && evn.m_param1.GetAsID() == hirer->GetBelong())
        {
            _OnHirerBecamesEnemyWithPlayer();
        }
    }
}

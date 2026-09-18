#include "queststate.h"

#include <stdexcept>

#include "quest.h"
#include "core/log.h"
#include "objects/base/objcontainer.h"

#include <algorithm>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/ref_ptr.h>
#include <m3dapp.h>
#include <script/scriptserver.h>

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, TakeQuest)
{
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    auto* quest = context->asString(1);
    manager->TakeQuest(quest);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CompleteQuest)
{
    // RVA 0x6A0C50
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    manager->CompleteQuest(context->asString(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CompleteQuestIfTaken)
{
    // RVA 0x6A0C80
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    manager->CompleteQuestIfTaken(context->asString(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, FailQuest)
{
    // RVA 0x6A0CB0
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    manager->FailQuest(context->asString(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, FailQuestIfTaken)
{
    // RVA 0x6A0CE0
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    manager->FailQuestIfTaken(context->asString(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, IsQuestTaken)
{
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    auto* quest = context->asString(1);
    bool res = manager->IsQuestTaken(quest);
    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, IsQuestComplete)
{
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    auto* quest = context->asString(1);
    bool res = manager->IsQuestComplete(quest);
    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, IsQuestFailed)
{
    // RVA 0x69D4D0
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    context->pushBool(manager->IsQuestFailed(context->asString(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CanQuestBeGiven)
{
    // RVA 0x69D510
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    context->pushBool(manager->CanQuestBeGiven(context->asString(1)));
    return 1;
}


namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(QuestStateManager)
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, TakeQuest, "", "", "")
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, CompleteQuest, "", "", "")
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, CompleteQuestIfTaken, "", "", "")
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, FailQuest, "", "", "")
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, FailQuestIfTaken, "", "", "")
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, IsQuestTaken, "", "", "")
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, IsQuestComplete, "", "", "")
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, IsQuestFailed, "", "", "")
        RT_CLASS_EXPORT(QuestStateManager, m3d::METHOD, CanQuestBeGiven, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(QuestStateManager);

    namespace
    {
        // RVA 0x69A8D0
        CStr CompleteStatus2Str(QuestState::CompleteStatus completeStatus)
        {
            switch (completeStatus)
            {
            case QuestState::NOT_COMPLETE:
                return CStr("not_complete");
            case QuestState::COMPLETE:
                return CStr("complete");
            case QuestState::FAILED:
                return CStr("failed");
            default:
                // NOTE: the status itself is not appended to the message.
                M3D_LOG_ERR("Error: undefined quest complete status: ");
                return CStr("not_complete");
            }
        }

        // RVA 0x69B2F0
        QuestState::CompleteStatus Str2CompleteStatus(CStr const& str)
        {
            if (str == CStr("not_complete"))
            {
                return QuestState::NOT_COMPLETE;
            }
            if (str == CStr("complete"))
            {
                return QuestState::COMPLETE;
            }
            if (str == CStr("failed"))
            {
                return QuestState::FAILED;
            }
            M3D_LOG_ERR("Error: undefined quest complete status: " + str);
            return QuestState::NOT_COMPLETE;
        }
    }  // namespace

    int QuestState::GetQuestId() const
    {
        return m_questId;
    }

    long long QuestState::getTakeTime() const
    {
        // RVA 0x69B120
        M3D_ASSERT(m_bIsTaken);
        return m_TakeTime;
    }

    QuestState::~QuestState()
    {
        // Nothing to release.
    }

    QuestState::QuestState(int questId) :
        m_questId(questId),
        m_completeStatus(NOT_COMPLETE),
        m_bIsTaken(false),
        m_bCanBeGiven(false),
        m_TakeTime(0)
    {
        // The quest states are built by QuestStateManager::LoadFromXml.
    }

    bool QuestState::PostLoad()
    {
        // RVA 0x69E550 - tells the interface about a quest that was already taken or finished when the game was saved.
        if (m_bIsTaken)
        {
            M3D_APP->EnqueueMessage(66538, m_questId, 0, 0, 0, CStr(), m3d::AIParam());
        }
        if (m_completeStatus != NOT_COMPLETE)
        {
            M3D_APP->EnqueueMessage(66539, m_questId, 0, 0, 0, CStr(), m3d::AIParam());
        }
        return true;
    }

    Quest* QuestState::GetQuest() const
    {
        // RVA 0x699FC0
        return theQuestManager->GetQuestById(m_questId);
    }

    void QuestState::RecalcState()
    {
        // RVA 0x69EFE0 - works the quest's status out from its parent and its subquests: an AND quest needs them
        // all, an OR quest any one of them, and an XOR quest exactly one.
        if (m_completeStatus != NOT_COMPLETE)
        {
            return;
        }

        Quest const* const quest = theQuestManager->GetQuestById(m_questId);
        int const parentQuestId = quest->GetParentId();
        if (parentQuestId >= 0)
        {
            QuestState const* const parent = theQuestStateManager->GetQuestStateById(parentQuestId);
            if (parent)
            {
                // A failed parent fails its subquests, and so does a completed XOR parent: another subquest won it.
                if (parent->GetCompleteStatus() == FAILED ||
                    (theQuestManager->GetQuestById(parent->GetQuestId())->GetSubQuestCondition() == Quest::SUBQUEST_XOR &&
                     parent->GetCompleteStatus() == COMPLETE))
                {
                    m_completeStatus = FAILED;
                }
            }
        }

        if (m_completeStatus == NOT_COMPLETE && !quest->GetSubQuestIds().empty())
        {
            bool const bCheckAllSubQuests = quest->bCheckAllSubQuests();
            bool bSomeoneNotComplete = false;
            switch (quest->GetSubQuestCondition())
            {
            case Quest::SUBQUEST_AND:
            {
                bool bAllComplete = true;
                bool bSomeoneFailed = false;
                for (int const subQuestId : quest->GetSubQuestIds())
                {
                    CompleteStatus const status = theQuestStateManager->GetQuestStateById(subQuestId)->GetCompleteStatus();
                    bAllComplete &= status == COMPLETE;
                    bSomeoneFailed |= status == FAILED;
                    bSomeoneNotComplete |= status == NOT_COMPLETE;
                }
                if (!bCheckAllSubQuests || !bSomeoneNotComplete)
                {
                    if (bAllComplete)
                    {
                        m_completeStatus = COMPLETE;
                    }
                    else if (bSomeoneFailed)
                    {
                        m_completeStatus = FAILED;
                    }
                }
                break;
            }

            case Quest::SUBQUEST_OR:
            {
                bool bAllFailed = true;
                bool bSomeoneComplete = false;
                for (int const subQuestId : quest->GetSubQuestIds())
                {
                    CompleteStatus const status = theQuestStateManager->GetQuestStateById(subQuestId)->GetCompleteStatus();
                    bAllFailed &= status == FAILED;
                    bSomeoneComplete |= status == COMPLETE;
                    bSomeoneNotComplete |= status == NOT_COMPLETE;
                }
                if (bCheckAllSubQuests && bSomeoneNotComplete)
                {
                    break;
                }
                if (bAllFailed)
                {
                    m_completeStatus = FAILED;
                }
                else if (bSomeoneComplete)
                {
                    m_completeStatus = COMPLETE;
                }
                break;
            }

            case Quest::SUBQUEST_XOR:
            {
                bool bAllFailed = true;
                bool bSomeoneComplete = false;
                for (int const subQuestId : quest->GetSubQuestIds())
                {
                    CompleteStatus const status = theQuestStateManager->GetQuestStateById(subQuestId)->GetCompleteStatus();
                    bAllFailed &= status == FAILED;
                    if (status == COMPLETE)
                    {
                        if (bSomeoneComplete)
                        {
                            M3D_LOG_ERR(
                                "Error in quest runtime: Two subquests are complete for XOR quest '" + quest->GetName() +
                                CStr("'"));
                        }
                        bSomeoneComplete = true;
                    }
                }
                if (bAllFailed)
                {
                    m_completeStatus = FAILED;
                }
                else if (bSomeoneComplete)
                {
                    m_completeStatus = COMPLETE;
                }
                break;
            }

            default:
                break;
            }
        }

        if (m_completeStatus != NOT_COMPLETE)
        {
            theQuestStateManager->OnQuestCompleteStatusChanged(m_questId);
        }
    }

    bool QuestState::bCanBeGiven() const
    {
        // RVA 0x699FB0
        return m_bCanBeGiven;
    }

    void QuestState::OnPlayerPassToMap(CStr const& targetMapName)
    {
        // RVA 0x6A0D10 - a taken quest that names the maps it may be done on fails when the player leaves them.
        Quest const* const quest = theQuestManager->GetQuestById(m_questId);
        if (quest->getActionLevels().empty() || !m_bIsTaken)
        {
            return;
        }
        auto const& levels = quest->getActionLevels();
        if (std::find(levels.begin(), levels.end(), targetMapName) == levels.end())
        {
            theQuestStateManager->FailQuest(theQuestManager->GetQuestById(m_questId)->GetName().c_str());
        }
    }

    bool QuestState::Fail()
    {
        // RVA 0x69F6E0
        return _SetCompleteStatusUnsafe(FAILED);
    }

    bool QuestState::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x69B3D0
        Quest const* const quest = theQuestManager->GetQuestById(m_questId);
        M3D_ASSERT(quest);
        xmlNode->SetAttribute("Name", quest->GetName().c_str());
        xmlNode->SetAttribute("CanBeGiven", CStr(static_cast<int>(m_bCanBeGiven)).c_str());
        xmlNode->SetAttribute("IsTaken", CStr(static_cast<int>(m_bIsTaken)).c_str());
        if (m_bIsTaken)
        {
            xmlNode->SetAttribute("TakeTime", CStr(m_TakeTime).c_str());
        }
        xmlNode->SetAttribute("CompleteStatus", CompleteStatus2Str(m_completeStatus).c_str());
        return true;
    }

    QuestState::CompleteStatus QuestState::GetCompleteStatus() const
    {
        return m_completeStatus;
    }

    bool QuestState::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x69B690
        CStr questName;
        m3d::SafeStrAttrib(questName, xmlNode, "Name");
        m_questId = theQuestManager->GetQuestIdByName(questName);
        if (m_questId == -1)
        {
            M3D_LOG_ERR("Error loading quest info: the quest '" + questName + CStr("' does not exist."));
            return false;
        }
        m3d::SafeBoolAttrib(m_bCanBeGiven, xmlNode, "CanBeGiven");
        m3d::SafeBoolAttrib(m_bIsTaken, xmlNode, "IsTaken");
        if (m_bIsTaken)
        {
            m3d::SafeInt64Attrib(m_TakeTime, xmlNode, "TakeTime");
        }
        CStr strCompleteStatus;
        m3d::SafeStrAttrib(strCompleteStatus, xmlNode, "CompleteStatus");
        m_completeStatus = Str2CompleteStatus(strCompleteStatus);
        return true;
    }

    void QuestState::RecalcCanBeGiven()
    {
        // RVA 0x69F2C0 - a quest can be given once its preceding quests are in the state its condition asks for;
        // one with no preceders can always be given.
        Quest const* const quest = theQuestManager->GetQuestById(m_questId);
        Quest::ConditionToGive const& condition = quest->GetConditionToGive();
        auto const& precedingQuestIds = theQuestManager->GetQuestById(m_questId)->GetPrecedingQuestIds();
        bool const oldCanBeGiven = m_bCanBeGiven;

        if (precedingQuestIds.empty())
        {
            m_bCanBeGiven = true;
        }
        else
        {
            bool canBeGiven = condition.m_operator == Quest::ConditionToGive::PRECEDERS_AND;
            for (int const precedingQuestId : precedingQuestIds)
            {
                QuestState const* const preceder = theQuestStateManager->GetQuestStateById(precedingQuestId);
                bool held = false;
                switch (condition.m_type)
                {
                case Quest::ConditionToGive::PRECEDERS_COMPLETE:
                    held = preceder->GetCompleteStatus() == COMPLETE;
                    break;
                case Quest::ConditionToGive::PRECEDERS_FAILED:
                    held = preceder->GetCompleteStatus() == FAILED;
                    break;
                case Quest::ConditionToGive::PRECEDERS_TAKEN:
                    held = preceder->bIsTaken();
                    break;
                }
                if (condition.m_operator == Quest::ConditionToGive::PRECEDERS_AND)
                {
                    canBeGiven &= held;
                }
                else
                {
                    canBeGiven |= held;
                }
            }
            m_bCanBeGiven = canBeGiven;
        }

        if (m_bCanBeGiven && !oldCanBeGiven)
        {
            theQuestStateManager->OnQuestCanBeGiven(m_questId);
            if (theQuestManager->GetQuestById(m_questId)->bSubscribeAutomatic())
            {
                Take();
            }
        }
    }

    bool QuestState::bIsTaken() const
    {
        return m_bIsTaken;
    }

    bool QuestState::Take()
    {
        if (!m_bIsTaken)
        {
            if (m_bCanBeGiven)
            {
                m_bIsTaken = true;
                m_TakeTime = theObjects->GetGameTimeInt64();
                theQuestStateManager->OnQuestTaken(m_questId);
                return true;
            }
            M3D_LOG_ERR("Error: attempt to take a quest which cannot be given: '" + theQuestManager->GetQuestById(m_questId)->GetName() + "'");
        }
        return false;
    }

    bool QuestState::LessByTakeTime(QuestState const* a, QuestState const* b)
    {
        // RVA 0x699FE0
        return a->m_TakeTime < b->m_TakeTime;
    }

    void QuestState::Update()
    {
        // RVA 0x69F710 - a taken quest with a time limit fails once that time is up.
        if (!m_bIsTaken)
        {
            return;
        }
        int64_t const timeForComplete = theQuestManager->GetQuestById(m_questId)->getTimeForComplete();
        if (timeForComplete > 0 && theObjects->GetGameTimeInt64() - getTakeTime() > timeForComplete &&
            m_completeStatus == NOT_COMPLETE)
        {
            m_completeStatus = FAILED;
            theQuestStateManager->OnQuestCompleteStatusChanged(m_questId);
        }
    }

    bool QuestState::Complete()
    {
        // RVA 0x69F570 - a quest with subquests is completed by them, never directly.
        Quest const* const quest = theQuestManager->GetQuestById(m_questId);
        if (!quest->GetSubQuestIds().empty())
        {
            M3D_LOG_ERR(
                "Error: the status COMPLETE is set directly for the non-leaf quest: '" +
                theQuestManager->GetQuestById(m_questId)->GetName() + CStr("'"));
            return false;
        }
        return _SetCompleteStatusUnsafe(COMPLETE);
    }

    bool QuestState::_SetCompleteStatusUnsafe(CompleteStatus completeStatus)
    {
        // RVA 0x69EFB0 - a quest that already finished keeps the status it has.
        if (m_completeStatus != NOT_COMPLETE)
        {
            return false;
        }
        m_completeStatus = completeStatus;
        theQuestStateManager->OnQuestCompleteStatusChanged(m_questId);
        return true;
    }

    bool QuestState::_bIsLeaf() const
    {
        // RVA 0x69B910
        return theQuestManager->GetQuestById(m_questId)->GetSubQuestIds().empty();
    }

    bool QuestStateManager::LoadFromXmlFile(CStr const& fileName)
    {
        // RVA 0x6A0000 - the states are loaded once; a second call keeps what is there.
        if (!m_questStates.empty())
        {
            return true;
        }
        CStr err;
        ref_ptr xmlFile = m3d::ReadXmlFile(fileName.c_str(), &err);
        if (!xmlFile)
        {
            M3D_LOG_INFO("No file: " + fileName + err);
            return false;
        }
        ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild(rootNode, nullptr);
        LoadFromXml(xmlFile, rootNode);
        return true;
    }

    bool QuestStateManager::SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x69C270
        for (QuestState const* const questState : m_questStates)
        {
            ref_ptr questNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "quest");
            xmlNode->AddChild(questNode);
            questState->SaveToXml(xmlFile, questNode);
        }
        return true;
    }

    m3d::Object* QuestStateManager::CreateObject()
    {
        return new QuestStateManager;
    }

    void QuestStateManager::FailQuestIfTaken(char const* questName)
    {
        // RVA 0x6A0AA0
        QuestState* const questState = _GetQuestStateByName(questName);
        if (!questState)
        {
            // NOTE: the message names FailQuest, not FailQuestIfTaken.
            M3D_LOG_ERR("Error in FailQuest(): invalid quest name: '" + CStr(questName) + CStr("'"));
            return;
        }
        if (questState->bIsTaken() && questState->Fail())
        {
            _RecalcStates(questState->GetQuestId());
        }
    }

    m3d::Class* QuestStateManager::GetClass() const
    {
        return RT_CLASS_LOCAL(QuestStateManager);
    }

    void QuestStateManager::CompleteQuestIfTaken(char const* questName)
    {
        // RVA 0x6A07C0
        QuestState* const questState = _GetQuestStateByName(questName);
        if (!questState)
        {
            // NOTE: the message names CompleteQuest, not CompleteQuestIfTaken.
            M3D_LOG_ERR("Error in CompleteQuest(): invalid quest name: '" + CStr(questName) + CStr("'"));
            return;
        }
        if (questState->bIsTaken() && questState->Complete())
        {
            _RecalcStates(questState->GetQuestId());
        }
    }

    void QuestStateManager::OnQuestCanBeGiven(int questId)
    {
        // RVA 0x69C920
        QuestState const* const questState = _GetQuestStateById(questId);
        CStr const& questName = theQuestManager->GetQuestById(questId)->GetName();
        CStr const onCanBeGivenFuncName = theQuestManager->GetQuestById(questState->GetQuestId())->GetFuncOnCanBeGiven();
        if (!onCanBeGivenFuncName.empty())
        {
            _ExecuteFunctionCode(onCanBeGivenFuncName, CStr("CAN_BE_GIVEN_") + questName);
        }
        M3D_LOG_INFO("Quest '" + questName + CStr("' can be given now"));
    }

    bool QuestStateManager::LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* rootNode)
    {
        // RVA 0x69F790 - one state per known quest, then whatever the file says about them. The loaded states are
        // posted in the order they were taken, and the quests with no preceders start the can-be-given sweep.
        _Clear();
        ref_ptr questNode = xmlFile->CreateNode();
        unsigned const numQuests = theQuestManager->GetNumQuests();
        m_questStates.resize(numQuests, nullptr);
        for (unsigned i = 0; i < numQuests; ++i)
        {
            m_questStates[i] = new QuestState(i);
        }

        retruxx::map<int, QuestState*> tmpQuestStateMap;
        int iQuest = 0;
        for (rootNode->GetFirstChild(questNode, "quest"); !questNode->IsEmpty(); questNode->GetNextSibling(questNode, "quest"))
        {
            ++iQuest;
            QuestState questState(-1);
            if (!questState.LoadFromXml(xmlFile, questNode))
            {
                M3D_LOG_ERR(
                    "Error: couldn't load quest info from XML file. The quest is " + CStr(iQuest) + CStr("th in the file"));
                continue;
            }
            if (tmpQuestStateMap.find(questState.GetQuestId()) != tmpQuestStateMap.end())
            {
                M3D_LOG_ERR(
                    "Error in quest info file: the record for the quest id=" + CStr(questState.GetQuestId()) +
                    CStr(" occurs more than once"));
                continue;
            }
            tmpQuestStateMap[questState.GetQuestId()] = new QuestState(questState);
        }

        for (auto const& [questId, questState] : tmpQuestStateMap)
        {
            delete m_questStates[questId];
            m_questStates[questId] = questState;
        }

        retruxx::vector<QuestState*> tmpQuestStates = m_questStates;
        std::stable_sort(tmpQuestStates.begin(), tmpQuestStates.end(), QuestState::LessByTakeTime);
        for (QuestState* const questState : tmpQuestStates)
        {
            questState->PostLoad();
        }

        std::queue<int> q;
        for (unsigned i = 0; i < numQuests; ++i)
        {
            if (theQuestManager->GetQuestById(m_questStates[i]->GetQuestId())->GetPrecedingQuestIds().empty())
            {
                q.push(i);
            }
        }
        _UpdateCanBeGiven(q);
        return true;
    }

    void QuestStateManager::OnPlayerPassToMap(CStr const& targetMapName)
    {
        // RVA 0x6A0DA0
        for (QuestState* const questState : m_questStates)
        {
            questState->OnPlayerPassToMap(targetMapName);
        }
    }

    m3d::Object* QuestStateManager::Clone()
    {
        // RVA 0x69E1B0 - NOTE: the binary builds a manager and then asserts anyway, leaking it.
        M3D_ASSERT(0);
        return nullptr;
    }

    void QuestStateManager::OnQuestCompleteStatusChanged(int questId)
    {
        // RVA 0x69E980
        QuestState const* const questState = _GetQuestStateById(questId);
        CStr const& questName = theQuestManager->GetQuestById(questId)->GetName();
        M3D_APP->EnqueueMessage(66539, questId, 0, 0, 0, CStr(), m3d::AIParam());

        CStr onChangeCompleteStatusFuncName;
        if (questState->GetCompleteStatus() == QuestState::COMPLETE)
        {
            onChangeCompleteStatusFuncName = theQuestManager->GetQuestById(questState->GetQuestId())->GetFuncOnComplete();
            M3D_LOG_INFO("Quest '" + questName + CStr("' is complete"));
        }
        else if (questState->GetCompleteStatus() == QuestState::FAILED)
        {
            onChangeCompleteStatusFuncName = theQuestManager->GetQuestById(questState->GetQuestId())->GetFuncOnFail();
            M3D_LOG_INFO("Quest '" + questName + CStr("' is failed"));
        }
        else
        {
            M3D_LOG_INFO("Something wrong with quest '" + questName + CStr("'"));
        }
        if (!onChangeCompleteStatusFuncName.empty())
        {
            _ExecuteFunctionCode(onChangeCompleteStatusFuncName, CStr("CHANGE_COMPLETE_STATUS_") + questName);
        }
    }

    void QuestStateManager::TakeQuest(char const* questName)
    {
        auto* quest = _GetQuestStateByName(questName);
        if (quest)
        {
            if (quest->Take())
            {
                _RecalcStates(quest->GetQuestId());
            }
        }
        else
        {
            M3D_LOG_ERR("Error in TakeQuest(): invalid quest name: '" + CStr(questName) + "'");
        }
    }

    bool QuestStateManager::IsQuestComplete(char const* questName) const
    {
        auto* quest = _GetQuestStateByName(questName);
        if (quest)
        {
            return quest->GetCompleteStatus() == QuestState::COMPLETE;
        }

        M3D_LOG_ERR("Error in IsQuestComplete(): invalid quest name: '" + CStr(questName) + "'");
        return false;
    }

    bool QuestStateManager::IsQuestTaken(char const* questName) const
    {
        auto* quest = _GetQuestStateByName(questName);
        if (quest)
        {
            return quest->bIsTaken();
        }

        M3D_LOG_ERR("Error in IsQuestTaken(): invalid quest name: '" + CStr(questName) + "'");
        return false;
    }

    void QuestStateManager::Update(float elapsedTime)
    {
        if (m_UpdateCount > 0x64)
        {
            for (auto& state : m_questStates)
            {
                state->Update();
            }
            m_UpdateCount = 0;
        }
        ++m_UpdateCount;
    }

    void QuestStateManager::FailQuest(char const* questName)
    {
        // RVA 0x6A0920
        QuestState* const questState = _GetQuestStateByName(questName);
        if (!questState)
        {
            M3D_LOG_ERR("Error in FailQuest(): invalid quest name: '" + CStr(questName) + CStr("'"));
            return;
        }
        if (questState->Fail())
        {
            _RecalcStates(questState->GetQuestId());
        }
    }

    QuestState const* QuestStateManager::GetQuestStateById(int questId) const
    {
        // RVA 0x550A90
        return _GetQuestStateById(questId);
    }

    bool QuestStateManager::CanQuestBeGiven(char const* questName) const
    {
        // RVA 0x69C7C0 - a quest can be given while it is neither taken nor finished.
        QuestState const* const questState = _GetQuestStateByName(questName);
        if (questState)
        {
            return questState->bCanBeGiven() && !questState->bIsTaken() &&
                questState->GetCompleteStatus() == QuestState::NOT_COMPLETE;
        }
        // NOTE: "CanQuestBeQiven" is the binary's own spelling.
        M3D_LOG_ERR("Error in CanQuestBeQiven(): invalid quest name: '" + CStr(questName) + CStr("'"));
        return false;
    }

    void QuestStateManager::CompleteQuest(char const* questName)
    {
        // RVA 0x6A0660
        QuestState* const questState = _GetQuestStateByName(questName);
        if (!questState)
        {
            M3D_LOG_ERR("Error in CompleteQuest(): invalid quest name: '" + CStr(questName) + CStr("'"));
            return;
        }
        if (questState->Complete())
        {
            _RecalcStates(questState->GetQuestId());
        }
    }

    QuestStateManager::~QuestStateManager()
    {
        _Clear();
    }

    m3d::Class* QuestStateManager::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    void QuestStateManager::OnQuestTaken(int questId)
    {
        // RVA 0x69E680
        QuestState const* const questState = _GetQuestStateById(questId);
        CStr const& questName = theQuestManager->GetQuestById(questId)->GetName();
        CStr const onTakeFuncName = theQuestManager->GetQuestById(questState->GetQuestId())->GetFuncOnTake();
        if (!onTakeFuncName.empty())
        {
            _ExecuteFunctionCode(onTakeFuncName, CStr("TAKE_") + questName);
        }
        M3D_APP->EnqueueMessage(66538, questId, 0, 0, 0, CStr(), m3d::AIParam());
        M3D_LOG_INFO("Quest '" + questName + CStr("' is taken"));
    }

    bool QuestStateManager::IsQuestFailed(char const* questName) const
    {
        // RVA 0x69C670
        QuestState const* const questState = _GetQuestStateByName(questName);
        if (questState)
        {
            return questState->GetCompleteStatus() == QuestState::FAILED;
        }
        M3D_LOG_ERR("Error in IsQuestFailed(): invalid quest name: '" + CStr(questName) + CStr("'"));
        return false;
    }

    QuestStateManager::QuestStateManager()
    {
    }

    QuestStateManager::QuestStateManager(QuestStateManager const&) : m_UpdateCount(0)
    {
        // Not emitted in the shipped binary.
    }

    void QuestStateManager::_ExecuteFunctionCode(CStr const& funcNameOrCode, CStr const& codeBuffer)
    {
        // RVA 0x69A9A0 - a plain name is called as a script function, anything else is run as a chunk of code.
        m3d::ScriptServer& scriptServer = M3D_KERNEL->GetScriptServer();
        m3d::eScriptError err;
        if (funcNameOrCode.findOneOf(" :.;,(){}[]") == -1)
        {
            m3d::sArgStack stack;
            err = scriptServer.callScriptFunc(funcNameOrCode.c_str(), stack, 0);
        }
        else
        {
            err = scriptServer.execute(funcNameOrCode.c_str(), codeBuffer.c_str());
        }
        if (err)
        {
            M3D_LOG_ERR(scriptServer.getFormatedScriptErrorDesc(err));
        }
    }

    void QuestStateManager::_UpdateCanBeGiven(std::queue<int, std::deque<int, std::allocator<int>>>& q)
    {
        // RVA 0x69F440 - recalculates each quest in the queue and walks on to the quests that follow it.
        while (!q.empty())
        {
            int const questId = q.front();
            q.pop();
            _GetQuestStateById(questId)->RecalcCanBeGiven();
            for (int const subsequentQuestId : theQuestManager->GetQuestById(questId)->GetSubsequentQuestIds())
            {
                q.push(subsequentQuestId);
            }
        }
    }

    void QuestStateManager::_UpdateCanBeGiven(int questId)
    {
        // RVA 0x69F520
        std::queue<int> q;
        q.push(questId);
        _UpdateCanBeGiven(q);
    }

    void QuestStateManager::_RecalcStates(int questId)
    {
        // RVA 0x6A0240 - walks up the quest tree from the one that changed, recalculating each quest and the
        // can-be-given flags below it. A finished quest also recalculates its subquests, and a completed one fails
        // every quest it is mutually exclusive with.
        M3D_LOG_INFO(CStr("\tRecalc begin..."));
        for (int id = questId; id != -1; id = theQuestManager->GetQuestById(id)->GetParentId())
        {
            QuestState* const questState = _GetQuestStateById(id);
            questState->RecalcState();
            Quest const* const quest = theQuestManager->GetQuestById(id);
            _UpdateCanBeGiven(id);

            if (quest->GetSubQuestCondition() == Quest::SUBQUEST_XOR || questState->GetCompleteStatus() == QuestState::FAILED)
            {
                for (int const subQuestId : quest->GetSubQuestIds())
                {
                    _GetQuestStateById(subQuestId)->RecalcState();
                }
            }

            if (questState->GetCompleteStatus() == QuestState::COMPLETE)
            {
                retruxx::set<int> const* const mutex = theQuestManager->GetMutexByQuestId(id);
                if (mutex)
                {
                    for (int const mutexQuestId : *mutex)
                    {
                        if (mutexQuestId == id)
                        {
                            continue;
                        }
                        if (_GetQuestStateById(mutexQuestId)->Fail())
                        {
                            _RecalcStates(mutexQuestId);
                        }
                    }
                }
            }
        }
        M3D_LOG_INFO(CStr("\tRecalc end..."));
    }

    QuestState* QuestStateManager::_GetQuestStateById(int questId) const
    {
        // RVA 0x69C320
        if (questId < 0 || questId >= static_cast<int>(m_questStates.size()))
        {
            return nullptr;
        }
        return m_questStates[questId];
    }

    QuestState* QuestStateManager::_GetQuestStateByName(char const* questName) const
    {
        auto id = theQuestManager->GetQuestIdByName(questName);
        if (id < 0 || id >= m_questStates.size())
        {
            return nullptr;
        }
        return m_questStates[id];
    }

    void QuestStateManager::_Clear()
    {
        for (auto& state : m_questStates)
        {
            delete state;
        }
        m_questStates.clear();
    }
}

#include "queststate.h"

#include <stdexcept>

#include "quest.h"
#include "core/log.h"
#include "objects/base/objcontainer.h"

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, TakeQuest)
{
    auto manager = (ai::QuestStateManager*)context->asObject(0, "QuestStateManager");
    auto* quest = context->asString(1);
    manager->TakeQuest(quest);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CompleteQuest)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CompleteQuestIfTaken)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, FailQuest)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, FailQuestIfTaken)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CanQuestBeGiven)
{
    RETRUXX_NOT_IMPLEMENTED;
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

    int QuestState::GetQuestId() const
    {
        return m_questId;
    }

    long long QuestState::getTakeTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    QuestState::~QuestState()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    QuestState::QuestState(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestState::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Quest* QuestState::GetQuest() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestState::RecalcState()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestState::bCanBeGiven() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestState::OnPlayerPassToMap(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestState::Fail()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestState::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    QuestState::CompleteStatus QuestState::GetCompleteStatus() const
    {
        return m_completeStatus;
    }

    bool QuestState::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestState::RecalcCanBeGiven()
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    bool QuestState::LessByTakeTime(QuestState const*, QuestState const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestState::Update()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestState::Complete()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestState::_SetCompleteStatusUnsafe(CompleteStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestState::_bIsLeaf() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestStateManager::LoadFromXmlFile(CStr const&)
    {
        // TODO: implement QuestStateManager::LoadFromXmlFile
        return false;
        //RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestStateManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* QuestStateManager::CreateObject()
    {
        return new QuestStateManager;
    }

    void QuestStateManager::FailQuestIfTaken(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* QuestStateManager::GetClass() const
    {
        return RT_CLASS_LOCAL(QuestStateManager);
    }

    void QuestStateManager::CompleteQuestIfTaken(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestStateManager::OnQuestCanBeGiven(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestStateManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestStateManager::OnPlayerPassToMap(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* QuestStateManager::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestStateManager::OnQuestCompleteStatusChanged(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void QuestStateManager::FailQuest(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    QuestState const* QuestStateManager::GetQuestStateById(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestStateManager::CanQuestBeGiven(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestStateManager::CompleteQuest(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    QuestStateManager::~QuestStateManager()
    {
        _Clear();
    }

    m3d::Class* QuestStateManager::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    void QuestStateManager::OnQuestTaken(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool QuestStateManager::IsQuestFailed(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    QuestStateManager::QuestStateManager()
    {
    }

    QuestStateManager::QuestStateManager(QuestStateManager const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestStateManager::_ExecuteFunctionCode(CStr const&, CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestStateManager::_UpdateCanBeGiven(std::queue<int, std::deque<int, std::allocator<int>>>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestStateManager::_UpdateCanBeGiven(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuestStateManager::_RecalcStates(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    QuestState* QuestStateManager::_GetQuestStateById(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

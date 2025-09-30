#include "queststate.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, TakeQuest)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CompleteQuest)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CompleteQuestIfTaken)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, FailQuest)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, FailQuestIfTaken)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, IsQuestTaken)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, IsQuestComplete)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, IsQuestFailed)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(QuestStateManager, CanQuestBeGiven)
{
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    long long QuestState::getTakeTime() const
    {
        throw std::logic_error("Not implemented");
    }

    QuestState::~QuestState()
    {
        throw std::logic_error("Not implemented");
    }

    QuestState::QuestState(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    Quest* QuestState::GetQuest() const
    {
        throw std::logic_error("Not implemented");
    }

    void QuestState::RecalcState()
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::bCanBeGiven() const
    {
        throw std::logic_error("Not implemented");
    }

    void QuestState::OnPlayerPassToMap(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::Fail()
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    QuestState::CompleteStatus QuestState::GetCompleteStatus() const
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestState::RecalcCanBeGiven()
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::bIsTaken() const
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::Take()
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::LessByTakeTime(QuestState const*, QuestState const*)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestState::Update()
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::Complete()
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::_SetCompleteStatusUnsafe(CompleteStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestState::_bIsLeaf() const
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestStateManager::LoadFromXmlFile(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestStateManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* QuestStateManager::CreateObject()
    {
        return new QuestStateManager;
    }

    void QuestStateManager::FailQuestIfTaken(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* QuestStateManager::GetClass() const
    {
        return RT_CLASS_LOCAL(QuestStateManager);
    }

    void QuestStateManager::CompleteQuestIfTaken(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::OnQuestCanBeGiven(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestStateManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::OnPlayerPassToMap(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* QuestStateManager::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::OnQuestCompleteStatusChanged(int)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::TakeQuest(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestStateManager::IsQuestComplete(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestStateManager::IsQuestTaken(char const*) const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    QuestState const* QuestStateManager::GetQuestStateById(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestStateManager::CanQuestBeGiven(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::CompleteQuest(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    QuestStateManager::~QuestStateManager()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* QuestStateManager::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    void QuestStateManager::OnQuestTaken(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool QuestStateManager::IsQuestFailed(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    QuestStateManager::QuestStateManager()
    {
    }

    QuestStateManager::QuestStateManager(QuestStateManager const&)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::_ExecuteFunctionCode(CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::_UpdateCanBeGiven(std::queue<int, std::deque<int, std::allocator<int>>>&)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::_UpdateCanBeGiven(int)
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::_RecalcStates(int)
    {
        throw std::logic_error("Not implemented");
    }

    QuestState* QuestStateManager::_GetQuestStateById(int) const
    {
        throw std::logic_error("Not implemented");
    }

    QuestState* QuestStateManager::_GetQuestStateByName(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void QuestStateManager::_Clear()
    {
        throw std::logic_error("Not implemented");
    }
}

#include "questinfo.h"
#include <stdexcept>

#include "game/m3dgame.h"
#include "server/quest.h"

int QuestInfoManager::SaveModifiedQuestInfosToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

QuestInfoManager::QuestInfoManager()
{
}

QuestInfo const* QuestInfoManager::GetQuestInfoByQuestInfoName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void QuestInfoManager::SetCoordinateForQuest(CStr const&, CStr const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

CStr QuestInfoManager::GetDynamicQuestInfoPatternNameByType(int)
{
    throw std::logic_error("Not implemented");
}

QuestInfo const* QuestInfoManager::GetQuestInfoForStaticQuest(int) const
{
    throw std::logic_error("Not implemented");
}

void QuestInfoManager::Clear()
{
    throw std::logic_error("Not implemented");
}

QuestInfoManager::~QuestInfoManager()
{
    throw std::logic_error("Not implemented");
}

int QuestInfoManager::Init()
{
    // TODO: implement QuestInfoManager::Init
    return 0;
    //throw std::logic_error("Not implemented");
    if (m_isInited)
    {
        ClearQuestInfos();
        ClearDynamicQuestInfos();
        m_isInited = false;
    }
    //M3D_APP->m_pInterfaceManager->GetPathToDialogsFileGlobal()
    return 1;
    throw std::logic_error("Not implemented");
}

int QuestInfoManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

bool QuestInfoManager::IsQuestDynamic(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int QuestInfoManager::GameDataUpdate(void* data, int dataType)
{
    switch (dataType)
    {
    case 67:
    {
        OnDynamicQuestStateChanged(data);
        return 1;
    }
    case 85:
    {
        OnStartLevel();
        return 1;
    }
    case 86:
    {
        ClearQuestInfos();
        ClearDynamicQuestInfos();
        m_isInited = false;
        return 1;
    }
    case 87:
    {
        ClearDynamicQuestInfos();
        return 1;
    }
    default:
    {
        return 1;
    }
    }
}

QuestInfo const* QuestInfoManager::GetQuestInfoForDynamicQuest(int)
{
    throw std::logic_error("Not implemented");
}

int QuestInfoManager::LoadModifiedQuestInfosFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

void QuestInfoManager::OnEndLevel()
{
    throw std::logic_error("Not implemented");
}

void QuestInfoManager::ClearQuestInfos()
{
    throw std::logic_error("Not implemented");
}

QuestInfo* QuestInfoManager::CreateQuestInfoForDynamicQuest(int) const
{
    throw std::logic_error("Not implemented");
}

void QuestInfoManager::OnStartLevel()
{
    //TODO: check this
    for (auto& info : m_questInfos)
    {
        if (info.second)
        {
            if (info.second->m_questType == help::QUESTTYPE_STATIC)
            {
                info.second->m_questId = ai::theQuestManager->GetQuestIdByName(info.second->m_questInfoName);
            }
        }
    }
}

void QuestInfoManager::OnDynamicQuestStateChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void QuestInfoManager::GetAllQuestInfosByNamePattern(CStr const&, std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

int QuestInfoManager::LoadFromFile(CStr const&)
{
    throw std::logic_error("Not implemented");
}

QuestInfo const* QuestInfoManager::AddQuestInfoForDynamicQuest(int)
{
    throw std::logic_error("Not implemented");
}

void QuestInfoManager::ClearDynamicQuestInfos()
{
    throw std::logic_error("Not implemented");
}

void QuestInfoManager::OnEndLevelBeforeContinuousLevel()
{
    throw std::logic_error("Not implemented");
}

int QuestInfoManager::RemoveQuestInfoForDynamicQuest(int)
{
    throw std::logic_error("Not implemented");
}

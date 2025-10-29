#include "questinfo.h"
#include <stdexcept>

#include "game/m3dgame.h"
#include "server/quest.h"

int QuestInfoManager::SaveModifiedQuestInfosToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfoManager::QuestInfoManager()
{
}

QuestInfo const* QuestInfoManager::GetQuestInfoByQuestInfoName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::SetCoordinateForQuest(CStr const&, CStr const&, CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr QuestInfoManager::GetDynamicQuestInfoPatternNameByType(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfo const* QuestInfoManager::GetQuestInfoForStaticQuest(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfoManager::~QuestInfoManager()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::Init()
{
    // TODO: implement QuestInfoManager::Init
    return 0;
    //RETRUXX_NOT_IMPLEMENTED;
    if (m_isInited)
    {
        ClearQuestInfos();
        ClearDynamicQuestInfos();
        m_isInited = false;
    }
    //M3D_APP->m_pInterfaceManager->GetPathToDialogsFileGlobal()
    return 1;
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool QuestInfoManager::IsQuestDynamic(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::LoadModifiedQuestInfosFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::OnEndLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::ClearQuestInfos()
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfo* QuestInfoManager::CreateQuestInfoForDynamicQuest(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::GetAllQuestInfosByNamePattern(CStr const&, std::vector<CStr>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::LoadFromFile(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfo const* QuestInfoManager::AddQuestInfoForDynamicQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::ClearDynamicQuestInfos()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::OnEndLevelBeforeContinuousLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::RemoveQuestInfoForDynamicQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

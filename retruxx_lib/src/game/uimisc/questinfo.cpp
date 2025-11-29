#include "questinfo.h"

#include "core/log.h"

#include <stdexcept>

#include "game/m3dgame.h"
#include "server/quest.h"

QuestInfo::AuxLevelInfo::~AuxLevelInfo()
{
    // TODO: check this
    delete m_coordinate;
}

QuestInfo::~QuestInfo() = default;

int QuestInfoManager::SaveModifiedQuestInfosToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestInfoManager::QuestInfoManager() = default;

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
    if (m_isInited)
    {
        ClearQuestInfos();
        ClearDynamicQuestInfos();
        m_isInited = false;
    }

    const auto pathToDlg = M3D_APP->m_pInterfaceManager->GetPathToQuestInfoFileGlobal();
    const auto res = LoadFromFile(pathToDlg);

    m_isInited = true;
    if (res)
    {
        M3D_LOG_INFO("QuestInfo manager was inited successfully");
    }
    else
    {
        M3D_LOG_INFO("QuestInfo manager was inited with errors");
    }

    return 1;
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
    for (auto& questInfo : m_questInfos)
    {
        delete questInfo.second;
    }

    m_questInfos.clear();
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
    // TODO: implement QuestInfoManager::LoadFromFile
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

QuestInfo const* QuestInfoManager::AddQuestInfoForDynamicQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestInfoManager::ClearDynamicQuestInfos()
{
    for (auto& questInfo : m_dynamicQuestInfos)
    {
        delete questInfo.second;
    }

    m_dynamicQuestInfos.clear();
}

void QuestInfoManager::OnEndLevelBeforeContinuousLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestInfoManager::RemoveQuestInfoForDynamicQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

#include "questinfo.h"
#include <stdexcept>

int QuestInfoManager::SaveModifiedQuestInfosToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

QuestInfoManager::QuestInfoManager()
{
    throw std::logic_error("Not implemented");
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

int QuestInfoManager::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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

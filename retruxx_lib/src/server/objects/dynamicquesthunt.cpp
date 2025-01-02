#include "dynamicquesthunt.h"
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestHunt)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestHunt);

    ai::DynamicQuest* DynamicQuestHuntPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestHuntPrototypeInfo::DynamicQuestHuntPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool DynamicQuestHuntPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestHunt::DynamicQuestHunt(DynamicQuestHuntPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestHunt::ConsiderPlayerKill(int)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestManager::QuestType DynamicQuestHunt::GetQuestType() const
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestHunt::UpdateBeforeShowInConversation()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestHunt::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    float DynamicQuestHunt::GetTimeBeforeHuntEnd() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestHunt::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestHunt::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestHunt::ProcessHuntCompletion()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestHunt::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestHuntPrototypeInfo const* DynamicQuestHunt::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    float DynamicQuestHunt::GetTimeFromHuntStart() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestHunt::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestHunt::PassToAnotherMap()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestHunt::_OnTake()
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestHunt::_CalcReward()
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestHunt::~DynamicQuestHunt()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestHunt::_OnCreate()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestHunt::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestHunt::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

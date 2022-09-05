#include "dynamicquestreach.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestReach)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestReach);

    DynamicQuestReachPrototypeInfo::DynamicQuestReachPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    ai::DynamicQuest* DynamicQuestReachPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool DynamicQuestReachPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestReach::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestReachPrototypeInfo const* DynamicQuestReach::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestReach::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestManager::QuestType DynamicQuestReach::GetQuestType() const
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestReach::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestReach::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestReach::DynamicQuestReach(DynamicQuestReachPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestReach::_OnCreate()
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestReach::_CalcReward()
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestReach::~DynamicQuestReach()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestReach::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestReach::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestReach::_OnObjectEntersLocation(Event const&)
    {
        throw std::logic_error("Not implemented");
    }
}

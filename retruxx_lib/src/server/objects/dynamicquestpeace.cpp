#include "dynamicquestpeace.h"
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestPeace)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestPeace);

    DynamicQuestPeacePrototypeInfo::DynamicQuestPeacePrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    ai::DynamicQuest* DynamicQuestPeacePrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool DynamicQuestPeacePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestPeace::DynamicQuestPeace(DynamicQuestPeacePrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestPeace::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestPeacePrototypeInfo const* DynamicQuestPeace::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestPeace::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestManager::QuestType DynamicQuestPeace::GetQuestType() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestPeace::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestPeace::PassToAnotherMap()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestPeace::_OnCreate()
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestPeace::_CalcReward()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestPeace::_OnTake()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestPeace::_OnHirerBecamesEnemyWithPlayer()
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestPeace::~DynamicQuestPeace()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestPeace::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestPeace::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

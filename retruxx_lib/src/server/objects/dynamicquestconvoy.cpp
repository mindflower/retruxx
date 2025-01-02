#include "dynamicquestconvoy.h"
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestConvoy)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestConvoy);

    DynamicQuestConvoyPrototypeInfo::DynamicQuestConvoyPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool DynamicQuestConvoyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuest* DynamicQuestConvoyPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestManager::QuestType DynamicQuestConvoy::GetQuestType() const
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestConvoy::GetCaravanId() const
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestConvoy::DynamicQuestConvoy(DynamicQuestConvoyPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestConvoy::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestConvoyPrototypeInfo const* DynamicQuestConvoy::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestConvoy::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestConvoy::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestConvoy::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::_Unsubscribe()
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestConvoy::~DynamicQuestConvoy()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::_OnTake()
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestConvoy::_CalcReward()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::_OnCreate()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::_Subscribe(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::_OnObjectDie(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::_OnTargetReached(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestConvoy::_OnObjectEntersLocation(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestConvoy::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestConvoy::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

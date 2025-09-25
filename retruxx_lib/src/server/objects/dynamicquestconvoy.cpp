#include "dynamicquestconvoy.h"
#include <stdexcept>

#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestConvoy)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestConvoy);

    DynamicQuestConvoyPrototypeInfo::DynamicQuestConvoyPrototypeInfo()
    {
        this->m_playerSchwarzPart = 0.0;
        this->m_criticalDistFromPlayer = 100.0;
        this->m_criticalTime = 20.0;
    }

    bool DynamicQuestConvoyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_playerSchwarzPart, xmlNode, "PlayerSchwarzPart");
            m3d::SafeFloatAttrib(m_criticalDistFromPlayer, xmlNode, "CriticalDistFromPlayer");
            m3d::SafeFloatAttrib(m_criticalTime, xmlNode, "CriticalTime");
            return 1;
        }
        return result;
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

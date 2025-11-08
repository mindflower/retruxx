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
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestManager::QuestType DynamicQuestConvoy::GetQuestType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestConvoy::GetCaravanId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestConvoy::DynamicQuestConvoy(DynamicQuestConvoyPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuestConvoy::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestConvoyPrototypeInfo const* DynamicQuestConvoy::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuestConvoy::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestConvoy::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestConvoy::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::_Unsubscribe()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestConvoy::~DynamicQuestConvoy()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::_OnTake()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestConvoy::_CalcReward()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::_OnCreate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::_Subscribe(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::_OnObjectDie(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::_OnTargetReached(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestConvoy::_OnObjectEntersLocation(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestConvoy::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestConvoy::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

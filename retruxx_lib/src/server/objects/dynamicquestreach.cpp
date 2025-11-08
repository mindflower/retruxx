#include "dynamicquestreach.h"
#include <stdexcept>

#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestReach)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestReach);

    DynamicQuestReachPrototypeInfo::DynamicQuestReachPrototypeInfo()
    {
        m_playerSchwarzPart = 0.0;
    }

    ai::DynamicQuest* DynamicQuestReachPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DynamicQuestReachPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_playerSchwarzPart, xmlNode, "PlayerSchwarzPart");
            return 1;
        }
        return result;
    }

    m3d::Class* DynamicQuestReach::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestReachPrototypeInfo const* DynamicQuestReach::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestReach::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestManager::QuestType DynamicQuestReach::GetQuestType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestReach::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuestReach::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    DynamicQuestReach::DynamicQuestReach(DynamicQuestReachPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestReach::_OnCreate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestReach::_CalcReward()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestReach::~DynamicQuestReach()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestReach::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestReach::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestReach::_OnObjectEntersLocation(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

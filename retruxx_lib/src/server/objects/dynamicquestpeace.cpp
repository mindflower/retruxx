#include "dynamicquestpeace.h"
#include <stdexcept>

#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestPeace)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestPeace);

    DynamicQuestPeacePrototypeInfo::DynamicQuestPeacePrototypeInfo()
    {
        m_playerMoneyPart = 0.0;
    }

    ai::DynamicQuest* DynamicQuestPeacePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DynamicQuestPeacePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_playerMoneyPart, xmlNode, "PlayerMoneyPart");
            return 1;
        }
        return result;
    }

    DynamicQuestPeace::DynamicQuestPeace(DynamicQuestPeacePrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuestPeace::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestPeacePrototypeInfo const* DynamicQuestPeace::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestPeace::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestManager::QuestType DynamicQuestPeace::GetQuestType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuestPeace::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestPeace::PassToAnotherMap()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestPeace::_OnCreate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestPeace::_CalcReward()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestPeace::_OnTake()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestPeace::_OnHirerBecamesEnemyWithPlayer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestPeace::~DynamicQuestPeace()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestPeace::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestPeace::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

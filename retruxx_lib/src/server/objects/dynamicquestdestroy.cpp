#include "dynamicquestdestroy.h"
#include <stdexcept>

#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestDestroy)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestDestroy);

    bool DynamicQuestDestroyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_targetSchwarzPart, xmlNode, "TargetSchwarzPart");
            return 1;
        }
        return result;
    }

    DynamicQuestDestroyPrototypeInfo::DynamicQuestDestroyPrototypeInfo()
    {
        m_targetSchwarzPart = 0.0;
    }

    DynamicQuest* DynamicQuestDestroyPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestDestroy::DynamicQuestDestroy(DynamicQuestDestroyPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestDestroy::LoadNamesForTargetsFromXml(m3d::cmn::XmlNode const*)
    {
        // TODO: implement DynamicQuestDestroy::LoadNamesForTargetsFromXml
        //RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuestDestroy::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    m3d::Class* DynamicQuestDestroy::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DynamicQuestDestroy::FreeNameForTargetExists()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestDestroyPrototypeInfo const* DynamicQuestDestroy::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestDestroy::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestDestroy::SaveNamesForTargetsToXml(m3d::cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestManager::QuestType DynamicQuestDestroy::GetQuestType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestDestroy::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr DynamicQuestDestroy::GetRandomNameForTarget()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestDestroy::_CalcReward()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestDestroy::_OnCreate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestDestroy::~DynamicQuestDestroy()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestDestroy::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestDestroy::_OnObjectDie(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestDestroy::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

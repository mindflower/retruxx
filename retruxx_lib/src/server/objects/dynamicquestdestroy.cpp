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
        throw std::logic_error("Not implemented");
    }

    DynamicQuestDestroy::DynamicQuestDestroy(DynamicQuestDestroyPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestDestroy::LoadNamesForTargetsFromXml(m3d::cmn::XmlNode const*)
    {
        // TODO: implement DynamicQuestDestroy::LoadNamesForTargetsFromXml
        //throw std::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuestDestroy::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    m3d::Class* DynamicQuestDestroy::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    bool DynamicQuestDestroy::FreeNameForTargetExists()
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestDestroyPrototypeInfo const* DynamicQuestDestroy::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestDestroy::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestDestroy::SaveNamesForTargetsToXml(m3d::cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestManager::QuestType DynamicQuestDestroy::GetQuestType() const
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestDestroy::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    CStr DynamicQuestDestroy::GetRandomNameForTarget()
    {
        throw std::logic_error("Not implemented");
    }

    int DynamicQuestDestroy::_CalcReward()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestDestroy::_OnCreate()
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuestDestroy::~DynamicQuestDestroy()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestDestroy::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void DynamicQuestDestroy::_OnObjectDie(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuestDestroy::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

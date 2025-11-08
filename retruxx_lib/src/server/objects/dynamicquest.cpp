#include "dynamicquest.h"
#include <stdexcept>

#include "core/ini.h"

RT_CLASS_EXPORT_METHOD_DEFINE(DynamicQuest, Take)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DynamicQuest, GetReward)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuest)
        RT_CLASS_EXPORT(DynamicQuest, m3d::METHOD, Take, "", "", "")
        RT_CLASS_EXPORT(DynamicQuest, m3d::METHOD, GetReward, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuest);

    DynamicQuestPrototypeInfo::DynamicQuestPrototypeInfo()
    {
        m_minReward = 0;
    }

    bool DynamicQuestPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeIntAttrib(m_minReward, xmlNode, "MinReward");
            return 1;
        }
        return result;
    }

    int DynamicQuest::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DynamicQuest::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuest::DynamicQuest(DynamicQuestPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestPrototypeInfo const* DynamicQuest::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr DynamicQuest::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GameTime const& DynamicQuest::GetTakeGameTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& DynamicQuest::GetTargetName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuest::GetHirerObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::Take()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::PassToAnotherMap()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuest::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    eGObjPropertySaveStatus DynamicQuest::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuest::GetReward() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& DynamicQuest::GetHirerName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DynamicQuest::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuest::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::UpdateBeforeShowInConversation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuest::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuest::GetTargetObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::Registration()
    {
        m_propertiesMap["PrototypeId"] = 2;
        m_propertiesSaveStatesMap[2] = SAVE_PROP_NEVER;
        m_propertiesMap["Belong"] = 0;
        m_propertiesSaveStatesMap[0] = SAVE_PROP_NEVER;
        m_propertiesMap["targetName"] = 57;
        m_propertiesMap["hirerName"] = 58;
        m_propertiesMap["status"] = 59;
        m_propertiesMap["reward"] = 60;
    }

    DynamicQuest::QuestStatus DynamicQuest::GetQuestStatus() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuest::~DynamicQuest()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::_OnHirerBecamesEnemyWithPlayer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DynamicQuest::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::_SetStatus(QuestStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::_OnCreate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::_OnTake()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool DynamicQuest::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuest::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::_OnObjectEntersLocation(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuest::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::_OnObjectDie(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuest::_OnRelationChanged(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

#include "dynamicquest.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(DynamicQuest, Take)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DynamicQuest, GetReward)
{
    throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    bool DynamicQuestPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int DynamicQuest::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool DynamicQuest::CanChildBeAdded(m3d::Class*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DynamicQuest::DynamicQuest(DynamicQuestPrototypeInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DynamicQuestPrototypeInfo const* DynamicQuest::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr DynamicQuest::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    GameTime const& DynamicQuest::GetTakeGameTime() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr const& DynamicQuest::GetTargetName() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int DynamicQuest::GetHirerObjId() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::Take()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::PassToAnotherMap()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuest::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    eGObjPropertySaveStatus DynamicQuest::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int DynamicQuest::GetReward() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr const& DynamicQuest::GetHirerName() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool DynamicQuest::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int DynamicQuest::OnEvent(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::UpdateBeforeShowInConversation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* DynamicQuest::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int DynamicQuest::GetTargetObjId() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    DynamicQuest::~DynamicQuest()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::_OnHirerBecamesEnemyWithPlayer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool DynamicQuest::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::_SetStatus(QuestStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::_OnCreate()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::_OnTake()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool DynamicQuest::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuest::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::_OnObjectEntersLocation(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* DynamicQuest::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::_OnObjectDie(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DynamicQuest::_OnRelationChanged(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }
}

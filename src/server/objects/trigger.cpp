#include "trigger.h"

#include <stdexcept>

#include "core/aiparam.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, AddEvent)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, DelEvent)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, DelEventObj)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetTriggeredObjectAmount)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetTriggeredObjectID)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, AddTriggeredObjectID)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCount)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, IncCount)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, IsActivated)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, Activate)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, Deactivate)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallEvent)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallObjName)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallObjId)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, Var)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, SetVar)
{
    throw std::logic_error("Not implemented");
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Trigger)
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, AddEvent, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, DelEvent, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, DelEventObj, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, GetTriggeredObjectAmount, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, GetTriggeredObjectID, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, AddTriggeredObjectID, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, GetCount, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, IncCount, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, IsActivated, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, Activate, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, Deactivate, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, GetCallEvent, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, GetCallObjName, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, GetCallObjId, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, Var, "", "", "")
        RT_CLASS_EXPORT(Trigger, m3d::METHOD, SetVar, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Trigger);

    Obj* TriggerPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    Trigger::CallEventInfo::CallEventInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::Deactivate()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Trigger::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    int Trigger::GetTriggeredObjectAmount() const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::DelEvent(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Trigger::GetTriggeredObjectID(int) const
    {
        throw std::logic_error("Not implemented");
    }

    int Trigger::GetCount() const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::ActivateIfNeeded()
    {
        throw std::logic_error("Not implemented");
    }

    int Trigger::IsActivated() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Trigger::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Trigger::NeedCinematicUpdate()
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::AddTriggeredObjectID(int)
    {
        throw std::logic_error("Not implemented");
    }

    char const* Trigger::GetCallObjName() const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::Activate()
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Trigger::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    int Trigger::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    Trigger::Trigger(TriggerPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Trigger::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Trigger::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    int Trigger::IncCount()
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::SetVar(char const*, m3d::AIParam&)
    {
        throw std::logic_error("Not implemented");
    }

    char const* Trigger::GetCallEvent() const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::LoadFromMapXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Trigger::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Trigger::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Trigger::Var(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    TriggerPrototypeInfo const* Trigger::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::DelEventObj(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::AddEvent(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Trigger::GetCallObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Trigger::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Trigger::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    Trigger::~Trigger()
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_LoadTriggerRuntimesFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_OnTargetReachedOrObjectDie(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Trigger::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_StoreCallEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_OnObjectChangesLocation(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_LoadScriptFromMapXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_OnFramesPassed(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    CStr Trigger::_EmbedTriggerBody(CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_SaveTriggerRuntimesToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_OnCinematicFly(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Trigger::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_OnTimePeriod(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_OnDefaultEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_OnCinemaMessage(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Trigger::_LoadEventsFromMapXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }
}

#include "trigger.h"
#include "base/prototypemanager.h"

#include <stdexcept>

#include "core/aiparam.h"
#include <core/kernel.h>
#include <script/scriptserver.h>
#include <script/funcstack.h>
#include <core/log.h>

#include "core/ini.h"
#include "core/ref_ptr.h"
#include <server/processmanager.h>
#include "base/objcontainer.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, AddEvent)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x8588F0
    trigger->AddEvent(context->asString(1), context->asString(2));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, DelEvent)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x858040
    trigger->DelEvent(context->asString(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, DelEventObj)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x858930
    trigger->DelEventObj(context->asString(1), context->asString(2));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetTriggeredObjectAmount)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x855E70
    context->pushInt(trigger->GetTriggeredObjectAmount());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetTriggeredObjectID)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x857060
    context->pushInt(trigger->GetTriggeredObjectID(context->asInt(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, AddTriggeredObjectID)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x857550
    trigger->AddTriggeredObjectID(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCount)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x855880
    context->pushInt(trigger->GetCount());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, IncCount)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x8558B0
    context->pushInt(trigger->IncCount());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, IsActivated)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x8558E0
    context->pushInt(trigger->IsActivated());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, Activate)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    trigger->Activate();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, Deactivate)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    trigger->Deactivate();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallEvent)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x855EC0
    context->pushString(trigger->GetCallEvent());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallObjName)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x855F00
    context->pushString(trigger->GetCallObjName());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallObjId)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    // RVA 0x855910
    context->pushInt(trigger->GetCallObjId());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, Var)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    auto* str = context->asString(1);
    auto aiParam = trigger->Var(str);
    context->pushAIParam(aiParam);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, SetVar)
{
    auto* trigger = (ai::Trigger *)context->asObject(0, "Trigger");
    auto& aiParam = context->asAIParam(2);
    auto* str = context->asString(1);
    trigger->SetVar(str, aiParam);
    return 1;
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
        return new Trigger(*this);
    }

    Trigger::CallEventInfo::CallEventInfo()
    {
        this->m_eventId = GE_UNKNOWN;
        this->m_callObjId = -1;
    }

    void Trigger::Deactivate()
    {
        this->m_Count = 0;
        this->m_state = TS_OFF;
        for (auto& info : m_eventInfos)
        {
            if (info.m_eventId != GE_TIME_PERIOD)
            {
                auto* obj = theObjects->GetEntityByObjName(info.m_objName);
                if (obj)
                {
                    theProcessManager->PostMessageA(3, obj->GetId(), GetId(), 0.0, { info.m_eventId}, {}, 1);
                }
            }
        }

        CauseEvent(GE_OBJECT_DEACTIVATED, 0.0, {}, {});
    }

    m3d::Class* Trigger::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    int Trigger::GetTriggeredObjectAmount() const
    {
        // RVA 0x855970
        return static_cast<int>(m_ObjIDs.size());
    }

    void Trigger::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8559F0
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("CanUpdate", CStr(m_bCanUpdate).c_str());
    }

    void Trigger::DelEvent(char const* eventName)
    {
        // RVA 0x857E60 - drops every subscription to that event, whatever object it named.
        eGameEvent const eventId = theProcessManager->GetEventId(CStr(eventName));
        if (eventId == GE_UNKNOWN)
        {
            return;
        }
        for (auto it = m_eventInfos.begin(); it != m_eventInfos.end();)
        {
            if (it->m_eventId == eventId)
            {
                it = m_eventInfos.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    int Trigger::GetTriggeredObjectID(int num) const
    {
        // RVA 0x856960
        if (num < 0 || num >= static_cast<int>(m_ObjIDs.size()))
        {
            return -1;
        }
        return m_ObjIDs[num];
    }

    int Trigger::GetCount() const
    {
        // RVA 0x855260
        return m_Count;
    }

    void Trigger::ActivateIfNeeded()
    {
        if (m_triggerScriptFuncName.empty())
        {
            m_state = TS_OFF;
        }

        if (m_state != TS_OFF)
        {
            Activate();
        }
    }

    int Trigger::IsActivated() const
    {
        // RVA 0x855280 - anything but TS_OFF counts as live.
        return m_state != TS_OFF;
    }

    bool Trigger::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x8552B0 - the override exists only to be in the vtable; a trigger owns no
        // properties of its own beyond the two the base class already handles.
        return Obj::SetPropertyById(propertyId, newValue);
    }

    bool Trigger::NeedCinematicUpdate()
    {
        return true;
    }

    void Trigger::AddTriggeredObjectID(int objId)
    {
        // RVA 0x8574F0 - NOTE: no check for the id already being in the list.
        m_ObjIDs.push_back(objId);
    }

    char const* Trigger::GetCallObjName() const
    {
        // RVA 0x8559B0
        return m_callEvent.m_objName.c_str();
    }

    void Trigger::Activate()
    {
        // RVA 0x858A90
        m_state = TS_EVENTWAIT;
        for (auto& eventInfo : m_eventInfos)
        {
            if (eventInfo.m_eventId == GE_TIME_PERIOD)
            {
                ai::Event ev;
                ev.m_recipientObjId = GetId();
                ev.m_senderObjId = ev.m_recipientObjId;
                ev.m_eventId = GE_TIME_PERIOD;
                ev.m_timeOut = m_timeOutForTimePeriod;
                ev.m_framesToPass = 1;
                theProcessManager->PostMessageA(ev);
            }
            else if (eventInfo.m_eventId == GE_FRAMES_PASSED)
            {
                ai::Event ev;
                ev.m_framesToPass = this->m_framesForFramesPassed;
                ev.m_eventId = GE_FRAMES_PASSED;
                ev.m_recipientObjId = GetId();
                ev.m_senderObjId = GetId();
                ev.m_timeOut = -1.0;
                theProcessManager->PostMessageA(ev);
            }
            else
            {
                auto* entity = theObjects->GetEntityByObjName(eventInfo.m_objName);
                if (entity)
                {
                    // The event being subscribed to travels as an AIPARAM_ID.
                    m3d::AIParam param(eventInfo.m_eventId);
                    theProcessManager->PostMessageA(GE_SUBSCRIBE, entity->GetId(), GetId(), 0.0, param, {}, 1);
                }
                else
                {
                    M3D_LOG_ERR("***************** TRIGGER ERROR *******************");
                    M3D_LOG_ERR("Error: object " + eventInfo.m_objName + " can't subscribe for " + GetName());
                }
            }
        }
        CauseEvent(GE_OBJECT_ACTIVATED, 0.0, {}, {});
    }

    void Trigger::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x857100 - the script variables are written as a child element each, then the
        // trigger's own runtime state as attributes.
        Obj::SaveToXML(xmlFile, xmlNode);
        for (auto const& variable : m_variables)
        {
            ref_ptr varNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Variable");
            xmlNode->AddChild(varNode);
            varNode->SetAttribute("name", variable.first.c_str());
            variable.second.SaveToXML(xmlFile, varNode);
        }
        _SaveTriggerRuntimesToXML(xmlFile, xmlNode);
    }

    void Trigger::Registration()
    {
        m_propertiesMap["PrototypeId"] = 2;
        m_propertiesSaveStatesMap[2] = SAVE_PROP_NEVER;
        m_propertiesMap["Belong"] = 0;
        m_propertiesSaveStatesMap[0] = SAVE_PROP_NEVER;
    }

    eGObjPropertySaveStatus Trigger::GetPropertySaveStatus(int id) const
    {
        // RVA 0x85A070
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertySaveStatus(id);
    }

    int Trigger::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x85A000
        auto const it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertyId(PropertyName);
    }

    Trigger::Trigger(TriggerPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        this->m_callEvent.m_eventId = GE_UNKNOWN;
        this->m_callEvent.m_callObjId = -1;
        this->m_state = TS_EVENTWAIT;
        this->m_timeOutForTimePeriod = 1.0;
        this->m_framesForFramesPassed = 0;
        this->m_idForCinemaMsg = -1;
        this->m_Count = 0;
        this->m_bScriptPresent = 0;
        this->m_StateKeep = 0;
        this->m_bCanUpdate = 0;
    }

    Trigger::Trigger(const ai::Trigger&)
    {
        // NOTE: the header declares this but the shipped build never emitted it - a trigger is
        // only ever created from its prototype - so its body does not come from the binary.
    }

    void Trigger::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x859CC0
        Obj::LoadFromXML(xmlFile, xmlNode);

        ref_ptr varNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(varNode, "Variable"); !varNode->IsEmpty();
             varNode->GetNextSibling(varNode, "Variable"))
        {
            CStr const name(varNode->GetAttribute("name"));
            if (!name.empty())
            {
                m_variables[name].LoadFromXML(xmlFile, varNode);
            }
        }
        _LoadTriggerRuntimesFromXML(xmlFile, xmlNode);
    }

    int Trigger::OnEvent(Event const& evn)
    {
        Obj::OnEvent(evn);
        int result = 0;
        switch (evn.m_eventId)
        {
        case GE_OBJECT_ENTERS_LOCATION:
        case GE_OBJECT_LEAVES_LOCATION:
        case GE_OBJECT_IN_LOCATION:
            ai::Trigger::_OnObjectChangesLocation(evn);
            result = 1;
            break;
        case GE_OBJECT_DIE:
        case GE_OBJECT_DIE_SENSE:
        case GE_TARGET_REACHED:
            ai::Trigger::_OnTargetReachedOrObjectDie(evn);
            result = 1;
            break;
        case GE_TIME_PERIOD:
            ai::Trigger::_OnTimePeriod(evn);
            result = 1;
            break;
        case GE_FRAMES_PASSED:
            ai::Trigger::_OnFramesPassed(evn);
            result = 1;
            break;
        case GE_START_CINEMATIC_MSG:
            ai::Trigger::_OnCinemaMessage(evn);
            result = 1;
            break;
        case GE_START_CINEMATIC_FLY:
            ai::Trigger::_OnCinematicFly(evn);
            result = 1;
            break;
        default:
            ai::Trigger::_OnDefaultEvent(evn);
            result = 1;
            break;
        }
        return result;
    }

    bool Trigger::CanChildBeAdded(m3d::Class*) const
    {
        // RVA 0x859FE0 - a trigger is a leaf; it never takes children.
        return false;
    }

    int Trigger::IncCount()
    {
        // RVA 0x855270
        return ++m_Count;
    }

    void Trigger::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8559C0
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_bCanUpdate, xmlNode, "CanUpdate");
    }

    void Trigger::SetVar(char const* name, m3d::AIParam& var)
    {
        m_variables[name] = var;
    }

    char const* Trigger::GetCallEvent() const
    {
        // RVA 0x855990
        return theProcessManager->GetEventName(m_callEvent.m_eventId).c_str();
    }

    void Trigger::LoadFromMapXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        if (!m_StateKeep)
        {
            CStr active = xmlNode->GetAttribute("active");
            if (active == "0")
            {
                m_state = TS_OFF;
            }
        }
        _LoadEventsFromMapXML(xmlFile, xmlNode);
        _LoadScriptFromMapXML(xmlFile, xmlNode);
    }

    void Trigger::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x85A130
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    void Trigger::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x85A0B0
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    m3d::Class* Trigger::GetClass() const
    {
        return RT_CLASS_LOCAL(Trigger);
    }

    CStr Trigger::GetPropertyName(int id) const
    {
        // RVA 0x85A1B0
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    m3d::AIParam Trigger::Var(char const* varName)
    {
        return m_variables[varName];
    }

    TriggerPrototypeInfo const* Trigger::GetPrototypeInfo() const
    {
        // RVA 0x8570D0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), TriggerPrototypeInfo const);
    }

    void Trigger::DelEventObj(char const* eventName, char const* objName)
    {
        // RVA 0x858180 - drops just the subscription naming that object, and tells the object it
        // is no longer being listened to.
        eGameEvent const eventId = theProcessManager->GetEventId(CStr(eventName));
        if (eventId == GE_UNKNOWN)
        {
            return;
        }

        for (auto it = m_eventInfos.begin(); it != m_eventInfos.end();)
        {
            // A time-period event has no object name to match, so it always goes.
            if (it->m_eventId == eventId &&
                (eventId == GE_TIME_PERIOD || strcmp(it->m_objName.c_str(), objName) == 0))
            {
                it = m_eventInfos.erase(it);
            }
            else
            {
                ++it;
            }
        }

        Obj* const obj = theObjects->GetEntityByObjName(CStr(objName));
        if (obj)
        {
            theProcessManager->PostMessageA(GE_UNSUBSCRIBE, obj->GetId(), GetId(), 0.0f,
                m3d::AIParam(static_cast<int>(eventId)), {}, 1);
        }
    }

    void Trigger::AddEvent(char const* eventName, char const* param)
    {
        // RVA 0x858070 - a time-period event carries its interval in the same string that every
        // other event uses for an object name.
        eGameEvent const eventId = theProcessManager->GetEventId(CStr(eventName));
        if (eventId == GE_TIME_PERIOD)
        {
            m_timeOutForTimePeriod = m3d::AIParam(CStr(param)).GetAsFloat();
        }
        else if (eventId == GE_UNKNOWN)
        {
            return;
        }

        auxEventInfo eventInfo;
        eventInfo.m_eventId = eventId;
        eventInfo.m_objName = param;
        m_eventInfos.push_back(eventInfo);
    }

    int Trigger::GetCallObjId() const
    {
        // RVA 0x855290
        return m_callEvent.m_callObjId;
    }

    void Trigger::Update(float elapsedTime, unsigned int workTime)
    {
        if (elapsedTime > 0.001)
        {
            m_bCanUpdate = true;
        }
        if (m_bCanUpdate && m_state == TS_ACTION && m_bScriptPresent)
        {
            m_state = TS_EVENTWAIT;
            auto& scriptServer = M3D_KERNEL->GetScriptServer();

            m3d::sArgStack argStack;
            auto* in = argStack.newIn();
            in->SetO(this);

            const auto error = scriptServer.callScriptFunc(m_triggerScriptFuncName.c_str(), argStack, 0);
            if (error != m3d::SUCCESS)
            {
                const auto desc = scriptServer.getFormatedScriptErrorDesc(error);
                M3D_LOG_INFO("***************** TRIGGER ERROR *******************");
                M3D_LOG_INFO(desc);
                m_bScriptPresent = false;
            }
            m_ObjIDs.clear();
        }
    }

    bool Trigger::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x8552A0
        return Obj::_GetPropertyInternal(propertyId, retVal);
    }

    bool Trigger::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x8552C0
        return Obj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    void Trigger::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x857E00 - SAVE_PROP_NORMAL is the default and is not recorded.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    Trigger::~Trigger() = default;

    void Trigger::_LoadTriggerRuntimesFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x857580 - m_StateKeep records whether the file actually named a state, so that a
        // trigger loaded from a map without one is not held to a stale value.
        CStr state(xmlNode->GetAttribute("State"));
        m_StateKeep = true;
        if (state == "TS_EVENTWAIT")
        {
            m_state = TS_EVENTWAIT;
        }
        else if (state == "TS_ACTION")
        {
            m_state = TS_ACTION;
        }
        else if (state == "TS_OFF")
        {
            m_state = TS_OFF;
        }
        else
        {
            m_StateKeep = false;
        }

        m3d::SafeIntAttrib(m_Count, xmlNode, "Count");
        m3d::SafeFloatAttrib(m_timeOutForTimePeriod, xmlNode, "Pause");
        m3d::SafeStrAttrib(m_flyPathForCinematicFly, xmlNode, "FlyPath");
        m3d::SafeIntAttrib(m_idForCinemaMsg, xmlNode, "IdForCinema");

        int framesToPass = 0;
        if (m3d::SafeIntAttrib(framesToPass, xmlNode, "FramesToPass") && framesToPass >= 0)
        {
            m_framesForFramesPassed = framesToPass;
        }

        CStr objectIds(xmlNode->GetAttribute("ObjectIDs"));
        m_ObjIDs.clear();
        if (!objectIds.empty())
        {
            // strtok writes into the string as it goes, as the shipped build does to the CStr's
            // own buffer; objectIds outlives the parsing.
            char* const raw = const_cast<char*>(objectIds.c_str());
            for (char* tok = strtok(raw, " "); tok; tok = strtok(nullptr, " "))
            {
                int const objId = atoi(tok);
                if (objId != -1)
                {
                    m_ObjIDs.push_back(objId);
                }
            }
        }

        int callEventId = m_callEvent.m_eventId;
        m3d::SafeIntAttrib(callEventId, xmlNode, "CallEventId");
        m_callEvent.m_eventId = static_cast<eGameEvent>(callEventId);
        m3d::SafeStrAttrib(m_callEvent.m_objName, xmlNode, "CallObjName");
        m3d::SafeIntAttrib(m_callEvent.m_callObjId, xmlNode, "CallObjId");
    }

    void Trigger::_OnTargetReachedOrObjectDie(Event const& evn)
    {
        // RVA 0x857840 - the object that fired is remembered whether or not the trigger is in a
        // state to act on it, so a script can ask afterwards what set it off.
        if (std::find(m_ObjIDs.begin(), m_ObjIDs.end(), evn.m_senderObjId) == m_ObjIDs.end())
        {
            m_ObjIDs.push_back(evn.m_senderObjId);
        }
        if (m_state == TS_EVENTWAIT)
        {
            m_state = TS_ACTION;
            _StoreCallEvent(evn);
        }
    }

    m3d::Object* Trigger::Clone()
    {
        // RVA 0x855F30
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void Trigger::_StoreCallEvent(Event const& evn)
    {
        m_callEvent.m_eventId = evn.m_eventId;
        auto* senderObj = theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (senderObj)
        {
            m_callEvent.m_objName = senderObj->GetName();
            // NOTE: the EVENT id is stored in the object-id field, so GetCallObjId reports an
            // event id rather than the sender. Verified against RVA 0x857210.
            m_callEvent.m_callObjId = evn.m_eventId;
        }
        else
        {
            m_callEvent.m_objName = "Unknown";
            m_callEvent.m_callObjId = -1;
        }
    }

    void Trigger::_OnObjectChangesLocation(Event const& evn)
    {
        auto id = evn.m_param1.GetAsID();
        auto it = std::find(m_ObjIDs.begin(), m_ObjIDs.end(), id);
        if (it == m_ObjIDs.end())
        {
            m_ObjIDs.push_back(id);
        }
        if (m_state == TS_EVENTWAIT)
        {
            m_state = TS_ACTION;
            _StoreCallEvent(evn);
        }
    }

    void Trigger::_LoadScriptFromMapXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        ref_ptr scriptNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(scriptNode, "script");
        if (!scriptNode->IsEmpty())
        {
            m_triggerScriptFuncName = "trigger" + m_name;

            ref_ptr scriptBody = xmlFile->CreateNode();
            if (scriptNode->GetFirstChild(scriptBody, nullptr))
            {
                CStr scriptValue = scriptBody->GetValue();

                auto embedBody = _EmbedTriggerBody(scriptValue, m_name);
                auto& scriptServer = M3D_KERNEL->GetScriptServer();

                if (auto res = scriptServer.execute(embedBody.c_str(), m_triggerScriptFuncName.c_str()))
                {
                    M3D_LOG_ERR(scriptServer.getFormatedScriptErrorDesc(res));
                }
                else
                {
                    m_bScriptPresent = true;
                }
            }
        }
    }

    void Trigger::_OnFramesPassed(Event const& evn)
    {
        // RVA 0x8587B0 - re-arms itself for the next interval before firing, so a frames-passed
        // trigger repeats.
        if (m_state != TS_EVENTWAIT)
        {
            return;
        }
        Event ev;
        ev.m_eventId = GE_FRAMES_PASSED;
        ev.m_recipientObjId = GetId();
        ev.m_senderObjId = GetId();
        ev.m_timeOut = -1.0f;
        ev.m_framesToPass = m_framesForFramesPassed;
        theProcessManager->PostMessageA(ev);

        m_state = TS_ACTION;
        _StoreCallEvent(evn);
    }

    CStr Trigger::_EmbedTriggerBody(CStr const& scriptCode, CStr const& triggerName)
    {
        return "function trigger" + triggerName + "( trigger )\n" + scriptCode + "\nend";
    }

    void Trigger::_SaveTriggerRuntimesToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x856B20
        CStr stateName;
        switch (m_state)
        {
            case TS_EVENTWAIT: stateName = "TS_EVENTWAIT"; break;
            case TS_ACTION: stateName = "TS_ACTION"; break;
            case TS_OFF: stateName = "TS_OFF"; break;
        }
        xmlNode->SetAttribute("State", stateName.c_str());
        xmlNode->SetAttribute("Count", CStr(m_Count).c_str());
        xmlNode->SetAttribute("Pause", CStr(m_timeOutForTimePeriod).c_str());
        xmlNode->SetAttribute("FlyPath", m_flyPathForCinematicFly.c_str());
        xmlNode->SetAttribute("IdForCinema", CStr(m_idForCinemaMsg).c_str());
        xmlNode->SetAttribute("FramesToPass", CStr(m_framesForFramesPassed).c_str());

        if (!m_ObjIDs.empty())
        {
            CStr objectIds;
            for (unsigned i = 0; i < m_ObjIDs.size(); ++i)
            {
                objectIds += CStr(m_ObjIDs[i]) + CStr(" ");
            }
            // The trailing separator is trimmed back off.
            xmlNode->SetAttribute(
                "ObjectIDs", objectIds.substr(0, objectIds.length() - 1).c_str());
        }

        xmlNode->SetAttribute("CallEventId", CStr(m_callEvent.m_eventId).c_str());
        xmlNode->SetAttribute("CallObjName", m_callEvent.m_objName.c_str());
        xmlNode->SetAttribute("CallObjId", CStr(m_callEvent.m_callObjId).c_str());
    }

    void Trigger::_OnCinematicFly(Event const& evn)
    {
        // RVA 0x8572F0 - only the fly-by this trigger was told to watch for counts.
        if (m_state != TS_EVENTWAIT || evn.m_param1.GetAsStr() != m_flyPathForCinematicFly)
        {
            return;
        }
        m_state = TS_ACTION;
        _StoreCallEvent(evn);
    }

    m3d::Object* Trigger::CreateObject()
    {
        // RVA 0x8560F0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void Trigger::_OnTimePeriod(Event const& evn)
    {
        if (m_state == TS_EVENTWAIT)
        {
            Event ev;
            ev.m_recipientObjId = GetId();
            ev.m_senderObjId = ev.m_recipientObjId;
            ev.m_eventId = GE_TIME_PERIOD;
            ev.m_timeOut = m_timeOutForTimePeriod;
            ev.m_framesToPass = 1;
            theProcessManager->PostMessageA(ev);
            m_state = TS_ACTION;
            _StoreCallEvent(evn);
        }
    }

    void Trigger::_OnDefaultEvent(Event const& evn)
    {
        if (evn.m_param1.GetType())
        {
            m_ObjIDs.push_back(evn.m_param1.GetAsID());
        }
        if (evn.m_param1.GetType())
        {
            m_ObjIDs.push_back(evn.m_param2.GetAsID());
        }
        if (this->m_state == TS_EVENTWAIT)
        {
            this->m_state = TS_ACTION;
            ai::Trigger::_StoreCallEvent(evn);
        }
    }

    void Trigger::_OnCinemaMessage(Event const& evn)
    {
        // RVA 0x857380
        if (m_state != TS_EVENTWAIT || evn.m_param1.GetAsID() != m_idForCinemaMsg)
        {
            return;
        }
        m_state = TS_ACTION;
        _StoreCallEvent(evn);
    }

    void Trigger::_LoadEventsFromMapXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        ref_ptr eventNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(eventNode, "event"); !eventNode->IsEmpty(); eventNode->GetNextSibling(eventNode, "event"))
        {
            CStr eventName = eventNode->GetAttribute("eventid");
            auto eventId = theProcessManager->GetEventId(eventName);
            switch(eventId)
            {
            case GE_TIME_PERIOD:
                m3d::SafeFloatAttrib(m_timeOutForTimePeriod, eventNode, "timeout");
                break;
            case GE_FRAMES_PASSED:
                m3d::SafeUintAttrib(m_framesForFramesPassed, eventNode, "numframes");
                break;
            case GE_START_CINEMATIC_MSG:
                m3d::SafeIntAttrib(m_idForCinemaMsg, eventNode, "msgid");
                break;
            case GE_START_CINEMATIC_FLY:
                m3d::SafeStrAttrib(m_flyPathForCinematicFly, eventNode, "flypath");
                break;
            }

            if (this->m_timeOutForTimePeriod < 0.0)
                this->m_timeOutForTimePeriod = 0.0;
            if (!this->m_framesForFramesPassed)
                this->m_framesForFramesPassed = 1;

            if (eventId)
            {
                auxEventInfo eventInfo;
                eventInfo.m_objName = eventNode->GetAttribute("ObjName");
                eventInfo.m_eventId = eventId;
                m_eventInfos.push_back(std::move(eventInfo));
            }
        }
    }
}

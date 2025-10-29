#include "trigger.h"

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
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, DelEvent)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, DelEventObj)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetTriggeredObjectAmount)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetTriggeredObjectID)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, AddTriggeredObjectID)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCount)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, IncCount)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, IsActivated)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, Activate)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, Deactivate)
{
    auto* trigger = (ai::Trigger*)context->asObject(0, "Trigger");
    trigger->Deactivate();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallEvent)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallObjName)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Trigger, GetCallObjId)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::DelEvent(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Trigger::GetTriggeredObjectID(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Trigger::GetCount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Trigger::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Trigger::NeedCinematicUpdate()
    {
        return true;
    }

    void Trigger::AddTriggeredObjectID(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    char const* Trigger::GetCallObjName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::Activate()
    {
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
                    // TODO: check this
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

    void Trigger::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::Registration()
    {
        m_propertiesMap["PrototypeId"] = 2;
        m_propertiesSaveStatesMap[2] = SAVE_PROP_NEVER;
        m_propertiesMap["Belong"] = 0;
        m_propertiesSaveStatesMap[0] = SAVE_PROP_NEVER;
    }

    eGObjPropertySaveStatus Trigger::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Trigger::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Trigger::IncCount()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::SetVar(char const* name, m3d::AIParam& var)
    {
        m_variables[name] = var;
    }

    char const* Trigger::GetCallEvent() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void Trigger::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Trigger::GetClass() const
    {
        return RT_CLASS_LOCAL(Trigger);
    }

    CStr Trigger::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam Trigger::Var(char const* varName)
    {
        return m_variables[varName];
    }

    TriggerPrototypeInfo const* Trigger::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::DelEventObj(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::AddEvent(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Trigger::GetCallObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RETRUXX_DLL_INJECT_VIRTUAL_FUNCITON_NAMESPACED(0x008569A0, ai, Trigger, Update)
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

    bool Trigger::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Trigger::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Trigger::~Trigger()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::_LoadTriggerRuntimesFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::_OnTargetReachedOrObjectDie(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Trigger::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::_StoreCallEvent(Event const& evn)
    {
        m_callEvent.m_eventId = evn.m_eventId;
        auto* senderObj = theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (senderObj)
        {
            m_callEvent.m_objName = senderObj->GetName();
            // TODO: check this
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

    void Trigger::_OnFramesPassed(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Trigger::_EmbedTriggerBody(CStr const& scriptCode, CStr const& triggerName)
    {
        return "function trigger" + triggerName + "( trigger )\n" + scriptCode + "\nend";
    }

    void Trigger::_SaveTriggerRuntimesToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Trigger::_OnCinematicFly(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Trigger::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void Trigger::_OnCinemaMessage(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

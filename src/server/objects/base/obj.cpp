#include "obj.h"
#include "objcontainer.h"
#include "prototypeinfo.h"
#include "prototypemanager.h"
#include <algorithm>
#include <core/aiparam.h>
#include <core/kernel.h>
#include <server/event.h>
#include <server/ipricecoeffprovider.h>
#include <server/modifier.h>
#include <server/processmanager.h>
#include <server/ai/ai.h>
#include <server/ai/aimanager.h>

namespace ai
{
    extern ObjContainer* theObjects;
    extern AIManager* theAIManager;
    extern ProcessManager* theProcessManager;
    extern PrototypeManager* thePrototypeManager;

    std::map<CStr, int> Obj::m_propertiesMap;
    std::map<int, eGObjPropertySaveStatus> Obj::m_propertiesSaveStatesMap;

    m3d::Class* Obj::GetBaseClass()
    {
        return &m_classObject;
    }

    void Obj::Registration()
    {
        theAIManager->RegisterFunc("AIGetCmdParam1", AIGetCmdParam1);
        theAIManager->RegisterFunc("AIGetCmdParam2", AIGetCmdParam2);
        theAIManager->RegisterFunc("AIGetCmdParam3", AIGetCmdParam3);
        theAIManager->RegisterFunc("AIGetState1Param1", AIGetState1Param1);
        theAIManager->RegisterFunc("AIGetState1Param2", AIGetState1Param2);
        theAIManager->RegisterFunc("AIGetState1Param3", AIGetState1Param3);
        theAIManager->RegisterFunc("AIGetState2Param1", AIGetState2Param1);
        theAIManager->RegisterFunc("AIGetState2Param2", AIGetState2Param2);
        theAIManager->RegisterFunc("AIGetState2Param3", AIGetState2Param3);
        theAIManager->RegisterFunc("AIGetParentID", AIGetParentID);
        theAIManager->RegisterFunc("AIGetOwnerID", AIGetOwnerID);

        m_propertiesMap["Belong"] = 0;
        m_propertiesMap["Prototype"] = 1;
        m_propertiesMap["PrototypeId"] = 2;
        m_propertiesMap["Name"] = 3;
    }

    m3d::AIParam Obj::AIGetParentID(Obj* obj)
    {
        return m3d::AIParam(obj->GetParentId());
        
    }

    m3d::AIParam Obj::AIGetCmdParam1(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetCmdParam(0);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetCmdParam2(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetCmdParam(1);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetCmdParam3(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetCmdParam(2);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetMessage1Param1(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetMessage1Param(0);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetMessage2Param1(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetMessage2Param(0);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetState1Param1(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetState1Param(0);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetState1Param2(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetState1Param(1);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetState1Param3(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetState1Param(2);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetState2Param1(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetState2Param(0);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetState2Param2(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetState2Param(1);
        }
        return {};
    }

    m3d::AIParam Obj::AIGetState2Param3(Obj* obj)
    {
        if (auto* ai = obj->GetAIPtr(); ai != nullptr)
        {
            return ai->GetState2Param(2);
        }
        return {};
    }

    void Obj::RegisterProperty(char const* name, int id, eGObjPropertySaveStatus saveStatus)
    {
        m_propertiesMap[name] = id;
        if (saveStatus != SAVE_PROP_NORMAL)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    void Obj::RemoveComponent(Obj* component)
    {
    }

    void Obj::SetBelong(int newBelong)
    {
        //TODO: check tis
        m_belong = newBelong;
        for (auto& [i, child] : m_allChildren)
        {
            child->SetBelong(newBelong);
        }
    }

    int Obj::OnEvent(Event const& ev)
    {
        if (ev.m_eventId == GE_SUBSCRIBE)
        {
            OnSubscribe(ev);
            return 1;
        }
        if (ev.m_eventId == GE_UNSUBSCRIBE)
        {
            OnUnsubscribe(ev);
            return 1;
        }
        return 0;
    }

    bool Obj::CanChildBeAdded(m3d::Class* pClass) const
    {
        return false;
    }

    m3d::Class* Obj::GetRtClass() const
    {
        return &m_classObject;
    }

    void Obj::PostCollide()
    {
    }

    void Obj::SetVisible()
    {
        auto* prototypeInfo = GetPrototypeInfo();
        if (!prototypeInfo || prototypeInfo->m_bIsUpdating)
        {
            theObjects->AddObjToUpdate(this);
        }
        m_flags |= 1;
    }

    void Obj::RenderDebugInfo() const
    {
    }

    void Obj::TransferPhysicParamsToSceneGraphNode()
    {
    }

    void Obj::StackClose()
    {
        if (auto* aiPtr = GetAIPtr())
        {
            aiPtr->CommandStackClose();
        }
    }

    void Obj::StackLoop()
    {
        if (auto* aiPtr = GetAIPtr())
        {
            aiPtr->PutCommand(17, {}, {}, {});
        }
    }

    Obj::Obj(PrototypeInfo const& prototypeInfo) :
        m_bIsUpdating(prototypeInfo.m_bIsUpdating),
        m_prototypeId(prototypeInfo.m_prototypeId)
    {
    }

    void Obj::SetParentRepository(GeomRepository* parentRepository)
    {
        //TODO: check logic
        if (!parentRepository || !m_parentRepository || m_parentRepository == parentRepository)
        {
            SYS_ERROR("!parentRepository || !m_parentRepository || m_parentRepository == parentRepository");
        }
        m_parentRepository = parentRepository;
    }

    float Obj::GetPriceCoeff(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        if (priceCoeffProvider == nullptr)
        {
            return 1.0;
        }
        return priceCoeffProvider->GetPriceCoeffForObj(this);
    }

    void Obj::CauseEvent(eGameEvent eventId, float timeOut, m3d::AIParam param1, m3d::AIParam param2) const
    {
        auto const idx = _GetIndexByEventId(eventId);
        Event ev;
        ev.m_eventId = eventId;
        ev.m_senderObjId = m_objId;
        ev.m_timeOut = timeOut;
        ev.m_framesToPass = 0;
        ev.m_param1 = param1;
        ev.m_param2 = param2;
        if (idx != -1)
        {
            auto const& eventRecipient = m_eventRecipients.at(idx);
            for (auto const recipientObjId : eventRecipient.m_objIds)
            {
                ev.m_recipientObjId = recipientObjId;
                theProcessManager->PostMessageA(ev);
            }
        }
        ev.m_recipientObjId = m_parentId;
        theProcessManager->PostMessageA(ev);
    }

    void Obj::Subscribe(eGameEvent eventId, int objId)
    {
        if (objId != m_parentId)
        {
            if (auto const idx = _GetIndexByEventId(eventId); idx == -1)
            {
                EventRecipientInfo info{eventId, {objId}};
                m_eventRecipients.push_back(std::move(info));
            }
            else
            {
                auto& eventRecipient = m_eventRecipients.at(idx);
                //TODO: check this
                if (std::find(cbegin(eventRecipient.m_objIds), cend(eventRecipient.m_objIds), objId) == cend(eventRecipient.m_objIds))
                {
                    eventRecipient.m_objIds.push_back(objId);
                }
            }
        }
    }

    void Obj::Unsubscribe(eGameEvent eventId, int objId)
    {
        //TODO: check correctness
        if (auto const idx = _GetIndexByEventId(eventId); idx != -1)
        {
            auto& eventRecipient = m_eventRecipients.at(idx);
            auto const it = std::find(cbegin(eventRecipient.m_objIds), cend(eventRecipient.m_objIds), objId);
            if (it != cend(eventRecipient.m_objIds))
            {
                eventRecipient.m_objIds.erase(it);
            }
            if (eventRecipient.m_objIds.empty())
            {
                m_eventRecipients.erase(cbegin(m_eventRecipients) + idx);
            }
        }
    }

    GeomRepository* Obj::GetParentRepository() const
    {
        return m_parentRepository;
    }

    void Obj::PostLoad()
    {
        if (m_bNeedPostLoad)
        {
            _InternalPostLoad();
            m_bNeedPostLoad = false;
        }
    }

    bool Obj::TimeOutFinished()
    {
        if (m_timeOut > 0.0)
        {
            m_timeOut -= theAIManager->m_elapsedTime;
            return false;
        }
        m_flags &= 0xFFFFFFFB;
        return true;
    }

    void Obj::LinkToParent(int newParentId, HierarchyType newHierarchyType)
    {
        auto* parent = GetParent();
        if (parent != nullptr)
        {
            if (m_hierarchyType == HIERARCHY_COMPONENT)
            {
                parent->RemoveComponent(this);
            }
            else
            {
                parent->RemoveChild(this);
            }
        }
        else
        {
            m_parentId = -1;
        }
        m_parentId = newParentId;
        m_hierarchyType = newHierarchyType;
        parent = GetParent();
        if (parent != nullptr)
        {
            SetBelong(parent->GetBelong());
        }
    }

    void Obj::AddModifier(Modifier const& modifier)
    {
        m_modifiers.push_back(modifier);
    }

    void Obj::AddModifier(char const* propertyName, char const* modification)
    {
        Modifier modifier;
        modifier.m_PropertyName = propertyName;
        toLower(modifier.m_PropertyName);
        modifier.m_SenderID = -1;
        modifier.ReadFromStr(modification);
        m_modifiers.push_back(std::move(modifier));
    }

    bool Obj::IsAlive() const
    {
        return
            (m_flags & 8) == 0 &&
            (m_flags & 2) == 0 &&
            GetParentRepository() == nullptr;
    }

    void Obj::CreateVisualPart()
    {
        if (m_bMustCreateVisualPart)
        {
            _InternalCreateVisualPart();
            m_bMustCreateVisualPart = false;
            m_bPassedToAnotherMap = false;
        }
    }

    void Obj::Send(Obj* receiverObj, char const* propertyName, char const* modification)
    {
        if (receiverObj != nullptr)
        {
            Modifier modifier;
            modifier.m_PropertyName = propertyName;
            toLower(modifier.m_PropertyName);
            modifier.m_SenderID = m_objId;
            modifier.ReadFromStr(modification);
            receiverObj->AddModifier(modifier);
        }
    }

    void Obj::_SetAllPropertiesToMax()
    {
    }

    bool Obj::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        switch (propertyId)
        {
        case 0:
        {
            retVal = m_belong;
            return true;
        }
        case 1:
        {
            retVal = thePrototypeManager->GetPrototypeName(m_prototypeId);
            return true;
        }
        case 2:
        {
            retVal = m_prototypeId;
            return true;
        }
        case 3:
        {
            retVal = GetName();
            return true;
        }
        default:
        {
                LOG("Error: getting invalid property", LOG_ERR);
                //TODO: add debug description
                return false;
        }
        }
    }

    void Obj::_InternalPostLoad()
    {
    }

    void Obj::_InternalCreateVisualPart()
    {
    }

    bool Obj::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        switch (propertyId)
        {
        case 0:
        {
            retVal = 1000;
            return true;
        }
        case 1:
        {
            retVal =
                m_prototypeId != -1 ?
                thePrototypeManager->GetPrototypeName(m_prototypeId) :
                m3d::AIParam{};
            return true;
        }
        case 2:
        {
            retVal = -1;
            return true;
        }
        case 3:
        {
            retVal = m3d::AIParam{};
            return true;
        }
        default:
        {
            LOG("Error: getting invalid default property", LOG_ERR);
            //TODO: add debug description
            return false;
        }
        }
    }

    int Obj::_GetIndexByEventId(eGameEvent eventId) const
    {
        //TODO: check correctness
        auto const it = std::find_if(cbegin(m_eventRecipients), cend(m_eventRecipients), [eventId](auto const& info)
        {
            return eventId == info.m_eventId;
        });
        if (it != cend(m_eventRecipients))
        {
            return std::distance(cbegin(m_eventRecipients), it);
        }
        return -1;
    }
}

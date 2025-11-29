#include "obj.h"
#include "objcontainer.h"
#include "prototypeinfo.h"
#include "prototypemanager.h"
#include <algorithm>
#include <stdexcept>
#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <server/damageinfo.h>
#include <server/event.h>
#include <server/ipricecoeffprovider.h>
#include <server/modifier.h>
#include <server/processmanager.h>
#include <server/ai/ai.h>
#include <server/ai/aimanager.h>

#include "core/ini.h"
#include "core/ref_ptr.h"
#include <server/server.h>

#include "server/affix.h"
#include <server/resourcemanager.h>

#include "game/m3dgame.h"
#include "server/dynamicscene.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetId)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    auto id = obj->GetId();
    context->pushInt(id);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, Remove)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->Remove();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, IsAlive)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetChild)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, AddChild)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    auto* child = (ai::Obj*)context->asObject(1, "Obj");
    obj->AddChild(child);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, AddModifier)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, Send)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetParent)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetBelong)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, StackOpen)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->StackOpen();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, StackClose)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->StackClose();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, StackLoop)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->StackLoop();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetProperty)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetPropertyId)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetPropertyById)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetProperty)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    const auto* prop = context->asString(1);
    const auto& aiParam = context->asAIParam(2);
    obj->SetProperty(prop, aiParam);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetPropertyById)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, ApplyAffixByName)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetBelong)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetNameFromScript)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetSchwarz)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    //std::map<CStr, int> Obj::m_propertiesMap;
    //std::map<int, eGObjPropertySaveStatus> Obj::m_propertiesSaveStatesMap;

    RT_CLASS_EXPORTS_BEGIN(Obj)
        RT_CLASS_EXPORT(Obj, m3d::METHOD, GetId, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, Remove, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, IsAlive, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, GetChild, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, AddChild, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, AddModifier, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, Send, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, GetParent, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, GetBelong, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, StackOpen, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, StackClose, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, StackLoop, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, GetProperty, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, GetPropertyId, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, GetPropertyById, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, SetProperty, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, SetPropertyById, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, ApplyAffixByName, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, SetBelong, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, SetNameFromScript, "", "", "")
        RT_CLASS_EXPORT(Obj, m3d::METHOD, GetSchwarz, "", "", "")
	RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Obj);


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

    m3d::AIParam Obj::AIGetOwnerID(Obj* obj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        m3d::SafeUintAttrib(m_flags, xmlNode, "Flags");

        ref_ptr runTimeNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(runTimeNode, "Runtime");
        m3d::SafeUintAttrib(m_flags, runTimeNode, "Flags");

        ref_ptr attr = xmlNode->CreateAttribute();
        for (xmlNode->GetFirstAttribute(attr); !attr->IsEmpty(); attr->GetNextSibling(attr))
        {
            auto name = attr->GetName();
            if (stricmp(name, "Prototype"))
            {
                m3d::AIParam param(attr->GetValue());
                auto propId = GetPropertyId(name);
                if (propId != -1)
                {
                    SetPropertyById(propId, param);
                }
            }
        }

        m_bAffixesWasApplied = false;
        m3d::SafeBoolAttrib(m_bAffixesWasApplied, xmlNode, "AffixesWasApplied");

        ref_ptr prefixNode = xmlFile->CreateNode();
        m_appliedPrefixIds.clear();
        for (xmlNode->GetFirstChild(prefixNode, "Prefix"); !prefixNode->IsEmpty(); prefixNode->GetNextSibling(prefixNode, "Prefix"))
        {
            CStr name;
            m3d::SafeStrAttrib(name, prefixNode, "name");

            CStr res;
            m3d::SafeStrAttrib(res, prefixNode, "targetResource");

            auto id = pServer->GetAffixManager()->GetAffixIdByNameAndResource(name, theResourceManager->GetResourceId(res));
            if (id == -1)
            {
                M3D_LOG_ERR("Error: trying to apply invalid prefix: name = '" + name + "', resource = '" + res + "' to " + GetDebugDescription());
            }
            m_appliedPrefixIds.push_back(id);
        }

        ref_ptr suffixNode = xmlFile->CreateNode();
        m_appliedSuffixIds.clear();
        for (xmlNode->GetFirstChild(suffixNode, "Suffix"); !suffixNode->IsEmpty(); suffixNode->GetNextSibling(suffixNode, "Suffix"))
        {
            CStr name;
            m3d::SafeStrAttrib(name, suffixNode, "name");

            CStr res;
            m3d::SafeStrAttrib(res, suffixNode, "targetResource");

            auto id = pServer->GetAffixManager()->GetAffixIdByNameAndResource(name, theResourceManager->GetResourceId(res));
            if (id == -1)
            {
                M3D_LOG_ERR("Error: trying to apply invalid suffix: name = '" + name + "', resource = '" + res + "' to " + GetDebugDescription());
            }
            m_appliedSuffixIds.push_back(id);
        }

        if (!runTimeNode->IsEmpty())
        {
            LoadRuntimeValues(xmlFile, runTimeNode);
        }

        ref_ptr objNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(objNode, "Object"); !objNode->IsEmpty(); objNode->GetNextSibling(objNode, "Suffix"))
        {
            if (objNode->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
            {
                retruxx::vector<m3d::Class*> classes;
                auto objId = gDynamicScene->ReadNewObjectFromXml(xmlFile, objNode, classes);
                if (objId >= 0)
                {
                    auto obj = theObjects->GetEntityByObjId(objId);
                    if (obj)
                    {
                        AddChild(obj);
                    }
                }
            }
        }
    }

    Obj* Obj::CloneObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::RemoveChild(Obj* pChild)
    {
        m_allChildren.erase(pChild->GetId());
        return false;
    }

    int Obj::RemoveChild(m3d::Object*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::ApplyModifier(Modifier const& modifier)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PrototypeInfo const* Obj::GetPrototypeInfo() const
    {
        return thePrototypeManager->GetPrototypeInfo(m_prototypeId);
    }

    void Obj::CreateChildren()
    {
    }

    void Obj::ReceiveNodesToLink(retruxx::list<m3d::SgNode*>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::bIsEqualToPrototype() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::Dump() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Obj::GetPrice(IPriceCoeffProvider const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::AddChild(Obj* pObj)
    {
        if (pObj)
        {
            if (CanChildBeAdded(pObj->GetClass()))
            {
                m_allChildren[pObj->GetId()] = pObj;
            }
        }
        else
        {
            M3D_LOG_INFO("Warning: null object is added to " + GetDebugDescription());
        }
    }

    int Obj::AddChild(m3d::Object*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam Obj::GetPropertyDefaultById(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (!propertyId)
        {
            SetBelong(newValue.GetAsID());
            return true;
        }
        if (propertyId == 3)
        {
            theObjects->SetObjName(m_objId, newValue.GetAsStr());
            return true;
        }

        M3D_LOG_ERR("Error: setting invalid property " + CStr(propertyId) + " to " + GetDebugDescription());
        return false;
    }

    int Obj::GetProperty(unsigned, void*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam Obj::GetProperty(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* Obj::GetParent() const
    {
        return theObjects->GetEntityByObjId(m_parentId);
    }

    void Obj::Remove()
    {
        _SetDeadStatus();
        m_flags |= 2u;
        if (m_objId != -1)
        {
            ai::theObjects->AddObjIdToRemove(m_objId);
        }
    }

    bool Obj::NeedCinematicUpdate()
    {
        return (m_flags >> 4) & 1;
    }

    m3d::AIParam Obj::GetPropertyDefault(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* Obj::GetChild(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::ClearSavedStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Obj::GetSchwarz() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Obj::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::StackOpen()
    {
        if (auto ai = GetAIPtr())
        {
            ai->CommandStackOpen();
        }
    }

    void Obj::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam Obj::GetPropertyById(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Obj::SetProperty(unsigned, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Obj::GetPropertyId(char const* propName) const
    {
        auto it = Obj::m_propertiesMap.find(propName);
        if (it != Obj::m_propertiesMap.end())
        {
            return it->second;
        }

        return -1;
    }

    CStr Obj::GetFullDescriptionWithAffixes() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::bIsEnemyWith(const Obj* pObj) const
    {
        bool result = false;
        if (pObj)
        {
            const auto flags = pObj->GetFlags();
            if ((flags & 8) == 0 && (flags & 2) == 0 && !pObj->GetParentRepository())
            {
                auto v3 = m_flags;
                if ((v3 & 8) == 0 && (v3 & 2) == 0 && !this->m_parentRepository && (pObj->m_flags & 1) != 0 && (this->m_flags & 1) != 0 &&
                    theRelationship->CheckTolerance(this->m_belong, pObj->m_belong) <= RS_ENEMY)
                {
                    return true;
                }
            }
        }
        return result;
    }

    CStr Obj::GetDebugDescription() const
    {
        const auto* prototypeInfo = GetPrototypeInfo();
        return "name = '" + CStr(GetName()) + "', prototype name = '" + prototypeInfo->m_prototypeName + "', class name = '" + CStr(GetClassNameA()) + "', id = '" + CStr(GetId()) + "'";
    }

    void Obj::ValidateEventRecipientsList()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::ApplyAffixByName(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::ApplyAffix(Affix const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::ApplyRandomAffixes(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Obj::GetFlags() const
    {
        return m_flags;
    }

    bool Obj::bIsVisible() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::bHasParent() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::IsAffixesApplied() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Obj::GetId() const
    {
        return m_objId;
    }

    int Obj::TimeOutActivated() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Obj::GetBelong() const
    {
        return this->m_belong;
    }

    void Obj::RemoveFromCinematic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::SetTimeOut(float timeOut)
    {
        this->m_flags |= 4u;
        this->m_timeOut = timeOut;
    }

    int Obj::GetParentId() const
    {
        return this->m_parentId;
    }

    void Obj::SetAffixesApplied(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::GetDeletedStatus() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Obj::GetPrototypeId() const
    {
        return this->m_prototypeId;
    }

    retruxx::map<int, Obj*>& Obj::GetChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::SetName(CStr const& str)
    {
        theObjects->SetObjName(m_objId, str);
    }

    void Obj::UnlinkFromParent()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::SetParentInvalid()
    {
        m_parentId = -1;
    }

    void Obj::SetNameFromScript(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Obj::IsUpdating() const
    {
        return m_bIsUpdating;
    }

    bool Obj::GetPassedToAnotherMapStatus() const
    {
        return m_bPassedToAnotherMap;
    }

    void Obj::AddToCinematic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::StopTimeOut()
    {
        m_flags &= ~4u;
        m_timeOut = -1.0;
    }

    bool Obj::SetProperty(char const* PropertyName, m3d::AIParam const& newValue)
    {
        const auto propFromObj = GetPropertyId(PropertyName);
        if (propFromObj != -1)
        {
            return SetPropertyById(propFromObj, newValue);
        }
        return false;
    }

    retruxx::map<int, Obj*>& Obj::getAllChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj::Obj()
    {
        this->m_parentRepository = 0;
        this->m_bPassedToAnotherMap = 0;
        this->m_hierarchyType = HIERARCHY_CHILD;
        this->m_bIsAlreadySaved = 0;
        this->m_bAffixesWasApplied = 0;
        this->m_prototypeId = -1;
        this->m_flags = 17;
        this->m_bNeedPostLoad = 1;
        this->m_bMustCreateVisualPart = 1;
        this->m_parentId = -1;
        this->m_belong = 1000;
        this->m_objId = -1;
        this->m_updatingObjId = -1;
        this->m_bIsUpdating = 1;
        this->m_bMustBeUpdating = 1;
        this->m_timeOut = 0.0;
        this->m_LastDamageSource = -1;
    }

    bool Obj::_GetDeadStatus() const
    {
        return (this->m_flags & 8) != 0;
    }

    int Obj::GetLastDamageSource() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::_SetDeadStatus()
    {
        m_flags |= 8;
        if ((m_flags & 8) == 0)
        {
            M3D_APP->ImmediateMessage(66542, (int)this, 0, 0, 0, {}, {});
            CauseEvent(GE_OBJECT_DIE, 0.0, { GetId() }, {});
        }
    }

    void Obj::SetLastDamageSource(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj::~Obj()
    {
    }

    void Obj::_Init()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Obj::OnUnsubscribe(Event const& evn)
    {
        M3D_ASSERT(evn.m_param1.GetType() == m3d::AIPARAM_ID);
        Obj::Unsubscribe(static_cast<eGameEvent>(evn.m_param1.GetAsID()), evn.m_senderObjId);
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

    void Obj::GetPropertiesIDs(retruxx::set<int>& props) const
    {
        for (auto const& prop : m_propertiesMap)
        {
            props.insert(prop.second);
        }
    }

    void Obj::SetInvisible()
    {
        theObjects->AddObjToNotUpdate(this);
        m_flags &= 0xFFFFFFFE;
    }

    void Obj::InflictDamage(DamageInfo const& damageInfo)
    {
        m3d::AIParam const value(damageInfo.damage);
        Modifier modToHealth;
        modToHealth.Create("hp", MO_SUB, value);
        modToHealth.m_SenderID = damageInfo.attackerId;
        m_modifiers.push_back(modToHealth);
        m_LastDamageSource = damageInfo.attackerId;
    }

    void Obj::Update(float elapsedTime, unsigned workTime)
    {
        for (auto const& modifier : m_modifiers)
        {
            ApplyModifier(modifier);
        }
        //TODO: check tis
        m_modifiers.resize(0, {});
    }

    void Obj::GetPropertiesNames(retruxx::set<CStr>& props) const
    {
        for (auto const& prop : m_propertiesMap)
        {
            props.insert(prop.first);
        }
    }

    void Obj::RelinkSceneGraphNode()
    {
    }

    AI* Obj::GetAIPtr()
    {
        return nullptr;
    }

    CStr Obj::GetPropertyName(int id) const
    {
        //TODO: check correctness
        auto const it = std::find_if(std::begin(m_propertiesMap), std::end(m_propertiesMap), [id](auto const& prop)
        {
            return prop.second == id;
        });
        if (it != std::end(m_propertiesMap))
        {
            return it->first;
        }
        return {};
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
        this->m_parentRepository = 0;
        this->m_bPassedToAnotherMap = 0;
        this->m_hierarchyType = HIERARCHY_CHILD;
        this->m_bIsAlreadySaved = 0;
        this->m_bAffixesWasApplied = 0;
        this->m_prototypeId = -1;
        this->m_bIsUpdating = 1;
        this->m_flags = 17;
        this->m_bNeedPostLoad = 1;
        this->m_bMustCreateVisualPart = 1;
        this->m_parentId = -1;
        this->m_belong = 1000;
        this->m_objId = -1;
        this->m_updatingObjId = -1;
        this->m_bMustBeUpdating = 1;
        this->m_timeOut = 0.0;
        this->m_LastDamageSource = -1;
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
                EventRecipientInfo info{};
                info.m_eventId = eventId;
                info.m_objIds.push_back(objId);
                m_eventRecipients.push_back(info);
            }
            else
            {
                auto& eventRecipient = m_eventRecipients.at(idx);
                //TODO: check this
                if (std::find(std::begin(eventRecipient.m_objIds), std::end(eventRecipient.m_objIds), objId) == std::end(eventRecipient.m_objIds))
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
            auto const it = std::find(std::begin(eventRecipient.m_objIds), std::end(eventRecipient.m_objIds), objId);
            if (it != std::end(eventRecipient.m_objIds))
            {
                eventRecipient.m_objIds.erase(it);
            }
            if (eventRecipient.m_objIds.empty())
            {
                m_eventRecipients.erase(std::begin(m_eventRecipients) + idx);
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
        modifier.m_PropertyName.toLower();
        modifier.m_SenderID = -1;
        modifier.ReadFromStr(modification);
        m_modifiers.push_back(modifier);
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
            modifier.m_PropertyName.toLower();
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
            M3D_LOG_ERR("Error: getting invalid property");
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
            M3D_LOG_ERR("Error: getting invalid default property");
            //TODO: add debug description
            return false;
        }
        }
    }

    void Obj::OnSubscribe(Event const& evn)
    {
        M3D_ASSERT(evn.m_param1.GetType() == m3d::AIPARAM_ID);
        Subscribe(static_cast<eGameEvent>(evn.m_param1.GetAsID()), evn.m_senderObjId);
    }

    int Obj::_GetIndexByEventId(eGameEvent eventId) const
    {
        //TODO: check correctness
        auto const it = std::find_if(std::begin(m_eventRecipients), std::end(m_eventRecipients), [eventId](auto const& info)
        {
            return eventId == info.m_eventId;
        });
        if (it != std::end(m_eventRecipients))
        {
            return std::distance(std::begin(m_eventRecipients), it);
        }
        return -1;
    }
}

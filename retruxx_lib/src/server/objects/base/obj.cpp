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
#include "thirdparty/injecttools.h"
#include <server/server.h>

#include "server/affix.h"
#include <server/resourcemanager.h>

#include "server/dynamicscene.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, Remove)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, IsAlive)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetChild)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, AddChild)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, AddModifier)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, Send)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetParent)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetBelong)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, StackOpen)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, StackClose)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, StackLoop)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetProperty)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetPropertyId)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetPropertyById)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetProperty)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetPropertyById)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, ApplyAffixByName)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetBelong)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetNameFromScript)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetSchwarz)
{
    throw std::logic_error("Not implemented");
}

namespace ai
{
    extern ObjContainer* theObjects;
    extern AIManager* theAIManager;
    extern ProcessManager* theProcessManager;
    extern PrototypeManager* thePrototypeManager;

    //std::map<CStr, int> Obj::m_propertiesMap;
    //std::map<int, eGObjPropertySaveStatus> Obj::m_propertiesSaveStatesMap;

    RT_CLASS_EXPORTS_BEGIN(Obj)
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Obj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    int Obj::RemoveChild(m3d::Object*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::ApplyModifier(Modifier const& modifier)
    {
        throw std::logic_error("Not implemented");
    }

    PrototypeInfo const* Obj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::CreateChildren()
    {
    }

    void Obj::ReceiveNodesToLink(retruxx::list<m3d::SgNode*>&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::bIsEqualToPrototype() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::Dump() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Obj::GetPrice(IPriceCoeffProvider const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::AddChild(Obj* pObj)
    {
        if (pObj)
        {
            throw std::logic_error("Not implemented");
        }
        else
        {
            M3D_LOG_INFO("Warning: null object is added to " + GetDebugDescription());
        }
    }

    int Obj::AddChild(m3d::Object*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Obj::GetPropertyDefaultById(int) const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Obj::GetProperty(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* Obj::GetParent() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::NeedCinematicUpdate()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Obj::GetPropertyDefault(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* Obj::GetChild(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::ClearSavedStatus()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Obj::GetSchwarz() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Obj::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::StackOpen()
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Obj::GetPropertyById(int) const
    {
        throw std::logic_error("Not implemented");
    }

    int Obj::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    bool Obj::bIsEnemyWith(Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Obj::GetDebugDescription() const
    {
        // TODO: implement Obj::GetDebugDescription
        //throw std::logic_error("Not implemented");
        return {};
    }

    void Obj::ValidateEventRecipientsList()
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::ApplyAffixByName(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::ApplyAffix(Affix const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::ApplyRandomAffixes(int)
    {
        throw std::logic_error("Not implemented");
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x00602610, Obj::GetFlags)
    unsigned Obj::GetFlags() const
    {
        return m_flags;
    }

    bool Obj::bIsVisible() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::bHasParent() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::IsAffixesApplied() const
    {
        throw std::logic_error("Not implemented");
    }

    int Obj::GetId() const
    {
        return m_objId;
    }

    int Obj::TimeOutActivated() const
    {
        throw std::logic_error("Not implemented");
    }

    int Obj::GetBelong() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::RemoveFromCinematic()
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::SetTimeOut(float)
    {
        throw std::logic_error("Not implemented");
    }

    int Obj::GetParentId() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::SetAffixesApplied(bool)
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::GetDeletedStatus() const
    {
        throw std::logic_error("Not implemented");
    }

    int Obj::GetPrototypeId() const
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::map<int, Obj*>& Obj::GetChildren()
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::SetName(CStr const& str)
    {
        theObjects->SetObjName(m_objId, str);
    }

    void Obj::UnlinkFromParent()
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::SetParentInvalid()
    {
        this->m_parentId = -1;
    }

    void Obj::SetNameFromScript(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::IsUpdating() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::GetPassedToAnotherMapStatus() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::AddToCinematic()
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::StopTimeOut()
    {
        throw std::logic_error("Not implemented");
    }

    bool Obj::SetProperty(char const*, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::map<int, Obj*>& Obj::getAllChildren()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    int Obj::GetLastDamageSource() const
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::_SetDeadStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::SetLastDamageSource(int)
    {
        throw std::logic_error("Not implemented");
    }

    Obj::~Obj()
    {
    }

    void Obj::_Init()
    {
        throw std::logic_error("Not implemented");
    }

    void Obj::OnUnsubscribe(Event const&)
    {
        throw std::logic_error("Not implemented");
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

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x006894A0, Obj::GetParentRepository)
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

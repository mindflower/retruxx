#include "obj.h"
#include "objcontainer.h"
#include "server/objects/ware.h"
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
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushBool(obj->IsAlive());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetChild)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushObject(obj->GetChild(context->asInt(1)));
    return 1;
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
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->AddModifier(context->asString(1), context->asString(2));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, Send)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->Send((ai::Obj*)context->asObject(1, "Obj"), context->asString(2), context->asString(3));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetParent)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushObject(obj->GetParent());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetBelong)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushInt(obj->GetBelong());
    return 1;
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
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushAIParam(obj->GetProperty(context->asString(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetPropertyId)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushInt(obj->GetPropertyId(context->asString(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetPropertyById)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushAIParam(obj->GetPropertyById(context->asInt(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetProperty)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    auto const* prop = context->asString(1);
    auto const& aiParam = context->asAIParam(2);
    obj->SetProperty(prop, aiParam);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetPropertyById)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->SetPropertyById(context->asInt(1), context->asAIParam(2));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, ApplyAffixByName)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushBool(obj->ApplyAffixByName(context->asString(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetBelong)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->SetBelong(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, SetNameFromScript)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    obj->SetNameFromScript(CStr(context->asString(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Obj, GetSchwarz)
{
    auto* obj = (ai::Obj*)context->asObject(0, "Obj");
    context->pushInt(obj->GetSchwarz());
    return 1;
}

namespace ai
{
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
        // RVA 0x691900 - walks up to the topmost object, so a gun on a vehicle reports the
        // vehicle rather than its immediate mount.
        // NOTE: nothing checks the walk, so a dangling parent id crashes here rather than
        // stopping at the last object that still exists.
        while (obj->m_parentId != -1)
        {
            obj = theObjects->GetEntityByObjId(obj->m_parentId);
        }
        return m3d::AIParam(obj->m_objId);
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
        for (xmlNode->GetFirstChild(prefixNode, "Prefix"); !prefixNode->IsEmpty();
             prefixNode->GetNextSibling(prefixNode, "Prefix"))
        {
            CStr name;
            m3d::SafeStrAttrib(name, prefixNode, "name");

            CStr res;
            m3d::SafeStrAttrib(res, prefixNode, "targetResource");

            auto id =
                pServer->GetAffixManager()->GetAffixIdByNameAndResource(name, theResourceManager->GetResourceId(res));
            if (id == -1)
            {
                M3D_LOG_ERR(
                    "Error: trying to apply invalid prefix: name = '" + name + "', resource = '" + res + "' to " +
                    GetDebugDescription());
            }
            m_appliedPrefixIds.push_back(id);
        }

        ref_ptr suffixNode = xmlFile->CreateNode();
        m_appliedSuffixIds.clear();
        for (xmlNode->GetFirstChild(suffixNode, "Suffix"); !suffixNode->IsEmpty();
             suffixNode->GetNextSibling(suffixNode, "Suffix"))
        {
            CStr name;
            m3d::SafeStrAttrib(name, suffixNode, "name");

            CStr res;
            m3d::SafeStrAttrib(res, suffixNode, "targetResource");

            auto id =
                pServer->GetAffixManager()->GetAffixIdByNameAndResource(name, theResourceManager->GetResourceId(res));
            if (id == -1)
            {
                M3D_LOG_ERR(
                    "Error: trying to apply invalid suffix: name = '" + name + "', resource = '" + res + "' to " +
                    GetDebugDescription());
            }
            m_appliedSuffixIds.push_back(id);
        }

        if (!runTimeNode->IsEmpty())
        {
            LoadRuntimeValues(xmlFile, runTimeNode);
        }

        ref_ptr objNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(objNode, "Object"); !objNode->IsEmpty(); objNode->GetNextSibling(objNode, "Object"))
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
        // RVA 0x68D260 - there is no copy constructor for a game object, so a clone is made by
        // saving this one to a scratch document and loading a fresh entity back out of it.
        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        ref_ptr xmlNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "hack");
        xmlFile->AddChild(xmlNode);

        auto const oldSaveType = theObjects->m_SaveType;
        theObjects->m_SaveType = ObjContainer::SAVE_LEVEL;

        ClearSavedStatus();
        SaveToXML(xmlFile, xmlNode);

        int const objId =
            theObjects->CreateEntityForLoad(m_prototypeId, (CStr("Clone_") + m_name).c_str(), -1, -1);
        auto* clone = theObjects->GetEntityByObjId(objId);
        if (!clone)
        {
            return nullptr;
        }

        clone->LoadFromXML(xmlFile, xmlNode);

        // CreateEntityForLoad leaves the object half-built, so the two deferred steps that the
        // container would normally run are done here for the clone and each of its children.
        if (clone->m_bNeedPostLoad)
        {
            clone->_InternalPostLoad();
            clone->m_bNeedPostLoad = false;
        }
        if (clone->m_bMustCreateVisualPart)
        {
            clone->_InternalCreateVisualPart();
            clone->m_bMustCreateVisualPart = false;
            clone->m_bPassedToAnotherMap = false;
        }

        for (auto const& child : clone->m_allChildren)
        {
            auto* childObj = child.second;
            if (!childObj)
                continue;

            if (childObj->m_bNeedPostLoad)
            {
                childObj->_InternalPostLoad();
                childObj->m_bNeedPostLoad = false;
            }
            if (childObj->m_bMustCreateVisualPart)
            {
                childObj->_InternalCreateVisualPart();
                childObj->m_bMustCreateVisualPart = false;
                childObj->m_bPassedToAnotherMap = false;
            }
        }

        theObjects->m_SaveType = oldSaveType;
        return clone;
    }

    void Obj::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x692BD0 - the counterpart of SaveRuntimeValues.
        m3d::SafeFloatAttrib(m_timeOut, xmlNode, "TimeOut");

        ref_ptr recipientsNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(recipientsNode, "EventRecipients");
        if (!recipientsNode->IsEmpty())
        {
            ref_ptr eventNode = xmlFile->CreateNode();
            for (recipientsNode->GetFirstChild(eventNode, "Event"); !eventNode->IsEmpty();
                 eventNode->GetNextSibling(eventNode, "Event"))
            {
                EventRecipientInfo recipient;
                recipient.m_eventId = theProcessManager->GetEventId(CStr(eventNode->GetAttribute("EventId")));

                CStr objects;
                m3d::SafeStrAttrib(objects, eventNode, "Objects");

                // strtok writes into the string, so it runs over the CStr's own buffer.
                CStr buf = objects;
                for (char* token = strtok(const_cast<char*>(buf.c_str()), " ,"); token;
                     token = strtok(nullptr, " ,"))
                {
                    recipient.m_objIds.push_back(atoi(token));
                }

                m_eventRecipients.push_back(recipient);
            }
        }

        // Whatever the prototype set up is replaced wholesale by what the save holds.
        m_modifiers.clear();

        ref_ptr modifierNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(modifierNode, "Modifier"); !modifierNode->IsEmpty();
             modifierNode->GetNextSibling(modifierNode, "Modifier"))
        {
            Modifier modifier;
            modifier.LoadFromXML(xmlFile, modifierNode);
            m_modifiers.push_back(modifier);
        }

        m3d::SafeIntAttrib(m_LastDamageSource, xmlNode, "LastDamageSource");
    }

    bool Obj::RemoveChild(Obj* pChild)
    {
        m_allChildren.erase(pChild->GetId());
        return false;
    }

    int Obj::RemoveChild(m3d::Object*)
    {
        // RVA 0x68B6E0
        // NOTE: the m3d::Object interface is not supported on game objects - the Obj* and
        // AIParam overloads are the ones to use. Reports success anyway.
        M3D_ASSERT(0);
        return 1;
    }

    bool Obj::ApplyModifier(Modifier const& modifier)
    {
        // RVA 0x68A0A0 - the base class understands one property, "belong";
        // derived objects override this and chain up for anything they do not
        // recognise themselves. Returning false means "not mine".
        if (!(modifier.m_PropertyName == "belong"))
        {
            return false;
        }

        // The modifier arithmetic is all in floats, so the side goes through a
        // float and back. The current value doubles as the base, which makes
        // MO_BACK a no-op here.
        float belong = static_cast<float>(m_belong);
        modifier.Apply(&belong, belong);

        if (static_cast<int>(belong) != m_belong)
        {
            SetBelong(static_cast<int>(belong));
        }
        return true;
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
        // RVA 0x5FD660 - empty; only objects that own scene graph nodes override this.
    }

    bool Obj::bIsEqualToPrototype() const
    {
        // RVA 0x690B30 - true while nothing the save file would record has been changed.
        retruxx::set<int> properties;
        GetPropertiesIDs(properties);

        for (int const propertyId : properties)
        {
            // NOTE: property id 0 is skipped outright, and anything not saved normally is
            // ignored - only the plain properties count towards being "unchanged".
            if (!propertyId)
                continue;

            if (GetPropertySaveStatus(propertyId) != SAVE_PROP_NORMAL)
                continue;

            m3d::AIParam value = GetPropertyById(propertyId);
            m3d::AIParam defaultValue = GetPropertyDefaultById(propertyId);
            if (!(value == defaultValue))
                return false;
        }
        return true;
    }

    void Obj::Dump() const
    {
        // RVA 0x689580 - empty in the shipped build; subclasses do the printing.
    }

    unsigned Obj::GetPrice(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        // RVA 0x68A120
        float const price = GetPrototypeInfo()->GetBasePrice() * GetPriceCoeff(priceCoeffProvider);
        return GetIntPrice(price);
    }

    void Obj::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x68C380 - only what a running game has accumulated: a pending timeout, who hurt
        // this object last, who is listening to its events, and any modifiers laid on it.
        if (fabs(m_timeOut) > 0.001)
        {
            xmlNode->SetAttribute("TimeOut", CStr(m_timeOut).c_str());
        }

        if (m_LastDamageSource != -1)
        {
            xmlNode->SetAttribute("LastDamageSource", CStr(m_LastDamageSource).c_str());
        }

        if (!m_eventRecipients.empty())
        {
            ref_ptr recipientsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "EventRecipients");
            xmlNode->AddChild(recipientsNode);

            for (auto const& recipient : m_eventRecipients)
            {
                // An event nobody is listening to is not worth a row.
                if (recipient.m_objIds.empty())
                    continue;

                CStr strObjIds;
                for (unsigned i = 0; i < recipient.m_objIds.size(); ++i)
                {
                    if (i)
                    {
                        strObjIds += CStr(",");
                    }
                    strObjIds += CStr(recipient.m_objIds[i]);
                }

                ref_ptr eventNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Event");
                eventNode->SetAttribute("EventId", theProcessManager->GetEventName(recipient.m_eventId).c_str());
                eventNode->SetAttribute("Objects", strObjIds.c_str());
                recipientsNode->AddChild(eventNode);
            }
        }

        for (auto const& modifier : m_modifiers)
        {
            ref_ptr modifierNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Modifier");
            modifier.SaveToXML(xmlFile, modifierNode);
            xmlNode->AddChild(modifierNode);
        }
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
        // RVA 0x68B520
        // NOTE: the m3d::Object interface is not supported on game objects - the Obj* and
        // AIParam overloads are the ones to use. Reports success anyway.
        M3D_ASSERT(0);
        return 1;
    }

    m3d::AIParam Obj::GetPropertyDefaultById(int propertyId) const
    {
        m3d::AIParam res;
        _GetPropertyDefaultInternal(propertyId, res);
        return res;
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
        // RVA 0x68BA60
        // NOTE: the m3d::Object interface is not supported on game objects - the Obj* and
        // AIParam overloads are the ones to use. Reports success anyway.
        M3D_ASSERT(0);
        return 1;
    }

    m3d::AIParam Obj::GetProperty(char const* PropertyName) const
    {
        // RVA 0x691040
        // NOTE: whether the property was found is discarded, so an unknown name simply reads
        // back as an empty AIParam.
        m3d::AIParam retVal;
        _GetPropertyInternal(GetPropertyId(PropertyName), retVal);
        return retVal;
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

    m3d::AIParam Obj::GetPropertyDefault(char const* PropertyName) const
    {
        // RVA 0x6910B0 - see GetProperty above.
        m3d::AIParam retVal;
        _GetPropertyDefaultInternal(GetPropertyId(PropertyName), retVal);
        return retVal;
    }

    Obj* Obj::GetChild(int) const
    {
        // RVA 0x68B360 - children are addressed by object id, not by index.
        M3D_ASSERT(0);
        return nullptr;
    }

    void Obj::ClearSavedStatus()
    {
        // RVA 0x68D510 - the whole subtree has to be savable again, not just this object.
        m_bIsAlreadySaved = false;
        for (auto const& child : m_allChildren)
        {
            if (child.second)
            {
                child.second->ClearSavedStatus();
            }
        }
    }

    unsigned Obj::GetSchwarz() const
    {
        // RVA 0x6895E0 - the price with no trader's coefficient applied.
        return GetPrice(nullptr);
    }

    void Obj::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x68EEC0 - writes this object and everything below it. Only properties that
        // differ from the prototype are recorded, which is what keeps level files small.
        // The default flags are visible and not-in-a-cinematic, so they are left implicit.
        if (m_flags != 17)
        {
            xmlNode->SetAttribute("Flags", CStr(static_cast<int>(m_flags)).c_str());
        }

        M3D_ASSERT(theObjects->GetEntityByObjId(GetId()) == this);

        // Shared objects can be reached down more than one branch of the tree; ClearSavedStatus
        // resets this before each save.
        if (m_bIsAlreadySaved)
        {
            M3D_CRITICAL_ERROR(m_name + " is already saved to XML");
            return;
        }
        m_bIsAlreadySaved = true;

        if (theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            xmlNode->SetAttribute("ObjectId", CStr(m_objId).c_str());
        }

        CStr const name(m_name.c_str());
        if (!name.empty())
        {
            xmlNode->SetAttribute("Name", name.c_str());
        }

        retruxx::set<int> properties;
        GetPropertiesIDs(properties);
        for (int const propertyId : properties)
        {
            m3d::AIParam value = GetPropertyById(propertyId);

            auto const saveStatus = GetPropertySaveStatus(propertyId);
            if (saveStatus == SAVE_PROP_NORMAL)
            {
                // Unchanged from the prototype, so the loader can work it out again.
                m3d::AIParam defaultValue = GetPropertyDefaultById(propertyId);
                if (value == defaultValue)
                    continue;
            }
            else if (saveStatus != SAVE_PROP_ALWAYS)
            {
                continue;
            }

            CStr const propertyName = GetPropertyName(propertyId);
            if (!propertyName.empty())
            {
                xmlNode->SetAttribute(propertyName.c_str(), value.GetAsStr().c_str());
            }
        }

        if (m_bAffixesWasApplied)
        {
            xmlNode->SetAttribute("AffixesWasApplied", CStr(static_cast<int>(m_bAffixesWasApplied)).c_str());
        }

        for (int const prefixId : m_appliedPrefixIds)
        {
            auto const* affix = pServer->GetAffixManager()->GetAffixById(prefixId);
            if (!affix)
                continue;

            ref_ptr prefixNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Prefix");
            prefixNode->SetAttribute("name", affix->m_name.c_str());
            prefixNode->SetAttribute("targetResource",
                theResourceManager->GetResourceName(affix->m_pAffixGroup->GetTargetResourceId()).c_str());
            xmlNode->AddChild(prefixNode);
        }

        for (int const suffixId : m_appliedSuffixIds)
        {
            auto const* affix = pServer->GetAffixManager()->GetAffixById(suffixId);
            if (!affix)
                continue;

            ref_ptr suffixNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Suffix");
            suffixNode->SetAttribute("name", affix->m_name.c_str());
            suffixNode->SetAttribute("targetResource",
                theResourceManager->GetResourceName(affix->m_pAffixGroup->GetTargetResourceId()).c_str());
            xmlNode->AddChild(suffixNode);
        }

        for (auto const& child : m_allChildren)
        {
            // A child already handed over to another map is that map's to save.
            if (child.second->m_bPassedToAnotherMap)
                continue;

            ref_ptr childNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Object");
            child.second->SaveToXML(xmlFile, childNode);
            xmlNode->AddChild(childNode);
        }

        // Running state is only of interest to a real save, not to a level file.
        if (theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            ref_ptr runtimeNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Runtime");
            xmlNode->AddChild(runtimeNode);
            SaveRuntimeValues(xmlFile, runtimeNode);
        }
    }

    eGObjPropertySaveStatus Obj::GetPropertySaveStatus(int id) const
    {
        // RVA 0x5ED0B0
        auto const it = m_propertiesSaveStatesMap.find(id);
        return it != m_propertiesSaveStatesMap.end() ? it->second : SAVE_PROP_NORMAL;
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
        // RVA 0x6894C0 - the visual part belongs to the old map, so it has to be rebuilt.
        m_bPassedToAnotherMap = true;
        m_bMustCreateVisualPart = true;
    }

    m3d::AIParam Obj::GetPropertyById(int propertyId) const
    {
        m3d::AIParam res;
        _GetPropertyInternal(propertyId, res);
        return res;
    }

    int Obj::SetProperty(unsigned, void*)
    {
        // RVA 0x68B8A0
        // NOTE: the m3d::Object interface is not supported on game objects - the Obj* and
        // AIParam overloads are the ones to use. Reports success anyway.
        M3D_ASSERT(0);
        return 1;
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
        // RVA 0x68A840 - "<prefixes> <prototype name> <suffixes>", unless the object was given
        // a name of its own, which wins outright.
        CStr fullName = theObjects->GetObjectFullName(m_name);
        if (!fullName.empty())
            return fullName;

        unsigned const localizedForm = thePrototypeManager->GetPrototypeFullNameLocalizedForm(m_prototypeId);
        unsigned long const locale = M3D_APP->GetLocale();

        for (unsigned i = 0; i < m_appliedPrefixIds.size(); ++i)
        {
            auto const* affix = pServer->GetAffixManager()->GetAffixById(m_appliedPrefixIds[i]);
            if (!affix)
                continue;

            CStr prefixName = affix->GetLocalizedName(localizedForm);
            // Only the first one opens the name, so the rest stay lower case.
            if (i)
                prefixName.FirstCharToLower(locale);
            else
                prefixName.FirstCharToUpper(locale);

            fullName += prefixName + CStr(" ");
        }

        CStr prototypeFullName = thePrototypeManager->GetPrototypeFullName(m_prototypeId);
        // NOTE: capitalised unconditionally, so with a prefix in front of it the name ends up
        // with a capital in the middle.
        prototypeFullName.FirstCharToUpper(locale);
        fullName += prototypeFullName;

        for (int const suffixId : m_appliedSuffixIds)
        {
            auto const* affix = pServer->GetAffixManager()->GetAffixById(suffixId);
            if (!affix)
                continue;

            // NOTE: suffixes ask for form 0 rather than the prototype's own form.
            CStr suffixName = affix->GetLocalizedName(0);
            suffixName.FirstCharToLower(locale);
            fullName += CStr(" ") + suffixName;
        }
        return fullName;
    }

    bool Obj::bIsEnemyWith(Obj const* pObj) const
    {
        bool result = false;
        if (pObj)
        {
            auto const flags = pObj->GetFlags();
            if ((flags & 8) == 0 && (flags & 2) == 0 && !pObj->GetParentRepository())
            {
                auto v3 = m_flags;
                if ((v3 & 8) == 0 && (v3 & 2) == 0 && !this->m_parentRepository && (pObj->m_flags & 1) != 0 &&
                    (this->m_flags & 1) != 0 &&
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
        auto const* prototypeInfo = GetPrototypeInfo();
        return "name = '" + CStr(GetName()) + "', prototype name = '" + prototypeInfo->m_prototypeName +
            "', class name = '" + CStr(GetClassNameA()) + "', id = '" + CStr(GetId()) + "'";
    }

    void Obj::ValidateEventRecipientsList()
    {
        // RVA 0x692190 - drops subscribers that have since been destroyed, and then any event
        // left with nobody listening. Both use swap-with-last, so the order is not kept.
        for (unsigned i = 0; i < m_eventRecipients.size();)
        {
            auto& objIds = m_eventRecipients[i].m_objIds;
            for (unsigned j = 0; j < objIds.size();)
            {
                if (theObjects->GetEntityByObjId(objIds[j]))
                {
                    ++j;
                }
                else
                {
                    std::swap(objIds[j], objIds.back());
                    objIds.pop_back();
                }
            }

            if (!objIds.empty())
            {
                ++i;
            }
            else
            {
                std::swap(m_eventRecipients[i], m_eventRecipients.back());
                m_eventRecipients.pop_back();
            }
        }
    }

    bool Obj::ApplyAffixByName(char const* affixName)
    {
        // RVA 0x68DAE0 - affix names are only unique within a resource, so the lookup is keyed
        // by this object's resource as well.
        if (!affixName || !*affixName)
            return false;

        int const affixId = pServer->GetAffixManager()->GetAffixIdByNameAndResource(
            CStr(affixName), GetPrototypeInfo()->m_resourceId);
        if (affixId == -1)
            return false;

        return ApplyAffix(pServer->GetAffixManager()->GetAffixById(affixId));
    }

    bool Obj::ApplyAffix(Affix const* affix)
    {
        // RVA 0x68D890
        auto const* prototypeInfo = GetPrototypeInfo();
        if (prototypeInfo && !prototypeInfo->m_bApplyAffixes)
            return false;

        if (!affix || !affix->ApplyToObj(this))
            return false;

        if (affix->GetAffixType() == AFFIXTYPE_PREFIX)
        {
            m_appliedPrefixIds.push_back(affix->m_affixId);
        }
        else if (affix->GetAffixType() == AFFIXTYPE_SUFFIX)
        {
            m_appliedSuffixIds.push_back(affix->m_affixId);
        }
        else
        {
            // NOTE: an affix of any other type is applied but never recorded, so it will not be
            // written to a save and cannot be named in the object's description.
            M3D_LOG_ERR("Error: unknown affix type " + CStr(static_cast<int>(affix->GetAffixType())));
        }
        return true;
    }

    void Obj::ApplyRandomAffixes(int numAffixes)
    {
        // RVA 0x68D9B0 - one affix from each of numAffixes distinct groups, so the same kind of
        // bonus is never rolled twice on one object.
        auto const* prototypeInfo = GetPrototypeInfo();
        if (prototypeInfo && !prototypeInfo->m_bApplyAffixes)
            return;

        retruxx::vector<int> affixGroupIds;
        pServer->GetAffixManager()->GetAffixGroupsByResourceId(
            GetPrototypeInfo()->m_resourceId, affixGroupIds);

        for (int applied = 0; applied < numAffixes; ++applied)
        {
            if (affixGroupIds.empty())
                break;

            unsigned const groupNum = (affixGroupIds.size() * rand()) >> 15;
            auto* affixGroup = pServer->GetAffixManager()->GetAffixGroupById(affixGroupIds[groupNum]);
            ApplyAffix(pServer->GetAffixManager()->GetAffixById(affixGroup->GetRandomAffixId()));

            // Used up, so it cannot be drawn again.
            affixGroupIds.erase(affixGroupIds.begin() + groupNum);
        }
    }

    unsigned Obj::GetFlags() const
    {
        return m_flags;
    }

    bool Obj::bIsVisible() const
    {
        // RVA 0x532860 - bit 0 of m_flags, the one SetVisible/SetInvisible toggle.
        return (m_flags & 1) != 0;
    }

    bool Obj::bHasParent() const
    {
        // RVA 0x62D700
        return m_parentId != -1;
    }

    bool Obj::IsAffixesApplied() const
    {
        // RVA 0x8737F0
        return m_bAffixesWasApplied;
    }

    int Obj::GetId() const
    {
        return m_objId;
    }

    int Obj::TimeOutActivated() const
    {
        // RVA 0x689520 - bit 2 of m_flags.
        return m_flags & 4;
    }

    int Obj::GetBelong() const
    {
        return this->m_belong;
    }

    void Obj::RemoveFromCinematic()
    {
        // RVA 0x5F1300
        m_flags &= ~0x10u;
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

    void Obj::SetAffixesApplied(bool bApplied)
    {
        // RVA 0x873800
        m_bAffixesWasApplied = bApplied;
    }

    bool Obj::GetDeletedStatus() const
    {
        // RVA 0x4D9960 - bit 1 of m_flags, set by Remove.
        return (m_flags & 2) != 0;
    }

    int Obj::GetPrototypeId() const
    {
        return this->m_prototypeId;
    }

    retruxx::map<int, Obj*>& Obj::GetChildren()
    {
        // RVA 0x689F50
        return m_allChildren;
    }

    void Obj::SetName(CStr const& str)
    {
        theObjects->SetObjName(m_objId, str);
    }

    void Obj::UnlinkFromParent()
    {
        // RVA 0x5CE550: drop out of the parent's child or component list. With no
        // parent there is nothing to remove, so just clear the stale id.
        auto* parent = GetParent();
        if (parent == nullptr)
        {
            m_parentId = -1;
            return;
        }
        if (m_hierarchyType == HIERARCHY_COMPONENT)
        {
            parent->RemoveComponent(this);
        }
        else
        {
            parent->RemoveChild(this);
        }
    }

    void Obj::SetParentInvalid()
    {
        m_parentId = -1;
    }

    void Obj::SetNameFromScript(CStr const& str)
    {
        // RVA 0x689480 - renaming goes through the container, which keeps the name index.
        theObjects->SetObjName(m_objId, str);
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
        // RVA 0x5F12F0
        m_flags |= 0x10u;
    }

    void Obj::StopTimeOut()
    {
        m_flags &= ~4u;
        m_timeOut = -1.0;
    }

    bool Obj::SetProperty(char const* PropertyName, m3d::AIParam const& newValue)
    {
        auto const propFromObj = GetPropertyId(PropertyName);
        if (propFromObj != -1)
        {
            return SetPropertyById(propFromObj, newValue);
        }
        return false;
    }

    retruxx::map<int, Obj*>& Obj::getAllChildren()
    {
        // RVA 0x689590
        return m_allChildren;
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
        // RVA 0x6895A0
        return m_LastDamageSource;
    }

    void Obj::_SetDeadStatus()
    {
        // RVA 0x692760 - only the first call announces the death.
        unsigned const oldFlags = m_flags;
        m_flags |= 8;
        if ((oldFlags & 8) == 0)
        {
            M3D_APP->ImmediateMessage(66542, (int)this, 0, 0, 0, {}, {});
            CauseEvent(GE_OBJECT_DIE, 0.0, {GetId()}, {});
        }
    }

    void Obj::SetLastDamageSource(int objId)
    {
        m_LastDamageSource = objId;
    }

    Obj::~Obj()
    {
    }

    void Obj::_Init()
    {
        // RVA 0x689410 - every object starts unplaced, unowned and needing a post-load.
        m_bNeedPostLoad = true;
        m_bMustCreateVisualPart = true;
        m_bIsUpdating = true;
        m_bMustBeUpdating = true;
        m_prototypeId = -1;
        m_parentRepository = nullptr;
        // Bit 0 visible, bit 4 not in a cinematic.
        m_flags = 17;
        m_bPassedToAnotherMap = false;
        m_parentId = -1;
        m_hierarchyType = HIERARCHY_CHILD;
        m_belong = 1000;
        m_objId = -1;
        m_updatingObjId = -1;
        m_timeOut = 0.0f;
        m_LastDamageSource = -1;
        m_bIsAlreadySaved = false;
        m_bAffixesWasApplied = false;
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
        auto const it = std::find_if(
            std::begin(m_propertiesMap),
            std::end(m_propertiesMap),
            [id](auto const& prop)
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
        M3D_ASSERT(!parentRepository || !m_parentRepository || m_parentRepository == parentRepository);
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
                if (std::find(std::begin(eventRecipient.m_objIds), std::end(eventRecipient.m_objIds), objId) ==
                    std::end(eventRecipient.m_objIds))
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
        // RVA 0x5D4120: the shipped code inlines UnlinkFromParent here.
        UnlinkFromParent();
        m_parentId = newParentId;
        m_hierarchyType = newHierarchyType;
        if (auto* parent = GetParent())
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
        return (m_flags & 8) == 0 && (m_flags & 2) == 0 && GetParentRepository() == nullptr;
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
            retVal = m_prototypeId != -1 ? thePrototypeManager->GetPrototypeName(m_prototypeId) : m3d::AIParam{};
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
        auto const it = std::find_if(
            std::begin(m_eventRecipients),
            std::end(m_eventRecipients),
            [eventId](auto const& info)
            {
                return eventId == info.m_eventId;
            });
        if (it != std::end(m_eventRecipients))
        {
            return std::distance(std::begin(m_eventRecipients), it);
        }
        return -1;
    }
}  // namespace ai

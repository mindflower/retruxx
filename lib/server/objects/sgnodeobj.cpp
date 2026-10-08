#include "sgnodeobj.h"
#include "base/prototypemanager.h"
#include <core/kernel.h>
#include "core/ini.h"
#include "core/aiparam.h"
#include "scene/nodes/sgnode.h"
#include "scene/scenegraph.h"
#include "physicbodies/physicbody.h"

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetPosition)
{
    // RVA 0x82EBD0
    auto* obj = (ai::SgNodeObj*)context->asObject(0, "SgNodeObj");
    obj->SetPosition(context->asVector(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetPosition)
{
    // RVA 0x82F200
    auto* obj = (ai::SgNodeObj*)context->asObject(0, "SgNodeObj");
    context->pushVector(obj->GetPosition());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetRotation)
{
    // RVA 0x82EC00
    auto* obj = (ai::SgNodeObj*)context->asObject(0, "SgNodeObj");
    obj->SetRotation(context->asQuaternion(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetRotation)
{
    // RVA 0x82ED30
    auto* obj = (ai::SgNodeObj*)context->asObject(0, "SgNodeObj");
    context->pushQuaternion(obj->GetRotation());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetSgNode)
{
    // RVA 0x82F260
    auto* obj = (ai::SgNodeObj*)context->asObject(0, "SgNodeObj");
    obj->SetSgNode(CStr(context->asString(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetScale)
{
    // RVA 0x82EC30
    auto* obj = (ai::SgNodeObj*)context->asObject(0, "SgNodeObj");
    obj->SetScale(context->asFloat(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetScale)
{
    // RVA 0x82ED80
    auto* obj = (ai::SgNodeObj*)context->asObject(0, "SgNodeObj");
    context->pushFloat(obj->GetScale());
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(SgNodeObj)
    RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, SetPosition, "", "", "")
    RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, GetPosition, "", "", "")
    RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, SetRotation, "", "", "")
    RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, GetRotation, "", "", "")
    RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, SetSgNode, "", "", "")
    RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, SetScale, "", "", "")
    RT_CLASS_EXPORT(SgNodeObj, m3d::METHOD, GetScale, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgNodeObj);

    bool SgNodeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(this->m_engineModelName, xmlNode, "ModelFile");
            return 1;
        }
        return result;
    }

    SgNodeObjPrototypeInfo::SgNodeObjPrototypeInfo()
    {
    }

    CStr const& SgNodeObjPrototypeInfo::GetEngineModelName() const
    {
        return m_engineModelName;
    }

    ai::Obj* SgNodeObjPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x82F770
        return new SgNodeObj(*this);
    }

    SgNodeObj::SgNodeObj(SgNodeObjPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_node = 0;
        m_modelName = prototypeInfo.GetEngineModelName();
        m_position = ZeroVector;
        m_rotation = IdentityQuaternion;
        m_scale = 1.0f;
        m_needToRelink = 0;
    }

    void SgNodeObj::Update(float elapsedTime, unsigned workTime)
    {
        ai::Obj::Update(elapsedTime, workTime);
        if (m_needToRelink)
        {
            if (m_node)
            {
                m_node->GetGraph()->RelinkNode(m_node, 1);
                m_needToRelink = m_node->IsXFormUpdateNeeded();
            }
        }
    }

    void SgNodeObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x82E8E0
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    void SgNodeObj::SetRotation(Quaternion const& rot)
    {
        m_rotation = rot;
        if (m_node)
        {
            m_node->SetRotation(rot);
            m_needToRelink = 1;
        }
    }

    void SgNodeObj::Registration()
    {
        m_propertiesMap["Pos"] = 4;
        m_propertiesMap["Rot"] = 5;
        m_propertiesMap["ModelFile"] = 44;
        m_propertiesMap["NodeScale"] = 7;
    }

    bool SgNodeObj::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        bool res = true;
        switch (propertyId)
        {
        case 4:
            SetPosition(newValue.GetAsVector());
            break;

        case 5:
            SetRotation(newValue.GetAsQuaternion());
            break;

        case 7:
            SetScale(newValue.GetAsFloat());
            break;

        case 44:
            m_modelName = newValue.GetAsStr();
            SetSgNode();
            break;

        default:
            res = Obj::SetPropertyById(propertyId, newValue);
            break;
        }
        return res;
    }

    void SgNodeObj::SetSgNode(CStr const& modelName)
    {
        // RVA 0x82EF20
        m_modelName = modelName;
        SetSgNode();
    }

    void SgNodeObj::SetSgNode()
    {
        // RVA 0x82EE90 - drops whatever node is there and builds a fresh one from the current
        // model name, which is what makes assigning the ModelFile property swap the visual.
        DeleteSgNode();
        if (!m_modelName.empty())
        {
            m_node = PhysicBody::CreateEffectNode(m_modelName, m_position, m_rotation, 0, m_scale);
            auto* me = this;
            m_node->SetProperty(4361u, &me);
        }
    }

    m3d::Class* SgNodeObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    Quaternion SgNodeObj::GetRotation() const
    {
        // RVA 0x82ECC0
        return m_rotation;
    }

    int SgNodeObj::GetPropertyId(char const* propName) const
    {
        auto it = SgNodeObj::m_propertiesMap.find(propName);
        if (it != SgNodeObj::m_propertiesMap.end())
        {
            return it->second;
        }

        return Obj::GetPropertyId(propName);
    }

    SgNodeObjPrototypeInfo const* SgNodeObj::GetPrototypeInfo() const
    {
        // RVA 0x82F7A0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), SgNodeObjPrototypeInfo const);
    }

    void SgNodeObj::DeleteSgNode()
    {
        // RVA 0x82EC90
        if (m_node)
        {
            m_node->GetGraph()->RemoveNode(m_node);
            m_node = nullptr;
        }
    }

    eGObjPropertySaveStatus SgNodeObj::GetPropertySaveStatus(int id) const
    {
        // RVA 0x82E800 - NOTE: Registration only ever fills m_propertiesMap, so this class's
        // save-status map is always empty and every id falls through to the base class.
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertySaveStatus(id);
    }

    CStr SgNodeObj::GetPropertyName(int id) const
    {
        // RVA 0x82EAD0
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    m3d::Class* SgNodeObj::GetClass() const
    {
        // RVA 0x82EC80
        return RT_CLASS_LOCAL(SgNodeObj);
    }

    void SgNodeObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x82E9E0
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    void SgNodeObj::SetPosition(CVector const& pos)
    {
        m_position = pos;
        if (m_node)
        {
            m_node->SetOriginAbs(pos);
            m_needToRelink = true;
        }
    }

    float SgNodeObj::GetScale() const
    {
        // RVA 0x82ECF0
        return m_scale;
    }

    CVector SgNodeObj::GetPosition() const
    {
        // RVA 0x82EF40
        return m_position;
    }

    void SgNodeObj::SetScale(float scale)
    {
        // RVA 0x82F000 - the node scales uniformly on all three axes.
        m_scale = scale;
        if (m_node)
        {
            m_node->SetScale(CVector(scale, scale, scale));
            m_needToRelink = true;
        }
    }

    bool SgNodeObj::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x82FA50 - only the model name has a per-prototype default; the transform always
        // defaults to the identity.
        switch (propertyId)
        {
            case 4: retVal = ZeroVector; return true;
            case 5: retVal = IdentityQuaternion; return true;
            case 7: retVal = 1.0f; return true;
            case 44: retVal = GetPrototypeInfo()->GetEngineModelName(); return true;
            default: return Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    bool SgNodeObj::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x82F920
        switch (propertyId)
        {
            case 4: retVal = m_position; return true;
            case 5: retVal = m_rotation; return true;
            case 7: retVal = m_scale; return true;
            case 44: retVal = m_modelName; return true;
            default: return Obj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    void SgNodeObj::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x82F7D0 - SAVE_PROP_NORMAL is the default and is not recorded.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    void SgNodeObj::_InternalCreateVisualPart()
    {
        SgNodeObj::SetSgNode();
    }

    SgNodeObj::~SgNodeObj()
    {
        // RVA 0x82EE20 - the scene graph node is the only thing this class owns.
        DeleteSgNode();
    }

    m3d::Object* SgNodeObj::CreateObject()
    {
        // RVA 0x82F490
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* SgNodeObj::Clone()
    {
        // RVA 0x82F2D0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

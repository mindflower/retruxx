#include "sgnodeobj.h"
#include "core/ini.h"
#include "core/aiparam.h"
#include "scene/nodes/sgnode.h"
#include "scene/scenegraph.h"
#include "physicbodies/physicbody.h"

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetPosition)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetPosition)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetRotation)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetRotation)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetSgNode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, SetScale)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(SgNodeObj, GetScale)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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

    void SgNodeObj::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void SgNodeObj::SetSgNode(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::SetSgNode()
    {
        // TODO: check and refactor
        auto* p_m_node = &this->m_node;
        if (m_node)
        {
            m_node->GetGraph()->RemoveNode(m_node);
            *p_m_node = 0;
        }
        if (!m_modelName.empty())
        {
            auto* effectNode = ai::PhysicBody::CreateEffectNode(m_modelName, m_position, m_rotation, 0, m_scale);
            *p_m_node = effectNode;
            auto me = this;
            effectNode->SetProperty(4361u, &me);
        }
    }

    m3d::Class* SgNodeObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    Quaternion SgNodeObj::GetRotation() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::DeleteSgNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus SgNodeObj::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr SgNodeObj::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* SgNodeObj::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector SgNodeObj::GetPosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::SetScale(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool SgNodeObj::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool SgNodeObj::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgNodeObj::_InternalCreateVisualPart()
    {
        SgNodeObj::SetSgNode();
    }

    SgNodeObj::~SgNodeObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* SgNodeObj::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* SgNodeObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

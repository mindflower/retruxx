#include "breakableobject.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(BreakableObject, SetEnabled)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(BreakableObject, SetDestroyed)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BreakableObject)
    RT_CLASS_EXPORT(BreakableObject, m3d::METHOD, SetEnabled, "", "", "")
    RT_CLASS_EXPORT(BreakableObject, m3d::METHOD, SetDestroyed, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BreakableObject);

    Obj* BreakableObjectPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BreakableObjectPrototypeInfo::BreakableObjectPrototypeInfo()
    {
        m_destroyable = 0;
        m_effectType = "WOOD";
        m_destroyEffectType = "BLOW";
        m_brokenModelName = "brokenTest";
        m_destroyedModelName = "brokenTest";
        m_BlastWavePrototypeId = -1;
        m_bIsUpdating = false;
    }

    void BreakableObjectPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // TODO: implement ::RefreshFromXml
    }

    void BreakableObjectPrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    bool BreakableObjectPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeIntAttrib(m_destroyable, xmlNode, "Destroyable");
            m3d::SafeFloatAttrib(m_criticalHitEnergy, xmlNode, "CriticalHitEnergy");
            m3d::SafeStrAttrib(m_effectType, xmlNode, "EffectType");
            m3d::SafeStrAttrib(m_destroyEffectType, xmlNode, "DestroyEffectType");
            m3d::SafeStrAttrib(m_brokenModelName, xmlNode, "BrokenModel");
            m3d::SafeStrAttrib(m_destroyedModelName, xmlNode, "DestroyedModel");
            m3d::SafeStrAttrib(m_breakEffect, xmlNode, "BreakEffect");
            m3d::SafeStrAttrib(m_BlastWavePrototypeName, xmlNode, "BlastWave");
        }
        return result;
    }

    BreakableObject::BreakableObject(BreakableObjectPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BreakableObject::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int BreakableObject::IsDestroyable()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetRemovingEffectName(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetCauseForce(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BoxyBody* BreakableObject::_BoxyBody()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetEnabled()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BreakableObject::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector BreakableObject::GetGeometricCenter() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BreakableObject::DisconnectFromRope(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BreakableObjectPrototypeInfo const* BreakableObject::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetCausePos(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float BreakableObject::GetCriticalHitEnergy()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::DoTouch()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* BreakableObject::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BreakableObject::STATES BreakableObject::GetState()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned short BreakableObject::GetDestroyEffectType()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetJointAnchor(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BlastWave* BreakableObject::CreateBlastWave() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector BreakableObject::GetBounds()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* BreakableObject::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    unsigned short BreakableObject::GetEffectType()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetPositionSelf(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetState(STATES)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetInitVelocities(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::ConnectWithRope(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool BreakableObject::IsExplosive() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetDestroyed()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::SetRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::_BreakOffConnectedRopes(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::_StifleChildLights()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void BreakableObject::_Construct()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BreakableObject::~BreakableObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* BreakableObject::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* BreakableObject::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

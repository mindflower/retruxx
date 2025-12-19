#include "breakableobject.h"

#include "base/prototypemanager.h"
#include <stdexcept>
#include <server/utils.h>
#include "ode/objects.h"

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
        return new BreakableObject(*this);
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

    BreakableObject::BreakableObject(BreakableObjectPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        DisablePhysics();
        m_state = DISABLED;
        m_destroyable = prototypeInfo.m_destroyable;
        m_criticalHitEnergy = prototypeInfo.m_criticalHitEnergy;
        m_effectType = gDynamicScene->GetBoEffectTypeByName(prototypeInfo.m_effectType);
        m_destroyEffectType = ai::gDynamicScene->GetBoEffectTypeByName(prototypeInfo.m_destroyEffectType);
        m_jointId = 0;
        m_bPositioningOnGround = true;
        m_causePos = ZeroVector;
        m_causeForce = 0.0;
        m_initVelocities = 0;
        _SetStaticCollision();
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
        // TODO: implement BreakableObject::RenderDebugInfo
        // RETRUXX_NOT_IMPLEMENTED;
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
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), BreakableObjectPrototypeInfo const);
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
        return RT_CLASS_LOCAL(BreakableObject);
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

    void BreakableObject::SetPositionSelf(CVector const& pos)
    {
        int const physicState = GetPhysicState();
        if ((physicState & 1) != 0 || !m_bPositioningOnGround)
        {
            PhysicObj::SetPositionSelf(pos);
        }
        else
        {
            bool const enabled = (physicState & 2) != 0;
            if (enabled)
            {
                _SetGeomEnabledBit(false);
            }

            PhysicObj::SetPositionSelf(GetGroundPos(pos, 1, 0));
            if (enabled)
            {
                _SetGeomEnabledBit(true);
            }
        }
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
        SimplePhysicObj::_Construct();
        CVector massCenter;
        massCenter.x = 0.0;
        massCenter.y = m_collisionInfos.front().m_relTranslation.y * 0.5;
        massCenter.z = 0.0;
        _SetMassCenter(massCenter);
    }

    BreakableObject::~BreakableObject()
    {
        if (m_jointId)
        {
            dJointDestroy(m_jointId);
            m_jointId = nullptr;
        }
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

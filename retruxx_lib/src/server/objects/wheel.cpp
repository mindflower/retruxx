#include "wheel.h"

#include <stdexcept>
#include "base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Wheel)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Wheel);

    WheelPrototypeInfo::WheelPrototypeInfo()
    {
        this->m_suspensionRange = 0.5;
        this->m_suspensionCFM = 0.1;
        this->m_suspensionERP = 0.80000001;
        this->m_mU = 1.0;
        this->m_typeName = "BIG";
        this->m_blowEffectName = "ET_PS_HARD_BLOW";
    }

    ai::Obj* WheelPrototypeInfo::CreateTargetObject() const
    {
        return new Wheel(*this);
    }

    bool WheelPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_SPHERE);
            m3d::SafeFloatAttrib(this->m_suspensionRange, xmlNode, "SuspensionRange");
            m3d::SafeStrAttrib(this->m_suspensionModelName, xmlNode, "SuspensionModelFile");
            m3d::SafeFloatAttrib(this->m_suspensionCFM, xmlNode, "SuspensionCFM");
            m3d::SafeFloatAttrib(this->m_suspensionERP, xmlNode, "SuspensionERP");
            m3d::SafeFloatAttrib(this->m_mU, xmlNode, "mU");
            m3d::SafeStrAttrib(this->m_typeName, xmlNode, "EffectType");
            m3d::SafeStrAttrib(this->m_blowEffectName, xmlNode, "BlowEffect");
        }
        return result;
    }

    void Wheel::BreakModel()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Wheel::Wheel(WheelPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        this->m_jointID = 0;
        this->m_driven = 1;
        this->m_steering = STEERING_NO;
        this->m_SplashEffect = 0;
        this->m_SplashType = 0;
        this->m_MakeSplash = 0;
        this->m_wheelType = ai::gDynamicScene->GetWheelTypeByName(prototypeInfo.m_typeName);
        this->m_bModelBroken = 0;
        this->m_suspensionNode = 0;
        this->m_curAngle = 0.0;
        this->m_initialRotation = {0.0, 0.0, 0.0, 1.0};
    }

    WheelPrototypeInfo const* Wheel::GetPrototypeInfo() const
    {
        return dynamic_cast<WheelPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Wheel::RelinkGeomsToCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SphericBody const* Wheel::_SphericBody() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Wheel::GetClass() const
    {
        return RT_CLASS_LOCAL(Wheel);
    }

    void Wheel::LinkGeomsToCollisionCells()
    {
        SimplePhysicObj::LinkGeomsToCollisionCells();
    }

    bool Wheel::AttachToPhysicObj(PhysicObj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Wheel::GetWidth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Wheel::GetDirection() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::CreateSuspensionNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Wheel::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Vehicle* Wheel::GetVehicle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::SetInitialRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::HealModel()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::DetachFromPhysicObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Wheel::UnlinkGeomsFromCollisionCells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Wheel::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    float Wheel::GetRadius() const
    {
        auto* sphere = RT_DYNCAST(m_physicBody->m_pGeoms.front()->GetGeom(), Sphere);
        return sphere->GetRadius();
    }

    void Wheel::_InternalCreateVisualPart()
    {
        ai::SimplePhysicObj::_InternalCreateVisualPart();
        if (this->m_bModelBroken)
            ai::Wheel::BreakModel();
    }

    Wheel::~Wheel()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Wheel::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Wheel::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

#include "breakableobject.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(BreakableObject, SetEnabled)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(BreakableObject, SetDestroyed)
{
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    BreakableObjectPrototypeInfo::BreakableObjectPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObjectPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObjectPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    bool BreakableObjectPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    BreakableObject::BreakableObject(BreakableObjectPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    bool BreakableObject::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    int BreakableObject::IsDestroyable()
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetRemovingEffectName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetCauseForce(float)
    {
        throw std::logic_error("Not implemented");
    }

    BoxyBody* BreakableObject::_BoxyBody()
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetEnabled()
    {
        throw std::logic_error("Not implemented");
    }

    bool BreakableObject::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    CVector BreakableObject::GetGeometricCenter() const
    {
        throw std::logic_error("Not implemented");
    }

    bool BreakableObject::DisconnectFromRope(int)
    {
        throw std::logic_error("Not implemented");
    }

    BreakableObjectPrototypeInfo const* BreakableObject::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetCausePos(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    float BreakableObject::GetCriticalHitEnergy()
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::DoTouch()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* BreakableObject::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    BreakableObject::STATES BreakableObject::GetState()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned short BreakableObject::GetDestroyEffectType()
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetJointAnchor(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    BlastWave* BreakableObject::CreateBlastWave() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector BreakableObject::GetBounds()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* BreakableObject::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    unsigned short BreakableObject::GetEffectType()
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetPositionSelf(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetState(STATES)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetInitVelocities(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::ConnectWithRope(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool BreakableObject::IsExplosive() const
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetDestroyed()
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::SetRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::_BreakOffConnectedRopes(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::_StifleChildLights()
    {
        throw std::logic_error("Not implemented");
    }

    void BreakableObject::_Construct()
    {
        throw std::logic_error("Not implemented");
    }

    BreakableObject::~BreakableObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* BreakableObject::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* BreakableObject::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

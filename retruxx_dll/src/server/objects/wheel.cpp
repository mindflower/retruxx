#include "wheel.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Wheel)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Wheel);

    WheelPrototypeInfo::WheelPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* WheelPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool WheelPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::BreakModel()
    {
        throw std::logic_error("Not implemented");
    }

    Wheel::Wheel(WheelPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    WheelPrototypeInfo const* Wheel::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::RelinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    SphericBody const* Wheel::_SphericBody() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Wheel::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::LinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    bool Wheel::AttachToPhysicObj(PhysicObj const*)
    {
        throw std::logic_error("Not implemented");
    }

    float Wheel::GetWidth() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector Wheel::GetDirection() const
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::CreateSuspensionNode()
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Wheel::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* Wheel::GetVehicle() const
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::SetInitialRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::HealModel()
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::DetachFromPhysicObj()
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::UnlinkGeomsFromCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Wheel::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    float Wheel::GetRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    void Wheel::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }

    Wheel::~Wheel()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Wheel::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Wheel::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

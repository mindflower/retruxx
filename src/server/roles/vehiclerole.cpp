#include "vehiclerole.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRole)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRole);

    bool VehicleRolePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRolePrototypeInfo::VehicleRolePrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* VehicleRolePrototypeInfo::getBestOpponentFromTeam(Vehicle const*, Team const*) const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* VehicleRolePrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRole::UpdateVehicle(float, Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRole::FitAgainstVehicle(Vehicle const*, Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRole::setTargetTeam(Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRole::setTargetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRole::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void VehicleRole::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRole::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**)
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRole::FitAgainstObj(Vehicle const*, Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    Team* VehicleRole::getTargetTeam() const
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* VehicleRole::getTargetVehicle() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRole::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRole::setTargetObj(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRolePrototypeInfo const* VehicleRole::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRole::VehicleRole(VehicleRolePrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* VehicleRole::getTargetObj() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRole::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRole::_LookAndFireToEnemy(Vehicle*, float)
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* VehicleRole::getBestOpponentFromTeam(Vehicle const*, Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRole::~VehicleRole()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRole::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRole::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

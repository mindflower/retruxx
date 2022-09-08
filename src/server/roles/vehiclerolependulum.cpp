#include "vehiclerolependulum.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRolePendulum)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRolePendulum);

    VehicleRolePendulumPrototypeInfo::VehicleRolePendulumPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* VehicleRolePendulumPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRolePendulumPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRolePendulum::VehicleRolePendulum(VehicleRolePendulumPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRolePendulum::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRolePendulumPrototypeInfo const* VehicleRolePendulum::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRolePendulum::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRolePendulum::setTargetObj(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRolePendulum::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRolePendulum::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    void VehicleRolePendulum::setTargetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRolePendulum::setTargetTeam(Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRolePendulum::UpdateVehicle(float, Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRolePendulum::~VehicleRolePendulum()
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleRolePendulum::getPendulumPosition(Vehicle*, float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRolePendulum::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRolePendulum::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

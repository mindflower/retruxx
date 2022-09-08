#include "vehiclerolebarrier.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleBarrier)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleBarrier);

    VehicleRoleBarrierPrototypeInfo::VehicleRoleBarrierPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleBarrierPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* VehicleRoleBarrierPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleBarrier::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleBarrier::VehicleRoleBarrier(VehicleRoleBarrierPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleBarrier::setTargetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleBarrier::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    bool VehicleRoleBarrier::UpdateVehicle(float, Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleBarrier::setTargetTeam(Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleBarrier::setTargetObj(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleBarrierPrototypeInfo const* VehicleRoleBarrier::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleBarrier::~VehicleRoleBarrier()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleBarrier::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleRoleBarrier::getBarrierPosition(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleBarrier::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

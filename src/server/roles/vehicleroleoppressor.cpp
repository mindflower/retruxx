#include "vehicleroleoppressor.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleOppressor)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleOppressor);

    float VehicleRoleOppressorPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleOppressorPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleOppressorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleOppressorPrototypeInfo::VehicleRoleOppressorPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleOppressorPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* VehicleRoleOppressorPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleOppressor::VehicleRoleOppressor(VehicleRoleOppressorPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleOppressor::setTargetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleOppressor::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleOppressor::UpdateVehicle(float, Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleOppressor::setTargetObj(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleOppressor::setTargetTeam(Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleOppressorPrototypeInfo const* VehicleRoleOppressor::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleOppressor::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleOppressor::~VehicleRoleOppressor()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleOppressor::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleOppressor::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleRoleOppressor::getOppressorPosition(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }
}

#include "vehiclerolesniper.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleSniper)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleSniper);

    Obj* VehicleRoleSniperPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleSniperPrototypeInfo::VehicleRoleSniperPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleSniperPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleSniper::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleSniper::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleSniper::UpdateVehicle(float, Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleSniperPrototypeInfo const* VehicleRoleSniper::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleSniper::setTargetObj(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleSniper::setTargetTeam(Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleSniper::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleSniper::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleSniper::setTargetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleSniper::VehicleRoleSniper(VehicleRoleSniperPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleSniper::~VehicleRoleSniper()
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleSniper::CurrentPositionIsBad(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleSniper::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    PointBase<float> VehicleRoleSniper::getPeakPositionInt(Vehicle const*, PointBase<float> const&,
        PointBase<float> const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleSniper::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleRoleSniper::getPeakPosition(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<int, std::allocator<int>> VehicleRoleSniper::getTargetObjs() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleSniper::_LookAndFireToEnemy(Vehicle*, float)
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleSniper::PositionIsBad(Vehicle const*, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }
}

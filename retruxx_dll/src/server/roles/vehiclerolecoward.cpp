#include "vehiclerolecoward.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleCoward)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleCoward);

    VehicleRoleCowardPrototypeInfo::VehicleRoleCowardPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleCowardPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* VehicleRoleCowardPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleCoward::VehicleRoleCoward(VehicleRoleCowardPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleCoward::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    void VehicleRoleCoward::setTargetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleCoward::setTargetTeam(Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleCowardPrototypeInfo const* VehicleRoleCoward::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleCoward::setTargetObj(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleCoward::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleCoward::UpdateVehicle(float, Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleCoward::~VehicleRoleCoward()
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleRoleCoward::getCowardPosition(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleCoward::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleCoward::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

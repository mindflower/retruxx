#include "vehiclerolemeat.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleMeat)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleMeat);

    VehicleRoleMeatPrototypeInfo::VehicleRoleMeatPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleMeatPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* VehicleRoleMeatPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleMeat::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    VehicleRoleMeat::VehicleRoleMeat(VehicleRoleMeatPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleMeat::setTargetTeam(Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleMeat::setTargetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleMeat::UpdateVehicle(float, Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleMeat::setTargetObj(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleMeat::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleMeatPrototypeInfo const* VehicleRoleMeat::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleMeat::~VehicleRoleMeat()
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleRoleMeat::_EvaluateChasePointToMove(float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleMeat::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleMeat::_CreateChaseTacticsIfNeeded(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    ChaseMotionTactics* VehicleRoleMeat::CreateChaseMotionTactic(Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleMeat::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

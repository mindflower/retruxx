#include "vehiclerolecheater.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleCheater)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleCheater);

    VehicleRoleCheaterPrototypeInfo::VehicleRoleCheaterPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* VehicleRoleCheaterPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleCheaterPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleCheater::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleCheater::setTargetVehicle(Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleCheater::VehicleRoleCheater(VehicleRoleCheaterPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    bool VehicleRoleCheater::UpdateVehicle(float, Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleCheater::setTargetTeam(Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleCheater::setTargetObj(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleCheaterPrototypeInfo const* VehicleRoleCheater::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VehicleRoleCheater::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    VehicleRoleCheater::~VehicleRoleCheater()
    {
        throw std::logic_error("Not implemented");
    }

    ChaseMotionTactics* VehicleRoleCheater::CreateChaseMotionTactic(Vehicle const*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleCheater::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleRoleCheater::_EvaluateChasePointToMove(float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VehicleRoleCheater::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleRoleCheater::_CreateChaseTacticsIfNeeded(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }
}

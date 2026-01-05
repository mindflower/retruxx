#include "vehiclerolecheater.h"

#include "server/objects/vehicle.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleCheater)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleCheater);

    VehicleRoleCheaterPrototypeInfo::VehicleRoleCheaterPrototypeInfo()
    {
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* VehicleRoleCheaterPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstVehicle(Vehicle const* v, Vehicle const*) const
    {
        if (!v)
            return 0.0;
        auto va = v->GetMaxSpeed();
        return v->EstimateDamageAI() + va;
    }

    bool VehicleRoleCheaterPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    m3d::Class* VehicleRoleCheater::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    void VehicleRoleCheater::setTargetVehicle(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleCheater::VehicleRoleCheater(VehicleRoleCheaterPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleCheater::UpdateVehicle(float, Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleCheater::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleCheater::setTargetObj(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleCheaterPrototypeInfo const* VehicleRoleCheater::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleCheater::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleCheater::~VehicleRoleCheater()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ChaseMotionTactics* VehicleRoleCheater::CreateChaseMotionTactic(Vehicle const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleCheater::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleRoleCheater::_EvaluateChasePointToMove(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleCheater::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleCheater::_CreateChaseTacticsIfNeeded(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

#include "vehiclerolemeat.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleMeat)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleMeat);

    VehicleRoleMeatPrototypeInfo::VehicleRoleMeatPrototypeInfo()
    {
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleMeatPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Obj* VehicleRoleMeatPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleMeat::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    VehicleRoleMeat::VehicleRoleMeat(VehicleRoleMeatPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleMeat::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleMeat::setTargetVehicle(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleMeat::UpdateVehicle(float, Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleMeat::setTargetObj(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleMeat::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleMeatPrototypeInfo const* VehicleRoleMeat::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleMeat::~VehicleRoleMeat()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleRoleMeat::_EvaluateChasePointToMove(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleMeat::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleMeat::_CreateChaseTacticsIfNeeded(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ChaseMotionTactics* VehicleRoleMeat::CreateChaseMotionTactic(Vehicle const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleMeat::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

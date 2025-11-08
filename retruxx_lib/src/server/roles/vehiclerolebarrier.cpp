#include "vehiclerolebarrier.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleBarrier)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleBarrier);

    VehicleRoleBarrierPrototypeInfo::VehicleRoleBarrierPrototypeInfo()
    {
    }

    bool VehicleRoleBarrierPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* VehicleRoleBarrierPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleBarrier::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleBarrier::VehicleRoleBarrier(VehicleRoleBarrierPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleBarrier::setTargetVehicle(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleBarrier::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    bool VehicleRoleBarrier::UpdateVehicle(float, Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleBarrier::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleBarrier::setTargetObj(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleBarrierPrototypeInfo const* VehicleRoleBarrier::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleBarrier::~VehicleRoleBarrier()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleBarrier::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleRoleBarrier::getBarrierPosition(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleBarrier::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

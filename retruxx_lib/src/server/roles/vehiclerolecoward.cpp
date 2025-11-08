#include "vehiclerolecoward.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleCoward)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleCoward);

    VehicleRoleCowardPrototypeInfo::VehicleRoleCowardPrototypeInfo()
    {
        m_vehicleFiringRangeCoeff = 0.30000001;
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleCowardPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    Obj* VehicleRoleCowardPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleCoward::VehicleRoleCoward(VehicleRoleCowardPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleCoward::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    void VehicleRoleCoward::setTargetVehicle(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleCoward::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleCowardPrototypeInfo const* VehicleRoleCoward::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleCoward::setTargetObj(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleCoward::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleCoward::UpdateVehicle(float, Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleCoward::~VehicleRoleCoward()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleRoleCoward::getCowardPosition(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleCoward::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleCoward::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

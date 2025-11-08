#include "vehiclerole.h"

#include <stdexcept>

#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRole)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRole);

    bool VehicleRolePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_vehicleFiringRangeCoeff, xmlNode, "FiringRangeCoeff");
        }
        return result;
    }

    VehicleRolePrototypeInfo::VehicleRolePrototypeInfo()
    {
        m_vehicleFiringRangeCoeff = 1.0;
    }

    Vehicle* VehicleRolePrototypeInfo::getBestOpponentFromTeam(Vehicle const*, Team const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* VehicleRolePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRole::UpdateVehicle(float, Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRole::FitAgainstVehicle(Vehicle const*, Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRole::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRole::setTargetVehicle(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRole::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void VehicleRole::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRole::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRole::FitAgainstObj(Vehicle const*, Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Team* VehicleRole::getTargetTeam() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Vehicle* VehicleRole::getTargetVehicle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRole::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRole::setTargetObj(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRolePrototypeInfo const* VehicleRole::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRole::VehicleRole(VehicleRolePrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* VehicleRole::getTargetObj() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRole::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRole::_LookAndFireToEnemy(Vehicle*, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Vehicle* VehicleRole::getBestOpponentFromTeam(Vehicle const*, Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRole::~VehicleRole()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRole::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRole::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

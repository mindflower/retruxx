#include "vehicleroleoppressor.h"

#include <stdexcept>

#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleOppressor)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleOppressor);

    float VehicleRoleOppressorPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRoleOppressorPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleOppressorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeVector2Attrib(m_oppressionShift, xmlNode, "OppressionShift");
            return 1;
        }
        return result;
    }

    VehicleRoleOppressorPrototypeInfo::VehicleRoleOppressorPrototypeInfo()
    {
    }

    float VehicleRoleOppressorPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* VehicleRoleOppressorPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleOppressor::VehicleRoleOppressor(VehicleRoleOppressorPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleOppressor::setTargetVehicle(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleOppressor::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    bool VehicleRoleOppressor::UpdateVehicle(float, Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleOppressor::setTargetObj(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleOppressor::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleOppressorPrototypeInfo const* VehicleRoleOppressor::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleOppressor::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleOppressor::~VehicleRoleOppressor()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleOppressor::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleOppressor::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleRoleOppressor::getOppressorPosition(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

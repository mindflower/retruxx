#include "vehiclerolesniper.h"

#include "server/objects/vehicle.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleSniper)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleSniper);

    Obj* VehicleRoleSniperPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleSniperPrototypeInfo::VehicleRoleSniperPrototypeInfo()
    {
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstVehicle(Vehicle const* v, Vehicle const*) const
    {
        if (v)
            return v->EstimateDamageAI();
        else
            return 0.0;
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleSniperPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleSniper::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleSniper::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleSniper::UpdateVehicle(float, Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleSniperPrototypeInfo const* VehicleRoleSniper::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleSniper::setTargetObj(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleSniper::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRoleSniper::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    void VehicleRoleSniper::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleSniper::setTargetVehicle(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleSniper::VehicleRoleSniper(VehicleRoleSniperPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRoleSniper::~VehicleRoleSniper()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleSniper::CurrentPositionIsBad(Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleSniper::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PointBase<float> VehicleRoleSniper::getPeakPositionInt(Vehicle const*, PointBase<float> const&,
        PointBase<float> const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRoleSniper::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleRoleSniper::getPeakPosition(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    std::vector<int, std::allocator<int>> VehicleRoleSniper::getTargetObjs() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRoleSniper::_LookAndFireToEnemy(Vehicle*, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRoleSniper::PositionIsBad(Vehicle const*, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

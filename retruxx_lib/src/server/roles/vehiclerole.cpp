#include "vehiclerole.h"

#include <stdexcept>

#include "core/ini.h"
#include "server/objects/team.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"

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

    bool VehicleRole::UpdateVehicle(float, Vehicle* v)
    {
        auto* obj = theObjects->GetEntityByObjId(m_TargetObjId);
        if (obj && obj->bIsEnemyWith(v))
        {
            return true;
        }
        
        v->SetMoveStatus(Vehicle::MOVE_IDLE);
        v->SetAttackStatus(Vehicle::ATTACK_IDLE);
        auto* team = v->GetTeam();
        team->SetTeamTactic(nullptr);
        return false;
    }

    float VehicleRole::FitAgainstVehicle(Vehicle const*, Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRole::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRole::setTargetVehicle(Vehicle const* vehicle)
    {
        if (vehicle)
            this->m_TargetVehicleId = vehicle->GetId();
        else
            this->m_TargetVehicleId = -1;
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
        return RT_CLASS_LOCAL(VehicleRole);
    }

    void VehicleRole::setTargetObj(Obj const* obj)
    {
        if (obj)
            this->m_TargetObjId = obj->GetId();
        else
            this->m_TargetObjId = -1;
    }

    VehicleRolePrototypeInfo const* VehicleRole::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRole::VehicleRole(VehicleRolePrototypeInfo const&)
    {
        this->m_TargetVehicleId = -1;
        this->m_TargetTeamId = -1;
        this->m_TargetObjId = -1;
    }

    Obj* VehicleRole::getTargetObj() const
    {
        return theObjects->GetEntityByObjId(m_TargetObjId);
    }

    void VehicleRole::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleRole::_LookAndFireToEnemy(Vehicle* v, float elapsedTime)
    {
        auto* obj = RT_DYNCAST(theObjects->GetEntityByObjId(m_TargetObjId), Vehicle);
        v->FireFromWeaponAI(true, elapsedTime, obj);
    }

    Vehicle* VehicleRole::getBestOpponentFromTeam(Vehicle const*, Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRole::~VehicleRole() = default;

    m3d::Object* VehicleRole::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRole::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

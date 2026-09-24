#include "vehiclerole.h"

#include <stdexcept>

#include "core/ini.h"
#include <core/kernel.h>
#include "server/objects/team.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"

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

    Vehicle* VehicleRolePrototypeInfo::getBestOpponentFromTeam(Vehicle const* v, Team const* target) const
    {
        // RVA 0x905100 - the target team's vehicle this role fits best against. Nothing is
        // returned unless some vehicle fits better than 0.
        Vehicle* bestOpponent = nullptr;
        float fitValue = 0.0f;
        for (Vehicle* opponent : target->GetVehicles())
        {
            float const fit = FitAgainstVehicle(v, opponent);
            if (fit <= fitValue)
            {
                continue;
            }
            fitValue = fit;
            bestOpponent = opponent;
        }
        return bestOpponent;
    }

    Obj* VehicleRolePrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x904A70 - the base role is abstract.
        return nullptr;
    }

    bool VehicleRole::UpdateVehicle(float, Vehicle* v)
    {
        // RVA 0x905320 - the role is over once its target is gone or no longer an enemy; the
        // vehicle then stops and its team drops the tactic.
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

    float VehicleRole::FitAgainstVehicle(Vehicle const* v, Vehicle const* target)
    {
        // RVA 0x904AC0
        return GetPrototypeInfo()->FitAgainstVehicle(v, target);
    }

    void VehicleRole::setTargetTeam(Team const* team)
    {
        // RVA 0x904D20
        if (team)
            this->m_TargetTeamId = team->GetId();
        else
            this->m_TargetTeamId = -1;
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

    void VehicleRole::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x904C20
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("TargetVehicle", CStr(m_TargetVehicleId).c_str());
        xmlNode->SetAttribute("TargetTeam", CStr(m_TargetTeamId).c_str());
        xmlNode->SetAttribute("TargetObj", CStr(m_TargetObjId).c_str());
    }

    float VehicleRole::FitAgainstTeam(Vehicle const* v, Team const* target, Vehicle** targetVehicle)
    {
        // RVA 0x904AD0
        return GetPrototypeInfo()->FitAgainstTeam(v, target, targetVehicle);
    }

    float VehicleRole::FitAgainstObj(Vehicle const* v, Obj const* obj)
    {
        // RVA 0x904AE0
        return GetPrototypeInfo()->FitAgainstObj(v, obj);
    }

    Team* VehicleRole::getTargetTeam() const
    {
        // RVA 0x905240 - NOTE: the object is cast without a type check.
        return static_cast<Team*>(theObjects->GetEntityByObjId(m_TargetTeamId));
    }

    Vehicle* VehicleRole::getTargetVehicle() const
    {
        // RVA 0x905200 - NOTE: the object is cast without a type check.
        return static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_TargetVehicleId));
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
        // RVA 0x9051D0 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRolePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    VehicleRole::VehicleRole(VehicleRolePrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        // RVA 0x904A80
        this->m_TargetVehicleId = -1;
        this->m_TargetTeamId = -1;
        this->m_TargetObjId = -1;
    }

    Obj* VehicleRole::getTargetObj() const
    {
        return theObjects->GetEntityByObjId(m_TargetObjId);
    }

    void VehicleRole::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x904B80
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_TargetVehicleId, xmlNode, "TargetVehicle");
        m3d::SafeIntAttrib(m_TargetTeamId, xmlNode, "TargetTeam");
        m3d::SafeIntAttrib(m_TargetObjId, xmlNode, "TargetObj");
    }

    void VehicleRole::_LookAndFireToEnemy(Vehicle* v, float elapsedTime)
    {
        // RVA 0x9052C0 - the target object itself is aimed at, whatever it is.
        v->FireFromWeaponAI(true, elapsedTime, theObjects->GetEntityByObjId(m_TargetObjId));
    }

    Vehicle* VehicleRole::getBestOpponentFromTeam(Vehicle const* v, Team const* target)
    {
        // RVA 0x904AF0
        return GetPrototypeInfo()->getBestOpponentFromTeam(v, target);
    }

    VehicleRole::~VehicleRole() = default;

    m3d::Object* VehicleRole::Clone()
    {
        // RVA 0x904D80
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* VehicleRole::CreateObject()
    {
        // RVA 0x904F40
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai

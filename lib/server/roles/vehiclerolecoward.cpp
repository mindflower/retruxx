#include "vehiclerolecoward.h"

#include <cmath>
#include <cstdlib>
#include <stdexcept>

#include "core/ini.h"
#include "core/kernel.h"
#include "math/vector2.h"
#include "server/utils.h"
#include "server/objects/team.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/staticautogun.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleCoward)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleCoward);

    VehicleRoleCowardPrototypeInfo::VehicleRoleCowardPrototypeInfo()
    {
        // RVA 0x7FC970
        m_vehicleFiringRangeCoeff = 0.30000001f;
    }

    bool VehicleRoleCowardPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FC8A0
        return VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Obj* VehicleRoleCowardPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7FC9C0
        return new VehicleRoleCoward(*this);
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        // RVA 0x7FC8C0 - never picked on merit.
        return 0.0f;
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        // RVA 0x7FC8D0 - NOTE: *targetVehicle is left untouched.
        return 0.0f;
    }

    float VehicleRoleCowardPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        // RVA 0x7FC8E0
        return 0.0f;
    }

    VehicleRoleCoward::VehicleRoleCoward(VehicleRoleCowardPrototypeInfo const& prototype) :
        VehicleRole(prototype)
    {
        // RVA 0x7FC8F0
    }

    // RVA 0x7FC910
    VehicleRoleCoward::~VehicleRoleCoward() = default;

    void VehicleRoleCoward::setTargetVehicle(Vehicle const* vehicle)
    {
        // RVA 0x7FC920
        VehicleRole::setTargetVehicle(vehicle);
        setTargetObj(vehicle);
    }

    void VehicleRoleCoward::setTargetTeam(Team const* team)
    {
        // RVA 0x7FC940
        VehicleRole::setTargetTeam(team);
        setTargetObj(team);
    }

    void VehicleRoleCoward::setTargetObj(Obj const* obj)
    {
        // RVA 0x7FC960
        VehicleRole::setTargetObj(obj);
    }

    bool VehicleRoleCoward::UpdateVehicle(float elapsedTime, Vehicle* v)
    {
        // RVA 0x7FCF10
        if (!VehicleRole::UpdateVehicle(elapsedTime, v))
        {
            return false;
        }
        v->SetExternalDestination(getCowardPosition(v));
        _LookAndFireToEnemy(v, elapsedTime);
        return true;
    }

    CVector VehicleRoleCoward::getCowardPosition(Vehicle* v)
    {
        // RVA 0x7FCA30 - flees straight away from the target (to the point mirrored through
        // itself) while within one and a half of the target's firing range.
        Obj const* target = getTargetObj();
        if (!target)
        {
            return v->GetPosition();
        }
        float range = 100.0f;
        if (target->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
        {
            range = static_cast<Vehicle const*>(target)->GetMaxFiringRangeAI();
        }
        else if (target->IsKindOf(RT_CLASS_LOCAL(StaticAutoGun)))
        {
            range = static_cast<StaticAutoGun const*>(target)->GetMaxFiringRangeAI();
        }
        CVector const posMy = v->GetPosition();
        CVector const posTarget = getPhysicObjOrPhysicBodyPosition(target);
        double const dz = posMy.z - posTarget.z;
        double const dy = posMy.y - posTarget.y;
        double const dx = posMy.x - posTarget.x;
        if (range * 1.5 > sqrt(dz * dz + dy * dy + dx * dx))
        {
            CVector const pos = v->GetPosition();
            CVector const targetPos = getPhysicObjOrPhysicBodyPosition(target);
            return CVector(pos.x * 2.0f - targetPos.x, pos.y * 2.0f - targetPos.y, pos.z * 2.0f - targetPos.z);
        }
        return v->GetPosition();
    }

    m3d::Class* VehicleRoleCoward::GetClass() const
    {
        // RVA 0x7FC890
        return RT_CLASS_LOCAL(VehicleRoleCoward);
    }

    m3d::Class* VehicleRoleCoward::GetBaseClass()
    {
        // RVA 0x7FC880
        return RT_CLASS_LOCAL(VehicleRole);
    }

    VehicleRoleCowardPrototypeInfo const* VehicleRoleCoward::GetPrototypeInfo() const
    {
        // RVA 0x7FCF60 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRoleCowardPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Object* VehicleRoleCoward::CreateObject()
    {
        // RVA 0x7FCD50
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* VehicleRoleCoward::Clone()
    {
        // RVA 0x7FCB90
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

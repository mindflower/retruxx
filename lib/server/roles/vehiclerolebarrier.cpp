#include "vehiclerolebarrier.h"

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

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleBarrier)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleBarrier);

    VehicleRoleBarrierPrototypeInfo::VehicleRoleBarrierPrototypeInfo()
    {
        // RVA 0x7FDA90
    }

    bool VehicleRoleBarrierPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FD990
        return VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Obj* VehicleRoleBarrierPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7FDAE0
        return new VehicleRoleBarrier(*this);
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstVehicle(Vehicle const* v, Vehicle const*) const
    {
        // RVA 0x7FD9B0
        if (!v)
        {
            return 0.0f;
        }
        return static_cast<float>(v->GetMaxSpeed() * 0.1);
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstTeam(Vehicle const* v, Team const* target, Vehicle** targetVehicle) const
    {
        // RVA 0x7FD9D0
        *targetVehicle = getBestOpponentFromTeam(v, target);
        return FitAgainstVehicle(v, *targetVehicle);
    }

    float VehicleRoleBarrierPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        // RVA 0x7FDA00 - a barrier cannot block a plain object.
        return 0.0f;
    }

    VehicleRoleBarrier::VehicleRoleBarrier(VehicleRoleBarrierPrototypeInfo const& prototype) :
        VehicleRole(prototype)
    {
        // RVA 0x7FDA10
    }

    // RVA 0x7FDA30
    VehicleRoleBarrier::~VehicleRoleBarrier() = default;

    void VehicleRoleBarrier::setTargetVehicle(Vehicle const* vehicle)
    {
        // RVA 0x7FDA40
        VehicleRole::setTargetVehicle(vehicle);
        setTargetObj(vehicle);
    }

    void VehicleRoleBarrier::setTargetTeam(Team const* team)
    {
        // RVA 0x7FDA60
        // NOTE: only the team's best opponent is kept, as the target vehicle; the target
        // object is not set.
        VehicleRole::setTargetVehicle(getBestOpponentFromTeam(nullptr, team));
    }

    void VehicleRoleBarrier::setTargetObj(Obj const* obj)
    {
        // RVA 0x7FDA80
        VehicleRole::setTargetObj(obj);
    }

    bool VehicleRoleBarrier::UpdateVehicle(float elapsedTime, Vehicle* v)
    {
        // RVA 0x7FE0C0
        if (!VehicleRole::UpdateVehicle(elapsedTime, v))
        {
            return false;
        }
        v->SetExternalDestination(getBarrierPosition(v));
        _LookAndFireToEnemy(v, elapsedTime);
        return true;
    }

    CVector VehicleRoleBarrier::getBarrierPosition(Vehicle* v)
    {
        // RVA 0x7FDB50 - 10 units ahead of the target along its heading; once the barrier is
        // already more than 10 units ahead, it drives at the target itself.
        Obj const* target = getTargetObj();
        if (!target)
        {
            return v->GetPosition();
        }
        CVector const targetPos = getPhysicObjOrPhysicBodyPosition(target);
        CVector heading = getPhysicObjOrPhysicBodyLinearVelocity(target);
        if (sqrt(double(heading.y) * heading.y + double(heading.x) * heading.x + double(heading.z) * heading.z) < 5.0)
        {
            heading = getPhysicObjOrPhysicBodyDirection(target);
        }
        float forwardX = 0.0f;
        float forwardZ = 0.0f;
        float const lenSq = heading.z * heading.z + heading.x * heading.x;
        if (lenSq > 0.0000099999997f)
        {
            double const invLen = 1.0 / sqrt(double(lenSq));
            forwardX = static_cast<float>(heading.x * invLen);
            forwardZ = static_cast<float>(invLen * heading.z);
        }
        if (!(sqrt(double(forwardZ) * forwardZ + double(forwardX) * forwardX) >= 0.0099999998))
        {
            // No usable heading: fall back to +x.
            forwardX = 1.0f;
            forwardZ = 0.0f;
        }
        CVector const myPosition = v->GetPosition();
        float const aheadX = forwardX * 10.0f;
        float const aheadZ = forwardZ * 10.0f;
        float const ahead = ((0.0f - targetPos.x * aheadX) - targetPos.z * aheadZ + aheadX * myPosition.x) + aheadZ * myPosition.z;
        CVector result = targetPos;
        if (ahead <= 10.0f)
        {
            result.x = targetPos.x + aheadX;
            result.z = targetPos.z + aheadZ;
        }
        return result;
    }

    m3d::Class* VehicleRoleBarrier::GetClass() const
    {
        // RVA 0x7FD980
        return RT_CLASS_LOCAL(VehicleRoleBarrier);
    }

    m3d::Class* VehicleRoleBarrier::GetBaseClass()
    {
        // RVA 0x7FD970
        return RT_CLASS_LOCAL(VehicleRole);
    }

    VehicleRoleBarrierPrototypeInfo const* VehicleRoleBarrier::GetPrototypeInfo() const
    {
        // RVA 0x7FE110 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRoleBarrierPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Object* VehicleRoleBarrier::CreateObject()
    {
        // RVA 0x7FDF00
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* VehicleRoleBarrier::Clone()
    {
        // RVA 0x7FDD40
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

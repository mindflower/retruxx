#include "vehicleroleoppressor.h"

#include <cmath>
#include <stdexcept>

#include "core/ini.h"
#include <core/kernel.h>
#include "server/objects/team.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleOppressor)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleOppressor);

    float VehicleRoleOppressorPrototypeInfo::FitAgainstTeam(
        Vehicle const* v,
        Team const* target,
        Vehicle** targetVehicle) const
    {
        // RVA 0x7FE190
        *targetVehicle = getBestOpponentFromTeam(v, target);
        return FitAgainstVehicle(v, *targetVehicle);
    }

    float VehicleRoleOppressorPrototypeInfo::FitAgainstObj(Vehicle const* v, Obj const*) const
    {
        // RVA 0x7FE1C0 - the target plays no part in the fit.
        return FitAgainstVehicle(v, nullptr);
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

    float VehicleRoleOppressorPrototypeInfo::FitAgainstVehicle(Vehicle const* v, Vehicle const*) const
    {
        if (!v)
            return 0.0;
        auto va = v->GetMaxSpeed();
        return v->EstimateDamageAI() + va;
    }

    Obj* VehicleRoleOppressorPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7FE2E0
        return new VehicleRoleOppressor(*this);
    }

    VehicleRoleOppressor::VehicleRoleOppressor(VehicleRoleOppressorPrototypeInfo const& prototype) :
        VehicleRole(prototype)
    {
        // RVA 0x7FE1D0
    }

    void VehicleRoleOppressor::setTargetVehicle(Vehicle const* vehicle)
    {
        // RVA 0x7FE200 - the vehicle is also what the oppressor rides alongside.
        VehicleRole::setTargetVehicle(vehicle);
        setTargetObj(vehicle);
    }

    m3d::Class* VehicleRoleOppressor::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    bool VehicleRoleOppressor::UpdateVehicle(float elapsedTime, Vehicle* v)
    {
        // RVA 0x7FE870 - keeps the vehicle at its post beside the target and shooting at it.
        if (!VehicleRole::UpdateVehicle(elapsedTime, v))
        {
            return false;
        }
        v->SetExternalDestination(getOppressorPosition(v));
        _LookAndFireToEnemy(v, elapsedTime);
        return true;
    }

    void VehicleRoleOppressor::setTargetObj(Obj const* obj)
    {
        // RVA 0x7FE240
        VehicleRole::setTargetObj(obj);
    }

    void VehicleRoleOppressor::setTargetTeam(Team const* team)
    {
        // RVA 0x7FE220
        VehicleRole::setTargetTeam(team);
        setTargetObj(team);
    }

    VehicleRoleOppressorPrototypeInfo const* VehicleRoleOppressor::GetPrototypeInfo() const
    {
        // RVA 0x7FE8C0 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRoleOppressorPrototypeInfo const*>(
            thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* VehicleRoleOppressor::GetClass() const
    {
        // RVA 0x7FE150
        return RT_CLASS_LOCAL(VehicleRoleOppressor);
    }

    VehicleRoleOppressor::~VehicleRoleOppressor() = default;

    m3d::Object* VehicleRoleOppressor::CreateObject()
    {
        // RVA 0x7FE6B0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* VehicleRoleOppressor::Clone()
    {
        // RVA 0x7FE4F0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    CVector VehicleRoleOppressor::getOppressorPosition(Vehicle* v)
    {
        // RVA 0x7FE350 - the post is m_oppressionShift in the target's frame: x across and y along
        // the direction the target is heading (or facing, when it is nearly still).
        VehicleRoleOppressorPrototypeInfo const* prototype = GetPrototypeInfo();
        Obj const* target = getTargetObj();
        if (!target)
        {
            return v->GetPosition();
        }

        CVector const targetPos = getPhysicObjOrPhysicBodyPosition(target);
        CVector heading = getPhysicObjOrPhysicBodyLinearVelocity(target);
        // The original evaluates the lengths on the x87 stack.
        if (5.0 >
            std::sqrt(double(heading.y) * heading.y + double(heading.x) * heading.x + double(heading.z) * heading.z))
        {
            heading = getPhysicObjOrPhysicBodyDirection(target);
        }

        // The heading flattened onto the ground and normalized.
        float forwardX = 0.0f;
        float forwardZ = 0.0f;
        float const lenSq = heading.x * heading.x + heading.z * heading.z;
        if (lenSq > 0.0000099999997f)
        {
            double const invLen = 1.0 / std::sqrt(double(lenSq));
            forwardX = static_cast<float>(heading.x * invLen);
            forwardZ = static_cast<float>(invLen * heading.z);
        }
        if (0.0099999998 > std::sqrt(double(forwardZ) * forwardZ + double(forwardX) * forwardX))
        {
            // No usable heading: fall back to +x.
            forwardX = 1.0f;
            forwardZ = 0.0f;
        }

        CVector2 const& shift = prototype->m_oppressionShift;
        CVector result;
        result.x = (shift.x * forwardX - shift.y * forwardZ) + targetPos.x;
        result.y = targetPos.y;
        result.z = (shift.x * forwardZ + shift.y * forwardX) + targetPos.z;
        return result;
    }
}  // namespace ai

#include "vehiclerolesniper.h"

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
#include "landscape.h"
#include "world.h"
#include "server/server.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleSniper)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleSniper);

    VehicleRoleSniperPrototypeInfo::VehicleRoleSniperPrototypeInfo()
    {
        // RVA 0x7FE9D0
    }

    bool VehicleRoleSniperPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FE910
        return VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Obj* VehicleRoleSniperPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7FEF50
        return new VehicleRoleSniper(*this);
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstVehicle(Vehicle const* v, Vehicle const*) const
    {
        // RVA 0x7FE920
        if (!v)
        {
            return 0.0f;
        }
        return v->EstimateDamageAI();
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstTeam(Vehicle const* v, Team const* target, Vehicle**) const
    {
        // RVA 0x7FEA20 - the vehicle's fit, once per member of the team.
        // NOTE: *targetVehicle is left untouched.
        if (!target)
        {
            return 0.0f;
        }
        unsigned const count = static_cast<unsigned>(target->GetNumVehicles());
        return static_cast<float>(FitAgainstVehicle(v, nullptr) * static_cast<double>(count));
    }

    float VehicleRoleSniperPrototypeInfo::FitAgainstObj(Vehicle const* v, Obj const*) const
    {
        // RVA 0x7FE940 - the object plays no part in the fit.
        return FitAgainstVehicle(v, nullptr);
    }

    VehicleRoleSniper::VehicleRoleSniper(VehicleRoleSniperPrototypeInfo const& prototype) :
        VehicleRole(prototype),
        m_SniperState(ssIdle),
        m_DeniedHeight(10000.0f)
    {
        // RVA 0x7FEA70
    }

    // RVA 0x7FE950
    VehicleRoleSniper::~VehicleRoleSniper() = default;

    void VehicleRoleSniper::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FEAE0
        VehicleRole::LoadRuntimeValues(xmlFile, xmlNode);
        int state = m_SniperState;
        m3d::SafeIntAttrib(state, xmlNode, "SniperState");
        m_SniperState = static_cast<SniperState>(state);
        m3d::SafeVectorAttrib(m_Peak, xmlNode, "Peak");
        m3d::SafeFloatAttrib(m_DeniedHeight, xmlNode, "DeniedHeight");
        m3d::SafeFloatAttrib(m_LastChange, xmlNode, "LastChange");
    }

    void VehicleRoleSniper::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7FEFA0
        VehicleRole::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("SniperState", CStr(static_cast<int>(m_SniperState)).c_str());
        xmlNode->SetAttribute("Peak", CStr(m_Peak).c_str());
        xmlNode->SetAttribute("DeniedHeight", CStr(m_DeniedHeight).c_str());
        xmlNode->SetAttribute("LastChange", CStr(m_LastChange).c_str());
    }

    void VehicleRoleSniper::setTargetVehicle(Vehicle const* vehicle)
    {
        // RVA 0x7FE960
        VehicleRole::setTargetVehicle(vehicle);
        setTargetObj(vehicle);
    }

    void VehicleRoleSniper::setTargetTeam(Team const* team)
    {
        // RVA 0x7FE980
        VehicleRole::setTargetTeam(team);
    }

    void VehicleRoleSniper::setTargetObj(Obj const* obj)
    {
        // RVA 0x7FE990
        VehicleRole::setTargetObj(obj);
    }

    bool VehicleRoleSniper::UpdateVehicle(float elapsedTime, Vehicle* v)
    {
        // RVA 0x7FFF30 - finds the highest spot within reach of the targets that is safe
        // enough, drives there and fires from it; a spot it cannot reach in 300 seconds is
        // ruled out (nothing at or above that height is tried again).
        if (!VehicleRole::UpdateVehicle(elapsedTime, v))
        {
            return false;
        }
        switch (m_SniperState)
        {
        case ssIdle:
            m_Peak = getPeakPosition(v);
            v->SetExternalDestination(m_Peak);
            m_LastChange = theObjects->GetGameTimeDiff();
            m_SniperState = ssMovingToPeak;
            break;
        case ssMovingToPeak:
        {
            CVector const vel = v->GetLinearVelocity();
            if (theObjects->GetGameTimeDiff() - m_LastChange <= 300.0f ||
                sqrt(double(vel.x) * vel.x + double(vel.y) * vel.y + double(vel.z) * vel.z) >= 0.1)
            {
                CVector const pos = v->GetPosition();
                double const dz = m_Peak.z - pos.z;
                double const dy = m_Peak.y - pos.y;
                double const dx = m_Peak.x - pos.x;
                if (sqrt(dz * dz + dy * dy + dx * dx) < 2.0)
                {
                    m_SniperState = ssFiring;
                }
            }
            else
            {
                // Stuck on the way.
                m_DeniedHeight = v->GetPosition().y - 1.0f;
                m_SniperState = ssIdle;
            }
            break;
        }
        case ssFiring:
            if (CurrentPositionIsBad(v))
            {
                m_SniperState = ssIdle;
            }
            break;
        default:
            break;
        }
        _LookAndFireToEnemy(v, elapsedTime);
        return true;
    }

    bool VehicleRoleSniper::CurrentPositionIsBad(Vehicle const* v)
    {
        // RVA 0x7FF910 - bad when the targets could not take (in total) 3 damage from here.
        std::vector<int> const targets = getTargetObjs();
        // The damage estimate ignores the targets themselves and the sniper's own team.
        std::vector<int> exceptions(targets.begin(), targets.end());
        if (Team* team = v->GetTeam())
        {
            for (Vehicle* member : team->GetVehicles())
            {
                exceptions.push_back(member->GetId());
            }
        }
        float sumDamage = 0.0f;
        for (int id : targets)
        {
            if (Obj* target = theObjects->GetEntityByObjId(id))
            {
                CVector const targetPosition = getPhysicObjOrPhysicBodyPosition(target);
                sumDamage = v->EstimateDamageAI(targetPosition, exceptions) + sumDamage;
            }
        }
        return sumDamage < 3.0f;
    }

    bool VehicleRoleSniper::PositionIsBad(Vehicle const* v, CVector const& position)
    {
        // RVA 0x7FF750 - as CurrentPositionIsBad, but from the given position.
        std::vector<int> const targets = getTargetObjs();
        // The damage estimate ignores the targets themselves and the sniper's own team.
        std::vector<int> exceptions(targets.begin(), targets.end());
        if (Team* team = v->GetTeam())
        {
            for (Vehicle* member : team->GetVehicles())
            {
                exceptions.push_back(member->GetId());
            }
        }
        float sumDamage = 0.0f;
        for (int id : targets)
        {
            if (Obj* target = theObjects->GetEntityByObjId(id))
            {
                CVector const targetPosition = getPhysicObjOrPhysicBodyPosition(target);
                sumDamage = v->EstimateDamageFromPositionAI(position, targetPosition, exceptions) + sumDamage;
            }
        }
        return sumDamage < 3.0f;
    }

    CVector VehicleRoleSniper::getPeakPosition(Vehicle* v)
    {
        // RVA 0x7FFD00 - searches the square of two thirds of its firing range round the
        // targets' centre.
        std::vector<int> const targets = getTargetObjs();
        if (targets.empty())
        {
            return v->GetPosition();
        }
        float const seekRadius = static_cast<float>(v->GetMaxFiringRangeAI() * 0.66666669f);
        CVector center = ZeroVector;
        for (int id : targets)
        {
            // NOTE: a dead target is passed on as null.
            CVector const p = getPhysicObjOrPhysicBodyPosition(theObjects->GetEntityByObjId(id));
            center.x = p.x + center.x;
            center.y = p.y + center.y;
            center.z = p.z + center.z;
        }
        double const inv = 1.0 / double(targets.size());
        PointBase<float> leftBottom(static_cast<float>(inv * center.x) - seekRadius, static_cast<float>(inv * center.z) - seekRadius);
        PointBase<float> topRight(static_cast<float>(inv * center.x) + seekRadius, static_cast<float>(inv * center.z) + seekRadius);
        leftBottom = clampIntoLandscape(leftBottom);
        topRight = clampIntoLandscape(topRight);
        PointBase<float> const best = getPeakPositionInt(v, leftBottom, topRight, seekRadius * 0.2f);
        float const height = pServer->GetWorld()->GetLandscape().GetHeightWithCollisions(best.x, best.y, false);
        return CVector(best.x, height, best.y);
    }

    PointBase<float> VehicleRoleSniper::getPeakPositionInt(Vehicle const* v, PointBase<float> const& leftbottom, PointBase<float> const& righttop, float step)
    {
        // RVA 0x7FFAD0 - a coarse-to-fine search: sample the rectangle every step units for the
        // highest acceptable point below m_DeniedHeight, then search again round it at half the
        // step, down to a step of 1.
        if (step <= 1.0f)
        {
            return PointBase<float>((leftbottom.x + righttop.x) * 0.5f, (leftbottom.y + righttop.y) * 0.5f);
        }
        float bestX = (leftbottom.x + righttop.x) * 0.5f;
        float bestY = (righttop.y + leftbottom.y) * 0.5f;
        float maxValue = -1.0f;
        if (righttop.x >= leftbottom.x)
        {
            float x = leftbottom.x;
            do
            {
                float y = leftbottom.y;
                if (righttop.y >= y)
                {
                    do
                    {
                        float const height = pServer->GetWorld()->GetLandscape().GetHeightWithCollisions(x, y, false);
                        if (height > maxValue && m_DeniedHeight > height)
                        {
                            if (!PositionIsBad(v, CVector(x, height, y)))
                            {
                                maxValue = height;
                                bestX = x;
                                bestY = y;
                            }
                        }
                        y = y + step;
                    } while (!(righttop.y < y));
                }
                x = x + step;
            } while (!(righttop.x < x));
        }
        return getPeakPositionInt(v, PointBase<float>(bestX - step, bestY - step), PointBase<float>(bestX + step, bestY + step), step * 0.5f);
    }

    std::vector<int, std::allocator<int>> VehicleRoleSniper::getTargetObjs() const
    {
        // RVA 0x7FF4E0 - the target object, or else every vehicle of the target team.
        std::vector<int> result;
        if (Obj* target = getTargetObj())
        {
            result.push_back(target->GetId());
        }
        else if (Team* team = getTargetTeam())
        {
            std::vector<Vehicle*> const vehicles(team->GetVehicles().begin(), team->GetVehicles().end());
            for (Vehicle* vehicle : vehicles)
            {
                result.push_back(vehicle->GetId());
            }
        }
        return result;
    }

    void VehicleRoleSniper::_LookAndFireToEnemy(Vehicle* v, float elapsedTime)
    {
        // RVA 0x7FF5E0 - fires at the nearest live target.
        Obj* nearest = nullptr;
        float minDist = 1.0e10f;
        std::vector<int> const targets = getTargetObjs();
        for (int id : targets)
        {
            Obj* target = theObjects->GetEntityByObjId(id);
            if (!target || !target->IsAlive())
            {
                continue;
            }
            CVector const targetPosition = getPhysicObjOrPhysicBodyPosition(target);
            CVector const pos = v->GetPosition();
            double const dz = targetPosition.z - pos.z;
            double const dy = targetPosition.y - pos.y;
            double const dx = targetPosition.x - pos.x;
            float const dist = static_cast<float>(sqrt(dz * dz + dy * dy + dx * dx));
            if (minDist > dist)
            {
                nearest = target;
                minDist = dist;
            }
        }
        v->FireFromWeaponAI(nearest != nullptr, elapsedTime, nearest);
    }

    m3d::Class* VehicleRoleSniper::GetClass() const
    {
        // RVA 0x7FE900
        return RT_CLASS_LOCAL(VehicleRoleSniper);
    }

    m3d::Class* VehicleRoleSniper::GetBaseClass()
    {
        // RVA 0x7FE8F0
        return RT_CLASS_LOCAL(VehicleRole);
    }

    VehicleRoleSniperPrototypeInfo const* VehicleRoleSniper::GetPrototypeInfo() const
    {
        // RVA 0x7FF1C0 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRoleSniperPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Object* VehicleRoleSniper::CreateObject()
    {
        // RVA 0x7FED90
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* VehicleRoleSniper::Clone()
    {
        // RVA 0x7FEBD0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

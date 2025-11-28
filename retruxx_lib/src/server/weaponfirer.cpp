#include "weaponfirer.h"
#include <stdexcept>
#include "resourcemanager.h"
#include "objects/vehicle.h"
#include "objects/base/globalproperties.h"
#include "objects/guns/compoundgun.h"
#include "objects/guns/gun.h"
#include "objects/physicbodies/vehiclepart.h"
#include "roles/vehiclerole.h"

namespace ai
{
    void WeaponFirer::AimAndFireFromWeapons(ComplexPhysicObj* obj, bool enable, float elapsedTime, Obj* target)
    {
        CVector enemyPos = ZeroVector;
        if (enable)
        {
            if (target)
            {
                const auto flags = target->GetFlags();
                if ((flags & 8) == 0 && (flags & 2) == 0 && !target->GetParentRepository())
                {
                    enemyPos = obj->GetSmoothTargetPointForObj(target, elapsedTime);

                    auto objPos = obj->GetPosition();
                    objPos.x = enemyPos.x - objPos.x;
                    objPos.y = enemyPos.y - objPos.y;
                    objPos.z = enemyPos.z - objPos.z;

                    const auto range = objPos.length();
                    if (GetMaxFiringRange(obj) > range)
                    {
                        WeaponLookAtPoint(obj, enemyPos, elapsedTime);
                    }
                    else
                    {
                        enable = false;
                    }
                }
            }
        }
        FireFromWeaponsIfPossible(obj, enable, enemyPos, target);
    }

    void WeaponFirer::FireFromWeaponsIfPossible(ComplexPhysicObj* obj, bool enable, const CVector& targetPoint, Obj* target)
    {
        // TODO: check this!!!!!
        Vehicle* vehicle = nullptr;
        ai::Vehicle* controlledVehicle = nullptr;
        if (IS_KIND_OF(obj, Vehicle))
        {
            vehicle = RT_DYNCAST(obj, Vehicle);
            if (vehicle && vehicle->GetInSmokeScreenMode() && !vehicle->bIsControlledByPlayer())
            {
                enable = false;
            }
            controlledVehicle = vehicle;
        }

        for (auto& [name, vehiclePart] : obj->m_vehicleParts)
        {
            if (IS_KIND_OF(vehiclePart, Gun))
            {
                auto* gun = RT_DYNCAST(vehiclePart, Gun);

                std::vector<int> exceptions;
                if (enable && target)
                {
                    exceptions.push_back(target->GetId());
                    if (IS_KIND_OF(target, Vehicle))
                    {
                        auto* targetVehicle = RT_DYNCAST(obj, Vehicle);
                        for (size_t i = 0; i < targetVehicle->GetNumWheels(); ++i)
                        {
                            if (auto* wheel = targetVehicle->GetWheel(i))
                            {
                                exceptions.push_back(wheel->GetId());
                            }
                        }
                    }

                    bool isLookAtPoint = gun->PointIsReachable(targetPoint, exceptions);
                    if (isLookAtPoint)
                    {
                        isLookAtPoint = gun->isLookAtPoint(targetPoint, 0.02);
                    }
                    if (!isLookAtPoint)
                    {
                        gun->Fire(false);
                        continue;
                    }
                }

                // Set target ID for the gun
                if (target)
                {
                    gun->SetTargetId(target->GetId());
                }
                else if (controlledVehicle)
                {
                    gun->SetTargetId(controlledVehicle->GetSeenObjId());
                }

                // Fire the gun
                gun->Fire(enable);
            }
            else if (IS_KIND_OF(vehiclePart, CompoundGun))
            {
                auto* gun = RT_DYNCAST(vehiclePart, CompoundGun);
                int targetId = -1;
                int lockedId = -1;
                if (target)
                {
                    targetId = target->GetId();
                    lockedId = targetId;
                }
                if (controlledVehicle)
                {
                    targetId = controlledVehicle->GetSeenObjId();
                    lockedId = targetId;
                }
                gun->SetProperTargetId(targetId, lockedId);
                gun->Fire(enable);
            }
        }
    }

    void WeaponFirer::WeaponLookAtPoint(ComplexPhysicObj* obj, CVector const& lookAt, float elapsedTime)
    {
        const auto gunResourceId = theResourceManager->GetResourceId("GUN");
        const auto specialWeaponResourceId = theResourceManager->GetResourceId("SPECIAL_WEAPON");

        for (auto& [name, vehiclePart] : obj->m_vehicleParts)
        {
            const auto* prototypeInfo = vehiclePart->GetPrototypeInfo();
            const auto resId = prototypeInfo->m_resourceId;
            if (resId != specialWeaponResourceId && theResourceManager->bResourceIsKindOf(resId, gunResourceId))
            {
                if (IS_KIND_OF(vehiclePart, Gun))
                {
                    auto* gun = RT_DYNCAST(vehiclePart, Gun);
                    gun->LookAtPoint(lookAt, elapsedTime); 
                }
                else if (IS_KIND_OF(vehiclePart, CompoundGun))
                {
                    auto* gun = RT_DYNCAST(vehiclePart, CompoundGun);
                    gun->LookAtPoint(lookAt, elapsedTime);
                }
            }
        }
    }

    float WeaponFirer::GetMaxFiringRange(ComplexPhysicObj const* obj)
    {
        // TODO: check this
        float res = 0.0;
        for (auto const& [name, vehiclePart] : *obj)
        {
            if (IS_KIND_OF(vehiclePart, Gun))
            {
                auto* gun = RT_DYNCAST(vehiclePart, Gun);
                float range = gun->GetFiringRange();
                if (range > res)
                {
                    res = range;
                }
            }
            else if (IS_KIND_OF(vehiclePart, CompoundGun))
            {
                auto* gun = RT_DYNCAST(vehiclePart, CompoundGun);
                float range = gun->GetFiringRange();
                if (range > res)
                {
                    res = range;
                }
            }
        }

        if (IS_KIND_OF(obj, Vehicle))
        {
            auto* vehicle = RT_DYNCAST(obj, Vehicle const);
            if (auto* role = vehicle->GetRole())
            {
                auto* protoInfo = role->GetPrototypeInfo();
                res *= protoInfo->m_vehicleFiringRangeCoeff;
            }
            return res * ai::theGlobProp.m_vehicleAiFiringRangeMult;
        }
        return res;
    }
}
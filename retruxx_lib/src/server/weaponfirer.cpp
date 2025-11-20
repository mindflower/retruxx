#include "weaponfirer.h"
#include <stdexcept>
#include "resourcemanager.h"
#include "objects/guns/compoundgun.h"
#include "objects/guns/gun.h"
#include "objects/physicbodies/vehiclepart.h"

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

    void WeaponFirer::FireFromWeaponsIfPossible(ComplexPhysicObj*, bool, CVector const&, Obj*)
    {
        // TODO: implement WeaponFirer::FireFromWeaponsIfPossible
        //RETRUXX_NOT_IMPLEMENTED;
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

    float WeaponFirer::GetMaxFiringRange(ComplexPhysicObj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
        return 0.0f;
    }
}
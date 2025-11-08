#include "weaponfirer.h"
#include "thirdparty/injecttools.h"
#include <stdexcept>

namespace ai
{
    RETRUXX_DLL_INJECT_FUNCTION(0x007C3370, WeaponFirer::AimAndFireFromWeapons)
    void WeaponFirer::AimAndFireFromWeapons(ComplexPhysicObj* obj, bool enable, float elapsedTime, Obj* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
        CVector enemyPos;
        if (enable)
        {
            if (target)
            {
                auto const flags = target->GetFlags();
                if ((flags & 8) == 0 && (flags & 2) == 0 && !target->GetParentRepository())
                {
                    enemyPos = obj->GetSmoothTargetPointForObj(target, elapsedTime);

                    auto objPos = obj->GetPosition();
                    objPos.x = enemyPos.x - objPos.x;
                    objPos.y = enemyPos.y - objPos.y;
                    objPos.z = enemyPos.z - objPos.z;

                    auto const range = sqrt(objPos.x * objPos.x + objPos.y * objPos.y + objPos.z * objPos.z);
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

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x007C2FC0, WeaponFirer::FireFromWeaponsIfPossible)
    void WeaponFirer::FireFromWeaponsIfPossible(ComplexPhysicObj*, bool, CVector const&, Obj*)
    {
        // TODO: implement WeaponFirer::FireFromWeaponsIfPossible
        //RETRUXX_NOT_IMPLEMENTED;
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x007C3210, WeaponFirer::WeaponLookAtPoint)
    void WeaponFirer::WeaponLookAtPoint(ComplexPhysicObj*, CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    
    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x007C2EA0, WeaponFirer::GetMaxFiringRange)
    float WeaponFirer::GetMaxFiringRange(ComplexPhysicObj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
        return 0.0f;
    }
}
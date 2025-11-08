#pragma once
#include "objects/base/complexphysicobj.h"

namespace ai
{
    class WeaponFirer
    {
    public:
        static void AimAndFireFromWeapons(ComplexPhysicObj*, bool, float, Obj*);
        static void FireFromWeaponsIfPossible(ComplexPhysicObj*, bool, CVector const&, Obj*);
        static void WeaponLookAtPoint(ComplexPhysicObj*, CVector const&, float);
        static float GetMaxFiringRange(ComplexPhysicObj const*);
    };
}
#pragma once
#include <core/stringm3d.h>
#include "math/vector.h"

namespace ai
{
    enum DamageType
    {
        DAMAGE_PIERCING = 0x0,
        DAMAGE_BLAST = 0x1,
        DAMAGE_ENERGY = 0x2,
        DAMAGE_WATER = 0x3,
        DAMAGE_NUM_TYPES = 0x4,
    };

    class DamageInfo
    {
    public:
        DamageInfo();

    private:
        int attackerId;
        int attackingAgentId;
        bool bDamageFriends;
        int gunPrototypeId;
        float damage;
        DamageType damageType;
        CStr damagedPartName;
        CVector hitPos;
        CVector hitDir;
        CVector normal;
        int decalId;
    };
}

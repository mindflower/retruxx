#pragma once
#include <math/vector.h>
#include <core/stringm3d.h>

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

    struct DamageInfo
    {
        int attackerId;
        int attackingAgentId;
        bool bDamageFriends;
        int gunPrototypeId;
        float damage;
        ai::DamageType damageType;
        CStr damagedPartName;
        CVector hitPos;
        CVector hitDir;
        CVector normal;
        int decalId;

        DamageInfo()
        {
            this->bDamageFriends = 0;
            this->damage = 0.0;
            this->damageType = DAMAGE_BLAST;
            this->attackerId = -1;
            this->attackingAgentId = -1;
            this->gunPrototypeId = -1;
            this->hitPos.x = 0.0;
            this->hitPos.y = 0.0;
            this->hitPos.z = 0.0;
            this->hitDir.x = 0.0;
            this->hitDir.y = 1.0;
            this->hitDir.z = 0.0;
            this->normal.x = 1.0;
            this->normal.y = 0.0;
            this->normal.z = 0.0;
            this->decalId = -1;
        }
    };
}  // namespace ai

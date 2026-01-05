#pragma once
#include "math/vector.h"
#include "retruxx/common.h"

namespace ai
{
    class CombatMastermind
    {
        using SteeringForceMap = retruxx::map<int, CVector >;

    public:
        CombatMastermind(int teamId);
        void GenerateLineOfFireAvoidanceSteeringForces(retruxx::map<int, CVector>& steeringForces);

    private:
        /* 0x0000 */ int m_teamId;
    }; /* size: 0x0004 */
}

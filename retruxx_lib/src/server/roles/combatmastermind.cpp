#include "combatmastermind.h"

namespace ai
{
    CombatMastermind::CombatMastermind(int teamId)
    {
        this->m_teamId = teamId;
    }

    void CombatMastermind::GenerateLineOfFireAvoidanceSteeringForces(retruxx::map<int, CVector>& steeringForces)
    {
        throw std::logic_error("Not implemented");
    }
}

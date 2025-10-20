#include "combatmastermind.h"

namespace ai
{
    CombatMastermind::CombatMastermind(int teamId)
    {
        this->m_teamId = teamId;
    }

    CombatMastermind::~CombatMastermind()
    {
        throw std::logic_error("Not implemented");
    }

    void CombatMastermind::GenerateLineOfFireAvoidanceSteeringForces(retruxx::map<int, CVector>& steeringForces)
    {
        throw std::logic_error("Not implemented");
    }
}

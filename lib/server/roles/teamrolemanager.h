#pragma once
#include "teamtactic.h"
#include "retruxx/common.h"

namespace ai
{
    class TeamRoleManager
    {
    private:
        static void GetRolePrototypeIds(
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            retruxx::vector<int, retruxx::allocator<int>>& prototypeIds);
        static retruxx::vector<int, retruxx::allocator<int>> getAgainstVehicle(
            retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Vehicle const* target,
            float& value);
        static retruxx::vector<int, retruxx::allocator<int>> getAgainstTeam(
            retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Team const* target,
            float& value);
        static retruxx::vector<int, retruxx::allocator<int>> getAgainstObj(
            retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Obj const* target,
            float& value);

    public:
        static void
            AssignAgainstVehicle(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Vehicle const* target);
        static void AssignAgainstTeam(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Team const* target);
        static void AssignAgainstObj(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Obj const* target);
        static float
            FitAgainstVehicle(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Vehicle const* target);
        static float
            FitAgainstTeam(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Team const* target);
        static float FitAgainstObj(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Obj const* target);
        static void ClearRoles(ai::Team const* team);
        static void GetTargetsPositions(
            ai::Team const* team,
            retruxx::vector<CVector, retruxx::allocator<CVector>>& positions);
    };
}  // namespace ai

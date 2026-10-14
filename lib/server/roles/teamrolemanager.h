#pragma once
#include "teamtactic.h"
#include "retruxx/common.h"

namespace ai
{
    class TeamRoleManager
    {
    private:
        static void __fastcall GetRolePrototypeIds(
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            retruxx::vector<int, retruxx::allocator<int>>& prototypeIds);
        static retruxx::vector<int, retruxx::allocator<int>> __fastcall getAgainstVehicle(
            retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Vehicle const* target,
            float& value);
        static retruxx::vector<int, retruxx::allocator<int>> __fastcall getAgainstTeam(
            retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Team const* target,
            float& value);
        static retruxx::vector<int, retruxx::allocator<int>> __fastcall getAgainstObj(
            retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Obj const* target,
            float& value);

    public:
        static void __fastcall AssignAgainstVehicle(
            ai::TeamTacticWithRoles const* tactic,
            ai::Team* team,
            ai::Vehicle const* target);
        static void __fastcall AssignAgainstTeam(
            ai::TeamTacticWithRoles const* tactic,
            ai::Team* team,
            ai::Team const* target);
        static void __fastcall AssignAgainstObj(
            ai::TeamTacticWithRoles const* tactic,
            ai::Team* team,
            ai::Obj const* target);
        static float __fastcall FitAgainstVehicle(
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Vehicle const* target);
        static float __fastcall FitAgainstTeam(
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Team const* target);
        static float __fastcall FitAgainstObj(
            ai::TeamTacticWithRoles const* tactic,
            ai::Team const* team,
            ai::Obj const* target);
        static void __fastcall ClearRoles(ai::Team const* team);
        static void __fastcall GetTargetsPositions(
            ai::Team const* team,
            retruxx::vector<CVector, retruxx::allocator<CVector>>& positions);
    };
}  // namespace ai

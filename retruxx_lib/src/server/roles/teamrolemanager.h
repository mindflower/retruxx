#pragma once
#include "teamtactic.h"
#include "thirdparty/containers.h"

namespace ai
{
    class TeamRoleManager
    {
    private:
        static void __fastcall GetRolePrototypeIds(const ai::TeamTacticWithRoles* tactic, const ai::Team* team, retruxx::vector<int, retruxx::allocator<int> >& prototypeIds);
        static retruxx::vector<int, retruxx::allocator<int> > __fastcall getAgainstVehicle(const retruxx::vector<int, retruxx::allocator<int> >& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic, const ai::Team* team, const ai::Vehicle* target, float& value);
        static retruxx::vector<int, retruxx::allocator<int> > __fastcall getAgainstTeam(const retruxx::vector<int, retruxx::allocator<int> >& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic, const ai::Team* team, const ai::Team* target, float& value);
        static retruxx::vector<int, retruxx::allocator<int> > __fastcall getAgainstObj(const retruxx::vector<int, retruxx::allocator<int> >& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic, const ai::Team* team, const ai::Obj* target, float& value);

    public:
        static void __fastcall AssignAgainstVehicle(const ai::TeamTacticWithRoles* tactic, ai::Team* team, const ai::Vehicle* target);
        static void __fastcall AssignAgainstTeam(const ai::TeamTacticWithRoles* tactic, ai::Team* team, const ai::Team* target);
        static void __fastcall AssignAgainstObj(const ai::TeamTacticWithRoles* tactic, ai::Team* team, const ai::Obj* target);
        static float __fastcall FitAgainstVehicle(const ai::TeamTacticWithRoles* tactic, const ai::Team* team, const ai::Vehicle* target);
        static float __fastcall FitAgainstTeam(const ai::TeamTacticWithRoles* tactic, const ai::Team* team, const ai::Team* target);
        static float __fastcall FitAgainstObj(const ai::TeamTacticWithRoles* tactic, const ai::Team* team, const ai::Obj* target);
        static void __fastcall ClearRoles(const ai::Team* team);
        static void __fastcall GetTargetsPositions(const ai::Team* team, retruxx::vector<CVector, retruxx::allocator<CVector> >& positions);
    }; /* size: 0x0001 */
}

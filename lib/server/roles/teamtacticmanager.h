#pragma once
#include "retruxx/common.h"

class CStr;
namespace ai
{
    class Obj;
    class Vehicle;
    class Team;
    class TeamTactic;

    class TeamTacticManager
    {
    private:
        static retruxx::vector<int, std::allocator<int>> getAllTacticsPrototypeIds();
        static ai::TeamTactic* createTeamTactic(int protoId);

    public:
        void AssignBestTacticAgainstVehicle(ai::Team* team, ai::Vehicle const* target);
        void AssignBestTacticAgainstTeam(ai::Team* team, ai::Team const* target);
        void AssignBestTacticAgainstObj(ai::Team* team, ai::Obj const* target);
        void AssignTacticAgainstVehicle(ai::Team* team, ai::Vehicle const* target, CStr const& tacticName);
        void AssignTacticAgainstTeam(ai::Team* team, ai::Team const* target, CStr const& tacticName);
        void AssignTacticAgainstObj(ai::Team* team, ai::Obj const* target, CStr const& tacticName);
    }; /* size: 0x0001 */
}

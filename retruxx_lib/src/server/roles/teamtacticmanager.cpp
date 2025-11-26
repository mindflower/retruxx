#include "teamtacticmanager.h"

namespace ai
{

    retruxx::vector<int, std::allocator<int>> TeamTacticManager::getAllTacticsPrototypeIds()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::TeamTactic* TeamTacticManager::createTeamTactic(int protoId)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticManager::AssignBestTacticAgainstVehicle(ai::Team* team, ai::Vehicle const* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticManager::AssignBestTacticAgainstTeam(ai::Team* team, ai::Team const* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticManager::AssignBestTacticAgainstObj(ai::Team* team, ai::Obj const* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticManager::AssignTacticAgainstVehicle(ai::Team* team, ai::Vehicle const* target, CStr const& tacticName)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticManager::AssignTacticAgainstTeam(ai::Team* team, ai::Team const* target, CStr const& tacticName)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticManager::AssignTacticAgainstObj(ai::Team* team, ai::Obj const* target, CStr const& tacticName)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

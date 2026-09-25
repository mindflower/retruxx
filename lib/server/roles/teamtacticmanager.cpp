#include "teamtacticmanager.h"

#include "teamtactic.h"
#include "server/objects/team.h"
#include "server/objects/base/objcontainer.h"

#include <server/resourcemanager.h>
#include <server/objects/base/prototypemanager.h>

namespace ai
{

    retruxx::vector<int, std::allocator<int>> TeamTacticManager::getAllTacticsPrototypeIds()
    {
        auto const resourceId = theResourceManager->GetResourceId("TEAMTACTIC");
        retruxx::vector<int> result;
        thePrototypeManager->GetPrototypeIdsByResourceId(resourceId, result);
        return result;
    }

    ai::TeamTactic* TeamTacticManager::createTeamTactic(int protoId)
    {
        // RVA 0x8ADF30 - NOTE: the new object is taken to be a tactic without a type check.
        CStr const tacticName = CStr("Tactic") + CStr(protoId);
        int const objId = theObjects->CreateNewObject(protoId, tacticName.c_str(), -1, -1);
        return static_cast<TeamTactic*>(theObjects->GetEntityByObjId(objId));
    }

    void TeamTacticManager::AssignBestTacticAgainstVehicle(ai::Team* team, ai::Vehicle const* target)
    {
        auto const ids = getAllTacticsPrototypeIds();
        float maxValue = -1.0;
        int resultId = -1;
        for (auto const id : ids)
        {
            auto* tactic = createTeamTactic(id);
            auto const value = tactic->FitAgainstVehicle(team, target);
            if (value > maxValue)
            {
                resultId = id;
                maxValue = value;
            }
            tactic->Remove();
        }
        if (resultId != -1)
        {
            auto* tactic = createTeamTactic(resultId);
            team->SetTeamTactic(tactic);
            tactic->AssignAgainstVehicle(team, target);
        }
    }

    void TeamTacticManager::AssignBestTacticAgainstTeam(ai::Team* team, ai::Team const* target)
    {
        // RVA 0x8AE2A0 - tries every tactic against the target team and gives the team the one that
        // fits best (a first one only if it fits better than -1).
        auto const ids = getAllTacticsPrototypeIds();
        float maxValue = -1.0f;
        int resultId = -1;
        for (int const id : ids)
        {
            TeamTactic* tactic = createTeamTactic(id);
            float const value = tactic->FitAgainstTeam(team, target);
            if (value > maxValue)
            {
                resultId = id;
                maxValue = value;
            }
            tactic->Remove();
        }
        if (resultId != -1)
        {
            TeamTactic* tactic = createTeamTactic(resultId);
            team->SetTeamTactic(tactic);
            tactic->AssignAgainstTeam(team, target);
        }
    }

    void TeamTacticManager::AssignBestTacticAgainstObj(ai::Team* team, ai::Obj const* target)
    {
        // RVA 0x8AE360 - as AssignBestTacticAgainstTeam, against an object.
        auto const ids = getAllTacticsPrototypeIds();
        float maxValue = -1.0f;
        int resultId = -1;
        for (int const id : ids)
        {
            TeamTactic* tactic = createTeamTactic(id);
            float const value = tactic->FitAgainstObj(team, target);
            if (value > maxValue)
            {
                resultId = id;
                maxValue = value;
            }
            tactic->Remove();
        }
        if (resultId != -1)
        {
            TeamTactic* tactic = createTeamTactic(resultId);
            team->SetTeamTactic(tactic);
            tactic->AssignAgainstObj(team, target);
        }
    }

    void TeamTacticManager::AssignTacticAgainstVehicle(ai::Team* team, ai::Vehicle const* target, CStr const& tacticName)
    {
        // RVA 0x8AE060 - the named tactic, if there is one.
        int const id = thePrototypeManager->GetPrototypeId(tacticName);
        if (id != -1)
        {
            TeamTactic* tactic = createTeamTactic(id);
            team->SetTeamTactic(tactic);
            tactic->AssignAgainstVehicle(team, target);
        }
    }

    void TeamTacticManager::AssignTacticAgainstTeam(ai::Team* team, ai::Team const* target, CStr const& tacticName)
    {
        // RVA 0x8AE0B0
        int const id = thePrototypeManager->GetPrototypeId(tacticName);
        if (id != -1)
        {
            TeamTactic* tactic = createTeamTactic(id);
            team->SetTeamTactic(tactic);
            tactic->AssignAgainstTeam(team, target);
        }
    }

    void TeamTacticManager::AssignTacticAgainstObj(ai::Team* team, ai::Obj const* target, CStr const& tacticName)
    {
        // RVA 0x8AE100
        int const id = thePrototypeManager->GetPrototypeId(tacticName);
        if (id != -1)
        {
            TeamTactic* tactic = createTeamTactic(id);
            team->SetTeamTactic(tactic);
            tactic->AssignAgainstObj(team, target);
        }
    }
}  // namespace ai

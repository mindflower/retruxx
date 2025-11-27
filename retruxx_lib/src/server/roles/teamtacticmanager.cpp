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
        CStr const tacticName = "Tactic" + CStr(protoId);
        int const objId = theObjects->CreateNewObject(protoId, tacticName.c_str(), -1, -1);
        auto* obj = theObjects->GetEntityByObjId(objId);
        return RT_DYNCAST(obj, TeamTactic);
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

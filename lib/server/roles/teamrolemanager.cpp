#include "teamrolemanager.h"

#include "VehicleRole.h"
#include "graph/hungarianalgo.h"
#include "server/objects/team.h"
#include "server/objects/base/objcontainer.h"

#include <server/objects/base/prototypemanager.h>

namespace ai
{
    void TeamRoleManager::GetRolePrototypeIds(
        ai::TeamTacticWithRoles const* tactic,
        ai::Team const* team,
        retruxx::vector<int, retruxx::allocator<int>>& prototypeIds)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstVehicle(
        retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
        ai::TeamTacticWithRoles const* tactic,
        ai::Team const* team,
        ai::Vehicle const* target,
        float& value)
    {
        // TODO: generated code TeamRoleManager::getAgainstVehicle
        // Get the number of vehicles in the team
        size_t const numVehicles = team->GetNumVehicles();

        // Create a weight matrix for the assignment problem
        Graph::CSquareMatrix<float> weights(numVehicles);

        // Fill the weight matrix with compatibility scores
        for (size_t i = 0; i < numVehicles; ++i)
        {
            for (size_t j = 0; j < numVehicles; ++j)
            {
                // Get the role prototype
                ai::VehicleRolePrototypeInfo const* prototype = nullptr;
                if (!rolePrototypeIds.empty() && rolePrototypeIds[j] < ai::thePrototypeManager->GetNumOfPrototypes())
                {
                    prototype = RT_DYNCAST(ai::thePrototypeManager->GetPrototypeInfo(rolePrototypeIds[j]), VehicleRolePrototypeInfo const);
                }

                // Get the vehicle
                ai::Vehicle* vehicle = nullptr;
                if (i < numVehicles)
                {
                    vehicle = team->GetVehicle(i);
                }

                // Calculate compatibility score
                float score = 0.0f;
                if (prototype && vehicle)
                {
                    // Call the virtual function to get the compatibility score
                    score = prototype->FitAgainstVehicle(vehicle, target);
                }

                // Store in weight matrix
                weights(i, j) = score;
            }
        }

        // Find the optimal matching using maximum value matching
        Graph::getMaxValuedMatching(weights);

        // Calculate the matching value and get the result
        std::vector<int> matchingResult;
        value = Graph::getMatchingValue(weights, matchingResult);

        // Copy the result to output
        return matchingResult;
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstTeam(
        retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
        ai::TeamTacticWithRoles const* tactic,
        ai::Team const* team,
        ai::Team const* target,
        float& value)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstObj(
        retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
        ai::TeamTacticWithRoles const* tactic,
        ai::Team const* team,
        ai::Obj const* target,
        float& value)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamRoleManager::AssignAgainstVehicle(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Vehicle const* target)
    {
        std::vector<int> rolePrototypeIds;
        tactic->GetRolePrototypeIdsEx(team->GetNumVehicles(), rolePrototypeIds);

        float v = 0.0;
        std::vector<int> matching = getAgainstVehicle(rolePrototypeIds, tactic, team, target, v);
        for (int i =0; i < team->GetNumVehicles(); ++i)
        {
            auto const objId = theObjects->CreateNewObject(rolePrototypeIds[i], "", -1, -1);
            auto* role = RT_DYNCAST(theObjects->GetEntityByObjId(objId), VehicleRole);
            role->setTargetVehicle(target);
            team->GetVehicle(i)->SetRole(role);
        }
    }

    void TeamRoleManager::AssignAgainstTeam(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Team const* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamRoleManager::AssignAgainstObj(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Obj const* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float TeamRoleManager::FitAgainstVehicle(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Vehicle const* target)
    {
        std::vector<int> rolePrototypeIds;
        tactic->GetRolePrototypeIdsEx(team->GetNumVehicles(), rolePrototypeIds);

        float value = 0.0;
        getAgainstVehicle(rolePrototypeIds, tactic, team, target, value);
        return value;
    }

    float TeamRoleManager::FitAgainstTeam(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Team const* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float TeamRoleManager::FitAgainstObj(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Obj const* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamRoleManager::ClearRoles(ai::Team const* team)
    {
        for (auto& vehicle : team->GetVehicles())
        {
            auto role = vehicle->GetRole();
            if (role)
            {
                vehicle->SetRole(nullptr);
                vehicle->FireFromWeaponAI(false, 0.0, nullptr);
                role->Remove();
            }
        }
    }

    void TeamRoleManager::GetTargetsPositions(ai::Team const* team, retruxx::vector<CVector, retruxx::allocator<CVector>>& positions)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

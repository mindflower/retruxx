#include "teamrolemanager.h"

#include "VehicleRole.h"
#include "graph/hungarianalgo.h"
#include "server/objects/team.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"

#include <server/objects/base/prototypemanager.h>

namespace ai
{
    namespace
    {
        // The vehicle roles of a tactic are weighed against each vehicle of the team: entry (i, j)
        // is how well role j suits vehicle i. The best one-to-one pairing of roles with vehicles is
        // returned, and its total fit in value.
        template <class Fit>
        retruxx::vector<int> matchRoles(
            retruxx::vector<int> const& rolePrototypeIds, Team const* team, float& value, Fit fit)
        {
            int const numVehicles = static_cast<int>(team->GetVehicles().size());
            Graph::CSquareMatrix<float> weights(numVehicles);
            for (int i = 0; i < numVehicles; ++i)
            {
                for (int j = 0; j < numVehicles; ++j)
                {
                    // NOTE: the role prototype is taken without a type check.
                    auto const* role = static_cast<VehicleRolePrototypeInfo const*>(
                        thePrototypeManager->GetPrototypeInfo(rolePrototypeIds[j]));
                    weights(i, j) = fit(role, team->GetVehicles()[i]);
                }
            }
            retruxx::vector<int> matching = Graph::getMaxValuedMatching(weights);
            value = Graph::getMatchingValue(weights, matching);
            return matching;
        }

        // A new role of the given prototype, as the object container made it.
        VehicleRole* createRole(int prototypeId)
        {
            // NOTE: the new object is taken to be a vehicle role without a type check.
            return static_cast<VehicleRole*>(theObjects->GetEntityByObjId(theObjects->CreateNewObject(prototypeId, "", -1, -1)));
        }
    }  // namespace

    void TeamRoleManager::GetRolePrototypeIds(
        ai::TeamTacticWithRoles const* tactic,
        ai::Team const* team,
        retruxx::vector<int, retruxx::allocator<int>>& prototypeIds)
    {
        // RVA 0x8AC690
        tactic->GetRolePrototypeIdsEx(static_cast<int>(team->GetVehicles().size()), prototypeIds);
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstVehicle(
        retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
        ai::TeamTacticWithRoles const*,
        ai::Team const* team,
        ai::Vehicle const* target,
        float& value)
    {
        // RVA 0x8AD400
        return matchRoles(rolePrototypeIds, team, value, [target](VehicleRolePrototypeInfo const* role, Vehicle* v)
            { return role->FitAgainstVehicle(v, target); });
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstTeam(
        retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
        ai::TeamTacticWithRoles const*,
        ai::Team const* team,
        ai::Team const* target,
        float& value)
    {
        // RVA 0x8AD700 - the vehicle each role would pick in the target team is thrown away here.
        Vehicle* targetVehicle = nullptr;
        return matchRoles(rolePrototypeIds, team, value, [target, &targetVehicle](VehicleRolePrototypeInfo const* role, Vehicle* v)
            { return role->FitAgainstTeam(v, target, &targetVehicle); });
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstObj(
        retruxx::vector<int, retruxx::allocator<int>> const& rolePrototypeIds,
        ai::TeamTacticWithRoles const*,
        ai::Team const* team,
        ai::Obj const* target,
        float& value)
    {
        // RVA 0x8ADA50
        return matchRoles(rolePrototypeIds, team, value, [target](VehicleRolePrototypeInfo const* role, Vehicle* v)
            { return role->FitAgainstObj(v, target); });
    }

    void TeamRoleManager::AssignAgainstVehicle(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Vehicle const* target)
    {
        // RVA 0x8AD5A0 - every vehicle of the team gets a role aimed at the target.
        // NOTE: the best pairing of roles with vehicles is worked out, but the roles are then handed
        // out in the order they were drawn, so the pairing plays no part.
        retruxx::vector<int> rolePrototypeIds;
        tactic->GetRolePrototypeIdsEx(static_cast<int>(team->GetVehicles().size()), rolePrototypeIds);
        float value = 0.0f;
        retruxx::vector<int> const matching = getAgainstVehicle(rolePrototypeIds, tactic, team, target, value);
        for (int i = 0; i < static_cast<int>(team->GetVehicles().size()); ++i)
        {
            VehicleRole* role = createRole(rolePrototypeIds[i]);
            role->setTargetVehicle(target);
            team->GetVehicles()[i]->SetRole(role);
        }
    }

    void TeamRoleManager::AssignAgainstTeam(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Team const* target)
    {
        // RVA 0x8AD8A0 - every vehicle of the team gets a role aimed at the target team, and at the
        // vehicle of it that the role itself thinks best.
        // NOTE: as in AssignAgainstVehicle, the pairing is worked out and ignored.
        retruxx::vector<int> rolePrototypeIds;
        tactic->GetRolePrototypeIdsEx(static_cast<int>(team->GetVehicles().size()), rolePrototypeIds);
        float value = 0.0f;
        retruxx::vector<int> const matching = getAgainstTeam(rolePrototypeIds, tactic, team, target, value);
        for (int i = 0; i < static_cast<int>(team->GetVehicles().size()); ++i)
        {
            VehicleRole* role = createRole(rolePrototypeIds[i]);
            Vehicle* targetVehicle = nullptr;
            role->FitAgainstTeam(team->GetVehicles()[i], target, &targetVehicle);
            role->setTargetTeam(target);
            role->setTargetVehicle(targetVehicle);
            team->GetVehicles()[i]->SetRole(role);
        }
    }

    void TeamRoleManager::AssignAgainstObj(ai::TeamTacticWithRoles const* tactic, ai::Team* team, ai::Obj const* target)
    {
        // RVA 0x8ADBF0 - every vehicle of the team gets a role aimed at the target object.
        // NOTE: as in AssignAgainstVehicle, the pairing is worked out and ignored.
        retruxx::vector<int> rolePrototypeIds;
        tactic->GetRolePrototypeIdsEx(static_cast<int>(team->GetVehicles().size()), rolePrototypeIds);
        float value = 0.0f;
        retruxx::vector<int> const matching = getAgainstObj(rolePrototypeIds, tactic, team, target, value);
        for (int i = 0; i < static_cast<int>(team->GetVehicles().size()); ++i)
        {
            VehicleRole* role = createRole(rolePrototypeIds[i]);
            role->setTargetObj(target);
            team->GetVehicles()[i]->SetRole(role);
        }
    }

    float TeamRoleManager::FitAgainstVehicle(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Vehicle const* target)
    {
        // RVA 0x8ADD50 - how well the tactic's roles, best paired, suit the team against the target.
        retruxx::vector<int> rolePrototypeIds;
        tactic->GetRolePrototypeIdsEx(static_cast<int>(team->GetVehicles().size()), rolePrototypeIds);
        float value = 0.0f;
        getAgainstVehicle(rolePrototypeIds, tactic, team, target, value);
        return value;
    }

    float TeamRoleManager::FitAgainstTeam(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Team const* target)
    {
        // RVA 0x8ADDF0
        retruxx::vector<int> rolePrototypeIds;
        tactic->GetRolePrototypeIdsEx(static_cast<int>(team->GetVehicles().size()), rolePrototypeIds);
        float value = 0.0f;
        getAgainstTeam(rolePrototypeIds, tactic, team, target, value);
        return value;
    }

    float TeamRoleManager::FitAgainstObj(ai::TeamTacticWithRoles const* tactic, ai::Team const* team, ai::Obj const* target)
    {
        // RVA 0x8ADE90
        retruxx::vector<int> rolePrototypeIds;
        tactic->GetRolePrototypeIdsEx(static_cast<int>(team->GetVehicles().size()), rolePrototypeIds);
        float value = 0.0f;
        getAgainstObj(rolePrototypeIds, tactic, team, target, value);
        return value;
    }

    void TeamRoleManager::ClearRoles(ai::Team const* team)
    {
        // RVA 0x8AC7E0 - takes every vehicle's role away, stopping its fire.
        for (Vehicle* vehicle : team->GetVehicles())
        {
            if (VehicleRole* role = vehicle->GetRole())
            {
                vehicle->SetRole(nullptr);
                vehicle->FireFromWeaponAI(false, 0.0f, nullptr);
                role->Remove();
            }
        }
    }

    void TeamRoleManager::GetTargetsPositions(ai::Team const* team, retruxx::vector<CVector, retruxx::allocator<CVector>>& positions)
    {
        // RVA 0x8ACD30 - where the targets of the team's roles are.
        positions.clear();
        positions.reserve(team->GetVehicles().size());
        for (Vehicle* vehicle : team->GetVehicles())
        {
            if (VehicleRole* role = vehicle->GetRole())
            {
                if (Obj* target = role->getTargetObj())
                {
                    positions.push_back(getPhysicObjOrPhysicBodyPosition(target));
                }
            }
        }
    }
}  // namespace ai

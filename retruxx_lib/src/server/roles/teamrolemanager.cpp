#include "teamrolemanager.h"

#include "VehicleRole.h"
#include "server/objects/team.h"

namespace ai
{
    void TeamRoleManager::GetRolePrototypeIds(const ai::TeamTacticWithRoles* tactic, const ai::Team* team,
        retruxx::vector<int, retruxx::allocator<int>>& prototypeIds)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstVehicle(
        const retruxx::vector<int, retruxx::allocator<int>>& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic,
        const ai::Team* team, const ai::Vehicle* target, float& value)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstTeam(
        const retruxx::vector<int, retruxx::allocator<int>>& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic,
        const ai::Team* team, const ai::Team* target, float& value)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstObj(
        const retruxx::vector<int, retruxx::allocator<int>>& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic,
        const ai::Team* team, const ai::Obj* target, float& value)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamRoleManager::AssignAgainstVehicle(const ai::TeamTacticWithRoles* tactic, ai::Team* team,
        const ai::Vehicle* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamRoleManager::AssignAgainstTeam(const ai::TeamTacticWithRoles* tactic, ai::Team* team,
        const ai::Team* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamRoleManager::AssignAgainstObj(const ai::TeamTacticWithRoles* tactic, ai::Team* team, const ai::Obj* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float TeamRoleManager::FitAgainstVehicle(const ai::TeamTacticWithRoles* tactic, const ai::Team* team,
        const ai::Vehicle* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float TeamRoleManager::FitAgainstTeam(const ai::TeamTacticWithRoles* tactic, const ai::Team* team,
        const ai::Team* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float TeamRoleManager::FitAgainstObj(const ai::TeamTacticWithRoles* tactic, const ai::Team* team,
        const ai::Obj* target)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamRoleManager::ClearRoles(const ai::Team* team)
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

    void TeamRoleManager::GetTargetsPositions(const ai::Team* team,
        retruxx::vector<CVector, retruxx::allocator<CVector>>& positions)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

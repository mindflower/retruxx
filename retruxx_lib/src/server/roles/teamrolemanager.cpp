#include "teamrolemanager.h"

namespace ai
{
    void TeamRoleManager::GetRolePrototypeIds(const ai::TeamTacticWithRoles* tactic, const ai::Team* team,
        retruxx::vector<int, retruxx::allocator<int>>& prototypeIds)
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstVehicle(
        const retruxx::vector<int, retruxx::allocator<int>>& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic,
        const ai::Team* team, const ai::Vehicle* target, float& value)
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstTeam(
        const retruxx::vector<int, retruxx::allocator<int>>& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic,
        const ai::Team* team, const ai::Team* target, float& value)
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::vector<int, retruxx::allocator<int>> TeamRoleManager::getAgainstObj(
        const retruxx::vector<int, retruxx::allocator<int>>& rolePrototypeIds, const ai::TeamTacticWithRoles* tactic,
        const ai::Team* team, const ai::Obj* target, float& value)
    {
        throw std::logic_error("Not implemented");
    }

    void TeamRoleManager::AssignAgainstVehicle(const ai::TeamTacticWithRoles* tactic, ai::Team* team,
        const ai::Vehicle* target)
    {
        throw std::logic_error("Not implemented");
    }

    void TeamRoleManager::AssignAgainstTeam(const ai::TeamTacticWithRoles* tactic, ai::Team* team,
        const ai::Team* target)
    {
        throw std::logic_error("Not implemented");
    }

    void TeamRoleManager::AssignAgainstObj(const ai::TeamTacticWithRoles* tactic, ai::Team* team, const ai::Obj* target)
    {
        throw std::logic_error("Not implemented");
    }

    float TeamRoleManager::FitAgainstVehicle(const ai::TeamTacticWithRoles* tactic, const ai::Team* team,
        const ai::Vehicle* target)
    {
        throw std::logic_error("Not implemented");
    }

    float TeamRoleManager::FitAgainstTeam(const ai::TeamTacticWithRoles* tactic, const ai::Team* team,
        const ai::Team* target)
    {
        throw std::logic_error("Not implemented");
    }

    float TeamRoleManager::FitAgainstObj(const ai::TeamTacticWithRoles* tactic, const ai::Team* team,
        const ai::Obj* target)
    {
        throw std::logic_error("Not implemented");
    }

    void TeamRoleManager::ClearRoles(const ai::Team* team)
    {
        throw std::logic_error("Not implemented");
    }

    void TeamRoleManager::GetTargetsPositions(const ai::Team* team,
        retruxx::vector<CVector, retruxx::allocator<CVector>>& positions)
    {
        throw std::logic_error("Not implemented");
    }
}

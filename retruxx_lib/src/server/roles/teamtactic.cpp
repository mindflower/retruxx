#include "teamtactic.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(TeamTactic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(TeamTactic);

    RT_CLASS_EXPORTS_BEGIN(TeamTacticWithRoles)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(TeamTacticWithRoles);

    Obj* TeamTacticPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    TeamTacticPrototypeInfo::TeamTacticPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool TeamTacticPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TeamTactic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    TeamTactic::TeamTactic(TeamTacticPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    TeamTacticPrototypeInfo const* TeamTactic::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TeamTactic::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    TeamTactic::~TeamTactic()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TeamTactic::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TeamTactic::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    TeamTacticWithRolesPrototypeInfo::TeamTacticWithRolesPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* TeamTacticWithRolesPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<int> const& TeamTacticWithRolesPrototypeInfo::GetPrototypeIds() const
    {
        throw std::logic_error("Not implemented");
    }

    bool TeamTacticWithRolesPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void TeamTacticWithRolesPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void TeamTacticWithRoles::AssignAgainstObj(Team*, Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    TeamTacticWithRoles::TeamTacticWithRoles(TeamTacticWithRolesPrototypeInfo const& prototype) : TeamTactic(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TeamTacticWithRoles::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    TeamTacticWithRolesPrototypeInfo const* TeamTacticWithRoles::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void TeamTacticWithRoles::AssignAgainstVehicle(Team*, Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    void TeamTacticWithRoles::AssignAgainstTeam(Team*, Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    float TeamTacticWithRoles::FitAgainstVehicle(Team const*, Vehicle const*)
    {
        throw std::logic_error("Not implemented");
    }

    void TeamTacticWithRoles::GetRolePrototypeIdsEx(int, std::vector<int, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TeamTacticWithRoles::GetBaseClass()
    {
        return RT_CLASS_LOCAL(TeamTactic);
    }

    float TeamTacticWithRoles::FitAgainstTeam(Team const*, Team const*)
    {
        throw std::logic_error("Not implemented");
    }

    float TeamTacticWithRoles::FitAgainstObj(Team const*, Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    TeamTacticWithRoles::~TeamTacticWithRoles()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TeamTacticWithRoles::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<int, std::allocator<int>> const& TeamTacticWithRoles::GetRolePrototypeIds() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TeamTacticWithRoles::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

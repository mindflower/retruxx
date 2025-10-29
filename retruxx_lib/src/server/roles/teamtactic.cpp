#include "teamtactic.h"

#include <stdexcept>

#include "core/ini.h"
#include "core/ref_ptr.h"

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    TeamTacticPrototypeInfo::TeamTacticPrototypeInfo()
    {
    }

    bool TeamTacticPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    m3d::Class* TeamTactic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    TeamTactic::TeamTactic(TeamTacticPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TeamTacticPrototypeInfo const* TeamTactic::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* TeamTactic::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TeamTactic::~TeamTactic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TeamTactic::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TeamTactic::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TeamTacticWithRolesPrototypeInfo::TeamTacticWithRolesPrototypeInfo()
    {
    }

    Obj* TeamTacticWithRolesPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    std::vector<int> const& TeamTacticWithRolesPrototypeInfo::GetPrototypeIds() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool TeamTacticWithRolesPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ref_ptr node = xmlFile->CreateNode();
            for (xmlNode->GetFirstChild(node, "Role"); !node->IsEmpty(); node->GetNextSibling(node, "Role"))
            {
                CStr prototype;
                m3d::SafeStrAttrib(prototype, node, "Prototype");
                m_rolePrototypeNames.push_back(std::move(prototype));
            }
        }
        return result;
    }

    void TeamTacticWithRolesPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticWithRoles::AssignAgainstObj(Team*, Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TeamTacticWithRoles::TeamTacticWithRoles(TeamTacticWithRolesPrototypeInfo const& prototype) : TeamTactic(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* TeamTacticWithRoles::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TeamTacticWithRolesPrototypeInfo const* TeamTacticWithRoles::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticWithRoles::AssignAgainstVehicle(Team*, Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticWithRoles::AssignAgainstTeam(Team*, Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float TeamTacticWithRoles::FitAgainstVehicle(Team const*, Vehicle const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TeamTacticWithRoles::GetRolePrototypeIdsEx(int, std::vector<int, std::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* TeamTacticWithRoles::GetBaseClass()
    {
        return RT_CLASS_LOCAL(TeamTactic);
    }

    float TeamTacticWithRoles::FitAgainstTeam(Team const*, Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float TeamTacticWithRoles::FitAgainstObj(Team const*, Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TeamTacticWithRoles::~TeamTacticWithRoles()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TeamTacticWithRoles::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    std::vector<int, std::allocator<int>> const& TeamTacticWithRoles::GetRolePrototypeIds() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TeamTacticWithRoles::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

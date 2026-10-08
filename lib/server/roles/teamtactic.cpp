#include "teamtactic.h"

#include "teamrolemanager.h"

#include <stdexcept>

#include "core/ini.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include <server/objects/base/prototypemanager.h>
#include <algorithm>
#include <cstdlib>

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
        // RVA 0x800A30 - a plain tactic cannot be made; only tactics with roles.
        return nullptr;
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

    TeamTactic::TeamTactic(TeamTacticPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
    }

    TeamTacticPrototypeInfo const* TeamTactic::GetPrototypeInfo() const
    {
        // RVA 0x801630 - NOTE: the prototype is cast without a type check.
        return static_cast<TeamTacticPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* TeamTactic::GetClass() const
    {
        return RT_CLASS_LOCAL(TeamTactic);
    }

    TeamTactic::~TeamTactic() = default;

    m3d::Object* TeamTactic::CreateObject()
    {
        // RVA 0x800E30
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* TeamTactic::Clone()
    {
        // RVA 0x800C70
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    TeamTacticWithRolesPrototypeInfo::TeamTacticWithRolesPrototypeInfo() = default;

    Obj* TeamTacticWithRolesPrototypeInfo::CreateTargetObject() const
    {
        return new TeamTacticWithRoles(*this);
    }

    std::vector<int> const& TeamTacticWithRolesPrototypeInfo::GetPrototypeIds() const
    {
        return m_rolePrototypeIds;
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
        for (auto const& roleName :m_rolePrototypeNames)
        {
            auto const id = thePrototypeManager->GetPrototypeId(roleName);
            if (id != -1)
            {
                m_rolePrototypeIds.push_back(id);
            }
            else
            {
                M3D_LOG_INFO("Unknown role prototype: " + roleName);
            }
        }
    }

    void TeamTacticWithRoles::AssignAgainstObj(Team* v, Obj const* target)
    {
        // RVA 0x800B70
        ai::TeamRoleManager::AssignAgainstObj(this, v, target);
    }

    TeamTacticWithRoles::TeamTacticWithRoles(TeamTacticWithRolesPrototypeInfo const& prototype) : TeamTactic(prototype)
    {
    }

    m3d::Class* TeamTacticWithRoles::GetClass() const
    {
        return RT_CLASS_LOCAL(TeamTacticWithRoles);
    }

    TeamTacticWithRolesPrototypeInfo const* TeamTacticWithRoles::GetPrototypeInfo() const
    {
        // RVA 0x801660 - NOTE: the prototype is cast without a type check.
        return static_cast<TeamTacticWithRolesPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void TeamTacticWithRoles::AssignAgainstVehicle(Team* v, Vehicle const* target)
    {
        ai::TeamRoleManager::AssignAgainstVehicle(this, v, target);
    }

    void TeamTacticWithRoles::AssignAgainstTeam(Team* v, Team const* target)
    {
        // RVA 0x800B50
        ai::TeamRoleManager::AssignAgainstTeam(this, v, target);
    }

    float TeamTacticWithRoles::FitAgainstVehicle(Team const* v, Vehicle const* target)
    {
        return ai::TeamRoleManager::FitAgainstVehicle(this, v, target);
    }

    void TeamTacticWithRoles::GetRolePrototypeIdsEx(int vehicleNum, std::vector<int, std::allocator<int>>& prototypeIds) const
    {
        // RVA 0x801A70 - one role for each of vehicleNum vehicles: the tactic's roles repeated as
        // often as needed, and when that gives too many, shuffled and cut down to size.
        // NOTE: a tactic with no roles never fills the list, and the loop does not end.
        std::vector<int> const& rolePrototypeIds = GetRolePrototypeIds();
        prototypeIds.clear();
        while (vehicleNum > static_cast<int>(prototypeIds.size()))
        {
            prototypeIds.insert(prototypeIds.end(), rolePrototypeIds.begin(), rolePrototypeIds.end());
        }
        if (vehicleNum < static_cast<int>(prototypeIds.size()))
        {
            // std::random_shuffle of the shipped runtime (RVA 0x801590): element k is swapped with a
            // random one of the first k + 1, using 15 bits of rand().
            // NOTE: past 32768 elements the runtime widens the number with set bits, not more rand().
            for (unsigned index = 2; index <= prototypeIds.size(); ++index)
            {
                unsigned long range = 0x7FFF;
                unsigned long r = static_cast<unsigned long>(rand()) & 0x7FFF;
                while (range < index && range != ~0UL)
                {
                    range = (range << 15) | 0x7FFF;
                    r = (r << 15) | 0x7FFF;
                }
                std::swap(prototypeIds[index - 1], prototypeIds[r % index]);
            }
            prototypeIds.erase(prototypeIds.begin() + vehicleNum, prototypeIds.end());
        }
    }

    m3d::Class* TeamTacticWithRoles::GetBaseClass()
    {
        return RT_CLASS_LOCAL(TeamTactic);
    }

    float TeamTacticWithRoles::FitAgainstTeam(Team const* v, Team const* target)
    {
        // RVA 0x800AF0
        return ai::TeamRoleManager::FitAgainstTeam(this, v, target);
    }

    float TeamTacticWithRoles::FitAgainstObj(Team const* v, Obj const* target)
    {
        // RVA 0x800B10
        return ai::TeamRoleManager::FitAgainstObj(this, v, target);
    }

    TeamTacticWithRoles::~TeamTacticWithRoles() = default;

    m3d::Object* TeamTacticWithRoles::CreateObject()
    {
        // RVA 0x8011B0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    std::vector<int, std::allocator<int>> const& TeamTacticWithRoles::GetRolePrototypeIds() const
    {
        auto const* prototypeInfo = GetPrototypeInfo();
        M3D_ASSERT(prototypeInfo);
        return prototypeInfo->GetPrototypeIds();
    }

    m3d::Object* TeamTacticWithRoles::Clone()
    {
        // RVA 0x800FF0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}

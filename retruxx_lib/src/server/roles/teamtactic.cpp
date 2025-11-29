#include "teamtactic.h"

#include "teamrolemanager.h"

#include <stdexcept>

#include "core/ini.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include <server/objects/base/prototypemanager.h>
#include <algorithm>
#include <random>

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

    TeamTactic::TeamTactic(TeamTacticPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
    }

    TeamTacticPrototypeInfo const* TeamTactic::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* TeamTactic::GetClass() const
    {
        return RT_CLASS_LOCAL(TeamTactic);
    }

    TeamTactic::~TeamTactic() = default;

    m3d::Object* TeamTactic::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TeamTactic::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void TeamTacticWithRoles::AssignAgainstObj(Team*, Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), TeamTacticWithRolesPrototypeInfo const);
    }

    void TeamTacticWithRoles::AssignAgainstVehicle(Team* v, Vehicle const* target)
    {
        ai::TeamRoleManager::AssignAgainstVehicle(this, v, target);
    }

    void TeamTacticWithRoles::AssignAgainstTeam(Team*, Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float TeamTacticWithRoles::FitAgainstVehicle(Team const* v, Vehicle const* target)
    {
        return ai::TeamRoleManager::FitAgainstVehicle(this, v, target);
    }

    void TeamTacticWithRoles::GetRolePrototypeIdsEx(int vehicleNum, std::vector<int, std::allocator<int>>& prototypeIds) const
    {
        // TODO: generated code TeamTacticWithRoles::GetRolePrototypeIdsEx
        // Get role prototype IDs (returns a vector reference)
        std::vector<int> const& rolePrototypeIds = this->GetRolePrototypeIds();

        // Clear the output vector using the allocator
        prototypeIds.clear();

        // Fill the vector until it has at least vehicleNum elements
        while (prototypeIds.size() <= static_cast<size_t>(vehicleNum))
        {
            // Insert the entire rolePrototypeIds vector at the end
            prototypeIds.insert(prototypeIds.end(), rolePrototypeIds.begin(), rolePrototypeIds.end());
        }

        // If we have more elements than needed
        if (static_cast<size_t>(vehicleNum) < prototypeIds.size())
        {
            if (!prototypeIds.empty())
            {
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(prototypeIds.begin(), prototypeIds.end(), g);

                // Remove excess elements beyond vehicleNum
                prototypeIds.resize(vehicleNum);
            }
        }
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

    TeamTacticWithRoles::~TeamTacticWithRoles() = default;

    m3d::Object* TeamTacticWithRoles::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    std::vector<int, std::allocator<int>> const& TeamTacticWithRoles::GetRolePrototypeIds() const
    {
        auto const* prototypeInfo = GetPrototypeInfo();
        M3D_ASSERT(prototypeInfo);
        return prototypeInfo->GetPrototypeIds();
    }

    m3d::Object* TeamTacticWithRoles::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

#include "infectionteam.h"
#include <stdexcept>

#include "core/ini.h"
#include "base/prototypemanager.h"
#include <core/kernel.h>
#include <core/log.h>
#include <cmath>
#include <cstdlib>
#include "base/objcontainer.h"
#include "physicbodies/physichelpers.h"
#include "player.h"
#include "vehicle.h"
#include "vehiclesgenerator.h"
#include "../ai/ai.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(InfectionTeam)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(InfectionTeam);

    void InfectionTeam::SetCriticalDistAndTime(float criticalDist, float criticalTime)
    {
        // RVA 0x83B3F0
        m_criticalTeamDist = criticalDist;
        m_criticalTeamTime = criticalTime;
    }

    void InfectionTeam::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x653B70
        Team::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_bGenerated, xmlNode, "Generated");
        m3d::SafeFloatAttrib(m_timeBeyondCriticalDist, xmlNode, "TimeBeyondCriticalDist");
        m3d::SafeFloatAttrib(m_timeBeyondBlindDist, xmlNode, "TimeBeyondBlindDist");
    }

    InfectionTeam::InfectionTeam(InfectionTeamPrototypeInfo const& prototypeInfo) : Team(prototypeInfo)
    {
        // RVA 0x653830
        m_bGenerated = false;
        m_bRemoveWhenChildrenDead = false;
        m_criticalTeamDist = 1000000.0f;
        m_criticalTeamTime = 0.0f;
        m_timeBeyondCriticalDist = 0.0f;
        m_blindTeamDist = 1000000.0f;
        m_blindTeamTime = 0.0f;
        m_timeBeyondBlindDist = 0.0f;
    }

    void InfectionTeam::SetBlindDistAndTime(float blindDist, float blindTime)
    {
        // RVA 0x83B410
        m_blindTeamDist = blindDist;
        m_blindTeamTime = blindTime;
    }

    m3d::Class* InfectionTeam::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Team);
    }

    void InfectionTeam::Generate(std::vector<CVector, std::allocator<CVector>> const& points, unsigned restSchwarz)
    {
        // RVA 0x654EE0 - first the fixed vehicles listed in the prototype, then whatever the
        // vehicles generator makes for the remaining strength. With more than two kinds of vehicle
        // the fixed ones may be split between the first and the last drop out point.
        m_bGenerated = true;
        InfectionTeamPrototypeInfo const* prototype = GetPrototypeInfo();

        bool useTwoPoints = false;
        if (prototype->m_items.size() > 2)
        {
            useTwoPoints = static_cast<double>(prototype->m_items.size()) * 0.25 > static_cast<double>(rand()) * 0.000030518509;
        }

        unsigned int totalCount = 0;
        for (auto const& item : prototype->m_items)
        {
            totalCount += item.m_count;
        }

        int cnt = 0;
        for (auto const& item : prototype->m_items)
        {
            int const protoId = thePrototypeManager->GetPrototypeId(item.m_protoName);
            for (unsigned int j = 0; j < item.m_count; ++j)
            {
                ++cnt;
                CStr const vehicleName = CStr(GetName()) + CStr(cnt);
                // NOTE: the vehicle is used without a type or null check.
                auto* vehicle = static_cast<Vehicle*>(
                    theObjects->GetEntityByObjId(theObjects->CreateNewObject(protoId, vehicleName.c_str(), -1, -1)));

                CVector pos = useTwoPoints && static_cast<unsigned int>(cnt) > totalCount / 2 ? points.back() : points.front();
                CVector newPos;
                if (GetValidPosition(pos, vehicle->GetIntersectionRadius(), 0xFF, newPos, false, false, std::set<m3d::Class*>()))
                {
                    pos = newPos;
                }
                vehicle->SetGamePositionOnGround(pos, true, false);
                vehicle->SetRandomSkin();
                AddChild(vehicle);
            }
        }

        if (prototype->m_vehiclesGeneratorProtoId != -1)
        {
            // NOTE: the generator prototype is cast without a type or null check.
            auto const* generator = static_cast<VehiclesGeneratorPrototypeInfo const*>(
                thePrototypeManager->GetPrototypeInfo(prototype->m_vehiclesGeneratorProtoId));
            retruxx::vector<int> newVehicles;
            generator->GenerateAndPlace(points, restSchwarz, newVehicles);
            for (int vehicleId : newVehicles)
            {
                // NOTE: unlike the fixed vehicles, these get their skin after joining the team,
                // and the object is used without a type or null check.
                Obj* vehicle = theObjects->GetEntityByObjId(vehicleId);
                AddChild(vehicle);
                static_cast<Vehicle*>(vehicle)->SetRandomSkin();
            }
        }
    }

    m3d::Class* InfectionTeam::GetClass() const
    {
        // RVA 0x653820
        return RT_CLASS_LOCAL(InfectionTeam);
    }

    InfectionTeamPrototypeInfo const* InfectionTeam::GetPrototypeInfo() const
    {
        // RVA 0x6548F0 - NOTE: the prototype is cast without a type check.
        return static_cast<InfectionTeamPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void InfectionTeam::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x654540
        Team::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("Generated", CStr(static_cast<int>(m_bGenerated)).c_str());
        xmlNode->SetAttribute("TimeBeyondCriticalDist", CStr(m_timeBeyondCriticalDist).c_str());
        xmlNode->SetAttribute("TimeBeyondBlindDist", CStr(m_timeBeyondBlindDist).c_str());
    }

    void InfectionTeam::_InternalPostLoad()
    {
        // RVA 0x6538C0
        Team::_InternalPostLoad();
    }

    InfectionTeamPrototypeInfo::Item::Item(CStr const& protoName, int count) : m_protoName(protoName), m_count(count)
    {
    }

    InfectionTeamPrototypeInfo::InfectionTeamPrototypeInfo()
    {
    }

    bool InfectionTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::TeamPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(m_vehiclesGeneratorProtoName, xmlNode, "VehiclesGenerator");
            m_items.clear();

            ref_ptr vehiclesNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(vehiclesNode, "Vehicles");

            ref_ptr node = xmlFile->CreateNode();
            for (vehiclesNode->GetFirstChild(node, "Vehicle"); !node->IsEmpty(); node->GetNextSibling(node, "Vehicle"))
            {
                CStr protoName;
                unsigned count = 0;
                m3d::SafeStrAttrib(protoName, node, "PrototypeName");
                m3d::SafeUintAttrib(count, node, "Count");
                m_items.emplace_back(protoName, count);
            }
        }
        return result;
    }

    void InfectionTeamPrototypeInfo::PostLoad()
    {
        TeamPrototypeInfo::PostLoad();
        m_vehiclesGeneratorProtoId = thePrototypeManager->GetPrototypeId(m_vehiclesGeneratorProtoName);
        if (m_vehiclesGeneratorProtoId == -1 && m_items.empty())
        {
            if (m_vehiclesGeneratorProtoName.empty())
            {
                M3D_CRITICAL_ERROR("no vehicle generator and no vehicles for InfectionTeam '" + m_prototypeName + "'");
            }
            else
            {
                M3D_CRITICAL_ERROR("Unknown VehiclesGenerator: '" + m_vehiclesGeneratorProtoName + "' for infection team '" + m_prototypeName + "'");
            }
        }
    }

    ai::Obj* InfectionTeamPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x653B10
        return new InfectionTeam(*this);
    }

    InfectionTeam::~InfectionTeam() = default;

    void InfectionTeam::_TeamUpdate(float elapsedTime, unsigned workTime)
    {
        // RVA 0x655790 - the team measures how far its nearest vehicle is from the player. Beyond
        // the blind distance for long enough, it is sent a command (3) once. Beyond the critical
        // distance for long enough, all its vehicles are removed.
        Team::_TeamUpdate(elapsedTime, workTime);

        // NOTE: thePlayer is dereferenced without a null check.
        Vehicle* playerVehicle = thePlayer->GetVehicle();
        if (!playerVehicle)
        {
            return;
        }

        CVector const playerPos = playerVehicle->GetPosition();
        float distToPlayer = 1000000.0f;
        for (Vehicle* vehicle : GetVehicles())
        {
            CVector const pos = vehicle->GetPosition();
            double const dx = double(playerPos.x) - pos.x;
            double const dy = double(playerPos.y) - pos.y;
            double const dz = double(playerPos.z) - pos.z;
            float const dist = static_cast<float>(std::sqrt(dz * dz + dy * dy + dx * dx));
            distToPlayer = distToPlayer <= dist ? distToPlayer : dist;
        }

        if (m_blindTeamDist > distToPlayer || GetVehicles().empty())
        {
            m_timeBeyondBlindDist = 0.0f;
        }
        else
        {
            m_timeBeyondBlindDist = m_timeBeyondBlindDist + elapsedTime;
        }
        if (m_timeBeyondBlindDist > m_blindTeamTime)
        {
            GetAIPtr()->PutCommand(3, m3d::AIParam(), m3d::AIParam(), m3d::AIParam());
            // Pushed far below zero so the command is not repeated while the team stays away.
            m_timeBeyondBlindDist = -100000000.0f;
        }

        if (m_criticalTeamDist > distToPlayer || GetVehicles().empty())
        {
            m_timeBeyondCriticalDist = 0.0f;
        }
        else
        {
            m_timeBeyondCriticalDist = m_timeBeyondCriticalDist + elapsedTime;
        }
        // NOTE: the comparison is inclusive and the timer is reset to 0 while the team is close,
        // so a critical time of 0 removes the vehicles on every update however near they are.
        if (m_timeBeyondCriticalDist >= m_criticalTeamTime)
        {
            for (unsigned int i = 0; i < GetVehicles().size(); ++i)
            {
                GetVehicles()[i]->Remove();
            }
            m_timeBeyondCriticalDist = 0.0f;
        }
    }

    m3d::Object* InfectionTeam::CreateObject()
    {
        // RVA 0x653F80
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* InfectionTeam::Clone()
    {
        // RVA 0x653DC0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

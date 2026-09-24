#include "caravanteam.h"

#include <algorithm>
#include <cstdlib>

#include "core/log.h"
#include "core/kernel.h"
#include <core/ini.h>
#include "base/globalproperties.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "player.h"
#include "vehicle.h"
#include "wanderersgenerator.h"
#include "../event.h"
#include "../processmanager.h"
#include "../utils.h"

namespace ai
{
    namespace
    {
        // Puts one to three of a random ware from the caravan's list into a trader's repository.
        struct AddWarePred
        {
            explicit AddWarePred(retruxx::vector<CStr> const& waresPrototypes) : m_waresPrototypes(&waresPrototypes)
            {
            }

            void operator()(int vehicleId)
            {
                // NOTE: the vehicle is used without a null check, and an empty ware list divides
                // by zero.
                auto* vehicle = static_cast<Vehicle*>(theObjects->GetEntityByObjId(vehicleId));
                int const wareIndex = rand() % static_cast<int>(m_waresPrototypes->size());
                int const amount = rand() % 3 + 1;
                vehicle->AddItemsToRepository((*m_waresPrototypes)[wareIndex].c_str(), amount);
            }

            retruxx::vector<CStr> const* m_waresPrototypes;
        };
    }  // namespace

    RT_CLASS_EXPORTS_BEGIN(CaravanTeam)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CaravanTeam);

    CaravanTeamPrototypeInfo::CaravanTeamPrototypeInfo()
    {
        m_bRemoveWhenChildrenDead = 1;
        m_formationPrototypeName = "caravanFormation";
    }

    std::vector<CStr, std::allocator<CStr>> const& CaravanTeamPrototypeInfo::GetWaresPrototypes() const
    {
        // RVA 0x8304C0
        return m_waresPrototypes;
    }

    Obj* CaravanTeamPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x8317F0
        return new CaravanTeam(*this);
    }

    void CaravanTeamPrototypeInfo::PostLoad()
    {
        // RVA 0x830A20 - either generator may be left out, but a named one has to exist.
        TeamPrototypeInfo::PostLoad();

        m_tradersGeneratorPrototypeId = thePrototypeManager->GetPrototypeId(m_tradersGeneratorPrototypeName);
        if (m_tradersGeneratorPrototypeId == -1 && !m_tradersGeneratorPrototypeName.empty())
        {
            M3D_CRITICAL_ERROR(
                CStr("Unknown VehiclesGenerator for traders: '") + m_tradersGeneratorPrototypeName +
                CStr("' for caravan team '") + m_prototypeName + CStr("'"));
        }

        m_guardsGeneratorPrototypeId = thePrototypeManager->GetPrototypeId(m_guardsGeneratorPrototypeName);
        if (m_guardsGeneratorPrototypeId == -1 && !m_guardsGeneratorPrototypeName.empty())
        {
            M3D_CRITICAL_ERROR(
                CStr("Unknown VehiclesGenerator for guards: '") + m_guardsGeneratorPrototypeName +
                CStr("' for caravan team '") + m_prototypeName + CStr("'"));
        }
    }

    bool CaravanTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = TeamPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(m_tradersGeneratorPrototypeName, xmlNode, "TradersVehiclesGeneratorName");
            m3d::SafeStrAttrib(m_guardsGeneratorPrototypeName, xmlNode, "GuardVehiclesGeneratorName");

            CStr strWares;
            m3d::SafeStrAttrib(strWares, xmlNode, "WaresPrototypes");
            m3d::Tokenize(strWares, m_waresPrototypes, "(), ;\t");

            if (!m_tradersGeneratorPrototypeName.empty() && m_waresPrototypes.empty())
            {
                M3D_CRITICAL_ERROR("no wares for caravan with traders: " + m_prototypeName + "'");
            }
        }
        return result;
    }

    void CaravanTeam::SetWaitingPlayerToMoveout()
    {
        // RVA 0x8304D0
        m_waitingForPlayerToMoveout = true;
    }

    CaravanTeamPrototypeInfo const* CaravanTeam::GetPrototypeInfo() const
    {
        // RVA 0x8311A0 - NOTE: the prototype is cast without a type check.
        return static_cast<CaravanTeamPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* CaravanTeam::GetClass() const
    {
        // RVA 0x8304A0
        return RT_CLASS_LOCAL(CaravanTeam);
    }

    int CaravanTeam::OnEvent(Event const& evn)
    {
        // RVA 0x8325D0
        int const result = Team::OnEvent(evn);
        if (evn.m_eventId == GE_ENEMY_DESTROYED)
        {
            _OnEnemyDestroyed(evn);
            return 1;
        }
        return result;
    }

    void CaravanTeam::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x830550
        Team::LoadFromXML(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_waitingForPlayerToMoveout, xmlNode, "WaitingForPlayerToMoveout");
    }

    CaravanTeam::CaravanTeam(CaravanTeamPrototypeInfo const& prototype) : Team(prototype)
    {
        // RVA 0x831270 - the caravan decides for itself when it is finished (see
        // _RemoveUnlessChildrenExist), since its guards may have moved to another team.
        m_bRemoveWhenChildrenDead = false;
        m_bUseStandardUpdatingBehavior = false;
        m_waitingForPlayerToMoveout = false;
        m_guardTeamId = -1;
        m_curAttackerId = -1;
    }

    void CaravanTeam::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8305E0
        Team::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_guardTeamId, xmlNode, "GuardTeamId");
        CStr strGuardVehicleIds;
        m3d::SafeStrAttrib(strGuardVehicleIds, xmlNode, "GuardVehicles");
        StrToIntVector(strGuardVehicleIds, m_guardVehiclesIds);
        m3d::SafeIntAttrib(m_curAttackerId, xmlNode, "CurAttackerId");
    }

    void CaravanTeam::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x830580
        Team::SaveToXML(xmlFile, xmlNode);
        xmlNode->SetAttribute("WaitingForPlayerToMoveout", CStr(static_cast<int>(m_waitingForPlayerToMoveout)).c_str());
    }

    void CaravanTeam::GenerateAndPlace(CVector const& start)
    {
        // RVA 0x831A10 - traders (each given some wares) and then guards, all dropped on the
        // ground at start; the guards are remembered so they can be split off when attacked.
        CaravanTeamPrototypeInfo const* prototype = GetPrototypeInfo();
        retruxx::vector<int> vehicleIds;
        if (prototype->m_tradersGeneratorPrototypeId != -1)
        {
            vehicleIds = _GenerateWithVehicleGenerator(prototype->m_tradersGeneratorPrototypeId, start);
            std::for_each(vehicleIds.begin(), vehicleIds.end(), AddWarePred(prototype->GetWaresPrototypes()));
        }
        if (prototype->m_guardsGeneratorPrototypeId != -1)
        {
            m_guardVehiclesIds = _GenerateWithVehicleGenerator(prototype->m_guardsGeneratorPrototypeId, start);
            vehicleIds.insert(vehicleIds.end(), m_guardVehiclesIds.begin(), m_guardVehiclesIds.end());
        }

        for (int vehicleId : vehicleIds)
        {
            // NOTE: the vehicle is used without a null check.
            auto* vehicle = static_cast<Vehicle*>(theObjects->GetEntityByObjId(vehicleId));
            vehicle->SetGamePositionOnGround(vehicle->GetPosition(), true, false);
            vehicle->SetRandomSkin();
            AddChild(vehicle);
        }
        m_waitingForPlayerToMoveout = false;
    }

    m3d::Class* CaravanTeam::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Team);
    }

    void CaravanTeam::Remove()
    {
        // RVA 0x831320 - the guard team goes with the caravan. NOTE: a stale guard team id is
        // dereferenced as a null object.
        Team::Remove();
        if (m_guardTeamId != -1)
        {
            theObjects->GetEntityByObjId(m_guardTeamId)->Remove();
        }
    }

    void CaravanTeam::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x831020
        Team::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("GuardTeamId", CStr(m_guardTeamId).c_str());
        xmlNode->SetAttribute("GuardVehicles", IntVectorToStr(m_guardVehiclesIds).c_str());
        xmlNode->SetAttribute("CurAttackerId", CStr(m_curAttackerId).c_str());
    }

    void CaravanTeam::_DoNoticeEnemy(int)
    {
        // RVA 0x8304E0 - a caravan ignores enemies until they attack it.
    }

    void CaravanTeam::_TeamUpdate(float elapsedTime, unsigned workTime)
    {
        // RVA 0x831820 - while an attacker is set the whole caravan shoots back at it; once the
        // attacker is gone the guns are stopped.
        Team::_TeamUpdate(elapsedTime, workTime);
        _RemoveChildrenWhenPlayerIsFarEnough();
        _RemoveUnlessChildrenExist();

        if (m_curAttackerId == -1)
        {
            return;
        }

        Obj* attacker = theObjects->GetEntityByObjId(m_curAttackerId);
        if (attacker)
        {
            for (unsigned int i = 0; i < GetVehicles().size(); ++i)
            {
                GetVehicles()[i]->FireFromWeaponAI(true, elapsedTime, attacker);
            }
        }
        else
        {
            for (unsigned int i = 0; i < GetVehicles().size(); ++i)
            {
                GetVehicles()[i]->FireFromWeaponAI(false, 0.0f, nullptr);
            }
            m_curAttackerId = -1;
        }
    }

    void CaravanTeam::_DoUnderAttack(int attackerId)
    {
        // RVA 0x832600 - the guards break away into their own team and go after the attacker.
        m_curAttackerId = attackerId;
        if (_HasAvailableGuards())
        {
            _EnsureGuardsAreInSeparateTeam();
            // NOTE: the guard team is used without a type or null check.
            static_cast<Team*>(theObjects->GetEntityByObjId(m_guardTeamId))->AttackNow(attackerId);
        }
    }

    void CaravanTeam::_DoPosUnreachable()
    {
        // RVA 0x8304F0 - a caravan that cannot go on waits for the player to leave, then vanishes.
        m_waitingForPlayerToMoveout = true;
    }

    CaravanTeam::~CaravanTeam() = default;

    void CaravanTeam::_RemoveChildrenWhenPlayerIsFarEnough()
    {
        // RVA 0x830500
        Vehicle* playerVehicle = thePlayer ? thePlayer->GetVehicle() : nullptr;
        if (m_waitingForPlayerToMoveout &&
            (!playerVehicle || GetDistToPhysicObj(playerVehicle) > theGlobProp.m_distanceFromPlayerToMoveout))
        {
            _RemoveVehicles();
        }
    }

    m3d::Object* CaravanTeam::CreateObject()
    {
        // RVA 0x830860
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    std::vector<int, std::allocator<int>> CaravanTeam::_GenerateWithVehicleGenerator(int protoId, CVector const& pos)
    {
        // RVA 0x831370 - NOTE: the prototype is cast to a wanderers generator without a type
        // check, and a missing one is only reported.
        if (protoId == -1)
        {
            SYS_ERROR("protoId != INVALID_ID");
        }
        auto const* proto = static_cast<WanderersGeneratorPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(protoId));
        if (!proto)
        {
            SYS_ERROR("proto");
        }
        return proto->GenerateAndPlace(pos, -1);
    }

    bool CaravanTeam::_HasAvailableGuards() const
    {
        // RVA 0x831140 - whether any guard is still travelling with the caravan.
        for (Vehicle* vehicle : GetVehicles())
        {
            if (std::find(m_guardVehiclesIds.begin(), m_guardVehiclesIds.end(), vehicle->GetId()) != m_guardVehiclesIds.end())
            {
                return true;
            }
        }
        return false;
    }

    void CaravanTeam::_RemoveUnlessChildrenExist()
    {
        // RVA 0x831720 - the caravan is gone once neither it nor its guard team has vehicles left.
        if (!GetVehicles().empty())
        {
            return;
        }

        // NOTE: a stale guard team id is dereferenced as a null team.
        if (m_guardTeamId == -1 || static_cast<Team*>(theObjects->GetEntityByObjId(m_guardTeamId))->GetVehicles().empty())
        {
            Remove();
        }
    }

    void CaravanTeam::_OnEnemyDestroyed(Event const& evn)
    {
        // RVA 0x8321C0 - once the guard team has dealt with the attacker, its vehicles rejoin the
        // caravan. If the traders are all gone, the guards are left to ask the manager where to go.
        Obj* guardTeamObj = theObjects->GetEntityByObjId(m_guardTeamId);
        if (!guardTeamObj)
        {
            if (m_guardTeamId != -1)
            {
                M3D_LOG_ERR(CStr("Error: guardTeamId != NULL but guardTeam == NULL"));
                return;
            }

            M3D_LOG_ERR(CStr("Error: GE_ENEMY_DESTROYED passed from unknown object: objId = ") + CStr(evn.m_senderObjId));
            Obj* sender = theObjects->GetEntityByObjId(evn.m_senderObjId);
            if (sender)
            {
                M3D_LOG_ERR(CStr("Sender: ") + sender->GetDebugDescription());
            }
            return;
        }

        // NOTE: the guard team is cast without a type check.
        auto* guardTeam = static_cast<Team*>(guardTeamObj);
        retruxx::vector<int> vehiclesToMoveIds;
        for (Vehicle* vehicle : guardTeam->GetVehicles())
        {
            vehiclesToMoveIds.push_back(vehicle->GetId());
        }

        if (GetVehicles().empty() && !vehiclesToMoveIds.empty())
        {
            CauseEvent(GE_LOST_GUARDS_NEED_DIRECTION, 0.0f, m3d::AIParam(), m3d::AIParam());
            m_waitingForPlayerToMoveout = false;
        }

        for (int vehicleId : vehiclesToMoveIds)
        {
            // NOTE: the vehicle is used without a null check.
            auto* vehicle = static_cast<Vehicle*>(theObjects->GetEntityByObjId(vehicleId));
            vehicle->SetRole(nullptr);
            guardTeam->RemoveChild(vehicle);
            AddChild(vehicle);
        }
    }

    void CaravanTeam::_EnsureGuardsAreInSeparateTeam()
    {
        // RVA 0x831EC0 - creates the guard team on first use, subscribed so the caravan hears
        // when the attacker is destroyed or lost, and moves every guard still with the caravan
        // into it.
        bool const bCreated = m_guardTeamId == -1;
        if (bCreated)
        {
            m_guardTeamId = theObjects->CreateNewObject(thePrototypeManager->GetPrototypeId(CStr("guardTeam")), "", -1, -1);
            theProcessManager->PostMessageA(
                GE_SUBSCRIBE, m_guardTeamId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_ENEMY_DESTROYED)), m3d::AIParam(),
                1);
            theProcessManager->PostMessageA(
                GE_SUBSCRIBE, m_guardTeamId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_ENEMY_LOST)), m3d::AIParam(), 1);
        }

        // NOTE: the guard team is used without a type or null check.
        auto* guardTeam = static_cast<Team*>(theObjects->GetEntityByObjId(m_guardTeamId));
        if (bCreated)
        {
            guardTeam->SetBelong(GetBelong());
            guardTeam->SetRemoveWhenChilrenDead(false);
        }

        retruxx::vector<int> vehiclesToMoveIds;
        for (Vehicle* vehicle : GetVehicles())
        {
            int const vehicleId = vehicle->GetId();
            if (std::find(m_guardVehiclesIds.begin(), m_guardVehiclesIds.end(), vehicleId) != m_guardVehiclesIds.end())
            {
                vehiclesToMoveIds.push_back(vehicleId);
            }
        }

        for (int vehicleId : vehiclesToMoveIds)
        {
            Obj* vehicle = theObjects->GetEntityByObjId(vehicleId);
            RemoveChild(vehicle);
            guardTeam->AddChild(vehicle);
        }
    }

    m3d::Object* CaravanTeam::Clone()
    {
        // RVA 0x8306A0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

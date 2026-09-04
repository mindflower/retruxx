#include "radiomanager.h"

#include <core/aiparam.h>
#include <core/kernel.h>

#include "server/event.h"
#include "server/processmanager.h"
#include "server/relationship.h"
#include "server/obstacle.h"
#include <game/m3dgame.h>

#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "player.h"
#include "vehicle.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RadioManager)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RadioManager);

    ai::Obj* RadioManagerPrototypeInfo::CreateTargetObject() const
    {
        return new RadioManager(*this);
    }

    RadioManager::~RadioManager() = default;

    RadioManager::RadioManager(ai::RadioManagerPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        // RVA 0x45A400
        m_bRadioEnabled = true;
    }

    m3d::Object* RadioManager::Clone()
    {
        // RVA 0x45A4A0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* RadioManager::CreateObject()
    {
        // RVA 0x45A660
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* RadioManager::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::Class* RadioManager::GetClass() const
    {
        return RT_CLASS_LOCAL(RadioManager);
    }

    ai::RadioManagerPrototypeInfo const* RadioManager::GetPrototypeInfo() const
    {
        // RVA 0x45A820
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), RadioManagerPrototypeInfo const);
    }

    void RadioManager::EnableRadio()
    {
        // RVA 0x7430
        m_bRadioEnabled = true;
    }

    void RadioManager::DisableRadio()
    {
        // RVA 0x7440
        m_bRadioEnabled = false;
    }

    void RadioManager::ReadyMessage(ai::RadioManager::RM_ENUM type, int belong, CStr const& mes)
    {
        // RVA 0x45A850
        if (m_bRadioEnabled)
        {
            M3D_APP->EnqueueMessage(SM_SOUND_MESSAGE, belong, type, 0, 0, mes, {});
        }
    }

    int RadioManager::OnEvent(ai::Event const& evn)
    {
        int result = Obj::OnEvent(evn);
        switch (evn.m_eventId)
        {
        case GE_OBJECT_DIE:
            _OnObjectDie(evn);
            result = 1;
            break;
        case GE_PART_BROKEN:
            _OnPartBroken(evn);
            result = 1;
            break;
        case GE_RELATION_CHANGED:
            _OnRelationChanged(evn);
            result = 1;
            break;
        case GE_NOTICE_SOMEONE:
            _OnSomeoneAtSight(evn);
            result = 1;
            break;
        case GE_UNDER_ATTACK:
            _OnUnderAttack(evn);
            result = 1;
            break;
        case GE_PLAYER_VEHICLE_CHANGED:
            _OnPlayerVehicleChanged(evn);
            result = 1;
            break;
        default:
            return result;
        }
        return result;
    }

    void RadioManager::_OnObjectDie(ai::Event const& evn)
    {
        // RVA 0x45A8C0 - a nearby vehicle was destroyed; broadcast a radio line
        // to the player and to every other vehicle that witnessed it.
        ai::Obj* senderObj = ai::theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (!senderObj || !senderObj->IsKindOf(&ai::Vehicle::m_classVehicle))
        {
            return;
        }
        auto* deadVeh = static_cast<ai::Vehicle*>(senderObj);
        if (!deadVeh->IsHealthZero())
        {
            return;
        }

        int const playerBelong = ai::thePlayer->GetBelong();
        int const deadBelong = deadVeh->GetBelong();

        if (ai::theRelationship->CheckTolerance(playerBelong, deadBelong) > RS_ENEMY)
        {
            ReadyMessage(RM_OBJECT_DIE, deadBelong, "neutral_self_died");
        }
        else
        {
            ReadyMessage(RM_OBJECT_DIE, deadBelong, "enemy_self_died");
        }

        for (auto const& obstacle : deadVeh->GetNearbyObstacles())
        {
            ai::PhysicObj* owner = obstacle->GetOwnerPhysicObj();
            if (!owner || !owner->IsKindOf(&ai::Vehicle::m_classVehicle))
            {
                continue;
            }
            auto* witnessVeh = static_cast<ai::Vehicle*>(owner);
            if (witnessVeh == deadVeh)
            {
                continue;
            }

            int const witnessBelong = witnessVeh->GetBelong();
            bool const witnessNeutralToPlayer =
                ai::theRelationship->CheckTolerance(playerBelong, witnessBelong) > RS_ENEMY;
            bool const deadNeutralToWitness = ai::theRelationship->CheckTolerance(deadBelong, witnessBelong) > RS_ENEMY;

            if (witnessNeutralToPlayer)
            {
                ReadyMessage(
                    RM_OBJECT_DIE, witnessBelong, deadNeutralToWitness ? "neutral_neutral_died" : "neutral_enemy_died");
            }
            else if (deadNeutralToWitness)
            {
                ReadyMessage(RM_OBJECT_DIE, witnessBelong, "enemy_neutral_died");
            }
            else
            {
                ReadyMessage(RM_OBJECT_DIE, witnessBelong, "enemy_enemy_died");
            }
        }
    }

    void RadioManager::_OnPartBroken(ai::Event const& evn)
    {
        // RVA 0x45AAE0 - twin of _OnObjectDie for a broken vehicle part; witness
        // lines are only sent when the damaged vehicle is an enemy of the witness.
        ai::Obj* senderObj = ai::theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (!senderObj || !senderObj->IsKindOf(&ai::Vehicle::m_classVehicle))
        {
            return;
        }
        auto* brokenVeh = static_cast<ai::Vehicle*>(senderObj);

        int const playerBelong = ai::thePlayer->GetBelong();
        int const brokenBelong = brokenVeh->GetBelong();

        if (ai::theRelationship->CheckTolerance(playerBelong, brokenBelong) > RS_ENEMY)
        {
            ReadyMessage(RM_PART_BROKEN, brokenBelong, "neutral_self_broken");
        }
        else
        {
            ReadyMessage(RM_PART_BROKEN, brokenBelong, "enemy_self_broken");
        }

        for (auto const& obstacle : brokenVeh->GetNearbyObstacles())
        {
            ai::PhysicObj* owner = obstacle->GetOwnerPhysicObj();
            if (!owner || !owner->IsKindOf(&ai::Vehicle::m_classVehicle))
            {
                continue;
            }
            auto* witnessVeh = static_cast<ai::Vehicle*>(owner);
            if (witnessVeh == brokenVeh)
            {
                continue;
            }

            int const witnessBelong = witnessVeh->GetBelong();
            bool const witnessNeutralToPlayer =
                ai::theRelationship->CheckTolerance(playerBelong, witnessBelong) > RS_ENEMY;
            bool const brokenIsEnemyOfWitness =
                ai::theRelationship->CheckTolerance(brokenBelong, witnessBelong) <= RS_ENEMY;
            if (!brokenIsEnemyOfWitness)
            {
                continue;
            }

            // NOTE: the shipped build tags the witness line with the *broken*
            // vehicle's belong, not the witness's.
            ReadyMessage(
                RM_PART_BROKEN, brokenBelong, witnessNeutralToPlayer ? "neutral_enemy_broken" : "enemy_enemy_broken");
        }
    }

    void RadioManager::_OnUnderAttack(ai::Event const& evn)
    {
        // RVA 0x45ACF0
        ai::theProcessManager->PostMessageA(3, evn.m_senderObjId, GetId(), 0.0f, m3d::AIParam(47), m3d::AIParam(), 1);

        int victimBelong = 0;
        if (ai::Obj* victim = ai::theObjects->GetEntityByObjId(evn.m_senderObjId))
        {
            victimBelong = victim->GetBelong();
        }

        int attackerBelong = 0;
        if (ai::Obj* attacker = ai::theObjects->GetEntityByObjId(evn.m_param1.GetAsID()))
        {
            attackerBelong = attacker->GetBelong();
        }

        int const playerBelong = ai::thePlayer->GetBelong();
        if (victimBelong != playerBelong && attackerBelong == playerBelong &&
            ai::theRelationship->CheckTolerance(playerBelong, victimBelong) > RS_ENEMY)
        {
            ReadyMessage(RM_UNDER_ATTACK, victimBelong, "neutral_player_hit_me");
        }
    }

    void RadioManager::_OnRelationChanged(ai::Event const& evn)
    {
        // RVA 0x45AE70 - only reacts when the player's own belong is involved.
        int const playerBelong = ai::thePlayer->GetBelong();
        if (evn.m_param1.GetAsID() != playerBelong)
        {
            return;
        }

        ai::theProcessManager->PostMessageA(3, evn.m_senderObjId, GetId(), 0.0f, m3d::AIParam(45), m3d::AIParam(), 1);

        int senderBelong = 0;
        if (ai::Obj* sender = ai::theObjects->GetEntityByObjId(evn.m_senderObjId))
        {
            senderBelong = sender->GetBelong();
        }
        ReadyMessage(RM_RELATION_CHANGED, senderBelong, "neutral_player_become_enemy");
    }

    void RadioManager::_OnSomeoneAtSight(ai::Event const& evn)
    {
        // RVA 0x45AFA0 - a vehicle spotted someone; only relevant when the spotted
        // one belongs to the player.
        ai::Obj* sender = ai::theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (!sender)
        {
            return;
        }
        int const senderBelong = sender->GetBelong();

        ai::Obj* spotted = ai::theObjects->GetEntityByObjId(evn.m_param1.GetAsID());
        if (!spotted)
        {
            return;
        }

        int const playerBelong = ai::thePlayer->GetBelong();
        if (spotted->GetBelong() != playerBelong)
        {
            return;
        }

        if (ai::theRelationship->CheckTolerance(playerBelong, senderBelong) <= RS_ENEMY)
        {
            ReadyMessage(RM_NOTICE, playerBelong, "neutral_first_see_other");
        }

        if (ai::theRelationship->CheckTolerance(playerBelong, senderBelong) > RS_ENEMY)
        {
            ReadyMessage(RM_NOTICE, senderBelong, "neutral_first_see_other");
        }
        else
        {
            ReadyMessage(RM_NOTICE, senderBelong, "enemy_first_see_other");
        }
    }

    void RadioManager::_OnPlayerVehicleChanged(ai::Event const& evn)
    {
        // RVA 0x45B0F0
        if (m_bRadioEnabled)
        {
            M3D_APP->EnqueueMessage(SM_SOUND_MESSAGE, -1, 0, 0, 0, {}, {});
        }
    }
}  // namespace ai

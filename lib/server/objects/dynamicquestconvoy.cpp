#include "dynamicquestconvoy.h"
#include <stdexcept>

#include "core/ini.h"

#include "base/prototypemanager.h"
#include "base/objcontainer.h"
#include "server/dynamicscene.h"
#include "server/processmanager.h"
#include "vehicle.h"

#include <core/kernel.h>
#include <core/log.h>
#include <m3dapp.h>
#include "location.h"
#include "team.h"
#include "town.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestConvoy)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestConvoy);

    DynamicQuestConvoyPrototypeInfo::DynamicQuestConvoyPrototypeInfo()
    {
        this->m_playerSchwarzPart = 0.0;
        this->m_criticalDistFromPlayer = 100.0;
        this->m_criticalTime = 20.0;
    }

    bool DynamicQuestConvoyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_playerSchwarzPart, xmlNode, "PlayerSchwarzPart");
            m3d::SafeFloatAttrib(m_criticalDistFromPlayer, xmlNode, "CriticalDistFromPlayer");
            m3d::SafeFloatAttrib(m_criticalTime, xmlNode, "CriticalTime");
            return 1;
        }
        return result;
    }

    DynamicQuest* DynamicQuestConvoyPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x747310
        return new DynamicQuestConvoy(*this);
    }

    DynamicQuestManager::QuestType DynamicQuestConvoy::GetQuestType() const
    {
        // RVA 0x7471C0
        return DynamicQuestManager::TYPE_CONVOY;
    }

    int DynamicQuestConvoy::GetCaravanId() const
    {
        // RVA 0x7471F0
        return m_caravanId;
    }

    DynamicQuestConvoy::DynamicQuestConvoy(DynamicQuestConvoyPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        // RVA 0x747180
        m_caravanId = -1;
        m_targetLocationId = -1;
        m_timePlayerIsTooFar = 0.0f;
    }

    m3d::Class* DynamicQuestConvoy::GetClass() const
    {
        // RVA 0x747170
        return RT_CLASS_LOCAL(DynamicQuestConvoy);
    }

    void DynamicQuestConvoy::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7477B0
        DynamicQuest::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("CaravanId", CStr(m_caravanId).c_str());
        xmlNode->SetAttribute("LocationId", CStr(m_targetLocationId).c_str());
        xmlNode->SetAttribute("TimePlayerIsTooFar", CStr(m_timePlayerIsTooFar).c_str());
    }

    void DynamicQuestConvoy::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x747390
        DynamicQuest::LoadRuntimeValues(xmlFile, xmlNode);
        if (!xmlNode->IsEmpty())
        {
            char const* const caravanId = xmlNode->GetAttribute("CaravanId");
            if (caravanId)
            {
                m_caravanId = atoi(caravanId);
            }
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const locationId = xmlNode->GetAttribute("LocationId");
            if (locationId)
            {
                m_targetLocationId = atoi(locationId);
            }
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const timePlayerIsTooFar = xmlNode->GetAttribute("TimePlayerIsTooFar");
            if (timePlayerIsTooFar)
            {
                m_timePlayerIsTooFar = static_cast<float>(atof(timePlayerIsTooFar));
            }
        }
    }

    DynamicQuestConvoyPrototypeInfo const* DynamicQuestConvoy::GetPrototypeInfo() const
    {
        // RVA 0x747880
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), DynamicQuestConvoyPrototypeInfo const);
    }

    m3d::Class* DynamicQuestConvoy::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestConvoy::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x747D90 - the player has to stay with the caravan; wandering off for long enough loses
        // the quest.
        Obj::Update(elapsedTime, workTime);

        auto* const caravan = (Team*)theObjects->GetEntityByObjId(m_caravanId);
        if (!caravan)
        {
            return;
        }
        Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (!playerVehicle)
        {
            return;
        }

        DynamicQuestConvoyPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (caravan->GetDistToPhysicObj(playerVehicle) <= prototypeInfo->m_criticalDistFromPlayer)
        {
            m_timePlayerIsTooFar = 0.0f;
            return;
        }

        m_timePlayerIsTooFar += elapsedTime;
        if (m_timePlayerIsTooFar >= prototypeInfo->m_criticalTime)
        {
            _SetStatus(STATUS_FORGOTTEN);
            _Unsubscribe();
            m_caravanId = -1;
            M3D_APP->EnqueueMessage(66564, FM_CONVOY_FAILED, 0, 0, 0, {}, {});
        }
    }

    int DynamicQuestConvoy::OnEvent(Event const& evn)
    {
        // RVA 0x748570
        int const result = DynamicQuest::OnEvent(evn);
        if (evn.m_eventId == GE_OBJECT_ENTERS_LOCATION)
        {
            _OnObjectEntersLocation(evn);
            return 1;
        }
        if (evn.m_eventId == GE_OBJECT_DIE)
        {
            _OnObjectDie(evn);
            return 1;
        }
        if (evn.m_eventId == GE_TARGET_REACHED)
        {
            _SetStatus(STATUS_COMPLETE);
            _Unsubscribe();
            return 1;
        }
        return result;
    }

    void DynamicQuestConvoy::_Unsubscribe()
    {
        // RVA 0x747BD0
        if (m_caravanId != -1)
        {
            m3d::AIParam targetReached(static_cast<int>(GE_TARGET_REACHED));
            theProcessManager->PostMessageA(
                GE_UNSUBSCRIBE, m_caravanId, GetId(), 0.0, targetReached, {}, 1);
            m3d::AIParam objectDie(static_cast<int>(GE_OBJECT_DIE));
            theProcessManager->PostMessageA(GE_UNSUBSCRIBE, m_caravanId, GetId(), 0.0, objectDie, {}, 1);
        }
        if (m_targetLocationId != -1)
        {
            m3d::AIParam entersLocation(static_cast<int>(GE_OBJECT_ENTERS_LOCATION));
            theProcessManager->PostMessageA(
                GE_UNSUBSCRIBE, m_targetLocationId, GetId(), 0.0, entersLocation, {}, 1);
        }
        m_caravanId = -1;
        m_targetLocationId = -1;
    }

    DynamicQuestConvoy::~DynamicQuestConvoy()
    {
        // RVA 0x7471D0 - the base class does all the work.
    }

    void DynamicQuestConvoy::_OnTake()
    {
        // RVA 0x747EF0 - taking the quest sends a caravan on its way from the hiring town to the place it
        // has to reach.
        DynamicQuest::_OnTake();

        auto* const town = (Town*)theObjects->GetEntityByObjId(GetHirerObjId());
        if (!town)
        {
            M3D_LOG_ERR("Error: NULL town in OnTake for " + GetDebugDescription());
            return;
        }

        Obj* const target = theObjects->GetEntityByObjId(GetTargetObjId());
        if (!target ||
            (!target->IsKindOf(&ai::Location::m_classLocation) && !target->IsKindOf(&ai::Town::m_classTown)))
        {
            M3D_LOG_ERR("Error: invalid target for " + GetDebugDescription());
            if (target)
            {
                M3D_LOG_ERR("Target: " + target->GetDebugDescription());
            }
            else
            {
                M3D_LOG_INFO("Target is NULL");
            }
            return;
        }

        Location* targetLocation = nullptr;
        if (target->IsKindOf(&ai::Location::m_classLocation))
        {
            targetLocation = (Location*)target;
        }
        else
        {
            targetLocation = ((Settlement*)target)->GetLocation(Location::LOCATION_CARAVAN_ARRIVE);
        }
        if (!targetLocation)
        {
            M3D_LOG_ERR("Error: targetLocation is NULL for " + GetDebugDescription());
            return;
        }

        Team* const caravan = town->SpawnCaravanToLocation(targetLocation);
        if (!caravan)
        {
            M3D_LOG_ERR("Error: caravan was not spawned for " + GetDebugDescription());
            return;
        }
        _Subscribe(caravan->GetId(), targetLocation->GetId());
    }

    int DynamicQuestConvoy::_CalcReward()
    {
        // RVA 0x7478B0 - the same distance-based price as a Reach quest: the player's rating times the
        // distance in kilometres times the prototype's share.
        DynamicQuestConvoyPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (!playerVehicle)
        {
            return prototypeInfo->m_minReward;
        }

        // NOTE: neither object is checked for null or for actually being a physic object.
        auto* const hirer = (PhysicObj*)theObjects->GetEntityByObjId(GetHirerObjId());
        auto* const target = (PhysicObj*)theObjects->GetEntityByObjId(GetTargetObjId());
        CVector const delta = target->GetPosition() - hirer->GetPosition();

        int const reward = static_cast<int>(
            playerVehicle->GetSchwarz() * (sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) *
                                          0.001) *
            prototypeInfo->m_playerSchwarzPart);
        return reward > prototypeInfo->m_minReward ? reward : prototypeInfo->m_minReward;
    }

    void DynamicQuestConvoy::_OnCreate()
    {
        // RVA 0x7471E0 - there is nothing to listen to until the caravan exists; _OnTake subscribes.
    }

    void DynamicQuestConvoy::_Subscribe(int caravanId, int targetLocationId)
    {
        // RVA 0x747A00 - watch the caravan arriving, the caravan dying, and the player reaching the
        // destination himself.
        if (caravanId == -1 || targetLocationId == -1)
        {
            return;
        }
        m3d::AIParam targetReached(static_cast<int>(GE_TARGET_REACHED));
        theProcessManager->PostMessageA(GE_SUBSCRIBE, caravanId, GetId(), 0.0, targetReached, {}, 1);
        m3d::AIParam objectDie(static_cast<int>(GE_OBJECT_DIE));
        theProcessManager->PostMessageA(GE_SUBSCRIBE, caravanId, GetId(), 0.0, objectDie, {}, 1);
        m3d::AIParam entersLocation(static_cast<int>(GE_OBJECT_ENTERS_LOCATION));
        theProcessManager->PostMessageA(
            GE_SUBSCRIBE, targetLocationId, GetId(), 0.0, entersLocation, {}, 1);

        m_caravanId = caravanId;
        m_targetLocationId = targetLocationId;
    }

    void DynamicQuestConvoy::_OnObjectDie(Event const&)
    {
        // RVA 0x7484C0 - losing the caravan loses the quest.
        if (GetQuestStatus() == STATUS_PROCESSING)
        {
            M3D_APP->EnqueueMessage(66564, FM_CONVOY_FAILED, 0, 0, 0, {}, {});
        }
        _SetStatus(STATUS_FORGOTTEN);
        _Unsubscribe();
    }

    void DynamicQuestConvoy::_OnTargetReached(Event const&)
    {
        // RVA 0x748350
        _SetStatus(STATUS_COMPLETE);
        _Unsubscribe();
    }

    void DynamicQuestConvoy::_OnObjectEntersLocation(Event const& evn)
    {
        // RVA 0x748370 - the destination reports an arrival; the quest is done only when the arrival is
        // one of the caravan's own vehicles, and the caravan is then told to stop.
        auto* const caravan = (Team*)theObjects->GetEntityByObjId(m_caravanId);
        if (!caravan)
        {
            return;
        }
        int const enteredObjId = evn.m_param1.GetAsID();
        if (enteredObjId == -1)
        {
            return;
        }
        for (Vehicle* const vehicle : caravan->GetVehicles())
        {
            if (vehicle->GetId() == enteredObjId)
            {
                caravan->Stop();
                caravan->CauseEvent(GE_TARGET_REACHED, 0.0, {}, {});
                _SetStatus(STATUS_COMPLETE);
                _Unsubscribe();
                return;
            }
        }
    }

    m3d::Object* DynamicQuestConvoy::Clone()
    {
        // RVA 0x747430
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* DynamicQuestConvoy::CreateObject()
    {
        // RVA 0x7475F0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}

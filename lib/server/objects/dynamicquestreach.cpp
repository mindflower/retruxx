#include "dynamicquestreach.h"
#include <stdexcept>

#include "core/ini.h"

#include "base/prototypemanager.h"
#include "server/dynamicscene.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"
#include "server/processmanager.h"
#include "server/relationship.h"

#include <core/kernel.h>
#include "town.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestReach)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestReach);

    DynamicQuestReachPrototypeInfo::DynamicQuestReachPrototypeInfo()
    {
        m_playerSchwarzPart = 0.0;
    }

    ai::DynamicQuest* DynamicQuestReachPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x82CF60
        return new DynamicQuestReach(*this);
    }

    bool DynamicQuestReachPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_playerSchwarzPart, xmlNode, "PlayerSchwarzPart");
            return 1;
        }
        return result;
    }

    m3d::Class* DynamicQuestReach::GetClass() const
    {
        // RVA 0x82CE70
        return RT_CLASS_LOCAL(DynamicQuestReach);
    }

    DynamicQuestReachPrototypeInfo const* DynamicQuestReach::GetPrototypeInfo() const
    {
        // RVA 0x82D350
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), DynamicQuestReachPrototypeInfo const);
    }

    int DynamicQuestReach::OnEvent(Event const& evn)
    {
        // RVA 0x82D700
        int const result = DynamicQuest::OnEvent(evn);
        if (evn.m_eventId == GE_OBJECT_ENTERS_LOCATION)
        {
            _OnObjectEntersLocation(evn);
            return 1;
        }
        return result;
    }

    DynamicQuestManager::QuestType DynamicQuestReach::GetQuestType() const
    {
        // RVA 0x82CEA0
        return DynamicQuestManager::TYPE_REACH;
    }

    void DynamicQuestReach::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x82D380 - the quest is forgotten once the place to reach is gone, or once it is a town that
        // has been ruined.
        Obj::Update(elapsedTime, workTime);
        Obj* const target = theObjects->GetEntityByObjId(GetTargetObjId());
        if (!target || (target->IsKindOf(&ai::Town::m_classTown) && ((Town*)target)->IsRuined()))
        {
            _SetStatus(STATUS_FORGOTTEN);
        }
    }

    m3d::Class* DynamicQuestReach::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    DynamicQuestReach::DynamicQuestReach(DynamicQuestReachPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        // RVA 0x82CE80 - nothing of its own to set up.
    }

    void DynamicQuestReach::_OnCreate()
    {
        // RVA 0x82D550 - listen for the player driving into the place that has to be reached.
        if (GetTargetObjId() != -1)
        {
            m3d::AIParam param(static_cast<int>(GE_OBJECT_ENTERS_LOCATION));
            theProcessManager->PostMessageA(
                GE_SUBSCRIBE, GetTargetObjId(), GetId(), 0.0, param, {}, 1);
        }
    }

    int DynamicQuestReach::_CalcReward()
    {
        // RVA 0x82D400 - the further the place is from whoever is hiring, the more the trip is worth: the
        // player's own rating times the distance in kilometres times the prototype's share.
        DynamicQuestReachPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
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

    DynamicQuestReach::~DynamicQuestReach()
    {
        // RVA 0x82CEB0 - the base class does all the work.
    }

    m3d::Object* DynamicQuestReach::CreateObject()
    {
        // RVA 0x82D190
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* DynamicQuestReach::Clone()
    {
        // RVA 0x82CFD0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void DynamicQuestReach::_OnObjectEntersLocation(Event const& evn)
    {
        // RVA 0x82D5F0 - the quest is done the moment the player's own vehicle arrives.
        Obj* const entered = theObjects->GetEntityByObjId(evn.m_param1.GetAsID());
        if (entered != (Obj*)thePlayer->GetVehicle())
        {
            return;
        }
        if (GetQuestStatus() == STATUS_NOT_TAKEN || GetQuestStatus() == STATUS_FORGOTTEN)
        {
            return;
        }
        // NOTE: _OnCreate subscribed to GE_OBJECT_ENTERS_LOCATION, but this unsubscribes
        // GE_OBJECT_DIE - so the arrival subscription is never actually dropped.
        m3d::AIParam param(static_cast<int>(GE_OBJECT_DIE));
        theProcessManager->PostMessageA(GE_UNSUBSCRIBE, GetTargetObjId(), GetId(), 0.0, param, {}, 1);
        _SetStatus(STATUS_COMPLETE);
    }
}

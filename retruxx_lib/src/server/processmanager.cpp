#include "processmanager.h"
#include <stdexcept>
#include <core/aiparam.h>
#include "event.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/timer.h"
#include "objects/base/objcontainer.h"

CStr STR_UNKNOWN = "Unknown";

namespace ai
{
    //ProcessManager* theProcessManager = nullptr;

    int ProcessManager::GetNumProcessedEvents() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ProcessManager::Update(float elapsedTime, unsigned framesPassed, unsigned workTime)
    {
        // TODO: generated code
        for (auto it = m_eventQueue.begin(); it != m_eventQueue.end(); ++it)
        {
            it->m_timeOut -= elapsedTime;
            it->m_framesToPass -= framesPassed;
        }

        // Process events that are ready
        auto it = m_eventQueue.begin();
        while (it != m_eventQueue.end())
        {
            // Check if event is not ready yet
            if (it->m_timeOut >= 0.00001 || it->m_framesToPass > 0)
            {
                ++it;
                continue;
            }

            // Event is ready to process
            if (it->m_recipientObjId == -10)
            {
                // Broadcast event to all objects
                M3D_LOG_INFO("Broadcast event!");

                // Send event to all objects in the container
                for (auto* node : *theObjects)
                {
                    node->OnEvent(*it);
                }
            }
            else if (it->m_recipientObjId >= 0)
            {
                // Send event to specific object
                int objId = it->m_recipientObjId;
                auto* node = ai::theObjects->GetEntityByObjId(objId);
                if (node)
                {
                    node->OnEvent(*it);
                }
            }

            // Remove processed event
            ++m_numProcessedEvents;
            it = m_eventQueue.erase(it);
        }
    }

    ProcessManager::ProcessManager()
    {
        _RegisterEvents();
    }

    CStr const& ProcessManager::GetEventName(eGameEvent eventId) const
    {
        for (auto const& ev : m_eventInfo)
        {
            if (ev.m_eventId == eventId)
            {
                return ev.m_eventName;
            }
        }
        return STR_UNKNOWN;
    }

    void ProcessManager::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ProcessManager::Clear()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ProcessManager::~ProcessManager() = default;

    void ProcessManager::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGameEvent ProcessManager::GetEventId(CStr const& eventName) const
    {
        for (auto const& ev : m_eventInfo)
        {
            if (eventName == ev.m_eventName)
            {
                return ev.m_eventId;
            }
        }
        return GE_UNKNOWN;
    }

    int ProcessManager::GetNumEventsInQueue() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ProcessManager::UpdateDieEvents()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ProcessManager::PostMessageA(ai::Event const& evn)
    {
        if (evn.m_recipientObjId != -1)
        {
            auto curTime = M3D_KERNEL->GetTimer().GetCurTime();
            ai::Event e = evn;
            e.m_timeStamp = curTime * 0.001;
            e.m_debugNum = ++m_eventDebugNum;
            m_eventQueue.push_back(std::move(e));
        }
    }

    void ProcessManager::PostMessageA(int eventId, int recipientObjId, int senderObjId, float timeOut, m3d::AIParam param1, m3d::AIParam param2, int framesToPass)
    {
        if (recipientObjId == -1)
        {
            return;
        }
        ai::Event e;
        e.m_eventId = static_cast<eGameEvent>(eventId);
        e.m_senderObjId = senderObjId;
        e.m_recipientObjId = recipientObjId;
        e.m_timeOut = timeOut;
        e.m_framesToPass = framesToPass;
        e.m_param1 = param1;
        e.m_param2 = param2;
        e.m_timeStamp = M3D_KERNEL->GetTimer().GetCurTime() * 0.001;
        e.m_debugNum = ++m_eventDebugNum;
        m_eventQueue.push_back(std::move(e));
    }

    void ProcessManager::_RegisterEvents()
    {
        _RegisterEvent("GE_GAME_START", GE_GAME_START);
        _RegisterEvent("GE_SUBSCRIBE", GE_SUBSCRIBE);
        _RegisterEvent("GE_UNSUBSCRIBE", GE_UNSUBSCRIBE);
        _RegisterEvent("GE_OBJECT_ENTERS_LOCATION", GE_OBJECT_ENTERS_LOCATION);
        _RegisterEvent("GE_OBJECT_LEAVES_LOCATION", GE_OBJECT_LEAVES_LOCATION);
        _RegisterEvent("GE_OBJECT_IN_LOCATION", GE_OBJECT_IN_LOCATION);
        _RegisterEvent("GE_OBJECT_ACTIVATED", GE_OBJECT_ACTIVATED);
        _RegisterEvent("GE_OBJECT_DEACTIVATED", GE_OBJECT_DEACTIVATED);
        _RegisterEvent("GE_OBJECT_DIE", GE_OBJECT_DIE);
        _RegisterEvent("GE_OBJECT_DIE_SENSE", GE_OBJECT_DIE_SENSE);
        _RegisterEvent("GE_END_TTL", GE_END_TTL);
        _RegisterEvent("GE_END_ANIMATION", GE_END_ANIMATION);
        _RegisterEvent("GE_TIME_PERIOD", GE_TIME_PERIOD);
        _RegisterEvent("GE_FRAMES_PASSED", GE_FRAMES_PASSED);
        _RegisterEvent("GE_FREE", GE_FREE);
        _RegisterEvent("GE_ENEMY_LOST", GE_ENEMY_LOST);
        _RegisterEvent("GE_ENEMY_DESTROYED", GE_ENEMY_DESTROYED);
        _RegisterEvent("GE_TUTORIAL_VEHICLE", GE_TUTORIAL_VEHICLE);
        _RegisterEvent("GE_TUTORIAL_INVENTORY", GE_TUTORIAL_INVENTORY);
        _RegisterEvent("GE_TUTORIAL_TOWN", GE_TUTORIAL_TOWN);
        _RegisterEvent("GE_TUTORIAL_BAR", GE_TUTORIAL_BAR);
        _RegisterEvent("GE_TUTORIAL_WORKSHOP", GE_TUTORIAL_WORKSHOP);
        _RegisterEvent("GE_TUTORIAL_SHOP", GE_TUTORIAL_SHOP);
        _RegisterEvent("GE_TUTORIAL_QUESTLOG", GE_TUTORIAL_QUESTLOG);
        _RegisterEvent("GE_TUTORIAL_MAP", GE_TUTORIAL_MAP);
        _RegisterEvent("GE_TUTORIAL_STATS", GE_TUTORIAL_STATS);
        _RegisterEvent("GE_TUTORIAL_HISTORY", GE_TUTORIAL_HISTORY);
        _RegisterEvent("GE_TUTORIAL_BOOKS", GE_TUTORIAL_BOOKS);
        _RegisterEvent("GE_TUTORIAL_ENCYCLOPAEDIA", GE_TUTORIAL_ENCYCLOPAEDIA);
        _RegisterEvent("GE_TUTORIAL_JOURNAL", GE_TUTORIAL_JOURNAL);
        _RegisterEvent("GE_TUTORIAL_RELATIONS", GE_TUTORIAL_RELATIONS);
        _RegisterEvent("GE_TUTORIAL_TAB_GOODS", GE_TUTORIAL_TAB_GOODS);
        _RegisterEvent("GE_TUTORIAL_TAB_WEAPON", GE_TUTORIAL_TAB_WEAPON);
        _RegisterEvent("GE_TUTORIAL_CABINS", GE_TUTORIAL_CABINS);
        _RegisterEvent("GE_TUTORIAL_CABIN_SELECT", GE_TUTORIAL_CABIN_SELECT);
        _RegisterEvent("GE_TUTORIAL_BASKETS", GE_TUTORIAL_BASKETS);
        _RegisterEvent("GE_TUTORIAL_BASKET_SELECT", GE_TUTORIAL_BASKET_SELECT);
        _RegisterEvent("GE_TUTORIAL_NEW_VEHICLE", GE_TUTORIAL_NEW_VEHICLE);
        _RegisterEvent("GE_TUTORIAL_NEW_VEHICLE_SELECT", GE_TUTORIAL_NEW_VEHICLE_SELECT);
        _RegisterEvent("GE_TUTORIAL_SKIN", GE_TUTORIAL_SKIN);
        _RegisterEvent("GE_TUTORIAL_REFUEL", GE_TUTORIAL_REFUEL);
        _RegisterEvent("GE_TUTORIAL_REPAIR", GE_TUTORIAL_REPAIR);
        _RegisterEvent("GE_TUTORIAL_RECHARGE", GE_TUTORIAL_RECHARGE);
        _RegisterEvent("GE_PART_BROKEN", GE_PART_BROKEN);
        _RegisterEvent("GE_RELATION_CHANGED", GE_RELATION_CHANGED);
        _RegisterEvent("GE_NOTICE_SOMEONE", GE_NOTICE_SOMEONE);
        _RegisterEvent("GE_UNDER_ATTACK", GE_UNDER_ATTACK);
        _RegisterEvent("GE_TEAM_NEEDS_REINFORCEMENT", GE_TEAM_NEEDS_REINFORCEMENT);
        _RegisterEvent("GE_LOST_GUARDS_NEED_DIRECTION", GE_LOST_GUARDS_NEED_DIRECTION);
        _RegisterEvent("GE_NOTICE_ENEMY", GE_NOTICE_ENEMY);
        _RegisterEvent("GE_TARGET_REACHED", GE_TARGET_REACHED);
        _RegisterEvent("GE_TARGET_UNREACHED", GE_TARGET_UNREACHED);
        _RegisterEvent("GE_OBJECT_ENTERS_OBJECT", GE_OBJECT_ENTERS_OBJECT);
        _RegisterEvent("GE_OBJECT_LEAVES_OBJECT", GE_OBJECT_LEAVES_OBJECT);
        _RegisterEvent("GE_TALK_WITH_OBJECT", GE_TALK_WITH_OBJECT);
        _RegisterEvent("GE_SKIP_CINEMATIC", GE_SKIP_CINEMATIC);
        _RegisterEvent("GE_END_CINEMATIC", GE_END_CINEMATIC);
        _RegisterEvent("GE_START_CINEMATIC_MSG", GE_START_CINEMATIC_MSG);
        _RegisterEvent("GE_START_CINEMATIC_FLY", GE_START_CINEMATIC_FLY);
        _RegisterEvent("GE_IN_CINEMATIC", GE_IN_CINEMATIC);
        _RegisterEvent("GE_CINEMATIC_ENTER_FADE_IN", GE_CINEMATIC_ENTER_FADE_IN);
        _RegisterEvent("GE_CUSTOM_GUN_POINTED", GE_CUSTOM_GUN_POINTED);
        _RegisterEvent("GE_CUSTOM_GUN_DISPOINTED", GE_CUSTOM_GUN_DISPOINTED);
        _RegisterEvent("GE_PLAYER_VEHICLE_HORN", GE_PLAYER_VEHICLE_HORN);
        _RegisterEvent("GE_PLAYER_VEHICLE_CHANGED", GE_PLAYER_VEHICLE_CHANGED);
        _RegisterEvent("GE_VEHICLE_WITHOUT_HEALTH", GE_VEHICLE_WITHOUT_HEALTH);
        _RegisterEvent("GE_BOSS_CRITICAL_LOADS_EXPLODED", GE_BOSS_CRITICAL_LOADS_EXPLODED);
        _RegisterEvent("GE_BOSS_ARM_ACTION_FINISHED", GE_BOSS_ARM_ACTION_FINISHED);
        _RegisterEvent("GE_BOSS04_STATION_DESTROYED", GE_BOSS04_STATION_DESTROYED);
        _RegisterEvent("GE_LEAVE_TOWN", GE_LEAVE_TOWN);
        _RegisterEvent("GE_TOWN_CONDITIONAL_CLOSING", GE_TOWN_CONDITIONAL_CLOSING);
        _RegisterEvent("GE_DYNAMIC_QUEST_TAKEN", GE_DYNAMIC_QUEST_TAKEN);
        _RegisterEvent("GE_DYNAMIC_QUEST_COMPLETE", GE_DYNAMIC_QUEST_COMPLETE);
        _RegisterEvent("GE_DYNAMIC_QUEST_FORGOTTEN", GE_DYNAMIC_QUEST_FORGOTTEN);
        _RegisterEvent("GE_DYNAMIC_QUEST_FAILED", GE_DYNAMIC_QUEST_FAILED);
        _EnsureAllEventsAreRegistered();
    }

    void ProcessManager::_RegisterEvent(CStr const& eventName, eGameEvent eventId)
    {
        M3D_ASSERT(GetEventId(eventName) == GE_UNKNOWN);
        auxEventInfo ev;
        ev.m_eventName = eventName;
        ev.m_eventId = eventId;
        m_eventInfo.push_back(ev);
    }

    void ProcessManager::_EnsureAllEventsAreRegistered() const
    {
        for (int eventId = 1; eventId < GE_NUM_EVENTS; ++eventId)
        {
            M3D_ASSERT(GetEventName((eGameEvent)eventId) != STR_UNKNOWN);
        }
    }
}

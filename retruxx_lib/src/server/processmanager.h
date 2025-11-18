#pragma once
#include "objects/base/obj.h"
#include "event.h"

namespace ai
{
    enum ServerMessage
    {
        SM_REPOSITORY_CHANGED = 0x103e9,
        SM_QUEST_WAS_TAKEN = 0x103ea,
        SM_QUESTSTATE_CHANGED = 0x103eb,
        SM_TOWN = 0x103ec,
        SM_OBJECT_CREATED = 0x103ed,
        SM_OBJECT_DESTROYED = 0x103ee,
        SM_OBJECTS_CLEARED = 0x103ef,
        SM_PLAYER_VEHICLE_CHANGED = 0x103f0,
        SM_VEHICLEPART_CHANGED = 0x103f1,
        SM_DYNAMIC_QUEST_CREATED = 0x103f2,
        SM_DYNAMIC_QUESTSTATE_CHANGED = 0x103f3,
        SM_PLAYER_GOLD_CHANGED = 0x103f4,
        SM_PLAYER_PASS_TO_MAP = 0x103f5,
        SM_MESSAGE_BOX = 0x103f6,
        SM_GADGET_CHANGED = 0x103f7,
        SM_TARGET_CAPTURED = 0x103f9,
        SM_LOCATION_NPC = 0x103fa,
        SM_PLAYER_FIGHT_STATE_CHANGED = 0x103fb,
        SM_LOCATION_STATE_CHANGED = 0x103fc,
        SM_OBJ_MET = 0x103fd,
        SM_PROTOTYPE_MET = 0x103fe,
        SM_PLAYER_MONEY_CHANGED = 0x103ff,
        SM_SOUND_MESSAGE = 0x10400,
        SM_PLAYER_VEHICLE_DAMAGED = 0x10401,
        SM_QUEST_ITEMS_CHANGED = 0x10402,
        SM_FADING_MESSAGE = 0x10403,
        SM_IMPORTANT_FADING_MESSAGE = 0x10404,
        SM_TOWN_RUINED = 0x10405,
        SM_PLAYER_PASS_TO_MAP_FADING = 0x10406,
        SM_NEARBY_CHESTS = 0x10407,
        SM_APPROACH_TOWN = 0x10408,
    };

    class ProcessManager
    {
        struct auxEventInfo
        {
            /* 0x0000 */ ai::eGameEvent m_eventId;
            /* 0x0004 */ CStr m_eventName;
        }; /* size: 0x0010 */

    private:
        /* 0x0000 */ retruxx::vector<ai::ProcessManager::auxEventInfo, retruxx::allocator<ai::ProcessManager::auxEventInfo> > m_eventInfo;

        using EventList = retruxx::list<ai::Event, retruxx::allocator<ai::Event> >;

    private:
        /* 0x0010 */ retruxx::list<ai::Event, retruxx::allocator<ai::Event> > m_eventQueue;
        /* 0x001c */ int m_numProcessedEvents;
        /* 0x0020 */ int m_eventDebugNum;

    public:
        ProcessManager(const ai::ProcessManager&);
        ProcessManager();
        ~ProcessManager();
        const CStr& GetEventName(ai::eGameEvent eventId) const;
        ai::eGameEvent GetEventId(const CStr& eventName) const;
        void Update(float elapsedTime, unsigned int framesPassed, unsigned int workTime);
        void UpdateDieEvents();
        void PostMessageA(ai::Event const&);
        void PostMessageA(int eventId, int recipientObjId, int senderObjId, float timeOut, m3d::AIParam param1, m3d::AIParam param2, int framesToPass);
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void Clear();
        int GetNumEventsInQueue() const;
        int GetNumProcessedEvents() const;

    private:
        void _RegisterEvents();
        void _RegisterEvent(const CStr& eventName, ai::eGameEvent eventId);
        void _EnsureAllEventsAreRegistered() const;
    }; /* size: 0x0024 */

    static_assert(sizeof(ProcessManager) == 0x0024);

    inline ProcessManager* theProcessManager = nullptr;
}

#pragma once
#include "objects/base/obj.h"

namespace ai
{
    class Event;

    class ProcessManager
    {
        struct auxEventInfo;

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
        void PostMessageA(const ai::Event& evn);
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

#ifdef RETRUXX_DLL
    inline ProcessManager* theProcessManager = (*inject::cast<ai::ProcessManager**>(0x00A17990));
#else
    inline ProcessManager* theProcessManager = nullptr;
#endif
}

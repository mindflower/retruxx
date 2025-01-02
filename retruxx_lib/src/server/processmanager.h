#pragma once
#include "objects/base/obj.h"

namespace ai
{
    class Event;

    class ProcessManager
    {
    public:
        class auxEventInfo
        {
        public:
            eGameEvent m_eventId;
            CStr m_eventName;
        };

    public:
        int GetNumProcessedEvents() const;
        void Update(float, unsigned int, unsigned int);
        ProcessManager();
        CStr const& GetEventName(eGameEvent) const;
        void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void Clear();
        ~ProcessManager();
        void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        eGameEvent GetEventId(CStr const&) const;
        int GetNumEventsInQueue() const;
        void UpdateDieEvents();
        void PostMessageA(Event const&);
        void PostMessageA(int, int, int, float, m3d::AIParam, m3d::AIParam, int);

    private:
        void _RegisterEvents();
        void _RegisterEvent(CStr const&, eGameEvent);
        void _EnsureAllEventsAreRegistered() const;

    private:
        std::vector<auxEventInfo> m_eventInfo;
        std::list<Event> m_eventQueue;
        int m_numProcessedEvents = 0;
        int m_eventDebugNum = 0;
    };

#ifdef RETRUXX_DLL
    inline ProcessManager* theProcessManager = (*inject::cast<ai::ProcessManager**>(0x00A17990));
#else
    inline ProcessManager* theProcessManager = nullptr;
#endif
}

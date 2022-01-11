#pragma once
#include <core/aiparam.h>
#include <core/stringm3d.h>

namespace m3d
{
    class Event;

    //IMPORTANT: fields and member order is strict!
    class IEventHandler
    {
    public:
        virtual ~IEventHandler() = default;
        virtual int HandleEvent(m3d::Event const&) = 0;
    };

    class Event
    {
    public:
        long double m_timeStamp = 0.0;
        int m_eventType = 0;
        CStr m_strEv;
        AIParam m_aiParamEv;
        //TODO: union
        union
        {
            void* m_void[4] = {0};
            IEventHandler* m_handle[4];
            unsigned int m_uintEv[4];
            int m_intEv[4];
            unsigned __int16 m_ushortEv[8];
            __int16 m_shortEv[8];
            unsigned __int8 m_byteEv[16];
        };
        Event(Event const&);
        Event();
        ~Event();
    };
}

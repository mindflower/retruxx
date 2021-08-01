#pragma once
#include <core/aiparam.h>
#include <core/stringm3d.h>

namespace m3d
{
    class IEventHandler
    {
    public:
        virtual ~IEventHandler();

    private:
        //IEventHandler_vtbl *__vftable /*VFT*/;
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
    };
}

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
        Event();

    private:
        long double m_timeStamp;
        int m_eventType;
        CStr m_strEv;
        AIParam m_aiParamEv;
        //$D533278145F5EFDE7C11BA6D07B21BE6 ___u4;
    };
}

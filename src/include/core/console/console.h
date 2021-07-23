#pragma once
#include <i_event.h>
#include <core/clazz.h>

namespace m3d
{
    class CVar;

    class CConsoleParams
    {
    private:
        char* string;
        int length;
        int numTokens;
    };

    class IConHandler
    {
    private:
        //m3d::IConHandler_vtbl* __vftable /*VFT*/;
    };

    class IConsole :
        public Object,
        public IEventHandler
    {
    public:
        virtual Class* GetRtClass() const;
        virtual ~IConsole();
        static Class* __fastcall GetBaseClass();

    protected:
        IConsole();
    };
}

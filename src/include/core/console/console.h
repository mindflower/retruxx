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

    struct CConsoleCommands
    {
        char const* m_name;
        int m_id = 0;
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

        Class*  GetRtClass();
        void RegisterCommand(char const*, int, IConHandler*);
        void RegisterCVar(CVar&, IConHandler*);
        void UnregisterCVar(CVar&);
        void Init(int, int);
        void Clear();
        void CheckResize(int, int);
        void PrintF(CStr const&);
        void ForceRender();
        void Render();
        void NotifyMode(bool);
        void setNumNotifyLines(int);
        bool DumpToFile(char const*);
        void ScrollUp(int);
        void ScrollDown(int);
        bool isActive();
        bool Toggle();
        void SetScreenSize(float);
        int InputLine(CStr const&);
        void executeCommand(CStr const&);
        int Load(CStr const&);
        int Save(CStr const&);

    protected:
        IConsole();
    };
}

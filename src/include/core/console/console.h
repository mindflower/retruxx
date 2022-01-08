#pragma once
#include <i_event.h>
#include <core/clazz.h>

namespace m3d
{
    class CVar;

    class CConsoleParams
    {
    public:
        void Set(char const*);
        char const* UnsafeStringToken(int, char) const;
        ~CConsoleParams();
        CConsoleParams& operator=(char const*);
        int IntToken(int, char) const;
        CConsoleParams(CConsoleParams const&);
        CConsoleParams(char const*);
        CConsoleParams(int);
        int NumOfTokens(char) const;
        char* StringToken(int, char*, int, char) const;
        float FloatToken(int, char) const;

    private:
        char* string = nullptr;
        int length = 0;
        int numTokens = 0;
    };

    struct CConsoleCommands
    {
        char const* m_name = nullptr;
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
        static Class* GetBaseClass();

    public:
        RT_CLASS_DECLARE(IConsole);

    public:
        virtual ~IConsole() = default;
        virtual void RegisterCommand(char const*, int, IConHandler*) = 0;
        virtual void RegisterCVar(CVar*, IConHandler*) = 0;
        virtual void UnregisterCVar(CVar*) = 0;
        virtual void Init(int, int) = 0;
        virtual void Clear() = 0;
        virtual void CheckResize(int, int) = 0;
        virtual void PrintF(CStr const&) = 0;
        virtual void ForceRender() = 0;
        virtual void Render() = 0;
        virtual void NotifyMode(bool) = 0;
        virtual void setNumNotifyLines(int) = 0;
        virtual bool DumpToFile(char const*) const = 0;
        virtual void ScrollUp(int) = 0;
        virtual void ScrollDown(int) = 0;
        virtual bool isActive() const = 0;
        virtual bool Toggle() = 0;
        virtual void SetScreenSize(float) = 0;
        virtual int InputLine(CStr const&) = 0;
        virtual void executeCommand(CStr const&) = 0;
        virtual int Load(CStr const&) = 0;
        virtual int Save(CStr const&) = 0;

    protected:
        IConsole() = default;
    };

    IConsole* ConsoleFactory();
}

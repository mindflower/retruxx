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
        static inline char szParmBuffer[1024] = {0};
        char* string = nullptr;
        int length = 0;
        mutable int numTokens = 0;
    };

    struct CConsoleCommands
    {
        char const* m_name = nullptr;
        int m_id = 0;
    };

    //IMPORTANT: fields and members order is strict
    struct IConHandler
    {
        /* 0x0000 */;
        virtual void HandleCommand(int, const m3d::CConsoleParams&) = 0 /* 0x00 */;
        virtual bool HandleCVar(const m3d::CVar*, const m3d::CConsoleParams&) = 0 /* 0x04 */;
    }; /* size: 0x0004 */

    class IConsole :
        public Object,
        public IEventHandler
    {
    public:
        IConsole(IConsole const&);
        IConsole();
        virtual ~IConsole() = default;
        static Class* GetBaseClass();
        virtual Class* GetRtClass() const;
        RT_CLASS_DECLARE(IConsole);

    public:
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
    };

    IConsole* ConsoleFactory();
}

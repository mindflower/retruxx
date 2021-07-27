#pragma once
#include "clazz.h"
#include "ini.h"
#include <windows.h>

namespace m3d
{
    class EngineConfig;
    class Log;
    class MemoryManager;
    class ScriptServer;

    namespace fs
    {
        class FileServer;
    }

    namespace cmn
    {
        class Timer;
    }

    struct MemoryAllocationRoutines
    {
        void* (__fastcall* AllocMem)(unsigned int, const char*, int);
        void* (__fastcall* ReallocMem)(void*, unsigned int, const char*, int);
        void(__fastcall* FreeMem)(void*, const char*, int);
    };

    class Kernel
    {
    public:
        virtual void UnRegisterGlobal(char const*);
        virtual void SysError(CStr const&, CStr const&);
        virtual ScriptServer& GetScriptServer();
        virtual int __stdcall MessageBoxA(HWND, char const*, char const*, unsigned int);
        virtual EngineConfig& GetEngineCfg();
        virtual unsigned int debugMemUsed() const;
        virtual cmn::Timer& GetTimer();
        virtual Class* FindClass(char const*);
        virtual ~Kernel();
        virtual void DumpMem(char const*);
        virtual unsigned int debugMemAllocated() const;
        virtual void AddClass(Class*);
        virtual fs::FileServer& GetFileServer();
        virtual cmn::IniFile* CreateIniFile();
        virtual unsigned int debugMemOverhead() const;
        virtual void SetClipboardData(char const*) const;
        virtual void __cdecl KernelLog(char const*, ...);
        virtual void TurnAggressiveMemoryDebugMode(bool);
        virtual Object* RegisterGlobal(Object*, char const*);
        virtual Object* New(char const*);
        virtual Object* New(Class*);
        virtual Object* FindGlobal(char const*);
        virtual cmn::XmlFile* CreateXmlFile();
        virtual CStr GetClipboardData() const;
        virtual int debugMemLastAllocSize() const;
        virtual void UnRegisterGlobalObject(Object const*);
        virtual void RemoveClass(Class*);
        virtual void GetListOfClasses(Class**&, unsigned int&);

        Kernel();
        int GetUniqueId();
        bool OpenLog(char const*);

    private:
        MemoryManager* m_memMan;
        fs::FileServer* m_fileMan;
        cmn::Timer* m_timer;
        EngineConfig* m_engineConfig;
        ScriptServer* m_scriptServer;
        MemoryAllocationRoutines g_mar;
        Log* m_Log;
    };

    extern Kernel* g_Kernel;
}

#define LOG(msg, level) //g_Kernel->GetLog().sourceLine() = __LINE__; g_Kernel->GetLog().setSourceFile(__FILE__); g_Kernel->GetLog().logTex(msg, level)

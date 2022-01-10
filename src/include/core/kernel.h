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

    //IMPORTANT: fields and member order is strict!
    class Kernel
    {
    private:
        MemoryManager* m_memMan = nullptr;
        fs::FileServer* m_fileMan = nullptr;
        cmn::Timer* m_timer = nullptr;
        EngineConfig* m_engineConfig = nullptr;
        ScriptServer* m_scriptServer = nullptr;

    public:
        virtual ~Kernel();

    public:
        MemoryAllocationRoutines g_mar;

    public:
        int GetUniqueId();
        virtual void AddClass(Class*);
        virtual void RemoveClass(Class*);
        virtual Class* FindClass(char const*);
        virtual void GetListOfClasses(Class**&, unsigned int&);
        virtual Object* New(Class*);
        virtual Object* New(char const*);
        virtual Object* RegisterGlobal(Object*, char const*);
        virtual Object* FindGlobal(char const*);
        virtual void UnRegisterGlobal(char const*);
        virtual void UnRegisterGlobalObject(Object const*);
        virtual void DumpMem(char const*);
        virtual void TurnAggressiveMemoryDebugMode(bool);
        virtual unsigned int debugMemUsed() const;
        virtual unsigned int debugMemAllocated() const;
        virtual unsigned int debugMemOverhead() const;
        virtual int debugMemLastAllocSize() const;
        virtual void SysError(CStr const&, CStr const&);
        virtual cmn::IniFile* CreateIniFile();
        virtual cmn::XmlFile* CreateXmlFile();
        virtual cmn::Timer& GetTimer();
        virtual fs::FileServer& GetFileServer();
        virtual ScriptServer& GetScriptServer();
        virtual EngineConfig& GetEngineCfg();
        virtual CStr GetClipboardData() const;
        virtual void SetClipboardData(char const*) const;
        bool OpenLog(char const*);
        virtual void KernelLog(char const*, ...);
        virtual int MessageBoxA(HWND, char const*, char const*, unsigned int);

    public:
        Log* m_Log = nullptr;

        class auxLogFlow
        {
            const char* m_str;
        };

        class auxLogBlock
        {
            const char* m_str;
        };

        Kernel();
    };

    extern Kernel* g_Kernel;
}

#define M3D_LOG_INFO(msg) //g_Kernel->GetLog().sourceLine() = __LINE__; g_Kernel->GetLog().setSourceFile(__FILE__); g_Kernel->GetLog().logTex(msg, LOG_INFO)
#define M3D_LOG_ERR(msg) //g_Kernel->GetLog().sourceLine() = __LINE__; g_Kernel->GetLog().setSourceFile(__FILE__); g_Kernel->GetLog().logTex(msg, LOG_ERR)
#define M3D_LOG_WARN(msg) //g_Kernel->GetLog().sourceLine() = __LINE__; g_Kernel->GetLog().setSourceFile(__FILE__); g_Kernel->GetLog().logTex(msg, LOG_WARN)
#define SYS_ERROR(msg)
#define M3D_ASSERT(cond)
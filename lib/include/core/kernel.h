#pragma once
#include <windows.h>
#include <cstddef>

class CStr;

namespace m3d
{
    class EngineConfig;
    class Log;
    class MemoryManager;
    class ScriptServer;
    struct Class;
    class Object;

    namespace fs
    {
        class FileServer;
    }

    namespace cmn
    {
        class Timer;
        class IniFile;
        class XmlFile;
    }  // namespace cmn

    struct MemoryAllocationRoutines
    {
        void*(__fastcall* AllocMem)(std::size_t, char const*, int);
        void*(__fastcall* ReallocMem)(void*, std::size_t, char const*, int);
        void(__fastcall* FreeMem)(void*, char const*, int);
    };

    //IMPORTANT: fields and member order is strict!
    class Kernel
    {
    private:
        m3d::MemoryManager* m_memMan = nullptr;
        m3d::fs::FileServer* m_fileMan = nullptr;
        m3d::cmn::Timer* m_timer = nullptr;
        m3d::EngineConfig* m_engineConfig = nullptr;
        m3d::ScriptServer* m_scriptServer = nullptr;

    public:
        Kernel(m3d::Kernel const&);
        Kernel();
        virtual ~Kernel();

        m3d::MemoryAllocationRoutines g_mar;

        int GetUniqueId();
        virtual void AddClass(m3d::Class* rtClass);
        virtual void RemoveClass(m3d::Class* rtClass);
        virtual m3d::Class* FindClass(char const* className);
        virtual void GetListOfClasses(m3d::Class**& classList, unsigned int& numOfClasses);
        virtual m3d::Object* New(m3d::Class* cl);
        virtual m3d::Object* New(char const* className);
        virtual m3d::Object* RegisterGlobal(m3d::Object* object, char const* name);
        virtual m3d::Object* FindGlobal(char const* name);
        virtual void UnRegisterGlobal(char const* name);
        virtual void UnRegisterGlobalObject(m3d::Object const* object);
        virtual void DumpMem(char const* fileName);
        virtual void TurnAggressiveMemoryDebugMode(bool bOn);
        virtual unsigned int debugMemUsed() const;
        virtual unsigned int debugMemAllocated() const;
        virtual unsigned int debugMemOverhead() const;
        virtual int debugMemLastAllocSize() const;
        virtual void SysError(CStr const& whence, CStr const& descr);
        virtual m3d::cmn::IniFile* CreateIniFile();
        virtual m3d::cmn::XmlFile* CreateXmlFile();
        virtual m3d::cmn::Timer& GetTimer();
        virtual m3d::fs::FileServer& GetFileServer();
        virtual m3d::ScriptServer& GetScriptServer();
        virtual m3d::EngineConfig& GetEngineCfg();
        virtual CStr GetClipboardData() const;
        virtual void SetClipboardData(char const* str) const;
        bool OpenLog(char const* logFileName);
        virtual void KernelLog(char const* str, ...);
        virtual int
            MessageBoxA(HWND__* hWnd, char const* pszText, char const* pszCaption, unsigned int uType);

        m3d::Log* m_Log = nullptr;

        struct auxLogFlow
        {
            auxLogFlow(char const* functionName);
            ~auxLogFlow();
            char const* m_str;
        };

        struct auxLogBlock
        {
            auxLogBlock(char const* functionName);
            ~auxLogBlock();
            char const* m_str;
        };
    };

    // The kernel is handed to the original driver DLLs (renderer, input, sound), which call
    // its virtuals and read g_mar.AllocMem (+0x18) and g_mar.FreeMem (+0x20) directly.
    // Didnt true for x64
    // static_assert(sizeof(MemoryAllocationRoutines) == 0x000c);
    // static_assert(sizeof(Kernel) == 0x0028);
    // static_assert(offsetof(Kernel, g_mar) == 0x0018);

    extern Kernel* g_Kernel;
}  // namespace m3d

#define SYS_ERROR(msg) m3d::g_Kernel->SysError((__FILE__ ":") + CStr(__LINE__), (msg))
#define M3D_ASSERT(cond) \
    if (!(cond))         \
    SYS_ERROR(#cond)

#define M3D_CRITICAL_ERROR(msg)           \
    M3D_LOG_ERR(CStr("Error: ") + (msg)); \
    SYS_ERROR("!\"Critical error, see log\"");

#define M3D_KERNEL m3d::g_Kernel

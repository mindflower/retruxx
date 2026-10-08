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
        /* 0x0004 */ m3d::MemoryManager* m_memMan = nullptr;
        /* 0x0008 */ m3d::fs::FileServer* m_fileMan = nullptr;
        /* 0x000c */ m3d::cmn::Timer* m_timer = nullptr;
        /* 0x0010 */ m3d::EngineConfig* m_engineConfig = nullptr;
        /* 0x0014 */ m3d::ScriptServer* m_scriptServer = nullptr;

    public:
        Kernel(m3d::Kernel const&);
        Kernel();
        virtual ~Kernel() /* 0x00 */;

        /* 0x0018 */ m3d::MemoryAllocationRoutines g_mar;

        int GetUniqueId();
        virtual void AddClass(m3d::Class* rtClass) /* 0x04 */;
        virtual void RemoveClass(m3d::Class* rtClass) /* 0x08 */;
        virtual m3d::Class* FindClass(char const* className) /* 0x0c */;
        virtual void GetListOfClasses(m3d::Class**& classList, unsigned int& numOfClasses) /* 0x10 */;
        virtual m3d::Object* New(m3d::Class* cl) /* 0x18 */;
        virtual m3d::Object* New(char const* className) /* 0x18 */;
        virtual m3d::Object* RegisterGlobal(m3d::Object* object, char const* name) /* 0x1c */;
        virtual m3d::Object* FindGlobal(char const* name) /* 0x20 */;
        virtual void UnRegisterGlobal(char const* name) /* 0x24 */;
        virtual void UnRegisterGlobalObject(m3d::Object const* object) /* 0x28 */;
        virtual void DumpMem(char const* fileName) /* 0x2c */;
        virtual void TurnAggressiveMemoryDebugMode(bool bOn) /* 0x30 */;
        virtual unsigned int debugMemUsed() const /* 0x34 */;
        virtual unsigned int debugMemAllocated() const /* 0x38 */;
        virtual unsigned int debugMemOverhead() const /* 0x3c */;
        virtual int debugMemLastAllocSize() const /* 0x40 */;
        virtual void SysError(CStr const& whence, CStr const& descr) /* 0x44 */;
        virtual m3d::cmn::IniFile* CreateIniFile() /* 0x48 */;
        virtual m3d::cmn::XmlFile* CreateXmlFile() /* 0x4c */;
        virtual m3d::cmn::Timer& GetTimer() /* 0x50 */;
        virtual m3d::fs::FileServer& GetFileServer() /* 0x54 */;
        virtual m3d::ScriptServer& GetScriptServer() /* 0x58 */;
        virtual m3d::EngineConfig& GetEngineCfg() /* 0x5c */;
        virtual CStr GetClipboardData() const /* 0x60 */;
        virtual void SetClipboardData(char const* str) const /* 0x64 */;
        bool OpenLog(char const* logFileName);
        virtual void KernelLog(char const* str, ...) /* 0x68 */;
        virtual int
            MessageBoxA(HWND__* hWnd, char const* pszText, char const* pszCaption, unsigned int uType) /* 0x6c */;

        /* 0x0024 */ m3d::Log* m_Log = nullptr;

        struct auxLogFlow
        {
            auxLogFlow(char const* functionName);
            ~auxLogFlow();
            /* 0x0000 */ char const* m_str;
        }; /* size: 0x0004 */

        struct auxLogBlock
        {
            auxLogBlock(char const* functionName);
            ~auxLogBlock();
            /* 0x0000 */ char const* m_str;
        }; /* size: 0x0004 */
    }; /* size: 0x0028 */

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

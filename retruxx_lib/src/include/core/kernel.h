#pragma once
#include <windows.h>

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
        /* 0x0004 */ m3d::MemoryManager* m_memMan = nullptr;
        /* 0x0008 */ m3d::fs::FileServer* m_fileMan = nullptr;
        /* 0x000c */ m3d::cmn::Timer* m_timer = nullptr;
        /* 0x0010 */ m3d::EngineConfig* m_engineConfig = nullptr;
        /* 0x0014 */ m3d::ScriptServer* m_scriptServer = nullptr;

    public:
        Kernel(const m3d::Kernel&);
        Kernel();
        virtual  ~Kernel() /* 0x00 */;

        /* 0x0018 */ m3d::MemoryAllocationRoutines g_mar;

        int GetUniqueId();
        virtual void AddClass(m3d::Class* rtClass) /* 0x04 */;
        virtual void RemoveClass(m3d::Class* rtClass) /* 0x08 */;
        virtual m3d::Class* FindClass(const char* className) /* 0x0c */;
        virtual void GetListOfClasses(m3d::Class**& classList, unsigned int& numOfClasses) /* 0x10 */;
        virtual m3d::Object* New(m3d::Class* cl) /* 0x18 */;
        virtual m3d::Object* New(const char* className) /* 0x18 */;
        virtual m3d::Object* RegisterGlobal(m3d::Object* object, const char* name) /* 0x1c */;
        virtual m3d::Object* FindGlobal(const char* name) /* 0x20 */;
        virtual void UnRegisterGlobal(const char* name) /* 0x24 */;
        virtual void UnRegisterGlobalObject(const m3d::Object* object) /* 0x28 */;
        virtual void DumpMem(const char* fileName) /* 0x2c */;
        virtual void TurnAggressiveMemoryDebugMode(bool bOn) /* 0x30 */;
        virtual unsigned int debugMemUsed() const /* 0x34 */;
        virtual unsigned int debugMemAllocated() const /* 0x38 */;
        virtual unsigned int debugMemOverhead() const /* 0x3c */;
        virtual int debugMemLastAllocSize() const /* 0x40 */;
        virtual void SysError(const CStr& whence, const CStr& descr) /* 0x44 */;
        virtual m3d::cmn::IniFile* CreateIniFile() /* 0x48 */;
        virtual m3d::cmn::XmlFile* CreateXmlFile() /* 0x4c */;
        virtual m3d::cmn::Timer& GetTimer() /* 0x50 */;
        virtual m3d::fs::FileServer& GetFileServer() /* 0x54 */;
        virtual m3d::ScriptServer& GetScriptServer() /* 0x58 */;
        virtual m3d::EngineConfig& GetEngineCfg() /* 0x5c */;
        virtual CStr GetClipboardData() const /* 0x60 */;
        virtual void SetClipboardData(const char* str) const /* 0x64 */;
        bool OpenLog(const char* logFileName);
        virtual void KernelLog(const char* str, ...) /* 0x68 */;
        virtual int MessageBoxA(HWND__* hWnd, const char* pszText, const char* pszCaption, unsigned int uType) /* 0x6c */;

        /* 0x0024 */ m3d::Log* m_Log = nullptr;

        struct auxLogFlow
        {
            auxLogFlow(const char* functionName);
            ~auxLogFlow();
            /* 0x0000 */ const char* m_str;
        }; /* size: 0x0004 */

        struct auxLogBlock
        {
            auxLogBlock(const char* functionName);
            ~auxLogBlock();
            /* 0x0000 */ const char* m_str;
        }; /* size: 0x0004 */

        static Kernel* instance();

    }; /* size: 0x0028 */

    static_assert(sizeof(Kernel) == 0x0028);

    extern Kernel* g_Kernel;
}

#define SYS_ERROR(msg)              m3d::g_Kernel->SysError((__FILE__ ":") + CStr(__LINE__), (msg))
#define M3D_ASSERT(cond)            if (!(cond)) SYS_ERROR(#cond)
#define M3D_CRITICAL_ERROR(msg)     M3D_LOG_ERR(CStr("Error: ") + (msg)); SYS_ERROR("!\"Critical error, see log\"");
#define M3D_KERNEL                  m3d::g_Kernel
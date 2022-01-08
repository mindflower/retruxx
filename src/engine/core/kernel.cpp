#include "memoryman.h"
#include <atomic>
#include <cassert>
#include <config.h>
#include <m3dapp.h>
#include <map>
#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>
#include <core/timer.h>
#include <file/fileserver.h>
#include <ode/odememory.h>
#include <script/scriptserver.h>


namespace
{
    std::map<CStr, m3d::Class*>* m_classes = nullptr;
    std::map<CStr, m3d::Object*>* m_lGlobals = nullptr;
    m3d::MemoryManager* mm = nullptr;

    void* __fastcall AllocateMemory(unsigned int sz, char const* file, int linenum)
    {
        return mm->Malloc(sz, file, linenum);
    }

    void* __fastcall ReallocateMemory(void* mem, unsigned int sz, char const* file, int linenum)
    {
        return mm->Realloc(mem, sz, file, linenum);
    }

    void __fastcall FreeMemory(void* p, char const* file , int linenum)
    {
        return mm->Free(p);
    }
}

namespace m3d
{
    Kernel* g_Kernel = nullptr;
    Kernel kernelObject;

    void Kernel::UnRegisterGlobal(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Kernel::SysError(CStr const& whence, CStr const& descr)
    {
        if (Application::g_pApp)
        {
            Application::g_pApp->sysError(whence, descr);
        }
        ::MessageBox(0, (descr + ':' + whence).c_str(), TEXT("Error"), MB_ICONHAND);
    }

    ScriptServer& Kernel::GetScriptServer()
    {
        throw std::logic_error("Not implemented");
    }

    int Kernel::MessageBoxA(HWND, char const*, char const*, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    EngineConfig& Kernel::GetEngineCfg()
    {
        return *m_engineConfig;
    }

    unsigned Kernel::debugMemUsed() const
    {
        throw std::logic_error("Not implemented");
    }

    cmn::Timer& Kernel::GetTimer()
    {
        throw std::logic_error("Not implemented");
    }

    Class* Kernel::FindClass(char const* className)
    {
        //TODO: check correctness
        const auto it = m_classes->find(className);
        if (it != m_classes->cend())
        {
            return it->second;
        }
        return nullptr;
    }

    Kernel::~Kernel()
    {
        delete m_engineConfig;
        m_fileMan->Shutdown();
        delete m_fileMan;
        delete m_timer;
        m_scriptServer->done();
        m_scriptServer->DecRef();
        delete m_lGlobals;
        delete m_classes;
        delete m_Log;
        delete m_memMan;

    }

    void Kernel::DumpMem(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Kernel::debugMemAllocated() const
    {
        throw std::logic_error("Not implemented");
    }

    void Kernel::AddClass(Class* rtClass)
    {
        assert(nullptr == FindClass(rtClass->m_className));
        rtClass->m_index = m_classes->size();
        m_classes->emplace(rtClass->m_className, rtClass);
    }

    fs::FileServer& Kernel::GetFileServer()
    {
        throw std::logic_error("Not implemented");
    }

    cmn::IniFile* Kernel::CreateIniFile()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Kernel::debugMemOverhead() const
    {
        throw std::logic_error("Not implemented");
    }

    void Kernel::SetClipboardData(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Kernel::KernelLog(char const*, ...)
    {
        throw std::logic_error("Not implemented");
    }

    void Kernel::TurnAggressiveMemoryDebugMode(bool)
    {
        throw std::logic_error("Not implemented");
    }

    Object* Kernel::RegisterGlobal(Object* object, char const* name)
    {
        //TODO: check this
        auto const it = m_lGlobals->find(name);
        if (it != m_lGlobals->cend())
        {
            return it->second;
        }
        return (*m_lGlobals)[name] = object;
    }

    Object* Kernel::New(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    Object* Kernel::New(Class*)
    {
        throw std::logic_error("Not implemented");
    }

    Object* Kernel::FindGlobal(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    cmn::XmlFile* Kernel::CreateXmlFile()
    {
        throw std::logic_error("Not implemented");
    }

    CStr Kernel::GetClipboardData() const
    {
        throw std::logic_error("Not implemented");
    }

    int Kernel::debugMemLastAllocSize() const
    {
        throw std::logic_error("Not implemented");
    }

    void Kernel::UnRegisterGlobalObject(Object const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Kernel::RemoveClass(Class*)
    {
        throw std::logic_error("Not implemented");
    }

    void Kernel::GetListOfClasses(Class**&, unsigned&)
    {
        throw std::logic_error("Not implemented");
    }

    Kernel::Kernel()
    {
        assert(nullptr == g_Kernel);
        g_Kernel = this;

        //TODO: operator new
        m_memMan = new MemoryManager;
        g_mar.AllocMem = AllocateMemory;
        g_mar.ReallocMem = ReallocateMemory;
        g_mar.FreeMem = FreeMemory;
        mm = m_memMan;

        m_classes = new std::map<CStr, m3d::Class*>;
        AddClass(RT_CLASS_LOCAL(Object));

        m_lGlobals = new std::map<CStr, m3d::Object*>;

        m_engineConfig = new EngineConfig;
        m_fileMan = new fs::FileServer;
        m_fileMan->Initialize("data\\datasources.txt");

        char workingDirectory[MAX_PATH] = { 0 };
        if (::GetCurrentDirectoryA(0x100, workingDirectory))
        {
            m_fileMan->SetCurrentWorkDir(workingDirectory);
        }
        m_timer = new cmn::Timer;

        AddClass(RT_CLASS_LOCAL(ScriptServer));
        auto scriptServer = dynamic_cast<ScriptServer*>(ScriptServer::m_classScriptServer.NewInstance());
        if (scriptServer)
        {
            scriptServer->m_scriptHandle = 0;
        }
        m_scriptServer = scriptServer;
        m_scriptServer->IncRef();
        m_scriptServer->init();
        RegisterGlobal(m_scriptServer, "script server");
        OdeSetMemoryHandlers();
    }

    bool Kernel::OpenLog(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Kernel::GetUniqueId()
    {
        static std::atomic_int g_uniqueId = 0;
        if (g_uniqueId < 0)
        {
            g_uniqueId = 0;
        }
        return g_uniqueId++;
    }
}

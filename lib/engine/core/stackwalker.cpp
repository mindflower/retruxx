#include <stdexcept>
#include <core/kernel.h>
#include <core/log.h>
#include <core/stackwalker.h>
#include <file/fileserver.h>

CStr g_currentBinaryName;

TCHAR s_szExceptionLogFileName[MAX_PATH + 1] = "\\exceptions.log";
TCHAR s_szDumpFileName[MAX_PATH + 1] = "\\dump.dmp";
TCHAR s_szGameLogFileName[MAX_PATH + 1] = "\\exmachina.log";

TCHAR* g_pszAllocLogName = nullptr;
eAllocCheckOutput g_CallstackOutputType = ACOutput_Simple;

int s_bUnhandledExeptionFilterSet = 0;
int g_bInitialized = 0;

HMODULE g_hImagehlpDll = nullptr;

using MiniDumpWriteDumpType = BOOL(WINAPI*)(HANDLE, DWORD, HANDLE, int, void*, void*, void*);

// NOTE: InitStackWalk does not resolve MiniDumpWriteDump yet, so this stays null and WriteDump is a no-op.
MiniDumpWriteDumpType pMDWD = nullptr;

void __fastcall WriteDump(char const* szFileName, _EXCEPTION_POINTERS* pExcP, unsigned long& lastError)
{
    // RVA 0x75A0E0
    struct MinidumpExceptionInformation
    {
        DWORD ThreadId;
        _EXCEPTION_POINTERS* ExceptionPointers;
        BOOL ClientPointers;
    };

    lastError = 0;
    if (!pMDWD)
    {
        return;
    }

    HANDLE const file = ::CreateFileA(szFileName, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
    {
        lastError = ::GetLastError();
        return;
    }

    MinidumpExceptionInformation mdeInfo;
    mdeInfo.ExceptionPointers = pExcP;
    mdeInfo.ThreadId = ::GetCurrentThreadId();
    mdeInfo.ClientPointers = FALSE;
    // 1 = MiniDumpWithDataSegs
    if (!pMDWD(::GetCurrentProcess(), ::GetCurrentProcessId(), file, 1, pExcP ? &mdeInfo : nullptr, nullptr, nullptr))
    {
        lastError = ::GetLastError();
    }
    ::FlushFileBuffers(file);
    ::CloseHandle(file);
}

RTL_CRITICAL_SECTION g_csFileOpenClose;


//using SymCleanupType = int(*)(void*);
//using SymFunctionTableAccessType = void* (*)(void*, unsigned long);
//using SymGetLineFromAddrType = int(*)(void*, unsigned long, unsigned long*, struct _IMAGEHLP_LINE*);
//using SymGetModuleBaseType = unsigned long(*)(void*, unsigned long);
//using SymGetModuleInfoType = ;
//using SymGetOptionsType = ;
//using SymGetSymFromAddrType = ;
//using SymInitializeType = ;
//using SymSetOptionsType = ;
//using StackWalkType = ;
//using UnDecorateSymbolNameType = ;
//using SymLoadModuleType = ;
//using MiniDumpWriteDumpType = ;

LONG WINAPI CrashHandlerExceptionFilter(PEXCEPTION_POINTERS pe)
{
    RETRUXX_NOT_IMPLEMENTED;
    //TODO: ....
    //char tmp_1[0x400] = { 0 };
    //::GetCurrentDirectoryA(0x400, tmp_1);
    //if (FALSE == ::CreateDirectoryA("exceptions", 0))
    //{
    //    //TODO: check this
    //    return ::GetLastError();
    //}
    //auto
    return 0;
}

unsigned int __fastcall DeInitAllocCheck()
{
    // RVA 0x759DD0
    if (g_bInitialized)
    {
        ::EnterCriticalSection(&g_csFileOpenClose);
        g_bInitialized = 0;
        ::FreeLibrary(g_hImagehlpDll);
        ::LeaveCriticalSection(&g_csFileOpenClose);
        if (g_pszAllocLogName)
        {
            free(g_pszAllocLogName);
            g_pszAllocLogName = nullptr;
        }
        ::DeleteCriticalSection(&g_csFileOpenClose);
    }
    // NOTE: the test is inverted in the binary, so the filter is only cleared when this module did not set it.
    if (s_bUnhandledExeptionFilterSet != 1)
    {
        ::SetUnhandledExceptionFilter(nullptr);
        s_bUnhandledExeptionFilterSet = 0;
    }
    return 0;
}

int InitStackWalk()
{
    if (g_bInitialized)
    {
        return 0;
    }
    g_hImagehlpDll = ::LoadLibraryA("dbghelp.dll");
    if (g_hImagehlpDll == NULL)
    {
        M3D_LOG_INFO("LoadLibrary( \"dbghelp.dll\" ): GetLastError = " + CStr(::GetLastError()));
        g_bInitialized = 0;
        return 1;
    }
    //TODO:...
    {
        g_bInitialized = 1;
        ::InitializeCriticalSection(&g_csFileOpenClose);
        return 0;
    }
    //pSC = GetProcAddress(g_hImagehlpDll, "SymCleanup");
    //pSFTA = GetProcAddress(g_hImagehlpDll, "SymFunctionTableAccess");
    //pSGLFA = GetProcAddress(g_hImagehlpDll, "SymGetLineFromAddr");
    //pSGMB = GetProcAddress(g_hImagehlpDll, "SymGetModuleBase");
    //pSGMI = GetProcAddress(g_hImagehlpDll, "SymGetModuleInfo");
    //pSGO = GetProcAddress(g_hImagehlpDll, "SymGetOptions");
    //pSGSFA = GetProcAddress(g_hImagehlpDll, "SymGetSymFromAddr");
    //pSI = GetProcAddress(g_hImagehlpDll, "SymInitialize");
    //pSSO = GetProcAddress(g_hImagehlpDll, "SymSetOptions");
    //pSW = GetProcAddress(g_hImagehlpDll, "StackWalk");
    //pUDSN = GetProcAddress(g_hImagehlpDll, "UnDecorateSymbolName");
    //pSLM = GetProcAddress(g_hImagehlpDll, "SymLoadModule");
    //pMDWD = GetProcAddress(g_hImagehlpDll, "MiniDumpWriteDump");
    //if (pSC && pSFTA && pSGMB && pSGMI && pSGO && pSGSFA && pSI && pSSO && pSW && pUDSN && pSLM && pMDWD)
    //{
    //    g_bInitialized = 1;
    //    ::InitializeCriticalSection(&g_csFileOpenClose);
    //    return 0;
    //}
    //M3D_LOG_INFO("GetProcAddress(): some required function not found.");
    //::FreeLibrary(g_hImagehlpDll);
    //g_bInitialized = 0;
    //return 1;
}

int InitAllocCheck(eAllocCheckOutput eOutput, int bSetUnhandledExeptionFilter, unsigned long ulShowStackAtAlloc)
{
    //TODO: check correctness
    //RETRUXX_NOT_IMPLEMENTED;
    auto const* workDir = m3d::g_Kernel->GetFileServer().GetCurrentWorkDir();
    TCHAR szModName[MAX_PATH + 1] = { 0 };
    if (::GetModuleFileName(NULL, szModName, MAX_PATH) != 0)
    {
        strncpy_s(s_szExceptionLogFileName, szModName, MAX_PATH);
    }
    else
    {
        //TODO: check this
        strncpy_s(szModName, "error", 5);
    }
    auto const dirName = DirectoryFromFileName(workDir);
    auto const fileName = NameFromFileName(s_szExceptionLogFileName);
    g_currentBinaryName = fileName;
    g_currentBinaryName.toLower();

    TCHAR szName[MAX_PATH + 1] = { 0 };
    unsigned counter = 0;
    do
    {
        sprintf_s(szModName, "%s/exceptions/%s%04u.log", dirName.c_str(), fileName.c_str(), counter);
        sprintf_s(s_szDumpFileName, "%s/exceptions/%s%04u.dmp", dirName.c_str(), fileName.c_str(), counter);
        sprintf_s(s_szGameLogFileName, "%s/exceptions/%s%04u_game.log", dirName.c_str(), fileName.c_str(), counter++);
    } while (GetFileAttributesA(szName) != -1);
    strncpy_s(s_szExceptionLogFileName, szName, MAX_PATH);
    M3D_LOG_INFO(CStr("Stackwalker: log file name will be ") + s_szExceptionLogFileName);
    M3D_LOG_INFO(CStr("Stackwalker: dump file name will be ") + s_szDumpFileName);
    M3D_LOG_INFO(CStr("Stackwalker: game log file name will be ") + s_szGameLogFileName);
    if (bSetUnhandledExeptionFilter)
    {
        ::SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);
        s_bUnhandledExeptionFilterSet = 1;
    }
    if (g_bInitialized)
    {
        return 2;
    }
    g_pszAllocLogName = strdup(szModName);
    g_CallstackOutputType = eOutput;
    return InitStackWalk();
}

#include <core/kernel.h>
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

LONG WINAPI CrashHandlerExceptionFilter(PEXCEPTION_POINTERS pe)
{
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

int InitStackWalk()
{
    if (g_bInitialized)
    {
        return 0;
    }
    g_hImagehlpDll = ::LoadLibraryA("dbghelp.dll");
    if (g_hImagehlpDll == NULL)
    {
        LOG("LoadLibrary( \"dbghelp.dll\" ): GetLastError = " + std::to_string(::GetLastError()), LOG_INFO);
        g_bInitialized = 0;
        return 1;
    }
    //TODO:...
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
    LOG("GetProcAddress(): some required function not found.", LOG_INFO);
    ::FreeLibrary(g_hImagehlpDll);
    g_bInitialized = 0;
    return 1;
}

int InitAllocCheck(eAllocCheckOutput eOutput, int bSetUnhandledExeptionFilter, unsigned long ulShowStackAtAlloc)
{
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
    toLower(g_currentBinaryName);

    TCHAR szName[MAX_PATH + 1] = { 0 };
    unsigned counter = 0;
    do
    {
        sprintf_s(szModName, "%s/exceptions/%s%04u.log", dirName.c_str(), fileName.c_str(), counter);
        sprintf_s(s_szDumpFileName, "%s/exceptions/%s%04u.dmp", dirName.c_str(), fileName.c_str(), counter);
        sprintf_s(s_szGameLogFileName, "%s/exceptions/%s%04u_game.log", dirName.c_str(), fileName.c_str(), counter++);
    } while (GetFileAttributesA(szName) != -1);
    strncpy_s(s_szExceptionLogFileName, szName, MAX_PATH);
    LOG(CStr("Stackwalker: log file name will be ") + s_szExceptionLogFileName, LOG_INFO);
    LOG(CStr("Stackwalker: dump file name will be ") + s_szDumpFileName, LOG_INFO);
    LOG(CStr("Stackwalker: game log file name will be ") + s_szGameLogFileName, LOG_INFO);
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
    //TODO::
}

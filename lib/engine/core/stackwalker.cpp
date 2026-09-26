#include <cstdio>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <string>
#include <vector>
#include <core/kernel.h>
#include <core/log.h>
#include <core/stackwalker.h>
#include <file/fileserver.h>
#include <m3dapp.h>
#include <dbghelp.h>
#include <psapi.h>
#include <tlhelp32.h>

CStr g_currentBinaryName;

TCHAR s_szExceptionLogFileName[MAX_PATH + 1] = "\\exceptions.log";
TCHAR s_szDumpFileName[MAX_PATH + 1] = "\\dump.dmp";
TCHAR s_szGameLogFileName[MAX_PATH + 1] = "\\exmachina.log";

TCHAR* g_pszAllocLogName = nullptr;
eAllocCheckOutput g_CallstackOutputType = ACOutput_Simple;

int s_bUnhandledExeptionFilterSet = 0;
int g_bInitialized = 0;

HMODULE g_hImagehlpDll = nullptr;

// dbghelp.dll functions, resolved by InitStackWalk.
using SymCleanupType = BOOL(WINAPI*)(HANDLE);
using SymFunctionTableAccessType = PVOID(WINAPI*)(HANDLE, DWORD);
using SymGetLineFromAddrType = BOOL(WINAPI*)(HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE);
using SymGetModuleBaseType = DWORD(WINAPI*)(HANDLE, DWORD);
using SymGetModuleInfoType = BOOL(WINAPI*)(HANDLE, DWORD, PIMAGEHLP_MODULE);
using SymGetOptionsType = DWORD(WINAPI*)();
using SymGetSymFromAddrType = BOOL(WINAPI*)(HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL);
using SymInitializeType = BOOL(WINAPI*)(HANDLE, PSTR, BOOL);
using SymSetOptionsType = DWORD(WINAPI*)(DWORD);
using StackWalkType = BOOL(WINAPI*)(DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID, PREAD_PROCESS_MEMORY_ROUTINE,
                                    PFUNCTION_TABLE_ACCESS_ROUTINE, PGET_MODULE_BASE_ROUTINE,
                                    PTRANSLATE_ADDRESS_ROUTINE);
using UnDecorateSymbolNameType = DWORD(WINAPI*)(PCSTR, PSTR, DWORD, DWORD);
using SymLoadModuleType = DWORD(WINAPI*)(HANDLE, HANDLE, PSTR, PSTR, DWORD, DWORD);
using MiniDumpWriteDumpType = BOOL(WINAPI*)(HANDLE, DWORD, HANDLE, int, void*, void*, void*);

SymCleanupType pSC = nullptr;
SymFunctionTableAccessType pSFTA = nullptr;
SymGetLineFromAddrType pSGLFA = nullptr;
SymGetModuleBaseType pSGMB = nullptr;
SymGetModuleInfoType pSGMI = nullptr;
SymGetOptionsType pSGO = nullptr;
SymGetSymFromAddrType pSGSFA = nullptr;
SymInitializeType pSI = nullptr;
SymSetOptionsType pSSO = nullptr;
StackWalkType pSW = nullptr;
UnDecorateSymbolNameType pUDSN = nullptr;
SymLoadModuleType pSLM = nullptr;
MiniDumpWriteDumpType pMDWD = nullptr;

// Symbol buffer for SymGetSymFromAddr, allocated on the first stack walk and never freed.
PIMAGEHLP_SYMBOL pSym = nullptr;
LONG g_dwShowCount = 0;
// Set until the first stack walk has loaded the module symbols.
int bFirstTime = 1;

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

int InitStackWalk();

namespace
{
    struct ModuleEntry
    {
        std::string imageName;
        std::string moduleName;
        DWORD baseAddress;
        DWORD size;
    };

    char const* GetExpectionCodeText(DWORD dwExceptionCode)
    {
        // RVA 0x759E70
        switch (dwExceptionCode)
        {
        case 0x40010005: return "DBG CONTROL C ";
        case 0x80000002: return "DATATYPE MISALIGNMENT";
        case 0x80000003: return "BREAKPOINT";
        case 0x80000004: return "SINGLE STEP";
        case 0xC0000005: return "ACCESS VIOLATION";
        case 0xC0000006: return "IN PAGE ERROR";
        case 0xC000001D: return "ILLEGAL INSTRUCTION";
        case 0xC0000025: return "NONCONTINUABLE EXCEPTION";
        case 0xC0000026: return "INVALID DISPOSITION";
        case 0xC000008C: return "ARRAY BOUNDS EXCEEDED";
        case 0xC000008D: return "FLT DENORMAL OPERAND";
        case 0xC000008E: return "FLT DIVIDE BY ZERO";
        case 0xC000008F: return "FLT INEXACT RESULT";
        case 0xC0000090: return "FLT INVALID OPERATION";
        case 0xC0000091: return "FLT OVERFLOW";
        case 0xC0000092: return "FLT STACK CHECK";
        case 0xC0000093: return "FLT UNDERFLOW";
        case 0xC0000094: return "INT DIVIDE BY ZERO";
        case 0xC0000095: return "INT OVERFLOW";
        case 0xC0000096: return "PRIV INSTRUCTION";
        case 0xC00000FD: return "STACK OVERFLOW";
        default: return "<unkown exception>";
        }
    }

    bool GetModuleListTH32(std::vector<ModuleEntry>& modules, DWORD pid, FILE* fLogFile)
    {
        // RVA 0x75ACF0 - appends the modules of process pid, found with the ToolHelp functions.
        using CreateToolhelp32SnapshotType = HANDLE(WINAPI*)(DWORD, DWORD);
        using Module32Type = BOOL(WINAPI*)(HANDLE, tagMODULEENTRY32*);

        char const* const dllname[] = {"kernel32.dll", "tlhelp32.dll"};
        CreateToolhelp32SnapshotType pCT32S = nullptr;
        Module32Type pM32F = nullptr;
        Module32Type pM32N = nullptr;
        tagMODULEENTRY32 me;
        me.dwSize = sizeof(me);
        ModuleEntry e;

        HMODULE hToolhelp = nullptr;
        for (unsigned i = 0; i < 2; ++i)
        {
            hToolhelp = ::LoadLibraryA(dllname[i]);
            if (!hToolhelp)
            {
                continue;
            }
            pCT32S = reinterpret_cast<CreateToolhelp32SnapshotType>(::GetProcAddress(hToolhelp, "CreateToolhelp32Snapshot"));
            pM32F = reinterpret_cast<Module32Type>(::GetProcAddress(hToolhelp, "Module32First"));
            pM32N = reinterpret_cast<Module32Type>(::GetProcAddress(hToolhelp, "Module32Next"));
            if (pCT32S && pM32F && pM32N)
            {
                break;
            }
            ::FreeLibrary(hToolhelp);
            hToolhelp = nullptr;
        }
        if (!hToolhelp)
        {
            return false;
        }

        // 8 = TH32CS_SNAPMODULE
        HANDLE const hSnap = pCT32S(8, pid);
        if (hSnap == INVALID_HANDLE_VALUE)
        {
            // NOTE: the library is not freed on this path.
            return false;
        }
        if (pM32F(hSnap, &me))
        {
            do
            {
                e.imageName = me.szExePath;
                e.moduleName = me.szModule;
                e.baseAddress = reinterpret_cast<DWORD>(me.modBaseAddr);
                e.size = me.modBaseSize;
                modules.push_back(e);
            } while (pM32N(hSnap, &me));
        }
        ::CloseHandle(hSnap);
        ::FreeLibrary(hToolhelp);
        return !modules.empty();
    }

    bool GetModuleListPSAPI(std::vector<ModuleEntry>& modules, HANDLE hProcess, FILE* fLogFile)
    {
        // RVA 0x75AEE0 - replaces the list with the modules of hProcess, found with the PSAPI functions.
        using EnumProcessModulesType = BOOL(WINAPI*)(HANDLE, HMODULE*, DWORD, LPDWORD);
        using GetModuleNameType = DWORD(WINAPI*)(HANDLE, HMODULE, LPSTR, DWORD);
        using GetModuleInformationType = BOOL(WINAPI*)(HANDLE, HMODULE, LPMODULEINFO, DWORD);
        enum
        {
            TTBUFLEN = 8096
        };

        ModuleEntry e;
        HMODULE const hPsapi = ::LoadLibraryA("psapi.dll");
        if (!hPsapi)
        {
            return false;
        }
        std::vector<ModuleEntry>().swap(modules);

        auto const pEPM = reinterpret_cast<EnumProcessModulesType>(::GetProcAddress(hPsapi, "EnumProcessModules"));
        auto const pGMFNE = reinterpret_cast<GetModuleNameType>(::GetProcAddress(hPsapi, "GetModuleFileNameExA"));
        auto const pGMBN = reinterpret_cast<GetModuleNameType>(::GetProcAddress(hPsapi, "GetModuleBaseNameA"));
        auto const pGMI = reinterpret_cast<GetModuleInformationType>(::GetProcAddress(hPsapi, "GetModuleInformation"));
        if (!pEPM || !pGMFNE || !pGMBN || !pGMI)
        {
            ::FreeLibrary(hPsapi);
            return false;
        }

        HMODULE* const hMods = static_cast<HMODULE*>(malloc(TTBUFLEN));
        char* const tt = static_cast<char*>(malloc(TTBUFLEN));
        DWORD cbNeeded;
        if (!pEPM(hProcess, hMods, TTBUFLEN, &cbNeeded))
        {
            fprintf(fLogFile, "%lu: EPM failed, GetLastError = %lu\n", g_dwShowCount, ::GetLastError());
        }
        else if (cbNeeded > TTBUFLEN)
        {
            // NOTE: the module count printed is always 1.
            fprintf(fLogFile, "%lu: More than %lu module handles. Huh?\n", g_dwShowCount, 1ul);
        }
        else
        {
            for (DWORD i = 0; i < cbNeeded / sizeof(HMODULE); ++i)
            {
                MODULEINFO mi;
                pGMI(hProcess, hMods[i], &mi, sizeof(mi));
                e.baseAddress = reinterpret_cast<DWORD>(mi.lpBaseOfDll);
                e.size = mi.SizeOfImage;
                tt[0] = 0;
                pGMFNE(hProcess, hMods[i], tt, TTBUFLEN);
                e.imageName = tt;
                tt[0] = 0;
                pGMBN(hProcess, hMods[i], tt, TTBUFLEN);
                e.moduleName = tt;
                modules.push_back(e);
            }
        }
        ::FreeLibrary(hPsapi);
        free(tt);
        free(hMods);
        return !modules.empty();
    }

    void EnumAndLoadModuleSymbols(HANDLE hProcess, DWORD pid, FILE* fLogFile)
    {
        // RVA 0x75B170
        // NOTE: the list is static and the ToolHelp path appends to it, so a second call would load every module
        // twice; only the first stack walk calls this.
        static std::vector<ModuleEntry> modules;
        if (!GetModuleListTH32(modules, pid, fLogFile))
        {
            GetModuleListPSAPI(modules, hProcess, fLogFile);
        }
        for (auto const& module : modules)
        {
            char* const img = _strdup(module.imageName.c_str());
            char* const mod = _strdup(module.moduleName.c_str());
            pSLM(hProcess, nullptr, img, mod, module.baseAddress, module.size);
            free(img);
            free(mod);
        }
    }

    void __stdcall ShowStackRM(HANDLE hThread, CONTEXT const& c, FILE* fLogFile,
                               PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryFunction, HANDLE hSWProcess)
    {
        // RVA 0x75B270
        enum
        {
            TTBUFLEN = 8096,
            IMGSYMLEN = sizeof(IMAGEHLP_SYMBOL),
            MAXNAMELEN = 1024,
            MAX_FRAMES = 200
        };

        CONTEXT tmpContext = c;
        HANDLE const hProcess = ::GetCurrentProcess();
        std::string symSearchPath;
        if (!fLogFile)
        {
            fLogFile = stdout;
        }
        STACKFRAME stackFrame;
        memset(&stackFrame, 0, sizeof(stackFrame));

        if (!g_bInitialized)
        {
            if (bFirstTime != 1 || (InitStackWalk(), !g_bInitialized))
            {
                bFirstTime = 0;
                fprintf(fLogFile, "%lu: Stackwalker not initialized (or was not able to initialize)!\n",
                        g_dwShowCount);
                return;
            }
        }

        ::EnterCriticalSection(&g_csFileOpenClose);
        ::InterlockedIncrement(&g_dwShowCount);
        fprintf(fLogFile, "%lu: ", g_dwShowCount);
        if (fLogFile)
        {
            char date[12];
            char time[12];
            _strdate(date);
            _strtime(time);
            fprintf(fLogFile, "%s %s", date, time);
        }
        fprintf(fLogFile, "\n");

        MEMORYSTATUS memStatus;
        memset(&memStatus, 0, sizeof(memStatus));
        memStatus.dwLength = sizeof(memStatus);
        ::GlobalMemoryStatus(&memStatus);
        if (::GetLastError())
        {
            // NOTE: the format lacks its first '%', so the show count is printed as the error code and the error
            // code is dropped.
            DWORD const lastError = ::GetLastError();
            fprintf(fLogFile, "lu: GlobalMemoryStatus(): GetLastError = %lu\n", g_dwShowCount, lastError);
        }
        else
        {
            fprintf(fLogFile, "%lu: Physical memory =%5lu MB\n", g_dwShowCount, memStatus.dwTotalPhys >> 20);
            fprintf(fLogFile, "%lu: Total memory    =%5lu MB\n", g_dwShowCount, memStatus.dwTotalPageFile >> 20);
            fprintf(fLogFile, "%lu: Physical memory available      =%5lu MB\n", g_dwShowCount,
                    memStatus.dwAvailPhys >> 20);
            fprintf(fLogFile, "%lu: Total virtual memory           =%5lu MB\n", g_dwShowCount,
                    memStatus.dwTotalVirtual >> 20);
            fprintf(fLogFile, "%lu: Total virtual memory available =%5lu MB\n", g_dwShowCount,
                    memStatus.dwAvailVirtual >> 20);
            fprintf(fLogFile, "%lu: Page file available            =%5lu MB\n", g_dwShowCount,
                    memStatus.dwAvailPageFile >> 20);
        }
        fprintf(fLogFile, "\n%lu: Data from MemoryManager:\n", g_dwShowCount);
        fprintf(fLogFile, "%lu: mem used:          %10d\n", g_dwShowCount, m3d::g_Kernel->debugMemUsed());
        fprintf(fLogFile, "%lu: mem allocated:     %10d\n", g_dwShowCount, m3d::g_Kernel->debugMemAllocated());
        fprintf(fLogFile, "%lu: mem overhead:      %10d\n", g_dwShowCount, m3d::g_Kernel->debugMemOverhead());
        fprintf(fLogFile, "%d: last allocation:   %10d\n", g_dwShowCount, m3d::g_Kernel->debugMemLastAllocSize());
        if (M3D_APP->m_sound)
        {
            unsigned int curUsedBySound = 0;
            unsigned int maxUsedBySound = 0;
            M3D_APP->m_sound->GetMemUsage(curUsedBySound, maxUsedBySound);
            fprintf(fLogFile, "%lu: mem used by sound: %10d\n", g_dwShowCount, curUsedBySound);
        }
        fprintf(fLogFile, "\n");

        bool walk = true;
        if (!pSym)
        {
            pSym = static_cast<PIMAGEHLP_SYMBOL>(malloc(IMGSYMLEN + MAXNAMELEN));
            walk = pSym != nullptr;
        }
        if (walk && bFirstTime)
        {
            walk = false;
            char* const tt = static_cast<char*>(malloc(TTBUFLEN));
            if (tt)
            {
                symSearchPath = "";
                if (::GetCurrentDirectoryA(TTBUFLEN, tt))
                {
                    symSearchPath += tt + std::string(";");
                }
                if (::GetModuleFileNameA(nullptr, tt, TTBUFLEN))
                {
                    char* p = tt + strlen(tt) - 1;
                    for (; p >= tt; --p)
                    {
                        if (*p == '\\' || *p == '/' || *p == ':')
                        {
                            break;
                        }
                    }
                    if (p != tt)
                    {
                        if (*p == ':')
                        {
                            ++p;
                        }
                        *p = 0;
                        symSearchPath += tt + std::string(";");
                    }
                }
                if (::GetEnvironmentVariableA("_NT_SYMBOL_PATH", tt, TTBUFLEN))
                {
                    symSearchPath += tt + std::string(";");
                }
                if (::GetEnvironmentVariableA("_NT_ALTERNATE_SYMBOL_PATH", tt, TTBUFLEN))
                {
                    symSearchPath += tt + std::string(";");
                }
                if (::GetEnvironmentVariableA("SYSTEMROOT", tt, TTBUFLEN))
                {
                    symSearchPath += tt + std::string(";");
                }
                if (symSearchPath.size() > 0)
                {
                    symSearchPath = symSearchPath.substr(0, symSearchPath.size() - 1);
                }
                strncpy(tt, symSearchPath.c_str(), TTBUFLEN);
                tt[TTBUFLEN - 1] = 0;
                if (!pSI(hProcess, tt, FALSE))
                {
                    fprintf(fLogFile, "%lu: SymInitialize(): GetLastError = %lu\n", g_dwShowCount, ::GetLastError());
                    free(tt);
                }
                else
                {
                    DWORD symOptions = pSGO();
                    // SYMOPT_LOAD_LINES on, SYMOPT_UNDNAME and SYMOPT_DEFERRED_LOADS off.
                    symOptions = (symOptions & ~6u) | 0x10;
                    pSSO(symOptions);
                    EnumAndLoadModuleSymbols(hProcess, ::GetCurrentProcessId(), fLogFile);
                    free(tt);
                    walk = true;
                }
            }
        }

        if (walk)
        {
            stackFrame.AddrPC.Offset = c.Eip;
            stackFrame.AddrPC.Mode = AddrModeFlat;
            stackFrame.AddrFrame.Offset = c.Ebp;
            stackFrame.AddrFrame.Mode = AddrModeFlat;
            memset(pSym, 0, IMGSYMLEN + MAXNAMELEN);
            pSym->SizeOfStruct = IMGSYMLEN;
            pSym->MaxNameLength = MAXNAMELEN;
            IMAGEHLP_LINE Line;
            memset(&Line, 0, sizeof(Line));
            Line.SizeOfStruct = sizeof(Line);
            IMAGEHLP_MODULE Module;
            memset(&Module, 0, sizeof(Module));
            Module.SizeOfStruct = sizeof(Module);
            bFirstTime = 0;

            char undName[MAXNAMELEN];
            char undFullName[MAXNAMELEN];
            char ty[80];
            int frameNum = 0;
            for (;;)
            {
                // 0x14C = IMAGE_FILE_MACHINE_I386
                if (!pSW(0x14C, hSWProcess, hThread, &stackFrame, &tmpContext, ReadMemoryFunction, pSFTA, pSGMB,
                         nullptr))
                {
                    break;
                }
                if (g_CallstackOutputType == ACOutput_Advanced)
                {
                    fprintf(fLogFile, "\n%lu: %3d", g_dwShowCount, frameNum);
                }
                if (stackFrame.AddrPC.Offset == 0)
                {
                    if (g_CallstackOutputType == ACOutput_Simple)
                    {
                        fprintf(fLogFile, "%lu: (-nosymbols- PC == 0)\n", g_dwShowCount);
                    }
                    else if (g_CallstackOutputType == ACOutput_Advanced)
                    {
                        fprintf(fLogFile, "   (-nosymbols- PC == 0)\n");
                    }
                }
                else
                {
                    undName[0] = 0;
                    undFullName[0] = 0;
                    DWORD offsetFromSymbol = 0;
                    if (!pSGSFA(hProcess, stackFrame.AddrPC.Offset, &offsetFromSymbol, pSym))
                    {
                        if (g_CallstackOutputType == ACOutput_Advanced)
                        {
                            if (::GetLastError() != ERROR_INVALID_ADDRESS)
                            {
                                fprintf(fLogFile, "   SymGetSymFromAddr(): GetLastError = %lu\n", ::GetLastError());
                            }
                            else
                            {
                                fprintf(fLogFile, "\n");
                            }
                        }
                    }
                    else
                    {
                        // 0x1000 = UNDNAME_NAME_ONLY, 0 = UNDNAME_COMPLETE
                        pUDSN(pSym->Name, undName, MAXNAMELEN, 0x1000);
                        pUDSN(pSym->Name, undFullName, MAXNAMELEN, 0);
                        if (g_CallstackOutputType == ACOutput_Advanced)
                        {
                            if (strlen(undName) > 0)
                            {
                                fprintf(fLogFile, "     %s %+ld bytes\n", undName, offsetFromSymbol);
                            }
                            else
                            {
                                fprintf(fLogFile, "     Sig:  %s %+ld bytes\n", pSym->Name, offsetFromSymbol);
                                strcpy(undName, pSym->Name);
                            }
                            fprintf(fLogFile, "%lu:     Decl: %s\n", g_dwShowCount, undFullName);
                        }
                    }

                    DWORD offsetFromLine = 0;
                    if (pSGLFA)
                    {
                        if (!pSGLFA(hProcess, stackFrame.AddrPC.Offset, &offsetFromLine, &Line))
                        {
                            if (::GetLastError() != ERROR_INVALID_ADDRESS && frameNum > 0)
                            {
                                fprintf(fLogFile, "%lu: SymGetLineFromAddr(): GetLastError = %lu\n", g_dwShowCount,
                                        ::GetLastError());
                            }
                        }
                        else if (g_CallstackOutputType == ACOutput_Simple)
                        {
                            fprintf(fLogFile, "%lu: %s(%lu) %+ld bytes (%s)\n", g_dwShowCount, Line.FileName,
                                    Line.LineNumber, offsetFromLine, undName);
                        }
                        else if (g_CallstackOutputType == ACOutput_Advanced)
                        {
                            fprintf(fLogFile, "%lu:     Line: %s(%lu) %+ld bytes\n", g_dwShowCount, Line.FileName,
                                    Line.LineNumber, offsetFromLine);
                        }
                    }

                    if (g_CallstackOutputType == ACOutput_Advanced)
                    {
                        if (!pSGMI(hProcess, stackFrame.AddrPC.Offset, &Module))
                        {
                            fprintf(fLogFile, "%lu: SymGetModuleInfo): GetLastError = %lu\n", g_dwShowCount,
                                    ::GetLastError());
                        }
                        else
                        {
                            switch (Module.SymType)
                            {
                            case SymNone: strcpy(ty, "-nosymbols-"); break;
                            case SymCoff: strcpy(ty, "COFF"); break;
                            case SymCv: strcpy(ty, "CV"); break;
                            case SymPdb: strcpy(ty, "PDB"); break;
                            case SymExport: strcpy(ty, "-exported-"); break;
                            case SymDeferred: strcpy(ty, "-deferred-"); break;
                            case SymSym: strcpy(ty, "SYM"); break;
                            default: _snprintf(ty, sizeof(ty), "symtype=%ld", static_cast<long>(Module.SymType)); break;
                            }
                            fprintf(fLogFile, "%lu:     Mod:  %s, base: %08lxh\n", g_dwShowCount, Module.ModuleName,
                                    Module.BaseOfImage);
                            if (Module.SymType == SymNone)
                            {
                                fprintf(fLogFile, "%lu:     Offset: 0x%8.8x\n", g_dwShowCount,
                                        stackFrame.AddrPC.Offset);
                                fprintf(fLogFile, "%lu:     Sym:  type: %s, file: %s\n", g_dwShowCount, ty,
                                        Module.LoadedImageName);
                            }
                        }
                    }
                }

                if (stackFrame.AddrReturn.Offset == 0)
                {
                    ::SetLastError(0);
                    break;
                }
                if (++frameNum >= MAX_FRAMES)
                {
                    break;
                }
            }
            if (::GetLastError())
            {
                fprintf(fLogFile, "\n%lu: StackWalk(): GetLastError = %lu\n", g_dwShowCount, ::GetLastError());
            }
            if (frameNum >= MAX_FRAMES)
            {
                fprintf(fLogFile, "\nToo much frames: %d\n", frameNum);
            }
        }

        if (fLogFile)
        {
            fprintf(fLogFile, "\n\n");
            if (g_dwShowCount % 1000)
            {
                fflush(fLogFile);
            }
        }
        ::LeaveCriticalSection(&g_csFileOpenClose);
    }

    void __stdcall ShowStack(CONTEXT const& c, FILE* fLogFile)
    {
        // RVA 0x75BE30 - walks the current thread's stack.
        HANDLE hThread;
        // 2 = DUPLICATE_SAME_ACCESS
        ::DuplicateHandle(::GetCurrentProcess(), ::GetCurrentThread(), ::GetCurrentProcess(), &hThread, 0, FALSE, 2);
        ShowStackRM(hThread, c, fLogFile, nullptr, ::GetCurrentProcess());
        ::CloseHandle(hThread);
    }
}

unsigned long __fastcall StackwalkFilter(_EXCEPTION_POINTERS const* ep, unsigned long status, char const* pszLogFile,
                                         unsigned long lastErrorFromMiniDump)
{
    // RVA 0x75BE80
    FILE* fFile = stdout;
    if (pszLogFile)
    {
        CStr tmpName(pszLogFile);
        UnifyFileName(tmpName);
        fFile = fopen(tmpName.c_str(), "at");
    }
    if (!fFile)
    {
        fFile = stdout;
    }
    fprintf(fFile, "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n");
    fprintf(fFile, "-==\n");
    fprintf(fFile, "-== Log category   : Unhandled exception info\n");
    fprintf(fFile, "-== Build          : %s\n", "ExMachina - release version release build v1.02 (Dec 21 2005 12:15:04)");
    fprintf(fFile, "-==\n");
    if (lastErrorFromMiniDump)
    {
        fprintf(fFile, "-== Minidump could not be written, GetLastError = %lu\n", lastErrorFromMiniDump);
    }
    fprintf(fFile, "-== Exception      : 0x%8.8X at address 0x%8.8X\n", ep->ExceptionRecord->ExceptionCode,
            reinterpret_cast<DWORD>(ep->ExceptionRecord->ExceptionAddress));

    char szTemp[100];
    char const* details = "";
    EXCEPTION_RECORD const* const record = ep->ExceptionRecord;
    if (record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && record->NumberParameters == 2)
    {
        if (record->ExceptionInformation[0] == 0)
        {
            sprintf(szTemp, " read attempt to address 0x%8.8X ", record->ExceptionInformation[1]);
            details = szTemp;
        }
        else if (record->ExceptionInformation[0] == 1)
        {
            sprintf(szTemp, " write attempt to address 0x%8.8X ", record->ExceptionInformation[1]);
            details = szTemp;
        }
    }
    fprintf(fFile, "-== Exception type : %s %s\n", GetExpectionCodeText(ep->ExceptionRecord->ExceptionCode), details);
    fprintf(fFile, "-==\n");
    fprintf(fFile, "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n\n");
    ShowStack(*ep->ContextRecord, fFile);
    fprintf(fFile, "Registers:\n");
    fprintf(fFile, "\tCS:EIP 0x%4.4X:%8.8X\n", ep->ContextRecord->SegCs, ep->ContextRecord->Eip);
    fprintf(fFile, "\tSS:ESP 0x%4.4X:%8.8X\n", ep->ContextRecord->SegSs, ep->ContextRecord->Esp);
    fprintf(fFile, "\tDS:EDI 0x%4.4X:%8.8X\n", ep->ContextRecord->SegDs, ep->ContextRecord->Edi);
    fprintf(fFile, "\tES:ESI 0x%4.4X:%8.8X\n", ep->ContextRecord->SegEs, ep->ContextRecord->Esi);
    fprintf(fFile, "\t   EAX        %8.8X\n", ep->ContextRecord->Eax);
    fprintf(fFile, "\t   EBX        %8.8X\n", ep->ContextRecord->Ebx);
    fprintf(fFile, "\t   ECX        %8.8X\n", ep->ContextRecord->Ecx);
    fprintf(fFile, "\t   EDX        %8.8X\n", ep->ContextRecord->Edx);
    fprintf(fFile, "\n");
    // NOTE: stdout is closed too when the log file could not be opened.
    fclose(fFile);
    return status;
}

LONG WINAPI CrashHandlerExceptionFilter(PEXCEPTION_POINTERS pExPtrs)
{
    // RVA 0x75C1A0 - writes a minidump, the stack log and a copy of the game log, then reports the exception.
    // NOTE: the files go to the names InitAllocCheck built, not to the "exceptions" directory created here relative
    // to the current directory, which is read and not used.
    char tmp_1[0x400];
    ::GetCurrentDirectoryA(sizeof(tmp_1), tmp_1);
    if (!::CreateDirectoryA("exceptions", nullptr))
    {
        ::GetLastError();
    }
    unsigned long lastError = 0;
    WriteDump(s_szDumpFileName, pExPtrs, lastError);
    // 1 = EXCEPTION_EXECUTE_HANDLER
    LONG const status = StackwalkFilter(pExPtrs, 1, s_szExceptionLogFileName, lastError);
    ::CopyFileA("ExMachina.log", s_szGameLogFileName, FALSE);
    char buffer[512];
    sprintf(buffer, "*** Unhandled Exception!\n   ExpCode: 0x%8.8X\n   ExpFlags: %d\n   ExpAddress: 0x%8.8X\n   Please report!",
            pExPtrs->ExceptionRecord->ExceptionCode, pExPtrs->ExceptionRecord->ExceptionFlags,
            reinterpret_cast<DWORD>(pExPtrs->ExceptionRecord->ExceptionAddress));
    // MB_ICONHAND | MB_TASKMODAL
    m3d::g_Kernel->MessageBoxA(nullptr, buffer, "Application Error", 0x2010);
    return status;
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
    // RVA 0x759B90
    if (g_bInitialized)
    {
        return 0;
    }
    g_hImagehlpDll = ::LoadLibraryA("dbghelp.dll");
    if (!g_hImagehlpDll)
    {
        printf("LoadLibrary( \"dbghelp.dll\" ): GetLastError = %lu\n", ::GetLastError());
        g_bInitialized = 0;
        return 1;
    }
    pSC = reinterpret_cast<SymCleanupType>(::GetProcAddress(g_hImagehlpDll, "SymCleanup"));
    pSFTA = reinterpret_cast<SymFunctionTableAccessType>(::GetProcAddress(g_hImagehlpDll, "SymFunctionTableAccess"));
    pSGLFA = reinterpret_cast<SymGetLineFromAddrType>(::GetProcAddress(g_hImagehlpDll, "SymGetLineFromAddr"));
    pSGMB = reinterpret_cast<SymGetModuleBaseType>(::GetProcAddress(g_hImagehlpDll, "SymGetModuleBase"));
    pSGMI = reinterpret_cast<SymGetModuleInfoType>(::GetProcAddress(g_hImagehlpDll, "SymGetModuleInfo"));
    pSGO = reinterpret_cast<SymGetOptionsType>(::GetProcAddress(g_hImagehlpDll, "SymGetOptions"));
    pSGSFA = reinterpret_cast<SymGetSymFromAddrType>(::GetProcAddress(g_hImagehlpDll, "SymGetSymFromAddr"));
    pSI = reinterpret_cast<SymInitializeType>(::GetProcAddress(g_hImagehlpDll, "SymInitialize"));
    pSSO = reinterpret_cast<SymSetOptionsType>(::GetProcAddress(g_hImagehlpDll, "SymSetOptions"));
    pSW = reinterpret_cast<StackWalkType>(::GetProcAddress(g_hImagehlpDll, "StackWalk"));
    pUDSN = reinterpret_cast<UnDecorateSymbolNameType>(::GetProcAddress(g_hImagehlpDll, "UnDecorateSymbolName"));
    pSLM = reinterpret_cast<SymLoadModuleType>(::GetProcAddress(g_hImagehlpDll, "SymLoadModule"));
    pMDWD = reinterpret_cast<MiniDumpWriteDumpType>(::GetProcAddress(g_hImagehlpDll, "MiniDumpWriteDump"));
    if (pSC && pSFTA && pSGMB && pSGMI && pSGO && pSGSFA && pSI && pSSO && pSW && pUDSN && pSLM && pMDWD)
    {
        g_bInitialized = 1;
        ::InitializeCriticalSection(&g_csFileOpenClose);
        return 0;
    }
    printf("GetProcAddress(): some required function not found.\n");
    ::FreeLibrary(g_hImagehlpDll);
    g_bInitialized = 0;
    return 1;
}

int InitAllocCheck(eAllocCheckOutput eOutput, int bSetUnhandledExeptionFilter, unsigned long ulShowStackAtAlloc)
{
    // RVA 0x75C270 - picks the first free "exceptions/<binary>NNNN" names in the working directory for the crash
    // log, dump and game log copy.
    char const* const workDir = m3d::g_Kernel->GetFileServer().GetCurrentWorkDir();
    char szModName[MAX_PATH];
    if (::GetModuleFileNameA(nullptr, szModName, MAX_PATH))
    {
        strcpy(s_szExceptionLogFileName, szModName);
    }
    else
    {
        strcpy(szModName, "\\error");
    }
    CStr const dirName = DirectoryFromFileName(CStr(workDir));
    CStr const fileName = NameFromFileName(CStr(s_szExceptionLogFileName));
    g_currentBinaryName = fileName;
    g_currentBinaryName.toLower();

    char szName[1024];
    unsigned counter = 0;
    do
    {
        sprintf(szName, "%s/exceptions/%s%04u.log", dirName.c_str(), fileName.c_str(), counter);
        sprintf(s_szDumpFileName, "%s/exceptions/%s%04u.dmp", dirName.c_str(), fileName.c_str(), counter);
        sprintf(s_szGameLogFileName, "%s/exceptions/%s%04u_game.log", dirName.c_str(), fileName.c_str(), counter++);
    } while (::GetFileAttributesA(szName) != INVALID_FILE_ATTRIBUTES);
    strcpy(s_szExceptionLogFileName, szName);
    M3D_LOG_INFO(CStr("Stackwalker: log file name will be '") + CStr(s_szExceptionLogFileName) + CStr("'"));
    M3D_LOG_INFO(CStr("Stackwalker: dump file name will be '") + CStr(s_szDumpFileName) + CStr("'"));
    M3D_LOG_INFO(CStr("Stackwalker: game log file name will be '") + CStr(s_szGameLogFileName) + CStr("'"));
    if (bSetUnhandledExeptionFilter)
    {
        ::SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);
        s_bUnhandledExeptionFilterSet = 1;
    }
    if (g_bInitialized)
    {
        return 2;
    }
    g_pszAllocLogName = _strdup(szModName);
    g_CallstackOutputType = eOutput;
    return InitStackWalk();
}

#pragma once

enum eAllocCheckOutput
{
    ACOutput_Simple = 0x0,
    ACOutput_Advanced = 0x1,
};

int InitAllocCheck(eAllocCheckOutput, int, unsigned long);

#include <windows.h>

// Writes a minidump with MiniDumpWriteDump; does nothing when dbghelp's function was not resolved.
void __fastcall WriteDump(char const* szFileName, _EXCEPTION_POINTERS* pExcP, unsigned long& lastError);
unsigned int __fastcall DeInitAllocCheck();

#include <core/stringm3d.h>
extern CStr g_currentBinaryName;

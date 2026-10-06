#pragma once
#include <core/stringm3d.h>

namespace m3d
{
    class Kernel;
}

// The kernel the executable handed to createIInput. The driver reads the engine configuration and
// the timer through it and asserts through its SysError.
extern m3d::Kernel* g_kernel;

// Writes a line to the engine log through the callback the executable passes to IInput::Init
// (the executable prefixes it with "input: "). Nothing is written before Init.
void LogMsg(CStr const& msg);
void LogMsg(char const* msg);
void SetLogFunc(void(__fastcall* logFunc)(CStr const&));

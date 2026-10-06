#pragma once
#include <core/stringm3d.h>

namespace m3d
{
    class Kernel;
}

// The kernel the executable handed to createIRenderer. The driver reads the engine configuration,
// the file server and the memory routines through it.
extern m3d::Kernel* g_kernel;

// Writes a line to the engine log through the callback the executable passed to
// IRenderer::Create. Nothing is written before Create.
void LogMsg(CStr const& msg);
void LogMsg(char const* msg);
void SetLogFunc(void(__fastcall* logFunc)(CStr const&));

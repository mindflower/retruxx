// The driver's entry points: the createIInput export the executable looks up with GetProcAddress,
// the log callback and the memory routines shared with the executable.
#include <Windows.h>
#include <cstdlib>
#include <new>

#include <core/kernel.h>
#include <core/stringm3d.h>
#include <iface.h>

#include "log.h"

m3d::Kernel* g_kernel = nullptr;

// The engine sources compiled into the DLL (core/timer.cpp) reach the kernel through the engine's
// own global; it is the DLL's copy of that pointer, set together with g_kernel.
namespace m3d
{
    Kernel* g_Kernel = nullptr;

    // orig 0x570b40 input_di8.cpp:1829
    input::IInput* InputFactory();
}

namespace
{
    void(__fastcall* g_logFunc)(CStr const&) = nullptr;
}

void LogMsg(CStr const& msg)
{
    if (g_logFunc)
    {
        g_logFunc(msg);
    }
}

void LogMsg(char const* msg)
{
    LogMsg(CStr(msg));
}

void SetLogFunc(void(__fastcall* logFunc)(CStr const&))
{
    g_logFunc = logFunc;
}

// Everything the driver allocates goes through the kernel's memory routines, like the executable's
// own operator new does, so objects created on one side can be freed on the other. Until
// createIInput has handed over the kernel the CRT heap is used; nothing is allocated before that.
void* __cdecl operator new(std::size_t count)
{
    if (g_kernel)
    {
        return g_kernel->g_mar.AllocMem(static_cast<unsigned int>(count), nullptr, 0);
    }
    return std::malloc(count);
}

void* __cdecl operator new[](std::size_t count)
{
    return operator new(count);
}

void* __cdecl operator new(std::size_t count, std::nothrow_t const&) noexcept
{
    return operator new(count);
}

void* __cdecl operator new[](std::size_t count, std::nothrow_t const&) noexcept
{
    return operator new(count);
}

void __cdecl operator delete(void* p) noexcept
{
    if (!p)
    {
        return;
    }
    if (g_kernel)
    {
        g_kernel->g_mar.FreeMem(p, nullptr, 0);
        return;
    }
    std::free(p);
}

void __cdecl operator delete[](void* p) noexcept
{
    operator delete(p);
}

void __cdecl operator delete(void* p, std::size_t) noexcept
{
    operator delete(p);
}

void __cdecl operator delete[](void* p, std::size_t) noexcept
{
    operator delete(p);
}

// Application::createInput: m_input = createIInput(g_Kernel), followed by m_input->IncRef() and
// m_input->Init(g_Kernel, logInputFunc). Hard Truck Apocalypse's input_di8.dll stores the kernel
// here and constructs the driver through the same path as the original's InputFactory.
extern "C" __declspec(dllexport) m3d::input::IInput* createIInput(m3d::Kernel* kernel)
{
    g_kernel = kernel;
    m3d::g_Kernel = kernel;
    return m3d::InputFactory();
}

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID)
{
    return TRUE;
}

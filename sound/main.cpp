// The driver's entry points: the createISound export the executable looks up with GetProcAddress
// and the memory routines shared with the executable.
#include <Windows.h>
#include <cstdlib>
#include <new>

#include <core/kernel.h>
#include <iface.h>

m3d::Kernel* g_kernel = nullptr;

// The engine sources compiled into the DLL (the ini.cpp helpers) reach the kernel through the
// engine's own global; it is the DLL's copy of that pointer, set together with g_kernel.
namespace m3d
{
    Kernel* g_Kernel = nullptr;

    // orig 0x5f69d0 CM3DSoundManager.cpp:2035
    snd::ISound* SoundFactory();
}

// Everything the driver allocates goes through the kernel's memory routines, like the executable's
// own operator new does, so objects created on one side can be freed on the other. Until
// createISound has handed over the kernel the CRT heap is used, so no static object of the DLL may
// allocate in its constructor: it would be freed through the kernel when the executable unloads
// the DLL, and the kernel's allocator rejects a block it did not hand out.
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

// Application::createSound: m_sound = createISound(g_Kernel), retried three times, followed by
// m_sound->IncRef() and m_sound->Init(logSoundFunc, sampleRate, bitsPerSample, maxSounds,
// pathToSoundGroups). Hard Truck Apocalypse's sound.dll constructs the manager the same way as the
// original's SoundFactory.
extern "C" __declspec(dllexport) snd::ISound* createISound(m3d::Kernel* kernel)
{
    g_kernel = kernel;
    m3d::g_Kernel = kernel;
    return m3d::SoundFactory();
}

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID)
{
    return TRUE;
}

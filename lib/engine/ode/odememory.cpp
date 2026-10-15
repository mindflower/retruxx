#include <ode/memory.h>
#include <ode/odememory.h>
#include <cstring>
#include <core/kernel.h>

// ODE's allocations go through kernel allocator.
void* OdeMemoryAlloc(size_t Size)
{
    return M3D_KERNEL->g_mar.AllocMem(Size, nullptr, 0);
}

void* OdeMemoryRealloc(void* Block, size_t, size_t NewSize)
{
    return M3D_KERNEL->g_mar.ReallocMem(Block, NewSize, nullptr, 0);
}

void OdeMemoryFree(void* Block, size_t)
{
    M3D_KERNEL->g_mar.FreeMem(Block, 0, 0);
}

void OdeSetMemoryHandlers()
{
    dSetAllocHandler(OdeMemoryAlloc);
    dSetReallocHandler(OdeMemoryRealloc);
    dSetFreeHandler(OdeMemoryFree);
}

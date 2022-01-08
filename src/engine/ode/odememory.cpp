#include <ode/memory.h>
#include <ode/odememory.h>
#include <core/kernel.h>

void* OdeMemoryAlloc(unsigned int Size)
{
    return m3d::g_Kernel->g_mar.AllocMem(Size, 0, 0);
}

void* OdeMemoryRealloc(void* Block, unsigned int OldSize, unsigned int NewSize)
{
    return m3d::g_Kernel->g_mar.ReallocMem(Block, NewSize, 0, 0);
}

void OdeMemoryFree(void* Block, unsigned int Size)
{
    m3d::g_Kernel->g_mar.FreeMem(Block, 0, 0);
}

void OdeSetMemoryHandlers()
{
    dSetAllocHandler(OdeMemoryAlloc);
    dSetReallocHandler(OdeMemoryRealloc);
    dSetFreeHandler(OdeMemoryFree);
}

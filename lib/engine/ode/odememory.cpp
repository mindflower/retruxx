#include <ode/memory.h>
#include <ode/odememory.h>
#include <cstring>

// ODE's allocations go through plain new[]/delete[] (the engine routed them through its kernel allocator).
void* OdeMemoryAlloc(size_t Size)
{
    return new unsigned char[Size];
}

void* OdeMemoryRealloc(void* Block, size_t OldSize, size_t NewSize)
{
    unsigned char* const newBlock = new unsigned char[NewSize];
    if (Block)
    {
        std::memcpy(newBlock, Block, OldSize < NewSize ? OldSize : NewSize);
        delete[] static_cast<unsigned char*>(Block);
    }
    return newBlock;
}

void OdeMemoryFree(void* Block, size_t Size)
{
    delete[] static_cast<unsigned char*>(Block);
}

void OdeSetMemoryHandlers()
{
    dSetAllocHandler(OdeMemoryAlloc);
    dSetReallocHandler(OdeMemoryRealloc);
    dSetFreeHandler(OdeMemoryFree);
}

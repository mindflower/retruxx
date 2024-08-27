#include "memoryman.h"
#include <stdexcept>

namespace m3d
{
    int MemoryManager::debugMemLastUnsuccessfulAllocSize() const
    {
        throw std::logic_error("Not implemented");
    }

    MemoryManager::MemoryManager()
    {
        //TODO: implement memeory manager initialization
    }

    void* MemoryManager::Realloc(void* p, int newSize, char const* src_name, int src_line)
    {
        AutoLock lock(m_cs);
        return realloc(p, newSize);
    }

    void MemoryManager::Free(void* p)
    {
        AutoLock lock(m_cs);
        free(p);
    }

    MemoryManager::~MemoryManager()
    {
        //TODO: implement memeory manager deinitialization
    }

    unsigned MemoryManager::debugMemUsed() const
    {
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::CheckMemory()
    {
        throw std::logic_error("Not implemented");
    }

    auxBlockHeader* MemoryManager::NewChunk(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned MemoryManager::debugMemAllocated() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned MemoryManager::debugMemOverhead() const
    {
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::DumpMemory(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void* MemoryManager::Malloc(int size, char const* src_name, int src_line)
    {
        AutoLock lock(m_cs);
        return malloc(size);
    }

    void MemoryManager::turnAggressiveDebugMode(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::operator delete(void* d)
    {
        free(d);
    }

    void* MemoryManager::operator new(size_t sz)
    {
        return malloc(sz);
    }

    void MemoryManager::DumpMemoryFootprint(bool) const
    {
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::FreeChunk(auxChunkHeader*)
    {
        throw std::logic_error("Not implemented");
    }
}

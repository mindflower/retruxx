#include "memoryman.h"
#include "thirdparty/injecttools.h"
#include <stdexcept>

namespace m3d
{
    int MemoryManager::debugMemLastUnsuccessfulAllocSize() const
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00589400, MemoryManager::debugMemLastUnsuccessfulAllocSize);
        throw std::logic_error("Not implemented");
    }

    MemoryManager::MemoryManager()
    {
        (this->*inject::cast<void(MemoryManager::*)()>(0x007487A0))();
        return;

        //TODO: implement memeory manager initialization
        this->m_memAllocated = 0;
        this->m_memUsed = 0;
        this->m_memNumAlloc = 1;
        this->m_numNewChunks = 0;
        this->m_memOverhead = 0;
        this->m_memNumAllocToBreakIn = 0;
        this->m_lastUnsuccessfulAllocationSize = 0;
        this->m_chunks = 0;

        auto v2 = 0;
        auto mm_b_num = m_b_num;
        do
        {
            auto v4 = 1 << (v2 + 4);
            *(mm_b_num - 7) = v4;
            *mm_b_num = 0x4000 / v4;
            auto v5 = NewChunk(v4, 0x4000 / v4);
            *(mm_b_num - 14) = reinterpret_cast<int>(v5);
            if (!v5)
                __debugbreak();
            ++v2;
            ++mm_b_num;
        } while (v2 < 7);
    }

    void* MemoryManager::Realloc(void* p, int newSize, char const* src_name, int src_line)
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748C10, MemoryManager::Realloc, p, newSize, src_name, src_line);
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::Free(void* p)
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748B50, MemoryManager::Free, p);
        throw std::logic_error("Not implemented");
    }

    MemoryManager::~MemoryManager()
    {
        //TODO: implement memeory manager deinitialization
        throw std::logic_error("Not implemented");
    }

    unsigned MemoryManager::debugMemUsed() const
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x005893D0, MemoryManager::debugMemUsed);
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::CheckMemory()
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748620, MemoryManager::CheckMemory);
        throw std::logic_error("Not implemented");
    }

    auxBlockHeader* MemoryManager::NewChunk(int bsize, int bnum)
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748670, MemoryManager::NewChunk, bsize, bnum);
        throw std::logic_error("Not implemented");
    }

    unsigned MemoryManager::debugMemAllocated() const
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x005893E0, MemoryManager::debugMemAllocated);
        throw std::logic_error("Not implemented");
    }

    unsigned MemoryManager::debugMemOverhead() const
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x005893F0, MemoryManager::debugMemOverhead);
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::DumpMemory(char const* filename)
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748780, MemoryManager::DumpMemory, filename);
        throw std::logic_error("Not implemented");
    }

    void* MemoryManager::Malloc(int size, char const* src_name, int src_line)
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748A30, MemoryManager::Malloc, size, src_name, src_line);
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::turnAggressiveDebugMode(bool bOn)
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748610, MemoryManager::turnAggressiveDebugMode, bOn);
        throw std::logic_error("Not implemented");
    }

    void __fastcall MemoryManager::operator delete(void* d)
    {
        free(d);
    }

    void* __fastcall MemoryManager::operator new(unsigned int sz)
    {
        return malloc(sz);
    }

    void MemoryManager::DumpMemoryFootprint(bool bDetailed) const
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748790, MemoryManager::DumpMemoryFootprint, bDetailed);
        throw std::logic_error("Not implemented");
    }

    void MemoryManager::FreeChunk(auxChunkHeader* ch)
    {
        RETUXX_DLL_JMP_TO_FUNCTION(0x00748710, MemoryManager::FreeChunk, ch);
        throw std::logic_error("Not implemented");
    }
}

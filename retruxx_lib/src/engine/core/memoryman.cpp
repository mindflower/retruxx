#include "memoryman.h"
#include "thirdparty/injecttools.h"
#include <stdexcept>

#include "thirdparty/containers.h"

namespace m3d
{
    int MemoryManager::debugMemLastUnsuccessfulAllocSize() const
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00589400, MemoryManager::debugMemLastUnsuccessfulAllocSize);
        RETRUXX_NOT_IMPLEMENTED;
    }

    MemoryManager::MemoryManager()
    {
        //(this->*inject::cast<void(MemoryManager::*)()>(0x007487A0))();
        //return;

        //TODO: implement memeory manager initialization
        //this->m_memAllocated = 0;
        //this->m_memUsed = 0;
        //this->m_memNumAlloc = 1;
        //this->m_numNewChunks = 0;
        //this->m_memOverhead = 0;
        //this->m_memNumAllocToBreakIn = 0;
        //this->m_lastUnsuccessfulAllocationSize = 0;
        //this->m_chunks = 0;
        //
        //auto v2 = 0;
        //auto mm_b_num = m_b_num;
        //do
        //{
        //    auto v4 = 1 << (v2 + 4);
        //    *(mm_b_num - 7) = v4;
        //    *mm_b_num = 0x4000 / v4;
        //    auto v5 = NewChunk(v4, 0x4000 / v4);
        //    *(mm_b_num - 14) = reinterpret_cast<int>(v5);
        //    if (!v5)
        //        __debugbreak();
        //    ++v2;
        //    ++mm_b_num;
        //} while (v2 < 7);
    }

    void* MemoryManager::Realloc(void* p, int newSize, char const* src_name, int src_line)
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748C10, MemoryManager::Realloc, p, newSize, src_name, src_line);
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MemoryManager::Free(void* p)
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748B50, MemoryManager::Free, p);
        RETRUXX_NOT_IMPLEMENTED;
    }

    MemoryManager::~MemoryManager()
    {
        //TODO: implement memeory manager deinitialization
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned MemoryManager::debugMemUsed() const
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x005893D0, MemoryManager::debugMemUsed);
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MemoryManager::CheckMemory()
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748620, MemoryManager::CheckMemory);
        RETRUXX_NOT_IMPLEMENTED;
    }

    auxBlockHeader* MemoryManager::NewChunk(int bsize, int bnum)
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748670, MemoryManager::NewChunk, bsize, bnum);
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned MemoryManager::debugMemAllocated() const
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x005893E0, MemoryManager::debugMemAllocated);
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned MemoryManager::debugMemOverhead() const
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x005893F0, MemoryManager::debugMemOverhead);
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MemoryManager::DumpMemory(char const* filename)
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748780, MemoryManager::DumpMemory, filename);
        RETRUXX_NOT_IMPLEMENTED;
    }

    void* MemoryManager::Malloc(int size, char const* src_name, int src_line)
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748A30, MemoryManager::Malloc, size, src_name, src_line);
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MemoryManager::turnAggressiveDebugMode(bool bOn)
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748610, MemoryManager::turnAggressiveDebugMode, bOn);
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748790, MemoryManager::DumpMemoryFootprint, bDetailed);
        RETRUXX_NOT_IMPLEMENTED;
    }

    void MemoryManager::FreeChunk(auxChunkHeader* ch)
    {
        RETRUXX_DLL_JMP_TO_FUNCTION(0x00748710, MemoryManager::FreeChunk, ch);
        RETRUXX_NOT_IMPLEMENTED;
    }
}

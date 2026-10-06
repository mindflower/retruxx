#pragma once
#include <cstddef>

#include <core/threadsync.h>

namespace m3d
{
    struct auxChunkHeader
    {
        auxChunkHeader* m_nextChunk;
        auxChunkHeader* m_prevChunk;
        std::size_t     m_bnum;
        std::size_t     m_bsize;
    };

    struct auxBlockHeader
    {
        auxBlockHeader* m_nextBlock;
        std::size_t     m_size;
        int             m_magic;
    };

    class MemoryManager
    {
    public:
        /* 0x0000 */ m3d::auxChunkHeader* m_firstChunk = nullptr;
        /* 0x0004 */ m3d::auxChunkHeader* m_chunks = nullptr;
        /* 0x0008 */ m3d::auxBlockHeader* m_blocks[7];
        /* 0x0024 */ std::size_t m_b_size[7];
        /* 0x0040 */ std::size_t m_b_num[7];
        MemoryManager();
        ~MemoryManager();
        static void* operator new(std::size_t sz);
        static void operator delete(void* d);
        m3d::auxBlockHeader* NewChunk(std::size_t bsize, int bnum);
        void FreeChunk(m3d::auxChunkHeader* ch);
        void* Malloc(size_t size, const char* src_name, int src_line);
        void* Realloc(void* p, size_t newSize, const char* src_name, int src_line);
        void Free(void* p);
        void DumpMemory(const char* filename);
        void DumpMemoryFootprint(bool bDetailed) const;
        std::size_t debugMemUsed() const;
        std::size_t debugMemAllocated() const;
        std::size_t debugMemOverhead() const;
        std::size_t debugMemLastUnsuccessfulAllocSize() const;
        void turnAggressiveDebugMode(bool bOn);
        void CheckMemory();

    private:
        /* 0x005c */ m3d::CriticalSection m_cs;
        /* 0x0074 */ std::size_t m_memAllocated = 0;
        /* 0x0078 */ std::size_t m_memUsed = 0;
        /* 0x007c */ std::size_t m_numNewChunks = 0;
        /* 0x0080 */ std::size_t m_memNumAlloc = 0;
        /* 0x0084 */ std::size_t m_memNumAllocToBreakIn = 0;
        /* 0x0088 */ std::size_t m_memOverhead = 0;
        /* 0x008c */ std::size_t m_lastUnsuccessfulAllocationSize = 0;
    }; /* size: 0x0090 */
}

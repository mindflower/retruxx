#pragma once
#include <core/threadsync.h>

namespace m3d
{
    struct auxChunkHeader
    {
        auxChunkHeader* m_nextChunk;
        auxChunkHeader* m_prevChunk;
        int m_bnum;
        int m_bsize;
    };

    struct auxBlockHeader
    {
        auxBlockHeader* m_nextBlock;
        int m_size;
        int m_magic;
    };

    class MemoryManager
    {
        //enum __unnamed;

    public:
        /* 0x0000 */ m3d::auxChunkHeader* m_firstChunk;
        /* 0x0004 */ m3d::auxChunkHeader* m_chunks;
        /* 0x0008 */ m3d::auxBlockHeader* m_blocks[7];
        /* 0x0024 */ int m_b_size[7];
        /* 0x0040 */ int m_b_num[7];
        MemoryManager();
        ~MemoryManager();
        static void* __fastcall operator new(unsigned int sz);
        static void __fastcall operator delete(void* d);
        m3d::auxBlockHeader* NewChunk(int bsize, int bnum);
        void FreeChunk(m3d::auxChunkHeader* ch);
        void* Malloc(int size, const char* src_name, int src_line);
        void* Realloc(void* p, int newSize, const char* src_name, int src_line);
        void Free(void* p);
        void DumpMemory(const char* filename);
        void DumpMemoryFootprint(bool bDetailed) const;
        unsigned int debugMemUsed() const;
        unsigned int debugMemAllocated() const;
        unsigned int debugMemOverhead() const;
        int debugMemLastUnsuccessfulAllocSize() const;
        void turnAggressiveDebugMode(bool bOn);
        void CheckMemory();

    private:
        /* 0x005c */ m3d::CriticalSection m_cs;
        /* 0x0074 */ unsigned int m_memAllocated;
        /* 0x0078 */ unsigned int m_memUsed;
        /* 0x007c */ unsigned int m_numNewChunks;
        /* 0x0080 */ unsigned int m_memNumAlloc;
        /* 0x0084 */ unsigned int m_memNumAllocToBreakIn;
        /* 0x0088 */ unsigned int m_memOverhead;
        /* 0x008c */ int m_lastUnsuccessfulAllocationSize;
    }; /* size: 0x0090 */

    static_assert(sizeof(MemoryManager) == 0x0090);
}

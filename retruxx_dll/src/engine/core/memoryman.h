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
    public:
        int debugMemLastUnsuccessfulAllocSize() const ;
        MemoryManager();
        void * Realloc(void *,int,char const *,int);
        void Free(void *);
        ~MemoryManager();
        unsigned int debugMemUsed() const ;
        void CheckMemory();
        auxBlockHeader * NewChunk(int,int);
        unsigned int debugMemAllocated() const ;
        unsigned int debugMemOverhead() const ;
        void DumpMemory(char const *);
        void * Malloc(int,char const *,int);
        void turnAggressiveDebugMode(bool);
        static void __fastcall operator delete(void *);
        static void * __fastcall operator new(size_t);
        void DumpMemoryFootprint(bool) const ;
        void FreeChunk(auxChunkHeader *);

    private:
        auxChunkHeader* m_firstChunk;
        auxChunkHeader* m_chunks = nullptr;
        auxBlockHeader* m_blocks[7];
        int m_b_size[7];
        int m_b_num[7];
        CriticalSection m_cs;
        unsigned int m_memAllocated = 0;
        unsigned int m_memUsed = 0;
        unsigned int m_numNewChunks = 0;
        unsigned int m_memNumAlloc = 1;
        unsigned int m_memNumAllocToBreakIn = 0;
        unsigned int m_memOverhead = 0;
        int m_lastUnsuccessfulAllocationSize = 0;
    };
}

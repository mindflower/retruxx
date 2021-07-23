#pragma once
#include <core/threadsync.h>

namespace m3d
{
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
        static void * __fastcall operator new(unsigned int);
        void DumpMemoryFootprint(bool) const ;
        void FreeChunk(auxChunkHeader *);

    private:
        operator new(uint);
        auxChunkHeader *m_firstChunk;
        auxChunkHeader *m_chunks;
        auxBlockHeader *m_blocks[7];
        int m_b_size[7];
        int m_b_num[7];
        CriticalSection m_cs;
        unsigned int m_memAllocated;
        unsigned int m_memUsed;
        unsigned int m_numNewChunks;
        unsigned int m_memNumAlloc;
        unsigned int m_memNumAllocToBreakIn;
        unsigned int m_memOverhead;
        int m_lastUnsuccessfulAllocationSize;
    };
}

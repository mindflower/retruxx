#include "memoryman.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <stdexcept>

#include "retruxx/common.h"

// NOTE: the port's global allocator (g_mar.AllocMem, see kernel.cpp) goes straight to malloc and never reaches this
// pool, so the pool only holds the chunks its constructor pre-allocates and the leak report written on shutdown is
// empty.

namespace
{
    // Block header magic values: live blocks carry kMagic in m_magic and kTrailer after their data, freed ones
    // carry the freed pair.
    constexpr int kMagic = static_cast<int>(0xDEADBEEF);
    constexpr int kTrailer = static_cast<int>(0xFEEBDAED);
    constexpr int kFreedMagic = 0x21524110;
    constexpr int kFreedTrailer = 0x01142512;

    // m_nextBlock of a block that has its own chunk (a big allocation).
    m3d::auxBlockHeader* const kOwnChunk = reinterpret_cast<m3d::auxBlockHeader*>(-1);

    int& Trailer(m3d::auxBlockHeader* block)
    {
        return *reinterpret_cast<int*>(reinterpret_cast<char*>(block + 1) + block->m_size);
    }
}

namespace m3d
{
    int MemoryManager::debugMemLastUnsuccessfulAllocSize() const
    {
        // RVA 0x589400
        return m_lastUnsuccessfulAllocationSize;
    }

    MemoryManager::MemoryManager()
    {
        // RVA 0x7487A0 - the critical section is initialized by the m_cs member.
        m_memAllocated = 0;
        m_memUsed = 0;
        m_memNumAlloc = 1;
        m_numNewChunks = 0;
        m_memOverhead = 0;
        m_memNumAllocToBreakIn = 0;
        m_lastUnsuccessfulAllocationSize = 0;
        m_chunks = nullptr;
        for (int i = 0; i < 7; ++i)
        {
            int const size = 1 << (i + 4);
            m_b_size[i] = size;
            m_b_num[i] = 0x4000 / size;
            m_blocks[i] = NewChunk(size, 0x4000 / size);
            if (!m_blocks[i])
            {
                __debugbreak();
            }
        }
    }

    void* MemoryManager::Realloc(void* p, int newSize, char const* src_name, int src_line)
    {
        // RVA 0x748C10
        AutoLock lock(m_cs);
        if (!p)
        {
            return newSize ? Malloc(newSize, src_name, src_line) : nullptr;
        }
        if (!newSize)
        {
            Free(p);
            return nullptr;
        }
        void* const n = Malloc(newSize, src_name, src_line);
        if (n)
        {
            int const oldSize = (static_cast<auxBlockHeader*>(p) - 1)->m_size;
            memcpy(n, p, newSize > oldSize ? oldSize : newSize);
            Free(p);
        }
        return n;
    }

    void MemoryManager::Free(void* p)
    {
        // RVA 0x748B50
        AutoLock lock(m_cs);
        auxBlockHeader* const block = static_cast<auxBlockHeader*>(p) - 1;
        if (block->m_magic != kMagic)
        {
            __debugbreak();
        }
        int& trailer = Trailer(block);
        if (trailer != kTrailer)
        {
            __debugbreak();
        }
        block->m_magic = kFreedMagic;
        trailer = kFreedTrailer;
        m_memUsed -= block->m_size;
        if (block->m_nextBlock == kOwnChunk)
        {
            auxChunkHeader* const chunk = reinterpret_cast<auxChunkHeader*>(block) - 1;
            block->m_size = 0;
            m_memAllocated += -16 - chunk->m_bnum * chunk->m_bsize;
            if (chunk->m_nextChunk)
            {
                chunk->m_nextChunk->m_prevChunk = chunk->m_prevChunk;
            }
            if (chunk->m_prevChunk)
            {
                chunk->m_prevChunk->m_nextChunk = chunk->m_nextChunk;
            }
            if (m_chunks == chunk)
            {
                m_chunks = chunk->m_prevChunk;
            }
            m_memOverhead -= 16;
            free(chunk);
            return;
        }
        // m_nextBlock of a pooled block holds the index of its size class while the block is in use.
        int const v = reinterpret_cast<int>(block->m_nextBlock);
        block->m_nextBlock = m_blocks[v];
        m_blocks[v] = block;
        block->m_size = 0;
        m_memOverhead -= 16;
    }

    MemoryManager::~MemoryManager()
    {
        // RVA 0x748820 - writes a leak report and frees every chunk; the critical section is deleted by the m_cs
        // member.
        // NOTE: m_firstChunk is left pointing at the freed first chunk.
        unsigned int appMemLost = 0;
        unsigned int appMemLostOverhead = 0;
        int blocksLost = 0;
        FILE* const file = fopen("memlog.txt", "w");
        if (file)
        {
            time_t now;
            time(&now);
            char s[32];
            strcpy(s, asctime(localtime(&now)));
            s[24] = 0;
            fprintf(file, " -----------------------------------------------------------------------\n");
            fprintf(file, "|            Memory leaks dump (%s)               |\n", s);
            fprintf(file, " -----------------------------------------------------------------------\n\n");
            fprintf(file, ".----------------------------.-------------------.---------------------------------------.\n");
            fprintf(file, "|       SOURCE FILE          |       LINE        |        BYTES        |     ALLOC #     |\n");
            fprintf(file, "+----------------------------|-------------------|---------------------------------------+\n");
        }
        for (auxChunkHeader* chunk = m_firstChunk; chunk;)
        {
            auxChunkHeader* const nextChunk = chunk->m_nextChunk;
            char* block = reinterpret_cast<char*>(chunk + 1);
            for (int i = 0; i < chunk->m_bnum; ++i, block += chunk->m_bsize)
            {
                int const size = reinterpret_cast<auxBlockHeader*>(block)->m_size;
                if (size)
                {
                    appMemLostOverhead += 4;
                    appMemLost += size;
                    ++blocksLost;
                    if (file)
                    {
                        fprintf(file, "| %26s |      [%04d]       | %15d     | %15d |\n", "", 0, size, 0);
                    }
                }
            }
            m_memAllocated += -16 - chunk->m_bnum * chunk->m_bsize;
            if (chunk->m_nextChunk)
            {
                chunk->m_nextChunk->m_prevChunk = chunk->m_prevChunk;
            }
            if (chunk->m_prevChunk)
            {
                chunk->m_prevChunk->m_nextChunk = chunk->m_nextChunk;
            }
            if (m_chunks == chunk)
            {
                m_chunks = chunk->m_prevChunk;
            }
            m_memOverhead -= 16;
            free(chunk);
            chunk = nextChunk;
        }
        if (file)
        {
            unsigned int const total = appMemLost + appMemLostOverhead;
            fprintf(file, " -----------------------------------------------------------------------------------------\n\n");
            fprintf(file, "# of memory blocks lost: %d\n", blocksLost);
            fprintf(file, "Memory lost (bytes): app = %d, memman overhead = %d, total = %d \n", appMemLost,
                    appMemLostOverhead, total);
            float const ratio =
                appMemLostOverhead ? static_cast<float>(appMemLostOverhead) * 100.0f / static_cast<float>(total) : 0.0f;
            fprintf(file, "Memman overhead/Total memory lost ratio: %0.1f%s\n", ratio, "%");
            fclose(file);
        }
    }

    unsigned MemoryManager::debugMemUsed() const
    {
        // RVA 0x5893D0
        return m_memUsed;
    }

    void MemoryManager::CheckMemory()
    {
        // RVA 0x748620
        for (auxChunkHeader* chunk = m_firstChunk; chunk; chunk = chunk->m_nextChunk)
        {
            char* block = reinterpret_cast<char*>(chunk + 1);
            for (int i = 0; i < chunk->m_bnum; ++i, block += chunk->m_bsize)
            {
                auxBlockHeader* const header = reinterpret_cast<auxBlockHeader*>(block);
                if (header->m_size)
                {
                    if (header->m_magic != kMagic)
                    {
                        __debugbreak();
                    }
                    if (Trailer(header) != kTrailer)
                    {
                        __debugbreak();
                    }
                }
            }
        }
    }

    auxBlockHeader* MemoryManager::NewChunk(int bsize, int bnum)
    {
        // RVA 0x748670 - a chunk is a header followed by bnum blocks of a 12-byte header, bsize bytes of data and a
        // 4-byte trailer; its blocks are returned linked into a free list.
        int const stride = bsize + 16;
        m_memOverhead += 16;
        int const total = bnum * stride + 16;
        auxChunkHeader* const chunk = static_cast<auxChunkHeader*>(malloc(total));
        if (!chunk)
        {
            return nullptr;
        }
        m_memAllocated += total;
        ++m_numNewChunks;
        chunk->m_nextChunk = nullptr;
        chunk->m_prevChunk = m_chunks;
        chunk->m_bnum = bnum;
        chunk->m_bsize = stride;
        if (m_chunks)
        {
            m_chunks->m_nextChunk = chunk;
        }
        else
        {
            m_firstChunk = chunk;
        }
        m_chunks = chunk;

        char* const first = reinterpret_cast<char*>(chunk + 1);
        char* block = reinterpret_cast<char*>(chunk) + total;
        auxBlockHeader* next = nullptr;
        while (block > first)
        {
            block -= stride;
            auxBlockHeader* const header = reinterpret_cast<auxBlockHeader*>(block);
            header->m_nextBlock = next;
            header->m_size = 0;
            header->m_magic = kFreedMagic;
            next = header;
        }
        return reinterpret_cast<auxBlockHeader*>(first);
    }

    unsigned MemoryManager::debugMemAllocated() const
    {
        // RVA 0x5893E0
        return m_memAllocated;
    }

    unsigned MemoryManager::debugMemOverhead() const
    {
        // RVA 0x5893F0
        return m_memOverhead;
    }

    void MemoryManager::DumpMemory(char const* filename)
    {
        // RVA 0x748780 - the file name is not used.
        CheckMemory();
    }

    void* MemoryManager::Malloc(int size, char const* src_name, int src_line)
    {
        // RVA 0x748A30
        AutoLock lock(m_cs);
        if (!size)
        {
            size = 1;
        }
        else if (size > 1024)
        {
            // NOTE: unlike the pooled path, a big block adds no overhead of its own beyond NewChunk's.
            auxBlockHeader* const block = NewChunk(size, 1);
            if (!block)
            {
                m_lastUnsuccessfulAllocationSize = size;
                __debugbreak();
                return nullptr;
            }
            block->m_nextBlock = kOwnChunk;
            block->m_size = size;
            block->m_magic = kMagic;
            Trailer(block) = kTrailer;
            m_memUsed += size;
            ++m_memNumAlloc;
            return block + 1;
        }

        int v = 0;
        while (size > m_b_size[v])
        {
            ++v;
        }
        auxBlockHeader* const block = m_blocks[v];
        if (!block)
        {
            return nullptr;
        }
        m_blocks[v] = block->m_nextBlock;
        if (!m_blocks[v])
        {
            m_blocks[v] = NewChunk(m_b_size[v], m_b_num[v]);
            if (!m_blocks[v])
            {
                // NOTE: the block already taken is still handed out after the break.
                m_lastUnsuccessfulAllocationSize = size;
                __debugbreak();
            }
        }
        block->m_nextBlock = reinterpret_cast<auxBlockHeader*>(v);
        block->m_size = size;
        block->m_magic = kMagic;
        Trailer(block) = kTrailer;
        m_memUsed += size;
        ++m_memNumAlloc;
        m_memOverhead += 16;
        return block + 1;
    }

    void MemoryManager::turnAggressiveDebugMode(bool bOn)
    {
        // RVA 0x748610 - empty.
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
        // RVA 0x748790 - empty.
    }

    void MemoryManager::FreeChunk(auxChunkHeader* ch)
    {
        // RVA 0x748710
        m_memAllocated += -16 - ch->m_bnum * ch->m_bsize;
        if (ch->m_nextChunk)
        {
            ch->m_nextChunk->m_prevChunk = ch->m_prevChunk;
        }
        if (ch->m_prevChunk)
        {
            ch->m_prevChunk->m_nextChunk = ch->m_nextChunk;
        }
        if (m_chunks == ch)
        {
            m_chunks = ch->m_prevChunk;
        }
        m_memOverhead -= 16;
        free(ch);
    }
}

#pragma once
#include <windows.h>
#include <vector>

namespace m3d
{
    namespace fs
    {
        struct auxChunkInfo
        {
            unsigned int tag;
            unsigned int size;
            unsigned int offset;
            unsigned int crc32;
        };

        class auxTaggedFile
        {
        public:
            enum eError
            {
                SUCCESS = 0,
                NOT_INITIALIZED = 1,
                FILE_NOT_FOUND = 2,
                BAD_FORMAT = 3,
                BAD_NUM_CHUNKS = 4,
                BAD_CHUNK_CRC = 5,
                TAG_NOT_FOUND = 6,
                TAG_EXISTS = 7,
                COMMON_ERROR = 100,
                UNKNOWN_ERROR = 200,
            };

            enum eOpenFlag
            {
                PROCESS_NORMAL = 0x0,
                PROCESS_MAPPED = 0x1,
                PROCESS_NORMAL_IGNORE_CRC = 0x2,
                PROCESS_MAPPED_IGNORE_CRC = 0x3,
                CREATE = 0x4,
                CREATE_IGNORE_CRC = 0x5,
            };

            struct mTaggedHeader
            {
                char cSignature[7];
                unsigned int numChunks;
            };

            struct mChunkData
            {
                bool is_copy;
                unsigned int size;
                void* data;
            };

            struct mChunk
            {
                auxChunkInfo chunk_header;
                std::vector<mChunkData> chunk_data;
            };

        public:
            auxTaggedFile();
            ~auxTaggedFile();
            eError addChunk(unsigned int);
            bool isChunkPresent(unsigned int) const;
            eError getChunkDataCopy(unsigned int, void*) const;
            eError addChunkDataCopy(unsigned int, unsigned int, void const*);
            eError setFormatTitle(char const*);
            eError getFormatTitle(char**);
            eError getChunkData(unsigned int, void**) const;
            eError getFormatVersion(unsigned int&);
            eError setFormatVersion(unsigned int);
            eError getChunkInfo(unsigned int, auxChunkInfo&) const;
            eError addChunkData(unsigned int, unsigned int, void const*);
            eError Open(char const*, enum eOpenFlag);
            eError Close();

        private:
            unsigned int findChunk(unsigned int) const;

        private:
            std::vector<mChunk> m_lAllChunks;
            bool m_bOpened = false;
            eOpenFlag m_openflag;
            HANDLE m_hFile = nullptr;
            HANDLE m_hFileMapping = nullptr;
            void* m_pFileData = nullptr;
            char* m_format_name = nullptr;
            unsigned int m_format_version = 0;
        };
    }
}

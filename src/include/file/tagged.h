#pragma once
#include <script/scriptserver.h>
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
            using eError = eScriptError;

            enum eOpenFlag
            {
                PROCESS_NORMAL = 0x0,
                PROCESS_MAPPED = 0x1,
                PROCESS_NORMAL_IGNORE_CRC = 0x2,
                PROCESS_MAPPED_IGNORE_CRC = 0x3,
                CREATE = 0x4,
                CREATE_IGNORE_CRC = 0x5,
            };

            struct mChunkData
            {
                bool is_copy;
                unsigned int size;
                void* data;
            };

            class mChunk
            {
            private:
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
            bool m_bOpened;
            eOpenFlag m_openflag;
            void* m_hFile;
            void* m_hFileMapping;
            unsigned __int8* m_pFileData;
            char* m_format_name;
            unsigned int m_format_version;
        };
    }
}

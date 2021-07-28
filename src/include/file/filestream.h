#pragma once
#include "i_stream.h"

namespace m3d
{
    namespace fs
    {
        class FileStream : public IStream
        {
        public:
            virtual ~FileStream() = default;
            virtual int Open(char const*, OpenFlags) = 0;
            virtual bool IsOpen() = 0;
            virtual FileStream& operator>>(CStr&) = 0;
            virtual FileStream& operator<<(char const*) = 0;
            virtual int ReadLine(CStr&) = 0;
            virtual unsigned int FRead(void*, unsigned int, unsigned int) = 0;
            virtual int FSeek(int, int) = 0;
            virtual int FTell() = 0;
            virtual int Close() = 0;

        protected:
            int m_lastError = 0;
            unsigned int m_FileSize = 0;
        };
    }
}

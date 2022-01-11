#pragma once
#include "i_stream.h"
#include <core/stringm3d.h>

namespace m3d
{
    namespace fs
    {
        //IMPORTANT: fields and members order is strict
        class FileStream : public IStream
        {
        protected:
            int m_lastError = 0;
            unsigned int m_FileSize = 0;

        public:
            FileStream(){};
            virtual ~FileStream() = default;
            virtual IStream* Clone() = 0;
            virtual int Open(char const*, OpenFlags) = 0;
            virtual bool IsOpen() = 0;
            virtual unsigned int ReadBytes(void*, unsigned int) = 0;
            virtual unsigned int PeekBytes(void*, unsigned int) = 0;
            virtual unsigned int WriteBytes(void const*, unsigned int) = 0;
            virtual int Eof() = 0;
            virtual int Error() = 0;
            virtual int Flush() = 0;
            virtual FileStream& operator>>(float&) = 0;
            virtual FileStream& operator>>(unsigned int&) = 0;
            virtual FileStream& operator>>(int&) = 0;
            virtual FileStream& operator>>(CStr&) = 0;
            virtual FileStream& operator<<(float) = 0;
            virtual FileStream& operator<<(unsigned int) = 0;
            virtual FileStream& operator<<(int) = 0;
            virtual FileStream& operator<<(char const*) = 0;
            virtual int ReadLine(CStr&) = 0;
            virtual unsigned int FRead(void*, unsigned int, unsigned int) = 0;
            virtual int FSeek(long, int) = 0;
            virtual long FTell() = 0;
            virtual int Close() = 0;
        };
    }
}

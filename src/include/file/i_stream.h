#pragma once
#include <Windows.h>

namespace m3d
{
    namespace fs
    {
        class IStream
        {
        public:
            enum OpenFlags
            {
                OPEN_WRITE = 0x0,
                OPEN_READ = 0x1,
            };

        public:
            virtual ~IStream() = default;
            virtual unsigned int GetSize() const = 0;
            virtual FILETIME GetDate() const = 0;
            virtual IStream*  Clone() = 0;
            virtual unsigned int ReadBytes(void*, unsigned int) = 0;
            virtual unsigned int PeekBytes(void*, unsigned int) = 0;
            virtual unsigned int WriteBytes(void const*, unsigned int) = 0;
            virtual int Eof() = 0;
            virtual int Error() = 0;
            virtual int Flush() = 0;
        };
    }
}

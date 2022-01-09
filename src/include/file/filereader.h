#pragma once
//TODO: windows.h
#include "filestream.h"
#include <Windows.h>
#include <core/stringm3d.h>

namespace m3d
{
    namespace fs
    {
        class FileReader : public FileStream
        {
            friend class FileServer;
        public:
            virtual IStream* Clone();
            virtual int ReadLine(CStr&);
            virtual int Close();
            virtual int Open(char const*, OpenFlags);
            virtual unsigned int PeekBytes(void*, unsigned int);
            virtual int Flush();
            virtual int FSeek(long, int);
            virtual long FTell();
            virtual unsigned int ReadBytes(void*, unsigned int);
            virtual unsigned int WriteBytes(void const*, unsigned int);
            virtual FILETIME GetDate() const;
            virtual bool IsOpen();
            virtual FileStream& operator<<(char const*);
            virtual FileStream& operator<<(float);
            virtual FileStream& operator<<(unsigned int);
            virtual FileStream& operator<<(int);
            virtual ~FileReader();
            virtual FileStream& operator>>(CStr&);
            virtual FileStream& operator>>(float&);
            virtual FileStream& operator>>(int&);
            virtual FileStream& operator>>(unsigned int&);
            virtual int Eof();
            virtual unsigned int FRead(void*, unsigned int, unsigned int);
            virtual unsigned int GetSize() const;
            virtual int Error();

        protected:
            FileStream* InternalObject = nullptr;
        };
    }
}

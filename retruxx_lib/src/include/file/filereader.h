#pragma once
//TODO: windows.h
#include "filestream.h"
#include <Windows.h>
#include <core/stringm3d.h>

namespace m3d
{
    namespace fs
    {
        //IMPORTANT: fields and members order is strict
        class FileReader : public FileStream
        {
            friend class FileServer;

        protected:
            FileStream* InternalObject = nullptr;

        public:
            FileReader();
            virtual int Open(char const*, OpenFlags);
            virtual int Close();
            virtual ~FileReader();
            virtual IStream* Clone();
            virtual bool IsOpen();
            virtual unsigned int GetSize() const;
            virtual FILETIME GetDate() const;
            virtual unsigned int ReadBytes(void*, unsigned int);
            virtual unsigned int WriteBytes(void const*, unsigned int);
            virtual unsigned int PeekBytes(void*, unsigned int);
            virtual int Eof();
            virtual int Error();
            virtual int Flush();
            virtual FileStream& operator>>(float&);
            virtual FileStream& operator>>(unsigned int&);
            virtual FileStream& operator>>(int&);
            virtual FileStream& operator>>(CStr&);
            virtual FileStream& operator<<(float);
            virtual FileStream& operator<<(unsigned int);
            virtual FileStream& operator<<(int);
            virtual FileStream& operator<<(char const*);
            virtual int ReadLine(CStr&);
            virtual unsigned int FRead(void*, unsigned int, unsigned int);
            virtual int FSeek(long, int);
            virtual long FTell();
        };
    }
}

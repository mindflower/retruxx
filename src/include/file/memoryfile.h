#pragma once
#include "filestream.h"
#include "package.h"
//TODO: windows.h
#include <Windows.h>

class CStr;

namespace m3d
{
    namespace fs
    {
        class MemoryFile : public FileStream
        {
        public:
            virtual ~MemoryFile();
            virtual FILETIME GetDate() const;
            virtual unsigned int WriteBytes(void const*, unsigned int);
            virtual long FTell();
            virtual unsigned int ReadBytes(void*, unsigned int);
            virtual int Open(char const*, OpenFlags);
            virtual IStream* Clone();
            virtual int ReadLine(CStr&);
            virtual int Close();
            virtual unsigned int PeekBytes(void*, unsigned int);
            virtual int FSeek(long, int);
            virtual int Flush();
            virtual unsigned int SetPosition(unsigned int);
            virtual unsigned int FRead(void*, unsigned int, unsigned int);
            virtual unsigned int GetSize() const;
            virtual int Error();
            virtual bool IsOpen();
            virtual unsigned int GetPosition();
            virtual FileStream& operator<<(char const*);
            virtual FileStream& operator<<(int);
            virtual FileStream& operator<<(unsigned int);
            virtual FileStream& operator<<(float);
            virtual int Eof();
            virtual FileStream& operator>>(float&);
            virtual FileStream& operator>>(int&);
            virtual FileStream& operator>>(unsigned int&);
            virtual FileStream& operator>>(CStr&);
            virtual MemoryFile* Copy();

            MemoryFile(char const*, Package*);
            MemoryFile(MemoryFile const&);

        protected:
            MemoryFile();
            int Open(char const*, Package*);

        private:
            Package* m_Package;
            unsigned int m_FileID;
            unsigned __int8* m_Data;
            unsigned __int8* m_CurrData;
        };
    }
}

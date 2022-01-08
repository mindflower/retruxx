#pragma once
#include "filestream.h"
#include <Windows.h>
#include <core/stringm3d.h>

namespace m3d
{
    namespace fs
    {
        class RawFile : public FileStream
        {
        public:
            virtual int FSeek(long, int);
            virtual int ReadLine(CStr&);
            virtual RawFile* Copy();
            virtual unsigned int ReadBytes(void*, unsigned int);
            virtual bool IsOpen();
            virtual unsigned int GetSize() const;
            virtual IStream* Clone();
            virtual ~RawFile();
            virtual FileStream& operator<<(int);
            virtual FileStream& operator<<(unsigned int);
            virtual FileStream& operator<<(char const*);
            virtual FileStream& operator<<(float);
            virtual unsigned int FRead(void*, unsigned int, unsigned int);
            virtual FileStream& operator>>(unsigned int&);
            virtual FileStream& operator>>(int&);
            virtual FileStream& operator>>(CStr&);
            virtual FileStream& operator>>(float&);
            virtual unsigned int WriteBytes(void const*, unsigned int);
            virtual unsigned int SetPosition(unsigned int);
            virtual int Eof();
            virtual int Flush();
            virtual int Open(char const*, OpenFlags);
            virtual int Close();
            virtual int Error();
            virtual FILETIME GetDate() const;
            virtual unsigned int GetPosition();
            virtual long FTell();
            virtual unsigned int PeekBytes(void*, unsigned int);

            RawFile(RawFile const&);
            RawFile(char const*, OpenFlags, bool);

        protected:
            virtual bool CheckBeforeAction(OpenFlags);

            RawFile();
            unsigned int PeekBytesInternal(void*, unsigned int);
            int Open(char const*, OpenFlags, bool);
            unsigned int WriteBytesInternal(void const*, unsigned int);
            unsigned int ReadBytesInternal(void*, unsigned int);

        private:
            HANDLE m_hFile = INVALID_HANDLE_VALUE;      //TODO: check this
            HANDLE m_hMapping = INVALID_HANDLE_VALUE;   //TODO: check this
            bool m_EnableMapping = false;
            OpenFlags m_OpenMode = OPEN_READ;
            void* m_Data = nullptr;
            void* m_CurrData = nullptr;
        };
    }
}

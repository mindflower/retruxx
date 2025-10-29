#include <stdexcept>
#include <core/kernel.h>
#include <file/filereader.h>
#include <file/fileserver.h>

namespace m3d
{
    namespace fs
    {
        FileReader::FileReader()
        {
        }

        IStream* FileReader::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FileReader::ReadLine(CStr&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FileReader::Close()
        {
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->Close();
            }
            return -1;
        }

        int FileReader::Open(char const* fileName, OpenFlags openMode)
        {
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            return g_Kernel->GetFileServer().OpenFileStream(this, fileName, openMode);
        }

        unsigned FileReader::PeekBytes(void*, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FileReader::Flush()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FileReader::FSeek(long offset, int origin)
        {
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->FSeek(offset, origin);
            }
            return -1;
        }

        long FileReader::FTell()
        {
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->FTell();
            }
            return -1;
        }

        unsigned FileReader::ReadBytes(void* dst, unsigned numBytes)
        {
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->ReadBytes(dst, numBytes);
            }
            return 0;
        }

        unsigned FileReader::WriteBytes(void const* dst, unsigned numBytes)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FILETIME FileReader::GetDate() const
        {
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->GetDate();
            }
            return FILETIME{};
        }

        bool FileReader::IsOpen()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& FileReader::operator<<(char const*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& FileReader::operator<<(float k)
        {
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                InternalObject->operator<<(k);
            }
            return *this;
        }

        FileStream& FileReader::operator<<(unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& FileReader::operator<<(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileReader::~FileReader()
        {
            delete InternalObject;
        }

        FileStream& FileReader::operator>>(CStr&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& FileReader::operator>>(float&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& FileReader::operator>>(int&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& FileReader::operator>>(unsigned&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FileReader::Eof()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned FileReader::FRead(void*, unsigned, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned FileReader::GetSize() const
        {
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->GetSize();
            }
            return 0;
        }

        int FileReader::Error()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}

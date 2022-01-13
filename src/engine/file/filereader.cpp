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
            throw std::logic_error("Not implemented");
        }

        int FileReader::ReadLine(CStr&)
        {
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
        }

        int FileReader::Flush()
        {
            throw std::logic_error("Not implemented");
        }

        int FileReader::FSeek(long, int)
        {
            throw std::logic_error("Not implemented");
        }

        long FileReader::FTell()
        {
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
        }

        FileStream& FileReader::operator<<(char const*)
        {
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
        }

        FileStream& FileReader::operator<<(int)
        {
            throw std::logic_error("Not implemented");
        }

        FileReader::~FileReader()
        {
            delete InternalObject;
        }

        FileStream& FileReader::operator>>(CStr&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& FileReader::operator>>(float&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& FileReader::operator>>(int&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& FileReader::operator>>(unsigned&)
        {
            throw std::logic_error("Not implemented");
        }

        int FileReader::Eof()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned FileReader::FRead(void*, unsigned, unsigned)
        {
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
        }
    }
}

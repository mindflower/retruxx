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
            // RVA 0x757580
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->Clone();
            }
            return nullptr;
        }

        int FileReader::ReadLine(CStr& s)
        {
            // RVA 0x757AE0
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->ReadLine(s);
            }
            return 0;
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

        unsigned FileReader::PeekBytes(void* dst, unsigned numBytes)
        {
            // RVA 0x757790
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->PeekBytes(dst, numBytes);
            }
            return 0;
        }

        int FileReader::Flush()
        {
            // RVA 0x757890
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->Flush();
            }
            return 0;
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

        unsigned FileReader::WriteBytes(void const* src, unsigned numBytes)
        {
            // RVA 0x757730
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->WriteBytes(src, numBytes);
            }
            return 0;
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
            // RVA 0x7575D0
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->IsOpen();
            }
            return false;
        }

        FileStream& FileReader::operator<<(char const* c)
        {
            // RVA 0x757920
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                *InternalObject << c;
            }
            return *this;
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

        FileStream& FileReader::operator<<(unsigned k)
        {
            // RVA 0x757A20
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                *InternalObject << k;
            }
            return *this;
        }

        FileStream& FileReader::operator<<(int k)
        {
            // RVA 0x7579A0
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                *InternalObject << k;
            }
            return *this;
        }

        FileReader::~FileReader()
        {
            delete InternalObject;
        }

        FileStream& FileReader::operator>>(CStr& k)
        {
            // RVA 0x7578E0
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                *InternalObject >> k;
            }
            return *this;
        }

        FileStream& FileReader::operator>>(float& k)
        {
            // RVA 0x757A60
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                *InternalObject >> k;
            }
            return *this;
        }

        FileStream& FileReader::operator>>(int& k)
        {
            // RVA 0x757960
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                *InternalObject >> k;
            }
            return *this;
        }

        FileStream& FileReader::operator>>(unsigned& k)
        {
            // RVA 0x7579E0
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                *InternalObject >> k;
            }
            return *this;
        }

        int FileReader::Eof()
        {
            // RVA 0x7577F0
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->Eof();
            }
            return -1;
        }

        unsigned FileReader::FRead(void* buffer, unsigned size, unsigned count)
        {
            // RVA 0x757B30
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->FRead(buffer, size, count);
            }
            return 0;
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
            // RVA 0x757840
            AutoLock guard(g_Kernel->GetFileServer().GetCriticalSecton());
            if (InternalObject)
            {
                return InternalObject->Error();
            }
            return ERROR_OBJECT_NOT_FOUND;
        }
    }
}

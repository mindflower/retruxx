#include <stdexcept>
#include <file/memoryfile.h>

namespace m3d
{
    namespace fs
    {
        MemoryFile::~MemoryFile()
        {
            m_Package->CloseFile(m_FileID);
        }

        FILETIME MemoryFile::GetDate() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned MemoryFile::WriteBytes(void const*, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        long MemoryFile::FTell()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned MemoryFile::ReadBytes(void*, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MemoryFile::Open(char const*, OpenFlags)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        IStream* MemoryFile::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MemoryFile::ReadLine(CStr&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MemoryFile::Close()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned MemoryFile::PeekBytes(void*, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MemoryFile::FSeek(long, int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MemoryFile::Flush()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned MemoryFile::SetPosition(unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned MemoryFile::FRead(void*, unsigned, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned MemoryFile::GetSize() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MemoryFile::Error()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        bool MemoryFile::IsOpen()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned MemoryFile::GetPosition()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& MemoryFile::operator<<(char const*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& MemoryFile::operator<<(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& MemoryFile::operator<<(unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& MemoryFile::operator<<(float)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MemoryFile::Eof()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& MemoryFile::operator>>(float&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& MemoryFile::operator>>(int&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& MemoryFile::operator>>(unsigned&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& MemoryFile::operator>>(CStr&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        MemoryFile* MemoryFile::Copy()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        MemoryFile::MemoryFile(char const* fileName, Package* package) :
            m_Package(package)
        {
            m_FileID = m_Package->OpenFile(fileName, &m_Data, &m_FileSize);
            if (m_FileID)
            {
                m_CurrData = m_Data;
            }
        }

        int MemoryFile::Open(char const*, Package*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}

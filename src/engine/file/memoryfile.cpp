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
            throw std::logic_error("Not implemented");
        }

        unsigned MemoryFile::WriteBytes(void const*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        long MemoryFile::FTell()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned MemoryFile::ReadBytes(void*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int MemoryFile::Open(char const*, OpenFlags)
        {
            throw std::logic_error("Not implemented");
        }

        IStream* MemoryFile::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int MemoryFile::ReadLine(CStr&)
        {
            throw std::logic_error("Not implemented");
        }

        int MemoryFile::Close()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned MemoryFile::PeekBytes(void*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int MemoryFile::FSeek(long, int)
        {
            throw std::logic_error("Not implemented");
        }

        int MemoryFile::Flush()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned MemoryFile::SetPosition(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned MemoryFile::FRead(void*, unsigned, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned MemoryFile::GetSize() const
        {
            throw std::logic_error("Not implemented");
        }

        int MemoryFile::Error()
        {
            throw std::logic_error("Not implemented");
        }

        bool MemoryFile::IsOpen()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned MemoryFile::GetPosition()
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& MemoryFile::operator<<(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& MemoryFile::operator<<(int)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& MemoryFile::operator<<(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& MemoryFile::operator<<(float)
        {
            throw std::logic_error("Not implemented");
        }

        int MemoryFile::Eof()
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& MemoryFile::operator>>(float&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& MemoryFile::operator>>(int&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& MemoryFile::operator>>(unsigned&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& MemoryFile::operator>>(CStr&)
        {
            throw std::logic_error("Not implemented");
        }

        MemoryFile* MemoryFile::Copy()
        {
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
        }
    }
}

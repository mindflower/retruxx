#include <stdexcept>
#include <file/filereader.h>

namespace m3d
{
    namespace fs
    {
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
            throw std::logic_error("Not implemented");
        }

        int FileReader::Open(char const*, OpenFlags)
        {
            throw std::logic_error("Not implemented");
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

        unsigned FileReader::ReadBytes(void*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned FileReader::WriteBytes(void const*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        FILETIME FileReader::GetDate() const
        {
            throw std::logic_error("Not implemented");
        }

        bool FileReader::IsOpen()
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& FileReader::operator<<(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& FileReader::operator<<(float)
        {
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
        }

        int FileReader::Error()
        {
            throw std::logic_error("Not implemented");
        }
    }
}

#include <stdexcept>
#include <file/rawfile.h>

namespace m3d
{
    namespace fs
    {
        int RawFile::FSeek(long, int)
        {
            throw std::logic_error("Not implemented");
        }

        int RawFile::ReadLine(CStr&)
        {
            throw std::logic_error("Not implemented");
        }

        RawFile* RawFile::Copy()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::ReadBytes(void* dst, unsigned numBytes)
        {
            if (CheckBeforeAction(OPEN_READ))
            {
                return 0;
            }
            return ReadBytesInternal(dst, numBytes);
        }

        bool RawFile::IsOpen()
        {
            return m_hFile != INVALID_HANDLE_VALUE;
        }

        unsigned RawFile::GetSize() const
        {
            return m_FileSize;
        }

        IStream* RawFile::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        RawFile::~RawFile()
        {
            Close();
        }

        FileStream& RawFile::operator<<(int)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& RawFile::operator<<(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& RawFile::operator<<(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& RawFile::operator<<(float)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::FRead(void*, unsigned, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& RawFile::operator>>(unsigned&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& RawFile::operator>>(int&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& RawFile::operator>>(CStr&)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream& RawFile::operator>>(float&)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::WriteBytes(void const*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::SetPosition(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int RawFile::Eof()
        {
            throw std::logic_error("Not implemented");
        }

        int RawFile::Flush()
        {
            throw std::logic_error("Not implemented");
        }

        int RawFile::Open(char const* fileName, OpenFlags openMode)
        {
            if (m_hFile != INVALID_HANDLE_VALUE)
            {
                return 1;
            }

            auto access = GENERIC_READ;
            auto disposition = OPEN_EXISTING;
            if (openMode == OPEN_WRITE)
            {
                access = GENERIC_WRITE;
                disposition = OPEN_ALWAYS;
            }

            CStr unifiedName(fileName);
            UnifyFileName(unifiedName);
            m_OpenMode = openMode;
            m_hFile = ::CreateFileA(unifiedName.c_str(), access, 1, 0, disposition, 0x80, NULL);
            if (m_hFile == INVALID_HANDLE_VALUE)
            {
                m_lastError = ::GetLastError();
                return 1;
            }
            m_FileSize = ::GetFileSize(m_hFile, NULL);
            if (m_FileSize && m_EnableMapping && m_OpenMode == OPEN_READ)
            {
                m_hMapping = ::CreateFileMappingA(m_hFile, 0, 2, 0, m_FileSize, NULL);
                if (m_hMapping != INVALID_HANDLE_VALUE)
                {
                    m_Data = ::MapViewOfFile(m_hMapping, 4, 0, 0, m_FileSize);
                }
                m_CurrData = m_Data;
            }
            return 0;
        }

        int RawFile::Close()
        {
            if (m_hMapping != INVALID_HANDLE_VALUE)
            {
                if (m_Data)
                {
                    ::UnmapViewOfFile(m_Data);
                }
                ::CloseHandle(m_hMapping);
                m_CurrData = nullptr;
                m_Data = nullptr;
                m_hMapping = INVALID_HANDLE_VALUE;
            }
            if (m_hFile != INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(m_hFile);
            }
            m_hFile = INVALID_HANDLE_VALUE;
            m_lastError = ::GetLastError();
            return m_lastError;
        }

        int RawFile::Error()
        {
            throw std::logic_error("Not implemented");
        }

        FILETIME RawFile::GetDate() const
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::GetPosition()
        {
            throw std::logic_error("Not implemented");
        }

        long RawFile::FTell()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::PeekBytes(void*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        RawFile::RawFile(RawFile const&)
        {
            throw std::logic_error("Not implemented");
        }

        RawFile::RawFile(char const* fileName, OpenFlags openMode, bool enableMapping) :
            m_EnableMapping(enableMapping)
        {
            Open(fileName, openMode);
        }

        bool RawFile::CheckBeforeAction(OpenFlags openMode)
        {
            if (IsOpen() && m_OpenMode == openMode)
            {
                return false;
            }
            m_lastError = 6;
            return true;
        }

        RawFile::RawFile()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::PeekBytesInternal(void*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int RawFile::Open(char const*, m3d::fs::IStream::OpenFlags, bool)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::WriteBytesInternal(void const*, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        unsigned RawFile::ReadBytesInternal(void* dst, unsigned numBytes)
        {
            if (m_Data)
            {
                //TODO: check this
                auto const availSize = m_FileSize - (static_cast<char*>(m_CurrData) - m_Data);
                if (numBytes > availSize)
                {
                    numBytes = availSize;
                }
                memcpy(dst, m_CurrData, numBytes);
                return numBytes;
            }
            else
            {
                if (::ReadFile(m_hFile, dst, numBytes, reinterpret_cast<LPDWORD>(&numBytes), 0))
                {
                    m_lastError = 0;
                }
                else
                {
                    m_lastError = ::GetLastError();
                }
                return numBytes;
            }
        }
    }
}

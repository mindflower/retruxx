#include <stdexcept>
#include <core/log.h>
#include <file/rawfile.h>

namespace m3d
{
    namespace fs
    {
        int RawFile::FSeek(long offset, int origin)
        {
            //TODO: check and refactor
            if (m_hFile == INVALID_HANDLE_VALUE)
            {
                return -1;
            }
            if (!m_Data)
            {
                DWORD moveMethod = 0;
                if (origin)
                {
                    if (origin == 1)
                    {
                        moveMethod = 1;
                        if (offset <= 0)
                        {
                            if (-offset > FTell())
                            {
                                offset = 0;
                                moveMethod = 0;
                            }
                        }
                        else if (offset + FTell() > m_FileSize)
                        {
                            offset = 0;
                            moveMethod = 2;
                        }
                    }
                    else
                    {
                        if (origin != 2)
                        {
                            return -1;
                        }
                        moveMethod = 2;
                        if (offset <= 0)
                        {
                            if (-offset > m_FileSize)
                            {
                                offset = 0;
                                moveMethod = 0;
                            }
                        }
                        else
                        {
                            offset = 0;
                        }
                    }
                }
                else
                {
                    moveMethod = 0;
                    if (offset > m_FileSize)
                    {
                        offset = m_FileSize;
                    }
                }
                if (::SetFilePointer(m_hFile, offset, 0, moveMethod) != -1)
                {
                    return 0;
                }
                M3D_LOG_ERR("Error: could not set file pointer, fileSize = " + CStr(m_FileSize));
                return -1;
            }
            if (!origin)
            {
                if (offset < 0)
                {
                    offset = 0;
                }
                if (offset > m_FileSize)
                {
                    offset = m_FileSize;
                    m_lastError = 38;
                }
                m_CurrData = static_cast<char*>(m_Data) + offset;
                return 0;
            }
            if (origin != 1)
            {
                if (origin != 2)
                {
                    return -1;
                }
                if (offset > 0)
                {
                    offset = 0;
                    m_lastError = 38;
                }
                if (-offset < m_FileSize)
                {
                    m_CurrData = static_cast<char*>(m_Data) + m_FileSize + offset;
                    return 0;
                }
                m_CurrData = m_Data;
                return 0;
            }
            if (offset <= 0)
            {
                if (-offset <= static_cast<char*>(m_CurrData) - m_Data)
                {
                    m_CurrData = static_cast<char*>(m_CurrData) + offset;
                    return 0;
                }
                m_CurrData = m_Data;
                return 0;
            }
            if (offset < static_cast<char*>(m_Data) + m_FileSize - m_CurrData)
            {
                m_CurrData = static_cast<char*>(m_CurrData) + offset;
                return 0;
            }
            m_CurrData = static_cast<char*>(m_Data) + m_FileSize;
            m_lastError = 38;
            return 0;
        }

        int RawFile::ReadLine(CStr&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        RawFile* RawFile::Copy()
        {
            RETRUXX_NOT_IMPLEMENTED;
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
            RETRUXX_NOT_IMPLEMENTED;
        }

        RawFile::~RawFile()
        {
            Close();
        }

        FileStream& RawFile::operator<<(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& RawFile::operator<<(unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& RawFile::operator<<(char const*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& RawFile::operator<<(float)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned RawFile::FRead(void*, unsigned, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& RawFile::operator>>(unsigned&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& RawFile::operator>>(int&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& RawFile::operator>>(CStr&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FileStream& RawFile::operator>>(float&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned RawFile::WriteBytes(void const*, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned RawFile::SetPosition(unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int RawFile::Eof()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int RawFile::Flush()
        {
            RETRUXX_NOT_IMPLEMENTED;
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
            RETRUXX_NOT_IMPLEMENTED;
        }

        FILETIME RawFile::GetDate() const
        {
            BY_HANDLE_FILE_INFORMATION info{};
            ::GetFileInformationByHandle(m_hFile, &info);
            return info.ftLastWriteTime;
        }

        unsigned RawFile::GetPosition()
        {
            if (m_hFile == INVALID_HANDLE_VALUE)
            {
                return 0;
            }
            if (m_Data)
            {
                return static_cast<char*>(m_CurrData) - m_Data;
            }
            DWORD res = ::SetFilePointer(m_hFile, 0, 0, 1);
            if (res != -1)
            {
                return res;
            }
            M3D_LOG_ERR("Error: could not set file pointer, fileSize = " + CStr(m_FileSize));
            return res;
        }

        long RawFile::FTell()
        {
            return GetPosition();
        }

        unsigned RawFile::PeekBytes(void*, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        RawFile::RawFile(RawFile const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
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
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned RawFile::PeekBytesInternal(void*, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int RawFile::Open(char const*, m3d::fs::IStream::OpenFlags, bool)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned RawFile::WriteBytesInternal(void const*, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
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

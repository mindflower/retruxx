#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <core/log.h>
#include <file/rawfile.h>

namespace m3d
{
    namespace fs
    {
        namespace
        {
            // Error codes the stream reports (Win32 values).
            int const ERROR_BAD_HANDLE = 6;
            int const ERROR_BAD_ACCESS = 12;
            int const ERROR_PAST_END = 38;
        }

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

        int RawFile::ReadLine(CStr& s)
        {
            // RVA 0x8B0FF0 - skips the line breaks in front, then takes everything up to the next one. A mapped file
            // is scanned in memory; otherwise the file is read one byte at a time (at most 1023 characters).
            if (CheckBeforeAction(OPEN_READ))
            {
                m_lastError = ERROR_BAD_ACCESS;
                s = CStr("");
                return 0;
            }

            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (data)
            {
                unsigned char*& curr = reinterpret_cast<unsigned char*&>(m_CurrData);
                while (curr - data < static_cast<int>(m_FileSize) && (*curr == '\r' || *curr == '\n'))
                {
                    ++curr;
                }
                unsigned int const start = static_cast<unsigned int>(curr - data);
                while (static_cast<int>(start) < static_cast<int>(m_FileSize) &&
                    curr - data < static_cast<int>(m_FileSize) && *curr != '\r' && *curr != '\n')
                {
                    ++curr;
                }
                // NOTE: at the end of the data the line is empty but the call still succeeds.
                if (start >= m_FileSize)
                {
                    s = CStr("");
                    return 1;
                }
                unsigned int const length = static_cast<unsigned int>(curr - data) - start;
                char* const line = new char[length + 1];
                memcpy(line, data + start, length);
                line[length] = 0;
                s = CStr(line);
                delete[] line;
                return 1;
            }

            char* const line = new char[1024];
            char ch;
            PeekBytesInternal(&ch, 1);
            while (ch == '\r' || ch == '\n')
            {
                if (Eof() || Error())
                {
                    break;
                }
                ReadBytesInternal(&ch, 1);
                    {
                        DWORD const pos = ::SetFilePointer(m_hFile, 0, nullptr, FILE_CURRENT);
                        ReadBytesInternal(&ch, 1);
                        ::SetFilePointer(m_hFile, pos, nullptr, FILE_BEGIN);
                    }
            }
            int i = 0;
            while (ch != '\n' && ch != '\r' && !Eof() && !Error() && i < 1023)
            {
                line[i++] = ch;
                ReadBytesInternal(&ch, 1);
                    {
                        DWORD const pos = ::SetFilePointer(m_hFile, 0, nullptr, FILE_CURRENT);
                        ReadBytesInternal(&ch, 1);
                        ::SetFilePointer(m_hFile, pos, nullptr, FILE_BEGIN);
                    }
            }
            line[i] = 0;
            s = CStr(line);
            delete[] line;
            return 1;
        }

        RawFile* RawFile::Copy()
        {
            // RVA 0x8B0AB0
            return new RawFile(*this);
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
            // RVA 0x8B0A80
            return new RawFile(*this);
        }

        RawFile::~RawFile()
        {
            Close();
        }

        FileStream& RawFile::operator<<(int k)
        {
            // RVA 0x8B0DD0
            if (CheckBeforeAction(OPEN_WRITE))
            {
                m_lastError = ERROR_BAD_ACCESS;
                return *this;
            }
            *this << CStr(k).c_str();
            return *this;
        }

        FileStream& RawFile::operator<<(unsigned k)
        {
            // RVA 0x8B0EB0
            if (CheckBeforeAction(OPEN_WRITE))
            {
                m_lastError = ERROR_BAD_ACCESS;
                return *this;
            }
            *this << CStr(k).c_str();
            return *this;
        }

        FileStream& RawFile::operator<<(char const* c)
        {
            // RVA 0x8B0640
            if (CheckBeforeAction(OPEN_WRITE))
            {
                m_lastError = ERROR_BAD_ACCESS;
                return *this;
            }
            if (c)
            {
                WriteBytesInternal(c, static_cast<unsigned>(strlen(c)));
            }
            return *this;
        }

        FileStream& RawFile::operator<<(float k)
        {
            // RVA 0x8B0F90
            if (CheckBeforeAction(OPEN_WRITE))
            {
                m_lastError = ERROR_BAD_ACCESS;
                return *this;
            }
            *this << CStr(k).c_str();
            return *this;
        }

        unsigned RawFile::FRead(void* buffer, unsigned size, unsigned count)
        {
            // RVA 0x8B0690
            unsigned int const total = count * size;
            if (!total)
            {
                return total;
            }
            return ReadBytes(buffer, total) / size;
        }

        FileStream& RawFile::operator>>(unsigned& k)
        {
            // RVA 0x8B0E30
            if (CheckBeforeAction(OPEN_READ))
            {
                m_lastError = ERROR_BAD_ACCESS;
                return *this;
            }
            CStr s;
            *this >> s;
            k = atoi(s.c_str());
            return *this;
        }

        FileStream& RawFile::operator>>(int& k)
        {
            // RVA 0x8B0D50
            if (CheckBeforeAction(OPEN_READ))
            {
                m_lastError = ERROR_BAD_ACCESS;
                return *this;
            }
            CStr s;
            *this >> s;
            k = atoi(s.c_str());
            return *this;
        }

        FileStream& RawFile::operator>>(CStr& k)
        {
            // RVA 0x8B0AE0 - the next whitespace-separated word (at most 1023 characters when not mapped). NOTE: the
            // unmapped path compares signed chars, so bytes from 0x80 up count as whitespace there but not in the
            // mapped path.
            if (CheckBeforeAction(OPEN_READ))
            {
                m_lastError = ERROR_BAD_ACCESS;
                return *this;
            }

            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (!data)
            {
                char* const word = new char[1024];
                char ch;
                PeekBytesInternal(&ch, 1);
                while (ch <= 32)
                {
                    if (Eof() || Error())
                    {
                        break;
                    }
                    ReadBytesInternal(&ch, 1);
                    {
                        DWORD const pos = ::SetFilePointer(m_hFile, 0, nullptr, FILE_CURRENT);
                        ReadBytesInternal(&ch, 1);
                        ::SetFilePointer(m_hFile, pos, nullptr, FILE_BEGIN);
                    }
                }
                int i = 0;
                while (ch > 32 && !Eof() && !Error() && i < 1023)
                {
                    word[i++] = ch;
                    ReadBytesInternal(&ch, 1);
                    {
                        DWORD const pos = ::SetFilePointer(m_hFile, 0, nullptr, FILE_CURRENT);
                        ReadBytesInternal(&ch, 1);
                        ::SetFilePointer(m_hFile, pos, nullptr, FILE_BEGIN);
                    }
                }
                word[i] = 0;
                k = CStr(word);
                delete[] word;
                return *this;
            }

            unsigned char*& curr = reinterpret_cast<unsigned char*&>(m_CurrData);
            while (curr - data < static_cast<int>(m_FileSize) && *curr <= 0x20)
            {
                ++curr;
            }
            unsigned int const start = static_cast<unsigned int>(curr - data);
            while (static_cast<int>(start) < static_cast<int>(m_FileSize) && curr - data < static_cast<int>(m_FileSize) &&
                *curr > 0x20)
            {
                ++curr;
            }
            if (start < m_FileSize)
            {
                unsigned int const length = static_cast<unsigned int>(curr - data) - start;
                char* const word = new char[length + 1];
                memcpy(word, data + start, length);
                word[length] = 0;
                k = CStr(word);
                delete[] word;
            }
            else
            {
                k = CStr("");
            }
            return *this;
        }

        FileStream& RawFile::operator>>(float& k)
        {
            // RVA 0x8B0F10
            if (CheckBeforeAction(OPEN_READ))
            {
                m_lastError = ERROR_BAD_ACCESS;
                return *this;
            }
            CStr s;
            *this >> s;
            k = static_cast<float>(atof(s.c_str()));
            return *this;
        }

        unsigned RawFile::WriteBytes(void const* src, unsigned numBytes)
        {
            // RVA 0x8B0A20
            if (CheckBeforeAction(OPEN_WRITE))
            {
                return 0;
            }
            return WriteBytesInternal(src, numBytes);
        }

        unsigned RawFile::SetPosition(unsigned newPosition)
        {
            // RVA 0x8B0580 - positions past the end are clamped to it and set ERROR_PAST_END.
            m_lastError = 0;
            if (m_hFile == INVALID_HANDLE_VALUE)
            {
                m_lastError = ERROR_BAD_HANDLE;
                return 0;
            }
            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (data)
            {
                if (newPosition < m_FileSize)
                {
                    m_CurrData = data + newPosition;
                    return newPosition;
                }
                m_CurrData = data + m_FileSize;
                m_lastError = ERROR_PAST_END;
                return m_FileSize;
            }
            if (newPosition < m_FileSize)
            {
                return ::SetFilePointer(m_hFile, newPosition, nullptr, FILE_BEGIN);
            }
            m_lastError = ERROR_PAST_END;
            return ::SetFilePointer(m_hFile, 0, nullptr, FILE_END);
        }

        int RawFile::Eof()
        {
            // RVA 0x8B0600
            if (m_hFile == INVALID_HANDLE_VALUE)
            {
                return 1;
            }
            if (m_Data)
            {
                return static_cast<unsigned char*>(m_CurrData) >= static_cast<unsigned char*>(m_Data) + m_FileSize;
            }
            return ::SetFilePointer(m_hFile, 0, nullptr, FILE_CURRENT) >= m_FileSize;
        }

        int RawFile::Flush()
        {
            // RVA 0x8B0530 - NOTE: returns 0 on success, the reverse of the other streams.
            return !::FlushFileBuffers(m_hFile);
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
            // RVA 0x8B0710
            return m_lastError;
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
            m_lastError = ::GetLastError();
            M3D_LOG_ERR("Error: could not set file pointer, fileSize = " + CStr(m_FileSize));
            return res;
        }

        long RawFile::FTell()
        {
            return GetPosition();
        }

        unsigned RawFile::PeekBytes(void* dst, unsigned numBytes)
        {
            // RVA 0x8B09B0
            if (CheckBeforeAction(OPEN_READ))
            {
                return 0;
            }
            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (!data)
            {
                return PeekBytesInternal(dst, numBytes);
            }
            unsigned char* const curr = static_cast<unsigned char*>(m_CurrData);
            unsigned int count = numBytes;
            if (data + m_FileSize < curr + numBytes)
            {
                count = static_cast<unsigned int>(data + m_FileSize - curr);
            }
            memcpy(dst, curr, count);
            return count;
        }

        RawFile::RawFile(RawFile const& rf)
        {
            // RVA 0x8B0770 - the copy gets its own duplicate of the file handle (and its own view of the file when
            // the original was mapped) at the same position.
            m_EnableMapping = rf.m_EnableMapping;
            m_OpenMode = rf.m_OpenMode;
            if (rf.m_hFile == INVALID_HANDLE_VALUE)
            {
                m_hFile = INVALID_HANDLE_VALUE;
                m_hMapping = INVALID_HANDLE_VALUE;
                m_Data = nullptr;
                m_FileSize = 0;
                m_CurrData = nullptr;
                return;
            }
            HANDLE const process = ::GetCurrentProcess();
            ::DuplicateHandle(::GetCurrentProcess(), rf.m_hFile, process, &m_hFile, 0, FALSE, DUPLICATE_SAME_ACCESS);
            m_FileSize = ::GetFileSize(m_hFile, nullptr);
            if (rf.m_hMapping == INVALID_HANDLE_VALUE)
            {
                m_hMapping = INVALID_HANDLE_VALUE;
            }
            else
            {
                m_hMapping = ::CreateFileMappingA(m_hFile, nullptr, PAGE_READONLY, 0, m_FileSize, nullptr);
            }
            // NOTE: an unmapped copy leaves m_Data and m_CurrData unset.
            if (m_hMapping == INVALID_HANDLE_VALUE)
            {
                return;
            }
            unsigned char* const data = static_cast<unsigned char*>(::MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, m_FileSize));
            m_Data = data;
            m_CurrData = data + (static_cast<unsigned char*>(rf.m_CurrData) - static_cast<unsigned char*>(rf.m_Data));
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
            // RVA 0x8B0730
        }

        unsigned RawFile::PeekBytesInternal(void* dst, unsigned numBytes)
        {
            // RVA 0x8B0490 - reads from the file and puts the file pointer back. NOTE: on a mapped file the read
            // advances m_CurrData and is not undone.
            DWORD const pos = ::SetFilePointer(m_hFile, 0, nullptr, FILE_CURRENT);
            unsigned int const read = ReadBytesInternal(dst, numBytes);
            ::SetFilePointer(m_hFile, pos, nullptr, FILE_BEGIN);
            return read;
        }

        int RawFile::Open(char const* fileName, m3d::fs::IStream::OpenFlags openMode, bool enableMapping)
        {
            // RVA 0x8B0350
            m_EnableMapping = enableMapping;
            return Open(fileName, openMode);
        }

        unsigned RawFile::WriteBytesInternal(void const* src, unsigned numBytes)
        {
            // RVA 0x8B04E0
            DWORD written = numBytes;
            if (::WriteFile(m_hFile, src, numBytes, &written, nullptr))
            {
                m_lastError = 0;
            }
            else
            {
                m_lastError = ::GetLastError();
            }
            m_FileSize = ::GetFileSize(m_hFile, nullptr);
            return written;
        }

        unsigned RawFile::ReadBytesInternal(void* dst, unsigned numBytes)
        {
            // RVA 0x8B0400
            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (data)
            {
                unsigned char* const curr = static_cast<unsigned char*>(m_CurrData);
                unsigned int count = numBytes;
                if (data + m_FileSize < curr + numBytes)
                {
                    count = static_cast<unsigned int>(data + m_FileSize - curr);
                }
                memcpy(dst, curr, count);
                m_CurrData = curr + count;
                return count;
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

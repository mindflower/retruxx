#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <file/memoryfile.h>

namespace m3d
{
    namespace fs
    {
        namespace
        {
            // Error code the stream reports when a seek or position runs past the end of the data.
            int const ERROR_PAST_END = 38;
        }

        MemoryFile::~MemoryFile()
        {
            // RVA 0x8AFE40
            m_Package->CloseFile(m_FileID);
        }

        FILETIME MemoryFile::GetDate() const
        {
            // RVA 0x8AFE10 - files inside a package carry no date.
            FILETIME date;
            date.dwLowDateTime = 0;
            date.dwHighDateTime = 0;
            return date;
        }

        unsigned MemoryFile::WriteBytes(void const*, unsigned)
        {
            // RVA 0x8AFBA0 - package files are read-only.
            return 0;
        }

        long MemoryFile::FTell()
        {
            // RVA 0x8AFD70
            return GetPosition();
        }

        unsigned MemoryFile::ReadBytes(void* dst, unsigned numBytes)
        {
            // RVA 0x8AFB00
            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (!data)
            {
                return 0;
            }
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

        int MemoryFile::Open(char const*, OpenFlags)
        {
            // RVA 0x8AFDF0 - a memory file can only be opened from a package.
            return 0;
        }

        IStream* MemoryFile::Clone()
        {
            // RVA 0x8AFE80 - NOTE: the clone is a fresh, unopened file, not a copy of this one.
            return new MemoryFile();
        }

        int MemoryFile::ReadLine(CStr& s)
        {
            // RVA 0x8B01C0 - skips the line breaks in front, then takes everything up to the next one. NOTE: at the
            // end of the data it still returns 1, with an empty string; only an unopened file returns 0.
            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (!data)
            {
                s = CStr("");
                return 0;
            }

            unsigned char*& curr = reinterpret_cast<unsigned char*&>(m_CurrData);
            while (curr - data < static_cast<int>(m_FileSize) && (*curr == '\r' || *curr == '\n'))
            {
                ++curr;
            }
            unsigned int const start = static_cast<unsigned int>(curr - data);
            while (static_cast<int>(start) < static_cast<int>(m_FileSize) && curr - data < static_cast<int>(m_FileSize) &&
                *curr != '\r' && *curr != '\n')
            {
                ++curr;
            }
            if (start < m_FileSize)
            {
                unsigned int const length = static_cast<unsigned int>(curr - data) - start;
                char* const line = new char[length + 1];
                memcpy(line, data + start, length);
                line[length] = 0;
                s = CStr(line);
                delete[] line;
            }
            else
            {
                s = CStr("");
            }
            return 1;
        }

        int MemoryFile::Close()
        {
            // RVA 0x8AFAE0
            m_Package->CloseFile(m_FileID);
            return 1;
        }

        unsigned MemoryFile::PeekBytes(void* dst, unsigned numBytes)
        {
            // RVA 0x8AFB50
            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (!data)
            {
                return 0;
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

        int MemoryFile::FSeek(long offset, int origin)
        {
            // RVA 0x8AFCA0 - positions are clamped to the data; running past the end sets ERROR_PAST_END, and
            // seeking back past the start from the current position lands on the start.
            unsigned char* data = static_cast<unsigned char*>(m_Data);
            if (!data)
            {
                return -1;
            }
            if (origin == SEEK_SET)
            {
                long pos = offset < 0 ? 0 : offset;
                if (pos >= static_cast<long>(m_FileSize))
                {
                    pos = static_cast<long>(m_FileSize);
                    m_lastError = ERROR_PAST_END;
                }
                m_CurrData = data + pos;
                return 0;
            }
            if (origin == SEEK_CUR)
            {
                unsigned char* const curr = static_cast<unsigned char*>(m_CurrData);
                if (offset > 0)
                {
                    if (offset >= static_cast<long>(data + m_FileSize - curr))
                    {
                        m_CurrData = data + m_FileSize;
                        m_lastError = ERROR_PAST_END;
                        return 0;
                    }
                    m_CurrData = curr + offset;
                    return 0;
                }
                if (-offset <= curr - data)
                {
                    m_CurrData = curr + offset;
                    return 0;
                }
                m_CurrData = data;
                return 0;
            }
            if (origin != SEEK_END)
            {
                return -1;
            }
            long back = offset;
            if (offset > 0)
            {
                back = 0;
                m_lastError = ERROR_PAST_END;
            }
            if (static_cast<unsigned long>(-back) < m_FileSize)
            {
                m_CurrData = data + m_FileSize + back;
                return 0;
            }
            m_CurrData = data;
            return 0;
        }

        int MemoryFile::Flush()
        {
            // RVA 0x8AFBB0
            return 1;
        }

        unsigned MemoryFile::SetPosition(unsigned newPosition)
        {
            // RVA 0x8AFBE0
            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            m_lastError = 0;
            if (!data)
            {
                return 0;
            }
            if (newPosition < m_FileSize)
            {
                m_CurrData = data + newPosition;
                return newPosition;
            }
            m_CurrData = data + m_FileSize;
            m_lastError = ERROR_PAST_END;
            return m_FileSize;
        }

        unsigned MemoryFile::FRead(void* buffer, unsigned size, unsigned count)
        {
            // RVA 0x8AFC70
            unsigned int const total = count * size;
            if (!total)
            {
                return total;
            }
            return ReadBytes(buffer, total) / size;
        }

        unsigned MemoryFile::GetSize() const
        {
            // RVA 0x8AFE00
            return m_FileSize;
        }

        int MemoryFile::Error()
        {
            // RVA 0x8AFDE0
            return m_lastError;
        }

        bool MemoryFile::IsOpen()
        {
            // RVA 0x8AFBD0
            return m_FileID != 0;
        }

        unsigned MemoryFile::GetPosition()
        {
            // RVA 0x8AFBC0
            if (!m_Data)
            {
                return 0;
            }
            return static_cast<unsigned>(static_cast<unsigned char*>(m_CurrData) - static_cast<unsigned char*>(m_Data));
        }

        FileStream& MemoryFile::operator<<(char const*)
        {
            // RVA 0x8AFC40 - package files are read-only.
            return *this;
        }

        FileStream& MemoryFile::operator<<(int)
        {
            // RVA 0x8AFC50
            return *this;
        }

        FileStream& MemoryFile::operator<<(unsigned)
        {
            // RVA 0x8AFC60
            return *this;
        }

        FileStream& MemoryFile::operator<<(float k)
        {
            // RVA 0x8B0170 - formats the number and hands it to the (ignored) string writer.
            *this << CStr(k).c_str();
            return *this;
        }

        int MemoryFile::Eof()
        {
            // RVA 0x8AFC20
            return !m_Data || static_cast<unsigned char*>(m_CurrData) >= static_cast<unsigned char*>(m_Data) + m_FileSize;
        }

        FileStream& MemoryFile::operator>>(float& k)
        {
            // RVA 0x8B0110
            CStr s;
            *this >> s;
            k = static_cast<float>(atof(s.c_str()));
            return *this;
        }

        FileStream& MemoryFile::operator>>(int& k)
        {
            // RVA 0x8B0050
            CStr s;
            *this >> s;
            k = atoi(s.c_str());
            return *this;
        }

        FileStream& MemoryFile::operator>>(unsigned& k)
        {
            // RVA 0x8B00B0
            CStr s;
            *this >> s;
            k = atoi(s.c_str());
            return *this;
        }

        FileStream& MemoryFile::operator>>(CStr& k)
        {
            // RVA 0x8AFF00 - the next whitespace-separated word; an empty string at the end of the data.
            unsigned char* const data = static_cast<unsigned char*>(m_Data);
            if (!data)
            {
                k = CStr("");
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

        MemoryFile* MemoryFile::Copy()
        {
            // RVA 0x8AFEC0 - NOTE: like Clone, an unopened file rather than a copy.
            return new MemoryFile();
        }

        MemoryFile::MemoryFile(char const* fileName, Package* package) :
            m_Package(package)
        {
            // RVA 0x8AFD80
            m_FileID = m_Package->OpenFile(fileName, &m_Data, &m_FileSize);
            if (m_FileID)
            {
                m_CurrData = m_Data;
            }
        }

        int MemoryFile::Open(char const* fileName, Package* package)
        {
            // RVA 0x8AFAA0
            m_Package = package;
            m_FileID = package->OpenFile(fileName, &m_Data, &m_FileSize);
            if (m_FileID)
            {
                m_CurrData = m_Data;
            }
            return m_FileID != 0;
        }
    }
}

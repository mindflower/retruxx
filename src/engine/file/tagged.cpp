#include <stdexcept>
#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <file/tagged.h>

namespace m3d
{
    namespace fs
    {
        auxTaggedFile::auxTaggedFile()
        {
        }

        auxTaggedFile::~auxTaggedFile()
        {
            throw std::logic_error("Not implemented");
        }

        auxTaggedFile::eError auxTaggedFile::addChunk(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        bool auxTaggedFile::isChunkPresent(unsigned) const
        {
            throw std::logic_error("Not implemented");
        }

        auxTaggedFile::eError auxTaggedFile::getChunkDataCopy(unsigned _chunkTag, void* _data) const
        {
            if (!m_bOpened)
            {
                return NOT_INITIALIZED;
            }
            if (m_openflag != PROCESS_NORMAL && m_openflag != PROCESS_NORMAL_IGNORE_CRC && m_openflag != PROCESS_MAPPED && m_openflag != PROCESS_MAPPED_IGNORE_CRC)
            {
                return NOT_INITIALIZED;
            }
            auto chunkNum = findChunk(_chunkTag);
            if (chunkNum == m_lAllChunks.size())
            {
                return TAG_NOT_FOUND;
            }
            memcpy(_data, reinterpret_cast<void*>(m_lAllChunks[chunkNum].chunk_header.offset), m_lAllChunks[chunkNum].chunk_header.size);
            return SUCCESS;
        }

        auxTaggedFile::eError auxTaggedFile::addChunkDataCopy(unsigned, unsigned, void const*)
        {
            throw std::logic_error("Not implemented");
        }

        auxTaggedFile::eError auxTaggedFile::setFormatTitle(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        auxTaggedFile::eError auxTaggedFile::getFormatTitle(char** _formatName)
        {
            if (!m_bOpened)
            {
                return NOT_INITIALIZED;
            }
            if (m_openflag != PROCESS_NORMAL && m_openflag != PROCESS_NORMAL_IGNORE_CRC && m_openflag != PROCESS_MAPPED && m_openflag != PROCESS_MAPPED_IGNORE_CRC)
            {
                return NOT_INITIALIZED;
            }
            *_formatName = m_format_name;
            return SUCCESS;
        }

        auxTaggedFile::eError auxTaggedFile::getChunkData(unsigned _chunkTag, void** _data) const
        {
            if (!m_bOpened)
            {
                return NOT_INITIALIZED;
            }
            if (m_openflag != PROCESS_NORMAL && m_openflag != PROCESS_NORMAL_IGNORE_CRC && m_openflag != PROCESS_MAPPED && m_openflag != PROCESS_MAPPED_IGNORE_CRC)
            {
                return NOT_INITIALIZED;
            }
            auto chunkNum = findChunk(_chunkTag);
            if (chunkNum == m_lAllChunks.size())
            {
                return TAG_NOT_FOUND;
            }
            *_data = reinterpret_cast<void*>(m_lAllChunks[chunkNum].chunk_header.offset);
            return SUCCESS;
        }

        auxTaggedFile::eError auxTaggedFile::getFormatVersion(unsigned& version)
        {
            if (!m_bOpened)
            {
                return NOT_INITIALIZED;
            }
            if (m_openflag != PROCESS_NORMAL && m_openflag != PROCESS_NORMAL_IGNORE_CRC && m_openflag != PROCESS_MAPPED && m_openflag != PROCESS_MAPPED_IGNORE_CRC)
            {
                return NOT_INITIALIZED;
            }
            version = m_format_version;
            return SUCCESS;
        }

        auxTaggedFile::eError auxTaggedFile::setFormatVersion(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        auxTaggedFile::eError auxTaggedFile::getChunkInfo(unsigned, auxChunkInfo&) const
        {
            throw std::logic_error("Not implemented");
        }

        auxTaggedFile::eError auxTaggedFile::addChunkData(unsigned, unsigned, void const*)
        {
            throw std::logic_error("Not implemented");
        }

        auxTaggedFile::eError auxTaggedFile::Open(char const* _fname, eOpenFlag _flag)
        {
            //TODO: check this
            if (_flag != PROCESS_MAPPED && _flag != PROCESS_MAPPED_IGNORE_CRC)
            {
                if (_flag != PROCESS_NORMAL && _flag != PROCESS_NORMAL_IGNORE_CRC)
                {
                    if (_flag != CREATE && _flag != CREATE_IGNORE_CRC)
                    {
                        return COMMON_ERROR;
                    }
                    m_hFile = ::CreateFileA(_fname, 0x40000000u, 0, 0, 2u, 0x80u, 0);
                    if (m_hFile == INVALID_HANDLE_VALUE)
                    {
                        return UNKNOWN_ERROR;
                    }
                    m_openflag = _flag;
                    m_bOpened = true;
                    return SUCCESS;
                }
                m_pFileData = nullptr;
                unsigned size = 0;
                scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
                if (fileStream->Open(_fname, IStream::OPEN_READ))
                {
                    size = fileStream->GetSize();
                    m_pFileData = new unsigned char[size];
                    fileStream->ReadBytes(m_pFileData, size);
                    fileStream->Close();
                }
                if (!m_pFileData)
                {
                    return UNKNOWN_ERROR;
                }
                if (size < 0xC)
                {
                    delete[] m_pFileData;
                    return BAD_FORMAT;
                }
                auto* _header = reinterpret_cast<mTaggedHeader*>(m_pFileData);
                if (strncmp(_header->cSignature, "ecbnt,t", sizeof(mTaggedHeader::cSignature)))
                {
                    delete[] m_pFileData;
                    return BAD_FORMAT;
                }
                if (16 * _header->numChunks + 12 > size)
                {
                    delete[] m_pFileData;
                    return BAD_NUM_CHUNKS;
                }
                auto fileData = reinterpret_cast<unsigned*>(static_cast<char*>(m_pFileData) + 12);
                for (unsigned i = 0; i <_header->numChunks; ++i)
                {
                    //TODO: check this
                    mChunk chunk;
                    chunk.chunk_header.tag = fileData[0];
                    chunk.chunk_header.size = fileData[1];
                    chunk.chunk_header.offset = fileData[2];
                    chunk.chunk_header.crc32 = fileData[3];
                    mChunkData data;
                    data.is_copy = false;
                    data.size = fileData[1];
                    chunk.chunk_header.offset += reinterpret_cast<unsigned>(m_pFileData);
                    chunk.chunk_data.push_back(data);
                    m_lAllChunks.push_back(chunk);
                    fileData += 4;
                }
                m_bOpened = true;
                m_openflag = _flag;
                char* formatName = nullptr;
                if (getChunkData(0xF001, reinterpret_cast<void**>(&formatName)))
                {
                    delete[] m_pFileData;
                    return BAD_FORMAT;
                }
                m_format_name = new char[strlen(formatName) + 1];
                strcpy(m_format_name, formatName);
                m_format_name[size] = '\0';
                if (getChunkDataCopy(0xF002, &m_format_version) == SUCCESS)
                {
                    return SUCCESS;
                }
                delete[] m_pFileData;
                delete[] m_format_name;
                return BAD_FORMAT;
            }
            m_hFile = ::CreateFileA(_fname, 0x80000000, 1u, 0, 3u, 0x80u, 0);
            if (m_hFile == INVALID_HANDLE_VALUE)
            {
                return FILE_NOT_FOUND;
            }
            m_hFileMapping = ::CreateFileMappingA(m_hFile, 0, 2u, 0, 0, 0);
            if (m_hFileMapping == INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(m_hFile);
                return FILE_NOT_FOUND;
            }
            auto size = ::GetFileSize(m_hFile, NULL);
            ::CloseHandle(m_hFile);
            if (size < 12)
            {
                ::CloseHandle(m_hFileMapping);
                return BAD_FORMAT;
            }
            m_pFileData = ::MapViewOfFile(this->m_hFileMapping, 4u, 0, 0, 0);
            if (!m_pFileData)
            {
                ::CloseHandle(m_hFileMapping);
                return UNKNOWN_ERROR;
            }

            auto* _header = reinterpret_cast<mTaggedHeader*>(m_pFileData);
            if (strncmp(_header->cSignature, "ecbnt,t", sizeof(mTaggedHeader::cSignature)))
            {
                ::UnmapViewOfFile(m_pFileData);
                ::CloseHandle(m_hFileMapping);
                return BAD_FORMAT;
            }
            if (16 * _header->numChunks + 12 > size)
            {
                ::UnmapViewOfFile(m_pFileData);
                ::CloseHandle(m_hFileMapping);
                return BAD_NUM_CHUNKS;
            }
            auto fileData = reinterpret_cast<unsigned*>(static_cast<char*>(m_pFileData) + 12);
            for (unsigned i = 0; i < _header->numChunks; ++i)
            {
                //TODO: check this
                mChunk chunk;
                chunk.chunk_header.tag = fileData[0];
                chunk.chunk_header.size = fileData[1];
                chunk.chunk_header.offset = fileData[2];
                chunk.chunk_header.crc32 = fileData[3];
                mChunkData data;
                data.is_copy = false;
                data.size = fileData[1];
                chunk.chunk_header.offset += reinterpret_cast<unsigned>(m_pFileData);
                chunk.chunk_data.push_back(data);
                m_lAllChunks.push_back(chunk);
                fileData += 4;
            }
            m_bOpened = true;
            m_openflag = _flag;
            char* formatName = nullptr;
            getChunkData(0xF001, reinterpret_cast<void**>(&formatName));
            m_format_name = new char[strlen(formatName) + 1];
            strcpy(m_format_name, formatName);
            m_format_name[size] = '\0';
            getChunkDataCopy(0xF002, &m_format_version);
            return SUCCESS;
        }

        auxTaggedFile::eError auxTaggedFile::Close()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned auxTaggedFile::findChunk(unsigned _chunkTag) const
        {
            unsigned result = 0;
            for (auto const& chunk : m_lAllChunks)
            {
                if (chunk.chunk_header.tag == _chunkTag)
                {
                    break;
                }
                ++result;
            }
            return result;
        }
    }
}

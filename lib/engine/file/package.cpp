#include <stdexcept>
#include <cstring>
#include <file/package.h>
#include <Windows.h>
#include <zlib/zlib.h>

namespace m3d
{
    namespace fs
    {
        namespace
        {
            // The package format obscures its numbers: sizes are stored bitwise-inverted and offsets are shifted by
            // these constants.
            unsigned int const HEADER_SIZE = 0x2C;
            unsigned int const HEADER_DISP = 29782;
            unsigned int const FILE_DISP = 9568;

            // Package::Open results.
            int const OPEN_OK = 0;
            int const OPEN_NOT_FOUND = 2;
            int const OPEN_BAD_FORMAT = 13;
        }  // namespace

        Package::DetailedSegment::DetailedSegment(void* pAddress, unsigned int uiSize)
        {
            // RVA 0x8AE440 - NOTE: Position is left unset.
            Address = pAddress;
            Size = uiSize;
        }

        Package::DetailedSegment::DetailedSegment(DetailedSegment const& dseg)
        {
            // RVA 0x8AE460 - NOTE: Position is not copied.
            Address = dseg.Address;
            Size = dseg.Size;
        }

        PackageFileRecordHeader::PackageFileRecordHeader()
        {
            // RVA 0x8AE420
            memset(this, 0, sizeof(PackageFileRecordHeader));
        }

        Package::Package(bool UseMapping)
        {
            // RVA 0x664FB0
            m_hFile = INVALID_HANDLE_VALUE;
            m_hMapping = INVALID_HANDLE_VALUE;
            m_UseMapping = UseMapping;
            m_NextID = 0;
            m_EraseZipBuffers = true;
            m_Data = nullptr;
            m_refCount = 0;
        }

        Package::~Package()
        {
            // RVA 0x8AF8B0
            Close();
        }

        void Package::DecRef()
        {
            // RVA 0x662790 - NOTE: the last release only destroys the package; its memory is never freed.
            if (m_refCount-- == 1)
            {
                this->~Package();
            }
        }

        unsigned Package::OpenFile(char const* FileName, void** Buffer, unsigned* Size)
        {
            // RVA 0x8AF2A0 - a stored file is a view straight into the package; an archived one is unpacked into a
            // buffer of its own. Either way the returned id closes it.
            CStr name(FileName);
            UnifyFileName0(name);
            FilesMapType::iterator const it = m_FilesMap.find(name);
            if (it == m_FilesMap.end())
            {
                *Buffer = nullptr;
                return 0;
            }

            PackageFileRecordHeader const& record = it->second;
            unsigned int const storedSize = ~record.FileSizeDisp2Inv - FILE_DISP;
            unsigned int const position = record.FilePositionDisp2 - FILE_DISP;
            if (!record.IsArchived)
            {
                return MakeView(position, storedSize, Size, Buffer);
            }

            unsigned int archiveSize = 0;
            void* archive = nullptr;
            unsigned int const view = MakeView(position, storedSize, &archiveSize, &archive);
            unsigned long realSize = record.RealSizeDisp1 - HEADER_DISP;
            unsigned char* const data = new unsigned char[realSize];
            *Buffer = data;
            uncompress(data, &realSize, static_cast<unsigned char const*>(archive), archiveSize);
            CloseView(view);
            *Size = static_cast<unsigned int>(realSize);
            ++m_NextID;
            m_OpenViewsMap.insert(std::make_pair(m_NextID, DetailedSegment(data, static_cast<unsigned int>(realSize))));
            return m_NextID;
        }

        void Package::CloseFile(unsigned FileID)
        {
            // RVA 0x8AF080
            OpenViewsType::iterator const it = m_OpenViewsMap.find(FileID);
            if (it != m_OpenViewsMap.end())
            {
                delete[] static_cast<unsigned char*>(it->second.Address);
                m_OpenViewsMap.erase(it);
            }
        }

        int Package::Open(char const* PackageName)
        {
            // RVA 0x8AF9C0 - the package starts with a zip-like "PK\4" header; when its type byte is 1 a table of
            // file records follows it.
            Close();
            int const result = OpenPackageFile(PackageName);
            if (result != OPEN_OK)
            {
                return result;
            }

            void* header = nullptr;
            unsigned int readedBytes = 0;
            unsigned int const headerView = MakeView(0, HEADER_SIZE, &readedBytes, &header);
            unsigned char const* const h = static_cast<unsigned char const*>(header);
            if (!headerView || h[0] != 'P' || h[1] != 'K' || h[2] != 4)
            {
                CloseView(headerView);
                return OPEN_BAD_FORMAT;
            }
            if (h[12] == 1)
            {
                unsigned int const* const fields = static_cast<unsigned int const*>(header);
                unsigned int const tableSize = ~fields[7] - HEADER_DISP;
                void* table = nullptr;
                unsigned int const tableView = MakeView(HEADER_SIZE, tableSize, &readedBytes, &table);
                if (tableView)
                {
                    if (readedBytes == tableSize)
                    {
                        LoadFileRecords(static_cast<unsigned char*>(table), ~fields[5] - HEADER_DISP);
                    }
                    CloseView(tableView);
                }
            }
            CloseView(headerView);
            return OPEN_OK;
        }

        void Package::Close()
        {
            // RVA 0x8AF4D0 - NOTE: the buffers of files still open are dropped without being freed.
            m_OpenViewsMap.clear();
            m_FilesMap.clear();
            ClosePackageFile();
        }

        bool Package::HaveFile(char const* FileName)
        {
            // RVA 0x8AED60
            CStr name(FileName);
            UnifyFileName0(name);
            return m_FilesMap.find(name) != m_FilesMap.end();
        }

        void Package::GetFilesList(retruxx::vector<CStr>& List)
        {
            // RVA 0x8AF570
            List.clear();
            for (FilesMapType::const_iterator it = m_FilesMap.begin(); it != m_FilesMap.end(); ++it)
            {
                List.push_back(it->first);
            }
        }

        void Package::IncRef()
        {
            // RVA 0x661BB0
            ++m_refCount;
        }

        void Package::ClosePackageFile()
        {
            // RVA 0x8AE520 - NOTE: the handles are tested against 0, not INVALID_HANDLE_VALUE, so handles that are
            // already closed get closed again.
            if (m_Data)
            {
                ::UnmapViewOfFile(m_Data);
                m_Data = nullptr;
            }
            if (m_hMapping)
            {
                ::CloseHandle(m_hMapping);
                m_hMapping = INVALID_HANDLE_VALUE;
            }
            if (m_hFile)
            {
                ::CloseHandle(m_hFile);
                m_hFile = INVALID_HANDLE_VALUE;
            }
        }

        unsigned Package::MakeView(unsigned Position, unsigned Size, unsigned* ReadedSize, void** Data)
        {
            // RVA 0x8AEFA0 - a mapped package hands out pointers into the mapping (and does not track them); an
            // unmapped one reads the range into a buffer that the view id owns.
            if (Position >= m_FileSize)
            {
                *Data = nullptr;
                return 0;
            }
            *ReadedSize = Size + Position <= m_FileSize ? Size : m_FileSize - Position;
            if (m_UseMapping)
            {
                *Data = m_Data + Position;
                return ++m_NextID;
            }

            unsigned char* const buffer = new unsigned char[*ReadedSize];
            *Data = buffer;
            ::SetFilePointer(m_hFile, Position, nullptr, FILE_BEGIN);
            DWORD read = 0;
            ::ReadFile(m_hFile, buffer, *ReadedSize, &read, nullptr);
            *ReadedSize = read;
            ++m_NextID;
            m_OpenViewsMap.insert(std::make_pair(m_NextID, DetailedSegment(buffer, *ReadedSize)));
            return m_NextID;
        }

        void Package::LoadFileRecords(unsigned char* buffFAT, int FAT_RecordCount)
        {
            // RVA 0x8AF920 - each record is the fixed header followed by the file's name; its inverted length
            // leads to the next one.
            unsigned int offset = 0;
            for (int i = 0; i < FAT_RecordCount; ++i)
            {
                PackageFileRecordHeader fileInfo;
                memcpy(&fileInfo, buffFAT + offset, sizeof(fileInfo));
                CStr const name(reinterpret_cast<char const*>(buffFAT + offset + sizeof(PackageFileRecordHeader)));
                offset += ~fileInfo.RecordLengthInv;
                m_FilesMap[name] = fileInfo;
            }
        }

        int Package::OpenPackageFile(char const* PackageName)
        {
            // RVA 0x8AE480 - NOTE: a failed mapping is tested against INVALID_HANDLE_VALUE, but CreateFileMapping
            // returns NULL on failure, so the fallback to plain reads only triggers when the view fails.
            m_hFile = ::CreateFileA(PackageName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL, nullptr);
            if (m_hFile == INVALID_HANDLE_VALUE)
            {
                return OPEN_NOT_FOUND;
            }
            m_FileSize = ::GetFileSize(m_hFile, nullptr);
            if (!m_UseMapping)
            {
                m_hMapping = INVALID_HANDLE_VALUE;
                m_Data = nullptr;
                return OPEN_OK;
            }
            m_hMapping = ::CreateFileMappingA(m_hFile, nullptr, PAGE_READONLY, 0, m_FileSize, nullptr);
            if (m_hMapping == INVALID_HANDLE_VALUE)
            {
                m_UseMapping = false;
                m_Data = nullptr;
                return OPEN_OK;
            }
            m_Data = static_cast<unsigned char*>(::MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, m_FileSize));
            if (!m_Data)
            {
                m_UseMapping = false;
            }
            return OPEN_OK;
        }

        void Package::CloseView(unsigned ViewID)
        {
            // RVA 0x8AEDF0
            OpenViewsType::iterator const it = m_OpenViewsMap.find(ViewID);
            if (it != m_OpenViewsMap.end())
            {
                delete[] static_cast<unsigned char*>(it->second.Address);
                m_OpenViewsMap.erase(it);
            }
        }

        void Package::GetOpenFilesList(retruxx::vector<CStr>& fileList) const
        {
            // RVA 0x8AF620 - NOTE: only empties the list.
            fileList.clear();
        }
    }
}

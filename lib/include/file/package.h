#pragma once
#include <core/stringm3d.h>
#include "retruxx/common.h"

#include <cstdint>

namespace m3d
{
    namespace fs
    {
        // One entry of a package file table, read byte for byte.
#pragma pack(push, 4)
        struct PackageFileRecordHeader
        {
            uint32_t RecordLengthInv;
            uint32_t FileDateHigh;
            uint32_t FileDateLow;
            bool IsArchived;
            char Padding_16[3];
            uint32_t reserved1;
            uint32_t FilePositionDisp2;
            uint32_t FileSizeDisp2Inv;
            uint32_t RealSizeDisp1;
            uint32_t reserved2;
            int32_t Stat1;
            int32_t Stat2;
            int32_t Stat3;
            uint32_t ExternalData;
            PackageFileRecordHeader();
            PackageFileRecordHeader(unsigned int, unsigned long, unsigned long, bool, unsigned int, unsigned int, unsigned int);
        };
#pragma pack(pop)

        static_assert(sizeof(PackageFileRecordHeader) == 0x0034);

        class Package
        {
        public:
            void IncRef();
            void DecRef();
            int GetRefCount();
            unsigned int OpenFile(const char* FileName, void** Buffer, unsigned int* Size);
            void CloseFile(unsigned int FileID);
            void GetFilesList(retruxx::vector<CStr, retruxx::allocator<CStr> >& List);
            void GetOpenFilesList(retruxx::vector<CStr, retruxx::allocator<CStr> >& fileList) const;
            bool HaveFile(const char* FileName);

            enum ZipCloseFunction;

        public:
            Package(const m3d::fs::Package&);
            Package(bool UseMapping);
            int Open(const char* PackageName);
            void Close();
            void EnableEraseZipBuffers(bool);
            void EraseZipBuffers();
            unsigned int GetOpenFilesCount() const;
            ~Package();

        protected:
            void* m_hFile;
            void* m_hMapping;
            bool m_UseMapping;
            char Padding_54[3];
            unsigned int m_NextID;
            bool m_EraseZipBuffers;
            char Padding_55[3];
            unsigned char* m_Data;
            unsigned int m_FileSize;
            int OpenPackageFile(const char* PackageName);
            void ClosePackageFile();
            void CloseView(unsigned int ViewID);
            void LoadFileRecords(unsigned char* buffFAT, int FAT_RecordCount);
            unsigned int MakeView(unsigned int Position, unsigned int Size, unsigned int* ReadedSize, void** Data);

            using FilesMapType = retruxx::map<CStr, m3d::fs::PackageFileRecordHeader, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, m3d::fs::PackageFileRecordHeader> > >;
            class FilesMapIterator;

        protected:
            retruxx::map<CStr, m3d::fs::PackageFileRecordHeader, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, m3d::fs::PackageFileRecordHeader> > > m_FilesMap;

            struct DetailedSegment
            {
                void* Address;
                unsigned int Size;
                unsigned int Position;
                DetailedSegment(const m3d::fs::Package::DetailedSegment& dseg);
                DetailedSegment(void* pAddress, unsigned int uiSize);
                DetailedSegment();
            };

            using OpenViewsType = retruxx::map<unsigned int, m3d::fs::Package::DetailedSegment, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, m3d::fs::Package::DetailedSegment> > >;
            class OpenViewsIterator;

        protected:
            retruxx::map<unsigned int, m3d::fs::Package::DetailedSegment, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, m3d::fs::Package::DetailedSegment> > > m_OpenViewsMap;

        private:
            int m_refCount;
        };
    }
}

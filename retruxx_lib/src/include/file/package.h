#pragma once
#include <core/stringm3d.h>
#include "retruxx/common.h"

namespace m3d
{
    namespace fs
    {
        struct PackageFileRecordHeader
        {
            /* 0x0000 */ unsigned int RecordLengthInv;
            /* 0x0004 */ unsigned long FileDateHigh;
            /* 0x0008 */ unsigned long FileDateLow;
            /* 0x000c */ bool IsArchived;
            /* 0x000d */ char Padding_16[3];
            /* 0x0010 */ unsigned long reserved1;
            /* 0x0014 */ unsigned int FilePositionDisp2;
            /* 0x0018 */ unsigned int FileSizeDisp2Inv;
            /* 0x001c */ unsigned int RealSizeDisp1;
            /* 0x0020 */ unsigned long reserved2;
            /* 0x0024 */ int Stat1;
            /* 0x0028 */ int Stat2;
            /* 0x002c */ int Stat3;
            /* 0x0030 */ unsigned int ExternalData;
            PackageFileRecordHeader();
            PackageFileRecordHeader(unsigned int, unsigned long, unsigned long, bool, unsigned int, unsigned int, unsigned int);
        }; /* size: 0x0034 */

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
            /* 0x0000 */ void* m_hFile;
            /* 0x0004 */ void* m_hMapping;
            /* 0x0008 */ bool m_UseMapping;
            /* 0x0009 */ char Padding_54[3];
            /* 0x000c */ unsigned int m_NextID;
            /* 0x0010 */ bool m_EraseZipBuffers;
            /* 0x0011 */ char Padding_55[3];
            /* 0x0014 */ unsigned char* m_Data;
            /* 0x0018 */ unsigned int m_FileSize;
            int OpenPackageFile(const char* PackageName);
            void ClosePackageFile();
            void CloseView(unsigned int ViewID);
            void LoadFileRecords(unsigned char* buffFAT, int FAT_RecordCount);
            unsigned int MakeView(unsigned int Position, unsigned int Size, unsigned int* ReadedSize, void** Data);

            using FilesMapType = retruxx::map<CStr, m3d::fs::PackageFileRecordHeader, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, m3d::fs::PackageFileRecordHeader> > >;
            class FilesMapIterator;

        protected:
            /* 0x001c */ retruxx::map<CStr, m3d::fs::PackageFileRecordHeader, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, m3d::fs::PackageFileRecordHeader> > > m_FilesMap;

            struct DetailedSegment
            {
                /* 0x0000 */ void* Address;
                /* 0x0004 */ unsigned int Size;
                /* 0x0008 */ unsigned int Position;
                DetailedSegment(const m3d::fs::Package::DetailedSegment& dseg);
                DetailedSegment(void* pAddress, unsigned int uiSize);
                DetailedSegment();
            }; /* size: 0x000c */

            using OpenViewsType = retruxx::map<unsigned int, m3d::fs::Package::DetailedSegment, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, m3d::fs::Package::DetailedSegment> > >;
            class OpenViewsIterator;

        protected:
            /* 0x0028 */ retruxx::map<unsigned int, m3d::fs::Package::DetailedSegment, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, m3d::fs::Package::DetailedSegment> > > m_OpenViewsMap;

        private:
            /* 0x0034 */ int m_refCount;
        }; /* size: 0x0038 */

        static_assert(sizeof(Package) == 0x0038);
    }
}

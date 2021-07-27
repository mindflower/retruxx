#pragma once
#include <map>
#include <vector>
#include <core/stringm3d.h>

namespace m3d
{
    namespace fs
    {
        class PackageFileRecordHeader;

        class Package
        {
        public:
            class DetailedSegment
            {
            public:
                DetailedSegment(void*, unsigned int);
                DetailedSegment(DetailedSegment const&);

            private:
                void* Address;
                unsigned int Size;
                unsigned int Position;
            };

        public:
            Package(bool);
            ~Package();

            void DecRef();
            unsigned int OpenFile(char const*, void**, unsigned int*);
            void CloseFile(unsigned int);
            int Open(char const*);
            void Close();
            bool HaveFile(char const*);
            void GetOpenFilesList(std::vector<CStr>&) const;
            void GetFilesList(std::vector<CStr>&);
            void IncRef();

        protected:
            void ClosePackageFile();
            unsigned int MakeView(unsigned int, unsigned int, unsigned int*, void**);
            void LoadFileRecords(unsigned char*, int);
            int OpenPackageFile(char const*);
            void CloseView(unsigned int);

        private:
            void* m_hFile;
            void* m_hMapping;
            bool m_UseMapping;
            unsigned int m_NextID;
            bool m_EraseZipBuffers;
            unsigned __int8* m_Data;
            unsigned int m_FileSize;
            std::map<CStr, PackageFileRecordHeader> m_FilesMap;
            std::map<unsigned int, DetailedSegment> m_OpenViewsMap;
            int m_refCount;
        };

    }
}

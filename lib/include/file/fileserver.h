#pragma once
#include "i_stream.h"
#include <core/ref_ptr.h>
#include <core/stringm3d.h>
#include <core/threadsync.h>
#include <file/package.h>

namespace m3d
{
    namespace fs
    {
        class FileReader;
        class FileStream;

        //IMPORTANT: fields and members order is strict
        class FileServer
        {
        public:
            FileServer();
            virtual  ~FileServer();
            int Shutdown();
            int Initialize(const char* dataSourcesFile);
            int Reinitialize(const char* dataSourcesFile);
            int AddFolder(const char* FolderName, const char* FileMask, bool RecurseSubFolders);
            int RemoveFolder(const char* FolderName);
            int AddPackage(const char* PackageName);
            int AddFile(const char* FileName);
            int RemoveFile(const char* FileName);
            void EnableMapping(bool State);
            void SetCurrentWorkDir(const char* CurrentDirectory);
            const char* GetCurrentWorkDir() const;
            virtual m3d::fs::FileStream* CreateFileStream();
            int OpenFileStream(m3d::fs::FileReader* Reader, const char* filename, m3d::fs::IStream::OpenFlags flags);
            virtual bool FileExists(const char* filename);
            void GetOpenFilesList(retruxx::vector<CStr, retruxx::allocator<CStr> >& fileList) const;
            m3d::CriticalSection& GetCriticalSecton();

        protected:
            void DecryptFileName(const char* fileName, CStr& decryptedFileName);
            int InternalAddPackage(const CStr& PackageName);
            int EnumDataFolderFiles(const char*);

            bool m_Initialized = false;
            bool m_EnableMapping = false;
            char Padding_132[2];
            m3d::CriticalSection m_cs;

            using PackagePrt = ref_ptr<m3d::fs::Package>;
            using PackageStorageType = retruxx::vector<ref_ptr<m3d::fs::Package>, retruxx::allocator<ref_ptr<m3d::fs::Package> > >;

        protected:
            retruxx::vector<ref_ptr<m3d::fs::Package>, retruxx::allocator<ref_ptr<m3d::fs::Package> > > m_Packages;

            using FilesStorageType = retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >;
            class FilesStorageIterator;

        protected:
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> > m_Files;
            CStr m_CurrentWorkDir;
        };
    }
}

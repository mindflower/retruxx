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
            virtual ~FileServer();
            int Shutdown();
            int Initialize(char const* dataSourcesFile);
            int Reinitialize(char const* dataSourcesFile);
            int AddFolder(char const* FolderName, char const* FileMask, bool RecurseSubFolders);
            int RemoveFolder(char const* FolderName);
            int AddPackage(char const* PackageName);
            int AddFile(char const* FileName);
            int RemoveFile(char const* FileName);
            void EnableMapping(bool State);
            void SetCurrentWorkDir(char const* CurrentDirectory);
            char const* GetCurrentWorkDir() const;
            virtual m3d::fs::FileStream* CreateFileStream();
            int OpenFileStream(m3d::fs::FileReader* Reader, char const* filename, m3d::fs::IStream::OpenFlags flags);
            virtual bool FileExists(char const* filename);
            void GetOpenFilesList(retruxx::vector<CStr, retruxx::allocator<CStr>>& fileList) const;
            m3d::CriticalSection& GetCriticalSecton();

        protected:
            void DecryptFileName(char const* fileName, CStr& decryptedFileName);
            int InternalAddPackage(CStr const& PackageName);
            int EnumDataFolderFiles(char const*);

            bool m_Initialized = false;
            bool m_EnableMapping = false;
            m3d::CriticalSection m_cs;

            using PackagePrt = ref_ptr<m3d::fs::Package>;
            using PackageStorageType =
                retruxx::vector<ref_ptr<m3d::fs::Package>, retruxx::allocator<ref_ptr<m3d::fs::Package>>>;

        protected:
            retruxx::vector<ref_ptr<m3d::fs::Package>, retruxx::allocator<ref_ptr<m3d::fs::Package>>> m_Packages;

            using FilesStorageType = retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>;
            class FilesStorageIterator;

        protected:
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>> m_Files;
            CStr m_CurrentWorkDir;
        };
    }  // namespace fs
}  // namespace m3d

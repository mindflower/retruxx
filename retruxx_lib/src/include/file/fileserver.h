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
            virtual  ~FileServer() /* 0x00 */;
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
            virtual m3d::fs::FileStream* CreateFileStream() /* 0x04 */;
            int OpenFileStream(m3d::fs::FileReader* Reader, const char* filename, m3d::fs::IStream::OpenFlags flags);
            virtual bool FileExists(const char* filename) /* 0x08 */;
            void GetOpenFilesList(retruxx::vector<CStr, retruxx::allocator<CStr> >& fileList) const;
            m3d::CriticalSection& GetCriticalSecton();

        protected:
            void DecryptFileName(const char* fileName, CStr& decryptedFileName);
            int InternalAddPackage(const CStr& PackageName);
            int EnumDataFolderFiles(const char*);

            /* 0x0004 */ bool m_Initialized = false;
            /* 0x0005 */ bool m_EnableMapping = false;
            /* 0x0006 */ char Padding_132[2];
            /* 0x0008 */ m3d::CriticalSection m_cs;

            using PackagePrt = ref_ptr<m3d::fs::Package>;
            using PackageStorageType = retruxx::vector<ref_ptr<m3d::fs::Package>, retruxx::allocator<ref_ptr<m3d::fs::Package> > >;

        protected:
            /* 0x0020 */ retruxx::vector<ref_ptr<m3d::fs::Package>, retruxx::allocator<ref_ptr<m3d::fs::Package> > > m_Packages;

            using FilesStorageType = retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >;
            class FilesStorageIterator;

        protected:
            /* 0x0030 */ retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> > m_Files;
            /* 0x003c */ CStr m_CurrentWorkDir;
        }; /* size: 0x0048 */

        static_assert(sizeof(FileServer) == 0x0048);
    }
}

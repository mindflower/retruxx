#pragma once
#include "i_stream.h"
#include <set>
#include <vector>
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
            int Initialize(char const*);
            int Reinitialize(char const*);
            int AddFolder(char const*, char const*, bool);
            int RemoveFolder(char const*);
            int AddPackage(char const*);
            int AddFile(char const*);
            int RemoveFile(char const*);
            void EnableMapping(bool);
            void SetCurrentWorkDir(char const*);
            char const* GetCurrentWorkDir() const;
            virtual FileStream* CreateFileStream();
            int OpenFileStream(FileReader*, char const*, IStream::OpenFlags);
            virtual bool FileExists(char const*);
            void GetOpenFilesList(std::vector<CStr>&) const;
            CriticalSection& GetCriticalSecton();

        protected:
            void DecryptFileName(char const*, CStr&);
            int InternalAddPackage(CStr const&);
            int EnumDataFolderFiles(char const*);

        protected:
            bool m_Initialized = false;
            bool m_EnableMapping = false;
            CriticalSection m_cs;
            std::vector<ref_ptr<fs::Package>> m_Packages;
            std::set<CStr> m_Files;
            CStr m_CurrentWorkDir;
        };
    }
}

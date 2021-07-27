#pragma once
#include "i_stream.h"
#include <set>
#include <vector>
#include <core/ref_ptr.h>
#include <core/stringm3d.h>
#include <core/threadsync.h>

namespace m3d
{
    namespace fs
    {
        class Package;
        class FileReader;
        class FileStream;

        class FileServer
        {
        public:
            virtual ~FileServer();
            virtual bool FileExists(char const*);
            virtual FileStream* CreateFileStream();

            FileServer();
            int RemoveFile(char const*);
            void EnableMapping(bool);
            int Reinitialize(char const*);
            int Shutdown();
            CriticalSection& GetCriticalSecton();
            int OpenFileStream(FileReader*, char const*, IStream::OpenFlags);
            int AddPackage(char const*);
            int AddFile(char const*);
            char const* GetCurrentWorkDir() const;
            int RemoveFolder(char const*);
            int Initialize(char const*);
            void GetOpenFilesList(std::vector<CStr>&) const;
            int AddFolder(char const*, char const*, bool);
            void SetCurrentWorkDir(char const*);

        protected:
            void DecryptFileName(char const*, CStr&);
            int InternalAddPackage(CStr const&);

        private:
            //m3d::fs::FileServer_vtbl* __vftable /*VFT*/;
            bool m_Initialized;
            bool m_EnableMapping;
            CriticalSection m_cs;
            std::vector<ref_ptr<fs::Package>> m_Packages;
            std::set<CStr> m_Files;
            CStr m_CurrentWorkDir;
        };
    }
}

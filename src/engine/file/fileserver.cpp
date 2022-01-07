#include <stdexcept>
#include <file/fileserver.h>
#include <file/package.h>

namespace m3d
{
    namespace fs
    {
        FileServer::~FileServer()
        {
            throw std::logic_error("Not implemented");
        }

        bool FileServer::FileExists(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        FileStream* FileServer::CreateFileStream()
        {
            throw std::logic_error("Not implemented");
        }

        FileServer::FileServer()
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::RemoveFile(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        void FileServer::EnableMapping(bool)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::Reinitialize(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::Shutdown()
        {
            throw std::logic_error("Not implemented");
        }

        CriticalSection& FileServer::GetCriticalSecton()
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::OpenFileStream(FileReader*, char const*, IStream::OpenFlags)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::AddPackage(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::AddFile(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::RemoveFolder(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::Initialize(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::AddFolder(char const*, char const*, bool)
        {
            throw std::logic_error("Not implemented");
        }

        void FileServer::SetCurrentWorkDir(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        void FileServer::DecryptFileName(char const*, CStr&)
        {
            throw std::logic_error("Not implemented");
        }

        int FileServer::InternalAddPackage(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        char const* FileServer::GetCurrentWorkDir() const
        {
            return m_CurrentWorkDir.c_str();
        }

        void FileServer::GetOpenFilesList(std::vector<CStr>& fileList) const
        {
            fileList.clear();
            for (const auto& package : m_Packages)
            {
                std::vector<CStr> tmpList;
                package->GetOpenFilesList(tmpList);
                fileList.insert(end(fileList), begin(tmpList), end(tmpList));
            }
        }
    }
}

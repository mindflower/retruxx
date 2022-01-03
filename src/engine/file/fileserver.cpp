#include <file/fileserver.h>
#include <file/package.h>

namespace m3d
{
    namespace fs
    {
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

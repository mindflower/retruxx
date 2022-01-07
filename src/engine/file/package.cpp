#include <stdexcept>
#include <file/package.h>

namespace m3d
{
    namespace fs
    {
        PackageFileRecordHeader::PackageFileRecordHeader()
        {
            throw std::logic_error("Not implemented");
        }

        Package::Package(bool)
        {
            throw std::logic_error("Not implemented");
        }

        Package::~Package()
        {
            throw std::logic_error("Not implemented");
        }

        void Package::DecRef()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned Package::OpenFile(char const*, void**, unsigned*)
        {
            throw std::logic_error("Not implemented");
        }

        void Package::CloseFile(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int Package::Open(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        void Package::Close()
        {
            throw std::logic_error("Not implemented");
        }

        bool Package::HaveFile(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        void Package::GetFilesList(std::vector<CStr>&)
        {
            throw std::logic_error("Not implemented");
        }

        void Package::IncRef()
        {
            throw std::logic_error("Not implemented");
        }

        void Package::ClosePackageFile()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned Package::MakeView(unsigned, unsigned, unsigned*, void**)
        {
            throw std::logic_error("Not implemented");
        }

        void Package::LoadFileRecords(unsigned char*, int)
        {
            throw std::logic_error("Not implemented");
        }

        int Package::OpenPackageFile(char const*)
        {
            throw std::logic_error("Not implemented");
        }

        void Package::CloseView(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void Package::GetOpenFilesList(std::vector<CStr>& fileList) const
        {
            fileList.clear();
            //TODO: thats all?
        }
    }
}

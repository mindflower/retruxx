#include <stdexcept>
#include <file/package.h>

namespace m3d
{
    namespace fs
    {
        PackageFileRecordHeader::PackageFileRecordHeader()
        {
            throw retruxx::logic_error("Not implemented");
        }

        Package::Package(bool)
        {
            throw retruxx::logic_error("Not implemented");
        }

        Package::~Package()
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::DecRef()
        {
            throw retruxx::logic_error("Not implemented");
        }

        unsigned Package::OpenFile(char const*, void**, unsigned*)
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::CloseFile(unsigned)
        {
            throw retruxx::logic_error("Not implemented");
        }

        int Package::Open(char const*)
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::Close()
        {
            throw retruxx::logic_error("Not implemented");
        }

        bool Package::HaveFile(char const*)
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::GetFilesList(retruxx::vector<CStr>&)
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::IncRef()
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::ClosePackageFile()
        {
            throw retruxx::logic_error("Not implemented");
        }

        unsigned Package::MakeView(unsigned, unsigned, unsigned*, void**)
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::LoadFileRecords(unsigned char*, int)
        {
            throw retruxx::logic_error("Not implemented");
        }

        int Package::OpenPackageFile(char const*)
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::CloseView(unsigned)
        {
            throw retruxx::logic_error("Not implemented");
        }

        void Package::GetOpenFilesList(retruxx::vector<CStr>& fileList) const
        {
            fileList.clear();
            //TODO: thats all?
        }
    }
}

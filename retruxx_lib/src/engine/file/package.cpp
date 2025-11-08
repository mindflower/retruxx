#include <stdexcept>
#include <file/package.h>

namespace m3d
{
    namespace fs
    {
        PackageFileRecordHeader::PackageFileRecordHeader()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Package::Package(bool)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Package::~Package()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::DecRef()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned Package::OpenFile(char const*, void**, unsigned*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::CloseFile(unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int Package::Open(char const*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::Close()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        bool Package::HaveFile(char const*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::GetFilesList(retruxx::vector<CStr>&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::IncRef()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::ClosePackageFile()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        unsigned Package::MakeView(unsigned, unsigned, unsigned*, void**)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::LoadFileRecords(unsigned char*, int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int Package::OpenPackageFile(char const*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::CloseView(unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Package::GetOpenFilesList(retruxx::vector<CStr>& fileList) const
        {
            fileList.clear();
        }
    }
}

#pragma once

namespace m3d
{
    namespace fs
    {
        //TODO: add static functions

        class PackageFileRecordHeader
        {
        public:
            PackageFileRecordHeader();

        private:
            unsigned int RecordLengthInv;
            unsigned int FileDateHigh;
            unsigned int FileDateLow;
            bool IsArchived;
            unsigned int reserved1;
            unsigned int FilePositionDisp2;
            unsigned int FileSizeDisp2Inv;
            unsigned int RealSizeDisp1;
            unsigned int reserved2;
            int Stat1;
            int Stat2;
            int Stat3;
            unsigned int ExternalData;
        };
    }
}

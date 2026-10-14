#pragma once

namespace m3d
{
    namespace fs
    {
        struct PackageFileRecordHeader
        {
            unsigned int RecordLengthInv;
            unsigned long FileDateHigh;
            unsigned long FileDateLow;
            bool IsArchived;
            char Padding_57[3];
            unsigned long reserved1;
            unsigned int FilePositionDisp2;
            unsigned int FileSizeDisp2Inv;
            unsigned int RealSizeDisp1;
            unsigned long reserved2;
            int Stat1;
            int Stat2;
            int Stat3;
            unsigned int ExternalData;

            PackageFileRecordHeader();
            PackageFileRecordHeader(
                unsigned int,
                unsigned long,
                unsigned long,
                bool,
                unsigned int,
                unsigned int,
                unsigned int);
        };
    }  // namespace fs
}  // namespace m3d

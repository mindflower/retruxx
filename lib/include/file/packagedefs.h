#pragma once

namespace m3d
{
    namespace fs
    {
        struct PackageFileRecordHeader
        {
            /* 0x0000 */ unsigned int RecordLengthInv;
            /* 0x0004 */ unsigned long FileDateHigh;
            /* 0x0008 */ unsigned long FileDateLow;
            /* 0x000c */ bool IsArchived;
            /* 0x000d */ char Padding_57[3];
            /* 0x0010 */ unsigned long reserved1;
            /* 0x0014 */ unsigned int FilePositionDisp2;
            /* 0x0018 */ unsigned int FileSizeDisp2Inv;
            /* 0x001c */ unsigned int RealSizeDisp1;
            /* 0x0020 */ unsigned long reserved2;
            /* 0x0024 */ int Stat1;
            /* 0x0028 */ int Stat2;
            /* 0x002c */ int Stat3;
            /* 0x0030 */ unsigned int ExternalData;

            PackageFileRecordHeader();
            PackageFileRecordHeader(
                unsigned int,
                unsigned long,
                unsigned long,
                bool,
                unsigned int,
                unsigned int,
                unsigned int);
        }; /* size: 0x0034 */
    }  // namespace fs
}  // namespace m3d

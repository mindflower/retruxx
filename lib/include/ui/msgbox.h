#pragma once

class CStr;

namespace m3d
{
    namespace ui
    {
        enum MbRetCodes
        {
            MBX_RET_OK = 0x1,
            MBX_RET_YES = 0x1,
            MBX_RET_NO = 0x2,
            MBX_RET_CANCEL = 0x3,
            MBX_RET_USER = 0x4,
        };

        MbRetCodes __fastcall RunMsgBoxDlg(CStr const& caption, CStr const& message, unsigned int flags, bool pause);
    }
}

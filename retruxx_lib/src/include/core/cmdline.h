#pragma once
#include "stringm3d.h"
#include "retruxx/common.h"

namespace m3d
{
    class CmdLine
    {
    public:
        unsigned int Init(const char* cmdStr);
        bool CheckParam(const char* param) const;
        unsigned int GetNumParams() const;
        const CStr& GetParam(unsigned int) const;

    private:
        /* 0x0000 */ CStr m_cmdLine;
        /* 0x000c */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_params;
    }; /* size: 0x001c */

    static_assert(sizeof(CmdLine) == 0x001c);
}

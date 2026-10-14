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
        CStr m_cmdLine;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_params;
    };
}

#pragma once
#include "stringm3d.h"
#include "retruxx/common.h"

namespace m3d
{
    class CmdLine
    {
    public:
        unsigned int Init(char const* cmdStr);
        bool CheckParam(char const* param) const;
        unsigned int GetNumParams() const;
        CStr const& GetParam(unsigned int) const;

    private:
        CStr m_cmdLine;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_params;
    };
}  // namespace m3d

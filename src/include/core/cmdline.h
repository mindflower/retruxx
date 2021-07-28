#pragma once
#include "stringm3d.h"
#include <vector>

namespace m3d
{
    class CmdLine
    {
    public:
        unsigned int Init(char const* cmdStr);
        bool CheckParam(char const* param) const;

    private:
        CStr m_cmdLine;
        std::vector<CStr> m_params;
    };
}

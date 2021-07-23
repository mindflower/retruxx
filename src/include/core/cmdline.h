#pragma once
#include "stringm3d.h"
#include <vector>

namespace m3d
{
    class CmdLine
    {
    public:
        bool CheckParam(char const*) const;
        unsigned int Init(char const*);

    private:
        CStr m_cmdLine;
        std::vector<CStr> m_params;
    };
}

#include <core/cmdline.h>

namespace m3d
{
    unsigned int CmdLine::Init(char const* cmdStr)
    {
        // RVA 0x5A6750 - splits the command line into parameters at control characters, spaces, DEL and bytes
        // above 127 (signed chars).
        m_cmdLine = cmdStr;
        m_params.clear();
        auto const isSeparator = [](char c) { return c <= ' ' || c == 127; };
        for (char const* c = cmdStr; *c;)
        {
            if (isSeparator(*c))
            {
                ++c;
                continue;
            }
            CStr param;
            for (; *c && !isSeparator(*c); ++c)
            {
                char const one[2] = {*c, 0};
                param += one;
            }
            if (!param.empty())
            {
                m_params.push_back(param);
            }
            if (*c)
            {
                ++c;
            }
        }
        return m_params.size();
    }

    bool CmdLine::CheckParam(char const* param) const
    {
        for (auto const& p : m_params)
        {
            if (p == param)
            {
                return true;
            }
        }
        return false;
    }
}

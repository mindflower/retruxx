#include <core/cmdline.h>

namespace m3d
{
    unsigned int CmdLine::Init(char const* cmdStr)
    {
        //TODO: check correctness
        m_cmdLine = cmdStr;
        for (size_t i = 0; i < m_cmdLine.size(); ++i)
        {
            CStr param;
            auto const first = m_cmdLine[i];
            if (first > 32 && first < 127)
            {
                for (size_t j = i; j < m_cmdLine.size(); ++j)
                {
                    auto const second = m_cmdLine[j];
                    if (second > 32 && second < 127)
                    {
                        param += second;
                    }
                    else
                    {
                        i = j;
                        break;
                    }
                }
            }
            m_params.push_back(std::move(param));
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

#include "core/console/console.h"
#include <stdexcept>

namespace m3d
{
    void CConsoleParams::Set(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    char const* CConsoleParams::UnsafeStringToken(int, char) const
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams::~CConsoleParams()
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams& CConsoleParams::operator=(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int CConsoleParams::IntToken(int, char) const
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams::CConsoleParams(CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams::CConsoleParams(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams::CConsoleParams(int)
    {
        throw std::logic_error("Not implemented");
    }

    int CConsoleParams::NumOfTokens(char) const
    {
        throw std::logic_error("Not implemented");
    }

    char* CConsoleParams::StringToken(int, char*, int, char) const
    {
        throw std::logic_error("Not implemented");
    }

    float CConsoleParams::FloatToken(int, char) const
    {
        throw std::logic_error("Not implemented");
    }
}

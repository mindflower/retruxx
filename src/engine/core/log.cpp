#include <stdexcept>
#include <core/log.h>

namespace m3d
{
    Log::Log()
    {
        throw std::logic_error("Not implemented");
    }

    Log::~Log()
    {
        throw std::logic_error("Not implemented");
    }

    void Log::logTex(CStr const&, eLogFlags)
    {
        throw std::logic_error("Not implemented");
    }

    char const* Log::getSourceFile() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned& Log::logMask()
    {
        throw std::logic_error("Not implemented");
    }

    void Log::setSourceFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Log::logStarted() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Log::endLog()
    {
        throw std::logic_error("Not implemented");
    }

    bool const& Log::lineCharsFlag() const
    {
        throw std::logic_error("Not implemented");
    }

    bool& Log::lineCharsFlag()
    {
        throw std::logic_error("Not implemented");
    }

    void Log::logRaw(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Log::logHex(char const*, unsigned, eLogFlags)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned& Log::sourceLine()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned const& Log::sourceLine() const
    {
        throw std::logic_error("Not implemented");
    }

    void Log::indent(CStr const&, eLogFlags)
    {
        throw std::logic_error("Not implemented");
    }

    void Log::undent(CStr const&, eLogFlags)
    {
        throw std::logic_error("Not implemented");
    }

    bool Log::startLog(char const*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Log::headerString(eLogFlags) const
    {
        throw std::logic_error("Not implemented");
    }
}

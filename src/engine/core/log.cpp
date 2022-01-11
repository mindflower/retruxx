#include <fstream>
#include <stdexcept>
#include <core/log.h>

namespace m3d
{
    Log::~Log()
    {
        endLog();
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

    bool Log::startLog(char const* fileName, bool flush)
    {
        if (m_logStarted)
        {
            return true;
        }
        m_flushImmediately = flush;

        char buf[0x400] = { 0 };
        ::GetCurrentDirectoryA(sizeof(buf), buf);
        CStr logFile(buf);
        logFile += "\\";
        logFile += fileName;
        UnifyFileName(logFile);
        std::ofstream logStream(logFile.c_str());
        if (logStream)
        {
            auto const timestamp = time(NULL);
            CStr timeStr = asctime(localtime(&timestamp));
            timeStr[timeStr.length() - 1] = '\0';
            logStream <<
                "---------------------------------------------- Log begins on " <<
                timeStr.c_str() <<
                " ----------------------------------------------" <<
                std::endl;
            if (m_flushImmediately)
            {
                logStream.flush();
            }
            m_logStarted = true;
            return 1;
        }
        return 0;
    }

    CStr const& Log::headerString(eLogFlags) const
    {
        throw std::logic_error("Not implemented");
    }
}

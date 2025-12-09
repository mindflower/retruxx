#include <fstream>
#include <sstream>
#include <stdexcept>
#include <core/log.h>

namespace m3d
{
    Log::Log()
    {
    }

    unsigned const& Log::logMask() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Log::~Log()
    {
        endLog();
    }

    void Log::logTex(CStr const& s, eLogFlags logFlags)
    {
        AutoLock guard(m_cs);
        if (m_logStarted)
        {
            if ((logFlags & m_logMask) != 0)
            {
                std::ofstream file(m_fileName.c_str(), std::ios_base::app);
                if (file)
                {
                    auto const header = headerString(logFlags);
                    file << header.c_str() << s.c_str() << std::endl;
                    if (m_flushImmediately)
                    {
                        file.flush();
                    }
                }
            }
        }
    }

    char const* Log::getSourceFile() const
    {
        return m_fileName.c_str();
    }

    unsigned& Log::logMask()
    {
        return m_logMask;
    }

    void Log::setSourceFile(char const* file)
    {
        m_sourceFile = file;
    }

    bool Log::logStarted() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Log::endLog()
    {
        AutoLock guard(m_cs);
        if (!m_logStarted)
        {
            return true;
        }

        std::ofstream logStream(m_fileName.c_str(), std::ios_base::app);
        if (logStream)
        {
            auto const timestamp = time(NULL);
            CStr timeStr = asctime(localtime(&timestamp));
            timeStr[timeStr.length() - 1] = '\0';
            logStream <<
                "----------------------------------------------- Log ends on " <<
                timeStr.c_str() <<
                " ----------------------------------------------" <<
                std::endl;
            if (m_flushImmediately)
            {
                logStream.flush();
            }
        }
        m_logStarted = false;
        return true;
    }

    bool const& Log::lineCharsFlag() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool& Log::lineCharsFlag()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Log::logRaw(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Log::logHex(char const*, unsigned, eLogFlags)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned& Log::sourceLine()
    {
        return m_sourceLine;
    }

    unsigned const& Log::sourceLine() const
    {
        return m_sourceLine;
    }

    void Log::indent(CStr const& s, eLogFlags logBits)
    {
        AutoLock guard(m_cs);
        if (m_logStarted && (logBits & m_logMask) != 0)
        {
            std::ofstream logStream(m_fileName.c_str(), std::ios_base::app);
            if (logStream)
            {
                auto const header = headerString(logBits);
                logStream << header.c_str() << " +- " << s.c_str() << std::endl;
                m_indentCount += m_indentChars;
                if (m_flushImmediately)
                {
                    logStream.flush();
                }
            }
        }
    }

    void Log::undent(CStr const& s, eLogFlags logBits)
    {
        AutoLock guard(m_cs);
        if (m_logStarted && (logBits & m_logMask) != 0)
        {
            m_indentCount -= m_indentChars;
            if (m_indentCount < 0)
            {
                m_indentCount = 0;
            }
            std::ofstream logStream(m_fileName.c_str(), std::ios_base::app);
            if (logStream)
            {
                auto const header = headerString(logBits);
                logStream << header.c_str() << " +- " << s.c_str() << std::endl;
                m_indentCount += m_indentChars;
                if (m_flushImmediately)
                {
                    logStream.flush();
                }
            }
        }
    }

    bool Log::startLog(char const* fileName, bool flush)
    {
        AutoLock guard(m_cs);
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
        m_fileName = logFile.c_str();
        std::ofstream logStream(m_fileName.c_str());
        if (logStream)
        {
            auto const timestamp = time(NULL);
            CStr timeStr = asctime(localtime(&timestamp));
            timeStr[timeStr.length() - 1] = '\0';
            logStream <<
                "---------------------------------------------- Retruxx log begins on " <<
                timeStr.c_str() <<
                " ----------------------------------------------" <<
                std::endl;
            if (m_flushImmediately)
            {
                logStream.flush();
            }
            m_logStarted = true;
            return true;
        }
        return false;
    }

    CStr const& Log::headerString(eLogFlags logFlags) const
    {
        AutoLock guard(m_cs);
        std::stringstream ss;
        switch (logFlags)
        {
        case LOG_ALL: ss << "A "; break;
        case LOG_INDENT: ss << "> "; break;
        case LOG_UNDENT: ss << "< "; break;
        case LOG_FLOW: ss << "F "; break;
        case LOG_DATA: ss << "D "; break;
        case LOG_INFO: ss << "I "; break;
        case LOG_WARN: ss << "W "; break;
        case LOG_ERR: ss << "E "; break;
        case LOG_CRIT: ss << "! "; break;
        default: ss << "  "; break;
        }
        auto pos = m_sourceFile.rfind('\\');
        if (pos == std::string::npos)
        {
            pos = 0;
        }
        else
        {
            pos += 1;
        }
        char temp[1024] = { 0 };
        sprintf(temp, "%20s[%04d]", m_sourceFile.substr(pos).c_str(), m_sourceLine);
        ss << temp;

        auto const curTime = time(NULL);
        auto const localTime = localtime(&curTime);
        sprintf(temp, "%02d/%02d %02d:%02d:%02d ", localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
        ss << temp;

        static thread_local CStr header;
        header = ss.str().c_str();
        return header;
    }
}

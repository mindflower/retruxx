#pragma once
#include "threadsync.h"
#include <string>

class CStr;

namespace m3d
{
    enum eLogFlags
    {
        LOG_INDENT = 0x1,
        LOG_UNDENT = 0x2,
        LOG_FLOW = 0x4,
        LOG_BLOK = 0x8,
        LOG_DATA = 0x10,
        LOG_INFO = 0x12,
        LOG_WARN = 0x14,
        LOG_ERR = 0x18,
        LOG_CRIT = 0x20,
        LOG_ALL = 0xFFFFFFFF,
    };

    class Log
    {
    public:
        Log();
        ~Log();

        void logTex(CStr const&, eLogFlags);
        char const* getSourceFile() const;
        unsigned int& logMask();
        void setSourceFile(char const*);
        bool logStarted() const;
        bool endLog();
        bool const& lineCharsFlag() const;
        bool& lineCharsFlag();
        void logRaw(char const*);
        void logHex(char const*, unsigned int, eLogFlags);
        unsigned int& sourceLine();
        unsigned int const& sourceLine() const;
        void indent(CStr const&, eLogFlags);
        void undent(CStr const&, eLogFlags);
        bool startLog(char const*, bool);

    private:
        CStr const& headerString(eLogFlags) const;

    private:
        bool m_logStarted;
        std::string m_fileName;
        unsigned int m_sourceLine;
        std::string m_sourceFile;
        unsigned int m_logMask;
        int m_indentCount;
        int m_indentChars;
        bool m_lineCharsFlag;
        bool m_flushImmediately;
        CriticalSection m_cs;
    };
}

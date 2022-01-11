#pragma once
#include "stringm3d.h"
#include "threadsync.h"
#include <string>

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
        bool m_logStarted = false;
        std::string m_fileName;
        unsigned int m_sourceLine = 0;
        std::string m_sourceFile;
        unsigned int m_logMask = -1;
        int m_indentCount = 0;
        int m_indentChars = 4;
        bool m_lineCharsFlag = false;
        bool m_flushImmediately = true;
        CriticalSection m_cs;
    };
}

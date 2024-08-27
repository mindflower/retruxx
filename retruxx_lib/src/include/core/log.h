#pragma once
#include "stringm3d.h"
#include "threadsync.h"
#include <string>
#include "kernel.h"

#define M3D_LOG_INFO(msg) m3d::g_Kernel->m_Log->sourceLine() = __LINE__; m3d::g_Kernel->m_Log->setSourceFile(__FILE__); m3d::g_Kernel->m_Log->logTex(msg, m3d::LOG_INFO)
#define M3D_LOG_ERR(msg) m3d::g_Kernel->m_Log->sourceLine() = __LINE__; m3d::g_Kernel->m_Log->setSourceFile(__FILE__); m3d::g_Kernel->m_Log->logTex(msg, m3d::LOG_ERR)
#define M3D_LOG_WARN(msg) m3d::g_Kernel->m_Log->sourceLine() = __LINE__; m3d::g_Kernel->m_Log->setSourceFile(__FILE__); m3d::g_Kernel->m_Log->logTex(msg, m3d::LOG_WARN)
#define M3D_LOG_DATA(msg) m3d::g_Kernel->m_Log->sourceLine() = __LINE__; m3d::g_Kernel->m_Log->setSourceFile(__FILE__); m3d::g_Kernel->m_Log->logTex(msg, m3d::LOG_DATA)
#define M3D_LOG_FLOW() m3d::g_Kernel->m_Log->sourceLine() = __LINE__; m3d::g_Kernel->m_Log->setSourceFile(__FILE__); m3d::Kernel::auxLogFlow _(__FUNCTION__)

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
        void ctor();
        void dtor();

        bool startLog(char const*, bool);
        bool endLog();
        bool logStarted() const;
        unsigned int& sourceLine();
        unsigned int const& sourceLine() const;
        char const* getSourceFile() const;
        void setSourceFile(char const*);
        unsigned int& logMask();
        unsigned int const& logMask() const;
        bool& lineCharsFlag();
        bool const& lineCharsFlag() const;
        void logTex(CStr const&, eLogFlags);
        void logRaw(char const*);
        void logHex(char const*, unsigned int, eLogFlags);
        void indent(CStr const&, eLogFlags);
        void undent(CStr const&, eLogFlags);

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

    public:
        CStr const& headerString(eLogFlags) const;

    private:
        mutable CriticalSection m_cs;
    };
}

#pragma once
#include "stringm3d.h"
#include "threadsync.h"
#include "kernel.h"
#include "retruxx/common.h"

#define M3D_LOG_INFO(msg)                       \
    M3D_KERNEL->m_Log->sourceLine() = __LINE__; \
    M3D_KERNEL->m_Log->setSourceFile(__FILE__); \
    M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_INFO)
#define M3D_LOG_ERR(msg)                        \
    M3D_KERNEL->m_Log->sourceLine() = __LINE__; \
    M3D_KERNEL->m_Log->setSourceFile(__FILE__); \
    M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_ERR)
#define M3D_LOG_WARN(msg)                       \
    M3D_KERNEL->m_Log->sourceLine() = __LINE__; \
    M3D_KERNEL->m_Log->setSourceFile(__FILE__); \
    M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_WARN)
#define M3D_LOG_DATA(msg)                       \
    M3D_KERNEL->m_Log->sourceLine() = __LINE__; \
    M3D_KERNEL->m_Log->setSourceFile(__FILE__); \
    M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_DATA)
#define M3D_LOG_FLOW()                          \
    M3D_KERNEL->m_Log->sourceLine() = __LINE__; \
    M3D_KERNEL->m_Log->setSourceFile(__FILE__); \
    m3d::Kernel::auxLogFlow _(__FUNCTION__)
#define M3D_LOG_CRIT(msg)                       \
    M3D_KERNEL->m_Log->sourceLine() = __LINE__; \
    M3D_KERNEL->m_Log->setSourceFile(__FILE__); \
    M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_CRIT);

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

    class m3d::Log
    {
    public:
        Log();
        ~Log();
        bool startLog(char const* fileName, bool flush);
        bool endLog();
        bool logStarted() const;
        unsigned int& sourceLine();
        unsigned int const& sourceLine() const;
        char const* getSourceFile() const;
        void setSourceFile(char const* value);
        unsigned int& logMask();
        unsigned int const& logMask() const;
        bool& lineCharsFlag();
        bool const& lineCharsFlag() const;
        void logTex(CStr const& s, m3d::eLogFlags const logFlags);
        void logRaw(char const* s);
        void logHex(char const* buffer, unsigned int const count, m3d::eLogFlags const logFlags);
        void indent(CStr const& s, m3d::eLogFlags const logBits);
        void undent(CStr const& s, m3d::eLogFlags const logBits);

    private:
        bool m_logStarted = false;
        retruxx::string m_fileName;
        unsigned int m_sourceLine = 0;
        retruxx::string m_sourceFile;
        unsigned int m_logMask = -1;
        int m_indentCount = 0;
        int m_indentChars = 4;
        bool m_lineCharsFlag = false;
        bool m_flushImmediately = true;

        CStr const& headerString(m3d::eLogFlags const logFlags) const;

        mutable m3d::CriticalSection m_cs;
    };
}  // namespace m3d

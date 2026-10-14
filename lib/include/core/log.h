#pragma once
#include "stringm3d.h"
#include "threadsync.h"
#include "kernel.h"
#include "retruxx/common.h"

#define M3D_LOG_INFO(msg) M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_INFO)
#define M3D_LOG_ERR(msg)  M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_ERR)
#define M3D_LOG_WARN(msg) M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_WARN)
#define M3D_LOG_DATA(msg) M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_DATA)
#define M3D_LOG_FLOW()    M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); m3d::Kernel::auxLogFlow _(__FUNCTION__)
#define M3D_LOG_CRIT(msg)  M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_CRIT);

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
        bool startLog(const char* fileName, bool flush);
        bool endLog();
        bool logStarted() const;
        unsigned int& sourceLine();
        const unsigned int& sourceLine() const;
        const char* getSourceFile() const;
        void setSourceFile(const char* value);
        unsigned int& logMask();
        const unsigned int& logMask() const;
        bool& lineCharsFlag();
        const bool& lineCharsFlag() const;
        void logTex(const CStr& s, const m3d::eLogFlags logFlags);
        void logRaw(const char* s);
        void logHex(const char* buffer, const unsigned int count, const m3d::eLogFlags logFlags);
        void indent(const CStr& s, const m3d::eLogFlags logBits);
        void undent(const CStr& s, const m3d::eLogFlags logBits);

    private:
        bool m_logStarted = false;
        char Padding_79[3];
        retruxx::string m_fileName;
        unsigned int m_sourceLine = 0;
        retruxx::string m_sourceFile;
        unsigned int m_logMask = -1;
        int m_indentCount = 0;
        int m_indentChars = 4;
        bool m_lineCharsFlag = false;
        bool m_flushImmediately = true;

        const CStr& headerString(const m3d::eLogFlags logFlags) const;

        char Padding_80[2];
        mutable m3d::CriticalSection m_cs;
    };
}

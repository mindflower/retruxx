#pragma once
#include "stringm3d.h"
#include "threadsync.h"
#include "kernel.h"
#include "thirdparty/containers.h"
#include "thirdparty/injecttools.h"

#define M3D_LOG_INFO(msg) M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_INFO)
#define M3D_LOG_ERR(msg)  M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_ERR)
#define M3D_LOG_WARN(msg) M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_WARN)
#define M3D_LOG_DATA(msg) M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); M3D_KERNEL->m_Log->logTex(msg, m3d::LOG_DATA)
#define M3D_LOG_FLOW()    M3D_KERNEL->m_Log->sourceLine() = __LINE__; M3D_KERNEL->m_Log->setSourceFile(__FILE__); m3d::Kernel::auxLogFlow _(__FUNCTION__)

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
        RETRUXX_DLL_FRIEND_CLASS(Log);

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
        /* 0x0000 */ bool m_logStarted = false;
        /* 0x0001 */ char Padding_79[3];
        /* 0x0004 */ retruxx::string m_fileName;
        /* 0x0020 */ unsigned int m_sourceLine = 0;
        /* 0x0024 */ retruxx::string m_sourceFile;
        /* 0x0040 */ unsigned int m_logMask = -1;
        /* 0x0044 */ int m_indentCount = 0;
        /* 0x0048 */ int m_indentChars = 4;
        /* 0x004c */ bool m_lineCharsFlag = false;
        /* 0x004d */ bool m_flushImmediately = true;

        const CStr& headerString(const m3d::eLogFlags logFlags) const;

        /* 0x004e */ char Padding_80[2];
        /* 0x0050 */ mutable m3d::CriticalSection m_cs;
    }; /* size: 0x0068 */

    static_assert(sizeof(Log) == 0x0068);
}

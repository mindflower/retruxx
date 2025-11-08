#include <stdexcept>
#include <core/threadsync.h>

namespace m3d
{
    CriticalSection::CriticalSection()
    {
        ::InitializeCriticalSection(&m_cs);
    }

    CriticalSection::~CriticalSection()
    {
        ::DeleteCriticalSection(&m_cs);
    }

    void CriticalSection::Lock()
    {
        ::EnterCriticalSection(&m_cs);
    }

    void CriticalSection::Unlock()
    {
        ::LeaveCriticalSection(&m_cs);
    }
}

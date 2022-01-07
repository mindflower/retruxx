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
        throw std::logic_error("Not implemented");
    }

    void CriticalSection::Unlock()
    {
        throw std::logic_error("Not implemented");
    }
}

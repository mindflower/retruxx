#pragma once
//TODO: windows.h
#include <Windows.h>

namespace m3d
{
    class CriticalSection
    {
    public:
        CriticalSection();
        ~CriticalSection();
        void Lock();
        void Unlock();

    private:
        CRITICAL_SECTION m_cs;
    };

    template<class T>
    class AutoLock
    {
    public:
        AutoLock(T&)
        {
            throw std::logic_error("Not implemented");
        }

        ~AutoLock()
        {
            throw std::logic_error("Not implemented");
        }

    private:
        T* m_lockObject;
    };
}

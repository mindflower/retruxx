#pragma once
#include <cassert>

template<class T>
class scoped_ptr
{
public:
    scoped_ptr(T* ptr) :
        m_ptr(ptr)
    {
        assert(nullptr != m_ptr);
    }

    ~scoped_ptr()
    {
        delete m_ptr;
    }

    T& operator*()
    {
        assert(nullptr != m_ptr);
        return *m_ptr;
    }

    T* operator->()
    {
        assert(nullptr != m_ptr);
        return m_ptr;
    }

private:
    T* m_ptr = nullptr;
};

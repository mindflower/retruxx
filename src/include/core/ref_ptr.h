#pragma once
#include <cassert>

template<class T>
class ref_ptr
{
public:
    ref_ptr() = default;

    ref_ptr(T* ptr) :
        m_ptr(ptr)
    {
        if (m_ptr)
        {
            m_ptr->IncRef();
        }
    }

    ~ref_ptr()
    {
        if (m_ptr)
        {
            m_ptr->DecRef();
        }
    }

    operator bool()
    {
        return m_ptr != nullptr;
    }

    bool operator!() const
    {
        return m_ptr == nullptr;
    }

    operator T*() const
    {
        assert(nullptr != m_ptr);
        return m_ptr;
    }

    bool operator==(ref_ptr<T> const& rhs) const
    {
        return m_ptr == rhs.m_ptr;
    }

    T* operator->() const
    {
        assert(nullptr != m_ptr);
        return m_ptr;
    }

private:
    T* m_ptr = nullptr;
};

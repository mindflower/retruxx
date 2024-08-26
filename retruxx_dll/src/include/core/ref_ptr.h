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

    ref_ptr(ref_ptr<T> const& rhs) :
        m_ptr(rhs.m_ptr)
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

    ref_ptr<T>& operator=(ref_ptr<T> const& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        if (m_ptr)
        {
            m_ptr->DecRef();
        }
        if (rhs.m_ptr)
        {
            rhs.m_ptr->IncRef();
        }
        m_ptr = rhs.m_ptr;
        return *this;
    }

private:
    T* m_ptr = nullptr;
};

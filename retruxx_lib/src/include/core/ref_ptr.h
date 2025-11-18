#pragma once
#include <cassert>

template<class T>
class ref_ptr
{
public:
    ref_ptr() = default;

    ref_ptr(T* ptr) : m_ptr(ptr)
    {
        if (m_ptr)
        {
            m_ptr->IncRef();
        }
    }

    ref_ptr(const ref_ptr<T>& rhs) : m_ptr(rhs.m_ptr)
    {
        if (m_ptr)
        {
            m_ptr->IncRef();
        }
    }

    ref_ptr(ref_ptr<T>&& rhs) noexcept : m_ptr(rhs.m_ptr)
    {
        rhs.m_ptr = nullptr;
    }

    ref_ptr& operator=(const ref_ptr<T>& rhs)
    {
        if (m_ptr == rhs.m_ptr)
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

    ref_ptr& operator=(ref_ptr<T>&& rhs) noexcept
    {
        if (m_ptr != rhs.m_ptr)
        {
            if (m_ptr)
            {
                m_ptr->DecRef();
            }
            m_ptr = rhs.m_ptr;
            rhs.m_ptr = nullptr;
        }
        return *this;
    }

    ~ref_ptr()
    {
        if (m_ptr)
        {
            m_ptr->DecRef();
        }
    }

    const T* get() const
    {
        return m_ptr;
    }

    T* get()
    {
        return m_ptr;
    }

    operator const T*() const
    {
        return m_ptr;
    }

    operator T*()
    {
        return m_ptr;
    }

    const T* operator->() const
    {
        assert(nullptr != m_ptr);
        return m_ptr;
    }

    T* operator->()
    {
        assert(nullptr != m_ptr);
        return m_ptr;
    }

    bool operator==(const T* rhs) const
    {
        return m_ptr == rhs;
    }

    bool operator==(const ref_ptr<T>& rhs) const
    {
        return m_ptr == rhs.m_ptr;
    }

    bool operator!() const
    {
        return m_ptr == nullptr;
    }

private:
    T* m_ptr = nullptr;
};

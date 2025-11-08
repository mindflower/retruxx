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

    ref_ptr(ref_ptr<T>&& rhs) noexcept :
        m_ptr(rhs.m_ptr)
    {
        rhs.m_ptr = nullptr;
    }

    ref_ptr& operator=(ref_ptr<T> const& rhs)
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

#pragma once
#include <cassert>

template<class T>
class scoped_ptr
{
public:
    scoped_ptr() : m_ptr(nullptr)
    {
    }

    scoped_ptr(T* ptr) : m_ptr(ptr)
    {
        assert(nullptr != m_ptr);
    }

    scoped_ptr(scoped_ptr<T> const&) = delete;
    scoped_ptr(scoped_ptr<T>&& rhs) noexcept : m_ptr(rhs.m_ptr)
    {
        rhs.m_ptr = nullptr;
    }

    scoped_ptr& operator=(scoped_ptr<T> const&) = delete;
    scoped_ptr& operator=(scoped_ptr<T>&& rhs) noexcept
    {
        if (m_ptr != rhs.m_ptr)
        {
            delete m_ptr;
            m_ptr = rhs.m_ptr;
            rhs.m_ptr = nullptr;
        }
        return *this;
    }

    ~scoped_ptr()
    {
        delete m_ptr;
    }

    T& operator*() const
    {
        assert(nullptr != m_ptr);
        return *m_ptr;
    }

    T* operator->() const
    {
        assert(nullptr != m_ptr);
        return m_ptr;
    }

    operator T*()
    {
        assert(nullptr != m_ptr);
        return m_ptr;
    }

    operator T const*() const
    {
        assert(nullptr != m_ptr);
        return m_ptr;
    }

private:
    T* m_ptr = nullptr;
};

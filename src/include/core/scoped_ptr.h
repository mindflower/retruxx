#pragma once

template<class T>
class scoped_ptr
{
public:
    T& operator*();
    T* operator->();

private:
    T* m_ptr;
};

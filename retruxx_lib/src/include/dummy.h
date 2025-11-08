#pragma once

template<class T>
class Dummy
{
public:
    Dummy(T const &);
    operator T *();

private:
    T m_value;
};

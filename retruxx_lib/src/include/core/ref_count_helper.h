#pragma once
#include "clazz.h"

template<class T>
class ref_count_helper : public m3d::RefCountedBase
{
public:
    const T& GetObjectA() const;
    T& GetObjectA();

private:
    T m_obj;
};

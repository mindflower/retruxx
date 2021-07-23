#pragma once
#include "clazz.h"

template<class T>
class ref_count_helper : public m3d::RefCountedBase
{
private:
    T m_obj;
};

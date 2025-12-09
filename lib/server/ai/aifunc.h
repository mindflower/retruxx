#pragma once
#include <core/stringm3d.h>

namespace m3d
{
    class AIParam;
}

namespace ai
{
    class Obj;

    
    class AIFunc
    {
    private:
        /* 0x0000 */ m3d::AIParam (*m_funcAction)(ai::Obj*);
        /* 0x0004 */ CStr m_name;

    public:
        /* 0x0010 */ long m_CallNum;
        /* 0x0014 */ float m_MaxTime;
        /* 0x0018 */ float m_SumTime;
        AIFunc(ai::AIFunc const& __that);
        AIFunc(CStr const& Name, m3d::AIParam (*funcAction)(ai::Obj*));
        AIFunc();
        void Set(CStr const& Name, m3d::AIParam (*funcAction)(ai::Obj*));
        m3d::AIParam Action(ai::Obj* pObj);
        CStr const& GetName() const;
    }; /* size: 0x001c */
}

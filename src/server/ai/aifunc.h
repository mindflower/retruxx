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
    public:
        void Set(CStr const &,m3d::AIParam (__fastcall*)(Obj *));
        AIFunc(AIFunc const &);
        AIFunc();
        CStr const & GetName() const ;
        m3d::AIParam Action(Obj *);

    private:
        m3d::AIParam *(__fastcall *m_funcAction)(m3d::AIParam *result, Obj *);
        CStr m_name;
        int m_CallNum;
        float m_MaxTime;
        float m_SumTime;
    };
}

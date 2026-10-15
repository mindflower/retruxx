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
        m3d::AIParam (*m_funcAction)(ai::Obj*);
        CStr m_name;

    public:
        long m_CallNum;
        float m_MaxTime;
        float m_SumTime;
        AIFunc(ai::AIFunc const& __that);
        AIFunc(CStr const& Name, m3d::AIParam (*funcAction)(ai::Obj*));
        AIFunc();
        void Set(CStr const& Name, m3d::AIParam (*funcAction)(ai::Obj*));
        m3d::AIParam Action(ai::Obj* pObj);
        CStr const& GetName() const;
    };
}  // namespace ai

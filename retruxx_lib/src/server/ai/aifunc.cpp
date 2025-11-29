#include "aifunc.h"

#include <stdexcept>

#include "core/aiparam.h"

namespace ai
{
    void AIFunc::Set(CStr const& name, m3d::AIParam(*funcAction)(Obj*))
    {
        m_name = name;
        m_funcAction = funcAction;
    }

    AIFunc::AIFunc(CStr const& name, m3d::AIParam(*funcAction)(Obj*))
    {
        Set(name, funcAction);
    }

    AIFunc::AIFunc(AIFunc const& rhd) :
        m_funcAction(rhd.m_funcAction),
        m_name(rhd.m_name),
        m_CallNum(rhd.m_CallNum),
        m_MaxTime(rhd.m_MaxTime),
        m_SumTime(rhd.m_SumTime)
    {
    }

    AIFunc::AIFunc()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& AIFunc::GetName() const
    {
        return m_name;
    }

    m3d::AIParam AIFunc::Action(Obj* pObj)
    {
        return m_funcAction(pObj);
    }
}

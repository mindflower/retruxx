#include "aisignal.h"

namespace ai
{
    AISignal::AISignal()
    {
        m_FuncNum = 0xFFFF;
    }

    void AISignal::Set(const CStr& Name, int FuncNum)
    {
        m_name = Name;
        m_FuncNum = FuncNum;
        m_ParamRefList.clear();
    }

    void AISignal::Set(const CStr& Name)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AISignal::Dump() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const CStr& AISignal::GetName() const
    {
        return m_name;
    }
}

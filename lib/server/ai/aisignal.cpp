#include "aisignal.h"

#include <cstdio>

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

    // RVA 0x906430
    void AISignal::Set(const CStr& Name)
    {
        m_name = Name;
        m_ParamRefList.clear();
        m_FuncNum = 0xFFFF;
    }

    // RVA 0x906410
    void AISignal::Dump() const
    {
        printf("%s", m_name.c_str());
    }

    const CStr& AISignal::GetName() const
    {
        return m_name;
    }
}

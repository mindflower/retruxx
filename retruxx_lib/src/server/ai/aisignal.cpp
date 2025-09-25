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
        throw std::logic_error("Not implemented");
    }

    void AISignal::Dump() const
    {
        throw std::logic_error("Not implemented");
    }

    const CStr& AISignal::GetName() const
    {
        return m_name;
    }
}

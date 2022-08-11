#include <stdexcept>
#include <core/debugcounter.h>
#include <core/stringm3d.h>

namespace m3d
{
    void DbgCounter::SetI(int i)
    {
        m_curType - DBG_COUNTER_INT;
        m_i = i;
    }

    DbgCounter::eType DbgCounter::GetType() const
    {
        throw std::logic_error("Not implemented");
    }

    int DbgCounter::GetI() const
    {
        throw std::logic_error("Not implemented");
    }

    bool DbgCounter::GetB() const
    {
        throw std::logic_error("Not implemented");
    }

    float DbgCounter::GetF() const
    {
        throw std::logic_error("Not implemented");
    }

    char const* DbgCounter::GetS() const
    {
        throw std::logic_error("Not implemented");
    }

    char const* DbgCounter::GetName() const
    {
        return m_name.c_str();
    }

    void DbgCounter::IncI()
    {
        throw std::logic_error("Not implemented");
    }

    DbgCounter::DbgCounter()
    {
    }

    DbgCounter::~DbgCounter()
    {
        throw std::logic_error("Not implemented");
    }

    void DbgCounter::SetName(char const* name)
    {
        m_name = name;
    }

    DbgCounter* DbgCounterStack::GetCounter(unsigned id)
    {
        if (id >= m_stack.size())
        {
            return 0;
        }
        return m_stack.at(id);
    }

    DbgCounterStack::~DbgCounterStack()
    {
        Clear();
    }

    unsigned DbgCounterStack::AddCounter(char const* name)
    {
        //TODO: check this
        std::string_view nameView(name);
        for (unsigned i =0; i<m_stack.size(); ++i)
        {
            if (nameView == m_stack[i]->GetName())
            {
                return i;
            }
        }
        auto counter = new DbgCounter;
        counter->SetName(name);
        m_stack.push_back(counter);
        m_numCounters++;
        return m_stack.size() - 1;
    }

    void DbgCounterStack::ClearStringStack()
    {
        m_numStrings = 0;
    }

    DbgCounter* DbgCounterStack::GetCounterByName(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    char const* DbgCounterStack::GetString(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    char const* DbgCounterStack::GetName(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    DbgCounterStack::DbgCounterStack() :
        m_stringStack(32, "")
    {
    }

    unsigned DbgCounterStack::GetNumStrings() const
    {
        throw std::logic_error("Not implemented");
    }

    void DbgCounterStack::Clear()
    {
        //TODO: check correctness
        for (auto* counter : m_stack)
        {
            delete counter;
        }
        m_stack.clear();
    }

    unsigned DbgCounterStack::GetNumCounters() const
    {
        return m_stack.size();
    }

    void DbgCounterStack::DrawStringThisFrame(char const*)
    {
        throw std::logic_error("Not implemented");
    }
}

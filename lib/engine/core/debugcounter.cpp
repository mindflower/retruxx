#include <cstring>
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
        return m_curType;
    }

    int DbgCounter::GetI() const
    {
        return m_i;
    }

    bool DbgCounter::GetB() const
    {
        return m_b;
    }

    float DbgCounter::GetF() const
    {
        return m_f;
    }

    char const* DbgCounter::GetS() const
    {
        return m_s.c_str();
    }

    char const* DbgCounter::GetName() const
    {
        return m_name.c_str();
    }

    void DbgCounter::IncI()
    {
        ++m_i;
    }

    DbgCounter::DbgCounter()
    {
    }

    DbgCounter::~DbgCounter()
    {
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
        for (unsigned i = 0; i < m_stack.size(); ++i)
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

    DbgCounter* DbgCounterStack::GetCounterByName(char const* name)
    {
        // RVA 0x7A03F0
        for (unsigned i = 0; i < m_numCounters; ++i)
        {
            if (!strcmp(m_stack[i]->m_name.c_str(), name))
            {
                return m_stack[i];
            }
        }
        return nullptr;
    }

    char const* DbgCounterStack::GetString(unsigned id) const
    {
        // RVA 0x5A3520
        if (id >= m_numStrings)
        {
            return nullptr;
        }
        return m_stringStack[id].c_str();
    }

    char const* DbgCounterStack::GetName(unsigned id) const
    {
        // RVA 0x7A0020
        if (id >= m_numCounters)
        {
            return nullptr;
        }
        return m_stack[id]->m_name.c_str();
    }

    DbgCounterStack::DbgCounterStack()
    {
        m_stringStack.resize(0x32, "");
        m_numCounters = 0;
        m_numStrings = 0;
    }

    unsigned DbgCounterStack::GetNumStrings() const
    {
        // RVA 0x59C9F0
        return m_numStrings;
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

    void DbgCounterStack::DrawStringThisFrame(char const* str)
    {
        if (m_numStrings < 0x32)
        {
            m_stringStack[m_numStrings] = str;
            ++m_numStrings;
        }
    }
}  // namespace m3d

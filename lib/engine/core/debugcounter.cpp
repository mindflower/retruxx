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
        // RVA 0x5A39F0
        if (id >= m_numCounters)
        {
            return nullptr;
        }
        return m_stack[id];
    }

    DbgCounterStack::~DbgCounterStack()
    {
        Clear();
    }

    unsigned DbgCounterStack::AddCounter(char const* name)
    {
        // RVA 0x7A06C0 - a name that is already registered returns its existing id.
        for (unsigned i = 0; i < m_numCounters; ++i)
        {
            if (!strcmp(m_stack[i]->m_name.c_str(), name))
            {
                return i;
            }
        }

        auto* counter = new DbgCounter;
        counter->m_name = name;
        m_stack.push_back(counter);
        return m_numCounters++;
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
        // RVA 0x7A0590 - deletes the counters and releases the stack's storage.
        // NOTE: m_numCounters is not reset, so the stack is not usable after a Clear; the
        // destructor is its only caller.
        for (unsigned i = 0; i < m_numCounters; ++i)
        {
            delete m_stack[i];
            m_stack[i] = nullptr;
        }
        decltype(m_stack)().swap(m_stack);
    }

    unsigned DbgCounterStack::GetNumCounters() const
    {
        // RVA 0x59C9D0
        return m_numCounters;
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

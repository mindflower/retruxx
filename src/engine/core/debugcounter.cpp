#include <stdexcept>
#include <core/debugcounter.h>

namespace m3d
{
    void DbgCounter::SetI(int)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void DbgCounter::IncI()
    {
        throw std::logic_error("Not implemented");
    }

    DbgCounter::DbgCounter()
    {
        throw std::logic_error("Not implemented");
    }

    DbgCounter::~DbgCounter()
    {
        throw std::logic_error("Not implemented");
    }

    void DbgCounter::SetName(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    DbgCounter* DbgCounterStack::GetCounter(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    DbgCounterStack::~DbgCounterStack()
    {
        Clear();
    }

    unsigned DbgCounterStack::AddCounter(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void DbgCounterStack::ClearStringStack()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void DbgCounterStack::DrawStringThisFrame(char const*)
    {
        throw std::logic_error("Not implemented");
    }
}

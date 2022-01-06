#include <stdexcept>
#include <core/debugcounter.h>

namespace m3d
{
    DbgCounter* DbgCounterStack::GetCounter(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    DbgCounterStack::~DbgCounterStack()
    {
        throw std::logic_error("Not implemented");
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

    DbgCounterStack::DbgCounterStack()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned DbgCounterStack::GetNumStrings() const
    {
        throw std::logic_error("Not implemented");
    }

    void DbgCounterStack::Clear()
    {
        throw std::logic_error("Not implemented");
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

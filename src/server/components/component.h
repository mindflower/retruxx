#pragma once
#include <functional>

namespace ai
{
    template<class T>
    class Component
    {
    public:
        Component();

    private:
        std::function<void(const T&)> m_AfterChange;
        std::function<bool(const T&)> m_BeforeChange;
        //FuncPtrOneArg<T,void> m_AfterChange;
        //FuncPtrOneArgRef<T,bool> m_BeforeChange;
    };
}

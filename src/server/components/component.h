#pragma once

namespace ai
{
    template<class T>
    class Component
    {
    public:
        Component();

    private:
        FuncPtrOneArg<T,void> m_AfterChange;
        FuncPtrOneArgRef<T,bool> m_BeforeChange;
    };
}

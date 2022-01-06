#pragma once
#include "component.h"

namespace ai
{
    template<class T>
    class Numeric : public Component<T>
    {
    public:
        void ApplyModifier(Modifier const&, T)
        {
            throw std::logic_error("Not implemented");
        }
        
        Numeric(T)
        {
            throw std::logic_error("Not implemented");
        }
        
        void sub(T)
        {
            throw std::logic_error("Not implemented");
        }
        
        T get() const
        {
            throw std::logic_error("Not implemented");
        }
        
        void set(T)
        {
            throw std::logic_error("Not implemented");
        }
        
        void SetUnsafe(T)
        {
            throw std::logic_error("Not implemented");
        }

    private:
        //FuncPtrTwoArgsRef<Modifier,T,bool> m_BeforeApplyModifier;
        T m_value;
    };
}

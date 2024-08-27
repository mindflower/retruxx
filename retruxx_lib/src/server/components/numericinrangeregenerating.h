#pragma once
#include "numericinrange.h"

namespace ai
{
    template<class T>
    class NumericInRangeRegenerating : public NumericInRange<T>
    {
    public:
        NumericInRangeRegenerating(T value, T minValue, T maxValue, T regeneration) :
            NumericInRange(value, minValue, maxValue),
            m_regeneration(regeneration)
        {
            throw std::logic_error("Not implemented");
        }
        
        Numeric<T>& regeneration()
        {
            throw std::logic_error("Not implemented");
        }
        
        void regenerate(float)
        {
            throw std::logic_error("Not implemented");
        }

    private:
        void _OnAfterRegenerationChange(T)
        {
            throw std::logic_error("Not implemented");
        }

    private:
        //MemberFunctionOneArg<NumericInRangeRegenerating<float>,float,void>::MemberFunctionOneArg<NumericInRangeRegenerating<float>,float,void>(NumericInRangeRegenerating<float> &,void (*const)(float));
        //FuncPtrOneArgRef<T,bool> m_BeforeRegenerate;
        //FuncPtrOneArg<T,void> m_AfterRegenerationChange;
        Numeric<T> m_regeneration;
    };
}

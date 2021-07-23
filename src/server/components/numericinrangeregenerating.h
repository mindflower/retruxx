#pragma once
#include "numericinrange.h"

namespace ai
{
    template<class T>
    class NumericInRangeRegenerating:  public NumericInRange<T>
    {
    public:
        NumericInRangeRegenerating(T,T,T,T);
        Numeric & regeneration();
        void regenerate(float);

    private:
        void _OnAfterRegenerationChange(T);

        private:
        //MemberFunctionOneArg<NumericInRangeRegenerating<float>,float,void>::MemberFunctionOneArg<NumericInRangeRegenerating<float>,float,void>(NumericInRangeRegenerating<float> &,void (*const)(float));
        FuncPtrOneArgRef<T,bool> m_BeforeRegenerate;
        FuncPtrOneArg<T,void> m_AfterRegenerationChange;
        Numeric<T> m_regeneration;
    };
}

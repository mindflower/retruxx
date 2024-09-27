#pragma once
#include "numericinrange.h"

namespace ai
{
    template<class T>
    class NumericInRangeRegenerating : public NumericInRange<T>
    {
        using ThisType = ai::NumericInRangeRegenerating<T>;
        using InnerNumeric = ai::Numeric<T>;
        using Base = ai::NumericInRange<T>;
        using ThisAfterChangeCallback = ai::MemberFunctionOneArg<ai::NumericInRangeRegenerating<T>, T, void>;

    public:
        /* 0x00ac */ ai::FuncPtrOneArgRef<T, bool> m_BeforeRegenerate;
        /* 0x00b4 */ ai::FuncPtrOneArg<T, void> m_AfterRegenerationChange;

        NumericInRangeRegenerating(T value, T minValue, T maxValue, T regeneration) :
            NumericInRange(value, minValue, maxValue),
            m_BeforeRegenerate(nullptr),
            m_AfterRegenerationChange(nullptr),
            m_regeneration(regeneration)
        {
            //throw std::runtime_error("not implemented");
        }

        const ai::Numeric<T>& regeneration() const;
        ai::Numeric<T>& regeneration();
        void regenerate(T time);
        void assign(const ai::NumericInRangeRegenerating<T>&);

    protected:
        void _AssignUnsafe(const ai::NumericInRangeRegenerating<T>&);

    private:
        void _OnAfterRegenerationChange(T oldRegeneration);
        /* 0x00bc */ ai::Numeric<T> m_regeneration;
    }; /* size: 0x00d8 */
}

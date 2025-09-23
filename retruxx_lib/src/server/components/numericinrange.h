#pragma once
#include "numeric.h"

namespace ai
{
    template<class T>
    class NumericInRange :  public Component<T>
    {
        using ThisType = ai::NumericInRange<T>;
        using InnerNumeric = ai::Numeric<T>;
        using Base = ai::Component<T>;
        using ThisAfterChangeCallback = ai::MemberFunctionOneArg<ai::NumericInRange<T>, T, void>;
        using ThisBeforeChangeCallback = ai::MemberFunctionOneArgRef<ai::NumericInRange<T>, T, bool>;
        using ThisBeforeApplyModifierCallback = ai::MemberFunctionTwoArgsRef<ai::NumericInRange<T>, ai::Modifier, T, bool>;

    public:
        /* 0x0010 */ ai::FuncPtrTwoArgsRef<ai::Modifier, T, bool> m_BeforeValueApplyModifier = nullptr;
        /* 0x0018 */ ai::FuncPtrTwoArgsRef<ai::Modifier, T, bool> m_BeforeMinValueApplyModifier = nullptr;
        /* 0x0020 */ ai::FuncPtrTwoArgsRef<ai::Modifier, T, bool> m_BeforeMaxValueApplyModifier = nullptr;
        /* 0x0028 */ ai::FuncPtrOneArgRef<T, bool> m_BeforeValueChange = nullptr;
        /* 0x0030 */ ai::FuncPtrOneArgRef<T, bool> m_BeforeMinValueChange = nullptr;
        /* 0x0038 */ ai::FuncPtrOneArgRef<T, bool> m_BeforeMaxValueChange = nullptr;
        /* 0x0040 */ ai::FuncPtrOneArg<T, void> m_AfterValueChange = nullptr;
        /* 0x0048 */ ai::FuncPtrOneArg<T, void> m_AfterMinValueChange = nullptr;
        /* 0x0050 */ ai::FuncPtrOneArg<T, void> m_AfterMaxValueChange = nullptr;

        NumericInRange(T value, T minValue, T maxValue) :
            m_value(value),
            m_minValue(minValue),
            m_maxValue(maxValue)
        {
            //throw std::runtime_error("not implemented");
        }

        const ai::Numeric<T>& value() const
        {
            return m_value;
        }

        ai::Numeric<T>& value()
        {
            return m_value;
        }

        const ai::Numeric<T>& minValue() const;
        ai::Numeric<T>& minValue();
        const ai::Numeric<T>& maxValue() const;
        ai::Numeric<T>& maxValue();
        void assign(const ai::NumericInRange<T>&);
        void setToMax();
        bool bIsMax() const;
        void setToMin();
        bool bIsMin() const;

    protected:
        void _AssignUnsafe(const ai::NumericInRange<T>&);

    private:
        void _OnAfterValueChange(T oldValue);
        void _OnAfterMinValueChange(T oldValue);
        void _OnAfterMaxValueChange(T oldValue);
        bool _OnBeforeValueChange(T& newValue);
        bool _OnBeforeMinValueChange(T& newValue);
        bool _OnBeforeMaxValueChange(T& newValue);
        bool _OnBeforeValueApplyModifier(const ai::Modifier& modifier, T& newValue);
        bool _OnBeforeMinValueApplyModifier(const ai::Modifier& modifier, T& newValue);
        bool _OnBeforeMaxValueApplyModifier(const ai::Modifier& modifier, T& newValue);
        /* 0x0058 */ ai::Numeric<T> m_value;
        /* 0x0074 */ ai::Numeric<T> m_minValue;
        /* 0x0090 */ ai::Numeric<T> m_maxValue;
    }; /* size: 0x00ac */
}

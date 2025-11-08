#pragma once
#include "numeric.h"

namespace ai
{
    template<class T>
    class NumericBoundedBelow : public ai::Component<T>
    {
        using ThisType = ai::NumericBoundedBelow<T>;
        using InnerNumeric = ai::Numeric<T>;
        using Base = ai::Component<T>;
        using ThisAfterChangeCallback = ai::MemberFunctionOneArg<ThisType, T, void>;
        using ThisBeforeChangeCallback = ai::MemberFunctionOneArgRef<ThisType, T, bool>;
        using ThisBeforeApplyModifierCallback = ai::MemberFunctionTwoArgsRef<ThisType, ai::Modifier, T, bool>;

    public:
        /* 0x0010 */ ai::FuncPtrTwoArgsRef<ai::Modifier, T, bool> m_BeforeValueApplyModifier = nullptr;
        /* 0x0018 */ ai::FuncPtrTwoArgsRef<ai::Modifier, T, bool> m_BeforeMinValueApplyModifier = nullptr;
        /* 0x0020 */ ai::FuncPtrOneArgRef<T, bool> m_BeforeValueChange = nullptr;
        /* 0x0028 */ ai::FuncPtrOneArgRef<T, bool> m_BeforeMinValueChange = nullptr;
        /* 0x0030 */ ai::FuncPtrOneArg<T, void> m_AfterValueChange = nullptr;
        /* 0x0038 */ ai::FuncPtrOneArg<T, void> m_AfterMinValueChange = nullptr;

        NumericBoundedBelow(T value, T minValue) :
            m_value{value},
            m_minValue{minValue}
        {
            m_value.m_AfterChange = new MemberFunctionOneArg<ThisType, T, void>(*this, &ThisType::_OnAfterValueChange);
            m_minValue.m_AfterChange = new MemberFunctionOneArg<ThisType, T, void>(*this, &ThisType::_OnAfterMinValueChange);

            m_value.m_BeforeChange = new MemberFunctionOneArgRef<ThisType, T, bool>(*this, &ThisType::_OnBeforeValueChange);
            m_minValue.m_BeforeChange = new MemberFunctionOneArgRef<ThisType, T, bool>(*this, &ThisType::_OnBeforeMinValueChange);

            m_value.m_BeforeApplyModifier = new MemberFunctionTwoArgsRef<ThisType, ai::Modifier, T, bool>(*this, &ThisType::_OnBeforeValueApplyModifier);
            m_minValue.m_BeforeApplyModifier = new MemberFunctionTwoArgsRef<ThisType, ai::Modifier, T, bool>(*this, &ThisType::_OnBeforeMinValueApplyModifier);
        }

        const ai::Numeric<T>& minValue() const;
        ai::Numeric<T>& minValue();

        const ai::Numeric<T>& value() const
        {
            return m_value;
        }

        ai::Numeric<T>& value()
        {
            return m_value;
        }

        void assign(const ai::NumericBoundedBelow<T>&);
        void setToMin();
        bool bIsMin() const;

    protected:
        void _AssignUnsafe(const ai::NumericBoundedBelow<T>&);

    private:
        void _AfterSomeChange()
        {
            throw std::runtime_error("not implemented");
        }

        void _OnAfterValueChange(T oldValue)
        {
            m_AfterValueChange(oldValue);
        }

        void _OnAfterMinValueChange(T oldValue)
        {
            m_AfterMinValueChange(oldValue);
        }

        bool _OnBeforeValueChange(T& oldValue)
        {
            return m_BeforeValueChange(oldValue);
        }

        bool _OnBeforeMinValueChange(T& oldValue)
        {
            return m_BeforeMinValueChange(oldValue);
        }

        bool _OnBeforeValueApplyModifier(const ai::Modifier& modifier, T& oldValue)
        {
            return m_BeforeValueApplyModifier(modifier, oldValue);
        }

        bool _OnBeforeMinValueApplyModifier(const ai::Modifier& modifier, T& oldValue)
        {
            return m_BeforeMinValueApplyModifier(modifier, oldValue);
        }

        /* 0x0040 */ ai::Numeric<T> m_value;
        /* 0x005c */ ai::Numeric<T> m_minValue;
    }; /* size: 0x0078 */
}

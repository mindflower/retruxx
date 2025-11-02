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
            m_value.m_AfterChange = new ThisAfterChangeCallback(*this, &NumericInRange<T>::_OnAfterValueChange);
            m_minValue.m_AfterChange = new ThisAfterChangeCallback(*this, &NumericInRange<T>::_OnAfterMinValueChange);
            m_maxValue.m_AfterChange = new ThisAfterChangeCallback(*this, &NumericInRange<T>::_OnAfterMaxValueChange);

            m_value.m_BeforeChange = new ThisBeforeChangeCallback(*this, &NumericInRange<T>::_OnBeforeValueChange);
            m_minValue.m_BeforeChange = new ThisBeforeChangeCallback(*this, &NumericInRange<T>::_OnBeforeMinValueChange);
            m_maxValue.m_BeforeChange = new ThisBeforeChangeCallback(*this, &NumericInRange<T>::_OnBeforeMaxValueChange);

            m_value.m_BeforeApplyModifier = new ThisBeforeApplyModifierCallback(*this, &NumericInRange<T>::_OnBeforeValueApplyModifier);
            m_minValue.m_BeforeApplyModifier = new ThisBeforeApplyModifierCallback(*this, &NumericInRange<T>::_OnBeforeMinValueApplyModifier);
            m_maxValue.m_BeforeApplyModifier = new ThisBeforeApplyModifierCallback(*this, &NumericInRange<T>::_OnBeforeMaxValueApplyModifier);
        }

        const ai::Numeric<T>& value() const
        {
            return m_value;
        }

        ai::Numeric<T>& value()
        {
            return m_value;
        }

        const ai::Numeric<T>& minValue() const
        {
            return m_minValue;
        }

        ai::Numeric<T>& minValue()
        {
            return m_minValue;
        }

        const ai::Numeric<T>& maxValue() const
        {
            return m_maxValue;
        }

        ai::Numeric<T>& maxValue()
        {
            return m_maxValue;
        }

        void assign(const ai::NumericInRange<T>&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void setToMax()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        bool bIsMax() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void setToMin()
        {
            // TODO: check this
            m_value.set(m_minValue.get());
        }

        bool bIsMin() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

    protected:
        void _AssignUnsafe(const ai::NumericInRange<T>&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

    private:
        void _OnAfterValueChange(T oldValue)
        {
            if (m_minValue.get() <= m_value.get())
            {
                if (m_value.get() > m_maxValue.get())
                    m_value.SetUnsafe(m_maxValue.get());
            }
            else
            {
                m_value.SetUnsafe(m_minValue.get());
            }
            m_AfterValueChange(oldValue);
        }

        void _OnAfterMinValueChange(T oldValue)
        {
            if (m_minValue.get() > m_value.get())
            {
                if (m_minValue.get() > m_maxValue.get())
                    m_minValue.SetUnsafe(m_maxValue.get());
                m_value.SetUnsafe(m_minValue.get());
            }
            m_AfterMinValueChange(oldValue);
        }

        void _OnAfterMaxValueChange(T oldValue)
        {
            if (m_value.get() > m_maxValue.get())
            {
                if (m_minValue.get() > m_maxValue.get())
                    m_maxValue.SetUnsafe(m_minValue.get());
                m_value.SetUnsafe(m_maxValue.get());
            }
            m_AfterMaxValueChange(oldValue);
        }

        bool _OnBeforeValueChange(T& newValue)
        {
            return m_BeforeValueChange(newValue);
        }

        bool _OnBeforeMinValueChange(T& newValue)
        {
            return m_BeforeMinValueChange(newValue);
        }

        bool _OnBeforeMaxValueChange(T& newValue)
        {
            return m_BeforeMaxValueChange(newValue);
        }

        bool _OnBeforeValueApplyModifier(const ai::Modifier& modifier, T& newValue)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        bool _OnBeforeMinValueApplyModifier(const ai::Modifier& modifier, T& newValue)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        bool _OnBeforeMaxValueApplyModifier(const ai::Modifier& modifier, T& newValue)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        /* 0x0058 */ ai::Numeric<T> m_value;
        /* 0x0074 */ ai::Numeric<T> m_minValue;
        /* 0x0090 */ ai::Numeric<T> m_maxValue;
    }; /* size: 0x00ac */
}

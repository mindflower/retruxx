#pragma once
#include "component.h"

namespace ai
{
    template<class T>
    class Numeric : public Component<T>
    {
        using ThisType = ai::Numeric<T>;
        using Base = ai::Component<T>;

    public:
        /* 0x0010 */ ai::FuncPtrTwoArgsRef<ai::Modifier, T, bool> m_BeforeApplyModifier;
        Numeric(T value) : m_value(value), m_BeforeApplyModifier(nullptr)
        {
        }

        T get() const
        {
            return m_value;
        }

        void set(T newValue)
        {
            if (!m_BeforeChange(newValue))
            {
                T const oldValue = m_value;
                m_value = newValue;
                m_AfterChange(oldValue);
            }
        }

        void assign(ai::Numeric<T> const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void add(T value)
        {
            T newValue = m_value + value;
            if (!m_BeforeChange(newValue))
            {
                T const oldValue = m_value;
                m_value = newValue;
                m_AfterChange(oldValue);
            }
        }

        void sub(T value)
        {
            T newValue = m_value - value;
            if (!m_BeforeChange(newValue))
            {
                T const oldValue = m_value;
                m_value = newValue;
                m_AfterChange(oldValue);
            }
        }

        void SetUnsafe(T newValue)
        {
            m_value = newValue;
        }

        void ApplyModifier(ai::Modifier const& modifier, T base)
        {
            T newValue = m_value;
            modifier.Apply(&newValue, base);
            if (!m_BeforeApplyModifier(modifier, newValue))
            {
                T const oldValue = m_value;
                m_value = newValue;
                m_AfterChange(oldValue);
            }
        }

    protected:
        void _AssignUnsafe(ai::Numeric<T> const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

    private:
        /* 0x0018 */ T m_value{};
    }; /* size: 0x001c */
}  // namespace ai

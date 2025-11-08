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
        Numeric(T value) :
            m_value(value),
            m_BeforeApplyModifier(nullptr)
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
                m_value = newValue;
                m_AfterChange(m_value);
            }
        }

        void assign(const ai::Numeric<T>&);
        void add(T);
        void sub(T value);

        void SetUnsafe(T newValue)
        {
            m_value = newValue;
        }

        void ApplyModifier(const ai::Modifier& modifier, T base);

    protected:
        void _AssignUnsafe(const ai::Numeric<T>&);

    private:
        /* 0x0018 */ T m_value{};
    }; /* size: 0x001c */
}

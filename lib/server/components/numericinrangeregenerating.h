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
        ai::FuncPtrOneArgRef<T, bool> m_BeforeRegenerate;
        ai::FuncPtrOneArg<T, void> m_AfterRegenerationChange;

        NumericInRangeRegenerating(T value, T minValue, T maxValue, T regeneration) :
            NumericInRange(value, minValue, maxValue),
            m_BeforeRegenerate(nullptr),
            m_AfterRegenerationChange(nullptr),
            m_regeneration(regeneration)
        {
            //throw std::runtime_error("not implemented");
        }

        ai::Numeric<T> const& regeneration() const
        {
            return m_regeneration;
        }

        ai::Numeric<T>& regeneration()
        {
            return m_regeneration;
        }

        void regenerate(T time)
        {
            // RVA 0x5D3840 (float) - advances the value by rate * time. The value is stored without the range
            // clamp of its BeforeChange guard; only m_BeforeRegenerate can veto or adjust it.
            T newValue = m_regeneration.get() * time + value().get();
            if (m_BeforeRegenerate(newValue))
            {
                return;
            }

            T const oldValue = value().get();
            value().SetUnsafe(newValue);
            value().m_AfterChange(oldValue);
        }

        void assign(ai::NumericInRangeRegenerating<T> const&);

    protected:
        void _AssignUnsafe(ai::NumericInRangeRegenerating<T> const&);

    private:
        void _OnAfterRegenerationChange(T oldRegeneration);
        ai::Numeric<T> m_regeneration;
    };
}  // namespace ai

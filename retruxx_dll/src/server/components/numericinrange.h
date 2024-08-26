#pragma once
#include "component.h"
#include "numeric.h"

namespace ai
{
    template<class T>
    class NumericInRange :  public Component<T>
    {
    public:
        Numeric<T> const & value() const ;
        Numeric<T> & value();
        void setToMax();
        Numeric<T> & minValue();
        Numeric<T> const & minValue() const ;
        NumericInRange(T,T,T);
        void setToMin();
        Numeric<T> const & maxValue() const ;
        Numeric<T> & maxValue();

    private:
        //bool _OnBeforeMinValueApplyModifier(Modifier const &,float &);
        //MemberFunctionOneArg<NumericInRange<T>,T,void>::MemberFunctionOneArg<NumericInRange<T>,T,void>(NumericInRange<T> &,void (*const)(T));
        //bool _OnBeforeMinValueChange(T &);
        //bool _OnBeforeValueChange(T &);
        //void _OnAfterValueChange(T);
        //bool _OnBeforeValueApplyModifier(Modifier const &,T &);
        //void _OnAfterMaxValueChange(T);
        //bool _OnBeforeMaxValueApplyModifier(Modifier const &,float &);
        //bool _OnBeforeMaxValueChange(float &);
        //MemberFunctionTwoArgsRef<NumericInRange<float>,Modifier,float,bool>::MemberFunctionTwoArgsRef<NumericInRange<float>,Modifier,float,bool>(NumericInRange<float> &,bool (*const)(Modifier const &,float &));
        //void _OnAfterMinValueChange(float);
        //MemberFunctionOneArgRef<NumericInRange<float>,float,bool>::MemberFunctionOneArgRef<NumericInRange<float>,float,bool>(NumericInRange<float> &,bool (*const)(float &));
        //FuncPtrTwoArgsRef<Modifier,float,bool> m_BeforeValueApplyModifier;
        //FuncPtrTwoArgsRef<Modifier,float,bool> m_BeforeMinValueApplyModifier;
        //FuncPtrTwoArgsRef<Modifier,float,bool> m_BeforeMaxValueApplyModifier;
        //FuncPtrOneArgRef<float,bool> m_BeforeValueChange;
        //FuncPtrOneArgRef<float,bool> m_BeforeMinValueChange;
        //FuncPtrOneArgRef<float,bool> m_BeforeMaxValueChange;
        //FuncPtrOneArg<float,void> m_AfterValueChange;
        //FuncPtrOneArg<float,void> m_AfterMinValueChange;
        //FuncPtrOneArg<float,void> m_AfterMaxValueChange;
        //Numeric<float> m_value;
        //Numeric<float> m_minValue;
        //Numeric<float> m_maxValue;
    };

    template <class T>
    Numeric<T> const& NumericInRange<T>::value() const
    {
        throw std::logic_error("Not implemented");
    }

    template <class T>
    Numeric<T>& NumericInRange<T>::value()
    {
        throw std::logic_error("Not implemented");
    }

    template <class T>
    void NumericInRange<T>::setToMax()
    {
        throw std::logic_error("Not implemented");
    }

    template <class T>
    Numeric<T>& NumericInRange<T>::minValue()
    {
        throw std::logic_error("Not implemented");
    }

    template <class T>
    Numeric<T> const& NumericInRange<T>::minValue() const
    {
        throw std::logic_error("Not implemented");
    }

    template <class T>
    NumericInRange<T>::NumericInRange(T, T, T)
    {
        throw std::logic_error("Not implemented");
    }

    template <class T>
    void NumericInRange<T>::setToMin()
    {
        throw std::logic_error("Not implemented");
    }

    template <class T>
    Numeric<T> const& NumericInRange<T>::maxValue() const
    {
        throw std::logic_error("Not implemented");
    }

    template <class T>
    Numeric<T>& NumericInRange<T>::maxValue()
    {
        throw std::logic_error("Not implemented");
    }
}

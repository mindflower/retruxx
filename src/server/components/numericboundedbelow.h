#pragma once

namespace ai
{
    template<class T>
    class NumericBoundedBelow : public Component<T>
    {
    public:
        NumericBoundedBelow(T,T);
        Numeric const & value() const ;
        Numeric & value();
        void setToMin();
        Numeric & minValue();

    private:
        void _OnAfterValueChange(T);
        //MemberFunctionOneArgRef<NumericBoundedBelow<T>,T,bool>::MemberFunctionOneArgRef<NumericBoundedBelow<T>,T,bool>(NumericBoundedBelow<T> &,bool (*const)(T &));
        void _AfterSomeChange();
        bool _OnBeforeValueChange(T &);
        bool _OnBeforeMinValueChange(T &);
        bool _OnBeforeValueApplyModifier(Modifier const &,T &);
        bool _OnBeforeMinValueApplyModifier(Modifier const &,T &);
        MemberFunctionTwoArgsRef<NumericBoundedBelow<T>,Modifier,T,bool>::MemberFunctionTwoArgsRef<NumericBoundedBelow<T>,Modifier,T,bool>(NumericBoundedBelow<T> &,bool (*const)(Modifier const &,T &));
        void _OnAfterMinValueChange(T);
        MemberFunctionOneArg<NumericBoundedBelow<T>,T,void>::MemberFunctionOneArg<NumericBoundedBelow<T>,T,void>(NumericBoundedBelow<T> &,void (*const)(T));
        FuncPtrTwoArgsRef<Modifier,T,bool> m_BeforeValueApplyModifier;
        FuncPtrTwoArgsRef<Modifier,T,bool> m_BeforeMinValueApplyModifier;
        FuncPtrOneArgRef<T,bool> m_BeforeValueChange;
        FuncPtrOneArgRef<T,bool> m_BeforeMinValueChange;
        FuncPtrOneArg<T,void> m_AfterValueChange;
        FuncPtrOneArg<T,void> m_AfterMinValueChange;
        Numeric<T> m_value;
        Numeric<T> m_minValue;
    };
}

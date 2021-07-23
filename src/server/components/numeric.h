#pragma once

namespace ai
{
    template<class T>
    class Numeric :  public Component<T>
    {
    public:
        void ApplyModifier(Modifier const &,T);
        Numeric(T);
        void sub(T);
        T get() const ;
        void set(T);
        void SetUnsafe(T);

    private:
        FuncPtrTwoArgsRef<Modifier,T,bool> m_BeforeApplyModifier;
        T m_value;
    };
}

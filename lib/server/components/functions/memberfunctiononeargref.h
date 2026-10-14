#pragma once
#include "basefunction.h"

namespace ai
{
    template<typename T1, typename T2>
    class FuncPtrOneArgRef : public ai::FuncPtr<ai::BaseFunctionOneArgRef<T1, T2>>
    {
        using BaseFunc = ai::BaseFunctionOneArgRef<T1, T2>;
        using Base = ai::FuncPtr<ai::BaseFunctionOneArgRef<T1, T2>>;

    public:
        FuncPtrOneArgRef(BaseFunc* pFunc) : FuncPtr(pFunc)
        {
        }

        T2 operator()(T1& arg)
        {
            return m_pFunc && m_pFunc->Execute(arg);
        }

        FuncPtrOneArgRef<T1, T2> const& operator=(BaseFunc* pNewFunc)
        {
            assert(m_pFunc == 0);
            if (pNewFunc)
            {
                m_pFunc = pNewFunc;
            }
            return *this;
        }
    };

    template<typename TClass, typename T1, typename RetType>
    class MemberFunctionOneArgRef : public ai::BaseFunctionOneArgRef<T1, RetType>
    {
        //using (*FuncMemberOfT) = RetType;

    public:
        MemberFunctionOneArgRef(ai::MemberFunctionOneArgRef<TClass, T1, RetType> const&)
        {
            throw std::runtime_error("not implemented");
        }

        MemberFunctionOneArgRef()
        {
            throw std::runtime_error("not implemented");
        }

        MemberFunctionOneArgRef(TClass& t, RetType (TClass::*func)(T1&))
        {
            m_pT = &t;
            m_func = func;
        }

        virtual RetType Execute(T1& arg) override
        {
            if (m_pT)
            {
                return (m_pT->*m_func)(arg);
            }
            return RetType();
        }

    private:
        TClass* m_pT = nullptr;
        RetType (TClass::*m_func)(T1&) = nullptr;
    };
}  // namespace ai
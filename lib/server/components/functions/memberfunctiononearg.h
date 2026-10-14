#pragma once
#include "basefunction.h"

namespace ai
{
    template<typename T1, typename RetType>
    class FuncPtrOneArg : public ai::FuncPtr<ai::BaseFunctionOneArg<T1, RetType>>
    {
        using BaseFunc = ai::BaseFunctionOneArg<T1, RetType>;
        using Base = ai::FuncPtr<ai::BaseFunctionOneArg<T1, RetType>>;

    public:
        FuncPtrOneArg(BaseFunc* pFunc) : FuncPtr(pFunc)
        {
        }

        RetType operator()(T1 arg)
        {
            if (m_pFunc)
            {
                return m_pFunc->Execute(arg);
            }
            return RetType();
        }

        FuncPtrOneArg<T1, RetType> const& operator=(BaseFunc* pNewFunc)
        {
            FuncPtr::operator=(pNewFunc);
            return *this;
        }
    };

    template<typename TClass, typename T1, typename RetType>
    class MemberFunctionOneArg : public ai::BaseFunctionOneArg<T1, RetType>
    {
        //using (*FuncMemberOfT) = RetType;

    public:
        MemberFunctionOneArg(ai::MemberFunctionOneArg<TClass, T1, RetType> const&)
        {
            throw std::runtime_error("not implemented");
        }

        MemberFunctionOneArg()
        {
            throw std::runtime_error("not implemented");
        }

        MemberFunctionOneArg(TClass& t, RetType (TClass::*func)(T1))
        {
            m_pT = &t;
            m_func = func;
        }

        virtual RetType Execute(T1 arg) override
        {
            if (m_pT)
            {
                return (m_pT->*m_func)(arg);
            }
            return RetType();
        }

    private:
        TClass* m_pT = nullptr;
        RetType (TClass::*m_func)(T1) = nullptr;
    };
}  // namespace ai

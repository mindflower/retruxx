#pragma once
#include "basefunction.h"

namespace ai
{
    template<typename T1, typename T2, typename RetType>
    class FuncPtrTwoArgsRef : public ai::FuncPtr<ai::BaseFunctionTwoArgsRef<T1, T2, RetType>>
    {
        using BaseFunc = ai::BaseFunctionTwoArgsRef<T1, T2, RetType>;
        using Base = ai::FuncPtr<ai::BaseFunctionTwoArgsRef<T1, T2, RetType>>;

    public:
        FuncPtrTwoArgsRef(BaseFunc* pFunc) : FuncPtr(pFunc)
        {
        }

        bool operator()(const T1& arg1, T2& arg2)
        {
            return m_pFunc && m_pFunc->Execute(arg1, arg2);
        }

        ai::FuncPtrTwoArgsRef<T1, T2, RetType> const& operator=(BaseFunc* pNewFunc)
        {
            assert(m_pFunc == 0);
            if (pNewFunc)
            {
                m_pFunc = pNewFunc;
            }
            return *this;
        }
    };

    template<typename TClass, typename T1, typename T2, typename RetType>
    class MemberFunctionTwoArgsRef : public ai::BaseFunctionTwoArgsRef<T1, T2, RetType>
    {
        // using (*FuncMemberOfT) = RetType;

    public:
        MemberFunctionTwoArgsRef(ai::MemberFunctionTwoArgsRef<TClass, T1, T2, RetType> const&)
        {
            throw std::runtime_error("not implemented");
        }

        MemberFunctionTwoArgsRef()
        {
            throw std::runtime_error("not implemented");
        }

        MemberFunctionTwoArgsRef(TClass& t, bool (TClass::*func)(const T1&, T2&))
        {
            this->m_pT = &t;
            this->m_func = func;
        }

        virtual bool Execute(const T1& arg1, T2& arg2) override
        {
            if (m_pT)
            {
                return (m_pT->*m_func)(arg1, arg2);
            }
            return RetType();
        }

    private:
        TClass* m_pT = nullptr;
        bool (TClass::*m_func)(const T1&, T2&) = nullptr;
    };
}  // namespace ai

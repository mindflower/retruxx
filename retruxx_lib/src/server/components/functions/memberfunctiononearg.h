#pragma once
#include "basefunction.h"

namespace ai
{
    template <typename T1, typename RetType>
    class FuncPtrOneArg : public ai::FuncPtr<ai::BaseFunctionOneArg<T1, RetType>>
    {
        using BaseFunc = ai::BaseFunctionOneArg<T1, RetType>;
        using Base = ai::FuncPtr<ai::BaseFunctionOneArg<T1, RetType>>;

    public:
        FuncPtrOneArg(BaseFunc* pFunc) :
            FuncPtr(pFunc)
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

        const FuncPtrOneArg<T1, RetType>& operator=(BaseFunc* pNewFunc)
        {
            FuncPtr::operator=(pNewFunc);
            return *this;
        }
    }; /* size: 0x0008 */

    template <typename TClass, typename T1, typename RetType>
    class MemberFunctionOneArg : public ai::BaseFunctionOneArg<T1, RetType>
    {
        //using (*FuncMemberOfT) = RetType;

    public:
        MemberFunctionOneArg(const ai::MemberFunctionOneArg<TClass, T1, RetType>&)
        {
            throw std::runtime_error("not implemented");
        }

        MemberFunctionOneArg()
        {
            throw std::runtime_error("not implemented");
        }

        MemberFunctionOneArg(TClass& t, RetType(TClass::*func)(T1))
        {
            m_pT = &t;
            m_func = func;
        }

        virtual RetType Execute(T1 arg) override /* 0x04 */
        {
            if (m_pT)
            {
                return (m_pT->*m_func)(arg);
            }
            return RetType();
        }

    private:
        /* 0x0004 */ TClass* m_pT = nullptr;
        /* 0x0008 */ RetType (TClass::*m_func)(T1) = nullptr;
    }; /* size: 0x000c */
}

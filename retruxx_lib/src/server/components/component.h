#pragma once
#include "server/modifier.h"
#include "functions/memberfunctiononearg.h"
#include "functions/memberfunctiononeargref.h"
#include "functions/memberfunctionstwoargsref.h"

namespace ai
{
    template<class T>
    class Component
    {
        using ThisType = ai::Component<T>;

 //   private:
    public:
        Component() :
            m_AfterChange(nullptr),
            m_BeforeChange(nullptr)
        {
            //throw std::runtime_error("not implemented");
        }

        Component(const ThisType&)
        {
            throw std::runtime_error("not implemented");
        }

        using AfterChangeFuncPtr = ai::FuncPtrOneArg<T, void>;
        using BeforeChangeFuncPtr = ai::FuncPtrOneArgRef<T, bool>;
        using BeforeApplyModifierFuncPtr = ai::FuncPtrTwoArgsRef<ai::Modifier, T, bool>;
        using StoringType = T;
        using AfterChangeCallback = ai::BaseFunctionOneArg<T, void>;
        using BeforeChangeCallback = ai::BaseFunctionOneArgRef<T, bool>;
        using BeforeApplyModifierCallback = ai::BaseFunctionTwoArgsRef<ai::Modifier, T, bool>;

    public:
        /* 0x0000 */ ai::FuncPtrOneArg<T, void> m_AfterChange;
        /* 0x0008 */ ai::FuncPtrOneArgRef<T, bool> m_BeforeChange;
        void assign(const ThisType&);

    protected:
        void _AssignUnsafe(const ThisType&);
    }; /* size: 0x0010 */
}

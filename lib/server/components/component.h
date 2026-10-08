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

    public:
        Component() : m_AfterChange(nullptr), m_BeforeChange(nullptr)
        {
            //throw std::runtime_error("not implemented");
        }

        Component(ThisType const&)
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

        // NOTE: assign and _AssignUnsafe are declared in the PDB but were never
        // instantiated, so their bodies do not come from the binary. They follow
        // the guarded/unguarded split used by NumericInRange.
        // The base only holds the callbacks, which stay bound to their owner, so
        // there is nothing to copy.
        void assign(ThisType const&)
        {
        }

    protected:
        void _AssignUnsafe(ThisType const&)
        {
        }
    }; /* size: 0x0010 */
}  // namespace ai

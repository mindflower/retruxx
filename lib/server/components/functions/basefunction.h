#pragma once
#include <cassert>

namespace ai
{
    class BaseFunction
    {
    public:
        virtual ~BaseFunction() = default;
    };

    template <typename T1, typename RetType>
    class BaseFunctionOneArg : public BaseFunction
    {
    public:
        virtual RetType Execute(T1) = 0;
    };

    template <typename T1, typename RetType>
    class BaseFunctionOneArgRef : public BaseFunction
    {
    public:
        virtual RetType Execute(T1&) = 0;
    };

    template <typename T1, typename T2, typename RetType>
    class BaseFunctionTwoArgsRef : public ai::BaseFunction
    {
    public:
        virtual RetType Execute(const T1&, T2&) = 0;
    };

    template <typename TFunc>
    class FuncPtr
    {
    //private:
    public:
        FuncPtr(TFunc* pFunc) :
            m_pFunc(pFunc)
        {
        }

        FuncPtr(const TFunc&)
        {
            throw std::runtime_error("not implemented");
        }

    public:
        const FuncPtr<TFunc>& operator=(TFunc* pNewFunc)
        {
            assert(m_pFunc == nullptr);
            m_pFunc = pNewFunc;
            return *this;
        }

        using FunctionType = TFunc;

    public:
        virtual ~FuncPtr()
        {
            delete m_pFunc;
        }

    protected:
        TFunc* m_pFunc = nullptr;
    };
}

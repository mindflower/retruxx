#pragma once
#include <vector>
#include <core/stringm3d.h>

namespace ai
{
    class AIParamRef;

    class AISignal
    {
    public:
        AISignal();
        CStr const & GetName() const ;
        void Dump() const ;
        void Set(CStr const &);
        void Set(CStr const &,int);

    private:
        int m_FuncNum;
        std::vector<AIParamRef> m_ParamRefList;
        CStr m_name;
    };
}

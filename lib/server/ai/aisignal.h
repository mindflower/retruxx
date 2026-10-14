#pragma once
#include <core/stringm3d.h>

#include "aiparamref.h"
#include "retruxx/common.h"

namespace ai
{
    class AISignal
    {
    public:
        int m_FuncNum;
        retruxx::vector<ai::AIParamRef, retruxx::allocator<ai::AIParamRef>> m_ParamRefList;

        AISignal();
        void Set(CStr const& Name, int FuncNum);
        void Set(CStr const& Name);
        void Dump() const;
        CStr const& GetName() const;

    private:
        CStr m_name;
    };
}  // namespace ai

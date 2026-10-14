#pragma once
#include "retruxx/common.h"

namespace ai
{
    class AIParamRef;

    class AIPassageCommand
    {
    public:
        int m_StateNum;
        retruxx::vector<ai::AIParamRef, retruxx::allocator<ai::AIParamRef> > m_ParamRefList;
        void Dump();
    };
}

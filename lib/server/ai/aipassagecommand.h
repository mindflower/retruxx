#pragma once
#include "retruxx/common.h"

namespace ai
{
    class AIParamRef;

    class AIPassageCommand
    {
    public:
        /* 0x0000 */ int m_StateNum;
        retruxx::vector<ai::AIParamRef, retruxx::allocator<ai::AIParamRef> > m_ParamRefList;
        void Dump();
    }; /* size: 0x0014 */
}

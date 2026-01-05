#pragma once
#include <core/stringm3d.h>

#include "aiparamref.h"
#include "retruxx/common.h"

namespace ai
{
    class AISignal
    {
    public:
        /* 0x0000 */ int m_FuncNum;
        retruxx::vector<ai::AIParamRef, retruxx::allocator<ai::AIParamRef> > m_ParamRefList;

        AISignal();
        void Set(const CStr& Name, int FuncNum);
        void Set(const CStr& Name);
        void Dump() const;
        const CStr& GetName() const;

    private:
        CStr m_name;
    }; /* size: 0x0020 */
}

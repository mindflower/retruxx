#pragma once
#include <core/stringm3d.h>

#include "aiparamref.h"
#include "decisionmatrix.h"
#include "retruxx/common.h"

namespace ai
{
    class AIState
    {
    private:
        CStr m_name;

    public:
        retruxx::vector<ai::AIParamRef, retruxx::allocator<ai::AIParamRef> > m_ParamRefList;
        /* 0x001c */ ai::DecisionMatrix* m_pChildDecisionMatrix;
        /* 0x0020 */ int m_FuncNum;
        /* 0x0024 */ unsigned int m_SignalIDs[16];

        AIState();
        void Set(const CStr& Name, int FuncNum);
        void SetRetValueInterpretation(int S, unsigned int SignalID);
        const CStr& GetName() const;
        void Dump() const;
    }; /* size: 0x0064 */
}

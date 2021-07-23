#pragma once
#include <vector>
#include <core/stringm3d.h>

namespace ai
{
    class DecisionMatrix;
    class AIParamRef;

    class AIState
    {
    public:
        void Set(CStr const &,int);
        void Dump() const ;
        CStr const & GetName() const ;
        void SetRetValueInterpretation(int,unsigned int);
        AIState();

    private:
        CStr m_name;
        std::vector<AIParamRef> m_ParamRefList;
        DecisionMatrix *m_pChildDecisionMatrix;
        int m_FuncNum;
        unsigned int m_SignalIDs[16];
    };
}

#pragma once
#include <core/stringm3d.h>

namespace ai
{
    class DecisionMatrix;

    class AIMatrix
    {
    public:
        AIMatrix(AIMatrix const &);
        AIMatrix(CStr const&, DecisionMatrix*);
        CStr const & GetName() const ;
        DecisionMatrix * GetPtr() const ;
        void Set(CStr const &,DecisionMatrix *);

    private:
        DecisionMatrix *m_pDM;
        CStr m_name;
    };
}

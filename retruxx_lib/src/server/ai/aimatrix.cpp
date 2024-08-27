#pragma once
#include "aimatrix.h"

#include <stdexcept>

namespace ai
{
    AIMatrix::AIMatrix(AIMatrix const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIMatrix::AIMatrix(CStr const& name, DecisionMatrix* pDM) :
        m_name(name),
        m_pDM(pDM)
    {
    }

    CStr const& AIMatrix::GetName() const
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix* AIMatrix::GetPtr() const
    {
        throw std::logic_error("Not implemented");
    }

    void AIMatrix::Set(CStr const&, DecisionMatrix*)
    {
        throw std::logic_error("Not implemented");
    }
}

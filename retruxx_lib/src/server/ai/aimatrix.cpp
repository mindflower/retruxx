#pragma once
#include "aimatrix.h"

#include <stdexcept>

namespace ai
{
    AIMatrix::AIMatrix(CStr const& name, DecisionMatrix* pDM) :
        m_name(name),
        m_pDM(pDM)
    {
    }

    CStr const& AIMatrix::GetName() const
    {
        return m_name;
    }

    DecisionMatrix* AIMatrix::GetPtr() const
    {
        return m_pDM;
    }

    void AIMatrix::Set(CStr const&, DecisionMatrix*)
    {
        throw std::logic_error("Not implemented");
    }
}

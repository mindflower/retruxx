#pragma once
#include "aimatrix.h"

#include <stdexcept>

#include "decisionmatrix.h"
#include "thirdparty/containers.h"

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

    void AIMatrix::Delete()
    {
        delete m_pDM;
        m_pDM = nullptr;
    }

    DecisionMatrix* AIMatrix::GetPtr() const
    {
        return m_pDM;
    }

    void AIMatrix::Set(CStr const&, DecisionMatrix*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

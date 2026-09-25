#pragma once
#include "aimatrix.h"

#include <stdexcept>

#include "decisionmatrix.h"
#include "retruxx/common.h"

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

    // RVA 0x7E71F0
    void AIMatrix::Set(CStr const& Name, DecisionMatrix* pDM)
    {
        m_name = Name;
        m_pDM = pDM;
    }
}

#include "aistate.h"

#include <cstdio>
#include <stdexcept>

namespace ai
{
    AIState::AIState()
    {
        this->m_FuncNum = 0;
        this->m_pChildDecisionMatrix = 0;
        this->m_SignalIDs[0] = 0xFFFF;
        this->m_SignalIDs[1] = 0xFFFF;
        this->m_SignalIDs[2] = 0xFFFF;
        this->m_SignalIDs[3] = 0xFFFF;
        this->m_SignalIDs[4] = 0xFFFF;
        this->m_SignalIDs[5] = 0xFFFF;
        this->m_SignalIDs[6] = 0xFFFF;
        this->m_SignalIDs[7] = 0xFFFF;
        this->m_SignalIDs[8] = 0xFFFF;
        this->m_SignalIDs[9] = 0xFFFF;
        this->m_SignalIDs[10] = 0xFFFF;
        this->m_SignalIDs[11] = 0xFFFF;
        this->m_SignalIDs[12] = 0xFFFF;
        this->m_SignalIDs[13] = 0xFFFF;
        this->m_SignalIDs[14] = 0xFFFF;
        this->m_SignalIDs[15] = 0xFFFF;
    }

    void AIState::Set(const CStr& Name, int FuncNum)
    {
        m_name = Name;
        m_FuncNum = FuncNum;
        m_ParamRefList.clear();
    }

    // RVA 0x825D00
    void AIState::SetRetValueInterpretation(int S, unsigned int SignalID)
    {
        if (S < 16)
        {
            m_SignalIDs[S] = SignalID;
        }
    }

    const CStr& AIState::GetName() const
    {
        return m_name;
    }

    // RVA 0x906310
    void AIState::Dump() const
    {
        printf("%s( ", m_name.c_str());
        for (int i = 0; i < (int)m_ParamRefList.size(); ++i)
        {
            printf("%d", m_ParamRefList[i].m_Num);
            if (i != (int)m_ParamRefList.size() - 1)
            {
                printf(",");
            }
        }
        printf(" ) = %d", m_FuncNum);
        for (unsigned int i = 0; i < 16; ++i)
        {
            if (m_SignalIDs[i] != 0xFFFF)
            {
                printf(", S%d = %d", i, m_SignalIDs[i]);
            }
        }
        if (m_pChildDecisionMatrix)
        {
            printf(" Sublevel present");
        }
    }
}

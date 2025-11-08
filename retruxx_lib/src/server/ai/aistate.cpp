#include "aistate.h"

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

    void AIState::SetRetValueInterpretation(int S, unsigned int SignalID)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const CStr& AIState::GetName() const
    {
        return m_name;
    }

    void AIState::Dump() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

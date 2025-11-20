#include "ai.h"
#include "aimessage.h"
#include "aipassagestate.h"
#include <core/aiparam.h>
#include <stdexcept>
#include "aimanager.h"
#include "aistate.h"
#include "decisionmatrix.h"

#include <cassert>

namespace ai
{
    namespace
    {
        const CStr UNDEFINED = "Undefined";
    }

    void AI::LoadAIFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AI::GetCurState2Num()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::CommandStackOpen()
    {
        m_CommandStackOpen = true;
    }

    void AI::CommandStackClose()
    {
        m_CommandStackOpen = false;
    }

    m3d::AIParam AI::GetMessage2Param(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam AI::GetMessage1Param(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::AIInit()
    {
        m_StateStack1.clear();
        m_StateStack2.clear();
        m_Messages1.clear();
        m_Messages2.clear();
        m_Commands.clear();
        m_fStateStack2Changed = 0;
        m_CommandStackOpen = 0;
        m_numCurCommand = 0;
        m_CommandProcessed = 0;
    }

    AI::AI()
    {
        this->m_fStateStack2Changed = 0;
        this->m_pDM = 0;
        this->m_CommandStackOpen = 0;
        this->m_numCurCommand = 0;
        this->m_CommandProcessed = 0;
    }

    int AI::GetCurState1Num()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::PutMessage2(AIMessage const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::PutMessage1(AIMessage const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::SaveAIToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::SetDecisionMatrix(int MatrixNum)
    {
        this->m_pDM = ai::theAIManager->GetDecisionMatrixPtr(MatrixNum);
    }

    void AI::PutCommand(int Num, m3d::AIParam const& Param1, m3d::AIParam const& Param2, m3d::AIParam const& Param3)
    {
        if (m_CommandStackOpen)
        {
            return;
        }

        m_numCurCommand = 0;
        m_CommandProcessed = 0;

        ai::AIMessage val(Num, Param1, Param2, Param3);
        m_Commands.push_back(std::move(val));
    }

    void AI::PutCommand(AIMessage const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::SetCommand(int, m3d::AIParam const&, m3d::AIParam const&, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::AIUpdate(Obj*)
    {
        // TODO: implement AI::AIUpdate
        // RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::Dump()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& AI::GetCurState2Name()
    {
        if (m_StateStack2.empty())
        {
            return UNDEFINED;
        }

        if (m_pDM)
        {
            auto stateNum = m_StateStack2.back().m_StateNum;
            if (stateNum < m_pDM->NumStates())
            {
                return m_pDM->GetState(stateNum).GetName();
            }
        }

        return UNDEFINED;
    }

    m3d::AIParam AI::GetCmdParam(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr AI::ToStr()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& AI::GetCurState1Name()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::InsCommand(int Num, const m3d::AIParam& Param1, const m3d::AIParam& Param2, const m3d::AIParam& Param3)
    {
        assert(m_numCurCommand >= 0);

        ai::AIMessage command(Num, Param1, Param2, Param3);
        command.m_RemoveAfterFinishing = 1;
        m_CommandProcessed = false;
        m_Commands.push_back(std::move(command));
    }

    void AI::SetState2Param(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AI::SetState1Param(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam AI::GetState2Param(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam AI::GetState1Param(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AI::_CurrentMessage1CommandNum()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AI::_CurrentMessage2CommandNum()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AI::_CurrentState1Num()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AI::_CurrentState2Num()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

#include "ai.h"
#include "aimessage.h"
#include "aipassagestate.h"
#include <core/aiparam.h>
#include <stdexcept>
#include "aimanager.h"

namespace ai
{
    void AI::LoadAIFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int AI::GetCurState2Num()
    {
        throw std::logic_error("Not implemented");
    }

    void AI::CommandStackOpen()
    {
        throw std::logic_error("Not implemented");
    }

    void AI::CommandStackClose()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam AI::GetMessage2Param(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam AI::GetMessage1Param(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::AIInit()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void AI::PutMessage2(AIMessage const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::PutMessage1(AIMessage const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::SaveAIToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void AI::SetDecisionMatrix(int MatrixNum)
    {
        this->m_pDM = ai::theAIManager->GetDecisionMatrixPtr(MatrixNum);
    }

    void AI::PutCommand(int, m3d::AIParam const&, m3d::AIParam const&, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::PutCommand(AIMessage const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::SetCommand(int, m3d::AIParam const&, m3d::AIParam const&, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::AIUpdate(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::Dump()
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& AI::GetCurState2Name()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam AI::GetCmdParam(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    CStr AI::ToStr()
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& AI::GetCurState1Name()
    {
        throw std::logic_error("Not implemented");
    }

    void AI::InsCommand(int, m3d::AIParam const&, m3d::AIParam const&, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::SetState2Param(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AI::SetState1Param(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam AI::GetState2Param(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam AI::GetState1Param(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int AI::_CurrentMessage1CommandNum()
    {
        throw std::logic_error("Not implemented");
    }

    int AI::_CurrentMessage2CommandNum()
    {
        throw std::logic_error("Not implemented");
    }

    int AI::_CurrentState1Num()
    {
        throw std::logic_error("Not implemented");
    }

    int AI::_CurrentState2Num()
    {
        throw std::logic_error("Not implemented");
    }
}

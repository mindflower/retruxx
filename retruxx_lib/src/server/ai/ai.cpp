#include "ai.h"
#include "aimessage.h"
#include "aipassagestate.h"
#include <core/aiparam.h>
#include <stdexcept>
#include "aimanager.h"
#include "aisignal.h"
#include "aistate.h"
#include "decisionmatrix.h"
#include "core/log.h"
#include "server/objects/base/obj.h"
#include "server/objects/base/prototypemanager.h"

#include <cassert>

namespace ai
{
    namespace
    {
        const CStr UNDEFINED = "Undefined";

        
        void PushToStateStack(std::vector<ai::AIPassageState>& stateStack, ai::AIPassageCommand const& passageCommand, ai::Obj* pObj)
        {
            ai::AIPassageState ps;
            ps.m_StateNum = passageCommand.m_StateNum;
            ps.m_ParamList.reserve(passageCommand.m_ParamRefList.size());
            for (auto const& ref : passageCommand.m_ParamRefList)
            {
                auto action = ai::theAIManager->AIAction(ref.m_Num, pObj);
                ps.m_ParamList.push_back(std::move(action));
            }
            stateStack.push_back(std::move(ps));
        }

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

    void AI::AIUpdate(Obj* pObj)
    {
        // TODO: generated code AI::AIUpdate
        assert(m_numCurCommand >= 0);

        DecisionMatrix* pDM = m_pDM;
        if (!pDM)
        {
            return;
        }

        // Initialize state stack 2 if empty
        if (m_StateStack2.empty())
        {
            PushToStateStack(m_StateStack2, pDM->GetDefault(), pObj);
            m_fStateStack2Changed = true;
        }

        int signalNum = 0xFFFF;

        // Process commands if not already processed
        if (!m_CommandProcessed)
        {
            if (m_Commands.empty())
            {
                m_Commands.clear();
                m_numCurCommand = 0;
            }
            else if (m_numCurCommand >= static_cast<int>(m_Commands.size()))
            {
                m_Commands.clear();
                m_numCurCommand = 0;
            }
            else
            {
                AIMessage& currentCommand = m_Commands[m_numCurCommand];

                if (currentCommand.m_Num == 17)
                {
                    m_numCurCommand = 0;
                }

                if (m_Commands[0].m_Num == 17)
                {
                    m_Commands.clear();
                }
                else
                {
                    signalNum = pDM->GetExternSignalMapping(currentCommand.m_Num);
                    m_CommandProcessed = true;

                    if (signalNum != 0xFFFF)
                    {
                        int funcNum = pDM->GetSignal(signalNum).m_FuncNum;
                        if (funcNum != 0xFFFF)
                        {
                            m3d::AIParam result = ai::theAIManager->AIAction(funcNum, pObj);
                            int actionID = result.GetAsID();
                            if (actionID != 0)
                            {
                                m_CommandProcessed = false;
                            }
                        }

                        // Process special signal commands
                        if (signalNum == 65534)
                        {
                            m_StateStack2.pop_back();
                            m_fStateStack2Changed = true;
                        }
                        else
                        {
                            int currentState = m_StateStack2.back().m_StateNum;
                            DecisionMatrixElement const* decision = pDM->UnsafeGetDecision(currentState, signalNum);

                            if (!decision->m_PassageCommands.empty())
                            {
                                if (decision->m_flags & 1)
                                {
                                    m_StateStack2.clear();
                                }

                                // Push commands in reverse order
                                for (int i = static_cast<int>(decision->m_PassageCommands.size()) - 1; i >= 0; --i)
                                {
                                    PushToStateStack(m_StateStack2, decision->m_PassageCommands[i], pObj);
                                }
                                m_fStateStack2Changed = true;
                            }
                            else
                            {
                                m_CommandProcessed = false;
                            }
                        }
                    }
                    else
                    {
                        // Log unsupported command
                        CStr prototypeName = ai::thePrototypeManager->GetPrototypeName(pObj->GetPrototypeId());


                        // Log warning about unsupported command
                        M3D_LOG_WARN(
                            "warning: unsupported command " + CStr(currentCommand.m_Num) + " in AIAction for " + CStr(pObj->GetName()) + " " + prototypeName);

                        m_CommandProcessed = false;
                    }
                }
            }
        }

        // Execute current state action
        int currentState = m_StateStack2.back().m_StateNum;
        int stateFuncNum = pDM->GetState(currentState).m_FuncNum;
        m3d::AIParam actionResult = ai::theAIManager->AIAction(stateFuncNum, pObj);
        int actionID = actionResult.GetAsID();

        if (actionID != 0)
        {
            signalNum = pDM->GetState(currentState).m_SignalIDs[actionID];
            if (signalNum != 0xFFFF)
            {
                // Process state transition based on action result
                if (signalNum == 65534)
                {
                    m_StateStack2.pop_back();
                    m_fStateStack2Changed = true;
                }
                else
                {
                    DecisionMatrixElement const* decision = pDM->UnsafeGetDecision(currentState, signalNum);

                    if (!decision->m_PassageCommands.empty())
                    {
                        if (decision->m_flags & 1)
                        {
                            m_StateStack2.clear();
                        }

                        for (int i = static_cast<int>(decision->m_PassageCommands.size()) - 1; i >= 0; --i)
                        {
                            PushToStateStack(m_StateStack2, decision->m_PassageCommands[i], pObj);
                        }
                        m_fStateStack2Changed = true;
                    }
                    else
                    {
                        m_CommandProcessed = false;
                    }
                }
            }
            else
            {
                m_CommandProcessed = false;
            }
        }

        // Process pending messages in m_Messages2
        while (!m_Messages2.empty())
        {
            AIMessage& lastMessage = m_Messages2.back();

            if (lastMessage.m_Num < 16)
            {
                signalNum = pDM->GetExternSignalMapping(lastMessage.m_Num);
                if (signalNum != 0xFFFF)
                {
                    int funcNum = pDM->GetSignal(signalNum).m_FuncNum;
                    if (funcNum != 0xFFFF)
                    {
                        ai::theAIManager->AIAction(funcNum, pObj);
                    }
                }
            }
            m_Messages2.pop_back();
        }

        // Process signal if valid
        if (signalNum != 0xFFFF)
        {
            if (signalNum == 65534)
            {
                m_StateStack2.pop_back();
                m_fStateStack2Changed = true;
            }
            else
            {
                int currentState = m_StateStack2.back().m_StateNum;
                DecisionMatrixElement const* decision = pDM->UnsafeGetDecision(currentState, signalNum);

                if (!decision->m_PassageCommands.empty())
                {
                    if (decision->m_flags & 1)
                    {
                        m_StateStack2.clear();
                    }

                    for (int i = static_cast<int>(decision->m_PassageCommands.size()) - 1; i >= 0; --i)
                    {
                        PushToStateStack(m_StateStack2, decision->m_PassageCommands[i], pObj);
                    }
                    m_fStateStack2Changed = true;
                }
                else
                {
                    m_CommandProcessed = false;
                }
            }
        }

        // Handle state stack changes
        if (m_fStateStack2Changed)
        {
            m_StateStack1.clear();
            m_fStateStack2Changed = false;
        }

        // Process child state machine if available
        if (!m_StateStack2.empty())
        {
            int currentState = m_StateStack2.back().m_StateNum;
            DecisionMatrix* childDM = pDM->GetState(currentState).m_pChildDecisionMatrix;

            if (childDM)
            {
                int childSignalNum = 0xFFFF;

                // Initialize child state stack if empty
                if (m_StateStack1.empty())
                {
                    PushToStateStack(m_StateStack1, childDM->GetDefault(), pObj);
                }

                int childState = m_StateStack1.back().m_StateNum;

                // Check for exit condition
                if (childState == childDM->GetExitStateNum())
                {
                    m_StateStack2.pop_back();
                    m_fStateStack2Changed = true;
                    m_CommandProcessed = false;

                    // Process child messages
                    while (!m_Messages1.empty())
                    {
                        AIMessage& lastChildMessage = m_Messages1.back();

                        if (lastChildMessage.m_Num < 16)
                        {
                            childSignalNum = childDM->GetExternSignalMapping(lastChildMessage.m_Num);
                            if (childSignalNum != 0xFFFF)
                            {
                                int funcNum = childDM->GetSignal(childSignalNum).m_FuncNum;
                                if (funcNum != 0xFFFF)
                                {
                                    ai::theAIManager->AIAction(funcNum, pObj);
                                }
                            }
                        }
                        m_Messages1.pop_back();
                    }
                }
                else
                {
                    // Execute child state action
                    int childFuncNum = childDM->GetState(childState).m_FuncNum;
                    m3d::AIParam childResult = ai::theAIManager->AIAction(childFuncNum, pObj);
                    int childActionID = childResult.GetAsID();

                    if (childActionID != 0)
                    {
                        childSignalNum = childDM->GetState(childState).m_SignalIDs[childActionID];
                    }

                    if (childSignalNum == 0xFFFF)
                    {
                        // Process child messages
                        while (!m_Messages1.empty())
                        {
                            AIMessage& lastChildMessage = m_Messages1.back();

                            if (lastChildMessage.m_Num < 16)
                            {
                                childSignalNum = childDM->GetExternSignalMapping(lastChildMessage.m_Num);
                                if (childSignalNum != 0xFFFF)
                                {
                                    int funcNum = childDM->GetSignal(childSignalNum).m_FuncNum;
                                    if (funcNum != 0xFFFF)
                                    {
                                        ai::theAIManager->AIAction(funcNum, pObj);
                                    }
                                }
                            }
                            m_Messages1.pop_back();
                        }
                    }

                    if (childSignalNum != 0xFFFF)
                    {
                        if (childSignalNum == 65534)
                        {
                            m_StateStack1.pop_back();
                        }
                        else
                        {
                            DecisionMatrixElement const* childDecision = childDM->UnsafeGetDecision(childState, childSignalNum);

                            if (!childDecision->m_PassageCommands.empty())
                            {
                                if (childDecision->m_flags & 1)
                                {
                                    m_StateStack1.clear();
                                }

                                for (int i = static_cast<int>(childDecision->m_PassageCommands.size()) - 1; i >= 0; --i)
                                {
                                    PushToStateStack(m_StateStack1, childDecision->m_PassageCommands[i], pObj);
                                }

                                pObj->StopTimeOut();
                            }
                        }
                    }
                }
            }
            else
            {
                m_CommandProcessed = false;
            }
        }
        else
        {
            m_CommandProcessed = false;
        }

        // Advance to next command if current one is processed
        if (!m_CommandProcessed)
        {
            if (m_numCurCommand < 0)
            {
                m_numCurCommand++;
            }
            else if (m_numCurCommand >= static_cast<int>(m_Commands.size()))
            {
                m_numCurCommand++;
            }
            else
            {
                AIMessage& currentCommand = m_Commands[m_numCurCommand];
                if (currentCommand.m_RemoveAfterFinishing == 0)
                {
                    m_numCurCommand++;
                }
                else
                {
                    m_Commands.erase(m_Commands.begin() + m_numCurCommand);
                }
            }
        }
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

    m3d::AIParam AI::GetCmdParam(unsigned paramNum)
    {
        if (m_numCurCommand < 0)
        {
            return m3d::AIParam(0);
        }

        if (m_numCurCommand < m_Commands.size() && paramNum < m_Commands[m_numCurCommand].m_ParamList.size())
        {
            return m3d::AIParam(m_Commands[m_numCurCommand].m_ParamList[paramNum]); 
        }

        return m3d::AIParam(0);
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

    m3d::AIParam AI::GetState2Param(unsigned paramNum)
    {
        if (!m_StateStack2.empty() && paramNum < m_StateStack2.back().m_ParamList.size())
        {
            return m3d::AIParam(m_StateStack2.back().m_ParamList[paramNum]);
        }

        return m3d::AIParam(0);
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

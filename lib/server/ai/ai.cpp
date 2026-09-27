#include "ai.h"
#include <core/ini.h>
#include <core/kernel.h>
#include <core/ref_ptr.h>
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

    void AI::LoadAIFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode)
    {
        // RVA 0x7F4C00 - reads back what SaveAIToXML wrote, plus the two message queues that it
        // never writes.
        // NOTE: the three flags share one scratch variable that is not reset between reads, so an
        // attribute that is absent from the file inherits the value of the previous one.
        int flag = 0;
        m3d::SafeIntAttrib(flag, OwnNode, "StateStack2Changed");
        m_fStateStack2Changed = flag != 0;
        m3d::SafeIntAttrib(flag, OwnNode, "CommandProcessed");
        m_CommandProcessed = flag != 0;
        m3d::SafeIntAttrib(flag, OwnNode, "CommandStackOpen");
        m_CommandStackOpen = flag != 0;
        m3d::SafeIntAttrib(m_numCurCommand, OwnNode, "CurCommand");

        ref_ptr node = xmlFile->CreateNode();

        for (int i = 0;; ++i)
        {
            OwnNode->GetFirstChild(node, (CStr("Command_") + CStr(i)).c_str());
            if (node->IsEmpty())
            {
                break;
            }
            AIMessage mess;
            mess.m_Num = 0xFFFF;
            mess.m_RemoveAfterFinishing = 0;
            mess.LoadFromXML(xmlFile, node);
            m_Commands.push_back(std::move(mess));
        }

        // A saved index that no longer addresses a command is reset rather than trusted.
        if (m_numCurCommand < 0 || m_numCurCommand >= static_cast<int>(m_Commands.size()))
        {
            m_numCurCommand = 0;
        }

        for (int i = 0;; ++i)
        {
            OwnNode->GetFirstChild(node, (CStr("Stack1Element_") + CStr(i)).c_str());
            if (node->IsEmpty())
            {
                break;
            }
            AIPassageState ps;
            ps.LoadFromXML(xmlFile, node);
            m_StateStack1.push_back(std::move(ps));
        }
        for (int i = 0;; ++i)
        {
            OwnNode->GetFirstChild(node, (CStr("Stack2Element_") + CStr(i)).c_str());
            if (node->IsEmpty())
            {
                break;
            }
            AIPassageState ps;
            ps.LoadFromXML(xmlFile, node);
            m_StateStack2.push_back(std::move(ps));
        }
        for (int i = 0;; ++i)
        {
            OwnNode->GetFirstChild(node, (CStr("Message1Element_") + CStr(i)).c_str());
            if (node->IsEmpty())
            {
                break;
            }
            AIMessage mess;
            mess.m_Num = 0xFFFF;
            mess.m_RemoveAfterFinishing = 0;
            mess.LoadFromXML(xmlFile, node);
            m_Messages1.push_back(std::move(mess));
        }
        for (int i = 0;; ++i)
        {
            OwnNode->GetFirstChild(node, (CStr("Message2Element_") + CStr(i)).c_str());
            if (node->IsEmpty())
            {
                break;
            }
            AIMessage mess;
            mess.m_Num = 0xFFFF;
            mess.m_RemoveAfterFinishing = 0;
            mess.LoadFromXML(xmlFile, node);
            m_Messages2.push_back(std::move(mess));
        }
    }

    int AI::GetCurState2Num()
    {
        // RVA 0x7F20D0 - 0xFFFF stands for "no state".
        if (m_StateStack2.empty())
        {
            return 0xFFFF;
        }
        return m_StateStack2.back().m_StateNum;
    }

    void AI::CommandStackOpen()
    {
        m_CommandStackOpen = true;
    }

    void AI::CommandStackClose()
    {
        m_CommandStackOpen = false;
    }

    m3d::AIParam AI::GetMessage2Param(unsigned paramNum)
    {
        // RVA 0x7F2490 - an out-of-range request reads back as an undefined parameter.
        if (m_Messages2.empty() || paramNum >= m_Messages2.back().m_ParamList.size())
        {
            return m3d::AIParam();
        }
        return m_Messages2.back().m_ParamList[paramNum];
    }

    m3d::AIParam AI::GetMessage1Param(unsigned paramNum)
    {
        // RVA 0x7F23C0 - an out-of-range request reads back as an undefined parameter.
        if (m_Messages1.empty() || paramNum >= m_Messages1.back().m_ParamList.size())
        {
            return m3d::AIParam();
        }
        return m_Messages1.back().m_ParamList[paramNum];
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
        // RVA 0x7F2110 - 0xFFFF stands for "no state".
        if (m_StateStack1.empty())
        {
            return 0xFFFF;
        }
        return m_StateStack1.back().m_StateNum;
    }

    void AI::PutMessage2(AIMessage const& Message)
    {
        // RVA 0x7F4BE0
        m_Messages2.push_back(Message);
    }

    void AI::PutMessage1(AIMessage const& Message)
    {
        // RVA 0x7F4BF0
        m_Messages1.push_back(Message);
    }

    void AI::SaveAIToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const
    {
        // RVA 0x7F0130 - the queues and both state stacks are written out as numbered child
        // elements, which is what LoadAIFromXML walks back in.
        OwnNode->SetAttribute("StateStack2Changed", CStr(m_fStateStack2Changed).c_str());
        OwnNode->SetAttribute("CommandProcessed", CStr(m_CommandProcessed).c_str());
        OwnNode->SetAttribute("CommandStackOpen", CStr(m_CommandStackOpen).c_str());
        if (m_numCurCommand != -1)
        {
            OwnNode->SetAttribute("CurCommand", CStr(m_numCurCommand).c_str());
        }

        for (unsigned i = 0; i < m_Commands.size(); ++i)
        {
            ref_ptr node = xmlFile->CreateNode(
                m3d::cmn::XML_NODE_ELEMENT, (CStr("Command_") + CStr(i)).c_str());
            OwnNode->AddChild(node);
            m_Commands[i].SaveToXML(xmlFile, node);
        }
        for (unsigned i = 0; i < m_StateStack1.size(); ++i)
        {
            ref_ptr node = xmlFile->CreateNode(
                m3d::cmn::XML_NODE_ELEMENT, (CStr("Stack1Element_") + CStr(i)).c_str());
            OwnNode->AddChild(node);
            m_StateStack1[i].SaveToXML(xmlFile, node);
        }
        for (unsigned i = 0; i < m_StateStack2.size(); ++i)
        {
            ref_ptr node = xmlFile->CreateNode(
                m3d::cmn::XML_NODE_ELEMENT, (CStr("Stack2Element_") + CStr(i)).c_str());
            OwnNode->AddChild(node);
            m_StateStack2[i].SaveToXML(xmlFile, node);
        }
        // NOTE: the message queues are not saved, so a reloaded AI comes back without them.
    }

    void AI::SetDecisionMatrix(int MatrixNum)
    {
        this->m_pDM = ai::theAIManager->GetDecisionMatrixPtr(MatrixNum);
    }

    void AI::PutCommand(int Num, m3d::AIParam const& Param1, m3d::AIParam const& Param2, m3d::AIParam const& Param3)
    {
        // RVA 0x7F49F0 - a closed command stack appends; an open one starts afresh.
        if (!m_CommandStackOpen)
        {
            m_Commands.clear();
        }

        m_numCurCommand = 0;
        m_CommandProcessed = false;

        ai::AIMessage val(Num, Param1, Param2, Param3);
        m_Commands.push_back(std::move(val));
    }

    void AI::PutCommand(AIMessage const& command)
    {
        // RVA 0x7F49C0 - a closed command stack appends; an open one starts afresh.
        if (!m_CommandStackOpen)
        {
            m_Commands.clear();
        }
        m_numCurCommand = 0;
        m_CommandProcessed = false;
        m_Commands.push_back(command);
    }

    void AI::SetCommand(int Num, m3d::AIParam const& Param1, m3d::AIParam const& Param2,
        m3d::AIParam const& Param3)
    {
        // RVA 0x7F31B0 - overwrites the command currently being worked on rather than queueing
        // a new one. Undefined parameters are dropped rather than stored.
        M3D_ASSERT(m_numCurCommand >= 0);

        AIMessage command;
        if (Param1.GetType() != m3d::AIPARAM_UNDEFINE)
        {
            command.m_ParamList.push_back(Param1);
        }
        if (Param2.GetType() != m3d::AIPARAM_UNDEFINE)
        {
            command.m_ParamList.push_back(Param2);
        }
        if (Param3.GetType() != m3d::AIPARAM_UNDEFINE)
        {
            command.m_ParamList.push_back(Param3);
        }

        m_CommandProcessed = false;
        M3D_ASSERT(m_numCurCommand < static_cast<int>(m_Commands.size()));

        AIMessage& target = m_Commands[m_numCurCommand];
        target.m_Num = Num;
        target.m_ParamList = command.m_ParamList;
        target.m_RemoveAfterFinishing = 1;
    }

    void AI::AIUpdate(Obj* pObj)
    {
        // RVA 0x7F41B0 - one step of the two-level state machine. At most one signal moves the
        // outer stack per step: the current command's, else the one the current state returns,
        // else the last queued outer message's. Then the sub-machine of the resulting outer
        // state does the same with its own stack and message queue.
        assert(m_numCurCommand >= 0);

        DecisionMatrix* const pDM = m_pDM;
        if (!pDM)
        {
            return;
        }

        if (m_StateStack2.empty())
        {
            PushToStateStack(m_StateStack2, pDM->GetDefault(), pObj);
            m_fStateStack2Changed = true;
        }

        int signalNum = 0xFFFF;
        // Set once a signal has been picked; the remaining sources are then skipped this step.
        bool haveSignal = false;

        if (!m_CommandProcessed)
        {
            if (m_numCurCommand >= static_cast<int>(m_Commands.size()))
            {
                m_Commands.clear();
                m_numCurCommand = 0;
            }
            else
            {
                // A "loop" command sends the queue back to its start.
                if (m_Commands[m_numCurCommand].m_Num == 17)
                {
                    m_numCurCommand = 0;
                }
                if (m_Commands[0].m_Num == 17)
                {
                    m_Commands.clear();
                }
                else
                {
                    AIMessage const& command = m_Commands[m_numCurCommand];
                    signalNum = pDM->GetExternSignalMapping(command.m_Num);
                    m_CommandProcessed = true;
                    if (signalNum != 0xFFFF)
                    {
                        int const funcNum = pDM->GetSignal(signalNum).m_FuncNum;
                        if (funcNum != 0xFFFF && ai::theAIManager->AIAction(funcNum, pObj).GetAsID() != 0)
                        {
                            m_CommandProcessed = false;
                        }
                        haveSignal = true;
                    }
                    else
                    {
                        CStr const prototypeName = ai::thePrototypeManager->GetPrototypeName(pObj->GetPrototypeId());
                        M3D_LOG_INFO(CStr("warning: unsupported command S") + CStr(command.m_Num) +
                            CStr(" in AIAction for ") + CStr(pObj->GetName()) + CStr(" ") + prototypeName);
                        m_CommandProcessed = false;
                    }
                }
            }
        }

        if (!haveSignal)
        {
            AIState const& state = pDM->GetState(m_StateStack2.back().m_StateNum);
            int const actionId = ai::theAIManager->AIAction(state.m_FuncNum, pObj).GetAsID();
            if (actionId != 0)
            {
                signalNum = state.m_SignalIDs[actionId];
                if (signalNum != 0xFFFF)
                {
                    haveSignal = true;
                }
                else
                {
                    m_CommandProcessed = false;
                }
            }

            if (!haveSignal)
            {
                // Queued messages are handled newest first; each one's signal function runs, and
                // the last one with a mapped signal decides the transition.
                while (!m_Messages2.empty())
                {
                    AIMessage const& message = m_Messages2.back();
                    if (message.m_Num < 16)
                    {
                        signalNum = pDM->GetExternSignalMapping(message.m_Num);
                        if (signalNum != 0xFFFF)
                        {
                            int const funcNum = pDM->GetSignal(signalNum).m_FuncNum;
                            if (funcNum != 0xFFFF)
                            {
                                ai::theAIManager->AIAction(funcNum, pObj);
                            }
                        }
                    }
                    m_Messages2.pop_back();
                }
                haveSignal = signalNum != 0xFFFF;
            }
        }

        if (haveSignal)
        {
            if (signalNum == 65534)
            {
                m_StateStack2.pop_back();
                m_fStateStack2Changed = true;
            }
            else
            {
                DecisionMatrixElement const* const decision =
                    pDM->UnsafeGetDecision(m_StateStack2.back().m_StateNum, signalNum);
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

        // A new outer state starts its sub-machine afresh.
        if (m_fStateStack2Changed)
        {
            m_StateStack1.clear();
            m_fStateStack2Changed = false;
        }

        DecisionMatrix* const childDM =
            m_StateStack2.empty() ? nullptr : pDM->GetState(m_StateStack2.back().m_StateNum).m_pChildDecisionMatrix;
        if (childDM)
        {
            int childSignalNum = 0xFFFF;
            if (m_StateStack1.empty())
            {
                PushToStateStack(m_StateStack1, childDM->GetDefault(), pObj);
            }

            // NOTE: the inner state's function runs even when that state is the exit state.
            int const childActionId =
                ai::theAIManager->AIAction(childDM->GetState(m_StateStack1.back().m_StateNum).m_FuncNum, pObj).GetAsID();

            auto const drainMessages1 = [&]() {
                while (!m_Messages1.empty())
                {
                    AIMessage const& message = m_Messages1.back();
                    if (message.m_Num < 16)
                    {
                        childSignalNum = childDM->GetExternSignalMapping(message.m_Num);
                        if (childSignalNum != 0xFFFF)
                        {
                            int const funcNum = childDM->GetSignal(childSignalNum).m_FuncNum;
                            if (funcNum != 0xFFFF)
                            {
                                ai::theAIManager->AIAction(funcNum, pObj);
                            }
                        }
                    }
                    m_Messages1.pop_back();
                }
            };

            int const childState = m_StateStack1.back().m_StateNum;
            if (childState == childDM->GetExitStateNum())
            {
                // The sub-machine has finished, and so has the outer state that owns it.
                m_StateStack2.pop_back();
                m_fStateStack2Changed = true;
                m_CommandProcessed = false;
                drainMessages1();
            }
            else
            {
                if (childActionId != 0)
                {
                    childSignalNum = childDM->GetState(childState).m_SignalIDs[childActionId];
                }
                if (childSignalNum == 0xFFFF)
                {
                    drainMessages1();
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
                    DecisionMatrixElement const* const childDecision =
                        childDM->UnsafeGetDecision(m_StateStack1.back().m_StateNum, childSignalNum);
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
        else
        {
            m_CommandProcessed = false;
        }

        // A finished command either stays in the queue (and the next one is taken) or is removed.
        if (!m_CommandProcessed)
        {
            if (m_numCurCommand < 0 || m_numCurCommand >= static_cast<int>(m_Commands.size()) ||
                m_Commands[m_numCurCommand].m_RemoveAfterFinishing == 0)
            {
                ++m_numCurCommand;
            }
            else
            {
                m_Commands.erase(m_Commands.begin() + m_numCurCommand);
            }
        }
    }

    void AI::Dump()
    {
        // RVA 0x7F0840 - both stacks are walked from the top down, innermost state first.
        CStr OutStr("AIDump: ");
        if (!m_StateStack2.empty())
        {
            OutStr += CStr("State2 = ");
            for (auto it = m_StateStack2.rbegin(); it != m_StateStack2.rend(); ++it)
            {
                OutStr += m_pDM->m_States[it->m_StateNum].GetName() + CStr("( ");
                for (unsigned i = 0; i < it->m_ParamList.size(); ++i)
                {
                    OutStr += it->m_ParamList[i].ToStr();
                    if (i != it->m_ParamList.size() - 1)
                    {
                        OutStr += CStr(",");
                    }
                }
                OutStr += CStr(" ) | ");
            }

            if (!m_StateStack1.empty())
            {
                DecisionMatrix const* const childDM =
                    m_pDM->m_States[m_StateStack2.back().m_StateNum].m_pChildDecisionMatrix;
                OutStr += childDM ? CStr(" State1 = ") : CStr("Warning: ");
                for (auto it = m_StateStack1.rbegin(); it != m_StateStack1.rend(); ++it)
                {
                    // Without a sub-matrix there is no name to print, so the raw index stands in.
                    OutStr += (childDM ? childDM->m_States[it->m_StateNum].GetName()
                                       : CStr(it->m_StateNum)) +
                        CStr("( ");
                    for (unsigned i = 0; i < it->m_ParamList.size(); ++i)
                    {
                        OutStr += it->m_ParamList[i].ToStr();
                        if (i != it->m_ParamList.size() - 1)
                        {
                            OutStr += CStr(",");
                        }
                    }
                    OutStr += CStr(" ) | ");
                }
            }
        }
        M3D_LOG_INFO(OutStr);
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
        // RVA 0x7F2190 - an out-of-range request reads back as an undefined parameter.
        if (m_numCurCommand < 0 || m_numCurCommand >= static_cast<int>(m_Commands.size()) ||
            paramNum >= m_Commands[m_numCurCommand].m_ParamList.size())
        {
            return m3d::AIParam();
        }
        return m_Commands[m_numCurCommand].m_ParamList[paramNum];
    }

    CStr AI::ToStr()
    {
        // RVA 0x7F1030 - the one-line form used by the AI debug overlay.
        CStr OutStr;
        if (!m_CommandProcessed)
        {
            OutStr += CStr("NCP ");
        }

        if (!m_Commands.empty())
        {
            // NOTE: this assigns rather than appends, so the "NCP " prefix above is thrown away
            // whenever there is a command to show.
            OutStr = CStr("command = ");
            for (unsigned i = 0; i < m_Commands.size(); ++i)
            {
                AIMessage const& command = m_Commands[i];
                if (static_cast<int>(i) == m_numCurCommand)
                {
                    OutStr += CStr("[");
                }

                if (command.m_Num < 16)
                {
                    int const signalNum = m_pDM->m_ExternSignalMappings[command.m_Num];
                    if (signalNum < 0 || signalNum >= m_pDM->m_numSignals)
                    {
                        OutStr += CStr("S") + CStr(command.m_Num) + CStr(" range out");
                    }
                    else
                    {
                        OutStr += m_pDM->m_Signals[signalNum].GetName();
                        if (!command.m_ParamList.empty())
                        {
                            OutStr += CStr("( ");
                            for (unsigned j = 0; j < command.m_ParamList.size(); ++j)
                            {
                                OutStr += command.m_ParamList[j].GetAsStr();
                                if (j != command.m_ParamList.size() - 1)
                                {
                                    OutStr += CStr(",");
                                }
                            }
                            OutStr += CStr(" )");
                        }
                    }
                }
                else if (command.m_Num == 17)
                {
                    OutStr += CStr("loop");
                }
                else
                {
                    OutStr += CStr("UNKNOWN S") + CStr(command.m_Num);
                }

                if (static_cast<int>(i) == m_numCurCommand)
                {
                    OutStr += CStr("]");
                }
                OutStr += CStr(" | ");
            }
        }

        if (!m_StateStack2.empty())
        {
            OutStr += CStr("State2 = ");
            for (auto it = m_StateStack2.rbegin(); it != m_StateStack2.rend(); ++it)
            {
                OutStr += m_pDM->m_States[it->m_StateNum].GetName() + CStr("( ");
                for (unsigned i = 0; i < it->m_ParamList.size(); ++i)
                {
                    OutStr += it->m_ParamList[i].ToStr();
                    if (i != it->m_ParamList.size() - 1)
                    {
                        OutStr += CStr(",");
                    }
                }
                OutStr += CStr(" ) | ");
            }

            if (!m_StateStack1.empty())
            {
                DecisionMatrix const* const childDM =
                    m_pDM->m_States[m_StateStack2.back().m_StateNum].m_pChildDecisionMatrix;
                if (childDM)
                {
                    OutStr += CStr(" State1 = ");
                    for (auto it = m_StateStack1.rbegin(); it != m_StateStack1.rend(); ++it)
                    {
                        if (it->m_StateNum >= childDM->m_numStates)
                        {
                            OutStr += CStr("Warning: bad index ") + CStr(it->m_StateNum);
                            continue;
                        }
                        OutStr += childDM->m_States[it->m_StateNum].GetName();
                        OutStr += CStr("( ");
                        for (unsigned i = 0; i < it->m_ParamList.size(); ++i)
                        {
                            OutStr += it->m_ParamList[i].ToStr();
                            if (i != it->m_ParamList.size() - 1)
                            {
                                OutStr += CStr(",");
                            }
                        }
                        OutStr += CStr(" ) | ");
                    }
                }
                else
                {
                    OutStr += CStr("Warning: ");
                    for (auto it = m_StateStack1.rbegin(); it != m_StateStack1.rend(); ++it)
                    {
                        OutStr += CStr(it->m_StateNum) + CStr("( ");
                        for (unsigned i = 0; i < it->m_ParamList.size(); ++i)
                        {
                            OutStr += it->m_ParamList[i].ToStr();
                            if (i != it->m_ParamList.size() - 1)
                            {
                                OutStr += CStr(",");
                            }
                        }
                        OutStr += CStr(" ) | ");
                    }
                }
            }
        }
        return OutStr;
    }

    CStr const& AI::GetCurState1Name()
    {
        // RVA 0x7F2040 - the inner state is named by the sub-matrix hanging off the outer one,
        // so both stacks and the sub-matrix all have to be there for the name to exist.
        if (m_StateStack2.empty())
        {
            return UNDEFINED;
        }
        DecisionMatrix const* const childDM =
            m_pDM->m_States[m_StateStack2.back().m_StateNum].m_pChildDecisionMatrix;
        if (!childDM || m_StateStack1.empty())
        {
            return UNDEFINED;
        }
        int const stateNum = m_StateStack1.back().m_StateNum;
        if (stateNum >= childDM->m_numStates)
        {
            return UNDEFINED;
        }
        return childDM->m_States[stateNum].GetName();
    }

    void AI::InsCommand(int Num, const m3d::AIParam& Param1, const m3d::AIParam& Param2, const m3d::AIParam& Param3)
    {
        // RVA 0x7F4A80 - the command goes in front of the current one and is removed once done;
        // it is not inserted twice in a row. With nothing current it is appended and made current.
        assert(m_numCurCommand >= 0);

        ai::AIMessage command(Num, Param1, Param2, Param3);
        command.m_RemoveAfterFinishing = 1;
        m_CommandProcessed = false;
        if (m_numCurCommand < static_cast<int>(m_Commands.size()))
        {
            if (!(m_Commands[m_numCurCommand] == command))
            {
                m_Commands.insert(m_Commands.begin() + m_numCurCommand, command);
            }
        }
        else
        {
            m_Commands.push_back(command);
            m_numCurCommand = static_cast<int>(m_Commands.size()) - 1;
        }
    }

    void AI::SetState2Param(int ParamNum, m3d::AIParam const& Param)
    {
        // RVA 0x7F2FD0 - the parameter list grows to fit whatever slot is being written.
        if (m_StateStack2.empty())
        {
            return;
        }
        auto& paramList = m_StateStack2.back().m_ParamList;
        if (static_cast<int>(paramList.size()) <= ParamNum)
        {
            paramList.resize(ParamNum + 1, m3d::AIParam());
        }
        paramList[ParamNum] = Param;
    }

    void AI::SetState1Param(int ParamNum, m3d::AIParam const& Param)
    {
        // RVA 0x7F30D0 - the parameter list grows to fit whatever slot is being written.
        if (m_StateStack1.empty())
        {
            return;
        }
        auto& paramList = m_StateStack1.back().m_ParamList;
        if (static_cast<int>(paramList.size()) <= ParamNum)
        {
            paramList.resize(ParamNum + 1, m3d::AIParam());
        }
        paramList[ParamNum] = Param;
    }

    m3d::AIParam AI::GetState2Param(unsigned paramNum)
    {
        // RVA 0x7F2320 - an out-of-range request reads back as an undefined parameter.
        if (m_StateStack2.empty() || paramNum >= m_StateStack2.back().m_ParamList.size())
        {
            return m3d::AIParam();
        }
        return m_StateStack2.back().m_ParamList[paramNum];
    }

    m3d::AIParam AI::GetState1Param(unsigned paramNum)
    {
        // RVA 0x7F2250 - an out-of-range request reads back as an undefined parameter.
        if (m_StateStack1.empty() || paramNum >= m_StateStack1.back().m_ParamList.size())
        {
            return m3d::AIParam();
        }
        return m_StateStack1.back().m_ParamList[paramNum];
    }

    int AI::_CurrentMessage1CommandNum()
    {
        // RVA 0x7F0830 - NOTE: the queue is not checked for being empty.
        return m_Messages1.back().m_Num;
    }

    int AI::_CurrentMessage2CommandNum()
    {
        // RVA 0x7F0820 - NOTE: the queue is not checked for being empty.
        return m_Messages2.back().m_Num;
    }

    int AI::_CurrentState1Num()
    {
        // RVA 0x7F0810 - NOTE: the stack is not checked for being empty.
        return m_StateStack1.back().m_StateNum;
    }

    int AI::_CurrentState2Num()
    {
        // RVA 0x7F0800 - NOTE: unlike GetCurState2Num this does not check that the stack has
        // anything on it.
        return m_StateStack2.back().m_StateNum;
    }
}

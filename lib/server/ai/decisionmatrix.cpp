#include "decisionmatrix.h"
#include "aisignal.h"
#include "aistate.h"
#include "aiparamref.h"
#include <stdexcept>
#include "aimanager.h"
#include "core/log.h"

#include <cstdio>

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddSignal)
{
    auto* matrix = dynamic_cast<ai::DecisionMatrix*>(context->asObject(0, "DecisionMatrix"));
    auto signalName = context->asString(1);
    auto externSignalName = context->asString(2);
    auto functionName = context->asString(3);
    matrix->AddSignal(signalName, externSignalName, functionName);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddState)
{
    auto* matrix = dynamic_cast<ai::DecisionMatrix*>(context->asObject(0, "DecisionMatrix"));
    auto stateName = context->asString(1);
    auto functionName = context->asString(2);
    matrix->AddState(stateName, functionName);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, SetRetValueInterpretation)
{
    auto* matrix = dynamic_cast<ai::DecisionMatrix*>(context->asObject(0, "DecisionMatrix"));
    auto stateName = context->asString(1);
    auto schemeName = context->asString(2);
    auto signalName = context->asString(3);
    matrix->SetRetValueInterpretation(stateName, schemeName, signalName);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, SetDefaultState)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    auto state = context->asString(1);
    decisionMatrix->SetDefaultState(state);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddDefaultStateParam)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    auto state = context->asString(1);
    decisionMatrix->AddDefaultStateParam(state);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, SetExitState)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    auto state = context->asString(1);
    decisionMatrix->SetExitState(state);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, FitMatrix)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    decisionMatrix->FitMatrix();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, SetSaveStackFlag)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    auto param = context->asString(1);
    auto param2 = context->asString(2);
    decisionMatrix->SetSaveStackFlag(param, param2);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, ClearTemporaryParams)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    decisionMatrix->ClearTemporaryParams();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddTemporaryParam)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    auto param = context->asString(1);
    decisionMatrix->AddTemporaryParam(param);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddCommand)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    auto param1 = context->asString(1);
    auto param2 = context->asString(2);
    auto param3 = context->asString(3);
    decisionMatrix->AddCommand(param1, param2, param3);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddSublevel)
{
    auto decisionMatrix = (ai::DecisionMatrix*)context->asObject(0, "DecisionMatrix");
    auto str = context->asString(1);
    auto anotherMatrix = context->asObject(2, "DecisionMatrix");
    decisionMatrix->AddSublevel(str, anotherMatrix);
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DecisionMatrix)
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, AddSignal, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, AddState, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, SetRetValueInterpretation, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, SetDefaultState, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, AddDefaultStateParam, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, SetExitState, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, FitMatrix, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, SetSaveStackFlag, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, ClearTemporaryParams, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, AddTemporaryParam, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, AddCommand, "", "", "")
    RT_CLASS_EXPORT(DecisionMatrix, m3d::METHOD, AddSublevel, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DecisionMatrix);

    DecisionMatrixElement::DecisionMatrixElement()
    {
        this->m_flags = 1;
    }

    // RVA 0x8268D0
    void DecisionMatrixElement::Dump()
    {
        if (m_PassageCommands.empty())
        {
            return;
        }
        // NOTE: the original prints "ClearStack" when the flag is set and an empty string otherwise.
        printf("%s ", (m_flags & 1) ? "ClearStack" : "");
        for (int i = 0; i < (int)m_PassageCommands.size(); ++i)
        {
            m_PassageCommands[i].Dump();
            if (i != (int)m_PassageCommands.size() - 1)
            {
                printf("-> ");
            }
        }
    }

    void DecisionMatrix::ClearTemporaryParams()
    {
        m_tmpParamRefList.clear();
    }

    void DecisionMatrix::FitMatrix()
    {
        m_numStates = m_States.size();
        m_numSignals = m_Signals.size();

        m_Elements.resize(m_numStates * m_numSignals);
    }

    int DecisionMatrix::GetExternSignalMapping(int numExternSignal) const
    {
        return m_ExternSignalMappings[numExternSignal];
    }

    void DecisionMatrix::AddTemporaryParam(char const* functionName)
    {
        auto funcNum = theAIManager->GetFuncNum(functionName);
        if (funcNum == 0xFFFF)
        {
            _LogUnexpectedToken(functionName);
        }
        else
        {
            m_tmpParamRefList.push_back(funcNum);
        }
    }

    // RVA 0x82CD00
    m3d::Object* DecisionMatrix::Clone()
    {
        return new DecisionMatrix(*this);
    }

    unsigned DecisionMatrix::GetSignalNum(CStr const& signalName) const
    {
        for (int i = 0; i < m_Signals.size(); ++i)
        {
            if (signalName == m_Signals[i].GetName())
            {
                return i;
            }
        }
        return 0xFFFF;
    }

    // RVA 0x826C30
    unsigned DecisionMatrix::UnsafeFirstDecision(unsigned stateNum, unsigned signalNum) const
    {
        const DecisionMatrixElement& element = m_Elements[stateNum + signalNum * m_numStates];
        if (element.m_PassageCommands.empty())
        {
            return 0xFFFF;
        }
        return element.m_PassageCommands.front().m_StateNum;
    }

    m3d::Class* DecisionMatrix::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    // RVA 0x826CA0
    void DecisionMatrix::Dump() const
    {
        printf("SIGNALS:\n");
        for (int i = 0; i < (int)m_Signals.size(); ++i)
        {
            printf("SIGNAL[%d] ", i);
            m_Signals[i].Dump();
            printf("\n");
        }

        printf("STATES:\n");
        for (int i = 0; i < (int)m_States.size(); ++i)
        {
            printf("state[%d] ", i);
            m_States[i].Dump();
            printf("\n");
        }

        if (m_Default.m_StateNum != 0xFFFF)
        {
            printf("default = %s( ", m_States[m_Default.m_StateNum].GetName().c_str());
            for (int i = 0; i < (int)m_Default.m_ParamRefList.size(); ++i)
            {
                printf("%d", m_Default.m_ParamRefList[i].m_Num);
                if (i != (int)m_Default.m_ParamRefList.size() - 1)
                {
                    printf(",");
                }
            }
            printf(" )\n");
        }

        if (m_ExitStateNum != 0xFFFF)
        {
            printf("exit = %s\n", m_States[m_ExitStateNum].GetName().c_str());
        }

        printf("DECISIONS:\n");
        for (int state = 0; state < (int)m_States.size(); ++state)
        {
            printf("state[%d] %s\n", state, m_States[state].GetName().c_str());
            for (int signal = 0; signal < (int)m_Signals.size(); ++signal)
            {
                const DecisionMatrixElement& element = m_Elements[state + signal * m_numStates];
                if (element.m_PassageCommands.empty())
                {
                    continue;
                }

                m_Signals[signal].Dump();
                printf(" ");
                for (int k = 0; k < (int)element.m_PassageCommands.size(); ++k)
                {
                    const AIPassageCommand& command = element.m_PassageCommands[k];
                    printf("%s", m_States[command.m_StateNum].GetName().c_str());
                    if (!command.m_ParamRefList.empty())
                    {
                        printf("( ");
                        for (int l = 0; l < (int)command.m_ParamRefList.size(); ++l)
                        {
                            printf("%d", command.m_ParamRefList[l].m_Num);
                            if (l != (int)command.m_ParamRefList.size() - 1)
                            {
                                printf(",");
                            }
                        }
                        printf(" )");
                    }
                    if (k != (int)element.m_PassageCommands.size() - 1)
                    {
                        printf(" -> ");
                    }
                }
                printf("\n");
            }
        }

        for (int i = 0; i < (int)m_States.size(); ++i)
        {
            if (m_States[i].m_pChildDecisionMatrix)
            {
                printf("SUBLEVEL state[%d] %s\n", i, m_States[i].GetName().c_str());
                m_States[i].m_pChildDecisionMatrix->Dump();
            }
        }
    }

    m3d::Object* DecisionMatrix::CreateObject()
    {
        return new DecisionMatrix;
    }

    // RVA 0x82CD80
    void DecisionMatrix::Create(int numStates, int numSignals)
    {
        m_numStates = numStates;
        m_numSignals = numSignals;
        m_States.reserve(numStates);
        m_Signals.reserve(numSignals);
        m_Elements.resize(numSignals * numStates, DecisionMatrixElement());
    }

    // RVA 0x827130
    void DecisionMatrix::LogDump() const
    {
        {
            M3D_LOG_INFO("SIGNALS:");
        }
        for (unsigned int i = 0; i < m_Signals.size(); ++i)
        {
            M3D_LOG_INFO(CStr("SIGNAL[") + CStr(i) + CStr("] = ") + m_Signals[i].GetName());
        }

        {
            M3D_LOG_INFO("STATES:");
        }
        for (unsigned int i = 0; i < m_States.size(); ++i)
        {
            M3D_LOG_INFO(CStr("state[") + CStr(i) + CStr("] = ") + m_States[i].GetName());
        }

        if (m_Default.m_StateNum != 0xFFFF)
        {
            M3D_LOG_INFO(CStr("default = ") + m_States[m_Default.m_StateNum].GetName());
        }

        if (m_ExitStateNum != 0xFFFF)
        {
            M3D_LOG_INFO(CStr("exit = ") + m_States[m_ExitStateNum].GetName());
        }

        CStr OutStr;
        {
            M3D_LOG_INFO("DECISIONS:");
        }
        for (int state = 0; state < (int)m_States.size(); ++state)
        {
            for (int signal = 0; signal < (int)m_Signals.size(); ++signal)
            {
                const DecisionMatrixElement& element = m_Elements[state + signal * m_numStates];
                if (element.m_PassageCommands.empty())
                {
                    continue;
                }

                OutStr = CStr("");
                if ((element.m_flags & 1) == 0)
                {
                    OutStr += CStr("STACK_PUSH -> ");
                }
                for (int k = 0; k < (int)element.m_PassageCommands.size(); ++k)
                {
                    OutStr += m_States[element.m_PassageCommands[k].m_StateNum].GetName();
                    if (k != (int)element.m_PassageCommands.size() - 1)
                    {
                        OutStr += CStr(" -> ");
                    }
                }
                M3D_LOG_INFO(m_States[state].GetName() + CStr(", ") + m_Signals[signal].GetName() + CStr(", ") + OutStr);
            }
        }

        {
            M3D_LOG_INFO("EXTERN:");
        }
        for (unsigned int i = 0; i < 16; ++i)
        {
            if (m_ExternSignalMappings[i] != 0xFFFF)
            {
                M3D_LOG_INFO(CStr("S") + CStr(i) + CStr(" = ") + m_Signals[m_ExternSignalMappings[i]].GetName());
            }
        }
    }

    void DecisionMatrix::SetDefaultState(char const* stateName)
    {
        auto stateNum = GetStateNum(stateName);
        if (stateNum == 0xFFFF)
        {
            _LogUnexpectedToken(stateName);
        }
        else
        {
            this->m_Default.m_StateNum = stateNum;
            this->m_Default.m_ParamRefList.clear();
        }
    }

    void DecisionMatrix::AddCommand(char const* stateName, char const* signalName, char const* newStateName)
    {
        auto stateNum = GetStateNum(stateName);
        if (stateNum == 0xFFFF)
        {
            _LogUnexpectedToken(stateName);
            return;
        }

        auto signalNum = GetSignalNum(signalName);
        if (signalNum == 0xFFFF)
        {
            _LogUnexpectedToken(signalName);
            return;
        }

        auto newStateNum = GetStateNum(newStateName);
        if (newStateNum == 0xFFFF)
        {
            _LogUnexpectedToken(newStateName);
            return;
        }

        AddCommand(stateNum, signalNum, newStateNum, m_tmpParamRefList);
        ClearTemporaryParams();
    }

    void DecisionMatrix::AddCommand(
        unsigned StateID,
        unsigned SignalID,
        unsigned PassStateID,
        std::vector<AIParamRef, std::allocator<AIParamRef>> const& ParamRefList)
    {
        if (StateID == 0xFFFF || SignalID == 0xFFFF || PassStateID == 0xFFFF)
        {
            M3D_LOG_ERR("AI Error: state or Signal absent");
            return;
        }

        if (m_numStates * m_numSignals != m_Elements.size())
        {
            M3D_LOG_ERR("AI Error: state or Signal absent");
            return;
        }

        ai::AIPassageCommand command;
        command.m_StateNum = PassStateID;
        command.m_ParamRefList = ParamRefList;

        m_Elements[StateID + SignalID * this->m_numStates].m_PassageCommands.push_back(std::move(command));
    }

    int DecisionMatrix::GetExitStateNum() const
    {
        return m_ExitStateNum;
    }

    int DecisionMatrix::NumStates() const
    {
        return m_numStates;
    }

    void DecisionMatrix::AddState(char const* stateName, char const* functionName)
    {
        CStr functionNameStr = functionName;

        auto funcNum = theAIManager->GetFuncNum(functionNameStr);
        if (funcNum == 0xFFFF)
        {
            _LogUnexpectedToken(functionNameStr);
        }
        else
        {
            AIState aiState;
            aiState.Set(stateName, funcNum);
            AddState(aiState);
        }
    }

    void DecisionMatrix::AddState(AIState const& state)
    {
        if (GetStateNum(state.GetName()) == 0xFFFF)
        {
            m_States.push_back(state);
        }
        else
        {
            M3D_LOG_ERR("AI Error: state already present");
        }
    }

    void DecisionMatrix::SetSaveStackFlag(char const* stateName, char const* signalName)
    {
        auto stateNum = GetStateNum(stateName);
        if (stateNum == 0xFFFF)
        {
            _LogUnexpectedToken(stateName);
            return;
        }

        auto signalNum = GetSignalNum(signalName);
        if (signalNum == 0xFFFF)
        {
            _LogUnexpectedToken(signalName);
            return;
        }

        m_Elements[stateNum + signalNum * m_numStates].m_flags &= ~1;
    }

    // RVA 0x826C10
    void DecisionMatrix::SetSaveStackFlag(int stateNum, int signalNum)
    {
        m_Elements[stateNum + signalNum * m_numStates].m_flags &= ~1;
    }

    void DecisionMatrix::AddSignal(char const* signalName, char const* externSignalName, char const* functionName)
    {
        int schemeNum = 0xFFFF;
        CStr externSignalNameStr = externSignalName;
        if (!externSignalNameStr.empty())
        {
            schemeNum = theAIManager->GetSchemeNum(externSignalNameStr);
            if (schemeNum == 0xFFFF)
            {
                _LogUnexpectedToken(externSignalNameStr);
            }
        }

        int funcNum = 0xFFFF;
        CStr functionNamelNameStr = functionName;
        if (!functionNamelNameStr.empty())
        {
            funcNum = theAIManager->GetFuncNum(functionNamelNameStr);
            if (funcNum == 0xFFFF)
            {
                _LogUnexpectedToken(functionNamelNameStr);
            }
        }

        AISignal aiSignal;
        aiSignal.Set(signalName, funcNum);
        AddSignal(aiSignal);

        if (schemeNum != 0xFFFF)
        {
            auto signalNum = GetSignalNum(signalName);
            m_ExternSignalMappings[schemeNum] = signalNum;
        }
    }

    void DecisionMatrix::AddSignal(AISignal const& signal)
    {
        if (GetSignalNum(signal.GetName()) == 0xFFFF)
        {
            m_Signals.push_back(signal);
        }
        else
        {
            M3D_LOG_ERR("AI Error: Signal already present");
        }
    }

    // RVA 0x7EF5F0
    int DecisionMatrix::NumSignals() const
    {
        return m_numSignals;
    }

    AISignal const& DecisionMatrix::GetSignal(int signalNum) const
    {
        return m_Signals[signalNum];
    }

    DecisionMatrix::~DecisionMatrix() = default;

    void DecisionMatrix::AddDefaultStateParam(char const* functionName)
    {
        auto funcNum = theAIManager->GetFuncNum(functionName);
        if (funcNum == 0xFFFF)
        {
            _LogUnexpectedToken(functionName);
        }
        else
        {
            this->m_Default.m_ParamRefList.push_back(funcNum);
        }
    }

    AIState const& DecisionMatrix::GetState(int stateNum) const
    {
        return m_States[stateNum];
    }

    // RVA 0x7EFBD0
    DecisionMatrix const* DecisionMatrix::GetSubmatrix(int stateNum) const
    {
        return m_States[stateNum].m_pChildDecisionMatrix;
    }

    AIPassageCommand const& DecisionMatrix::GetDefault() const
    {
        return m_Default;
    }

    m3d::Class* DecisionMatrix::GetClass() const
    {
        return RT_CLASS_LOCAL(DecisionMatrix);
    }

    void DecisionMatrix::SetRetValueInterpretation(char const* stateName, char const* schemeName, char const* signalName)
    {
        unsigned signalNum = 0xFFFF;
        auto stateNum = GetStateNum(stateName);
        if (stateNum == 0xFFFF)
        {
            _LogUnexpectedToken(stateName);
        }
        else
        {
            auto& state = m_States[stateNum];
            auto schemeNum = theAIManager->GetSchemeNum(schemeName);
            if (schemeNum != 0xFFFF)
            {
                if (CStr(signalName) == "STACK_POP")
                {
                    signalNum = 65534;
                }
                else
                {
                    signalNum = GetSignalNum(signalName);
                    if (signalNum == 0xFFFF)
                    {
                        _LogUnexpectedToken(signalName);
                        return;
                    }
                }
                if (schemeNum < 16)
                {
                    state.m_SignalIDs[schemeNum] = signalNum;
                }
                return;
            }
            _LogUnexpectedToken(schemeName);
        }
    }

    void DecisionMatrix::SetExitState(char const* stateName)
    {
        auto stateNum = GetStateNum(stateName);
        if (stateNum == 0xFFFF)
        {
            _LogUnexpectedToken(stateName);
            return;
        }

        m_ExitStateNum = stateNum;
    }

    void DecisionMatrix::AddSublevel(unsigned stateID, DecisionMatrix* pSubDM)
    {
        if (stateID != 0xFFFF)
        {
            delete m_States[stateID].m_pChildDecisionMatrix;
            m_States[stateID].m_pChildDecisionMatrix = pSubDM;
        }
    }

    void DecisionMatrix::AddSublevel(char const* stateName, m3d::Object* pSubDM)
    {
        auto stateNum = GetStateNum(stateName);
        if (stateNum == 0xFFFF)
        {
            _LogUnexpectedToken(stateName);
            return;
        }

        if (pSubDM->GetClass() == RT_CLASS_LOCAL(DecisionMatrix))
        {
            AddSublevel(stateNum, RT_DYNCAST(pSubDM, DecisionMatrix));
            return;
        }

        M3D_LOG_ERR("Error: Unexpected object in AddSublevel");
    }

    DecisionMatrixElement const* DecisionMatrix::UnsafeGetDecision(int stateNum, int signalNum) const
    {
        return &m_Elements[stateNum + signalNum * m_numStates];
    }

    int DecisionMatrix::GetStateNum(CStr const& stateName) const
    {
        for (int i = 0; i < m_States.size(); ++i)
        {
            if (stateName == m_States[i].GetName())
            {
                return i;
            }
        }
        return 0xFFFF;
    }

    // RVA 0x82CB40
    DecisionMatrix::DecisionMatrix(DecisionMatrix const&)
    {
        // NOTE: the original always asserts; decision matrices cannot be cloned.
        SYS_ERROR("0");
    }

    DecisionMatrix::DecisionMatrix()
    {
        this->m_Default.m_StateNum = 0xFFFF;
        this->m_ExitStateNum = 0xFFFF;
        this->m_ExternSignalMappings[0] = 0xFFFF;
        this->m_ExternSignalMappings[1] = 0xFFFF;
        this->m_ExternSignalMappings[2] = 0xFFFF;
        this->m_ExternSignalMappings[3] = 0xFFFF;
        this->m_ExternSignalMappings[4] = 0xFFFF;
        this->m_ExternSignalMappings[5] = 0xFFFF;
        this->m_ExternSignalMappings[6] = 0xFFFF;
        this->m_ExternSignalMappings[7] = 0xFFFF;
        this->m_ExternSignalMappings[8] = 0xFFFF;
        this->m_ExternSignalMappings[9] = 0xFFFF;
        this->m_ExternSignalMappings[10] = 0xFFFF;
        this->m_ExternSignalMappings[11] = 0xFFFF;
        this->m_ExternSignalMappings[12] = 0xFFFF;
        this->m_ExternSignalMappings[13] = 0xFFFF;
        this->m_ExternSignalMappings[14] = 0xFFFF;
        this->m_ExternSignalMappings[15] = 0xFFFF;
    }

    // RVA 0x826A40
    DecisionMatrixElement const& DecisionMatrix::_GetElement(int signalNum, int stateNum) const
    {
        return m_Elements[stateNum + signalNum * m_numStates];
    }

    // RVA 0x826A60
    DecisionMatrixElement& DecisionMatrix::_GetElement(int signalNum, int stateNum)
    {
        return m_Elements[stateNum + signalNum * m_numStates];
    }

    // RVA 0x8263F0
    void DecisionMatrix::_LogUnexpectedToken(CStr const& token)
    {
        M3D_LOG_INFO(CStr("Unexpected token: ") + token);
    }
}  // namespace ai

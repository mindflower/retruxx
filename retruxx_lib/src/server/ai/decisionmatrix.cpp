#include "decisionmatrix.h"
#include "aisignal.h"
#include "aistate.h"
#include "aiparamref.h"
#include <stdexcept>
#include "aimanager.h"
#include "core/log.h"

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
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddDefaultStateParam)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, SetExitState)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, FitMatrix)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, SetSaveStackFlag)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, ClearTemporaryParams)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddTemporaryParam)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddCommand)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddSublevel)
{
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrixElement::Dump()
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::ClearTemporaryParams()
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::FitMatrix()
    {
        throw std::logic_error("Not implemented");
    }

    int DecisionMatrix::GetExternSignalMapping(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddTemporaryParam(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DecisionMatrix::Clone()
    {
        throw std::logic_error("Not implemented");
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

    unsigned DecisionMatrix::UnsafeFirstDecision(unsigned, unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DecisionMatrix::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    void DecisionMatrix::Dump() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* DecisionMatrix::CreateObject()
    {
        return new DecisionMatrix;
    }

    void DecisionMatrix::Create(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::LogDump() const
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::SetDefaultState(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddCommand(char const*, char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddCommand(unsigned, unsigned, unsigned,
        std::vector<AIParamRef, std::allocator<AIParamRef>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    int DecisionMatrix::GetExitStateNum() const
    {
        throw std::logic_error("Not implemented");
    }

    int DecisionMatrix::NumStates() const
    {
        throw std::logic_error("Not implemented");
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

    void DecisionMatrix::SetSaveStackFlag(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::SetSaveStackFlag(int, int)
    {
        throw std::logic_error("Not implemented");
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

    int DecisionMatrix::NumSignals() const
    {
        throw std::logic_error("Not implemented");
    }

    AISignal const& DecisionMatrix::GetSignal(int) const
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix::~DecisionMatrix()
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddDefaultStateParam(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    AIState const& DecisionMatrix::GetState(int) const
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix const* DecisionMatrix::GetSubmatrix(int) const
    {
        throw std::logic_error("Not implemented");
    }

    AIPassageCommand const& DecisionMatrix::GetDefault() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* DecisionMatrix::GetClass() const
    {
        return RT_CLASS_LOCAL(DecisionMatrix);
    }

    void DecisionMatrix::SetRetValueInterpretation(const char* stateName, const char* schemeName, const char* signalName)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::SetExitState(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddSublevel(unsigned, DecisionMatrix*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddSublevel(char const*, m3d::Object*)
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrixElement const* DecisionMatrix::UnsafeGetDecision(int, int) const
    {
        throw std::logic_error("Not implemented");
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

    DecisionMatrix::DecisionMatrix(DecisionMatrix const&)
    {
        throw std::logic_error("Not implemented");
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

    DecisionMatrixElement const& DecisionMatrix::_GetElement(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrixElement& DecisionMatrix::_GetElement(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::_LogUnexpectedToken(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }
}

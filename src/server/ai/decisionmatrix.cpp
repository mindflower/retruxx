#include "decisionmatrix.h"
#include "aisignal.h"
#include "aistate.h"
#include "aiparamref.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddSignal)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, AddState)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(DecisionMatrix, SetRetValueInterpretation)
{
    throw std::logic_error("Not implemented");
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

    unsigned DecisionMatrix::GetSignalNum(CStr const&) const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    void DecisionMatrix::AddState(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddState(AIState const&)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::SetSaveStackFlag(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::SetSaveStackFlag(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddSignal(char const*, char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::AddSignal(AISignal const&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void DecisionMatrix::SetRetValueInterpretation(char const*, char const*, char const*)
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

    int DecisionMatrix::GetStateNum(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix::DecisionMatrix(DecisionMatrix const&)
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix::DecisionMatrix()
    {
        throw std::logic_error("Not implemented");
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

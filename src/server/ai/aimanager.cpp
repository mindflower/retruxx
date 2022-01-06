#include "aimanager.h"
#include <stdexcept>
#include "core/aiparam.h"
#include <server/ai/aifunc.h>
#include <server/ai/aimatrix.h>

namespace ai
{
    AIManager* theAIManager = nullptr;

    DecisionMatrix* AIManager::LoadMatrix(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AIManager::GetMatrixNum(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void AIManager::RegisterMatrix(CStr const&, DecisionMatrix*)
    {
        throw std::logic_error("Not implemented");
    }

    void AIManager::Dump()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* AIManager::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    int AIManager::RegisterFunc(CStr const&, m3d::AIParam(Obj*))
    {
        throw std::logic_error("Not implemented");
    }

    void AIManager::LogDump()
    {
        throw std::logic_error("Not implemented");
    }

    int AIManager::GetSchemeNum(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr AIManager::GetMatrixName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix* AIManager::GetDecisionMatrixPtr(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* AIManager::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* AIManager::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    AIManager::~AIManager()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam AIManager::AIAction(int, Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    int AIManager::GetFuncNum(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* AIManager::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix* AIManager::CreateNewDecisionMatrix()
    {
        throw std::logic_error("Not implemented");
    }

    AIManager::AIManager()
    {
        throw std::logic_error("Not implemented");
    }

    AIManager::AIManager(AIManager const&)
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix* AIManager::ReadNewMatrix(char const*)
    {
        throw std::logic_error("Not implemented");
    }
}

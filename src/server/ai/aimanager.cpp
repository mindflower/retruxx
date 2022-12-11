#include "aimanager.h"
#include <stdexcept>
#include "core/aiparam.h"
#include <server/ai/aifunc.h>
#include <server/ai/aimatrix.h>

RT_CLASS_EXPORT_METHOD_DEFINE(AIManager, CreateNewDecisionMatrix)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(AIManager, LoadMatrix)
{
    throw std::logic_error("Not implemented");
}

namespace ai
{
    AIManager* theAIManager = nullptr;

    RT_CLASS_EXPORTS_BEGIN(AIManager)
		RT_CLASS_EXPORT(AIManager, m3d::METHOD, CreateNewDecisionMatrix, "", "", "")
        RT_CLASS_EXPORT(AIManager, m3d::METHOD, LoadMatrix, "", "", "")
	RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(AIManager);

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
        return RT_CLASS_LOCAL(Object);
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
        return new AIManager;
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

    AIManager::AIManager() :
        m_Schemes{"S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "S10", "S11", "S12", "S13", "S14", "S15"}
    {
    }

    AIManager::AIManager(AIManager const&)
    {
        throw std::logic_error("Not implemented");
    }

    DecisionMatrix* AIManager::ReadNewMatrix(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void SetAIManager(AIManager* pAIManager)
    {
        theAIManager = pAIManager;
    }
}

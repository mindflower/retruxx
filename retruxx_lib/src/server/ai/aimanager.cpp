#include "aimanager.h"
#include <stdexcept>
#include "core/aiparam.h"
#include <server/ai/aifunc.h>
#include <server/ai/aimatrix.h>

#include "decisionmatrix.h"
#include "core/kernel.h"
#include "core/log.h"
#include "script/funcstack.h"
#include "script/scriptserver.h"

RT_CLASS_EXPORT_METHOD_DEFINE(AIManager, CreateNewDecisionMatrix)
{
    context->asObject(0, "AIManager");
    auto* obj = m3d::g_Kernel->New("DecisionMatrix");
    context->pushObject(obj);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(AIManager, LoadMatrix)
{
    auto* aiManager = (ai::AIManager*)context->asObject(0, "AIManager");
    auto str = context->asString(1);
    auto mat = aiManager->LoadMatrix(str);
    context->pushObject(mat);
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(AIManager)
		RT_CLASS_EXPORT(AIManager, m3d::METHOD, CreateNewDecisionMatrix, "", "", "")
        RT_CLASS_EXPORT(AIManager, m3d::METHOD, LoadMatrix, "", "", "")
	RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(AIManager);

    DecisionMatrix* AIManager::LoadMatrix(char const* fileName)
    {
        auto matrixNum = GetMatrixNum(fileName);
        if (matrixNum != 0xFFFF)
        {
            return m_Matrix[matrixNum].GetPtr();
        }

        auto newMatrix = ReadNewMatrix(fileName);
        ai::AIMatrix matrix(fileName, newMatrix);
        m_Matrix.push_back(std::move(matrix));

        return newMatrix;
    }

    int AIManager::GetMatrixNum(CStr const& name) const
    {
        for (int i = 0; i < m_Matrix.size(); ++i)
        {
            if (m_Matrix[i].GetName() == name)
            {
                return i;
            }
        }
        return 0xFFFF;
    }

    void AIManager::RegisterMatrix(CStr const& name, DecisionMatrix* pDM)
    {
        if (auto const matrixNum = GetMatrixNum(name); matrixNum == 0xFFFF)
        {
            m_Matrix.emplace_back(name, pDM);
        }
        else
        {
            m_Matrix[matrixNum].Set(name, pDM);
        }
    }

    void AIManager::Dump()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* AIManager::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int AIManager::RegisterFunc(const CStr& name, m3d::AIParam(*funcAction)(ai::Obj*))
    {
        auto const num = GetFuncNum(name);
        if (num == 0xFFFF)
        {
            m_Actions.emplace_back(name, funcAction);
            return m_Actions.size() - 1;
        }
        m_Actions[num].Set(name, funcAction);
        return num;
    }

    void AIManager::LogDump()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AIManager::GetSchemeNum(CStr const& word) const
    {
        for (int i = 0; i < m_Schemes.size(); ++i)
        {
            if (m_Schemes[i] == word)
            {
                return i;
            }
        }
        return 0xFFFF;
    }

    CStr AIManager::GetMatrixName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DecisionMatrix* AIManager::GetDecisionMatrixPtr(int MatrixNum) const
    {
        return this->m_Matrix[MatrixNum].GetPtr();
    }

    m3d::Object* AIManager::CreateObject()
    {
        return new AIManager;
    }

    m3d::Class* AIManager::GetClass() const
    {
        return RT_CLASS_LOCAL(AIManager);
    }

    AIManager::~AIManager()
    {
        for (auto& mat : m_Matrix)
        {
            mat.Delete();
        }
    }

    m3d::AIParam AIManager::AIAction(int, Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AIManager::GetFuncNum(CStr const& name) const
    {
        for (int i = 0; i < m_Actions.size(); ++i)
        {
            if (m_Actions[i].GetName() == name)
            {
                return i;
            }
        }
        return 0xFFFF;
    }

    m3d::Object* AIManager::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DecisionMatrix* AIManager::CreateNewDecisionMatrix()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AIManager::AIManager() :
        m_Schemes{"S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "S10", "S11", "S12", "S13", "S14", "S15"}
    {
    }

    AIManager::AIManager(AIManager const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DecisionMatrix* AIManager::ReadNewMatrix(char const* fileName)
    {
        auto extension = strchr(fileName, '.');
        if (CStr(extension) == ".lua")
        {
            auto& scriptServer = M3D_KERNEL->GetScriptServer();

            m3d::sArgStack argStack;
            argStack.newIn()->SetS(fileName);
            if (auto res = scriptServer.callScriptFunc("ReadMatrix", argStack, 1))
            {
                M3D_LOG_ERR(scriptServer.getFormatedScriptErrorDesc(res));
            }
            if (argStack.getNumOutArgs() != 1)
            {
                M3D_LOG_ERR("Error in return results: invalid number of return values");
                return 0;
            }

            auto* out = argStack.popOut();
            if (out->m_type != m3d::sArg::ARGTYPE_OBJECT)
            {
                M3D_LOG_ERR("Error in return results: invalid type of return value");
                return 0;
            }

            return dynamic_cast<DecisionMatrix*>(out->GetO());
        }
        M3D_LOG_ERR("Error: old-style matrices in *.ai are not supportted.");
        return 0;
    }

    void SetAIManager(AIManager* pAIManager)
    {
        theAIManager = pAIManager;
    }
}

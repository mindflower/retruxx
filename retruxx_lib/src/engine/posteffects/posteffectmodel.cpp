#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/ref_ptr.h>
#include <posteffects/posteffectmodel.h>

PostEffectModel::PostEffectModel(std::map<CStr, float*>* vList) : m_varList(vList)
{
}

bool PostEffectModel::IsInstant() const
{
    throw std::logic_error("Not implemented");
}

bool PostEffectModel::FindByName(CStr name)
{
    return m_name == name;
}

CStr& PostEffectModel::LoadFromXml(m3d::cmn::XmlFile*xmlFile, m3d::cmn::XmlNode const* xmlNode)
{
    m_allInstant = true;
    m3d::SafeStrAttrib(m_name, xmlNode, "Name");
    CStr condUnite;
    m3d::SafeStrAttrib(condUnite, xmlNode, "ExitConditionsUnite");
    m_conditionUnite = condUnite == "And"? CU_AND : CU_OR;

    ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    int unitNum = 0;
    for (xmlNode->GetFirstChild(node, "Unit"); !node->IsEmpty(); node->GetNextSibling(node, "Unit"))
    {
        CStr varName;
        m3d::SafeStrAttrib(varName, node, "VarName");

        EffectUnit tmpUnit;
        tmpUnit.m_var = (*m_varList)[varName];
        m3d::SafeFloatAttrib(tmpUnit.m_params[0], node, "Param1");
        m3d::SafeFloatAttrib(tmpUnit.m_params[1], node, "Param2");
        m3d::SafeFloatAttrib(tmpUnit.m_params[2], node, "Param3");
        m3d::SafeFloatAttrib(tmpUnit.m_params[3], node, "Param4");

        CStr action;
        m3d::SafeStrAttrib(action, node, "Action");
        tmpUnit.m_action = action == "Save" ? OA_SAVE_STATE : (action == "Restore" ? OA_RESTORE_STATE : OA_NONE);
        m_effectUnits.push_back(tmpUnit);

        if (tmpUnit.m_params[0] != 0.0
            || tmpUnit.m_params[1] != 0.0
            || tmpUnit.m_params[2] != 0.0
            || tmpUnit.m_params[3] != 0.0)
        {
            m_dynamicUnits.push_back(unitNum);
            m_allInstant = false;
        }
        ++unitNum;
    }

    for (xmlNode->GetFirstChild(node, "ExitConditions"); !node->IsEmpty(); node->GetNextSibling(node, "ExitConditions"))
    {
        CStr varName;
        m3d::SafeStrAttrib(varName, node, "VarName");

        ExitCondition tmpCond;
        tmpCond.m_var = (*m_varList)[varName];
        m3d::SafeFloatAttrib(tmpCond.m_limit, node, "LimitVal");

        //TODO: check this
        CStr cond;
        m3d::SafeStrAttrib(cond, node, "Condition");

        tmpCond.m_evalIncrease = true;
        m_conditionList.push_back(tmpCond);
    }

    for (auto it = begin(m_conditionList); it != end(m_conditionList); ++it)
    {
        for (auto& effect : m_effectUnits)
        {
            if (effect.m_var == it->m_var)
            {
                effect.m_condItor = it;
                break;
            }
        }
    }

    xmlNode->GetFirstChild(node, "VariableValue");
    if (node->IsEmpty())
    {
        m_pVarVal = nullptr;
        return m_name;
    }
    
    m3d::SafeIntAttrib(unitNum, node, "UnitNumb");
    CStr paramName;
    m3d::SafeStrAttrib(paramName, node, "ParamName");
    M3D_ASSERT(unitNum < m_effectUnits.size());

    if (paramName == "InitVal")
    {
        m_pVarVal = &m_effectUnits[unitNum].m_initVal;
    }
    else if (paramName == "Param1")
    {
        m_pVarVal = &m_effectUnits[unitNum].m_params[0];
    }
    else if (paramName == "Param2")
    {
        m_pVarVal = &m_effectUnits[unitNum].m_params[1];
    }
    else if (paramName == "Param3")
    {
        m_pVarVal = &m_effectUnits[unitNum].m_params[2];
    }
    else if (paramName == "Param4")
    {
        m_pVarVal = &m_effectUnits[unitNum].m_params[3];
    }
    return m_name;
}

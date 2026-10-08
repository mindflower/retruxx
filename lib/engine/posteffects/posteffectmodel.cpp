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
    // RVA 0x8B1610
    return m_allInstant;
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
    // NOTE: tmpUnit lives outside the loop in the shipped build, so a unit that
    // omits an attribute inherits the previous unit's value for it.
    EffectUnit tmpUnit;
    for (xmlNode->GetFirstChild(node, "Unit"); !node->IsEmpty(); node->GetNextSibling(node, "Unit"))
    {
        CStr varName;
        m3d::SafeStrAttrib(varName, node, "VarName");

        tmpUnit.m_var = (*m_varList)[varName];
        m3d::SafeFloatAttrib(tmpUnit.m_initVal, node, "InitVal");
        m3d::SafeFloatAttrib(tmpUnit.m_params[0], node, "Param1");
        m3d::SafeFloatAttrib(tmpUnit.m_params[1], node, "Param2");
        m3d::SafeFloatAttrib(tmpUnit.m_params[2], node, "Param3");
        m3d::SafeFloatAttrib(tmpUnit.m_params[3], node, "Param4");

        CStr action;
        m3d::SafeStrAttrib(action, node, "Action");
        tmpUnit.m_action = action == "Save" ? OA_SAVE_STATE : (action == "Restore" ? OA_RESTORE_STATE : OA_NONE);
        // "no exit condition" - see the fix-up pass below.
        tmpUnit.m_condItor = ConditionList::iterator();
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

    // NOTE: as with tmpUnit, the shipped build reuses one tmpCond across the
    // loop, so a condition without a LimitVal inherits the previous one's.
    ExitCondition tmpCond;
    for (xmlNode->GetFirstChild(node, "ExitConditions"); !node->IsEmpty(); node->GetNextSibling(node, "ExitConditions"))
    {
        CStr varName;
        m3d::SafeStrAttrib(varName, node, "VarName");

        tmpCond.m_var = (*m_varList)[varName];
        m3d::SafeFloatAttrib(tmpCond.m_limit, node, "LimitVal");

        // NOTE: the shipped code reads the "Condition" attribute and then
        // discards it - the direction is always recomputed from the variable's
        // starting value when a PostEffect is constructed, so m_evalIncrease is
        // seeded to true here regardless.
        CStr cond;
        m3d::SafeStrAttrib(cond, node, "Condition");

        tmpCond.m_evalIncrease = true;
        m_conditionList.push_back(tmpCond);
    }

    // Link each unit to the exit condition that watches the same variable. The
    // shipped build stores a raw pointer and leaves it null when a unit has no
    // condition; end() is the equivalent sentinel for a real iterator.
    for (auto& effect : m_effectUnits)
    {
        effect.m_condItor = end(m_conditionList);
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
        // NOTE: when no unit uses the condition's variable, the shipped loop
        // still writes the iterator through the past-the-end unit pointer -
        // a stray write we deliberately do not reproduce.
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
    M3D_ASSERT(unitNum < static_cast<int>(m_effectUnits.size()));

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

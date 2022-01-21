#pragma once
#include <map>
#include <vector>
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }
}

enum CONDITION_UNITE
{
    CU_AND = 0x0,
    CU_OR = 0x1,
};

enum OPT_ACTIONS
{
    OA_NONE = 0x0,
    OA_SAVE_STATE = 0x1,
    OA_RESTORE_STATE = 0x2,
};

class PostEffectModel
{
public:
    struct ExitCondition
    {
        float* m_var;
        float m_limit;
        bool m_evalIncrease;
    };

    class EffectUnit
    {
    public:
        float* m_var;
        float m_initVal;
        float m_params[4];
        OPT_ACTIONS m_action;
        std::vector<ExitCondition>::iterator m_condItor;
    };


public:
    PostEffectModel(std::map<CStr, float*>*);
    bool IsInstant() const;
    bool FindByName(CStr);
    ~PostEffectModel();
    CStr& LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

public:
    CStr m_name;
    std::map<CStr, float*>* m_varList = nullptr;
    std::vector<ExitCondition> m_conditionList;
    std::vector<EffectUnit> m_effectUnits;
    std::vector<int> m_dynamicUnits;
    bool m_allInstant = false;
    CONDITION_UNITE m_conditionUnite;
    float* m_pVarVal = nullptr;
};

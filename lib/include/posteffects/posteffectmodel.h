#pragma once
#include <map>
#include <vector>
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
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

class PostEffect;

class PostEffectModel
{
    // A running effect drives the model's units and exit conditions directly.
    friend class PostEffect;

    using VarList = std::map<CStr, float*, std::less<CStr>, std::allocator<std::pair<CStr const, float*> > >;

public:
    PostEffectModel(std::map<CStr, float*, std::less<CStr>, std::allocator<std::pair<CStr const, float*> > >* vList);
    CStr& LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    bool IsInstant() const;
    bool FindByName(const CStr name);

private:
    CStr m_name;
    std::map<CStr, float*, std::less<CStr>, std::allocator<std::pair<CStr const, float*> > >* m_varList;

    struct ExitCondition
    {
        float* m_var;
        float m_limit;
        bool m_evalIncrease;
    };

    using ConditionList = std::vector<PostEffectModel::ExitCondition, std::allocator<PostEffectModel::ExitCondition> >;

private:
    std::vector<PostEffectModel::ExitCondition, std::allocator<PostEffectModel::ExitCondition> > m_conditionList;

    struct EffectUnit
    {
        float* m_var;
        float m_initVal;
        float m_params[4];
        OPT_ACTIONS m_action;
        std::vector<PostEffectModel::ExitCondition, std::allocator<PostEffectModel::ExitCondition> >::iterator m_condItor;
    };

    using UnitsList = std::vector<PostEffectModel::EffectUnit, std::allocator<PostEffectModel::EffectUnit> >;

private:
    std::vector<PostEffectModel::EffectUnit, std::allocator<PostEffectModel::EffectUnit> > m_effectUnits;

    using PUnitsList = std::vector<int, std::allocator<int> >;

private:
    std::vector<int, std::allocator<int> > m_dynamicUnits;
    bool m_allInstant;
    char Padding_265[3];
    CONDITION_UNITE m_conditionUnite;
    float* m_pVarVal;
};

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
    /* 0x0000 */ CStr m_name;
    /* 0x000c */ std::map<CStr, float*, std::less<CStr>, std::allocator<std::pair<CStr const, float*> > >* m_varList;

    struct ExitCondition
    {
        /* 0x0000 */ float* m_var;
        /* 0x0004 */ float m_limit;
        /* 0x0008 */ bool m_evalIncrease;
    }; /* size: 0x000c */

    using ConditionList = std::vector<PostEffectModel::ExitCondition, std::allocator<PostEffectModel::ExitCondition> >;

private:
    /* 0x0010 */ std::vector<PostEffectModel::ExitCondition, std::allocator<PostEffectModel::ExitCondition> > m_conditionList;

    struct EffectUnit
    {
        /* 0x0000 */ float* m_var;
        /* 0x0004 */ float m_initVal;
        /* 0x0008 */ float m_params[4];
        /* 0x0018 */ OPT_ACTIONS m_action;
        /* 0x001c */ std::vector<PostEffectModel::ExitCondition, std::allocator<PostEffectModel::ExitCondition> >::iterator m_condItor;
    }; /* size: 0x0020 */

    using UnitsList = std::vector<PostEffectModel::EffectUnit, std::allocator<PostEffectModel::EffectUnit> >;

private:
    /* 0x0020 */ std::vector<PostEffectModel::EffectUnit, std::allocator<PostEffectModel::EffectUnit> > m_effectUnits;

    using PUnitsList = std::vector<int, std::allocator<int> >;

private:
    /* 0x0030 */ std::vector<int, std::allocator<int> > m_dynamicUnits;
    /* 0x0040 */ bool m_allInstant;
    /* 0x0041 */ char Padding_265[3];
    /* 0x0044 */ CONDITION_UNITE m_conditionUnite;
    /* 0x0048 */ float* m_pVarVal;
}; /* size: 0x004c */

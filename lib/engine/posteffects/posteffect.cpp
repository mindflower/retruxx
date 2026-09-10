#include "posteffects/posteffect.h"

#include <cmath>
#include <core/kernel.h>
#include <core/timer.h>
#include <posteffects/posteffectmanager.h>
#include <posteffects/posteffectmodel.h>

namespace
{
    // Every post-effect variable is a float[3]: [0] is the live value the
    // shaders read, [1] the value saved by an OA_SAVE_STATE unit and [2] a
    // non-zero "a state is currently saved" flag.
    int const VAR_VALUE = 0;
    int const VAR_SAVED_VALUE = 1;
    int const VAR_SAVED_FLAG = 2;

    // An exit condition whose limit is -1 tracks the saved value instead of a
    // constant. (Taken apart into plain arguments because ExitCondition itself
    // is private to PostEffectModel.)
    float LimitOf(float const* var, float limit)
    {
        return limit == -1.0f ? var[VAR_SAVED_VALUE] : limit;
    }

    // A condition is met once the variable has reached its limit from whichever
    // side it started on.
    bool IsConditionMet(float const* var, float limit, bool evalIncrease)
    {
        float const effectiveLimit = LimitOf(var, limit);
        return evalIncrease ? !(var[VAR_VALUE] < effectiveLimit) : !(effectiveLimit < var[VAR_VALUE]);
    }
}  // namespace

PostEffect::PostEffect(
    PostEffectModel* model,
    float effParam,
    PostEffectManager* parent,
    int nSeq,
    CStr const& seqName,
    bool /*isBeKilling*/) :
    // NOTE: isBeKilling is accepted but never read by the shipped constructor
    // (RVA 0x8B1960); AddEffect still computes and passes it.
    m_parent(parent),
    m_model(model),
    m_sequenceNumb(nSeq),
    m_sequenceName(seqName)
{
    m_startTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();

    if (model->m_pVarVal)
    {
        *model->m_pVarVal = effParam;
    }

    for (size_t i = 0; i < model->m_effectUnits.size(); ++i)
    {
        PostEffectModel::EffectUnit& unit = model->m_effectUnits[i];

        if (unit.m_var[VAR_SAVED_FLAG] == 0.0f && unit.m_action == OA_SAVE_STATE)
        {
            unit.m_var[VAR_SAVED_FLAG] = 1.0f;
            unit.m_var[VAR_SAVED_VALUE] = unit.m_var[VAR_VALUE];
        }

        if (unit.m_initVal != -1.0f)
        {
            unit.m_var[VAR_VALUE] = unit.m_initVal;
        }

        if (unit.m_action == OA_RESTORE_STATE)
        {
            unit.m_var[VAR_SAVED_FLAG] = 0.0f;
            unit.m_var[VAR_VALUE] = unit.m_var[VAR_SAVED_VALUE];
        }

        if (unit.m_condItor != model->m_conditionList.end())
        {
            // Decide which way the variable has to travel to reach its limit.
            unit.m_condItor->m_evalIncrease =
                unit.m_var[VAR_VALUE] < LimitOf(unit.m_var, unit.m_condItor->m_limit);
        }
    }

    if (model->m_allInstant)
    {
        // Nothing to animate - hand straight over to the next sequence unit.
        UnRegistedMe();
    }
}

bool PostEffect::FindByName(CStr const name)
{
    // RVA 0x668140
    return m_sequenceName == name;
}

void PostEffect::UnRegistedMe()
{
    // RVA 0x8B1620
    m_parent->UnregisterEffect(this);
    m_parent->AddEffect(m_sequenceName, 0.0f, m_sequenceNumb + 1);
}

void PostEffect::ApplyDynamic()
{
    // RVA 0x8B1AE0
    unsigned const now = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    float const time = static_cast<float>(static_cast<double>(now - m_startTime) * 0.001);
    m_startTime = now;

    for (auto unitIt = m_model->m_dynamicUnits.begin(); unitIt != m_model->m_dynamicUnits.end(); ++unitIt)
    {
        PostEffectModel::EffectUnit& unit = m_model->m_effectUnits[*unitIt];
        float const scale = unit.m_params[0];
        float linear = unit.m_params[1];
        float const quadratic = unit.m_params[2];
        float const offset = unit.m_params[3];
        float* var = unit.m_var;

        if (unit.m_condItor != m_model->m_conditionList.end())
        {
            // A unit that has an exit condition always moves towards it, so the
            // sign of the linear term follows the direction of travel and the
            // quadratic term is ignored.
            linear = std::fabs(linear);
            if (!unit.m_condItor->m_evalIncrease)
            {
                linear = -linear;
            }
        }
        else if (quadratic != 0.0f)
        {
            var[VAR_VALUE] = var[VAR_VALUE] * scale + std::pow(time, 2) * quadratic + linear * time + offset;
            continue;
        }

        var[VAR_VALUE] = var[VAR_VALUE] * scale + linear * time + offset;
    }
}

bool PostEffect::CheckExitAnd()
{
    // RVA 0x8B1850 - every condition has to be met, and each one that is gets
    // its variable clamped exactly onto the limit.
    bool res = true;
    for (auto it = m_model->m_conditionList.begin(); it != m_model->m_conditionList.end(); ++it)
    {
        if (!IsConditionMet(it->m_var, it->m_limit, it->m_evalIncrease))
        {
            res = false;
            continue;
        }
        it->m_var[VAR_VALUE] = LimitOf(it->m_var, it->m_limit);
    }
    return res;
}

bool PostEffect::CheckExitOr()
{
    // RVA 0x8B18E0 - the first condition that is met ends the effect; note that
    // unlike the "and" variant this one does not clamp the variable.
    for (auto it = m_model->m_conditionList.begin(); it != m_model->m_conditionList.end(); ++it)
    {
        if (IsConditionMet(it->m_var, it->m_limit, it->m_evalIncrease))
        {
            return true;
        }
    }
    return false;
}

bool PostEffect::CheckExit()
{
    // RVA 0x8B1C00
    if (m_model->m_conditionUnite == CU_AND)
    {
        return CheckExitAnd();
    }
    if (m_model->m_conditionUnite == CU_OR)
    {
        return CheckExitOr();
    }
    return false;
}

void PostEffect::DoFrame()
{
    // RVA 0x8B1C20
    ApplyDynamic();
    if (CheckExit())
    {
        UnRegistedMe();
    }
}

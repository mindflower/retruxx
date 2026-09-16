#include "graphinfo.h"

#include <cmath>

GraphInfo::GraphInfo()
{
    // RVA 0x59BAC0
    fillOffset = 1;
    averAll = 1.0f;
    devider = 1.0f;
    averVal = 1.0f;
}

void GraphInfo::AddValue(float val)
{
    // RVA 0x59BAF0
    valArray[fillOffset] = val;
    averAll = averAll * 0.995f + val * 0.005f;
    // The scale only follows the running average once it drifts by more than 10%.
    if (std::fabs(averAll - devider) > averAll * 0.1f)
    {
        devider = averAll;
    }
    fillOffset = (fillOffset + 1) % 1000;
}

void GraphInfo::AddFiltred(float val, int averCount)
{
    // RVA 0x59BB70
    averVal = static_cast<float>(averCount - 1) * averVal / static_cast<float>(averCount) + val / static_cast<float>(averCount);
    AddValue(averVal);
}

float GraphInfo::GetValue(int pos)
{
    // RVA 0x59BBC0
    return valArray[(fillOffset + pos - 1) % 1000] / devider;
}

float GraphInfo::GetAverValue()
{
    // RVA 0x59BBF0
    return devider;
}

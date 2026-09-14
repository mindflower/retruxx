#include <psprops.h>
#include <stdexcept>

#include "retruxx/common.h"

WorkTime::WorkTime()
{
    m_start = 0.0;
    m_length = 0.0;
    m_repeat = 0.0;
}

void Force::SetAxis(float forceMin, float forceMax, ForceType ft, float period)
{
    m_max = forceMax;
    m_min = forceMin;
    m_freq = 3.1415927 / period;
    m_type = ft;
}

Force::Force()
{
    m_min = 0.0;
    m_max = 0.0;
    m_freq = 0.0;
    m_type = PS_FORCE_RANDOM;
}

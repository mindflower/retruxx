#include <psprops.h>
#include <stdexcept>

WorkTime::WorkTime()
{
    this->m_start = 0.0;
    this->m_length = 0.0;
    this->m_repeat = 0.0;
}

void Force::SetAxis(float, float, ForceType, float)
{
    throw std::logic_error("Not implemented");
}

Force::Force()
{
    this->m_min = 0.0;
    this->m_max = 0.0;
    m_freq = 0.0;
    m_type = PS_FORCE_RANDOM;
}

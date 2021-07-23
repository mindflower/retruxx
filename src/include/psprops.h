#pragma once

class WorkTime
{
public:
    WorkTime();

private:
    float m_start;
    float m_length;
    float m_repeat;
};

enum ForceType
{
    PS_FORCE_RANDOM = 0x0,
    PS_FORCE_SINE = 0x1,
};

class Force
{
public:
    void SetAxis(float,float,ForceType,float);
    Force();

private:
    float m_min;
    float m_max;
    float m_freq;
    ForceType m_type;
};

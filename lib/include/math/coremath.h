#pragma once
#include "quaternion.h"

struct CVector2;
class CBrezLine
{
public:
    int start(int srcx, int srcy, int dstx, int dsty);
    int step(int& curx, int& cury);

protected:
    int m_x0;
    int m_x1;
    int m_y0;
    int m_y1;
    int m_numsteps;
    int m_d;
    int m_dinc0;
    int m_dinc1;
    int m_xinc0;
    int m_xinc1;
    int m_yinc0;
    int m_yinc1;
    int m_x;
    int m_y;
    int m_i;
};

Quaternion CubicInterpolation(float, Quaternion const&, Quaternion const&, Quaternion const&, Quaternion const&);
Quaternion Exp(Quaternion const&);
Quaternion Ln(Quaternion const&);
Quaternion SLerp(Quaternion const&, Quaternion const&, float);
Quaternion SLerpAcc(Quaternion const&, Quaternion const&, float);
Quaternion SQuad(float, Quaternion const&, Quaternion const&, Quaternion const&, Quaternion const&);
Quaternion getTangent(Quaternion const& prevQuat, Quaternion const& currentQuat, Quaternion const& nextQuat);

float CalculateAngle(CVector2 const&, CVector2 const&);

template<class T>
T lerp(T const& a, T const& b, float s)
{
    return a + ((b - a) * s);
}

template<class T>
T CatmullRomSubdivide(float t, T const& p1, T const& p2, T const& p3, T const& p4)
{
    return (p3 * ((4.0 - t * 3.0) * t + 1.0) * t + p2 * (((t * t) * t) * 3.0 - (t * t) * 5.0 + 2.0) +
            p1 * ((2.0 - t) * t * t - t) + p4 * (((t * t) * t) - (t * t))) *
        0.5;
}
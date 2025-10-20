#pragma once
#include "quaternion.h"

//TODO: add static functions

class CBrezLine
{
public:
    int start(int srcx, int srcy, int dstx, int dsty);
    int step(int& curx, int& cury);

protected:
    /* 0x0000 */ int m_x0;
    /* 0x0004 */ int m_x1;
    /* 0x0008 */ int m_y0;
    /* 0x000c */ int m_y1;
    /* 0x0010 */ int m_numsteps;
    /* 0x0014 */ int m_d;
    /* 0x0018 */ int m_dinc0;
    /* 0x001c */ int m_dinc1;
    /* 0x0020 */ int m_xinc0;
    /* 0x0024 */ int m_xinc1;
    /* 0x0028 */ int m_yinc0;
    /* 0x002c */ int m_yinc1;
    /* 0x0030 */ int m_x;
    /* 0x0034 */ int m_y;
    /* 0x0038 */ int m_i;
}; /* size: 0x003c */

Quaternion CubicInterpolation(float, Quaternion const&, Quaternion const&, Quaternion const&, Quaternion const&);
Quaternion Exp(Quaternion const&);
Quaternion Ln(Quaternion const&);
Quaternion SLerp(Quaternion const&, Quaternion const&, float);
Quaternion SLerpAcc(Quaternion const&, Quaternion const&, float);
Quaternion SQuad(float, Quaternion const&, Quaternion const&, Quaternion const&, Quaternion const&);
Quaternion getTangent(Quaternion const& prevQuat, Quaternion const& currentQuat, Quaternion const& nextQuat);

template<class T>
T lerp(const T& a, const T& b, float s)
{
    return a + ((b - a) * s) ;
}

template<class T>
T CatmullRomSubdivide(float t, T const& p1, T const& p2, T const& p3, T const& p4)
{
    return (p3 * ((4.0 - t * 3.0) * t + 1.0) * t
            + p2 * (((t * t) * t) * 3.0 - (t * t) * 5.0 + 2.0)
            + p1 * ((2.0 - t) * t * t - t)
            + p4 * (((t * t) * t) - (t * t)))
        * 0.5;
}
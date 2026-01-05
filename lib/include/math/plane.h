#pragma once
#include "vector.h"

class CPlane
{
public:
    CPlane(float x, float y, float z, float d);
    CPlane() = default;

    CVector m_normal;
    float m_dist = 0.0f;
    unsigned int m_type = 0u;
    unsigned int m_signbits = 0u;

    float dist(CVector const& to);
    CPlane Reverse();
    void translate(CVector const&);
    static void translate(CPlane*, CPlane*, CVector const&);
    void fromPointNormal(CVector const& org, CVector const& n);
    static void buildplane(CPlane* dst, CVector* wnd);
    void normalize();
    void calcStuff();
    CVector origin();
    float intersectRay(CVector const& v0, CVector const& dir);
};

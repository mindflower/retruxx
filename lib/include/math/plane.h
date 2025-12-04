#pragma once
#include "vector.h"

class CPlane
{
public:
    CPlane(const CPlane& __that);
    CPlane(float x, float y, float z, float d);
    CPlane();
    /* 0x0000 */ CVector m_normal;
    /* 0x000c */ float m_dist;
    /* 0x0010 */ unsigned int m_type;
    /* 0x0014 */ unsigned int m_signbits;
    float dist(const CVector& to);
    CPlane Reverse();
    void translate(const CVector&);
    static void __fastcall translate(CPlane*, CPlane*, const CVector&);
    void fromPointNormal(const CVector& org, const CVector& n);
    static void __fastcall buildplane(CPlane* dst, CVector* wnd);
    void normalize();
    void calcStuff();
    CVector origin();
    float intersectRay(const CVector& v0, const CVector& dir);
}; /* size: 0x0018 */

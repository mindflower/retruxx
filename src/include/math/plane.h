#pragma once
#include "vector.h"

class CPlane
{
public:
    static void buildplane(CPlane*, CVector*);

public:
    CPlane();
    CPlane(CPlane const&);
    void fromPointNormal(CVector const&, CVector const&);
    float intersectRay(CVector const&, CVector const&);
    CPlane Reverse();
    float dist(CVector const&);
    CVector origin();

private:
    CVector m_normal;
    float m_dist;
    unsigned int m_type;
    unsigned int m_signbits;
};

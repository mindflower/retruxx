#pragma once
#include "vector.h"
#include "math/matrix.h"

struct Aabb;

class Obb
{
public:
    CVector toWorld(const CVector& v) const;
    CVector toLocalRotate(const CVector& v) const;
    CVector m_origin;
    /* 0x000c */ CVector m_basis[3];
    CVector m_min;
    CVector m_max;
    void Create(const CVector& min, const CVector& max, const CMatrix& mat, bool noScale);
    void Create(const Aabb& aabb, const CMatrix& mat, bool noScale);
    void Draw(unsigned int clr);
    float IntersectRay(const CVector& v0, const CVector& dir) const;
    int IsPtInside(const CVector& v) const;
    int IsPtInside2(const CVector& v) const;
    Aabb GetBounds() const;
}; /* size: 0x0048 */

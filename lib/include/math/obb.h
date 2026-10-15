#pragma once
#include "vector.h"
#include "math/matrix.h"

class Aabb;

class Obb
{
public:
    CVector toWorld(CVector const& v) const;
    CVector toLocalRotate(CVector const& v) const;
    CVector m_origin;
    CVector m_basis[3];
    CVector m_min;
    CVector m_max;
    void Create(CVector const& min, CVector const& max, CMatrix const& mat, bool noScale);
    void Create(Aabb const& aabb, CMatrix const& mat, bool noScale);
    void Draw(unsigned int clr);
    float IntersectRay(CVector const& v0, CVector const& dir) const;
    int IsPtInside(CVector const& v) const;
    int IsPtInside2(CVector const& v) const;
    Aabb GetBounds() const;
};

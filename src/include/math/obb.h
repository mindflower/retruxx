#pragma once
#include "vector.h"

struct Aabb;

class Obb
{
public:
    void Create(Aabb const&, CMatrix const&, bool);
    void Create(CVector const&, CVector const&, CMatrix const&, bool);
    void Draw(unsigned int);
    int IsPtInside2(CVector const&) const;
    Aabb GetBounds() const;
    CVector toLocalRotate(CVector const&) const;
    float IntersectRay(CVector const&, CVector const&) const;
    CVector toWorld(CVector const&) const;
    int IsPtInside(CVector const&) const;

private:
    CVector m_origin;
    CVector m_basis[3];
    CVector m_min;
    CVector m_max;
};

#pragma once

struct CVector;

class Aabb
{
public:
    /* 0x0000 */ float m_box[6];

    void Create(const CVector& min, const CVector& max);
    void Offset(const CVector& ofs);
    void Scale(const CVector& sc);
    float GetSz() const;
    float GetSy() const;
    float GetSx() const;
    void Inflate(float sz);
    void StartEmbracing();
    void EmbracePoint(const CVector& v);
    void EmbraceBox(const Aabb& box);
    CVector Min() const;
    CVector Max() const;
    float MaximumComponent() const;
    bool IsPtInside(const CVector&) const;
    bool IsPtInside2(const CVector& pt) const;
    void Draw(unsigned int clr);
}; /* size: 0x0018 */

static_assert(sizeof(Aabb) == 0x0018);

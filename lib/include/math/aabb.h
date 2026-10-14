#pragma once

struct CVector;

class Aabb
{
public:
    float m_box[6];

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
};


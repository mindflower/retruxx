#pragma once

struct CVector;

class Aabb
{
public:
    float m_box[6];

    void Create(CVector const& min, CVector const& max);
    void Offset(CVector const& ofs);
    void Scale(CVector const& sc);
    float GetSz() const;
    float GetSy() const;
    float GetSx() const;
    void Inflate(float sz);
    void StartEmbracing();
    void EmbracePoint(CVector const& v);
    void EmbraceBox(Aabb const& box);
    CVector Min() const;
    CVector Max() const;
    float MaximumComponent() const;
    bool IsPtInside(CVector const&) const;
    bool IsPtInside2(CVector const& pt) const;
    void Draw(unsigned int clr);
};

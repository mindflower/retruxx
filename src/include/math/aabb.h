#pragma once

class CVector;

struct Aabb
{
    float m_box[6];

public:
    CVector Max() const;
    void EmbracePoint(CVector const&);
    float GetSx() const;
    float GetSy() const;
    float GetSz() const;
    void EmbraceBox(Aabb const&);
    void Draw(unsigned int);
    bool IsPtInside2(CVector const&) const;
    void StartEmbracing();
    //ai::Geom::CellCellAabb();
    CVector Min() const;
    void Offset(CVector const&);
    void Inflate(float);
    void Create(CVector const&, CVector const&);
    void Scale(CVector const&);
};

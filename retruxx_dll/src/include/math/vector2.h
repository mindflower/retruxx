#pragma once

class CVector2
{
public:
    CVector2(float, float);
    CVector2();
    CVector2(CVector2 const&);
    operator float* ();
    float length() const;
    CVector2& operator-=(CVector2 const&);
    CVector2& operator*=(float);
    float lengthSq() const;
    void zero();
    float randomValue() const;
    CVector2 normalize() const;

public:
    float x = 0.0;
    float y = 0.0;
};

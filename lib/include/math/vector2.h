#pragma once

struct CVector2
{
    float x = 0.0;
    float y = 0.0;
    CVector2(CVector2 const& vv);
    CVector2(float xx, float yy);
    CVector2();
    void zero();
    void one();
    CVector2 normalize() const;
    operator float const*() const;
    operator float*();
    float length() const;
    float lengthSq() const;
    CVector2& operator-=(CVector2 const& a);
    CVector2& operator+=(CVector2 const&);
    CVector2& operator*=(float v);
    CVector2& operator/=(float);
    float randomValue() const;
};

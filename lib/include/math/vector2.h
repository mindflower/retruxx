#pragma once

struct CVector2
{
    /* 0x0000 */ float x = 0.0;
    /* 0x0004 */ float y = 0.0;
    CVector2(const CVector2& vv);
    CVector2(float xx, float yy);
    CVector2();
    void zero();
    void one();
    CVector2 normalize() const;
    operator const float* () const;
    operator float* ();
    float length() const;
    float lengthSq() const;
    CVector2& operator-=(const CVector2& a);
    CVector2& operator+=(const CVector2&);
    CVector2& operator*=(float v);
    CVector2& operator/=(float);
    float randomValue() const;
}; /* size: 0x0008 */

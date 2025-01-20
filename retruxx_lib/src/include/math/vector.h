#pragma once

struct CVector
{
    /* 0x0000 */ float x;
    /* 0x0004 */ float y;
    /* 0x0008 */ float z;

    CVector(float* xx);
    CVector(float);
    CVector(float xx, float yy, float zz);
    CVector() = default;
    CVector operator-() const;
    CVector& operator-=(const CVector& a);
    CVector& operator+=(const CVector& a);
    CVector& operator*=(float v);
    CVector& operator/=(float v);
    CVector getNormalized() const;
    void normalizeInplace();
    CVector clampLength(float clampTo) const;
    void zero();
    void one();
    float length() const;
    float lengthSq() const;
    const float& operator[](int index) const;
    float& operator[](int index);
}; /* size: 0x000c */

static_assert(sizeof(CVector) == 0x000c);

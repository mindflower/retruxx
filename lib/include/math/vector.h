#pragma once

struct CVector
{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    CVector() = default;
    CVector(float* xx);
    CVector(float);
    CVector(float xx, float yy, float zz);

    CVector operator-() const;
    CVector operator-(CVector const& rhs) const;
    CVector operator*(float v) const;
    CVector operator+(CVector const& rhs) const;

    CVector& operator-=(CVector const& a);
    CVector& operator+=(CVector const& a);
    CVector& operator*=(float v);
    CVector& operator/=(float v);

    CVector getNormalized() const;
    void normalizeInplace();

    CVector clampLength(float clampTo) const;

    void zero();
    void one();

    float length() const;
    float lengthSq() const;

    float const& operator[](int index) const;
    float& operator[](int index);
};

static_assert(sizeof(CVector) == 0x000c);

inline CVector const ZeroVector(0.0, 0.0, 0.0);

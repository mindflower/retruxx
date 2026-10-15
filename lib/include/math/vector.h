#pragma once

#include <cmath>

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

inline CVector operator+(CVector const& l, CVector const& r)
{
    return {l.x + r.x, l.y + r.y, l.z + r.z};
};

inline CVector operator+(CVector const& l, float r)
{
    return {l.x + r, l.y + r, l.z + r};
};

inline CVector operator-(CVector const& l, CVector const& r)
{
    return {l.x - r.x, l.y - r.y, l.z - r.z};
};

inline CVector operator-(CVector const& l, float r)
{
    return {l.x - r, l.y - r, l.z - r};
};

inline CVector operator*(CVector const& l, CVector const& r)
{
    return {l.x * r.x, l.y * r.y, l.z * r.z};
};

inline CVector operator*(CVector const& l, float r)
{
    return {l.x * r, l.y * r, l.z * r};
};

inline CVector operator/(CVector const& l, CVector const& r)
{
    return {l.x / r.x, l.y / r.y, l.z / r.z};
};

inline CVector operator/(CVector const& l, float r)
{
    return {l.x / r, l.y / r, l.z / r};
};

inline float dot(CVector const& l, CVector const& r)
{
    return l.x * r.x + l.y * r.y + l.z * r.z;
};

inline float length(CVector const& v)
{
    return std::sqrt(dot(v, v));
};

inline CVector abs(CVector const& v)
{
    return {std::abs(v.x), std::abs(v.y), std::abs(v.z)};
};

inline float distance(CVector const& s, CVector const& e)
{
    return length(s - e);
};

inline bool iszero(CVector const& v)
{
    return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
};

inline bool isfinite(CVector const& v)
{
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
};

static_assert(sizeof(CVector) == 0x000c);

inline CVector const ZeroVector(0.0, 0.0, 0.0);

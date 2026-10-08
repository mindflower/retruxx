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

inline CVector operator + (const CVector& l, const CVector& r) {
    return { l.x + r.x, l.y + r.y, l.z + r.z };
};

inline CVector operator + (const CVector& l, float r) {
    return { l.x + r, l.y + r, l.z + r };
};

inline CVector operator - (const CVector& l, const CVector& r) {
    return { l.x - r.x, l.y - r.y, l.z - r.z };
};

inline CVector operator - (const CVector& l, float r) {
    return { l.x - r, l.y - r, l.z - r };
};

inline CVector operator * (const CVector& l, const CVector& r) {
    return { l.x * r.x, l.y * r.y, l.z * r.z };
};

inline CVector operator * (const CVector& l, float r) {
    return { l.x * r, l.y * r, l.z * r };
};

inline CVector operator / (const CVector& l, const CVector& r) {
    return { l.x / r.x, l.y / r.y, l.z / r.z };
};

inline CVector operator / (const CVector& l, float r) {
    return { l.x / r, l.y / r, l.z / r };
};

inline float dot(const CVector& l, const CVector& r) {
    return l.x * r.x + l.y * r.y + l.z * r.z;
};

inline float length(const CVector& v) {
    return std::sqrt(dot(v, v));
};

inline CVector abs(const CVector& v) {
    return { std::abs(v.x), std::abs(v.y), std::abs(v.z) };
};

inline float distance(const CVector& s, const CVector& e) {
    return length(s - e);
};

inline bool iszero(const CVector& v) {
    return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
};

inline bool isfinite(const CVector& v) {
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
};


static_assert(sizeof(CVector) == 0x000c);

inline CVector const ZeroVector(0.0, 0.0, 0.0);

#pragma once

class CVector
{
public:
    CVector operator-() const;
    CVector& operator-=(CVector const&);
    CVector getNormalized() const;
    CVector& operator/=(float);
    CVector& operator+=(CVector const&);
    float& operator[](int);
    float const& operator[](int) const;
    CVector& operator*=(float);
    float length() const;
    void normalizeInplace();
    void zero();
    CVector() = default;
    CVector(float*);
    CVector(float, float, float);
    void one();
    CVector clampLength(float) const;
    float lengthSq() const;

public:
    float x = 0;
    float y = 0;
    float z = 0;
};

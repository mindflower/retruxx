#include "math/vector.h"
#include <stdexcept>

namespace
{
    float constexpr THRESHOLD_EPS = 0.0000099999997;
}

CVector::CVector(float* xx)
{
    x = xx[0];
    y = xx[1];
    z = xx[2];
}

CVector::CVector(float const xx)
{
    x = xx;
}

CVector::CVector(float const xx, float const yy, float const zz)
{
    x = xx;
    y = yy;
    z = zz;
}

CVector CVector::operator-() const
{
    return {-x, -y, -z};
}

CVector CVector::operator-(CVector const& rhs) const
{
    CVector res = *this;
    res -= rhs;
    return res;
}

CVector CVector::operator*(float const v) const
{
    CVector res = *this;
    res *= v;
    return res;
}

CVector CVector::operator+(CVector const& rhs) const
{
    CVector res = *this;
    res += rhs;
    return res;
}

CVector& CVector::operator*=(float const v)
{
    x *= v;
    y *= v;
    z *= v;
    return *this;
}

CVector& CVector::operator+=(CVector const& a)
{
    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
}

CVector& CVector::operator-=(CVector const& a)
{
    x -= a.x;
    y -= a.y;
    z -= a.z;
    return *this;
}

CVector& CVector::operator/=(float const v)
{
    x /= v;
    y /= v;
    z /= v;
    return *this;
}

float& CVector::operator[](int const index)
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        throw std::runtime_error("bad CVector access");
    }
}

float const& CVector::operator[](int const index) const
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        throw std::runtime_error("bad CVector access");
    }
}

CVector CVector::clampLength(float const clampTo) const
{
    CVector res = *this;
    float const len = length();

    if (len <= THRESHOLD_EPS)
    {
        res.zero();
    }
    else
    {
        //TODO: check if right
        res.x *= clampTo / sqrt(len);
        res.y *= clampTo / sqrt(len);
        res.z *= clampTo / sqrt(len);
    }
    return res;
}

CVector CVector::getNormalized() const
{
    CVector vec = *this;
    vec.normalizeInplace();
    return vec;
}

float CVector::length() const
{
    return sqrt(lengthSq());
}

float CVector::lengthSq() const
{
    return x * x + y * y + z * z;
}

void CVector::normalizeInplace()
{
    float const scale = sqrt(x * x + y * y + z * z + FLT_EPSILON);
    x = 1.0 / scale * x;
    y = 1.0 / scale * y;
    z = 1.0 / scale * z;
}

void CVector::one()
{
    x = 1.0;
    y = 1.0;
    z = 1.0;
}

void CVector::zero()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

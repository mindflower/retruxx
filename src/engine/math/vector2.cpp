#include <cmath>
#include <math/vector2.h>

namespace
{
    auto const THRESHOLD_EPS = 0.0000099999997;
}

CVector2::CVector2(float xx, float yy) :
    x(xx),
    y(yy)
{
}

CVector2& CVector2::operator*=(float v)
{
    x *= v;
    y *= v;
    return *this;
}

CVector2& CVector2::operator-=(CVector2 const& a)
{
    x -= a.x;
    y -= a.y;
    return *this;
}

CVector2::operator float* ()
{
    return reinterpret_cast<float*>(this);
}

void CVector2::zero()
{
    x = 0.0;
    y = 0.0;
}

float CVector2::length() const
{
    return sqrt(lengthSq());
}

float CVector2::lengthSq() const
{
    return x * x + y * y;
}

CVector2 CVector2::normalize() const
{
    CVector2 result(*this);
    if (result.lengthSq() <= THRESHOLD_EPS)
    {
        result.zero();
    }
    else
    {
        auto const len = result.length();
        result.x /= len;
        result.y /= len;
    }
    return result;
}

float CVector2::randomValue() const
{
    auto const x1 = x <= y ? x : y;
    auto const x2 = y <= x ? x : y;
    auto const x3 = x <= y ? x : y;
    return std::rand() * (x2 - x3) * 0.000030518509 + x1;
}

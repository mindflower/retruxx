#include <math/quaternion.h>
#include <math/matrix.h>
#include <math/vector.h>
#include <stdexcept>

#include "retruxx/common.h"

const int nxt[3] = { 1,2,0 };

void Quaternion::FromMatrix(CMatrix const& m)
{
    auto ma = (float)(m._22 + m._11) + m._33;
    if (ma <= 0.0)
    {
        auto v4 = m._22 > m._11;
        if (m._33 > *(&m._11 + 5 * v4))
            v4 = 2;
        auto v5 = nxt[v4];
        auto v6 = nxt[v5];
        auto v7 = sqrt(*(&m._11 + 5 * v4) - (*(&m._11 + 5 * v6) + *(&m._11 + 5 * v5)) + 1.0);
        auto mc = v7;
        *(&this->x + v4) = v7 * 0.5;
        this->w = (float)(*(&m._11 + 4 * v5 + v6) - *(&m._11 + 4 * v6 + v5)) * (float)(0.5 / mc);
        *(&this->x + v5) = (float)(*(&m._11 + 4 * v5 + v4) + *(&m._11 + 4 * v4 + v5)) * (float)(0.5 / mc);
        *(&this->x + v6) = (float)(*(&m._11 + 4 * v6 + v4) + *(&m._11 + 4 * v4 + v6)) * (float)(0.5 / mc);
    }
    else
    {
        auto v3 = sqrt(ma + 1.0);
        auto mb = v3;
        this->w = v3 * 0.5;
        this->x = (float)(m._23 - m._32) * (float)(0.5 / mb);
        this->y = (float)(m._31 - m._13) * (float)(0.5 / mb);
        this->z = (float)(m._12 - m._21) * (float)(0.5 / mb);
    }
}

void Quaternion::Normalize()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Quaternion::ToAxisAngle(CVector&, float&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Quaternion::fromYPR(float, float, float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

Quaternion Quaternion::operator*(float v) const
{
    auto res_12 = this->w;
    auto res_4 = this->y * v;

    Quaternion result;
    result.x = this->x * v;
    result.y = res_4;
    result.z = z * v;
    result.w = res_12 * v;
    return result;
}

Quaternion Quaternion::operator+(Quaternion const& q) const
{
    auto res_4 = q.y + y;
    auto res_8 = q.z + z;
    auto v4 = q.w + w;

    Quaternion result;
    result.x = q.x + x;
    result.y = res_4;
    result.z = res_8;
    result.w = v4;
    return result;
}

Quaternion::Quaternion(CMatrix const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

Quaternion::Quaternion(float qx, float qy, float qz, float qw) :
    x(qx),
    y(qy),
    z(qz),
    w(qw)
{
}

CMatrix Quaternion::ToMatrix() const
{
    float v2; // xmm5_4
    float v3; // xmm3_4
    float v4; // xmm4_4
    float v5; // xmm2_4
    float v6; // xmm1_4
    float wy; // [esp+4h] [ebp-50h]
    float xy; // [esp+8h] [ebp-4Ch]
    float xx; // [esp+Ch] [ebp-48h]
    float yz; // [esp+10h] [ebp-44h]
    CMatrix m; // [esp+14h] [ebp-40h] BYREF

    v2 = this->z * this->w;
    v3 = this->z * this->x;
    xx = this->x * this->x;
    v4 = this->w * this->x;
    xy = this->y * this->x;
    yz = this->z * this->y;
    wy = this->y * this->w;
    v5 = this->z * this->z;
    v6 = this->y * this->y;
    m._11 = 1.0 - (float)((float)(v5 + v6) * 2.0);
    m._21 = (float)(xy - v2) * 2.0;
    m._31 = (float)(wy + v3) * 2.0;
    m._12 = (float)(v2 + xy) * 2.0;
    m._22 = 1.0 - (float)((float)(v5 + xx) * 2.0);
    m._33 = 1.0 - (float)((float)(v6 + xx) * 2.0);
    m._32 = (float)(yz - v4) * 2.0;
    m.m[0][2] = ((float)(v3 - wy) * 2.0);
    m.m[0][3] = 0.0;
    m.m[1][2] = ((float)(v4 + yz) * 2.0);
    m.m[1][3] = 0.0;
    memset(&m.m[2][3], 0, 16);
    m._44 = 1.0;
    return m;
}

Quaternion Quaternion::getConjugated() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Quaternion::Zero()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->w = 0.0;
}

Quaternion Quaternion::getInversed() const
{
    // TODO: generated code
    Quaternion result;

    // Calculate the squared length (magnitude) of the quaternion
    float squaredLength = (w * w) + (x * x) + (y * y) + (z * z);

    // Check for zero length quaternion to avoid division by zero
    if (squaredLength <= 0.0f)
    {
        // Return identity quaternion for zero-length input
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        result.w = 1.0f;
    }
    else
    {
        // Calculate inverse length (1 / magnitude)
        float invLength = 1.0f / sqrt(squaredLength);

        // For a unit quaternion, inverse is conjugate (negate x,y,z) divided by squared length
        // Since we're normalizing, we multiply conjugate by invLength
        result.x = -x * invLength;
        result.y = -y * invLength;
        result.z = -z * invLength;
        result.w = w * invLength;
    }

    return result;

}

float& Quaternion::operator[](unsigned int i)
{
    return *(float*)(this + i);
}

float Quaternion::operator[](unsigned int i) const
{
    return *(float*)(this + i);
}

void Quaternion::Lerp(Quaternion const& q1, Quaternion const& q2, float k2)
{
    auto v6 = 0;
    auto q1a = (float)((float)((float)(q1.z * q2.z) + (float)(q1.y * q2.y)) + (float)(q2.w * q1.w))
        + (float)(q1.x * q2.x);
    auto v5 = q1a;
    if (q1a < 0.0)
    {
        v5 = 0.0 - q1a;
        q1a = 0.0 - q1a;
        v6 = 1;
    }
    float v7, v8;
    if ((float)(1.0 - v5) >= 0.001)
    {
        auto v9 = acos(q1a);
        auto v14 = v9 * k2;
        auto v10 = v9;
        auto v11 = 1.0 / sqrt(1.0 - q1a * q1a);
        auto q1b = sin(v10 - v14) * v11;
        v8 = q1b;
        auto v15 = sin(v14) * v11;
        v7 = v15;
    }
    else
    {
        v7 = k2;
        v8 = 1.0 - k2;
    }
    if (v6)
        v7 = 0.0 - v7;
    this->x = (float)(q1.x * v8) + (float)(q2.x * v7);
    this->y = (float)(q1.y * v8) + (float)(v7 * q2.y);
    this->z = (float)(q1.z * v8) + (float)(v7 * q2.z);
    this->w = (float)(q2.w * v7) + (float)(v8 * q1.w);
}

void Quaternion::RotZ(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Quaternion::FromAxisAngle(const CVector& axis, float radians)
{
    auto v3 = radians * 0.5;
    auto v4 = sin(v3);
    x = v4 * axis.x;
    y = v4 * axis.y;
    z = v4 * axis.z;
    w = cos(v3);
}

void Quaternion::RotX(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Quaternion::RotY(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CVector Quaternion::vecRot(CVector const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Quaternion::Identity()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->w = 1.0;
}

float Quaternion::Norm() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

Quaternion& Quaternion::operator+=(Quaternion const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

Quaternion& Quaternion::operator*=(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Quaternion::operator*=(Quaternion const& b)
{
    auto v2 = (float)((float)((float)(b.w * this->y) + (float)(this->w * b.y)) + (float)(this->z * b.x))
        - (float)(b.z * this->x);
    auto v3 = (float)((float)((float)(b.w * this->z) + (float)(this->x * b.y)) + (float)(b.z * this->w))
        - (float)(b.x * this->y);
    auto v4 = (float)((float)((float)(b.w * this->w) - (float)(b.x * this->x)) - (float)(b.y * this->y))
        - (float)(this->z * b.z);
    this->x = (float)((float)((float)(this->w * b.x) + (float)(b.z * this->y)) + (float)(b.w * this->x))
        - (float)(this->z * b.y);
    this->y = v2;
    this->z = v3;
    this->w = v4;
}

#include <math/quaternion.h>
#include <math/matrix.h>
#include <math/vector.h>
#include <stdexcept>

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
    throw std::logic_error("Not implemented");
}

void Quaternion::ToAxisAngle(CVector&, float&)
{
    throw std::logic_error("Not implemented");
}

void Quaternion::fromYPR(float, float, float)
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float w = this->w;

    // Precompute common terms
    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    // Compute the rotation matrix from quaternion
    // First row
    CMatrix result;
    result._11 = 1.0f - 2.0f * (y2 + z2);
    result._12 = 2.0f * (xy + wz);
    result._13 = 2.0f * (xz - wy);
    result._14 = 0.0f;

    // Second row
    result._21 = 2.0f * (xy - wz);
    result._22 = 1.0f - 2.0f * (x2 + z2);
    result._23 = 2.0f * (yz + wx);
    result._24 = 0.0f;

    // Third row
    result._31 = 2.0f * (xz + wy);
    result._32 = 2.0f * (yz - wx);
    result._33 = 1.0f - 2.0f * (x2 + y2);
    result._34 = 0.0f;

    // Fourth row (homogeneous coordinates)
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;

    return result;
}

Quaternion Quaternion::getConjugated() const
{
    throw std::logic_error("Not implemented");
}

void Quaternion::Zero()
{
    throw std::logic_error("Not implemented");
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

void Quaternion::Lerp(Quaternion const&, Quaternion const&, float)
{
    throw std::logic_error("Not implemented");
}

void Quaternion::RotZ(float)
{
    throw std::logic_error("Not implemented");
}

void Quaternion::FromAxisAngle(CVector const&, float)
{
    throw std::logic_error("Not implemented");
}

void Quaternion::RotX(float)
{
    throw std::logic_error("Not implemented");
}

void Quaternion::RotY(float)
{
    throw std::logic_error("Not implemented");
}

CVector Quaternion::vecRot(CVector const&) const
{
    throw std::logic_error("Not implemented");
}

void Quaternion::Identity()
{
    throw std::logic_error("Not implemented");
}

float Quaternion::Norm() const
{
    throw std::logic_error("Not implemented");
}

Quaternion& Quaternion::operator+=(Quaternion const&)
{
    throw std::logic_error("Not implemented");
}

Quaternion& Quaternion::operator*=(float)
{
    throw std::logic_error("Not implemented");
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

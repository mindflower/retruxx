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

Quaternion Quaternion::operator*(float) const
{
    throw std::logic_error("Not implemented");
}

Quaternion Quaternion::operator+(Quaternion const&) const
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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
    return operator[](i);
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

void Quaternion::operator*=(Quaternion const&)
{
    throw std::logic_error("Not implemented");
}

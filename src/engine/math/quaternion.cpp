#include <math/quaternion.h>
#include <math/matrix.h>
#include <math/vector.h>
#include <stdexcept>

void Quaternion::FromMatrix(CMatrix const&)
{
    throw std::logic_error("Not implemented");
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

Quaternion::Quaternion(float, float, float, float)
{
    throw std::logic_error("Not implemented");
}

Quaternion::Quaternion()
{
    throw std::logic_error("Not implemented");
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
    throw std::logic_error("Not implemented");
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

float& Quaternion::operator[](unsigned)
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

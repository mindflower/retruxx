#include "math/aabb.h"

#include <stdexcept>

#include "math/vector.h"

void Aabb::Create(const CVector& min, const CVector& max)
{
    this->m_box[0] = min.x;
    this->m_box[1] = min.y;
    this->m_box[2] = min.z;
    this->m_box[3] = max.x;
    this->m_box[4] = max.y;
    this->m_box[5] = max.z;
}

void Aabb::Offset(const CVector& ofs)
{
    throw std::logic_error("Not implemented");
}

void Aabb::Scale(const CVector& sc)
{
    throw std::logic_error("Not implemented");
}

float Aabb::GetSz() const
{
    throw std::logic_error("Not implemented");
}

float Aabb::GetSy() const
{
    throw std::logic_error("Not implemented");
}

float Aabb::GetSx() const
{
    throw std::logic_error("Not implemented");
}

void Aabb::Inflate(float sz)
{
    throw std::logic_error("Not implemented");
}

void Aabb::StartEmbracing()
{
    throw std::logic_error("Not implemented");
}

void Aabb::EmbracePoint(const CVector& v)
{
    throw std::logic_error("Not implemented");
}

void Aabb::EmbraceBox(const Aabb& box)
{
    throw std::logic_error("Not implemented");
}

CVector Aabb::Min() const
{
    throw std::logic_error("Not implemented");
}

CVector Aabb::Max() const
{
    throw std::logic_error("Not implemented");
}

float Aabb::MaximumComponent() const
{
    throw std::logic_error("Not implemented");
}

bool Aabb::IsPtInside(const CVector&) const
{
    throw std::logic_error("Not implemented");
}

bool Aabb::IsPtInside2(const CVector& pt) const
{
    throw std::logic_error("Not implemented");
}

void Aabb::Draw(unsigned int clr)
{
    throw std::logic_error("Not implemented");
}

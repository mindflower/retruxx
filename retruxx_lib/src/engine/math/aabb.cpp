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
    auto v2 = &box;
    if (this->m_box[0] <= box.m_box[0])
        v2 = this;
    this->m_box[0] = v2->m_box[0];
    auto v3 = &box.m_box[3];
    if (box.m_box[3] <= this->m_box[3])
        v3 = &this->m_box[3];
    this->m_box[3] = *v3;
    auto v4 = &box.m_box[1];
    if (this->m_box[1] <= box.m_box[1])
        v4 = &this->m_box[1];
    this->m_box[1] = *v4;
    auto v5 = &box.m_box[4];
    if (box.m_box[4] <= this->m_box[4])
        v5 = &this->m_box[4];
    this->m_box[4] = *v5;
    auto v6 = &box.m_box[2];
    if (this->m_box[2] <= box.m_box[2])
        v6 = &this->m_box[2];
    this->m_box[2] = *v6;
    if (box.m_box[5] <= this->m_box[5])
        this->m_box[5] = this->m_box[5];
    else
        this->m_box[5] = box.m_box[5];
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

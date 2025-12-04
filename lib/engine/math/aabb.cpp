#include "math/aabb.h"

#include "m3dapp.h"

#include <stdexcept>

#include "math/vector.h"
#include "retruxx/common.h"

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
    RETRUXX_NOT_IMPLEMENTED;
}

void Aabb::Scale(const CVector& sc)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float Aabb::GetSz() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float Aabb::GetSy() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float Aabb::GetSx() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Aabb::Inflate(float sz)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Aabb::StartEmbracing()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Aabb::EmbracePoint(const CVector& v)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

CVector Aabb::Max() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

float Aabb::MaximumComponent() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool Aabb::IsPtInside(const CVector&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool Aabb::IsPtInside2(const CVector& pt) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void Aabb::Draw(unsigned int clr)
{
    CVector a;
    a.x = this->m_box[0];
    a.y = this->m_box[1];
    a.z = this->m_box[2];

    
    CVector b;
    b.x = this->m_box[0];
    b.y = this->m_box[1];
    b.z = this->m_box[5];
    
    CVector c;
    c.x = this->m_box[3];
    c.y = this->m_box[1];
    c.z = this->m_box[5];

    CVector d;
    d.x = this->m_box[3];
    d.y = this->m_box[1];
    d.z = this->m_box[2];

    CVector e;
    e.x = this->m_box[0];
    e.y = this->m_box[4];
    e.z = this->m_box[2];

    CVector f;
    f.x = this->m_box[0];
    f.y = this->m_box[4];
    f.z = this->m_box[5];

    CVector g;
    g.x = this->m_box[3];
    g.y = this->m_box[4];
    g.z = this->m_box[5];

    CVector h;
    h.x = this->m_box[3];
    h.y = this->m_box[4];
    h.z = this->m_box[2];

    M3D_RENDERER->SetTexture(0, {}, -1.0);
    M3D_RENDERER->SetTexture(1, {}, -1.0);
    M3D_APP->DrawLine(a, b, clr);
    M3D_APP->DrawLine(b, c, clr);
    M3D_APP->DrawLine(c, d, clr);
    M3D_APP->DrawLine(d, a, clr);
    M3D_APP->DrawLine(e, f, clr);
    M3D_APP->DrawLine(f, g, clr);
    M3D_APP->DrawLine(g, h, clr);
    M3D_APP->DrawLine(h, e, clr);
    M3D_APP->DrawLine(a, e, clr);
    M3D_APP->DrawLine(b, f, clr);
    M3D_APP->DrawLine(c, g, clr);
    M3D_APP->DrawLine(d, h, clr);
}

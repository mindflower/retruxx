#include "math/aabb.h"

#include "m3dapp.h"

#include <algorithm>
#include <cmath>
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
    // RVA 0x512420
    m_box[0] = ofs.x + m_box[0];
    m_box[1] = m_box[1] + ofs.y;
    m_box[2] = m_box[2] + ofs.z;
    m_box[3] = m_box[3] + ofs.x;
    m_box[4] = m_box[4] + ofs.y;
    m_box[5] = m_box[5] + ofs.z;
}

void Aabb::Scale(const CVector& sc)
{
    // RVA 0x63F220 - scales about the world origin, not the box's centre.
    m_box[0] = sc.x * m_box[0];
    m_box[3] = m_box[3] * sc.x;
    m_box[1] = m_box[1] * sc.y;
    m_box[4] = sc.y * m_box[4];
    m_box[2] = m_box[2] * sc.z;
    m_box[5] = sc.z * m_box[5];
}

float Aabb::GetSz() const
{
    // RVA 0x512E70
    return m_box[5] - m_box[2];
}

float Aabb::GetSy() const
{
    // RVA 0x512E50
    return m_box[4] - m_box[1];
}

float Aabb::GetSx() const
{
    // RVA 0x512E60
    return m_box[3] - m_box[0];
}

void Aabb::Inflate(float sz)
{
    // RVA 0x633FD0
    m_box[0] = m_box[0] - sz;
    m_box[1] = m_box[1] - sz;
    m_box[2] = m_box[2] - sz;
    m_box[3] = m_box[3] + sz;
    m_box[4] = m_box[4] + sz;
    m_box[5] = m_box[5] + sz;
}

void Aabb::StartEmbracing()
{
    // RVA 0x512DD0. Seeds an inverted box so the first EmbraceBox/EmbracePoint
    // snaps it onto real geometry. NOTE: the bounds are the shipped +-10000
    // literals, not FLT_MAX, so a box larger than that will not be embraced.
    m_box[0] = 10000.0f;
    m_box[1] = 10000.0f;
    m_box[2] = 10000.0f;
    m_box[3] = -10000.0f;
    m_box[4] = -10000.0f;
    m_box[5] = -10000.0f;
}

void Aabb::EmbracePoint(const CVector& v)
{
    // RVA 0x644F10
    if (m_box[0] > v.x)
        m_box[0] = v.x;
    if (v.x > m_box[3])
        m_box[3] = v.x;
    if (m_box[1] > v.y)
        m_box[1] = v.y;
    if (v.y > m_box[4])
        m_box[4] = v.y;
    if (m_box[2] > v.z)
        m_box[2] = v.z;
    if (v.z > m_box[5])
        m_box[5] = v.z;
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
    // RVA 0x512E80
    return CVector(m_box[0], m_box[1], m_box[2]);
}

CVector Aabb::Max() const
{
    // RVA 0x512EE0
    return CVector(m_box[3], m_box[4], m_box[5]);
}

float Aabb::MaximumComponent() const
{
    // NOTE: declared in the PDB but never instantiated in the shipped binary, so this body does not
    // come from it: the largest absolute coordinate of the box.
    float result = 0.0f;
    for (float component : m_box)
    {
        float const magnitude = std::fabs(component);
        if (magnitude > result)
        {
            result = magnitude;
        }
    }
    return result;
}

bool Aabb::IsPtInside(const CVector& pt) const
{
    // NOTE: declared in the PDB but never instantiated in the shipped binary, so this body does not
    // come from it: the full 3D version of IsPtInside2.
    return m_box[0] <= pt.x && pt.x <= m_box[3] && m_box[1] <= pt.y && pt.y <= m_box[4] && m_box[2] <= pt.z &&
        pt.z <= m_box[5];
}

bool Aabb::IsPtInside2(const CVector& pt) const
{
    // RVA 0x645660 - only x and y are tested.
    return m_box[0] <= pt.x && pt.x <= m_box[3] && m_box[1] <= pt.y && pt.y <= m_box[4];
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

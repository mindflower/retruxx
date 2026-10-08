#include "math/plane.h"
#include "retruxx/common.h"

#include <cmath>

CPlane::CPlane(float x, float y, float z, float d) : m_normal(x, y, z), m_dist(d)
{
    // RVA 0x5C0710 - m_type and m_signbits are not set by the original.
}

void CPlane::buildplane(CPlane* dst, CVector* wnd)
{
    // RVA 0x8A0B00 - the plane through three points, facing along (p1 - p0) x (p2 - p0). The small
    // epsilon keeps a degenerate triangle from dividing by zero.
    float const ax = wnd[1].x - wnd[0].x;
    float const ay = wnd[1].y - wnd[0].y;
    float const az = wnd[1].z - wnd[0].z;
    float const bx = wnd[2].x - wnd[0].x;
    float const by = wnd[2].y - wnd[0].y;
    float const bz = wnd[2].z - wnd[0].z;

    float const nx = bz * ay - by * az;
    float const ny = az * bx - bz * ax;
    float const nz = by * ax - ay * bx;
    float const invLen =
        static_cast<float>(1.0 / std::sqrt(double(nz) * nz + double(ny) * ny + double(nx) * nx + 0.00000011920929));

    dst->m_normal.x = invLen * nx;
    dst->m_normal.y = invLen * ny;
    dst->m_normal.z = invLen * nz;
    dst->m_dist = wnd[1].z * dst->m_normal.z + wnd[1].y * dst->m_normal.y + dst->m_normal.x * wnd[1].x;
}

void CPlane::fromPointNormal(CVector const& org, CVector const& n)
{
    // RVA 0x7A4550 - neither m_type nor m_signbits is touched; callers that
    // need them run calcStuff() afterwards.
    m_normal = n;
    m_dist = (org.z * n.z + org.y * n.y) + org.x * n.x;
}

float CPlane::intersectRay(CVector const& v0, CVector const& dir)
{
    // RVA 0x5C07F0 - the distance along dir to the plane: 0 when v0 is already on it, -1 when the
    // ray is parallel to it or points away from it.
    float const toPlane = m_normal.z * (m_normal.z * m_dist - v0.z) + m_normal.y * (m_normal.y * m_dist - v0.y) +
        m_normal.x * (m_normal.x * m_dist - v0.x);
    float const along = dir.z * m_normal.z + dir.y * m_normal.y + dir.x * m_normal.x;
    if (std::fabs(toPlane) < 0.0000099999997f)
    {
        return 0.0f;
    }
    if (std::fabs(along) < 0.0000099999997f)
    {
        return -1.0f;
    }
    float const t = toPlane / along;
    if (t < 0.0f)
    {
        return -1.0f;
    }
    return t;
}

CPlane CPlane::Reverse()
{
    // RVA 0x8A0FF0 - NOTE: the original fills m_type and m_signbits from uninitialized stack
    // memory; here they are left at their defaults.
    return CPlane(0.0f - m_normal.x, 0.0f - m_normal.y, 0.0f - m_normal.z, 0.0f - m_dist);
}

float CPlane::dist(CVector const& to)
{
    // RVA 0x7E09A0 - axis-aligned planes (m_type 0-2) read the matching coordinate directly.
    if (m_type < 3)
    {
        return (&to.x)[m_type] - m_dist;
    }
    return to.z * m_normal.z + to.y * m_normal.y + to.x * m_normal.x - m_dist;
}

CVector CPlane::origin()
{
    // RVA 0x5C07B0 - the point of the plane closest to the world origin.
    return CVector(m_normal.x * m_dist, m_normal.y * m_dist, m_normal.z * m_dist);
}

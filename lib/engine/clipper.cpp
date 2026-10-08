#include <clipper.h>
#include <stdexcept>

#include "m3dapp.h"
#include "math/matrix.h"
#include "math/plane.h"
#include <cmath>

// Keeps the part of a convex polygon behind the plane (math/obb.cpp).
int clipFrontSideInPlace(CVector* verts, int nverts, float* plane);
int CClipper::testVertexInside(CVector const& v)
{
    // RVA 0x8A0580 - 0 when the point is in front of an enabled plane.
    for (unsigned int i = 0; i < m_nfrustums; ++i)
    {
        float const* const plane = m_planes[i];
        if (((1 << i) & m_enabled) != 0 && plane[2] * v.z + plane[0] * v.x + v.y * plane[1] - plane[3] > 0.0f)
        {
            return 0;
        }
    }
    return 1;
}

void CClipper::CreateFromWinding(int dir, CVector* wnd, int n, CVector const& campos, CPlane* nearplane)
{
    // RVA 0x8A1640 - one plane through the camera and each edge of the winding, closed by the near plane (the
    // winding's own plane, flipped for dir == 1, unless one is given).
    // NOTE: n is clamped to 25 only when it exceeds 26, so a 26-vertex winding puts its near plane in m_planes[26],
    // over the start of m_indices.
    if (n > 26)
    {
        n = 25;
    }
    CPlane pl;
    if (!nearplane)
    {
        CPlane::buildplane(&pl, wnd);
        if (dir == 1)
        {
            pl.m_normal.x = 0.0f - pl.m_normal.x;
            pl.m_normal.y = 0.0f - pl.m_normal.y;
            pl.m_normal.z = 0.0f - pl.m_normal.z;
            pl.m_dist = 0.0f - pl.m_dist;
        }
        nearplane = &pl;
    }
    // NOTE: for dir == -1 the walk starts at n - 1 and runs while the index is <= 0, so no edge planes are built
    // unless the winding has a single vertex.
    int first;
    int last;
    if (dir == 1)
    {
        first = 0;
        last = n - 1;
    }
    else if (dir == -1)
    {
        first = n - 1;
        last = 0;
    }
    else
    {
        first = 0;
        last = 0;
    }
    for (int i = first; i <= last;)
    {
        CVector const& cur = wnd[i];
        float* const plane = m_planes[i];
        i += dir;
        CVector const& next = wnd[i % n];
        float const ax = cur.x - campos.x;
        float const ay = cur.y - campos.y;
        float const az = cur.z - campos.z;
        float const bx = next.x - campos.x;
        float const by = next.y - campos.y;
        float const bz = next.z - campos.z;
        float const nx = by * az - bz * ay;
        float const ny = bz * ax - az * bx;
        float const nz = ay * bx - by * ax;
        float const len2 = nz * nz + ny * ny + nx * nx;
        // Degenerate edges keep whatever the plane held before.
        if (!(1.0e-20f > len2))
        {
            float const inv = static_cast<float>(1.0 / sqrt(len2 + 1.1920929e-7f));
            plane[2] = inv * nz;
            plane[0] = inv * nx;
            plane[1] = inv * ny;
            plane[3] = campos.z * (inv * nz) + campos.x * (inv * nx) + inv * ny * campos.y;
        }
    }
    m_planes[n][0] = nearplane->m_normal.x;
    m_planes[n][1] = nearplane->m_normal.y;
    m_planes[n][2] = nearplane->m_normal.z;
    m_planes[n][3] = nearplane->m_dist;
    m_nfrustums = n + 1;
    createIndices();
    m_enabled = 0xFFFFFFFF;
}

int CClipper::clipPolyInPlace(CVector* verts, int nverts)
{
    // RVA 0x8A15C0 - clips against every enabled plane but the last (the far/near cap), giving up once the polygon
    // has degenerated or grown past 32 vertices.
    int result = nverts;
    if (m_nfrustums != 1)
    {
        unsigned int i = 0;
        do
        {
            if (result <= 2 || result > 32)
            {
                break;
            }
            if (((1 << i) & m_enabled) != 0)
            {
                result = clipFrontSideInPlace(verts, result, m_planes[i]);
            }
            ++i;
        } while (i < m_nfrustums - 1);
    }
    return result;
}

int CClipper::testBBox(tbEnum test, float* minmaxs, const CVector& ofs) const
{
    // RVA 0x8A0740 - 0 when the box (min/max corners, moved by ofs) lies in front of an enabled plane; otherwise,
    // for a full test, 2 when it is wholly behind every enabled plane (inside), else 1.
    // NOTE: the outside test allows the box's nearest corner to be up to 50 units in front of a plane, as shipped.
    auto const corner = [&](unsigned int const* idx) {
        return CVector(minmaxs[idx[0]] + ofs.x, minmaxs[idx[1]] + ofs.y, minmaxs[idx[2]] + ofs.z);
    };
    auto const distance = [](float const* plane, CVector const& p) {
        return p.z * plane[2] + p.x * plane[0] + p.y * plane[1] - plane[3];
    };
    for (unsigned int i = 0; i < m_nfrustums; ++i)
    {
        if ((1 << i) & m_enabled && distance(m_planes[i], corner(&m_indices[6 * i + 3])) > 50.0f)
        {
            return 0;
        }
    }
    if (test != tbFullTest)
    {
        return 1;
    }
    for (unsigned int i = 0; i < m_nfrustums; ++i)
    {
        if ((1 << i) & m_enabled && distance(m_planes[i], corner(&m_indices[6 * i])) > 0.0f)
        {
            return 1;
        }
    }
    return 2;
}

void CClipper::buildfrustum(float* fr, CVector const& n, CMatrix const& m, CVector const& org, float offset)
{
    // RVA 0x8A0C50 - rotates the normal into world space, normalizes it and puts the plane through org, pushed out
    // by offset.
    float const x = m._12 * n.y + m._13 * n.z + n.x * m._11;
    float const y = m._22 * n.y + m._23 * n.z + m._21 * n.x;
    float const z = m._32 * n.y + m._33 * n.z + m._31 * n.x;
    float const inv = static_cast<float>(1.0 / sqrt(z * z + y * y + x * x + 1.1920929e-7f));
    float const ny = inv * y;
    fr[3] = org.z * (inv * z) + org.y * ny + org.x * (inv * x) + offset;
    fr[0] = inv * x;
    fr[1] = ny;
    fr[2] = inv * z;
}

void CClipper::buildfrustum(float* fr, CVector const* const v, CMatrix const& m, CVector const& org, float offset)
{
    // RVA 0x8A0DA0 - the plane through three points, from the normal of the triangle.
    float const ex = v[2].x - v[1].x;
    float const ey = v[2].y - v[1].y;
    float const ez = v[2].z - v[1].z;
    float const fx = v[0].x - v[1].x;
    float const fy = v[0].y - v[1].y;
    float const fz = v[0].z - v[1].z;
    CVector n;
    n.x = fy * ez - fz * ey;
    n.y = fz * ex - ez * fx;
    n.z = fx * ey - fy * ex;
    buildfrustum(fr, n, m, org, offset);
}

unsigned CClipper::enableGetState() const
{
    // RVA 0x633D30
    return m_enabled;
}

void CClipper::enableSetState(unsigned news)
{
    // RVA 0x633D40
    m_enabled = news;
}

int CClipper::testSphere(CVector const& o, float r) const
{
    if (!m_nfrustums)
        return 1;

    uint32_t v4 = 0;
    for (auto i = &this->m_planes[0][1];
         ((1 << v4) & this->m_enabled) == 0 || ((((i[1] * o.z) + (*(i - 1) * o.x)) + (o.y * *i)) - i[2]) <= r;
         i += 4)
    {
        if (++v4 >= m_nfrustums)
            return 1;
    }
    return 0;
}

int CClipper::enableSetFromBox(float* minmaxs, CVector const& ofs)
{
    // RVA 0x8A08B0 - 0 when the box lies wholly in front of an enabled plane; planes the box lies wholly behind are
    // disabled.
    for (unsigned int i = 0; i < m_nfrustums; ++i)
    {
        if (((1 << i) & m_enabled) == 0)
        {
            continue;
        }
        float const* const plane = m_planes[i];
        unsigned int const* const idx = &m_indices[6 * i];
        if ((minmaxs[idx[3]] + ofs.x) * plane[0] + (minmaxs[idx[5]] + ofs.z) * plane[2]
                + (minmaxs[idx[4]] + ofs.y) * plane[1] - plane[3]
            > 0.0f)
        {
            return 0;
        }
        if ((minmaxs[idx[1]] + ofs.y) * plane[1] + (minmaxs[idx[0]] + ofs.x) * plane[0]
                + (minmaxs[idx[2]] + ofs.z) * plane[2] - plane[3]
            <= 0.0f)
        {
            m_enabled &= ~(1 << i);
        }
    }
    return 1;
}

void CClipper::createIndices()
{
    // RVA 0x8A0600 - per plane, the minmaxs indices of the box corner farthest along the normal (the first three)
    // and of the one farthest against it (the last three).
    for (unsigned int i = 0; i < m_nfrustums; ++i)
    {
        unsigned int* const idx = &m_indices[6 * i];
        for (int axis = 0; axis < 3; ++axis)
        {
            bool const positive = m_planes[i][axis] >= 0.0f;
            idx[axis] = positive ? axis + 3 : axis;
            idx[axis + 3] = positive ? axis : axis + 3;
        }
    }
}

int CClipper::clipLineZ(CVector* dst, CVector* src)
{
    // RVA 0x8A1900 - clips the segment against the last plane only.
    CVector v[2] = {src[0], src[1]};
    int const result = clipFrontSideInPlace(v, 2, m_planes[m_nfrustums - 1]);
    dst[0] = v[0];
    dst[1] = v[1];
    return result;
}

int CClipper::clipPolyInPlaceZ(CVector* verts, int nverts)
{
    // RVA 0x8A1620 - clips against the last plane only.
    return clipFrontSideInPlace(verts, nverts, m_planes[m_nfrustums - 1]);
}

void CClipper::createScreenFrustums(
    CVector const& origin, CMatrix const& rotMat, float fovx, float fovy, float znear, float zfar)
{
    // RVA 0x8A1040 - the view frustum: top, bottom, left and right planes through the eye and the near plane's
    // edges, then the far and near planes.
    float const halfHeight = static_cast<float>(tan(fovy * 0.5) * znear);
    float const halfWidth = static_cast<float>(tan(fovx * 0.5) * znear);
    CVector const eye(0.0f, 0.0f, 0.0f);
    CVector const topRight(halfWidth, halfHeight, znear);
    CVector const topLeft(0.0f - halfWidth, halfHeight, znear);
    CVector const bottomLeft(0.0f - halfWidth, 0.0f - halfHeight, znear);
    CVector const bottomRight(halfWidth, 0.0f - halfHeight, znear);
    CVector const top[3] = {eye, topRight, topLeft};
    CVector const bottom[3] = {eye, bottomLeft, bottomRight};
    CVector const left[3] = {eye, topLeft, bottomLeft};
    CVector const right[3] = {eye, bottomRight, topRight};
    buildfrustum(m_planes[0], top, rotMat, origin, 0.0f);
    buildfrustum(m_planes[1], bottom, rotMat, origin, 0.0f);
    buildfrustum(m_planes[2], left, rotMat, origin, 0.0f);
    buildfrustum(m_planes[3], right, rotMat, origin, 0.0f);
    buildfrustum(m_planes[4], CVector(0.0f, 0.0f, 1.0f), rotMat, origin, zfar);
    buildfrustum(m_planes[5], CVector(0.0f, 0.0f, -1.0f), rotMat, origin, 0.0f - znear);
    m_nfrustums = 6;
    createIndices();
    m_enabled = 0xFFFFFFFF;
}

void CClipper::CreateScreenFrustums(float farz, float lessen, float nearz, float fov)
{
    auto const viewport = M3D_APP->m_renderer->GetViewport();
    float const w = static_cast<float>(viewport.m_width);
    float const h = static_cast<float>(viewport.m_height);
    float const aspect = w / h;
    float x = 0.0;
    float y = 0.0;
    if (w <= h)
    {
        x = fov;
        y = aspect * fov;
    }
    else
    {
        x = aspect * fov;
        y = fov;
    }
    createScreenFrustums(M3D_APP->m_renderer->MatGetOrgInv(), M3D_APP->m_renderer->MatGet(), x * lessen, y * lessen, nearz, farz);
}

void CClipper::translate(CVector const& add)
{
    // RVA 0x8A0E40 - moves every plane by add.
    for (unsigned int i = 0; i < m_nfrustums; ++i)
    {
        float* const plane = m_planes[i];
        plane[3] = plane[2] * add.z + plane[0] * add.x + add.y * plane[1] + plane[3];
    }
    createIndices();
}

void CClipper::enableAll()
{
    // RVA 0x8A08A0
    m_enabled = 0xFFFFFFFF;
}

void CClipper::enableUpdateFromBox(float* minmaxs, const CVector& ofs)
{
    // RVA 0x8A09D0 - disables the planes the box (min/max corners, moved by ofs) lies wholly behind.
    for (unsigned int i = 0; i < m_nfrustums; ++i)
    {
        float const* const plane = m_planes[i];
        unsigned int const* const idx = &m_indices[6 * i];
        if ((1 << i) & m_enabled &&
            (minmaxs[idx[2]] + ofs.z) * plane[2] + (minmaxs[idx[0]] + ofs.x) * plane[0] +
                    (minmaxs[idx[1]] + ofs.y) * plane[1] - plane[3] <=
                0.0f)
        {
            m_enabled &= ~(1 << i);
        }
    }
}

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
    // TODO: generated code
    float* v4 = minmaxs;
    unsigned int m_nfrustums = this->m_nfrustums;
    unsigned int v7 = 0;

    if (m_nfrustums) {
        const float* v8 = &this->m_planes[0][1];
        const unsigned int* v9 = &this->m_indices[4];

        while (true) {
            v4 = minmaxs;
            if (((1 << v7) & this->m_enabled) != 0 &&
                ((float)((float)((float)((float)(minmaxs[v9[1]] + ofs.z) * v8[1]) +
                    (float)((float)(minmaxs[*(v9 - 1)] + ofs.x) * *(v8 - 1))) +
                    (float)((float)(minmaxs[*v9] + ofs.y) * *v8)) - v8[2]) > 50.0f) {
                return 0;
            }

            ++v7;
            v9 += 6;
            v8 += 4;

            if (v7 >= m_nfrustums) {
                break;
            }
        }
    }

    if (test == tbFullTest) {
        unsigned int v10 = 0;
        if (m_nfrustums == 0) {
            return 2;
        }

        const float* v11 = &this->m_planes[0][1];
        const unsigned int* i = &this->m_indices[1];

        while (true) {
            if (((1 << v10) & this->m_enabled) != 0 &&
                ((float)((float)((float)((float)(v4[i[1]] + ofs.z) * v11[1]) +
                    (float)((float)(v4[*(i - 1)] + ofs.x) * *(v11 - 1))) +
                    (float)((float)(v4[*i] + ofs.y) * *v11)) - v11[2] > 0.0f)) {
                break;
            }

            ++v10;
            i += 6;
            v11 += 4;

            if (v10 >= m_nfrustums) {
                return 2;
            }
        }
    }

    return 1;
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

    auto v4 = 0;
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
    //TODO: cehck this and refactor
    unsigned int v1; // edx
    unsigned int* v2; // eax
    float* v3; // esi

    v1 = 0;
    if (this->m_nfrustums)
    {
        v2 = &this->m_indices[3];
        v3 = &this->m_planes[0][2];
        do
        {
            if (*(v3 - 2) >= 0.0)
            {
                *(v2 - 3) = 3;
                *v2 = 0;
            }
            else
            {
                *(v2 - 3) = 0;
                *v2 = 3;
            }
            if (*(v3 - 1) >= 0.0)
            {
                *(v2 - 2) = 4;
                v2[1] = 1;
            }
            else
            {
                *(v2 - 2) = 1;
                v2[1] = 4;
            }
            if (*v3 >= 0.0)
            {
                *(v2 - 1) = 5;
                v2[2] = 2;
            }
            else
            {
                *(v2 - 1) = 2;
                v2[2] = 5;
            }
            ++v1;
            v3 += 4;
            v2 += 6;
        } while (v1 < this->m_nfrustums);
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

void CClipper::createScreenFrustums(CVector const& origin, CMatrix const& rotMat, float fovx, float fovy, float znear, float zfar)
{
    //TODO: check this and refactor
    float v9; // xmm7_4
    float v10; // xmm0_4
    float v11; // xmm3_4
    float v12; // xmm0_4
    float v13; // xmm3_4
    float v14; // xmm7_4
    CVector n; // [esp+10h] [ebp-3Ch] BYREF
    float v16; // [esp+1Ch] [ebp-30h]
    CVector v[3]; // [esp+28h] [ebp-24h] BYREF
    float fovxa; // [esp+58h] [ebp+Ch]
    float rx; // [esp+5Ch] [ebp+10h]
    float ry; // [esp+60h] [ebp+14h]

    v[0].x = 0.0;
    v[0].y = 0.0;
    v[0].z = 0.0;
    v[2].z = znear;
    v[1].z = znear;
    rx = tan(fovy * 0.5) * znear;
    fovxa = tan(fovx * 0.5) * znear;
    v9 = 0.0 - fovxa;
    v[2].x = 0.0 - fovxa;
    v[2].y = rx;
    v[1].x = fovxa;
    v16 = (float)(0.0 - fovxa) - fovxa;
    v[1].y = rx;
    n.x = (float)((float)(0.0 - rx) * (float)(znear - znear)) - (float)((float)(0.0 - znear) * (float)(rx - rx));
    n.y = (float)((float)(0.0 - znear) * v16) - (float)((float)(znear - znear) * (float)(0.0 - fovxa));
    n.z = (float)((float)(rx - rx) * (float)(0.0 - fovxa)) - (float)((float)(0.0 - rx) * v16);
    CClipper::buildfrustum(m_planes[0], n, rotMat, origin, 0.0);
    ry = 0.0 - rx;
    v[1].x = v9;
    v[1].z = znear;
    v[1].y = 0.0 - rx;
    v[2].x = fovxa;
    v16 = fovxa - (float)(0.0 - fovxa);
    v[2].y = 0.0 - rx;
    v10 = (float)(0.0 - rx) - (float)(0.0 - rx);
    v[2].z = znear;
    n.y = v[0].y - (float)(0.0 - rx);
    v11 = (float)(n.y * (float)(znear - znear)) - (float)((float)(v[0].z - znear) * v10);
    v12 = (float)(v10 * (float)(v[0].x - v9)) - (float)(n.y * (float)(fovxa - v9));
    n.x = v11;
    n.y = (float)((float)(v[0].z - znear) * (float)(fovxa - v9)) - (float)((float)(znear - znear) * (float)(v[0].x - v9));
    n.z = v12;
    CClipper::buildfrustum(m_planes[1], n, rotMat, origin, 0.0);
    v[2].x = 0.0 - fovxa;
    v[2].y = 0.0 - rx;
    v[2].z = znear;
    v[1].x = 0.0 - fovxa;
    v16 = v9 - v9;
    v[1].z = znear;
    v[1].y = rx;
    v13 = (float)(v[0].y - rx) * (float)(v9 - v9);
    v14 = v[0].x - (float)(0.0 - fovxa);
    n.x = (float)((float)(v[0].y - rx) * (float)(znear - znear))
        - (float)((float)(v[0].z - znear) * (float)((float)(0.0 - rx) - rx));
    n.y = (float)((float)(v[0].z - znear) * v16) - (float)((float)(znear - znear) * v14);
    n.z = (float)((float)((float)(0.0 - rx) - rx) * v14) - v13;
    CClipper::buildfrustum(m_planes[2], n, rotMat, origin, 0.0);
    v[1].x = fovxa;
    v[1].y = 0.0 - rx;
    v[1].z = znear;
    v[2].x = fovxa;
    v[2].y = rx;
    v[2].z = znear;
    n.x = (float)((float)(v[0].y - ry) * (float)(znear - znear)) - (float)((float)(v[0].z - znear) * (float)(rx - ry));
    n.y = (float)((float)(v[0].z - znear) * (float)(fovxa - fovxa))
        - (float)((float)(znear - znear) * (float)(v[0].x - fovxa));
    n.z = (float)((float)(rx - ry) * (float)(v[0].x - fovxa)) - (float)((float)(v[0].y - ry) * (float)(fovxa - fovxa));
    CClipper::buildfrustum(m_planes[3], n, rotMat, origin, 0.0);
    n.x = 0.0;
    v[0].x = 0.0;
    n.z = 1.0;
    n.y = 0.0;
    v[0].y = 0.0;
    v[0].z = 1.0;
    CClipper::buildfrustum(m_planes[4], v[0], rotMat, origin, zfar);
    n.x = 0.0;
    n.y = 0.0;
    n.z = -1.0;
    v[0].x = 0.0;
    v[0].y = 0.0;
    v[0].z = -1.0;
    CClipper::buildfrustum(m_planes[5], v[0], rotMat, origin, 0.0 - znear);
    this->m_nfrustums = 6;
    CClipper::createIndices();
    this->m_enabled = -1;
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
    // TODO: check and refactor this
    int v4 = 0;
    if (this->m_nfrustums)
    {
        auto v5 = &this->m_planes[0][1];
        auto v6 = &this->m_indices[1];
        do
        {
            if (((1 << v4) & this->m_enabled) != 0
                && (float)((float)((float)((float)((float)(minmaxs[v6[1]] + ofs.z) * v5[1])
                    + (float)((float)(minmaxs[*(v6 - 1)] + ofs.x) * *(v5 - 1)))
                    + (float)((float)(minmaxs[*v6] + ofs.y) * *v5))
                    - v5[2]) <= 0.0)
            {
                this->m_enabled &= ~(1 << v4);
            }
            ++v4;
            v6 += 6;
            v5 += 4;
        } while (v4 < this->m_nfrustums);
    }
}

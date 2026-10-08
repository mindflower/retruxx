#include "math/obb.h"
#include <corecrt_math.h>
#include "math/aabb.h"
#include <utility>
#include <cstring>

void Obb::Create(const CVector& min, const CVector& max, const CMatrix& mat, bool noScale)
{
    // RVA 0x5C7A20 - the box min..max in the space of mat. With noScale the basis is just
    // normalised; otherwise each axis' scale is moved from the basis into the extents.
    m_min = min;
    m_max = max;
    m_origin = CVector(mat._41, mat._42, mat._43);
    if (noScale)
    {
        mat.GetNormalizedBasis(m_basis[0], m_basis[1], m_basis[2]);
        return;
    }

    mat.GetBasis(m_basis[0], m_basis[1], m_basis[2]);
    for (int axis = 0; axis < 3; ++axis)
    {
        CVector& dir = m_basis[axis];
        float const scale = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        m_min[axis] = scale * m_min[axis];
        m_max[axis] = scale * m_max[axis];

        float const invScale = 1.0f / scale;
        dir.x = dir.x * invScale;
        dir.y = dir.y * invScale;
        dir.z = dir.z * invScale;
    }
}

// RVA 0x641AE0 - keeps the part of a convex polygon behind the plane (dot(v, plane.xyz) - plane.w < 0), in place.
// Returns the new vertex count: the polygon as is when nothing lies in front, 0 when nothing lies behind.
int clipFrontSideInPlace(CVector* verts, int nverts, float* plane)
{
    enum
    {
        SIDE_FRONT = 0,
        SIDE_BACK = 1,
        SIDE_ON = 2,
    };
    static CVector back[32];
    static int sides[32];
    static float dists[32];
    static int counts[3];

    counts[SIDE_FRONT] = 0;
    counts[SIDE_BACK] = 0;
    counts[SIDE_ON] = 0;
    if (nverts <= 0)
    {
        return nverts;
    }
    for (int i = 0; i < nverts; ++i)
    {
        float const dist = verts[i].x * plane[0] + verts[i].z * plane[2] + plane[1] * verts[i].y - plane[3];
        dists[i] = dist;
        if (dist < -0.1f)
        {
            sides[i] = SIDE_BACK;
        }
        else if (dist > 0.1f)
        {
            sides[i] = SIDE_FRONT;
        }
        else
        {
            sides[i] = SIDE_ON;
        }
        ++counts[sides[i]];
    }
    if (!counts[SIDE_FRONT] || counts[SIDE_ON] == nverts)
    {
        return nverts;
    }
    if (!counts[SIDE_BACK])
    {
        return 0;
    }

    int nbacks = 0;
    for (int i = 0; i < nverts; ++i)
    {
        if (sides[i] == SIDE_ON)
        {
            back[nbacks++] = verts[i];
            continue;
        }
        if (sides[i] == SIDE_BACK)
        {
            back[nbacks++] = verts[i];
        }
        // NOTE: the next side is read as sides[i + 1], not wrapped, so the last vertex looks at a stale entry.
        if (sides[i + 1] == SIDE_ON)
        {
            continue;
        }
        int const next = (i + 1) % nverts;
        if (sides[next] == sides[i])
        {
            continue;
        }
        float const t = dists[i] / (dists[i] - dists[next]);
        back[nbacks++] = CVector(
            (verts[next].x - verts[i].x) * t + verts[i].x,
            (verts[next].y - verts[i].y) * t + verts[i].y,
            (verts[next].z - verts[i].z) * t + verts[i].z);
    }
    int const result = nbacks < 32 ? nbacks : 32;
    memcpy(verts, back, sizeof(CVector) * result);
    return result;
}

// RVA 0x641D60 - clips the segment v[0]..v[1] to the box (min xyz, max xyz); false when nothing is left.
int clipLineToBox(CVector* v, float* box)
{
    float plane[4] = {-1.0f, 0.0f, 0.0f, 0.0f - box[0]};
    if (!clipFrontSideInPlace(v, 2, plane))
    {
        return 0;
    }
    float plane1[4] = {1.0f, 0.0f, 0.0f, box[3]};
    if (!clipFrontSideInPlace(v, 2, plane1))
    {
        return 0;
    }
    float plane2[4] = {0.0f, -1.0f, 0.0f, 0.0f - box[1]};
    if (!clipFrontSideInPlace(v, 2, plane2))
    {
        return 0;
    }
    float plane3[4] = {0.0f, 1.0f, 0.0f, box[4]};
    if (!clipFrontSideInPlace(v, 2, plane3))
    {
        return 0;
    }
    float plane4[4] = {0.0f, 0.0f, -1.0f, 0.0f - box[2]};
    if (!clipFrontSideInPlace(v, 2, plane4))
    {
        return 0;
    }
    float plane5[4] = {0.0f, 0.0f, 1.0f, box[5]};
    return clipFrontSideInPlace(v, 2, plane5) != 0;
}

float Obb::IntersectRay(CVector const& v0, CVector const& dir) const
{
    // RVA 0x641F60 - distance along the ray to where it enters the box, or -1. The ray is cut off at 20000.
    float const dy = v0.y - m_origin.y;
    float const dz = v0.z - m_origin.z;
    float const dx = v0.x - m_origin.x;
    CVector const start(
        m_basis[1].x * dy + m_basis[2].x * dz + m_basis[0].x * dx,
        m_basis[1].y * dy + dz * m_basis[2].y + dx * m_basis[0].y,
        m_basis[1].z * dy + m_basis[2].z * dz + m_basis[0].z * dx);
    CVector const localDir(
        m_basis[1].x * dir.y + dir.z * m_basis[2].x + m_basis[0].x * dir.x,
        dir.z * m_basis[2].y + dir.x * m_basis[0].y + m_basis[1].y * dir.y,
        m_basis[0].z * dir.x + m_basis[1].z * dir.y + dir.z * m_basis[2].z);

    // NOTE: the far end is the scaled direction itself, not the start plus it, so the segment ends near the box centre.
    CVector vv[2] = {start, CVector(localDir.x * 20000.0f, localDir.y * 20000.0f, localDir.z * 20000.0f)};
    float box[6] = {m_min.x, m_min.y, m_min.z, m_max.x, m_max.y, m_max.z};
    if (!clipLineToBox(vv, box))
    {
        return -1.0f;
    }
    double const ex = vv[0].x - start.x;
    double const ey = vv[0].y - start.y;
    double const ez = vv[0].z - start.z;
    return static_cast<float>(sqrt(ez * ez + ey * ey + ex * ex));
}

Aabb Obb::GetBounds() const
{
    // RVA 0x646F60 - the world-space bounding box of the eight corners.
    auto const corner = [this](float x, float y, float z)
    {
        // World coordinate c is dot(m_basis[c], local) + m_origin.c, summed z, y, x.
        CVector w;
        w.x = ((m_basis[0].z * z + m_basis[0].y * y) + m_basis[0].x * x) + m_origin.x;
        w.y = ((m_basis[1].z * z + m_basis[1].y * y) + m_basis[1].x * x) + m_origin.y;
        w.z = ((m_basis[2].z * z + m_basis[2].y * y) + m_basis[2].x * x) + m_origin.z;
        return w;
    };

    CVector const v[8] = {
        corner(m_min.x, m_min.y, m_min.z),
        corner(m_min.x, m_min.y, m_max.z),
        corner(m_min.x, m_max.y, m_max.z),
        corner(m_max.x, m_max.y, m_max.z),
        corner(m_max.x, m_max.y, m_min.z),
        corner(m_max.x, m_min.y, m_min.z),
        corner(m_min.x, m_max.y, m_min.z),
        corner(m_max.x, m_min.y, m_max.z),
    };

    CVector minV = v[0];
    CVector maxV = v[0];
    for (int c = 0; c < 3; ++c)
    {
        for (int k = 1; k < 8; ++k)
        {
            if (v[k][c] > maxV[c])
            {
                maxV[c] = v[k][c];
            }
            if (minV[c] > v[k][c])
            {
                minV[c] = v[k][c];
            }
        }
    }

    Aabb box;
    box.m_box[0] = minV.x;
    box.m_box[1] = minV.y;
    box.m_box[2] = minV.z;
    box.m_box[3] = maxV.x;
    box.m_box[4] = maxV.y;
    box.m_box[5] = maxV.z;
    return box;
}

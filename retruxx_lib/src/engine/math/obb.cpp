#include "math/obb.h"
#include <corecrt_math.h>
#include "math/aabb.h"
#include <utility>

void Obb::Create(const CVector& min, const CVector& max, const CMatrix& mat, bool noScale)
{
    // TODO: check this
    this->m_min = min;
    this->m_max = max;
    auto _42 = mat._42;
    auto _43 = mat._43;
    this->m_origin.x = mat._41;
    this->m_origin.y = _42;
    this->m_origin.z = _43;
    if (noScale)
    {
        mat.GetNormalizedBasis(m_basis[0], m_basis[1], m_basis[2]);
    }
    else
    {
        this->m_basis[0].x = mat._11;
        this->m_basis[0].y = mat._21;
        this->m_basis[0].z = mat._31;
        this->m_basis[1].x = mat._12;
        this->m_basis[1].y = mat._22;
        this->m_basis[1].z = mat._32;
        this->m_basis[2].x = mat._13;
        this->m_basis[2].y = mat._23;
        this->m_basis[2].z = mat._33;
        auto p_m_max = &this->m_max;
        auto p_z = &this->m_basis[0].z;
        auto v10 = 3;
        do
        {
            auto v11 = *p_z;
            auto v12 = *(p_z - 1);
            p_m_max = (CVector*)((char*)p_m_max + 4);
            auto v13 = *(p_z - 2);
            p_z += 3;
            --v10;
            auto v14 = sqrt(v13 * v13 + v12 * v12 + v11 * v11);
            auto noScalea = v14;
            p_m_max[-2].z = v14 * p_m_max[-2].z;
            p_m_max[-1].z = noScalea * p_m_max[-1].z;
            *(p_z - 5) = *(p_z - 5) * (float)(1.0 / noScalea);
            *(p_z - 4) = *(p_z - 4) * (float)(1.0 / noScalea);
            *(p_z - 3) = *(p_z - 3) * (float)(1.0 / noScalea);
        } while (v10);
    }
}

Aabb Obb::GetBounds() const
{
    // TODO: generated code
    // Calculate all 8 vertices of the OBB in world space
    CVector v[8];

    // Extract OBB properties for readability
    const CVector& origin = this->m_origin;
    const CVector& basis0 = this->m_basis[0];
    const CVector& basis1 = this->m_basis[1];
    const CVector& basis2 = this->m_basis[2];
    const CVector& min = this->m_min;
    const CVector& max = this->m_max;

    // Vertex 0: (min.x, min.y, min.z)
    v[0].x = (basis0.x * min.x + basis0.y * min.y + basis0.z * min.z) + origin.x;
    v[0].y = (basis1.x * min.x + basis1.y * min.y + basis1.z * min.z) + origin.y;
    v[0].z = (basis2.x * min.x + basis2.y * min.y + basis2.z * min.z) + origin.z;

    // Vertex 1: (min.x, min.y, max.z)
    v[1].x = (basis0.x * min.x + basis0.y * min.y + basis0.z * max.z) + origin.x;
    v[1].y = (basis1.x * min.x + basis1.y * min.y + basis1.z * max.z) + origin.y;
    v[1].z = (basis2.x * min.x + basis2.y * min.y + basis2.z * max.z) + origin.z;

    // Vertex 2: (min.x, max.y, min.z)
    v[2].x = (basis0.x * min.x + basis0.y * max.y + basis0.z * min.z) + origin.x;
    v[2].y = (basis1.x * min.x + basis1.y * max.y + basis1.z * min.z) + origin.y;
    v[2].z = (basis2.x * min.x + basis2.y * max.y + basis2.z * min.z) + origin.z;

    // Vertex 3: (min.x, max.y, max.z)
    v[3].x = (basis0.x * min.x + basis0.y * max.y + basis0.z * max.z) + origin.x;
    v[3].y = (basis1.x * min.x + basis1.y * max.y + basis1.z * max.z) + origin.y;
    v[3].z = (basis2.x * min.x + basis2.y * max.y + basis2.z * max.z) + origin.z;

    // Vertex 4: (max.x, min.y, min.z)
    v[4].x = (basis0.x * max.x + basis0.y * min.y + basis0.z * min.z) + origin.x;
    v[4].y = (basis1.x * max.x + basis1.y * min.y + basis1.z * min.z) + origin.y;
    v[4].z = (basis2.x * max.x + basis2.y * min.y + basis2.z * min.z) + origin.z;

    // Vertex 5: (max.x, min.y, max.z)
    v[5].x = (basis0.x * max.x + basis0.y * min.y + basis0.z * max.z) + origin.x;
    v[5].y = (basis1.x * max.x + basis1.y * min.y + basis1.z * max.z) + origin.y;
    v[5].z = (basis2.x * max.x + basis2.y * min.y + basis2.z * max.z) + origin.z;

    // Vertex 6: (max.x, max.y, min.z)
    v[6].x = (basis0.x * max.x + basis0.y * max.y + basis0.z * min.z) + origin.x;
    v[6].y = (basis1.x * max.x + basis1.y * max.y + basis1.z * min.z) + origin.y;
    v[6].z = (basis2.x * max.x + basis2.y * max.y + basis2.z * min.z) + origin.z;

    // Vertex 7: (max.x, max.y, max.z)
    v[7].x = (basis0.x * max.x + basis0.y * max.y + basis0.z * max.z) + origin.x;
    v[7].y = (basis1.x * max.x + basis1.y * max.y + basis1.z * max.z) + origin.y;
    v[7].z = (basis2.x * max.x + basis2.y * max.y + basis2.z * max.z) + origin.z;

    // Find min and max bounds by iterating through all vertices
    CVector minV = v[0];
    CVector maxV = v[0];

    for (int i = 1; i < 8; i++) {
        minV.x = std::min(minV.x, v[i].x);
        minV.y = std::min(minV.y, v[i].y);
        minV.z = std::min(minV.z, v[i].z);

        maxV.x = std::max(maxV.x, v[i].x);
        maxV.y = std::max(maxV.y, v[i].y);
        maxV.z = std::max(maxV.z, v[i].z);
    }

    Aabb result;

    // Store result in AABB format
    result.m_box[0] = minV.x; // minX
    result.m_box[1] = maxV.x; // maxX
    result.m_box[2] = minV.y; // minY
    result.m_box[3] = maxV.y; // maxY
    result.m_box[4] = minV.z; // minZ
    result.m_box[5] = maxV.z; // maxZ

    return result;
}

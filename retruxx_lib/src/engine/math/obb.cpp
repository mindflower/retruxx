#include "math/obb.h"
#include <corecrt_math.h>

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

#include <clipper.h>
#include <stdexcept>

#include "m3dapp.h"
#include "math/matrix.h"

int CClipper::testVertexInside(CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CClipper::CreateFromWinding(int, CVector*, int, CVector const&, CPlane*)
{
    throw std::logic_error("Not implemented");
}

int CClipper::clipPolyInPlace(CVector*, int)
{
    throw std::logic_error("Not implemented");
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

void CClipper::buildfrustum(float*, CVector const&, CMatrix const&, CVector const&, float)
{
    throw std::logic_error("Not implemented");
}

void CClipper::buildfrustum(float* fr, CVector const* const n, CMatrix const& m, CVector const& org, float offset)
{
    //TODO: check this and refactor
    float v5; // xmm0_4
    float v6; // xmm1_4
    float v7; // xmm1_4
    float v8; // [esp+8h] [ebp-10h]
    float ma; // [esp+1Ch] [ebp+4h]

    v5 = (float)((float)(m._12 * n->y) + (float)(m._13 * n->z)) + (float)(n->x * m._11);
    v6 = (float)((float)(m._22 * n->y) + (float)(m._23 * n->z)) + (float)(m._21 * n->x);
    v8 = (float)((float)(m._32 * n->y) + (float)(m._33 * n->z)) + (float)(m._31 * n->x);
    ma = 1.0 / sqrt(v8 * v8 + v6 * v6 + v5 * v5 + 0.00000011920929);
    v7 = ma * v6;
    fr[3] = (float)((float)((float)(org.z * (float)(ma * v8)) + (float)(org.y * v7)) + (float)(org.x * (float)(ma * v5)))
        + offset;
    *fr = ma * v5;
    fr[1] = v7;
    fr[2] = ma * v8;
}

unsigned CClipper::enableGetState() const
{
    throw std::logic_error("Not implemented");
}

void CClipper::enableSetState(unsigned)
{
    throw std::logic_error("Not implemented");
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

int CClipper::enableSetFromBox(float*, CVector const&)
{
    throw std::logic_error("Not implemented");
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

int CClipper::clipLineZ(CVector*, CVector*)
{
    throw std::logic_error("Not implemented");
}

int CClipper::clipPolyInPlaceZ(CVector*, int)
{
    throw std::logic_error("Not implemented");
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
    CClipper::buildfrustum((float*)this, &n, rotMat, origin, 0.0);
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
    CClipper::buildfrustum(this->m_planes[1], &n, rotMat, origin, 0.0);
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
    CClipper::buildfrustum(this->m_planes[2], &n, rotMat, origin, 0.0);
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
    CClipper::buildfrustum(this->m_planes[3], &n, rotMat, origin, 0.0);
    n.x = 0.0;
    v[0].x = 0.0;
    n.z = 1.0;
    n.y = 0.0;
    v[0].y = 0.0;
    v[0].z = 1.0;
    CClipper::buildfrustum(this->m_planes[4], v, rotMat, origin, zfar);
    n.x = 0.0;
    n.y = 0.0;
    n.z = -1.0;
    v[0].x = 0.0;
    v[0].y = 0.0;
    v[0].z = -1.0;
    CClipper::buildfrustum(this->m_planes[5], v, rotMat, origin, 0.0 - znear);
    this->m_nfrustums = 6;
    CClipper::createIndices();
    this->m_enabled = -1;
}

void CClipper::CreateScreenFrustums(float farz, float lessen, float nearz, float fov)
{
    auto const viewport = M3D_APP->m_renderer->GetViewport();
    float x = 0.0;
    float y = 0.0;
    if (viewport.m_width <= viewport.m_height)
    {
        x = fov;
        y = (viewport.m_width / viewport.m_height) * fov;
    }
    else
    {
        x = (viewport.m_width / viewport.m_height) * fov;
        y = fov;
    }
    createScreenFrustums(M3D_APP->m_renderer->MatGetOrgInv(), M3D_APP->m_renderer->MatGet(), x * lessen, y * lessen, nearz, farz);
}

void CClipper::translate(CVector const&)
{
    throw std::logic_error("Not implemented");
}

void CClipper::enableAll()
{
    m_enabled = 1;
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

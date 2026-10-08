// The view / world / projection matrix stacks and the camera of CDevice.
// Ported from the original dxrender9/matrices.cpp (matrices.obj).
#include "device.h"
#include "log.h"

// ------------------------------------------------------------------------------------------------
// Engine inlines the original compiler instantiated into matrices.obj (core/math/coremath.h). retruxx
// does not declare project/unproject; they are restored here as file-static helpers
// (calcDeterminantSimple / getInverseSimple, matrix.h:222 / :435, are CMatrix members defined in
// lib/engine/math/matrix.cpp).

// orig 0x625c90 coremath.h:183 - world point to screen pixel through the view rotation of m
// (translation ignored) and the projection scales scx/scy of a w x h viewport.
static CVector project(CMatrix const& m, float scx, float scy, int w, int h, CVector const& world)
{
    CVector view = m.vecRot(world);

    return CVector(w * view.x * scx / (2.0f * view.z) + w / 2, h / 2 - h * view.y * scy / (2.0f * view.z),
                   view.z);
}

// orig 0x625db0 coremath.h:196 - screen pixel to a unit direction in the space of m.
static CVector unproject(CMatrix const& m, float scx, float scy, int w, int h, CVector2 const& scr)
{
    CVector v((scr.x * 2.0f / w - 1.0f) / scx, -((scr.y * 2.0f / h - 1.0f) / scy), 1.0f);
    CVector dir = m.vecRotBack(v);

    return dir.getNormalized();
}

// ------------------------------------------------------------------------------------------------

// orig 0x625ed0 matrices.cpp:33
void CDevice::InitMatrices()
{
    m_matViewStackTop = 0;
    m_matViewStack[m_matViewStackTop].identity();

    m_matProjStackTop = 0;
    m_matProjStack[m_matProjStackTop].identity();

    m_matWorldStackTop = 0;
    m_matWorldStack[m_matWorldStackTop].identity();

    m_fastClipEnabled = false;

    m_matViewIsNotActuated = true;
    m_matInvViewIsNotActuated = true;
    m_matWorldIsNotActuated = true;
    m_updateModelViewProj = true;
    m_updateModelViewProjWithWorld = true;
    m_updateModelMatrix = true;
}

// orig 0x625530 matrices.cpp:58
HRESULT CDevice::SetXFormMatrix(int num, CMatrix const& mat)
{
    m_stats.swMatrices++;
    return m_pd3dDevice->SetTransform((D3DTRANSFORMSTATETYPE)num, (D3DMATRIX const*)&mat);
}

// orig 0x625560 matrices.cpp:66
void CDevice::ActuateMatrices()
{
    if (m_matWorldIsNotActuated)
    {
        SetXFormMatrix(D3DTS_WORLD, m_matWorldStack[m_matWorldStackTop]);
        m_matWorldIsNotActuated = false;
    }

    if (m_matViewIsNotActuated)
    {
        SetXFormMatrix(D3DTS_VIEW, m_matViewStack[m_matViewStackTop]);
        m_matViewIsNotActuated = false;
    }
}

// orig 0x6255e0 matrices.cpp:83
void CDevice::MatPush()
{
    if (m_matViewStackTop < 63)
        m_matViewStackTop++;
    m_matViewStack[m_matViewStackTop] = m_matViewStack[m_matViewStackTop - 1];

    m_matViewIsNotActuated = true;
    m_updateModelViewProj = true;
    m_updateModelMatrix = true;
}

// orig 0x625630 matrices.cpp:102 - dontset is not read.
void CDevice::MatPop(bool dontset)
{
    if (m_matViewStackTop > 0)
        m_matViewStackTop--;

    m_matInvViewIsNotActuated = true;
    m_matViewIsNotActuated = true;
    m_updateModelViewProj = true;
    m_updateModelMatrix = true;
}

// orig 0x625660 matrices.cpp:129
void CDevice::MatPush(CMatrix const& mat)
{
    MatPush();
    MatMul(mat);
}

// orig 0x625f90 matrices.cpp:137
CMatrix const& CDevice::MatGetInv()
{
    if (m_matInvViewIsNotActuated)
    {
        m_matInvView = m_matViewStack[m_matViewStackTop].getInverseSimple();
        m_matInvViewIsNotActuated = false;
    }
    return m_matInvView;
}

// orig 0x625680 matrices.cpp:146
void CDevice::MatSet(CMatrix const& mat)
{
    m_matViewStack[m_matViewStackTop] = mat;

    m_matInvViewIsNotActuated = true;
    m_matViewIsNotActuated = true;
    m_updateModelViewProj = true;
    m_updateModelMatrix = true;
}

// orig 0x6256c0 matrices.cpp:160 - view = mat * view
void CDevice::MatMul(CMatrix const& mat)
{
    D3DXMatrixMultiply((D3DXMATRIX*)&m_matViewStack[m_matViewStackTop], (D3DXMATRIX const*)&mat,
                       (D3DXMATRIX const*)&m_matViewStack[m_matViewStackTop]);

    m_matInvViewIsNotActuated = true;
    m_matViewIsNotActuated = true;
    m_updateModelViewProj = true;
    m_updateModelMatrix = true;
}

// orig 0x625fe0 matrices.cpp:180 - view = view * mat
void CDevice::MatMulR(CMatrix const* mat)
{
    m_matViewStack[m_matViewStackTop] = m_matViewStack[m_matViewStackTop] * *mat;

    m_matInvViewIsNotActuated = true;
    m_matViewIsNotActuated = true;
    m_updateModelViewProj = true;
    m_updateModelMatrix = true;
}

// orig 0x6263e0 matrices.cpp:194
CVector CDevice::MatGetOrgInv()
{
    if (m_matInvViewIsNotActuated)
    {
        m_matInvView = m_matViewStack[m_matViewStackTop].getInverseSimple();
        m_matInvViewIsNotActuated = false;
    }
    return m_matInvView.getOrg();
}

// orig 0x626460 matrices.cpp:203
CVector CDevice::MatGetOrg()
{
    return m_matViewStack[m_matViewStackTop].getOrg();
}

// orig 0x625700 matrices.cpp:210
void CDevice::MatSetWorld(CMatrix const& mat)
{
    m_matWorldStack[m_matWorldStackTop] = mat;

    m_matWorldIsNotActuated = true;
    m_updateModelViewProjWithWorld = true;
    m_updateModelMatrix = true;
}

// orig 0x625740 matrices.cpp:225
void CDevice::MatPushWorld()
{
    if (m_matWorldStackTop < 63)
        m_matWorldStackTop++;
    m_matWorldStack[m_matWorldStackTop] = m_matWorldStack[m_matWorldStackTop - 1];

    m_matWorldIsNotActuated = true;
}

// orig 0x625780 matrices.cpp:239
void CDevice::MatPopWorld()
{
    if (m_matWorldStackTop > 0)
        m_matWorldStackTop--;

    m_matWorldIsNotActuated = true;
    m_updateModelViewProjWithWorld = true;
    m_updateModelMatrix = true;
}

// orig 0x6257b0 matrices.cpp:258
void CDevice::MatSetProj(CMatrix const& mat)
{
    m_matProjStack[m_matProjStackTop] = mat;
    SetXFormMatrix(D3DTS_PROJECTION, m_matProjStack[m_matProjStackTop]);

    m_updateModelViewProj = true;
    ForceRecalcClipPlanes();
}

// orig 0x625810 matrices.cpp:274
void CDevice::ActuateProjectionMatrix()
{
    SetXFormMatrix(D3DTS_PROJECTION,
                   m_fastClipEnabled ? m_fastClipPlaneProjMatrix : m_matProjStack[m_matProjStackTop]);
}

// orig 0x625860 matrices.cpp:289
void CDevice::MatPushProj()
{
    if (m_matProjStackTop < 63)
        m_matProjStackTop++;
    m_matProjStack[m_matProjStackTop] = m_matProjStack[m_matProjStackTop - 1];

    m_updateModelViewProj = true;
    ForceRecalcClipPlanes();
}

// orig 0x6258b0 matrices.cpp:307
void CDevice::MatPopProj()
{
    if (m_matProjStackTop > 0)
        m_matProjStackTop--;
    SetXFormMatrix(D3DTS_PROJECTION, m_matProjStack[m_matProjStackTop]);

    m_updateModelViewProj = true;
    ForceRecalcClipPlanes();
}

// orig 0x6264a0 matrices.cpp:328
CVector CDevice::Unproject(CVector2 const& scr)
{
    int w = GetViewport().m_width;
    int h = GetViewport().m_height;
    float scx = MatGetProj()._11;
    float scy = MatGetProj()._22;

    return unproject(MatGet(), scx, scy, w, h, scr);
}

// orig 0x626520 matrices.cpp:340
CVector CDevice::Project(CVector const& world)
{
    int w = GetViewport().m_width;
    int h = GetViewport().m_height;
    float scx = MatGetProj()._11;
    float scy = MatGetProj()._22;

    return project(MatGet(), scx, scy, w, h, world);
}

// orig 0x6265a0 matrices.cpp:352 - the original returns the CVector by value; HTA's signature passes the
// return slot explicitly, which is the same ABI.
CVector* CDevice::ProjectWorldAbs(CVector* result, CVector const* world)
{
    CVector projectedVec;
    D3DXVec3Project((D3DXVECTOR3*)&projectedVec, (D3DXVECTOR3 const*)world, &m_curViewportD3D,
                    (D3DXMATRIX const*)&m_matProjStack[m_matProjStackTop],
                    (D3DXMATRIX const*)&m_matViewStack[m_matViewStackTop], NULL);

    *result = projectedVec;
    return result;
}

// orig 0x626610 matrices.cpp:369
CMatrix const* CDevice::GetModelViewProjMatrix()
{
    static CMatrix modelViewProj;

    if (m_updateModelViewProjWithWorld)
    {
        modelViewProj = m_matWorldStack[m_matWorldStackTop] * m_matViewStack[m_matViewStackTop] *
                        m_matProjStack[m_matProjStackTop];
    }
    else if (m_updateModelViewProj)
    {
        D3DXMatrixMultiply((D3DXMATRIX*)&modelViewProj, (D3DXMATRIX const*)&m_matViewStack[m_matViewStackTop],
                           (D3DXMATRIX const*)&m_matProjStack[m_matProjStackTop]);
    }

    m_updateModelViewProj = false;
    m_updateModelViewProjWithWorld = false;

    return &modelViewProj;
}

// orig 0x626e30 matrices.cpp:401
void CDevice::SetViewMatrix(CMatrix const& viewMatrix)
{
    m_viewMatrix = viewMatrix;
    m_viewMatrixInv = m_viewMatrix.getInverseSimple();
    m_viewOrigin = m_viewMatrixInv.getOrg();

    m_viewMatrixWasSetThisFrame = true;
    ForceRecalcClipPlanes();

    m_updateModelMatrix = true;
}

// orig 0x625900 matrices.cpp:417
CMatrix const& CDevice::GetViewMatrix()
{
    return m_viewMatrix;
}

// orig 0x625910 matrices.cpp:425
CMatrix const& CDevice::GetInvViewMatrix()
{
    return m_viewMatrixInv;
}

// orig 0x625920 matrices.cpp:433
CVector const& CDevice::GetViewOrigin()
{
    return m_viewOrigin;
}

// orig 0x626ed0 matrices.cpp:440
CMatrix const& CDevice::GetModelMatrix()
{
    static CMatrix modelMatrix;

    if (m_updateModelMatrix)
    {
        CMatrix worldView = MatGetWorld() * MatGet();
        modelMatrix = worldView * m_viewMatrixInv;
    }

    m_updateModelMatrix = false;

    return modelMatrix;
}

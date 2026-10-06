// User clip planes and the "fast" clip plane (a projection matrix with the clip plane folded into
// its z column) of CDevice.
// Ported from the original dxrender9/clipplanes.cpp (clipPlanes.obj).
#include "device.h"
#include "log.h"

#include <math/vector4.h>

// The engine inlines the original compiler instantiated into clipPlanes.obj (CMatrix::transposeInplace
// matrix.h:803, CMatrix::createPlaneTransform matrix.h:1264) are defined in
// lib/engine/math/matrix.cpp.

// orig 0x6277e0 clipplanes.cpp:25
unsigned int CDevice::GetMaxClipPlanes()
{
    return m_d3dCaps.MaxUserClipPlanes;
}

// orig 0x628a00 clipplanes.cpp:32
void CDevice::SetClipPlane(int index, CPlane const* plane)
{
    D3DXPLANE newPlane(plane->m_normal.x, plane->m_normal.y, plane->m_normal.z, -plane->m_dist);

    if (newPlane != m_userClipPlanesWorld[index])
    {
        m_userClipPlanesUpdated[index] = true;
        m_userClipPlanesWorld[index] = newPlane;
    }

    ForceRecalcClipPlanes();
}

// orig 0x6277f0 clipplanes.cpp:52
void CDevice::EnableClipPlane(int index, bool bEnable)
{
    int mask = 1 << index;

    if (bEnable)
    {
        if ((m_userClipPlaneEnabled & mask) && !m_userClipPlanesUpdated[index])
            return;

        m_userClipPlaneEnabled |= mask;
    }
    else
    {
        if (!(m_userClipPlaneEnabled & mask))
            return;

        m_userClipPlaneEnabled &= ~mask;
    }

    m_userClipPlanesUpdated[index] = false;
    setRenderState(D3DRS_CLIPPLANEENABLE, m_userClipPlaneEnabled);
}

// orig 0x627a10 clipplanes.cpp:81 - the world-space planes into clip space (for shaders). The loop
// bound is compared signed in 1.5.
void CDevice::ForceRecalcClipPlanes()
{
    CMatrix worldToProjInvTransposed = GetViewMatrix() * MatGetProj();
    worldToProjInvTransposed.createPlaneTransform();

    for (int i = 0; i < (int)GetMaxClipPlanes(); i++)
    {
        CVector4 planeCoof(m_userClipPlanesWorld[i].a, m_userClipPlanesWorld[i].b, m_userClipPlanesWorld[i].c,
                           m_userClipPlanesWorld[i].d);

        planeCoof = worldToProjInvTransposed.vecMul(planeCoof);

        m_userClipPlanesProj[i] = D3DXPLANE(planeCoof.x, planeCoof.y, planeCoof.z, planeCoof.w);
    }
}

// orig 0x627860 clipplanes.cpp:106
void CDevice::ActuateClipPlanes(bool bForFFP)
{
    for (int i = 0; i < (int)GetMaxClipPlanes(); i++)
    {
        int mask = 1 << i;

        if (m_userClipPlaneEnabled & mask)
        {
            D3DXPLANE const& plane = bForFFP ? m_userClipPlanesWorld[i] : m_userClipPlanesProj[i];

            m_pd3dDevice->SetClipPlane(i, plane);
        }
    }
}

// orig 0x6278d0 clipplanes.cpp:127
void CDevice::EnableFastClipPlane(bool bEnable)
{
    m_fastClipEnabled = bEnable;
}

// orig 0x627fc0 clipplanes.cpp:134
void CDevice::SetFastClipPlane(CPlane const& plane)
{
    D3DXPLANE newPlane(plane.m_normal.x, plane.m_normal.y, plane.m_normal.z, -plane.m_dist);

    if (newPlane != m_fastClipPlane)
    {
        m_fastClipPlane = newPlane;

        CVector4 planeCoof(plane.m_normal.x, plane.m_normal.y, plane.m_normal.z, -plane.m_dist);
        CMatrix worldToProjInvTransposed = GetViewMatrix() * MatGetProj();
        worldToProjInvTransposed = worldToProjInvTransposed.getInverse();
        worldToProjInvTransposed = worldToProjInvTransposed.getTransposed();

        planeCoof = worldToProjInvTransposed.vecMul(planeCoof);

        m_fastClipPlaneProjMatrix.identity();

        m_fastClipPlaneProjMatrix._13 = planeCoof.x;
        m_fastClipPlaneProjMatrix._23 = planeCoof.y;
        m_fastClipPlaneProjMatrix._33 = planeCoof.z;
        m_fastClipPlaneProjMatrix._43 = planeCoof.w;

        m_fastClipPlaneProjMatrix = MatGetProj() * m_fastClipPlaneProjMatrix;
    }
}

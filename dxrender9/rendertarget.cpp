// rendertarget.cpp - render-to-texture: the full-frame and buffered render target textures and the
// render target switch.
// Ported from the original dxrender9/rendertarget.cpp.
#include "device.h"
#include "log.h"

#include <config.h>
#include <core/kernel.h>

// orig 0x64f940 rendertarget.cpp:29
TexHandle CDevice::GetFullFrameFrameBufferTexture()
{
    if (!m_texFrameBufer.IsValid())
    {
        m_texFrameBufer = AddDynamicTexture("$TexFrameBufer", g_kernel->GetEngineCfg().m_r_width.GetI(),
                                            g_kernel->GetEngineCfg().m_r_height.GetI(), TM_DTF_RENDER_TARGET);

        SetTextureParameter(m_texFrameBufer, TM_WRAP_S, 3);
        SetTextureParameter(m_texFrameBufer, TM_WRAP_T, 3);
    }

    return m_texFrameBufer;
}

// orig 0x650770 rendertarget.cpp:44
TexHandle CDevice::GetBufferedTargetTexture(int sz)
{
    std::map<int, TexHandle>::iterator it = m_texBufferedRT.find(sz);
    if (it == m_texBufferedRT.end() || !it->second.IsValid())
    {
        TexHandle th = AddDynamicTexture("$TexBufferedRT", sz, sz, TM_DTF_RENDER_TARGET);

        SetTextureParameter(th, TM_WRAP_S, 3);
        SetTextureParameter(th, TM_WRAP_T, 3);
        ReferenceTexture(th);
        m_texBufferedRT[sz] = th;

        return th;
    }

    return it->second;
}

// orig 0x64f560 rendertarget.cpp:66
TexHandle CDevice::AddRenderTargetTexture(const char* texName, int sx, int sy)
{
    return AddDynamicTexture(texName, sx, sy, TM_DTF_RENDER_TARGET);
}

// orig 0x650190 rendertarget.cpp:73
int CDevice::RenderToTexStart(TexHandle const& destTex, bool wantZBuffer, TexHandle const& depthStencil)
{
    if (!destTex.IsValid())
        return 0;

    Viewport port;
    GetDims(destTex, port.m_width, port.m_height);
    port.m_x0 = 0;
    port.m_y0 = 0;
    port.m_zMin = 0.0f;
    port.m_zMax = 1.0f;

    m_rtsPtr = 0;
    m_lastResult = m_textures[TexId(destTex)].m_maps[0]->m_pTex2d->GetSurfaceLevel(0, &m_rtsPtr);
    if (FAILED(m_lastResult))
    {
        LogMsg("d3d: ERROR! RenderToTexStart:: Cannot get surface level");
        return 0;
    }

    m_rtsSaveColor = 0;
    m_lastResult = m_pd3dDevice->GetRenderTarget(0, &m_rtsSaveColor);
    if (FAILED(m_lastResult))
    {
        LogMsg("d3d: ERROR! RenderToTexStart:: Cannot get rt");
        m_rtsPtr->Release();
        return 0;
    }

    m_rtsSaveZs = 0;
    m_lastResult = m_pd3dDevice->GetDepthStencilSurface(&m_rtsSaveZs);
    if (FAILED(m_lastResult))
    {
        LogMsg("d3d: ERROR! RenderToTexStart:: Cannot get depth stencil");
        m_rtsPtr->Release();
        m_rtsSaveColor->Release();
        return 0;
    }

    m_rtsSaveViewport = GetViewport();

    m_rtsNewZs = 0;
    if (wantZBuffer)
    {
        if (!IsTexValid(depthStencil))
        {
            std::map<unsigned int, IDirect3DSurface9*>::iterator it = m_rtsZSurfaces.find(port.m_width);

            if (it == m_rtsZSurfaces.end())
            {
                IDirect3DSurface9* zSurf = 0;
                m_lastResult = m_pd3dDevice->CreateDepthStencilSurface(port.m_width, port.m_height, m_depthStencilFormatRt,
                                                                       D3DMULTISAMPLE_NONE, 0, TRUE, &zSurf, NULL);
                if (FAILED(m_lastResult))
                    LogMsg("d3d: ERROR! RenderToTexStart:: Cannot create zsurface");

                it = m_rtsZSurfaces.insert(std::make_pair(port.m_width, zSurf)).first;
            }

            m_rtsNewZs = it->second;

            if (m_rtsNewZs)
                m_rtsNewZs->AddRef();
        }
        else
        {
            m_textures[TexId(depthStencil)].m_maps[0]->m_pTex2d->GetSurfaceLevel(0, &m_rtsNewZs);
        }
    }

    m_rtsWantZ = wantZBuffer;
    if (!wantZBuffer)
        PushZbState(ZB_DISABLE);

    m_lastResult = m_pd3dDevice->SetRenderTarget(0, m_rtsPtr);
    if (FAILED(m_lastResult))
    {
        LogMsg("d3d: ERROR! RenderToTexStart:: Cannot set rt");
        return 0;
    }

    m_lastResult = m_pd3dDevice->SetDepthStencilSurface(m_rtsNewZs);
    if (FAILED(m_lastResult))
        LogMsg("d3d: ERROR! RenderToTexStart:: Cannot set depth stencil surface");

    MatPush();
    MatPushProj();

    SetViewport(port);

    m_activeStencilTarget = 1;
    m_stencilLevel[1] = -1;

    m_stats.swRenderTargets++;

    return 1;
}

// HTA-only wrapper, no original body
int CDevice::RenderToTexStart(TexHandle const& destTex, bool wantZBuffer)
{
    TexHandle h;
    h.SetInvalid();
    return RenderToTexStart(destTex, wantZBuffer, h);
}

// orig 0x64f590 rendertarget.cpp:202
void CDevice::RenderToTexFinish()
{
    if (m_rtsPtr)
    {
        m_rtsPtr->Release();

        m_pd3dDevice->SetRenderTarget(0, m_rtsSaveColor);
        m_pd3dDevice->SetDepthStencilSurface(m_rtsSaveZs);
        SetViewport(m_rtsSaveViewport);

        if (m_rtsSaveColor)
            m_rtsSaveColor->Release();

        if (m_rtsSaveZs)
            m_rtsSaveZs->Release();

        if (m_rtsNewZs)
            m_rtsNewZs->Release();

        MatPop(false);
        MatPopProj();

        if (!m_rtsWantZ)
            PopZbState();

        m_activeStencilTarget = 0;
    }
}

// orig 0x64fb20 rendertarget.cpp:234
void CDevice::CopyRenderTargetToTexture(TexHandle const& destTex)
{
    if (!destTex.IsValid())
        return;

    IDirect3DSurface9* srcSurf;
    HRESULT hr = m_pd3dDevice->GetRenderTarget(0, &srcSurf);
    if (FAILED(hr))
        return;

    IDirect3DSurface9* dstSurf;
    hr = m_textures[TexId(destTex)].m_maps[0]->m_pTex2d->GetSurfaceLevel(0, &dstSurf);

    if (SUCCEEDED(hr))
    {
        m_pd3dDevice->StretchRect(srcSurf, NULL, dstSurf, NULL, D3DTEXF_NONE);

        dstSurf->Release();
    }
    srcSurf->Release();
}

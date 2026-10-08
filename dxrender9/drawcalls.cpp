// Draw calls and the scene bracket (the original dxrender9/drawcalls.cpp).
#include <config.h>
#include <core/console/cvar.h>
#include <core/kernel.h>

#include "device.h"
#include "log.h"
#include "shaders/effects/effect.h"

// orig data 0x852728 (drawcalls.cpp): PrimType -> D3DPRIMITIVETYPE.
static D3DPRIMITIVETYPE const m3dPtToD3dPt[6] = {
    D3DPT_POINTLIST,      // M3DPT_POINTLIST
    D3DPT_LINELIST,       // M3DPT_LINELIST
    D3DPT_LINESTRIP,      // M3DPT_LINESTRIP
    D3DPT_TRIANGLELIST,   // M3DPT_TRIANGLELIST
    D3DPT_TRIANGLESTRIP,  // M3DPT_TRIANGLESTRIP
    D3DPT_TRIANGLEFAN,    // M3DPT_TRIANGLEFAN
};

// orig 0x644d40 drawcalls.cpp:49
int CDevice::DrawIndexedPrimitive(PrimType Type, unsigned int MinIndex, unsigned int NumVertices,
                                  unsigned int StartIndex, unsigned int PrimitiveCount)
{
    setVertexShader(0);
    setPixelShader(0);
    ActuateStates(true);

    m_curIbBaseIdx = m_latchedIbBaseIdx;
    HRESULT hr = m_pd3dDevice->DrawIndexedPrimitive(m3dPtToD3dPt[Type], m_latchedIbBaseIdx, MinIndex, NumVertices,
                                                    StartIndex, PrimitiveCount);

    m_stats.polyCount += PrimitiveCount;
    m_stats.DIPs++;

    m_lastResult = hr;
    return SUCCEEDED(hr);
}

// orig 0x644dc0 drawcalls.cpp:84
int CDevice::DrawPrimitive(PrimType PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount)
{
    setVertexShader(0);
    setPixelShader(0);
    ActuateStates(true);

    HRESULT hr = m_pd3dDevice->DrawPrimitive(m3dPtToD3dPt[PrimitiveType], StartVertex, PrimitiveCount);

    m_stats.polyCount += PrimitiveCount;
    m_stats.DPs++;

    m_lastResult = hr;
    return SUCCEEDED(hr);
}

// orig 0x644f70 drawcalls.cpp:119
int CDevice::DrawIndexedPrimitiveEffect(PrimType Type, IEffect* effect, unsigned int MinIndex,
                                        unsigned int NumVertices, unsigned int StartIndex,
                                        unsigned int PrimitiveCount)
{
    if (effect && effect->IsValid())
    {
        if (g_kernel->GetEngineCfg().m_r_renderToNull.GetB())
        {
            return 1;
        }

        ActuateStates(false);

        EffectImpl* effectImpl = static_cast<EffectImpl*>(effect);
        int numPasses = effectImpl->Begin();
        for (int pass = 0; pass < numPasses; pass++)
        {
            effectImpl->BeginPass(pass);

            m_curIbBaseIdx = m_latchedIbBaseIdx;
            m_pd3dDevice->DrawIndexedPrimitive(m3dPtToD3dPt[Type], m_latchedIbBaseIdx, MinIndex, NumVertices,
                                               StartIndex, PrimitiveCount);

            m_stats.polyCount += PrimitiveCount;
            effectImpl->m_numPrimitives += PrimitiveCount;

            effectImpl->EndPass();
        }
        effectImpl->End();

        m_stats.DIPs += numPasses;
        effectImpl->m_numDIPs += numPasses;

        return 1;
    }

    return DrawIndexedPrimitive(Type, MinIndex, NumVertices, StartIndex, PrimitiveCount);
}

// orig 0x645090 drawcalls.cpp:173
int CDevice::DrawPrimitiveEffect(PrimType PrimitiveType, IEffect* effect, unsigned int StartVertex,
                                 unsigned int PrimitiveCount)
{
    if (effect && effect->IsValid())
    {
        if (g_kernel->GetEngineCfg().m_r_renderToNull.GetB())
        {
            return 1;
        }

        ActuateStates(false);

        EffectImpl* effectImpl = static_cast<EffectImpl*>(effect);
        int numPasses = effectImpl->Begin();
        for (int pass = 0; pass < numPasses; pass++)
        {
            effectImpl->BeginPass(pass);

            m_pd3dDevice->DrawPrimitive(m3dPtToD3dPt[PrimitiveType], StartVertex, PrimitiveCount);

            m_stats.polyCount += PrimitiveCount;
            effectImpl->m_numPrimitives += PrimitiveCount;

            effectImpl->EndPass();
        }
        effectImpl->End();

        m_stats.DIPs += numPasses;
        effectImpl->m_numDIPs += numPasses;

        return 1;
    }

    return DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

// orig 0x644e30 drawcalls.cpp:230
int CDevice::DrawIndexedPrimitiveShader(PrimType Type, unsigned int MinIndex, unsigned int NumVertices,
                                        unsigned int StartIndex, unsigned int PrimitiveCount)
{
    ActuateStates(false);

    m_curIbBaseIdx = m_latchedIbBaseIdx;
    HRESULT hr = m_pd3dDevice->DrawIndexedPrimitive(m3dPtToD3dPt[Type], m_latchedIbBaseIdx, MinIndex, NumVertices,
                                                    StartIndex, PrimitiveCount);

    m_stats.polyCount += PrimitiveCount;
    m_stats.DIPs++;

    m_lastResult = hr;
    return SUCCEEDED(hr);
}

// orig 0x644ea0 drawcalls.cpp:263
int CDevice::DrawPrimitiveShader(PrimType PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount)
{
    return 0;
}

// orig 0x644eb0 drawcalls.cpp:272
int CDevice::BeginScene()
{
    m_lastResult = m_pd3dDevice->BeginScene();
    m_inScene = SUCCEEDED(m_lastResult);
    return InScene();
}

// orig 0x644ee0 drawcalls.cpp:283
int CDevice::EndScene()
{
    DrawFsRt();

    m_lastResult = m_pd3dDevice->EndScene();
    m_inScene = FAILED(m_lastResult);
    return !InScene();
}

// orig 0x644f20 drawcalls.cpp:296
int CDevice::PresentScene()
{
    m_presents++;
    m_viewMatrixWasSetThisFrame = false;
    m_lastResult = m_pd3dDevice->Present(0, 0, 0, 0);
    return SUCCEEDED(m_lastResult);
}

// orig 0x644f60 drawcalls.cpp:307
int CDevice::InScene()
{
    return m_inScene;
}

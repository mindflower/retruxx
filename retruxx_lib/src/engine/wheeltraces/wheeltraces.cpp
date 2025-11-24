#include "wheeltraces.h"
#include <stdexcept>

#include "m3dapp.h"
#include "world.h"
#include "core/kernel.h"
#include <client.h>

namespace m3d
{
    SkidStrip::SkidStrip()
    {
        this->m_lastFramestamp = 0;
        this->m_binUse = 0;
        this->m_timeStamp = 0;
        this->m_soilType = 0;
        this->m_stripSize = 0;
        this->m_texCoord = 0.0;
        this->m_boundCenter = ZeroVector;
        this->m_boundRadius = 0.0;
    }

    void WheelTraceMgr::Release()
    {
        for (int i = 0; i < 512; ++i)
        {
            m_skidStrips[i] = {};
        }

        for (auto& handle : m_texHandles)
        {
            M3D_RENDERER->ReleaseTexture(handle);
        }

        m_texHandles.clear();
    }

    void WheelTraceMgr::Render()
    {
        // TODO: generated code WheelTraceMgr::Render
        m_profiler->StartCountdown();

        // Save render states
        M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
        M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->PushFog(true);

        // Configure render states for wheel traces
        M3D_RENDERER->TgDisable(0);
        M3D_RENDERER->TgSetTcSource(1, rend::TC_FROM_VERTEX, 0);
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE2X);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        M3D_RENDERER->SetAlphaTest(1);
        M3D_RENDERER->DisableTextureStages(1);

        // Set vertex buffer
        M3D_RENDERER->SetToStream0(m_vb);

        auto* viewFrustum = &m3d::pClient->GetWorld().GetLandscape().m_frustumCull;
        uint32_t baseIndex = 0;

        // Render all skid strips
        for (int i = 0; i < 512; ++i)
        {
            SkidStrip& strip = m_skidStrips[i];

            // Skip if strip has insufficient vertices or is outside view frustum
            if (strip.m_stripSize >= 2 && viewFrustum->testSphere(strip.m_boundCenter, strip.m_boundRadius))
            {
                // Set appropriate texture based on soil type
                if (strip.m_soilType >= 0 && strip.m_soilType < static_cast<int>(m_texHandles.size()))
                {
                    M3D_RENDERER->SetTexture(0, m_texHandles[strip.m_soilType], -1.0f);
                }
                else
                {
                    // Use white texture as fallback
                    if (!m_texHandles.empty())
                    {
                        M3D_RENDERER->SetTexture(0, m_texHandles.back(), -1.0f);
                    }
                    else
                    {
                        M3D_RENDERER->SetWhiteTexture(0);
                    }
                }

                // Render the strip
                uint32_t vertexCount = 2 * strip.m_stripSize;
                M3D_RENDERER->SetIndices(m_ib, baseIndex);
                M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLESTRIP, 0, vertexCount, 0, vertexCount - 2);
            }

            baseIndex += 128;
        }

        // Restore render states
        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopZbState();

        m_profiler->EndCountdown();
    }

    WheelTraceMgr::~WheelTraceMgr()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WheelTraceMgr::EndSkidding(void*, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WheelTraceMgr::StartSkidding(void*, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WheelTraceMgr::ClearTraces()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WheelTraceMgr::WheelTraceMgr()
    {
        //TODO: check this
        m_skidStrips = new SkidStrip[512];
        this->m_vb = M3D_RENDERER->AddVb(
            rend::VERTEX_XYZCT1,
            0x10000,
            "WheelTrace",
            0);
        this->m_ib = M3D_RENDERER->AddIb(130, 0);
        //TODO: check this
        auto ibPtr = static_cast<WORD*>(M3D_RENDERER->LockIb(this->m_ib, 0, 0, 0));
        for (int i = 0; i < 130; ++i)
            ibPtr[i] = i;
        M3D_RENDERER->UnlockIb(this->m_ib);
        this->m_shader = M3D_RENDERER->NewEffect("data/shaders/wheeltrace.fx", true);
        M3D_ASSERT(m_shader);
        this->m_shader->SetDefaultTechnique(true);
        m_profiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(m3d::Application::g_pApp->GetProfilerStack().AddProfiler("wheeltraces", 0x1Eu));
    }

    void WheelTraceMgr::Init(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool WheelTraceMgr::IsSkiddingStarted(void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WheelTraceMgr::AddTrace(CVector const&, Quaternion const&, float, void*, int, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WheelTraceMgr::AddTextureBySoilType(int, CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

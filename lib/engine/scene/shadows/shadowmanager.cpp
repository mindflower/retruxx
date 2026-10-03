#include "shadowmanager.h"
#include "shadowcaster.h"

#include <cmath>
#include <m3dapp.h>
#include <config.h>
#include <core/kernel.h>
#include <core/profilerstack.h>
#include <core/debugcounter.h>

namespace m3d
{
    namespace
    {
        constexpr unsigned int MAX_VOLUMES = 100;
        // The vertex pool holds 100000 vertices; the volumes stop being built once 85000 are used.
        constexpr unsigned int MAX_POOL_VERTS = 0x14C08;

        Profiler* AddShadowProfiler(char const* name)
        {
            auto& profilers = M3D_APP->GetProfilerStack();
            unsigned int const id = profilers.AddProfiler(name, 0x1E);
            return id < profilers.GetNumProfilers() ? profilers.GetProfiler(id) : nullptr;
        }
    }  // namespace

    ShadowManager::ShadowManager()
    {
        // RVA 0x8F05E0
        m_bIsShadowsEnabled = false;
        m_shadowColor = 0x77000000;
        m_bInScene = false;
        m_numVolumes = 0;
        m_edgesBuf = new unsigned short[10000];
        m_volumeVertexPool = new CVector[100000];
        for (auto& volume : m_volumes)
        {
            volume = new ShadowVolume();
        }
        m_profiler1 = AddShadowProfiler("sv1");
        m_profiler2 = AddShadowProfiler("sv2");
    }

    ShadowManager::~ShadowManager()
    {
        // RVA 0x8EFFF0
        for (auto* volume : m_volumes)
        {
            delete volume;
        }
        delete[] m_edgesBuf;
        m_edgesBuf = nullptr;
        delete[] m_volumeVertexPool;
        m_volumeVertexPool = nullptr;
        if (m_fsQuadVb.IsValid())
        {
            M3D_RENDERER->ReleaseVb(m_fsQuadVb);
        }
    }

    void ShadowManager::EnableShadows(bool bEnable)
    {
        // RVA 0x8EFF40
        m_bIsShadowsEnabled = bEnable;
    }

    bool ShadowManager::ShadowsEnabled() const
    {
        // RVA 0x8EFF50
        return m_bIsShadowsEnabled;
    }

    void ShadowManager::SetShadowColor(unsigned int color)
    {
        // RVA 0x8EFF60
        m_shadowColor = color;
    }

    unsigned int ShadowManager::GetShadowColor() const
    {
        // RVA 0x8EFF70
        return m_shadowColor;
    }

    void ShadowManager::AddDirectionalLight(const CVector& lightDir)
    {
        // RVA 0x8EFF80
        m_lightDir = lightDir;
    }

    void ShadowManager::AddShadowCaster(const CMatrix& objToWorldMat, const AnimatedModel::Mesh& caster)
    {
        // RVA 0x8F0090 - the light direction is taken into the caster's own space.
        m_profiler1->StartCountdown();
        if (m_numVolumes < MAX_VOLUMES)
        {
            m_volumes[m_numVolumes]->Invalidate();
            m_volumes[m_numVolumes]->Initialize(caster, m_edgesBuf);
            CMatrix const& m = objToWorldMat;
            float const x = m._12 * m_lightDir.y + m._13 * m_lightDir.z + m._11 * m_lightDir.x;
            float const y = m._22 * m_lightDir.y + m._21 * m_lightDir.x + m._23 * m_lightDir.z;
            float const z = m._32 * m_lightDir.y + m._31 * m_lightDir.x + m._33 * m_lightDir.z;
            float const invLen = 1.0f / std::sqrt(z * z + y * y + x * x + 0.00000011920929f);
            m_objLightDir[m_numVolumes] = CVector(invLen * x, invLen * y, invLen * z);
            m_objToWorldMatrices[m_numVolumes] = objToWorldMat;
            ++m_numVolumes;
        }
        m_profiler1->EndCountdown();
    }

    void ShadowManager::BeginScene()
    {
        // RVA 0x8F06E0 - builds the volumes and counts them into the stencil buffer (depth-pass: +1 on front faces,
        // -1 on back faces), with two-sided stencil when the card has it.
        if (!m_bIsShadowsEnabled)
        {
            return;
        }

        m_volumeVertexPoolSize = 0;
        m_bInScene = true;
        m_profiler1->StartCountdown();
        unsigned int numBuilt = 0;
        for (; numBuilt < m_numVolumes; ++numBuilt)
        {
            m_volumeVertexPoolSize += m_volumes[numBuilt]->BuildShadowVolume(
                m_objLightDir[numBuilt], &m_volumeVertexPool[m_volumeVertexPoolSize]);
            // NOTE: the volume that overflows the pool is kept in it but not rendered.
            if (m_volumeVertexPoolSize > MAX_POOL_VERTS)
            {
                break;
            }
        }
        m_profiler1->EndCountdown();

        m_profiler2->StartCountdown();
        unsigned int numTris = 0;
        auto* const renderer = M3D_RENDERER;
        renderer->SetZbState(rend::ZB_NOWRITE, false);
        renderer->SetStencilState(true, false);
        renderer->PushShadeMode(rend::M3DSHADE_FLAT);
        renderer->SetStencilFunc(rend::M3DCMP_ALWAYS, false);
        renderer->SetStencilPass(rend::OP_INCR, false);
        renderer->SetColorWriteMask(0, false);
        renderer->SetAlphaTest(0);
        renderer->MatPushWorld();
        if (!M3D_ENGINE_CFG.m_g_stencilShadowsDebug.GetI())
        {
            if (renderer->IsFeatureSupported(rend::FEATURE_2SIDED_STENCIL) && M3D_ENGINE_CFG.m_g_twoSidedStencil.GetB())
            {
                renderer->SetStencil2SidedEnable(true, false);
                renderer->SetStencilCcwFunc(rend::M3DCMP_ALWAYS, false);
                renderer->SetStencilCcwPass(rend::OP_DECR, false);
                renderer->SetCull(rend::M3DCULL_NONE, false);
                for (unsigned int i = 0; i < numBuilt; ++i)
                {
                    m_volumes[i]->Render(m_objToWorldMatrices[i], 0x55FF0000);
                }
                renderer->SetStencil2SidedEnable(false, false);
                numTris = m_volumeVertexPoolSize / 3;
            }
            else
            {
                renderer->SetCull(rend::M3DCULL_CCW, false);
                for (unsigned int i = 0; i < numBuilt; ++i)
                {
                    m_volumes[i]->Render(m_objToWorldMatrices[i], 0x55FF0000);
                }
                renderer->SetCull(rend::M3DCULL_CW, false);
                renderer->SetStencilPass(rend::OP_DECR, false);
                for (unsigned int i = 0; i < numBuilt; ++i)
                {
                    m_volumes[i]->Render(m_objToWorldMatrices[i], 0x55FF0000);
                }
                numTris = static_cast<unsigned int>((0x155555556ull * m_volumeVertexPoolSize) >> 33);
            }
        }
        renderer->MatPopWorld();
        renderer->PopShadeMode();
        renderer->SetCull(rend::M3DCULL_CCW, false);
        renderer->SetZbState(rend::ZB_ENABLE, false);
        renderer->SetColorWriteMask(15, false);
        renderer->SetStencilState(false, false);

        auto& counters = M3D_APP->GetDbgCounterStack();
        counters.DrawStringThisFrame((CStr("# of sv = ") + CStr(numBuilt)).c_str());
        counters.DrawStringThisFrame((CStr("tris in svs = ") + CStr(numTris)).c_str());
        m_profiler2->EndCountdown();
    }

    void ShadowManager::EndScene()
    {
        // RVA 0x8F0420 - darkens every pixel whose stencil count is not zero.
        m_profiler2->StartCountdown();
        if (m_bIsShadowsEnabled)
        {
            m_bInScene = false;
            if (!M3D_ENGINE_CFG.m_g_stencilShadowsDebug.GetI())
            {
                auto* const renderer = M3D_RENDERER;
                renderer->PushZbState(rend::ZB_DISABLE);
                renderer->PushFog(false);
                renderer->PushBlend(rend::BM_ALPHA);
                renderer->SetStencilState(true, false);
                renderer->SetStencilRef(0, false);
                renderer->SetStencilFunc(rend::M3DCMP_NOTEQUAL, false);
                rend::TexHandle noTexture;
                renderer->SetTexture(0, noTexture, -1.0);
                renderer->SetToStream0(m_fsQuadVb);
                renderer->DrawPrimitive(rend::M3DPT_TRIANGLESTRIP, 0, 2);
                renderer->PopZbState();
                renderer->SetStencilState(false, false);
                renderer->PopFog();
                renderer->PopBlend();
            }
            m_numVolumes = 0;
        }
        m_profiler2->EndCountdown();
    }

    void ShadowManager::OnChangeScreenResolution()
    {
        // RVA 0x8F0250 - a pre-transformed quad over the whole 1024x768 virtual screen carrying the shadow alpha.
        auto* const renderer = M3D_RENDERER;
        if (m_fsQuadVb.IsValid())
        {
            renderer->ReleaseVb(m_fsQuadVb);
        }
        m_fsQuadVb = renderer->AddVb(rend::VERTEX_XYZWC, 4, CStr("Shadow"), 0);

        float x = 0.0f;
        float y = 0.0f;
        float w = 1024.0f;
        float h = 768.0f;
        renderer->RelToAbs(x, y);
        renderer->RelToAbs(w, h);

        struct VertexXYZWC
        {
            float x, y, z, w;
            unsigned int color;
        };
        auto* const v = static_cast<VertexXYZWC*>(renderer->LockVb(m_fsQuadVb, 0, 0, 0));
        unsigned int const color = (m_shadowColor >> 24) << 24;
        v[0] = {x, h + y, 0.0f, 0.1f, color};
        v[1] = {x, y, 0.0f, 0.1f, color};
        v[2] = {w + x, h + y, 0.0f, 0.1f, color};
        v[3] = {w + x, y, 0.0f, 0.1f, color};
        renderer->UnlockVb(m_fsQuadVb);
    }
}

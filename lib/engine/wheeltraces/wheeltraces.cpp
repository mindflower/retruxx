#include "wheeltraces.h"

#include "config.h"

#include <stdexcept>

#include "m3dapp.h"
#include "world.h"
#include "core/kernel.h"
#include "core/timer.h"

#include <client.h>

namespace m3d
{
    namespace
    {

        void FillTraceBuffer(m3d::rend::VertexXYZCT1* write, m3d::SkidQuad const& sq, unsigned int opaque, float texLength)
        {
            // RVA 0x697BE0 - one cross section of a strip: two vertices across the wheel.
            write->x = sq.p1.x;
            write->y = sq.p1.y;
            write->z = sq.p1.z;
            write[1].x = sq.p2.x;
            write[1].y = sq.p2.y;
            write[1].z = sq.p2.z;
            write->tu = 0.0;
            write->tv = texLength;
            write[1].tu = 1.0;
            write[1].tv = texLength;
            write->c = opaque;
            write[1].c = opaque;
        }

        void EmbraceSphere(CVector& org, float& rad, CVector const& newPoint)
        {
            // RVA 0x697A70 - grows the sphere just enough to take in newPoint: the new
            // sphere spans from the far side of the old one to the point.
            CVector const away(org.x - newPoint.x, org.y - newPoint.y, org.z - newPoint.z);
            float const distSq = away.z * away.z + away.y * away.y + away.x * away.x;
            if (rad * rad > distSq)
            {
                return;
            }

            // The epsilon is FLT_EPSILON, so a point at the centre does not divide by zero.
            float const invDist = static_cast<float>(1.0 / sqrt(distSq + 1.1920929e-7));
            CVector const farSide(
                org.x + invDist * away.x * rad, org.y + away.y * invDist * rad, away.z * invDist * rad + org.z);

            org.x = (newPoint.x + farSide.x) * 0.5f;
            org.y = (farSide.y + newPoint.y) * 0.5f;
            org.z = (newPoint.z + farSide.z) * 0.5f;

            CVector const diameter(newPoint.x - farSide.x, newPoint.y - farSide.y, newPoint.z - farSide.z);
            // Summed on the x87 stack in the original, hence the doubles.
            double const diameterSq = static_cast<double>(diameter.z) * diameter.z +
                static_cast<double>(diameter.y) * diameter.y + static_cast<double>(diameter.x) * diameter.x;
            rad = static_cast<float>(sqrt(diameterSq) * 0.5);
        }
    }


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
        // RVA 0x698730 - every strip owns 64 cross sections (128 vertices) of the vertex
        // buffer and is drawn as one triangle strip through the shared 0..129 index buffer,
        // rebased onto the strip's first vertex.
        m_profiler->StartCountdown();

        M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
        M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
        M3D_RENDERER->TgDisable(0);
        M3D_RENDERER->TgSetTcSource(1, rend::TC_FROM_VERTEX, 0);
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE2X);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->SetAlphaTest(1);
        M3D_RENDERER->DisableTextureStages(1);
        M3D_RENDERER->PushFog(true);
        M3D_RENDERER->SetHandleToStream0(m_vb);

        CClipper const& frustum = m3d::pClient->GetWorld().GetLandscape().m_frustumCull;
        int baseVertex = 0;
        for (int i = 0; i < 512; ++i, baseVertex += 128)
        {
            SkidStrip const& strip = m_skidStrips[i];
            if (strip.m_stripSize < 2 || !frustum.testSphere(strip.m_boundCenter, strip.m_boundRadius))
            {
                continue;
            }

            // An unknown soil type falls back to the last texture, or to white when there are none.
            if (strip.m_soilType >= 0 && strip.m_soilType < static_cast<int>(m_texHandles.size()))
            {
                M3D_RENDERER->SetTexture(0, m_texHandles[strip.m_soilType], -1.0);
            }
            else if (!m_texHandles.empty())
            {
                M3D_RENDERER->SetTexture(0, m_texHandles.back(), -1.0);
            }
            else
            {
                M3D_RENDERER->SetWhiteTexture(0);
            }

            unsigned const numVertices = 2 * strip.m_stripSize;
            M3D_RENDERER->SetHandleIndices(m_ib, baseVertex);
            M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLESTRIP, 0, numVertices, 0, numVertices - 2);
        }

        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopZbState();

        m_profiler->EndCountdown();
    }

    WheelTraceMgr::~WheelTraceMgr()
    {
        // RVA 0x699D60
        Release();
        M3D_RENDERER->ReleaseVb(m_vb);
        M3D_RENDERER->ReleaseIb(m_ib);
        delete[] m_skidStrips;
        m_skidStrips = nullptr;
        if (m_shader)
        {
            m_shader->Release();
            m_shader = nullptr;
        }
    }

    int WheelTraceMgr::EndSkidding(void* owner, bool smoothEnd)
    {
        // RVA 0x698630 - frees the owner's strip. The strip keeps its geometry and is only
        // recycled by StartSkidding, oldest first, by the time stamp set here.
        auto const it = m_ownersToIdxMap.find(owner);
        if (it == m_ownersToIdxMap.end())
        {
            return 0;
        }

        int const stripIdx = it->second;
        SkidStrip& strip = m_skidStrips[stripIdx];
        strip.m_binUse = false;
        m_ownersToIdxMap.erase(it);
        strip.m_timeStamp = g_Kernel->GetTimer().GetCurTime();

        if (smoothEnd && strip.m_stripSize > 0)
        {
            // Fades the trace out: the last cross section becomes fully transparent.
            auto* vertices = static_cast<rend::VertexXYZCT1*>(
                M3D_RENDERER->LockVb(m_vb, 2, 2 * (stripIdx * 64 + strip.m_stripSize - 1), 0));
            vertices[0].c = 0x7F7F7F;
            vertices[1].c = 0x7F7F7F;
            M3D_RENDERER->UnlockVb(m_vb);
        }

        return 1;
    }

    int WheelTraceMgr::StartSkidding(void* owner, int type)
    {
        // RVA 0x698BB0 - takes the free strip that was released longest ago.
        // NOTE: when all 512 strips are in use the last one is taken anyway, from under its
        // owner. And if the owner already has a strip, the map keeps the old index while the
        // strip returned here is reset regardless.
        int stripIdx = 511;
        int oldestTimeStamp = 0x7FFFFFFF;
        for (int i = 0; i < 512; ++i)
        {
            if (!m_skidStrips[i].m_binUse && m_skidStrips[i].m_timeStamp < oldestTimeStamp)
            {
                stripIdx = i;
                oldestTimeStamp = m_skidStrips[i].m_timeStamp;
            }
        }

        m_ownersToIdxMap.insert({owner, stripIdx});

        SkidStrip& strip = m_skidStrips[stripIdx];
        strip.m_binUse = true;
        strip.m_texCoord = 0.0f;
        strip.m_stripSize = 0;
        strip.m_lastFramestamp = g_Kernel->GetTimer().GetCurFrame();
        strip.m_soilType = type;
        strip.m_boundCenter = ZeroVector;
        strip.m_boundRadius = 0.0f;
        return stripIdx;
    }

    void WheelTraceMgr::ClearTraces()
    {
        // RVA 0x697370 - empties every strip; the soil types and last quads are kept.
        for (int i = 0; i < 512; ++i)
        {
            SkidStrip& strip = m_skidStrips[i];
            strip.m_binUse = false;
            strip.m_texCoord = 0.0f;
            strip.m_lastFramestamp = 0;
            strip.m_stripSize = 0;
            strip.m_timeStamp = 0;
            strip.m_boundCenter = ZeroVector;
            strip.m_boundRadius = 0.0f;
        }
    }

    WheelTraceMgr::WheelTraceMgr()
    {
        // RVA 0x699670 - 512 strips of up to 64 cross sections each. The index buffer is
        // just 0..129, shared by every strip through the base vertex of SetIndices.
        m_skidStrips = new SkidStrip[512];
        m_vb = M3D_RENDERER->AddVb(rend::VERTEX_XYZCT1, 0x10000, "WheelTrace", 0);
        m_ib = M3D_RENDERER->AddIb(130, 0);

        auto* indices = static_cast<uint16_t*>(M3D_RENDERER->LockIb(m_ib, 0, 0, 0));
        for (int i = 0; i < 130; ++i)
        {
            indices[i] = static_cast<uint16_t>(i);
        }
        M3D_RENDERER->UnlockIb(m_ib);

        m_shader = M3D_RENDERER->NewEffect("data/shaders/wheeltrace.fx", true);
        M3D_ASSERT(m_shader);
        m_shader->SetDefaultTechnique(true);

        ProfilerStack& profilers = m3d::Application::g_pApp->GetProfilerStack();
        m_profiler = profilers.GetProfiler(profilers.AddProfiler("wheeltraces", 0x1Eu));
    }

    void WheelTraceMgr::Init(int numSoilTypes)
    {
        m_texHandles.resize(numSoilTypes);
        for (size_t i = 0; i < 512; ++i)
            m_skidStrips[i].m_stripSize = 0;
    }

    bool WheelTraceMgr::IsSkiddingStarted(void* owner)
    {
        return m_ownersToIdxMap.find(owner) != m_ownersToIdxMap.end();
    }

    void WheelTraceMgr::AddTrace(CVector const& org, Quaternion const& quat, float scale, void* owner, int soilType, bool smoothStart)
    {
        // RVA 0x698CE0 - appends a cross section at the wheel's contact point. A new
        // section is only started once the wheel is skidMinDist past the one before the
        // last; until then the last section is moved along with the wheel.
        for (;;)
        {
            auto const it = m_ownersToIdxMap.find(owner);
            if (it == m_ownersToIdxMap.end())
            {
                return;
            }

            int const stripIdx = it->second;
            SkidStrip& strip = m_skidStrips[stripIdx];

            // A skipped frame or a change of soil ends the trace.
            unsigned const curFrame = g_Kernel->GetTimer().GetCurFrame();
            if (static_cast<int>(curFrame - strip.m_lastFramestamp) > 1 || strip.m_soilType != soilType)
            {
                EndSkidding(owner, true);
                return;
            }
            strip.m_lastFramestamp = curFrame;

            // The section runs across the wheel along its local x axis, a third of the
            // wheel's scale to each side, at the height of the contact point.
            float const halfWidth = scale * 0.33333334f;
            CMatrix const rot = quat.ToMatrix();
            SkidQuad sq;
            sq.p1.x = org.x + (rot._11 * halfWidth + rot._31 * 0.0f + rot._21 * 0.0f);
            sq.p1.y = org.y;
            sq.p1.z = (rot._13 * halfWidth + rot._33 * 0.0f + rot._23 * 0.0f) + org.z;
            sq.p2.x = org.x + ((0.0f - halfWidth) * rot._11 + rot._31 * 0.0f + rot._21 * 0.0f);
            sq.p2.y = org.y;
            sq.p2.z = (rot._13 * (0.0f - halfWidth) + rot._33 * 0.0f + rot._23 * 0.0f) + org.z;

            unsigned const opaque = 0x7F7F7F | (M3D_ENGINE_CFG.m_skidOpaque.GetI() << 24);

            if (strip.m_stripSize >= 63)
            {
                // The strip is full: the trace goes on in a fresh strip that starts with
                // the full strip's last section, and this section is added on the next pass.
                EndSkidding(owner, false);
                int const newIdx = StartSkidding(owner, strip.m_soilType);
                SkidStrip& newStrip = m_skidStrips[newIdx];
                SkidQuad const& lastQuad = strip.m_last[0];

                auto* vertices = static_cast<rend::VertexXYZCT1*>(
                    M3D_RENDERER->LockVb(m_vb, 2, 2 * (newIdx * 64 + newStrip.m_stripSize), 0));
                FillTraceBuffer(vertices, lastQuad, opaque, 0.0f);
                M3D_RENDERER->UnlockVb(m_vb);

                newStrip.m_last[1] = newStrip.m_last[0];
                newStrip.m_last[0] = lastQuad;
                ++newStrip.m_stripSize;
                newStrip.m_boundCenter = lastQuad.p1;
                newStrip.m_boundRadius = 0.0f;
                smoothStart = false;
                continue;
            }

            if (strip.m_stripSize <= 1)
            {
                // A smooth start fades the trace in from a transparent first section.
                auto* vertices = static_cast<rend::VertexXYZCT1*>(
                    M3D_RENDERER->LockVb(m_vb, 2, 2 * (stripIdx * 64 + strip.m_stripSize), 0));
                FillTraceBuffer(vertices, sq, smoothStart ? 0x7F7F7F : opaque, 0.0f);
                M3D_RENDERER->UnlockVb(m_vb);

                ++strip.m_stripSize;
                strip.m_last[1] = strip.m_last[0];
                strip.m_last[0] = sq;
                strip.m_boundCenter = org;
                strip.m_boundRadius = 0.0f;
                return;
            }

            CVector const beforeLastMid = (strip.m_last[1].p1 + strip.m_last[1].p2) * 0.5f;
            CVector const lastMid = (strip.m_last[0].p1 + strip.m_last[0].p2) * 0.5f;
            CVector const mid = (sq.p1 + sq.p2) * 0.5f;
            double const minDist = M3D_ENGINE_CFG.m_skidMinDist.GetF();

            // The texture repeats once per half width of wheel travelled.
            strip.m_texCoord = static_cast<float>(static_cast<double>((lastMid - mid).length()) / halfWidth + strip.m_texCoord);

            int const nextVertex = 2 * (stripIdx * 64 + strip.m_stripSize);
            if ((beforeLastMid - mid).lengthSq() <= minDist * minDist)
            {
                // Too close to the section before: move the last section here instead.
                auto* vertices = static_cast<rend::VertexXYZCT1*>(M3D_RENDERER->LockVb(m_vb, 2, nextVertex - 2, 0));
                FillTraceBuffer(vertices, sq, opaque, strip.m_texCoord);
                M3D_RENDERER->UnlockVb(m_vb);
            }
            else
            {
                auto* vertices = static_cast<rend::VertexXYZCT1*>(M3D_RENDERER->LockVb(m_vb, 2, nextVertex, 0));
                FillTraceBuffer(vertices, sq, opaque, strip.m_texCoord);
                M3D_RENDERER->UnlockVb(m_vb);

                ++strip.m_stripSize;
                strip.m_last[1] = strip.m_last[0];
            }
            strip.m_last[0] = sq;

            EmbraceSphere(strip.m_boundCenter, strip.m_boundRadius, org);
            return;
        }
    }

    void WheelTraceMgr::AddTextureBySoilType(int soilType, CStr const& textureName)
    {
        if (soilType < m_texHandles.size())
        {
            auto& texHandle = m_texHandles[soilType];
            if (!texHandle.IsValid())
            {
                texHandle = M3D_RENDERER->AddTexture(textureName, 2);
                M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_S, 1u);
                M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_T, 1u);
            }
        }
    }
}

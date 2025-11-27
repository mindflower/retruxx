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
            // TODO: check and refactor this
            auto v4 = org.y - newPoint.y;
            auto v5 = org.z - newPoint.z;
            auto v6 = org.x - newPoint.x;
            if ((float)(rad * rad) <= (float)((float)((float)(v5 * v5) + (float)(v4 * v4)) + (float)(v6 * v6)))
            {
                auto newPointa = 1.0 / sqrt((float)((float)((float)(v5 * v5) + (float)(v4 * v4)) + (float)(v6 * v6)) + 0.00000011920929);
                auto v7 = org.x + (float)((float)(newPointa * v6) * rad);
                auto v8 = org.y + (float)((float)(v4 * newPointa) * rad);
                auto v9 = (float)((float)(v5 * newPointa) * rad) + org.z;
                auto z = newPoint.z;
                auto v11 = v8 + newPoint.y;
                org.x = (float)(newPoint.x + v7) * 0.5;
                org.y = v11 * 0.5;
                org.z = (float)(z + v9) * 0.5;
                rad = sqrt(
                          (float)(newPoint.z - v9) * (float)(newPoint.z - v9) + (float)(newPoint.y - v8) * (float)(newPoint.y - v8) +
                          (float)(newPoint.x - v7) * (float)(newPoint.x - v7)) *
                    0.5;
            }
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

    int WheelTraceMgr::EndSkidding(void* owner, bool smoothEnd)
    {
        // TODO: generated code WheelTraceMgr::EndSkidding
        // Find the skid strip for this owner
        auto it = m_ownersToIdxMap.find(owner);
        if (it == m_ownersToIdxMap.end())
            return 0;

        int stripIndex = it->second;
        SkidStrip* strip = &m_skidStrips[stripIndex];

        // Mark the strip as unused and remove from owner mapping
        strip->m_binUse = 0;
        m_ownersToIdxMap.erase(it);

        // Update timestamp
        unsigned int currentTime = g_Kernel->GetTimer().GetCurTime();
        strip->m_timeStamp = currentTime;

        // Handle smooth ending by updating vertex colors
        if (smoothEnd)
        {
            int stripSize = strip->m_stripSize;
            if (stripSize > 0)
            {
                // Calculate vertex buffer offset
                int vertexOffset = 2 * (stripSize - 1 + (stripIndex << 6));

                // Lock vertex buffer
                auto* vertices = static_cast<m3d::rend::VertexXYZCT1*>(M3D_RENDERER->LockVb(m_vb, 2, vertexOffset, 0));

                // Update colors for smooth ending (fade out effect)
                vertices[0].c = 0x7F7F7F;  // First vertex color
                vertices[1].c = 0x7F7F7F;  // Second vertex color

                // Unlock vertex buffer
                M3D_RENDERER->UnlockVb(m_vb);
            }
        }

        return 1;
    }

    int WheelTraceMgr::StartSkidding(void* owner, int type)
    {
        // TODO: generated code WheelTraceMgr::StartSkidding
        int bestIndex = 511;               // Default to invalid index
        int oldestTimestamp = 0x7FFFFFFF;  // MAX_INT

        for (int i = 0; i < 512; ++i)
        {
            if (!m_skidStrips[i].m_binUse && m_skidStrips[i].m_timeStamp < oldestTimestamp)
            {
                bestIndex = i;
                oldestTimestamp = m_skidStrips[i].m_timeStamp;
            }
        }

        // Insert owner-to-index mapping
        std::pair<void*, int> newEntry(owner, bestIndex);
        auto result = m_ownersToIdxMap.insert(newEntry);

        // Initialize the skid strip
        int usedIndex = bestIndex;
        m_skidStrips[usedIndex].m_binUse = true;
        m_skidStrips[usedIndex].m_texCoord = 0.0f;
        m_skidStrips[usedIndex].m_stripSize = 0;
        m_skidStrips[usedIndex].m_lastFramestamp = g_Kernel->GetTimer().GetCurFrame();
        m_skidStrips[usedIndex].m_soilType = type;
        m_skidStrips[usedIndex].m_boundCenter = ZeroVector;
        m_skidStrips[usedIndex].m_boundRadius = 0.0f;

        return bestIndex;
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

    bool WheelTraceMgr::IsSkiddingStarted(void* owner)
    {
        return m_ownersToIdxMap.find(owner) != m_ownersToIdxMap.end();
    }

    void WheelTraceMgr::AddTrace(CVector const& org, Quaternion const& quat, float scale, void* owner, int soilType, bool smoothStart)
    {
        // TODO: generated code WheelTraceMgr::AddTrace 
        // Find existing skid strip for this owner
        auto it = m_ownersToIdxMap.find(owner);
        if (it == m_ownersToIdxMap.end())
            return;

        int stripIndex = it->second;
        SkidStrip* strip = &m_skidStrips[stripIndex];

        // Check if we should end skidding (frame gap or soil type change)
        unsigned int currentFrame = g_Kernel->GetTimer().GetCurFrame();
        if ((int)(currentFrame - strip->m_lastFramestamp) > 1 || strip->m_soilType != soilType)
        {
            EndSkidding(owner, true);
            return;
        }

        strip->m_lastFramestamp = currentFrame;

        // Calculate wheel width from scale
        float wheelWidth = scale * 0.33333334f;

        // Create rotation matrix from quaternion
        float xx = quat.x * quat.x;
        float xy = quat.x * quat.y;
        float xz = quat.x * quat.z;
        float xw = quat.x * quat.w;
        float yy = quat.y * quat.y;
        float yz = quat.y * quat.z;
        float yw = quat.y * quat.w;
        float zz = quat.z * quat.z;
        float zw = quat.z * quat.w;

        CMatrix rot;
        rot._11 = 1.0f - 2.0f * (yy + zz);
        rot._12 = 2.0f * (xy + zw);
        rot._13 = 2.0f * (xz - yw);
        rot._14 = 0.0f;
        rot._21 = 2.0f * (xy - zw);
        rot._22 = 1.0f - 2.0f * (xx + zz);
        rot._23 = 2.0f * (yz + xw);
        rot._24 = 0.0f;
        rot._31 = 2.0f * (xz + yw);
        rot._32 = 2.0f * (yz - xw);
        rot._33 = 1.0f - 2.0f * (xx + yy);
        rot._34 = 0.0f;
        rot._41 = 0.0f;
        rot._42 = 0.0f;
        rot._43 = 0.0f;
        rot._44 = 1.0f;

        // Calculate skid quad points
        SkidQuad sq;

        // First point (right side of wheel)
        sq.p1.x = org.x + (rot._11 * wheelWidth + rot._31 * 0.0f + rot._21 * 0.0f);
        sq.p1.y = org.y;
        sq.p1.z = org.z + (rot._13 * wheelWidth + rot._33 * 0.0f + rot._23 * 0.0f);

        // Second point (left side of wheel)
        sq.p2.x = org.x + (rot._11 * -wheelWidth + rot._31 * 0.0f + rot._21 * 0.0f);
        sq.p2.y = org.y;
        sq.p2.z = org.z + (rot._13 * -wheelWidth + rot._33 * 0.0f + rot._23 * 0.0f);

        // Get skid opacity from engine config
        unsigned int opaqueColor = 0x7F7F7F | (M3D_ENGINE_CFG.m_skidOpaque.GetI() << 24);

        // Handle strip capacity
        if (strip->m_stripSize >= 63)
        {
            // Strip is full, end current and start new one
            void* newOwner = owner;
            EndSkidding(newOwner, false);
            int newIndex = StartSkidding(newOwner, strip->m_soilType);
            SkidStrip* newStrip = &m_skidStrips[newIndex];

            // Initialize new strip with current quad
            int vertexOffset = 2 * (newStrip->m_stripSize + (newIndex << 6));
            auto* vertices = M3D_RENDERER->LockVb(m_vb, 2, vertexOffset, 0);
            FillTraceBuffer((rend::VertexXYZCT1*)vertices, sq, opaqueColor, 0.0f);
            M3D_RENDERER->UnlockVb(m_vb);

            newStrip->m_last[1] = newStrip->m_last[0];
            newStrip->m_last[0] = sq;
            newStrip->m_stripSize++;
            newStrip->m_boundCenter = CVector(sq.p1.x, sq.p1.y, sq.p1.z);
            newStrip->m_boundRadius = 0.0f;
            smoothStart = false;

            // Update iterator for the new strip
            it = m_ownersToIdxMap.find(owner);
            if (it == m_ownersToIdxMap.end())
                return;

            stripIndex = it->second;
            strip = &m_skidStrips[stripIndex];
        }

        // Add quad to strip based on strip size
        if (strip->m_stripSize <= 1)
        {
            // Add first quads to the strip
            int vertexOffset = 2 * (strip->m_stripSize + (stripIndex << 6));
            auto* vertices = M3D_RENDERER->LockVb(m_vb, 2, vertexOffset, 0);

            // TODO: this is wrong
            unsigned int color = owner ? opaqueColor : static_cast<unsigned int>(opaqueColor);
            FillTraceBuffer((rend::VertexXYZCT1*)vertices, sq, color, 0.0f);
            M3D_RENDERER->UnlockVb(m_vb);

            // Update strip data
            strip->m_last[1] = strip->m_last[0];
            strip->m_last[0] = sq;
            strip->m_stripSize++;

            strip->m_boundCenter = org;
            strip->m_boundRadius = 0.0f;
        }
        else
        {
            // Add subsequent quad with distance checking
            CVector prevMidpoint(
                (strip->m_last[1].p1.x + strip->m_last[1].p2.x) * 0.5f,
                (strip->m_last[1].p1.y + strip->m_last[1].p2.y) * 0.5f,
                (strip->m_last[1].p1.z + strip->m_last[1].p2.z) * 0.5f);

            CVector currentMidpoint(
                (strip->m_last[0].p1.x + strip->m_last[0].p2.x) * 0.5f,
                (strip->m_last[0].p1.y + strip->m_last[0].p2.y) * 0.5f,
                (strip->m_last[0].p1.z + strip->m_last[0].p2.z) * 0.5f);

            CVector newMidpoint((sq.p1.x + sq.p2.x) * 0.5f, (sq.p1.y + sq.p2.y) * 0.5f, (sq.p1.z + sq.p2.z) * 0.5f);

            // Calculate distances for texture coordinates
            CVector deltaCurrent = currentMidpoint - newMidpoint;
            strip->m_texCoord += deltaCurrent.length() / strip->m_texCoord;

            // Check minimum distance threshold
            float minDist = M3D_ENGINE_CFG.m_skidMinDist.GetF();
            CVector deltaPrev = prevMidpoint - newMidpoint;
            float prevDistSq = deltaPrev.lengthSq();

            if (prevDistSq <= minDist * minDist)
            {
                // Update existing vertices (close enough to previous point)
                int vertexOffset = 2 * (strip->m_stripSize + (stripIndex << 6)) - 2;
                auto* vertices = M3D_RENDERER->LockVb(m_vb, 2, vertexOffset, 0);
                FillTraceBuffer((rend::VertexXYZCT1*)vertices, sq, opaqueColor, strip->m_texCoord);
                M3D_RENDERER->UnlockVb(m_vb);
            }
            else
            {
                // Add new vertices
                int vertexOffset = 2 * (strip->m_stripSize + (stripIndex << 6));
                auto* vertices = M3D_RENDERER->LockVb(m_vb, 2, vertexOffset, 0);
                FillTraceBuffer((rend::VertexXYZCT1*)vertices, sq, opaqueColor, strip->m_texCoord);
                M3D_RENDERER->UnlockVb(m_vb);

                strip->m_stripSize++;
                strip->m_last[1] = strip->m_last[0];
            }

            // Update current quad
            strip->m_last[0] = sq;

            // Update bounding sphere
            EmbraceSphere(strip->m_boundCenter, strip->m_boundRadius, org);
        }
    }

    void WheelTraceMgr::AddTextureBySoilType(int, CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

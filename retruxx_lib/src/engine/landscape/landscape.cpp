#define NOMINMAX

#include "roads/road.h"
#include <geomobject.h>
#include <landscape.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/timer.h>
#include <core/ini.h>
#include <file/tagged.h>

#include "config.h"
#include "level.h"
#include "m3dapp.h"
#include "world.h"
#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "math/vector4.h"
#include "client.h"
#include <algorithm>
#include "scene/servers/dataserver.h"
#include "skelmodel.h"
#include "server/objects/base/physicobj.h"

#include <draftstructures.h>

#include "server/objects/physicbodies/physichelpers.h"
#include "server/objects/physicbodies/geoms/ray.h"
#include <server/objects/base/objcontainer.h>

extern "C" {
#include <ode/collision.h>
#include <ode/collision_trimesh.h>
}

namespace m3d
{
    extern CClient* pClient;

    m3d::RenderModes Landscape::m_renderMode = RM_GAME;

    struct AlphaSpecial
    {
        /* 0x0000 */ int m_maskindex;
        /* 0x0004 */ int m_rotate;
    }; /* size: 0x0008 */;

    AlphaSpecial specialMapper[16] = { {0, 0}, {3, 0}, {3, 1}, {1, 1}, {3, 3}, {1, 0}, {4, 1}, {2, 0}, {3, 2}, {4, 0}, {1, 2}, {2, 1}, {1, 3}, {2, 3}, {2, 2}, {0, 0} };

    RT_CLASS_EXPORTS_BEGIN(Landscape)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Landscape);

    void Landscape::LinkObstacleToCells(ai::Obstacle* obstacle)
    {
        // TODO: generated code
        // Get the obstacle's bounding box
        Aabb box = obstacle->GetAabb();

        const float VISCELL_EDGE_LENGTH_8 = 128.0;

        // Convert world coordinates to grid coordinates
        const float cellSizeInv = 1.0f / VISCELL_EDGE_LENGTH_8;
        int x0 = static_cast<int>(box.m_box[0] * cellSizeInv);
        int x1 = static_cast<int>(box.m_box[3] * cellSizeInv);
        int z0 = static_cast<int>(box.m_box[2] * cellSizeInv);
        int z1 = static_cast<int>(box.m_box[5] * cellSizeInv);

        // Get landscape bounds
        int land_size = this->m_owner->m_level->land_size;
        int max_index = land_size - 1;

        // Clamp coordinates to valid range
        x0 = std::clamp(x0, 0, max_index);
        x1 = std::clamp(x1, 0, max_index);
        z0 = std::clamp(z0, 0, max_index);
        z1 = std::clamp(z1, 0, max_index);

        // Iterate through all affected cells
        for (int z = z0; z <= z1; ++z)
        {
            for (int x = x0; x <= x1; ++x)
            {
                // Get the collision items container for this cell
                auto& collisionItems = m_oCollisionitems[x + z * land_size];
                collisionItems->m_obstacles->insert(obstacle);
            }
        }
    }

    bool Landscape::SaveShoreLine(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::GetTileSize() const
    {
        return 4 * this->m_owner->m_level->land_size;
    }

    void Landscape::SetOverlayShader(rend::IEffect*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::LinkNodeAndChildrenCollisionGeomsToCell(SgNode* node)
    {
        // TODO: generated code
    
        // Get OBB and convert to AABB bounds
        auto obb = node->GetObb();
        // Assuming Obb::GetBounds converts OBB to AABB
        auto bounds = obb.GetBounds();

        auto VISCELL_EDGE_LENGTH_8 = 128.0;
        // Convert world coordinates to cell coordinates
        int x0 = static_cast<int>(bounds.m_box[0] * (1.0f / VISCELL_EDGE_LENGTH_8));
        int z0 = static_cast<int>(bounds.m_box[2] * (1.0f / VISCELL_EDGE_LENGTH_8));
        int x1 = static_cast<int>(bounds.m_box[3] * (1.0f / VISCELL_EDGE_LENGTH_8));
        int z1 = static_cast<int>(bounds.m_box[5] * (1.0f / VISCELL_EDGE_LENGTH_8));

        int landSize = this->m_owner->m_level->land_size - 1;

        // Clamp coordinates to valid range
        x0 = std::clamp(x0, 0, landSize);
        x1 = std::clamp(x1, 0, landSize);
        z0 = std::clamp(z0, 0, landSize);
        z1 = std::clamp(z1, 0, landSize);

        // Check node properties
        int PhysicBodyPtr = 0;
        int isNodeHaveCollision = 0;
        node->GetProperty(4356u, &PhysicBodyPtr);
        node->GetProperty(4358u, &isNodeHaveCollision);

        if (!PhysicBodyPtr || isNodeHaveCollision)
        {
            // Link the main node
            LinkNodeCollisionGeomsToCell(node, x0, x1, z0, z1);
            UpdateNodeCollisionGeoms(node);

            // Process children using iterative DFS
            std::vector<m3d::Object*> stack;
            stack.push_back(dynamic_cast<m3d::Object*>(node->GetFirstChild()));

            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process all siblings of the current node
                m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current);
                while (sibling)
                {
                    // Link the sibling node
                    LinkNodeCollisionGeomsToCell(sibling, x0, x1, z0, z1);
                    UpdateNodeCollisionGeoms(sibling);

                    // If this sibling has children, add to stack for processing
                    if (sibling->GetFirstChild()) {
                        stack.push_back(sibling->GetFirstChild());
                    }

                    // Move to next sibling
                    sibling = dynamic_cast<m3d::SgNode*>(sibling->GetNextSibling());
                }
            }
        }
    }

    float Landscape::GetHeightWithCollisions(float x, float y, bool forVehicle) const
    {
        static scoped_ptr viewRay = ai::Ray::CreateObject(nullptr, 10000.0, nullptr);
        static const CVector down{ 0.0, -1.0, 0.0 };
        static dContact contact;

        viewRay->SetDirection(down);
        dGeomSetPosition(viewRay->GetGeomId(), x, 3000.0, y);

        const auto aabb = viewRay->GetAabb();
        if (ai::TraceLine(*viewRay, contact, 1, 0, 0, 1, 0, 1, forVehicle))
        {
            return contact.geom.pos[1];
        }
        return 0.0;
    }

    m3d::rend::IAsmShader* waterPs = nullptr;

    void Landscape::InitReflectionRefractionTextures()
    {
        // TODO: generated code
        // Release any existing textures
        ReleaseReflectionRefractionTextures();

        // Determine texture size based on water quality setting
        int waterQuality = M3D_KERNEL->GetEngineCfg().m_r_waterQuality.GetI();
        int textureSize = (waterQuality >= 2 && waterQuality <= 3) ? 512 : 256;

        // Create reflection texture
        m_texRtReflection = m3d::Application::g_pApp->m_renderer->AddDynamicTexture(
            "$RtReflection",
            textureSize,
            textureSize,
            0);

        // Set reflection texture parameters
        M3D_RENDERER->SetTextureParameter(m_texRtReflection, m3d::rend::TexParam::TM_WRAP_S, 3);
        M3D_RENDERER->SetTextureParameter(m_texRtReflection, m3d::rend::TexParam::TM_WRAP_T, 3);
        M3D_RENDERER->SetTextureParameter(m_texRtReflection, m3d::rend::TexParam::TM_TEX_FILTER, 2);

        // Create refraction texture
        m_texRtRefraction = M3D_RENDERER->AddDynamicTexture(
            "$RtRefraction",
            textureSize,
            textureSize,
            0);

        // Set refraction texture parameters
        M3D_RENDERER->SetTextureParameter(m_texRtRefraction, m3d::rend::TexParam::TM_WRAP_S, 3);
        M3D_RENDERER->SetTextureParameter(m_texRtRefraction, m3d::rend::TexParam::TM_WRAP_T, 3);
        M3D_RENDERER->SetTextureParameter(m_texRtRefraction, m3d::rend::TexParam::TM_TEX_FILTER, 2);

        // Determine shader version to use
        int forcedVersion = M3D_KERNEL->GetEngineCfg().m_g_forceWaterPSVersion.GetI();
        m_waterShaderVersion = (forcedVersion == 11 || forcedVersion == 14 || forcedVersion == 20)
            ? forcedVersion
            : 20;

        // Downgrade shader version if hardware doesn't support it
        if (m_waterShaderVersion == 20)
        {
            bool allowPS20 = M3D_KERNEL->GetEngineCfg().m_r_allowPS20.GetB();
            if (!allowPS20 || !M3D_RENDERER->IsFeatureSupported(m3d::rend::DeviceFeature::FEATURE_PS_2_0))
            {
                m_waterShaderVersion = 14;
            }
        }

        if (m_waterShaderVersion == 14 && !M3D_RENDERER->IsFeatureSupported(m3d::rend::DeviceFeature::FEATURE_PS_1_4))
        {
            m_waterShaderVersion = 11;
        }

        // Load appropriate shaders based on version
        if (m_waterShaderVersion == 11)
        {
            // PS1.1 shaders
            waterPs = M3D_RENDERER->NewAsmShader("data/shaders/waterTest_ps11.asm", m3d::rend::IAsmShader::Type::PIXEL_SHADER);
            m_waterVs = M3D_RENDERER->NewHlslShader("data/shaders/waterTest_ps11.vs", "WaterVS", m3d::rend::IHlslShader::VS_1_1);

            // Load wave bump texture
            m_waveBumpTex = M3D_RENDERER->AddTexture("data/textures/water_dsdt.shader", 2);
            M3D_RENDERER->SetTextureParameter(m_waveBumpTex, m3d::rend::TexParam::TM_WRAP_S, 3);
            M3D_RENDERER->SetTextureParameter(m_waveBumpTex, m3d::rend::TexParam::TM_WRAP_T, 1);
        }
        else
        {
            // PS1.4 or PS2.0 shaders
            if (m_waterShaderVersion == 14)
            {
                // PS1.4 shaders
                waterPs = M3D_RENDERER->NewAsmShader("data/shaders/waterTest_ps14.asm", m3d::rend::IAsmShader::Type::PIXEL_SHADER);
                m_waterVs = M3D_RENDERER->NewHlslShader("data/shaders/waterTest_ps11.vs", "WaterVS", m3d::rend::IHlslShader::VS_1_1);
            }
            else
            {
                if (m_solidDeepVs)
                {
                    m_solidDeepVs->Release();
                    m_solidDeepVs = nullptr;
                }

                if (m_solidDeepPs)
                {
                    m_solidDeepPs->Release();
                    m_solidDeepPs = nullptr;
                }

                // PS2.0 shaders - select quality level
                if (waterQuality == 2)
                {
                    m_waterVs = M3D_RENDERER->NewHlslShader("data/shaders/waterTestMed_ps20.vs", "WaterVS", m3d::rend::IHlslShader::VS_2_0);
                    m_waterPs = M3D_RENDERER->NewHlslShader("data/shaders/waterTestMed_ps20.ps", "WaterPS", m3d::rend::IHlslShader::PS_2_0);

                    // Load deep shaders
                    m_solidDeepVs = M3D_RENDERER->NewHlslShader(
                        "data/shaders/landscapeDeep_ps20.vs", "LandscapeVS", m3d::rend::IHlslShader::VS_2_0);
                    m_solidDeepPs = M3D_RENDERER->NewHlslShader(
                        "data/shaders/landscapeDeep_ps20.ps", "LandscapePS", m3d::rend::IHlslShader::PS_2_0);
                }
                else if (waterQuality == 3)
                {
                    m_waterVs = M3D_RENDERER->NewHlslShader("data/shaders/waterTest_ps20.vs", "WaterVS", m3d::rend::IHlslShader::VS_2_0);
                    m_waterPs = M3D_RENDERER->NewHlslShader("data/shaders/waterTest_ps20.ps", "WaterPS", m3d::rend::IHlslShader::PS_2_0);

                    // Load deep shaders
                    m_solidDeepVs = M3D_RENDERER->NewHlslShader(
                        "data/shaders/landscapeDeep_ps20.vs", "LandscapeVS", m3d::rend::IHlslShader::VS_2_0);
                    m_solidDeepPs = M3D_RENDERER->NewHlslShader(
                        "data/shaders/landscapeDeep_ps20.ps", "LandscapePS", m3d::rend::IHlslShader::PS_2_0);
                }
                else
                {
                    m_waterVs = M3D_RENDERER->NewHlslShader("data/shaders/waterTestLow_ps20.vs", "WaterVS", m3d::rend::IHlslShader::VS_2_0);
                    m_waterPs = M3D_RENDERER->NewHlslShader("data/shaders/waterTestLow_ps20.ps", "WaterPS", m3d::rend::IHlslShader::PS_2_0);

                    // Load deep shaders
                    m_solidDeepVs = M3D_RENDERER->NewHlslShader(
                        "data/shaders/landscapeDeep_ps20.vs", "LandscapeVS", m3d::rend::IHlslShader::VS_2_0);
                    m_solidDeepPs = M3D_RENDERER->NewHlslShader(
                        "data/shaders/landscapeDeep_ps20.ps", "LandscapePS", m3d::rend::IHlslShader::PS_2_0);
                }
            }
            // Load fresnel texture
            m_fresnelTex = M3D_RENDERER->AddTexture("data/textures/fresnel.dds", 0);
            M3D_RENDERER->SetTextureParameter(m_fresnelTex, m3d::rend::TexParam::TM_WRAP_S, 3);
            M3D_RENDERER->SetTextureParameter(m_fresnelTex, m3d::rend::TexParam::TM_WRAP_T, 3);
        }

        // Load simple water shaders
        m_waterDumbPs = M3D_RENDERER->NewHlslShader("data/shaders/water_dumb.ps", "WaterPS", m3d::rend::IHlslShader::PS_1_1);
        m_waterDumbVs = M3D_RENDERER->NewHlslShader("data/shaders/water_dumb.vs", "WaterVS", m3d::rend::IHlslShader::VS_1_1);

        // Initialize scale matrix
        m_matScale._12 = 0.0;
        m_matScale._13 = 0.0;
        m_matScale._14 = 0.0;
        m_matScale._21 = 0.0;
        m_matScale._23 = 0.0;
        m_matScale._24 = 0.0;
        m_matScale._31 = 0.0;
        m_matScale._32 = 0.0;
        m_matScale._33 = 0.0;
        m_matScale._34 = 0.0;
        m_matScale._11 = 0.5f;
        m_matScale._22 = -0.5f;

        float offset = (0.5f / textureSize) + 0.5f;
        m_matScale._41 = offset;
        m_matScale._42 = offset;
        m_matScale._43 = 1.0f;
        m_matScale._44 = 1.0f;
    }

    void Landscape::SetAllTexturesLoading(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::FreeShoresStuff()
    {
       if (m_shoresVb.IsValid())
       {
           M3D_RENDERER->ReleaseVb(m_shoresVb);
       }

       if (m_shoresIb.IsValid())
       {
           M3D_RENDERER->ReleaseIb(m_shoresIb);
       }
    }

    void Landscape::BuildSolidLandscape()
    {
        if (m_solidVb.IsValid())
        {
            M3D_RENDERER->ReleaseVb(m_solidVb);
        }

        // TODO: check and refactor
        auto land_size = this->m_owner->m_level->land_size;
        m_solidVb = M3D_RENDERER->AddVb(rend::VertexType::VERTEX_YNI, 289 * land_size * land_size, "SolidLandscape", 0);
        this->vertsPerCell = 289;
        float* buff = (float*)M3D_RENDERER->LockVb(m_solidVb, vertsPerCell * land_size * land_size, 0, 0);
        M3D_ASSERT(buff);

        auto sizeinCells = land_size;
        unsigned v9 = 0;
        float* v10 = buff;
        unsigned v = 0;
        if (land_size)
        {
            do
            {
                auto v11 = 0;
                auto i = 0;
                auto v32 = 16 * v9;
                do
                {
                    auto v12 = 0;
                    auto nn = 0;
                    auto v34 = 16 * v11;
                    do
                    {
                        auto v13 = v32 + v12;
                        auto v14 = 0;
                        auto v15 = v12 << 7;
                        do
                        {
                            auto v16 = v13 + (this->m_mapSize + 1) * (v14 + v34);
                            *((unsigned short*)v10 + 2) = v15 + v14;
                            *v10 = this->m_heightMap[v16];
                            v16 *= 12;
                            *((unsigned short*)v10 + 3) = (int)(float)(*(float*)((char*)&this->m_vnormal->x + v16) * 1024.0);
                            *((unsigned short*)v10 + 4) = (int)(float)(*(float*)((char*)&this->m_vnormal->z + v16) * 1024.0);
                            *((unsigned short*)v10 + 5) = (int)(float)(*(float*)((char*)&this->m_vnormal->y + v16) * 1024.0);
                            ++v14;
                            v10 += 3;
                        } while (v14 <= 0x10);
                        v12 = ++nn;
                    } while (nn <= 0x10);
                    v11 = ++i;
                } while (i < sizeinCells);
                v9 = ++v;
            } while (v < sizeinCells);
        }
        M3D_RENDERER->UnlockVb(m_solidVb);

        int v25 = 0;
        int v17 = 0;
        unsigned ia = 0;
        do
        {
            if (m_solidIb[ia].IsValid())
                M3D_RENDERER->ReleaseIb(m_solidIb[ia]);
            int v19 = 1 << v17;
            int v20 = (unsigned __int16)(1 << v17);
            m_solidIb[ia] = M3D_RENDERER->AddIb(32 * (16 / v20 + 2) / v20,
                0);
            auto asd = 32 * (16 / v20 + 2) / v20;
            short* v21 = (short*)M3D_RENDERER->LockIb(m_solidIb[ia],
                32 * (16 / v20 + 2) / v20,
                0,
                0);
            M3D_RENDERER->GetLastErrorStr();
            int v22 = 0;
            unsigned nna = 0;
            unsigned va = 0;
            int v23 = 17 * v19;
            do
            {
                auto v24 = 0;
                do
                {
                    *v21 = v23 + v22;
                    v21[1] = v22;
                    if (v24 == 16)
                        v22 = v23 + v22 - 16;
                    else
                        v22 += v19;
                    nna += 2;
                    v24 += v20;
                    v21 += 2;
                } while (v24 <= 16);
                nna += 2;
                *v21 = v22 - v23 + 16;
                v21[1] = v23 + v22;
                v21 += 2;
                va += v20;
            } while (va < 16);

            trisPerCell[ia] = nna - 4;
            M3D_RENDERER->UnlockIb(m_solidIb[ia]);
            v25 = ++ia < 4;
        } while (v25);
    }

    void Landscape::DrawCellsOverlayedEditor(cmn::vector<unsigned> const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawSolidLandscape(LandRenderMode landMode, int lod)
    {
        M3D_RENDERER->SetAlphaTest(0);

        const auto fogColor = m_owner->GetWeatherFogColor();
        M3D_RENDERER->SetFogColor(fogColor, false);
        M3D_RENDERER->PushFog(M3D_ENGINE_CFG.m_r_enableFog.GetB());

        float fogStart = 0.0;
        float fogEnd = 0.0;
        GetFogStartAndEnd(fogStart, fogEnd);

        const auto fogReduceFactor = m_owner->GetWeatherManager().GetFogReduceFactorFromWeather();

        float vsFloat[4] = { 0 };
        vsFloat[0] = fogReduceFactor * fogEnd;
        vsFloat[1] = 1.0 / ((fogReduceFactor * fogEnd) - (fogReduceFactor * fogStart));
        vsFloat[2] = fogReduceFactor * fogStart;
        vsFloat[3] = m_owner->m_level->waterlevel;
        M3D_RENDERER->SetVsFloatConst(4u, vsFloat , 1u);

        auto projMat = M3D_RENDERER->MatGetProj();
        auto mat = M3D_RENDERER->MatGet();

        CMatrix du;
        auto v14 = projMat._42 * mat._14;
        du._11 = (((mat._13 * projMat._31) + (projMat._21 * mat._12)) + (mat._11 * projMat._11))
            + (mat._14 * projMat._41);
        auto v15 = (((projMat._22 * mat._12) + v14) + (mat._11 * projMat._12)) + (projMat._32 * mat._13);
        auto v16 = projMat._13;
        du._12 = v15;
        auto v17 = (((projMat._33 * mat._13) + (v16 * mat._11)) + (projMat._23 * mat._12)) + (mat._14 * projMat._43);
        auto v18 = mat._14 * projMat._44;
        du._13 = v17;
        auto v19 = (((projMat._14 * mat._11) + v18) + (mat._13 * projMat._34)) + (projMat._24 * mat._12);
        auto v20 = mat._23 * projMat._31;
        du._14 = v19;
        auto v21 = (((projMat._11 * mat._21) + v20) + (mat._24 * projMat._41)) + (projMat._21 * mat._22);
        auto v22 = projMat._22 * mat._22;
        du._21 = v21;
        auto v23 = (((mat._23 * projMat._32) + v22) + (mat._24 * projMat._42)) + (mat._21 * projMat._12);
        auto v24 = mat._24 * projMat._43;
        du._22 = v23;
        auto v25 = (((projMat._23 * mat._22) + v24) + (projMat._13 * mat._21)) + (mat._23 * projMat._33);
        auto v26 = mat._23 * projMat._34;
        du._23 = v25;
        auto v27 = (((mat._24 * projMat._44) + v26) + (projMat._24 * mat._22)) + (projMat._14 * mat._21);
        auto v28 = mat._33 * projMat._31;
        du._24 = v27;
        du._31 = (((projMat._11 * mat._31) + v28) + (projMat._21 * mat._32)) + (mat._34 * projMat._41);
        auto v29 = projMat._13 * mat._31;
        du._32 = (((mat._34 * projMat._42) + (mat._33 * projMat._32)) + (projMat._22 * mat._32))
            + (mat._31 * projMat._12);
        auto v30 = (((projMat._23 * mat._32) + v29) + (mat._34 * projMat._43)) + (mat._33 * projMat._33);
        auto v31 = mat._33 * projMat._34;
        du._33 = v30;
        auto v32 = (((mat._34 * projMat._44) + v31) + (projMat._24 * mat._32)) + (projMat._14 * mat._31);
        auto v33 = mat._43 * projMat._31;
        du._34 = v32;
        auto v34 = (((projMat._11 * mat._41) + v33) + (projMat._21 * mat._42)) + (mat._44 * projMat._41);
        auto v35 = mat._43 * projMat._32;
        du._41 = v34;
        auto v36 = (((mat._44 * projMat._42) + v35) + (projMat._22 * mat._42)) + (mat._41 * projMat._12);
        auto v37 = projMat._13 * mat._41;
        du._42 = v36;
        auto v38 = (((projMat._23 * mat._42) + v37) + (mat._44 * projMat._43)) + (mat._43 * projMat._33);
        auto v39 = mat._43 * projMat._34;
        du._43 = v38;
        du._44 = (((mat._44 * projMat._44) + v39) + (projMat._24 * mat._42)) + (projMat._14 * mat._41);

        switch (landMode)
        {
        case LRM_DIRECT:
        {
            const auto transitionDivider = M3D_ENGINE_CFG.m_lsTransitionDevider.GetF();
            // TODO: enable when DrawLandscapeTextures is implemented
            m_owner->GetGraph().SortedCellsStartFetching(m_drawRadius * transitionDivider + 1, m_drawRadius + 1);
            //m_owner->GetGraph().SortedCellsStartFetching(0, m_drawRadius);
            m_solidPs->Apply();
            m_solidVs->Apply();

            int projMatrixHandle = m_solidVs->GetParamHandleByName("mViewProj");
            m_solidVs->SetMatrix(projMatrixHandle, du);

            auto lightmapTexture = GetLightmapTexture();
            M3D_RENDERER->SetTexture(0, lightmapTexture, -1.0);
            break;
        }
        case LRM_BIND:
        {
            const auto transitionDivider = M3D_ENGINE_CFG.m_lsTransitionDevider.GetF();
            m_owner->GetGraph().SortedCellsStartFetching(transitionDivider * m_drawRadius - 1, transitionDivider * m_drawRadius + 1);
            m_solidBindPs->Apply();
            m_solidBindVs->Apply();

            int projMatrixHandle = m_solidBindVs->GetParamHandleByName("mViewProj");
            m_solidBindVs->SetMatrix(projMatrixHandle, du);

            int viewPosHandle = m_solidBindVs->GetParamHandleByName("ViewPos");
            m_solidBindVs->SetVector3(viewPosHandle, du.getOrgInv());

            const auto transitionCFatror = M3D_ENGINE_CFG.m_lsTransitionCFactor.GetF();
            const auto viewDistanceDivider = M3D_ENGINE_CFG.m_lsViewDistanceDivider.GetF();
            float vsConst = ((this->m_drawRadius * transitionDivider) - (viewDistanceDivider * transitionCFatror)) * 128.0;
            M3D_RENDERER->SetVsFloatConst(7u, &vsConst, 1u);

            auto lightmapTexture = GetLightmapTexture();
            M3D_RENDERER->SetTexture(0, lightmapTexture, -1.0);
            break;
        }
        default:
            RETRUXX_NOT_IMPLEMENTED;
        }

        M3D_RENDERER->SetToStream0(m_solidVb);

        // TODO: check this!!!
        int x = 0;
        int y = 0;
        int vis = 0;
        int radius = 0;
        int v = 0;
        int landDips = 0;
        float sizeinCells = m_owner->m_level->land_size;

        int maxX = -1;
        int maxY = -1;
        int maxVis = -1;
        int maxRadius = -1;
        int minX = 9999999;
        int minY = 9999999;
        int minVis = 9999999;
        int minRadius = 9999999;
        while (m_owner->GetGraph().SortedCellsFetch(x, y, vis, radius))
        {
            if (vis)
            {
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);
                maxVis = std::max(maxVis, vis);
                maxRadius = std::max(maxRadius, radius);
                minX = std::min(minX, x);
                minY = std::min(minY, y);
                minVis = std::min(minVis, vis);
                minRadius = std::min(minRadius, radius);
                float buff[3] = { 0 };

                buff[0] = x * 128.0;
                buff[1] = y * 128.0;
                buff[2] = 8.0;
                M3D_RENDERER->SetVsFloatConst(5u, buff, 1u);

                buff[0] = x / sizeinCells;
                buff[1] = y / sizeinCells;
                buff[2] = 1.0 / (float)(16 * m_owner->m_level->land_size);
                M3D_RENDERER->SetVsFloatConst(6u, buff, 1u);

                v += trisPerCell[lod];
                ++landDips;

                M3D_RENDERER->SetIndices(m_solidIb[lod], vertsPerCell * (y + sizeinCells * x));
                M3D_RENDERER->DrawIndexedPrimitiveShader(rend::M3DPT_TRIANGLESTRIP,
                                                  0,
                                                  this->vertsPerCell,
                                                  0,
                                                  this->trisPerCell[lod]);
            }
        }

        M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("lanscapeDips = " + CStr(landDips)).c_str());
        M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("lanscapeTris = " + CStr(v)).c_str());

        M3D_RENDERER->PopFog();

    }

    void Landscape::RemoveGrassInstance(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::QueryWaterVisibility()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawLandScapeTextures(VisibilityMode visMode, bool drawMinimap, bool roadMap)
    {
        auto& sceneGraph = m_owner->m_sceneGraph;
        const auto transitionDivider = M3D_ENGINE_CFG.m_lsTransitionDevider.GetF();
        sceneGraph.SortedCellsStartFetching(0, m_drawRadius * transitionDivider + 1);

        float fogStart = 0.0;
        float fogEnd = 0.0;
        GetFogStartAndEnd(fogStart, fogEnd);

        M3D_RENDERER->SetFogStart(fogStart, false);
        M3D_RENDERER->SetFogEnd(fogEnd, false);
        
        if (drawMinimap)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        else
        {
            int landSize = this->m_owner->m_level->land_size;
            int gridSize = 4 * landSize;
            // Normal rendering - process visible cells from scene graph
            int cellX, cellY, vis, radius;
            while (sceneGraph.SortedCellsFetch(cellX, cellY, vis, radius))
            {
                if (vis == 0)
                    continue;

                bool isFullyUnderwater = true;
                bool hasUnderwaterParts = false;

                // Process 4x4 block of cells
                for (int subY = 0; subY < 4; subY++)
                {
                    for (int subX = 0; subX < 4; subX++)
                    {
                        int worldX = 4 * cellX + subX;
                        int worldY = 4 * cellY + subY;

                        // Check water status
                        if (this->m_waterMap[worldX + worldY * landSize])
                        {
                            const Landscape::CellParams& cellParams = this->m_drawedCellParams[worldX + worldY * landSize];
                            float waterHeight = m3d::Landscape::getWaterHeight(worldX, worldY);

                            if (cellParams.m_h1 > waterHeight)
                            {
                                isFullyUnderwater = false;
                            }
                            if (waterHeight > cellParams.m_h0)
                            {
                                hasUnderwaterParts = true;
                            }
                        }
                        else
                        {
                            isFullyUnderwater = false;
                        }

                        // In editor mode, collect cells per texture
                        if (m3d::Landscape::m_renderMode == RM_EDITOR)
                        {
                            const Landscape::TileInfo& tileInfo = m3d::Landscape::GetTileInfo(worldX, worldY);
                            if (tileInfo.m_numTexs > 0)
                            {
                                for (int texIndex = 0; texIndex < tileInfo.m_numTexs; texIndex++)
                                {
                                    cmn::vector<unsigned int>* cellsPerTex = &this->m_cellsPerTex.m_data[tileInfo.m_texFlags[texIndex]];
                                    cellsPerTex->push_back(worldX + ((worldY + ((tileInfo.m_angle + (tileInfo.m_texFlags[texIndex] << 8)) << 8)) << 8));
                                }
                            }
                        }
                    }
                }

                // Skip cells based on visibility mode
                if (m3d::Landscape::m_renderMode)
                {
                    continue;
                }
                if (visMode == VIS_DIRECT)
                {
                    // Always render in direct mode
                }
                else if (visMode == VIS_REFLECTION)
                {
                    if (!isFullyUnderwater)
                        continue;
                }
                else if (visMode == VIS_REFRACTION)
                {
                    if (!hasUnderwaterParts)
                        continue;
                }

                // Collect texture sets for this cell
                int cellIndex = cellX + landSize * cellY;
                std::set<unsigned int>& textureSets = this->m_texSetsmap[cellIndex];

                for (auto it = textureSets.begin(); it != textureSets.end(); ++it)
                {
                    cmn::vector<unsigned int>& cellsPerTex = m_cellsPerTex[*it];
                    unsigned int cellData = cellX + (cellY << 8);
                    cellsPerTex.push_back(cellData);
                }
            }

            if (m3d::Landscape::m_renderMode == RM_EDITOR)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }
        }

        M3D_RENDERER->SetLighting(1, 0);
        M3D_RENDERER->LightEnable(0, 1);

        m3d::rend::Material material;
        material.init({ 1.0, 1.0, 1.0, 1.0 });
        M3D_RENDERER->MaterialSet(material);

        m_firstpasscounter = 0;
        m_otherpasscounter = 0;

        // TODO: implement landscape textures rendering
        //if (false)
        if (M3D_ENGINE_CFG.m_lsShadows.GetB())
        {
            // TODO: check this
            M3D_RENDERER->SetToStream(1, m_landUVVb);
            M3D_RENDERER->SetTexture(0, m_AlphaSets.front().m_texMasks[0].front(), -1.0);

            auto lightmapTexture = GetLightmapTexture();
            M3D_RENDERER->SetTexture(2, lightmapTexture, -1.0);
            const auto weatherFogColor = m_owner->GetWeatherFogColor();
            M3D_RENDERER->SetFogColor(weatherFogColor, false);
            M3D_RENDERER->SetFogMode(rend::M3DFOG_LINEAR, false);
            if (m3d::Landscape::m_renderMode == RM_EDITOR)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }
            else
            {
                m_landscapeVs->Apply();

                CVector fogTerm;
                fogTerm.x = fogEnd;
                fogTerm.z = fogStart;
                fogTerm.y = 1.0 / (fogEnd - fogStart);

                const auto fogTermHandle = m_landscapeVs->GetParamHandleByName("g_FogTerm");
                m_landscapeVs->SetVector3(fogTermHandle, fogTerm);

                const auto mat = M3D_RENDERER->MatGet();
                const auto projMat = M3D_RENDERER->MatGetProj();
                const auto matWorld = M3D_RENDERER->MatGetWorld();

                auto resultMat = mat * projMat;

                const auto viewProjHandle = m_landscapeVs->GetParamHandleByName("mViewProj");
                m_landscapeVs->SetMatrix(viewProjHandle, resultMat);

                const float VISCELL_EDGE_LENGTH_24 = 128.0;

                CVector lightmapScale;
                lightmapScale.x = 1.0 / (this->m_owner->m_level->land_size * VISCELL_EDGE_LENGTH_24);
                lightmapScale.y = 0.0 - (1.0 / (this->m_owner->m_level->land_size * VISCELL_EDGE_LENGTH_24));;
                lightmapScale.z = 0.0;
                const auto lightmapScaleHandle = m_landscapeVs->GetParamHandleByName("lightmapScale");
                m_landscapeVs->SetVector3(lightmapScaleHandle, lightmapScale);

                const auto worldMatHandle = m_landscapeVs->GetParamHandleByName("mWorld");
                m_landscapeVs->SetMatrix(worldMatHandle, matWorld);
            }

            for (int i = 0; i < m_tilesTextures.size(); ++i)
            {
                if (m_cellsPerTex[i].empty())
                {
                    continue;
                }

                M3D_RENDERER->SetTexture(1, m_tilesTextures[i]->m_texHandle, -1.0);

                int alphaSetNum = 0;
                if (m_hashIdxToLandType.getValueByKey(i, alphaSetNum))
                {
                    m_CurAlphaSet = m_Lands[alphaSetNum].m_alphaset;
                }
                else
                {
                    m_CurAlphaSet = 0;
                }

                M3D_RENDERER->TgSetTcSource(1, rend::TC_FROM_VERTEX, 1);
                if (m3d::Landscape::m_renderMode != RM_GAME)
                {
                    RETRUXX_NOT_IMPLEMENTED;
                }
                else
                {
                    M3D_RENDERER->SetZbState(rend::ZB_ENABLE, false);
                    DrawCellsFast0(m_cellsPerTex[i], *m_tilesTextures[i], RT_FIRSTPASSLIGHT);

                    M3D_RENDERER->SetZbState(rend::ZB_NOWRITE, false);
                    DrawCellsFast0(m_cellsPerTex[i], *m_tilesTextures[i], RT_OTHERPASSES);
                }

                m_cellsPerTex[i].clear();
            }
        }
        else
        {
            M3D_RENDERER->DisableTextureStages(0);
            // TODO: check this
            for (int i = 0; i < m_tilesTextures.size(); ++i)
            {
                m_cellsPerTex[i].clear();
            }
        }

        M3D_RENDERER->SetAlphaTest(0);
        M3D_RENDERER->SetBlend(rend::BM_NONE, 0);
        M3D_RENDERER->TgSetTcSource(0, rend::TC_FROM_VERTEX, 0);
        M3D_RENDERER->TgSetTcSource(1, rend::TC_FROM_VERTEX, 1);
        M3D_RENDERER->TgDisable(2);
        M3D_RENDERER->DisableTextureStages(1);

        if (m3d::Landscape::m_renderMode == RM_EDITOR)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        if (drawMinimap && m3d::Landscape::m_renderMode == RM_EDITOR)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        M3D_RENDERER->LightEnable(0, 0);
        M3D_RENDERER->SetLighting(0, 0);
    }

    rend::TexHandle Landscape::GetLightmapTexture() const
    {
        return m_texLightmap;
    }

    void Landscape::RemoveGrassRectangle(CVector2 const&, CVector2 const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::ChangedNumberOfUsedTextures(unsigned numTexs)
    {
        m_cellsPerTex.Deallocate();
        m_cellsPerTex.Allocate(numTexs);
        for (unsigned i = 0; i < numTexs; ++i)
        {
            m_cellsPerTex[i].Allocate(0x10000);
        }
    }

    int Landscape::ConstructCollisionData()
    {
        // TODO: generated code
        int landSize = this->m_owner->m_level->land_size;

        // Allocate collision items array
        m_oCollisionitems = new CollisionCellItem*[landSize * landSize];
        for (int i = 0; i < landSize * landSize; i++)
        {
            m_oCollisionitems[i] = nullptr;
        }

        // Process each cell in the landscape
        for (int cellY = 0; cellY < landSize; cellY++)
        {
            for (int cellX = 0; cellX < landSize; cellX++)
            {
                // Create collision cell item
                auto* item = new CollisionCellItem;
                item->m_wasEnabledLastFrame = false;
                item->m_bMustCheck = false;

                // Process sub-cells (4x4 grid within each cell)
                for (int subY = 0; subY < 4; subY++)
                {
                    int worldY = subY + 4 * cellY;
                    int vertexYOffset = 16 * cellY + 4 * subY;

                    for (int subX = 0; subX < 4; subX++)
                    {
                        int worldX = subX + 4 * cellX;

                        // Check if this position has water
                        if (m_waterMap[4 * worldY * landSize + 4 * worldX])
                        {
                            // Create water geometry object
                            auto* waterObj = RT_DYNCAST(M3D_KERNEL->New("GeomObjectWater"), GeomObjectWater);

                            // Allocate vertices and indices
                            waterObj->m_Vertices = new CVector[4]; // 4 vertices for the quad
                            waterObj->m_Indices = new int[6];     // 6 indices for two triangles

                            // Set up indices for two triangles forming a quad
                            int* indices = waterObj->m_Indices;
                            indices[0] = 2; indices[1] = 1; indices[2] = 0; // First triangle
                            indices[3] = 3; indices[4] = 1; indices[5] = 2; // Second triangle

                            // Create vertices for water quad
                            int vertexCounter = 0;
                            for (int vertexSubY = 0; vertexSubY < 2; vertexSubY++)
                            {
                                float vertexZ = (vertexYOffset + vertexSubY * 4) * 8.0f;

                                for (int vertexSubX = 0; vertexSubX < 2; vertexSubX++)
                                {
                                    float vertexX = (4 * worldX + vertexSubX * 4) * 8.0f;
                                    float waterHeight = m3d::Landscape::getWaterHeight(worldX, worldY);

                                    CVector* vertex = &waterObj->m_Vertices[vertexCounter];
                                    vertex->x = vertexX;
                                    vertex->y = waterHeight;
                                    vertex->z = vertexZ;

                                    vertexCounter++;
                                }
                            }

                            // Create triangle mesh for collision
                            waterObj->m_TriData = dGeomTriMeshDataCreate();
                            dGeomTriMeshDataBuildSingle(
                                waterObj->m_TriData,
                                waterObj->m_Vertices,
                                sizeof(CVector),  // vertex stride
                                4,                // vertex count
                                waterObj->m_Indices,
                                6,                // index count
                                3 * sizeof(int)   // triangle stride
                            );

                            // Create ODE geometry
                            dxSpace* odeSpace = m_owner->GetOdeSpace();
                            dxGeom* triMeshGeom = dCreateTriMesh(odeSpace, waterObj->m_TriData, 0, 0, 0);
                            waterObj->SetGeom(triMeshGeom);

                            waterObj->m_needToDeleteInUnlink = false;

                            // Add to collision cell's geometry list
                            item->m_geomsList.insert(waterObj);
                        }
                    }
                }

                // Store collision cell item in the array
                m_oCollisionitems[cellY * landSize + cellX] = item;
            }
        }

        // Create terrain heightfield data
        float* heightData = new float[4 * this->m_mapSize * this->m_mapSize];

        // Copy heightmap data (convert from (n+1)x(n+1) to nxn)
        for (int y = 0; y < this->m_mapSize; y++)
        {
            float* sourceRow = &this->m_heightMap[y * (this->m_mapSize + 1)];
            float* destRow = &heightData[y * this->m_mapSize];
            memcpy(destRow, sourceRow, sizeof(float) * this->m_mapSize);
        }

        // Create terrain geometry object
        this->m_terrainObject = (m3d::GeomObject*)m3d::g_Kernel->New("GeomObjectLandscape");

        dxSpace* terrainSpace = m_owner->GetOdeSpace();
        dxGeom* terrainGeom = dCreateTerrainY(
            terrainSpace,
            heightData,
            this->m_mapSize * 8.0f,  // terrain width
            this->m_mapSize,         // grid size
            1.0f,                    // vertical scale
            0                        // flags
        );

        m_terrainObject->SetGeom(terrainGeom);
        dGeomEnable(this->m_terrainObject->GetGeom());

        // Free temporary height data
        delete[] heightData;
        return true;
    }

    void Landscape::setOwner(CWorld* world)
    {
        m_owner = world;
    }

    Class* Landscape::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Landscape::CollisionCellItem::CollisionCellItem()
    {
        m_obstacles = new retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >;
    }

    Landscape::CollisionCellItem::~CollisionCellItem()
    {
        delete m_obstacles;
    }

    void Landscape::CollisionCellItem::InsertPhysicObjId(int objId)
    {
        m_physicObjIds.insert(objId);
        m_bMustCheck = true;
    }

    void Landscape::CollisionCellItem::ErasePhysicObjId(int objId)
    {
        m_physicObjIds.erase(objId);
        m_bMustCheck = true;
    }

    void Landscape::CollisionCellItem::InsertObstacle(ai::Obstacle* ob)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::CollisionCellItem::EraseObstacle(ai::Obstacle* ob)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Landscape::CollisionCellItem::
    GetPhysicObjIds() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle>>, retruxx::allocator<ref_ptr<ai::
    Obstacle>>>& Landscape::CollisionCellItem::GetObstacles() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Landscape::CollisionInfo::~CollisionInfo()
    {
        delete[] m_verts;
        delete[] m_tris;
    }

    CStr const& Landscape::GetPathToTiles() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::SetEditorRenderMode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::LinkNodeObstacleToCells(SgNode* node)
    {
        ai::Obstacle* nodeObstacle = nullptr;
        node->GetProperty(8718u, &nodeObstacle);
        if (nodeObstacle)
        {
            int smh = 0;
            node->GetProperty(4356u, &smh);
            if (!smh)
                LinkObstacleToCells(nodeObstacle);
        }
    }

    void Landscape::ReadTileInfo(int loadExtraTextures)
    {
        // TODO: generated code
        // Construct the TileInfo.xml path
        CStr tileInfoPath = m_pathTile + "TileInfo.xml";

        // Create and open the file stream
        scoped_ptr fileStream(M3D_KERNEL->GetFileServer().CreateFileStream());

        if (!fileStream->Open(tileInfoPath.c_str(), m3d::fs::IStream::OpenFlags::OPEN_READ))
        {
            M3D_LOG_ERR("TileSet: cannot open resources " + tileInfoPath);
            return;
        }

        // Create and read the XML file
        ref_ptr xmlFile(M3D_KERNEL->CreateXmlFile());
        if (!xmlFile->Read(*fileStream))
        {
            M3D_LOG_ERR("TileSet: cannot parse resources (" + CStr(xmlFile->GetError()) + ") " + tileInfoPath);
            return;
        }

        // Create XML nodes for parsing
        ref_ptr tileSetNode(xmlFile->CreateNode());
        ref_ptr alphaSetsNode(xmlFile->CreateNode());
        ref_ptr alphaSetNode(xmlFile->CreateNode());
        ref_ptr maskNode(xmlFile->CreateNode());

        // Get the TileSetName node
        xmlFile->GetFirstChild(tileSetNode, "TileSetName");

        // Process AlphaSets
        tileSetNode->GetFirstChild(alphaSetsNode, "AlphaSets");
        m_AlphaSets.clear();

        alphaSetsNode->GetFirstChild(alphaSetNode, "Set");
        while (!alphaSetNode->IsEmpty())
        {
            // Create new AlphaMask entry
            AlphaMask alphaMask;
            alphaMask.m_name = alphaSetNode->GetAttribute("name");

            // Process mask textures (mask1, mask2, etc.)
            for (int i = 1; i <= 4; i++)
            {
                CStr maskName = "mask" + CStr(i);
                alphaSetNode->GetFirstChild(maskNode, maskName.c_str());

                while (!maskNode->IsEmpty())
                {
                    const char* textureName = maskNode->GetAttribute("name");
                    CStr fullTexturePath = m_pathTile + textureName;

                    // Add texture and set parameters
                    rend::TexHandle texHandle = M3D_RENDERER->AddTexture(fullTexturePath, 2);

                    M3D_RENDERER->SetTextureParameter(texHandle, m3d::rend::TexParam::TM_WRAP_S, 3);
                    M3D_RENDERER->SetTextureParameter(texHandle, m3d::rend::TexParam::TM_WRAP_T, 3);
                    M3D_RENDERER->SetTextureParameter(texHandle, m3d::rend::TexParam::TM_MIP_LOD_BIAS, -1.0f);

                    alphaMask.m_texMasks[i - 1].push_back(texHandle);

                    maskNode->GetNextSibling(maskNode, maskName.c_str());
                }
            }

            // Add to AlphaSets and mapping
            m_AlphaSets.push_back(std::move(alphaMask));
            m_hashAlphaToLand.add(alphaMask.m_name, m_AlphaSets.size() - 1);

            alphaSetNode->GetNextSibling(alphaSetNode, "Set");
        }

        // Process LandTypes
        ref_ptr landTypesNode(xmlFile->CreateNode());
        tileSetNode->GetFirstChild(landTypesNode, "LandTypes");
        m_Lands.clear();

        // Process by priority order (0-9)
        for (int priority = 0; priority < 10; priority++)
        {
            ref_ptr typeNode(xmlFile->CreateNode());
            landTypesNode->GetFirstChild(typeNode, "type");

            while (!typeNode->IsEmpty())
            {
                // Check if this type matches current priority
                int typePriority = 0;
                m3d::SafeIntAttrib(typePriority, typeNode, "priority");

                if (typePriority == priority)
                {
                    LandType landType;
                    landType.m_priority = typePriority;

                    // Get basic attributes
                    const char* nameAttr = typeNode->GetAttribute("name");
                    landType.m_name = nameAttr;

                    const char* passmaskAttr = typeNode->GetAttribute("passmask");
                    landType.m_passmask = passmaskAttr ? atoi(passmaskAttr) : 0;

                    const char* alphasetAttr = typeNode->GetAttribute("alphaset");
                    landType.m_alphaset = 0;
                    m_hashAlphaToLand.add(alphasetAttr, landType.m_alphaset);

                    // Process tiles
                    ref_ptr tileNode(xmlFile->CreateNode());
                    typeNode->GetFirstChild(tileNode, "tile");

                    while (!tileNode->IsEmpty())
                    {
                        const char* tileFile = tileNode->GetAttribute("file");

                        if (loadExtraTextures &&
                            (m_loadAllTextures || m_usedTexturesList.find(tileFile) != m_usedTexturesList.end()))
                        {
                            CStr fullTilePath = m_pathTile + tileFile;
                            int texIndex = AddOneTexture(fullTilePath.c_str());
                            landType.m_texIndices.push_back(texIndex);

                            // Update mappings
                            m_hashIdxToLandType.addValueByKey(texIndex, m_Lands.size());
                            m_hashIdxToPass.addValueByKey(texIndex, landType.m_passmask);
                        }

                        tileNode->GetNextSibling(tileNode, "tile");
                    }

                    m_Lands.push_back(landType);
                }

                typeNode->GetNextSibling(typeNode, "type");
            }
        }

        // Update texture count
        ChangedNumberOfUsedTextures(m_tilesTextures.size());
    }

    void Landscape::_dbgGenerateGrass()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawJoint(dxJoint*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Landscape::~Landscape()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::UnlinkNodeCollisionGeomsFromCell(SgNode* node, int x, int y, bool deleteList)
    {
        std::set<m3d::GeomObject*>* t = nullptr;
        node->GetProperty(4357u, &t);
        if (t)
        {
            if (m_oCollisionitems)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }

            if (deleteList)
            {
                for (auto& elem : *t)
                {
                    if (m_oCollisionitems)
                    {
                        RETRUXX_NOT_IMPLEMENTED;
                    }
                    elem->Release();
                    delete elem;
                }

                t->clear();
                delete t;
                t = nullptr;
                node->SetProperty(4357u, &t);
            }
        }
    }

    int Landscape::New(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::HandleCVar(CVar const*, CConsoleParams const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::AddOneTexture(CStr const& name)
    {
        int idx = 0;
        if (m_texToIdx.get(name, idx))
        {
            return idx;
        }

        auto tex = M3D_RENDERER->AddTexture(name, 2);
        M3D_RENDERER->SetTextureParameter(tex, m3d::rend::TexParam::TM_WRAP_S, 1);
        M3D_RENDERER->SetTextureParameter(tex, m3d::rend::TexParam::TM_WRAP_T, 1);

        idx = m_tilesTextures.size();
        m_texToIdx.add(name, idx);

        auto chunk = new TIVChunk;
        chunk->m_texHandle = tex;
        m_tilesTextures.push_back(chunk);
        return idx;
    }

    void Landscape::HandleCommand(int, CConsoleParams const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::AddCollisionTris(int, int, CVector*, int, unsigned short*, CMatrix const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawShoreLine()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::ReloadWaterTextures()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::SaveNormalMap(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::SaveColorMap(CStr const&, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::SetLsHeight(float, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Landscape::GetLsHeight(float x, float y) const
    {
        auto v3 = (int)(float)(x * 0.125);
        auto v4 = (int)(float)(y * 0.125);
        if (v3 < 0)
            return 0.0;
        if (v4 < 0)
            return 0.0;
        auto v6 = this->m_mapSize - 1;
        if (v3 > v6 || v4 > v6)
            return 0.0;
        auto z1 = getHgtAtHfPoint(v3, (int)(float)(y * 0.125));
        auto HgtAtHfPoint = m3d::Landscape::getHgtAtHfPoint(v3 + 1, v4);
        auto v8 = v4 + 1;
        auto z2 = HgtAtHfPoint;
        auto z3 = m3d::Landscape::getHgtAtHfPoint(v3, v8);
        auto z4 = m3d::Landscape::getHgtAtHfPoint(v3 + 1, v8);

        int ox;
        *(float*)&ox = (x - (double)(int)(float)(x * 0.125) * 8.0) * 0.125;
        auto v9 = (y - (double)(int)(float)(y * 0.125) * 8.0) * 0.125;
        auto xa = v9;
        if (fabs(v9) + fabs(*(float*)&ox) > 1.0)
            return (float)((float)((float)((float)(z2 - z4) * (float)(1.0 - xa))
                + (float)((float)(z3 - z4) * (float)(1.0 - *(float*)&ox)))
                + z4);
        else
            return (float)((float)((float)((float)(z3 - z1) * xa) + (float)((float)(z2 - z1) * *(float*)&ox)) + z1);
    }

    unsigned Landscape::GetNumGrassModels() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::setHgtAtHfPoint(int, int, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Landscape::getHgtAtHfPoint(int x, int y) const
    {
        if (x > m_mapSize || x < 0 || y > m_mapSize || y < 0)
            return 0.0;
        else
            return this->m_heightMap[x + y * (m_mapSize + 1)];
    }

    rend::TexHandle Landscape::GetTexHandleFromList(unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::SaveTileInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::Load()
    {
        M3D_LOG_FLOW();

        m_mapSize = 16 * m_owner->m_level->land_size;

        auto const fileName = m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_beachsetsName);
        CStr error;
        ref_ptr xmlFile = ReadXmlFile(fileName.c_str(), &error);
        if (!xmlFile)
        {
            xmlFile = ReadXmlFile("data\\Beachsets.xml", &error);
        }
        if (xmlFile)
        {
            ref_ptr beachsets = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            ref_ptr wave = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            xmlFile->GetFirstChild(beachsets, "Beachsets");
            for (beachsets->GetFirstChild(wave, "Wave"); !wave->IsEmpty(); wave->GetNextSibling(wave, "Wave"))
            {
                WaveSets ws;
                SafeFloatAttrib(ws.m_tcomp, wave, "tcomp");
                if (ws.m_tcomp < 0.0)
                {
                    SafeFloatAttrib(ws.m_tlevel, wave, "tlevel");
                    SafeFloatAttrib(ws.m_tamplitude, wave, "tamplitude");
                    SafeFloatAttrib(ws.m_tphase, wave, "tphase");
                    SafeFloatAttrib(ws.m_tfreq, wave, "tfreq");
                }
                SafeFloatAttrib(ws.m_scomp, wave, "scomp");
                if (ws.m_scomp < 0.0)
                {
                    SafeFloatAttrib(ws.m_slevel, wave, "slevel");
                    SafeFloatAttrib(ws.m_samplitude, wave, "samplitude");
                    SafeFloatAttrib(ws.m_sphase, wave, "sphase");
                    SafeFloatAttrib(ws.m_sfreq, wave, "sfreq");
                }
                CStr tex;
                SafeStrAttrib(tex, wave, "texture");
                auto texHandle = M3D_APP->m_renderer->AddTexture(tex, 2);
                if (texHandle.IsValid())
                {
                    M3D_APP->m_renderer->SetTextureParameter(texHandle, rend::TM_WRAP_S, 3u);
                    M3D_APP->m_renderer->SetTextureParameter(texHandle, rend::TM_WRAP_T, 3u);
                    M3D_APP->m_renderer->SetTextureParameter(texHandle, rend::TM_MIP_LOD_BIAS, M3D_KERNEL->GetEngineCfg().m_g_shoresMipBias.GetF());
                    ws.m_texHandle = texHandle;
                }
                m_waves.push_back(ws);
            }
        }
        else
        {
            M3D_LOG_INFO("Error:No Beachset.xml file");
        }

        delete[] m_colormap;
        //TODO: check size
        m_colormap = new unsigned[(m_mapSize + 1) * (m_mapSize + 1)];
        
        delete[] m_texSetsmap;
        m_texSetsmap = new retruxx::set<unsigned>[m_owner->m_level->land_size * m_owner->m_level->land_size + 1];
        
        
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
            stream->Open(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_colormapName).c_str(), fs::IStream::OPEN_READ))
        {
            //TODO: check this!!!!
            auto const streamSize = stream->GetSize();
            auto* data = new unsigned[streamSize / sizeof(unsigned)];
            stream->ReadBytes(data, streamSize);
            for (int i = 0; i < m_mapSize; ++i)
            {
                for (int j = 0; j < m_mapSize; ++j)
                {
                    auto idx = j + i * (m_mapSize + 1);
                    m_colormap[idx] = data[idx];
                }
                m_colormap[m_mapSize + i * (m_mapSize + 1)] = m_colormap[m_mapSize + i * (m_mapSize + 1) - 1];
            }
            delete[] data;
        }
        else
        {
            M3D_LOG_INFO("Cannot read colormap: " + m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_colormapName) + "using default values");
            for (int i = 0; i < (this->m_mapSize + 1) * (this->m_mapSize + 1); ++i)
                this->m_colormap[i] = 0xFF808080; // Default gray color
        }
        
        delete[] m_heightMap;
        m_heightMap = new float[(m_mapSize + 1) * (m_mapSize + 1)];
        
        delete[] m_cliffHeightMap;
        m_cliffHeightMap = new unsigned char[(m_mapSize + 1) * (m_mapSize + 1)];


        // TODO: check this
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
            stream->Open(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_hfName).c_str(), fs::IStream::OPEN_READ))
        {
            auto const streamSize = stream->GetSize();
            if (streamSize == 2 * m_mapSize * m_mapSize) // 16-bit heightfield
            {
                //TODO: check this
                auto* data = new unsigned short[streamSize];
                stream->ReadBytes(data, streamSize);
                for (int i = 0; i < m_mapSize; ++i)
                {
                    for (int j = 0; j < m_mapSize; ++j)
                    {
                        auto idx = j + i * (m_mapSize + 1);
                        m_heightMap[idx] = data[idx] * 0.12;
                    }
                    m_heightMap[m_mapSize + i * (m_mapSize + 1)] = m_heightMap[m_mapSize + i * (m_mapSize + 1) - 1];
                }
                delete[] data;
            }
            else
            {
                auto* data = new float[streamSize];
                stream->ReadBytes(data, streamSize);

                // Process 32-bit height data with border expansion
                for (int j = 0; j < m_mapSize; j++)
                {
                    memcpy(&m_heightMap[j * (m_mapSize + 1)],
                        &data[j * m_mapSize],
                        m_mapSize * sizeof(float));
                    // Expand last column
                    m_heightMap[m_mapSize + j * (m_mapSize + 1)] = m_heightMap[(m_mapSize - 1) + j * (m_mapSize + 1)];
                }
                delete[] data;
            }
            stream->Close();
        }
        else
        {
            M3D_LOG_INFO("Cannot read heightfield: " + m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_hfName));
            return 0;
        }

        memcpy(&m_heightMap[(m_mapSize + 1) * (m_mapSize - 2)], &m_heightMap[(m_mapSize + 1) * (m_mapSize - 3)], 4 * m_mapSize + 4);
        memcpy((char*)m_heightMap + (m_mapSize + 1) * (4 * m_mapSize - 4), &m_heightMap[(m_mapSize + 1) * (m_mapSize - 2)], 4 * m_mapSize + 4);
        memcpy(&this->m_heightMap[m_mapSize * (m_mapSize + 1)], (char*)this->m_heightMap + (m_mapSize + 1) * (4 * m_mapSize - 4), 4 * m_mapSize + 4);
        memcpy(&this->m_colormap[(m_mapSize + 1) * (m_mapSize - 2)], &this->m_colormap[(m_mapSize + 1) * (m_mapSize - 3)], 4 * m_mapSize + 4);
        memcpy((char*)this->m_colormap + (m_mapSize + 1) * (4 * m_mapSize - 4), &this->m_colormap[(m_mapSize + 1) * (m_mapSize - 2)], 4 * m_mapSize + 4);
        memcpy(&this->m_colormap[m_mapSize * (m_mapSize + 1)], (char*)this->m_colormap + (m_mapSize + 1) * (4 * m_mapSize - 4), 4 * m_mapSize + 4);

        delete[] m_waterMap;

        const auto landSize = 4 * this->m_owner->m_level->land_size;
        const auto waterMapSize = landSize * landSize * sizeof(short);
        m_waterMap = new short[landSize * landSize];
        memset(m_waterMap, 0, waterMapSize);

        // TODO check this!!
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
            stream->Open(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_waterName).c_str(), fs::IStream::OPEN_READ))
        {
            unsigned int waterDataSize = stream->GetSize();
            if (waterDataSize == waterMapSize)
            {
                // Directly read into watermap if sizes match
                stream->ReadBytes(m_waterMap, waterDataSize);
            }
            else
            {
                RETRUXX_NOT_IMPLEMENTED;
                // Process water data with conversion
                unsigned char* tempData = new unsigned char[waterDataSize];
                stream->ReadBytes(tempData, waterDataSize);

                float waterLevel = m_owner->m_level->waterlevel * 8.333334f;
                for (int i = 0; i < landSize * landSize; i++)
                {
                    if (tempData[i] == 0xFF)
                        m_waterMap[i] = (short)waterLevel;
                    else
                        m_waterMap[i] = 0;
                }

                delete[] tempData;
            }
        }
        else
        {
            M3D_LOG_INFO("Cannot open watermap: using empty waterfield", LOG_INFO);
            memset(m_waterMap, 0, landSize * landSize * sizeof(short));
        }

        // Load normal map
        delete[] m_normalMap;
        delete[] m_vnormal;

        m_vnormal = new CVector[(m_mapSize + 1) * (m_mapSize + 1)];

        unsigned int startTime = M3D_KERNEL->GetTimer().GetCurTime();

        if (!LoadNormalMap(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_normalMapName)))
        {
            RecalcNormalMap(0, 0, m_mapSize, m_mapSize);
        }
        
        unsigned int loadTime = M3D_KERNEL->GetTimer().GetCurTime() - startTime;
        M3D_LOG_INFO("Normal map loaded in: " + CStr(loadTime));

        // Load tiles
        startTime =  M3D_KERNEL->GetTimer().GetCurTime();
        if (!LoadTiles(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_tilesFileName)))
        {
            return 0;
        }

        loadTime = M3D_KERNEL->GetTimer().GetCurTime() - startTime;
        M3D_LOG_INFO(CStr("Tiles loaded in: ") + CStr(loadTime));

        // Create LOD
        CreateLod();

        // Initialize passed cells
        delete[] m_passedCells;

        m_passedCells = new char[landSize * landSize];

        // Initialize reflection/refraction textures
        InitReflectionRefractionTextures();

        // Set draw radius
        float viewDist = M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF();
        m_drawRadius = viewDist * 8 + 4;
        m_drawRadius = std::clamp(m_drawRadius, 4, 12);

        // Load shoreline
        startTime = M3D_KERNEL->GetTimer().GetCurTime();
        if (!LoadShoreLine(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_shoreLineName)))
        {
            GenerateShoreLine();
        }

        loadTime = M3D_KERNEL->GetTimer().GetCurTime() - startTime;
        M3D_LOG_INFO(CStr("ShoreLine loaded in: ") + CStr(loadTime));

        // Initialize grass
        InitGrass();
        ReadGrassFromXmlFile(m_owner->m_level->GetFullPathNameA("grass.xml").c_str());

        // Build final landscape
        BuildSolidLandscape();

        return 1;
    }

    void Landscape::RenderGrass(unsigned, GrassInstance**, int*, RenderGrassType)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::RenderGrass(retruxx::deque<retruxx::pair<int, int>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::GetLsSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::RecalcUV()
    {
        // TODO: check and rafactor this shit!!
        auto v48 = &this->m_uvForAngles[0][0][1];
        auto v29 = 0;
        auto v1 = &this->m_setAndUVs.m_sets[0][0].m_uvForAngles[0][0][1];
        do
        {
            for (int masknum = 0; masknum < 5; ++masknum)
            {
                auto v2 = v48;
                auto i = 0;
                bool v24 = false;
                do
                {
                    CMatrix shiftHalf0;
                    memset(&shiftHalf0, 0, sizeof(shiftHalf0));
                    auto v3 = -((double)i * 1.5707964);

                    CMatrix shiftHalf1;
                    CMatrix mat;
                    memset(&shiftHalf1, 0, sizeof(shiftHalf1));
                    memset(&mat, 0, sizeof(mat));

                    auto v27 = sin(v3);
                    auto v26 = cos(v3);
                    auto v47 = v26;
                    auto v4 = (float)((float)((float)(mat._41 * shiftHalf0._14) + (float)(mat._31 * shiftHalf0._13))
                                 + (float)(shiftHalf0._12 * (float)(0.0 - v27)))
                        + v26;
                    auto v5 = (float)((float)((float)(mat._43 * shiftHalf0._14) + (float)(mat._23 * shiftHalf0._12)) + mat._13)
                        + shiftHalf0._13;
                    auto v36 = shiftHalf0._12 * v26 + mat._42 * shiftHalf0._14 + mat._32 * shiftHalf0._13 + v27;
                    auto v37 = (float)((float)((float)(mat._34 * shiftHalf0._13) + (float)(mat._24 * shiftHalf0._12)) + mat._14)
                        + shiftHalf0._14;
                    auto v6 = (float)((float)((float)(shiftHalf0._21 * v26) + (float)(shiftHalf0._24 * mat._41))
                                 + (float)(shiftHalf0._23 * mat._31))
                        + (float)(0.0 - v27);
                    auto v38 = (float)((float)((float)(shiftHalf0._23 * mat._34) + (float)(shiftHalf0._21 * mat._14)) + shiftHalf0._24)
                        + mat._24;
                    auto v39 = (float)((float)((float)(shiftHalf0._32 * v26) + (float)(shiftHalf0._31 * v27))
                                  + (float)(shiftHalf0._34 * mat._42))
                        + mat._32;
                    auto v40 = (float)((float)((float)(shiftHalf0._34 * mat._43) + (float)(shiftHalf0._32 * mat._23))
                                  + (float)(shiftHalf0._31 * mat._13))
                        + 1.0;
                    auto v7 = (float)((float)((float)(shiftHalf0._21 * v27) + (float)(shiftHalf0._24 * mat._42))
                                 + (float)(shiftHalf0._23 * mat._32))
                        + v26;
                    auto v8 = (float)((float)((float)(shiftHalf0._24 * mat._43) + (float)(shiftHalf0._21 * mat._13)) + shiftHalf0._23)
                        + mat._23;
                    auto v9 = (float)((float)((float)(shiftHalf0._31 * v26) + (float)(shiftHalf0._34 * mat._41))
                                 + (float)(shiftHalf0._32 * (float)(0.0 - v27)))
                        + mat._31;
                    auto v41 = (float)((float)((float)(shiftHalf0._32 * mat._24) + (float)(shiftHalf0._31 * mat._14)) + shiftHalf0._34)
                        + mat._34;
                    auto v33 = v26 * -0.5;
                    auto v42 = (float)((float)((float)(mat._31 * 0.0) - (float)((float)(0.0 - v27) * 0.5)) + (float)(v26 * -0.5))
                        + mat._41;
                    auto v43 = (float)((float)((float)(mat._32 * 0.0) - (float)(v27 * 0.5)) + (float)(v26 * -0.5)) + mat._42;
                    auto v44 = mat._43 - (float)((float)(mat._23 + mat._13) * 0.5);
                    auto v35 = v37 * 0.5;
                    auto v45 = (float)((float)(mat._34 * 0.0) - (float)((float)(mat._24 + mat._14) * 0.5)) + 1.0;
                    auto v52 = (float)((float)((float)(shiftHalf1._31 * v5) + (float)(shiftHalf1._21 * v36)) + (float)(v37 * 0.5)) + v4;
                    auto v53 = (float)((float)((float)(shiftHalf1._32 * v5) + (float)(shiftHalf1._12 * v4)) + (float)(v37 * 0.5)) + v36;
                    auto v54 = (float)((float)((float)(shiftHalf1._23 * v36) + (float)(shiftHalf1._13 * v4)) + (float)(v37 * 0.0)) + v5;
                    auto v55 = (float)((float)((float)(shiftHalf1._34 * v5) + (float)(shiftHalf1._24 * v36))
                                  + (float)(shiftHalf1._14 * v4))
                        + v37;
                    auto v49 = v38 * 0.5;
                    auto v56 = (float)((float)((float)(shiftHalf1._31 * v8) + (float)(shiftHalf1._21 * v7)) + (float)(v38 * 0.5)) + v6;
                    auto v57 = (float)((float)((float)(shiftHalf1._32 * v8) + (float)(shiftHalf1._12 * v6)) + (float)(v38 * 0.5)) + v7;
                    auto v58 = (float)((float)((float)(shiftHalf1._23 * v7) + (float)(shiftHalf1._13 * v6)) + (float)(v38 * 0.0)) + v8;
                    auto v34 = v41 * 0.5;
                    auto v59 = (float)((float)((float)(shiftHalf1._31 * v40) + (float)(shiftHalf1._21 * v39)) + (float)(v41 * 0.5)) + v9;
                    auto v60 = (float)((float)((float)(shiftHalf1._32 * v40) + (float)(shiftHalf1._12 * v9)) + (float)(v41 * 0.5)) + v39;
                    auto v61 = (float)((float)((float)(shiftHalf1._23 * v39) + (float)(shiftHalf1._13 * v9)) + (float)(v41 * 0.0)) + v40;
                    auto v32 = v45 * 0.5;
                    auto v62 = (float)((float)((float)(shiftHalf1._31 * v44) + (float)(shiftHalf1._21 * v43)) + (float)(v45 * 0.5)) + v42;
                    auto v63 = (float)((float)((float)(shiftHalf1._32 * v44) + (float)(shiftHalf1._12 * v42)) + (float)(v45 * 0.5)) + v43;
                    float v64[16];
                    v64[0] = v52;
                    v64[1] = v53;
                    v64[2] = v54;
                    v64[3] = v55;
                    v64[4] = v56;
                    v64[5] = v57;
                    v64[6] = v58;
                    v64[8] = v59;
                    v64[9] = v60;
                    v64[10] = v61;
                    v64[12] = v62;
                    v64[11] = (float)((float)((float)(shiftHalf1._34 * v40) + (float)(shiftHalf1._24 * v39))
                                      + (float)(shiftHalf1._14 * v9))
                        + v41;
                    v64[7] = (float)((float)((float)(shiftHalf1._34 * v8) + (float)(shiftHalf1._24 * v7))
                                     + (float)(shiftHalf1._14 * v6))
                        + v38;
                    auto v10 = 0.5;
                    v64[13] = v63;
                    v64[14] = (float)((float)((float)(shiftHalf1._23 * v43) + (float)(shiftHalf1._13 * v42)) + (float)(v45 * 0.0))
                        + v44;
                    v64[15] = (float)((float)((float)(shiftHalf1._34 * v44) + (float)(shiftHalf1._24 * v43))
                                      + (float)(shiftHalf1._14 * v42))
                        + v45;
                    memcpy(&mat, v64, sizeof(mat));
                    auto yy = 0;
                    auto v11 = mat._31 * 0.0;
                    auto v12 = mat._32 * 0.0;
                    auto v13 = v1;
                    do
                    {
                        auto v14 = (float)yy * 0.25;
                        auto v15 = v14 * mat._21;
                        auto v16 = v13;
                        auto v17 = 0;
                        auto v18 = v2;
                        v13 += 10;
                        auto v46 = (float)(v14 * mat._22) + v12;
                        v2 += 10;
                        do
                        {
                            auto v19 = (float)v17 * 0.25;
                            auto v20 = (float)((float)(mat._12 * v19) + v46) + mat._42;
                            auto v21 = (float)((float)((float)(v19 * mat._11) + v11) + mat._41) + v15;
                            *(v18 - 1) = v21;
                            *v18 = v20;
                            auto v22 = (float)(v21 * 0.125) * 0.94999999;
                            auto v23 = (float)(v20 * v10) * 0.94999999;
                            if (masknum)
                            {
                                v22 = (float)((float)((v29 + masknum) % 8) * 0.125) + v22;
                                v10 = 0.5;
                                v23 = (float)((float)((v29 + masknum) / 8) * 0.5) + v23;
                            }
                            *(v16 - 1) = v22 + 0.0031250007;
                            *v16 = v23 + 0.012500003;
                            ++v17;
                            v18 += 2;
                            v16 += 2;
                        } while (v17 < 5);
                        ++yy;
                    } while (yy < 5);
                    v24 = ++i < 4;
                    v1 = v13;
                } while (v24);
            }
            v29 += 4;
        } while (v29 < 32);
    }

    void Landscape::UpdateTexturesFilters()
    {
        const auto color = M3D_KERNEL->GetEngineCfg().m_g_texturesFilter.GetC();
        for (auto& tile : m_tilesTextures)
        {
            M3D_RENDERER->SetTextureParameter(tile->m_texHandle, m3d::rend::TexParam::TM_TEX_FILTER, color);
            if (color == 3)
            {
                const auto maxAnisotropy = M3D_RENDERER->GetMaxAnisotropy();
                M3D_RENDERER->SetTextureParameter(tile->m_texHandle, m3d::rend::TexParam::TM_MAX_ANISOTROPY, maxAnisotropy);
            }
        }
    }

    void Landscape::ScaleGrassRadius(CVector const&, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    //static std::set<CStr> nen;

    void Landscape::LinkNodeCollisionGeomsToCell(SgNode* node, int startX, int endX, int startY, int endY)
    {
        // TODO: generated code
        const auto landSize = m_owner->m_level->land_size;

        m3d::AnimatedModel* mdl = nullptr;
        int srvId = 0;
        if (node->GetProperty(4360u, &srvId) && srvId != -1)
        {
            node->GetServer()->GetItemProperty(srvId, 16394, &mdl);
        }

        if (mdl == nullptr)
        {
            return;
        }

        auto* geomObjsList = new retruxx::set<m3d::GeomObject*>();

        // Process collision points and triangles

        //if (nen.find(mdl->GetName()) != nen.end())
        //{
        //    bool asd = true;
        //}
        //nen.insert(mdl->GetName());

        if (!mdl->GetCollisionTrimesh().Points.empty())
        {
            // Create GeomObjectStatics
            auto* geomStatic = RT_DYNCAST(M3D_KERNEL->New("GeomObjectStatics"), GeomObjectStatics);

            // Allocate and copy points
            const auto pointsCount = mdl->GetCollisionTrimesh().Points.size();
            geomStatic->m_Vertices = new CVector[pointsCount];

            const auto scale = node->GetScale().x;
            for (size_t i = 0; i < pointsCount; i++)
            {
                geomStatic->m_Vertices[i].x = mdl->GetCollisionTrimesh().Points[i].x * scale;
                geomStatic->m_Vertices[i].y = mdl->GetCollisionTrimesh().Points[i].y * scale;
                geomStatic->m_Vertices[i].z = mdl->GetCollisionTrimesh().Points[i].z * scale;
            }

            // Allocate and copy triangles
            const auto trisCount = mdl->GetCollisionTrimesh().Triangles.size();
            geomStatic->m_Indices = new int[trisCount * 3];

            for (size_t i = 0; i < trisCount; ++i)
            {
                geomStatic->m_Indices[i * 3] = mdl->GetCollisionTrimesh().Triangles[i].I[0];
                geomStatic->m_Indices[i * 3 + 1] = mdl->GetCollisionTrimesh().Triangles[i].I[1];
                geomStatic->m_Indices[i * 3 + 2] = mdl->GetCollisionTrimesh().Triangles[i].I[2];
            }


            // Create ODE trimesh
            geomStatic->m_TriData = dGeomTriMeshDataCreate();

            dGeomTriMeshDataBuildSingle(geomStatic->m_TriData,
                geomStatic->m_Vertices, sizeof(CVector), pointsCount,
                geomStatic->m_Indices, trisCount * 3, 12);

            dxSpace* odeSpace = m_owner->GetOdeSpace();
            dxGeom* triMesh = dCreateTriMesh(odeSpace, geomStatic->m_TriData, 0, 0, 0);
            geomStatic->SetGeom(triMesh);

            geomStatic->m_rotation = { 0.0, 0.0, 0.0, 1.0 };
            geomStatic->m_translation = { 0.0, 0.0, 0.0 };

            // Set bounds
            PointBase<int> startCell(startX, startY);
            PointBase<int> endCell(endX, endY);
            geomStatic->SetBounds(startCell, endCell);

            // Add to list
            geomObjsList->insert(geomStatic);

            // Add to collision cells
            for (int y = startY; y <= endY; y++)
            {
                for (int x = startX; x <= endX; x++)
                {
                    int cellIndex = x + y * landSize;
                    m_oCollisionitems[cellIndex]->m_geomsList.insert(geomStatic);

                    if (m_oCollisionitems[cellIndex]->m_wasEnabledLastFrame)
                    {
                        geomStatic->IncEnabledCellsCount();
                    }
                }
            }
        }

        // Process individual geoms
        for (size_t i = 0; i < mdl->GetNumGeoms(); i++)
        {
            auto* geom = mdl->GetGeom(i);
            const auto scale = node->GetScale().x;

            auto* geomStatic = RT_DYNCAST(M3D_KERNEL->New("GeomObjectStatics"), GeomObjectStatics);

            dxGeom* odeGeom = nullptr;

            switch (geom->Type)
            {
            case 0: // Box
            {
                const auto lx = geom->Sizes.BoxSizes.x * scale;
                const auto ly = geom->Sizes.BoxSizes.y * scale;
                const auto lz = geom->Sizes.BoxSizes.z * scale;
                dxSpace* odeSpace = m_owner->GetOdeSpace();
                odeGeom = dCreateBox(odeSpace, lx, ly, lz);
                break;
            }
            case 1: // Sphere
            {
                float radius = geom->Sizes.BoxSizes.x * scale;
                dxSpace* odeSpace = m_owner->GetOdeSpace();
                odeGeom = dCreateSphere(odeSpace, radius);
                break;
            }
            case 2: // Cylinder/Capsule
            {
                float radius = geom->Sizes.BoxSizes.x * scale;
                float length = geom->Sizes.BoxSizes.y * scale;
                dxSpace* odeSpace = m_owner->GetOdeSpace();

                // Apply rotation for cylinder (45 degrees around some axis)
                Quaternion rot(geom->Rotation[0], geom->Rotation[1],
                    geom->Rotation[2], geom->Rotation[3]);
                Quaternion addRot(0.7071f, 0.0f, 0.0f, 0.7071f); // 45 degrees
                rot *= addRot;

                geomStatic->m_rotation = rot;
                odeGeom = dCreateCCylinder(odeSpace, radius, length);
                break;
            }
            }

            // Set position and rotation
            geomStatic->m_translation = geom->Translation;
            geomStatic->m_rotation = geom->Rotation;

            geomStatic->SetGeom(odeGeom);

            // Set bounds
            PointBase<int> startCell(startX, startY);
            PointBase<int> endCell(endX, endY);
            geomStatic->SetBounds(startCell, endCell);

            // Add to list
            geomObjsList->insert(geomStatic);

            // Add to collision cells
            for (int y = startY; y <= endY; y++)
            {
                for (int x = startX; x <= endX; x++)
                {
                    int cellIndex = x + y * landSize;
                    m_oCollisionitems[cellIndex]->m_geomsList.insert(geomStatic);
                }
            }
        }

        // Set property or cleanup
        if (!geomObjsList->empty())
        {
            node->SetProperty(4357u, &geomObjsList);
        }
        else
        {
            delete geomObjsList;
        }
    }

    void Landscape::DrawShoresLayer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::drawCellOverlayedShader(int, int, rend::IEffect*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Landscape::TileInfo const& Landscape::GetTileInfo(int, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::Invalidate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Landscape::AddGrassInstance(int, CVector const&, float, float, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::EndWaterQuery()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::SetNodeCollisionGeomsEnabled(SgNode*, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::Save16bitDisplace(CStr const&, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawCollisionGeoms(bool allGeoms)
    {
        const bool cgDraw = M3D_ENGINE_CFG.m_cgDraw.GetB();
        if (cgDraw)
        {
            CMatrix ident;
            ident.zero();

            ident._44 = 1.0;
            ident._33 = 1.0;
            ident._22 = 1.0;
            ident._11 = 1.0;

            M3D_RENDERER->MatMul(ident);

            M3D_RENDERER->SetTexture(0, {}, -1.0);
            M3D_RENDERER->SetTexture(1, {}, -1.0);
            M3D_RENDERER->PushFillMode(m3d::rend::FillMode::M3DFILL_WIREFRAME);
            M3D_RENDERER->SetCull(rend::M3DCULL_NONE, 0);
            auto landSize = m_owner->m_level->land_size;
            for (int y = 0; y < landSize; ++y)
            {
                for (int x = 0; x < landSize; ++x)
                {
                    int cellIndex = x + y * landSize;
                    auto* cell = m_oCollisionitems[cellIndex];
                    // TODO: check this
                    if (cell->m_wasEnabledLastFrame || allGeoms)
                    {
                        for (auto* geomObj : cell->m_geomsList)
                        {
                            if (dGeomIsEnabled(geomObj->GetGeom()))
                                DrawGeom(geomObj->GetGeom());
                        }
                    }
                }
            }
            M3D_RENDERER->PopFillMode();
        }
    }

    void Landscape::ClearCollisionCellsMap()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::AddGrassModel(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::GenerateOneDPVSCellMesh(int, int, CVector*, int*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Landscape::GetFloatToShortScale() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::drawSpriteOverlayed2(float, float, float, float, unsigned, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::RecalcNormalMap(int, int, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::ChangeShoreState(int, int, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Landscape::GetHeight(float x, float y, int excludeTag, bool notForCamera)
    {
        auto height = GetLsHeight(x, y);
        auto v5 = -99999.0;
        if (notForCamera)
            v5 = -999999.0;
        if (height <= v5)
            return v5;
        return height;
    }

    void Landscape::CheckLandscapeCollisionTriMeshesForObjId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GeomObject* Landscape::GetTerrainGeomObject() const
    {
        return this->m_terrainObject;
    }

    void Landscape::GetDPVSCollisionInfo(int, int, int&, int&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::RemoveGrassRadius(CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DisableShoreRegion(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::UpdateVis(bool vp)
    {
        m_profilerUpdateVis->StartCountdown();
        if (!m_lockVis.GetB())
        {
            m_dirtyReflection = vp;
            m_frustumCull.CreateScreenFrustums(m_drawRadius * 128.0, 1.0, 1.0, 0.78539819);
            m_owner->GetGraph().UpdateVis(true, m_frustumCull, true);
            m_owner->GetGraph().EnableVisibleCells(m_frustumCull, 1u);
            m_owner->GetRoadManager().UpdateVis();
            m_numWaterCells = 0;
            if (M3D_KERNEL->GetEngineCfg().m_g_drawWater.GetB())
            {
                // TODO: implement this!!!
                //RETRUXX_NOT_IMPLEMENTED;
            }
            if (m_numWaterCells)
            {
                //RETRUXX_NOT_IMPLEMENTED;
            }
        }
        m_profilerUpdateVis->EndCountdown();
    }

    Landscape::CollisionCellItem* Landscape::GetCollisionCellItem(int x, int y) const
    {
        auto land_size = this->m_owner->m_level->land_size;
        if (x < 0 || x >= land_size || y < 0 || y >= land_size)
        {
            return nullptr;
        }
        else
        {
            auto idx = x + y * land_size;
            return this->m_oCollisionitems[idx];
        }
    }

    int Landscape::GenerateShoreLine()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::ReleaseOdeCollisionData()
    {
        // TODO: check this
        if (m_oCollisionitems)
        {
            m_owner->m_roadManager.ReleaseCollision();

            const auto landSize = m_owner->m_level->land_size;
            for (int y = 0; y < landSize; ++y)
            {
                for (int x = 0; x < landSize; ++x)
                {
                    auto* item = m_oCollisionitems[x + y * landSize];
                    for (auto it = item->m_geomsList.begin(); it != item->m_geomsList.end();)
                    {
                        if (!(*it)->m_needToDeleteInUnlink)
                        {
                            (*it)->m_needToDeleteInUnlink = true;
                            (*it)->Release();
                            delete (*it);
                            it = item->m_geomsList.erase(it);
                        }
                        else
                        {
                            ++it;
                        }
                    }
                    delete item;
                }
            }
            delete[] m_oCollisionitems;
            m_oCollisionitems = nullptr;
            if (m_terrainObject)
            {
                m_terrainObject->m_needToDeleteInUnlink = true;
                m_terrainObject->Release();
                delete m_terrainObject;
                m_terrainObject = nullptr;
            }
        }
    }

    bool Landscape::LoadNormalMap(CStr const& fileName)
    {
        fs::auxTaggedFile taggedFile;
        if (taggedFile.Open(fileName.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_ERR("Error: Couldn't load normal map from file " + fileName);
            return false;
        }

        char* formatTitle = nullptr;
        taggedFile.getFormatTitle(&formatTitle);
        if (strcmp(formatTitle, "RIV"))
        {
            M3D_LOG_ERR("Error: Wrong normal map file format: " + CStr(formatTitle));
            return false;
        }

        unsigned formatVersion = 0;
        taggedFile.getFormatVersion(formatVersion);
        if (formatVersion != 1)
        {
            M3D_LOG_ERR("Error: Wrong normal map format version: " + CStr(formatVersion));
            return false;
        }

        void* data = nullptr;
        taggedFile.getChunkData(0xBADF00Du, &data);
        auto* charData = static_cast<unsigned char*>(data);
        charData += 4;

        // TODO: check and refactor
        if ((m_mapSize + 1) * (m_mapSize + 1) > 0)
        {
            int v12 = 0;
            int v13 = 0;
            do
            {
                auto v14 = &this->m_vnormal[v13];
                v14->x = (float)*(__int16*)charData * 0.000030518509;
                m_vnormal = this->m_vnormal;
                charData += 2;
                auto p_y = &m_vnormal[v13].y;
                *p_y = (float)*(__int16*)charData * 0.000030518509;
                charData += 2;
                ++v12;
                this->m_vnormal[v13++].z = sqrt(1.0 - v14->x * v14->x - *p_y * *p_y);
            } while (v12 < (this->m_mapSize + 1) * (this->m_mapSize + 1));
        }

        taggedFile.Close();
        return true;
    }

    float Landscape::GetScaleForTile() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::SwitchDrawMode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::RemoveCollisionTris(int tag)
    {
        if (tag >= 0)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        else
        {
            for (auto& collision : m_collisions)
            {
                if (collision)
                {
                    delete collision;
                }
            }
            m_collisions.clear();
        }
    }

    Object* Landscape::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::getMinMaxHeightForBox(float* box, float buldgeY)
    {
        // TODO: generated code
        constexpr float VISCELL_EDGE_LENGTH = 128.0f;
        const float invCellSize = 1.0f / VISCELL_EDGE_LENGTH;

        // Calculate grid indices for bounding box
        const unsigned minGridX = static_cast<unsigned>(box[0] * invCellSize);
        const unsigned minGridZ = static_cast<unsigned>(box[2] * invCellSize);
        const float maxGridX = box[3] * invCellSize;
        const float maxGridZ = box[5] * invCellSize;

        const unsigned landSize = m_owner->m_level->land_size;

        box[1] = 999999.0;
        box[4] = -999999.0;

        // Process all grid cells in bounding box
        for (unsigned gridX = minGridX; gridX < maxGridX; ++gridX)
        {
            for (unsigned gridZ = minGridZ; gridZ < maxGridZ; ++gridZ)
            {
                // Skip out-of-bounds cells
                if (gridX >= landSize || gridZ >= landSize)
                    continue;

                const auto& cell = m_cellParams[gridX + gridZ * landSize];

                if (box[1] > cell.m_h0)
                    box[1] = cell.m_h0;
                if (cell.m_h0 > box[4])
                    box[4] = cell.m_h0;
                if (box[1] > cell.m_h1)
                    box[1] = cell.m_h1;
                if (cell.m_h1 > box[4])
                    box[4] = cell.m_h1;

                // Process water cells
                if (cell.m_iswatercell)
                {
                    box[4] = std::max(box[4], cell.m_maxwater);
                }
            }
        }

        // Apply final offset to max height
        box[4] += VISCELL_EDGE_LENGTH + buldgeY;
    }

    void Landscape::SetGameRenderMode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::CollectGrassCell(int, int, unsigned&, GrassInstance**, int*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawGeom(dxGeom*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::Update()
    {
    }

    void Landscape::DrawWaterLayer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::CreateHelperStructures()
    {
        // TODO: generated code Landscape::CreateHelperStructures
        const int landSize = this->m_owner->m_level->land_size;
        const int sizeInCells = landSize;
        const int stride = 4 * landSize;

        // Initialize flags for the four corners of a tile
        int cornerFlags[4] = {1, 2, 4, 8};

        // Initialize maskHash
        m3d::CIntHash<unsigned int> maskHash;

        // Process each tile in the landscape
        for (int y = 0; y < landSize; ++y)
        {
            for (int xOffset = 0; xOffset < landSize; ++xOffset)
            {
                const unsigned int baseOffset = 4 * stride * y;

                // Process 4x4 subtile grid
                for (int subY = 0; subY < 4; ++subY)
                {
                    const int globalY = subY + 4 * y;
                    unsigned int currentOffset = baseOffset;

                    for (int subX = 0; subX < 4; ++subX)
                    {
                        const int globalX = 4 * xOffset + subX;

                        // Get the current tile info
                        m3d::Landscape::TileInfo* currentTile = &m_tiles[currentOffset + globalX];

                        // Clamp coordinates to valid range
                        const int clampedX = std::clamp(globalX, 0, stride - 1);
                        const int clampedY = std::clamp(globalY, 0, stride - 1);

                        // Get texture indices for the four corners of this subtile
                        int textureIndices[4];
                        textureIndices[0] = m_tiles[clampedX + stride * clampedY].m_texIndex0;
                        textureIndices[1] = m_tiles[std::clamp(globalX + 1, 0, stride - 1) + stride * clampedY].m_texIndex0;
                        textureIndices[2] = m_tiles[clampedX + stride * std::clamp(globalY + 1, 0, stride - 1)].m_texIndex0;
                        textureIndices[3] = m_tiles[std::clamp(globalX + 1, 0, stride - 1) + stride * std::clamp(globalY + 1, 0, stride - 1)].m_texIndex0;

                        // Reset tile texture count
                        currentTile->m_numTexs = 0;
                        std::vector<int> processedCorners(4, 0);

                        // Process each corner to determine unique textures and their coverage
                        for (int corner = 0; corner < 4; ++corner)
                        {
                            if (!processedCorners[corner])
                            {
                                int coverageFlags = cornerFlags[corner];

                                // Check if other corners share the same texture
                                for (int otherCorner = corner + 1; otherCorner < 4; ++otherCorner)
                                {
                                    if (!processedCorners[otherCorner] && textureIndices[corner] == textureIndices[otherCorner])
                                    {
                                        coverageFlags |= cornerFlags[otherCorner];
                                        processedCorners[otherCorner] = 1;
                                    }
                                }

                                // Add unique texture with its coverage
                                currentTile->m_texFlags[currentTile->m_numTexs] = coverageFlags;
                                currentTile->m_texIndices[currentTile->m_numTexs] = textureIndices[corner];
                                ++currentTile->m_numTexs;
                                processedCorners[corner] = 1;
                            }
                        }

                        // Check if this is a uniform tile (all corners same texture)
                        bool isUniformTile = true;
                        int baseLandType = -1;

                        // Look up land type for the first texture
                        int landTypeIter = 0;
                        if (m_hashIdxToLandType.getValueByKey(textureIndices[0], landTypeIter))
                        {
                            baseLandType = landTypeIter;

                            // Check surrounding tiles for consistency
                            for (int checkY = globalY - 1; checkY <= globalY + 1; ++checkY)
                            {
                                for (int checkX = globalX - 1; checkX <= globalX + 1; ++checkX)
                                {
                                    if (checkX >= 0 && checkX < stride && checkY >= 0 && checkY < stride)
                                    {
                                        unsigned int neighborTex = m_tiles[checkX + stride * checkY].m_texIndex0;

                                        int neighborLandTypeIter = 0;
                                        if (m_hashIdxToLandType.getValueByKey(neighborTex, neighborLandTypeIter))
                                        {
                                            if (baseLandType != neighborLandTypeIter)
                                            {
                                                isUniformTile = false;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (!isUniformTile)
                                    break;
                            }
                        }

                        // Simplify texture data for uniform tiles
                        if (currentTile->m_numTexs == 1 || isUniformTile)
                        {
                            currentTile->m_texFlags[0] = 0;  // Full coverage
                            currentTile->m_numTexs = 1;
                        }

                        // Sort textures by some criteria (appears to be by angle or priority)
                        for (int i = currentTile->m_numTexs - 1; i > 0; --i)
                        {
                            // Sorting logic based on some tile property
                            if (currentTile->m_texIndices[i] < currentTile->m_texIndices[i - 1])
                            {
                                std::swap(currentTile->m_texFlags[i], currentTile->m_texFlags[i - 1]);
                                std::swap(currentTile->m_texIndices[i], currentTile->m_texIndices[i - 1]);
                            }
                        }

                        // Set full coverage flag for multi-texture tiles
                        if (currentTile->m_numTexs != 1)
                        {
                            currentTile->m_texFlags[0] = 15;  // All corners covered
                        }

                        // Update rendering data structures for game mode
                        if (m3d::Landscape::m_renderMode == RM_GAME)
                        {
                            for (int texIdx = 0; texIdx < currentTile->m_numTexs; ++texIdx)
                            {
                                unsigned int textureId = currentTile->m_texIndices[texIdx];

                                // Add to cells per texture
                                m3d::cmn::vector<unsigned int>& cells = m_cellsPerTex[textureId];
                                if (cells.size() < cells.m_maxItems)
                                {
                                    // Encode tile position and properties
                                    unsigned int encodedPos =
                                        (globalX + ((globalY + ((currentTile->m_angle + (currentTile->m_texFlags[texIdx] << 8)) << 8)) << 8));
                                    cells.push_back(encodedPos);
                                }

                                // Add to texture set mapping
                                int cellIndex = xOffset + landSize * y;
                                m_texSetsmap[cellIndex].insert(textureId);
                            }
                        }

                        currentOffset += stride;
                    }
                }
            }
        }

        if (m3d::Landscape::m_renderMode == RM_GAME)
        {
            // Build vertex buffers and process textures
            int maxCells = 0;

            // Find maximum number of cells
            for (size_t i = 0; i < this->m_tilesTextures.size(); ++i)
            {
                if (this->m_cellsPerTex[i].size() > maxCells)
                {
                    maxCells = this->m_cellsPerTex[i].size();
                }
            }

            if (maxCells > 0)
            {
                m3d::rend::VertexLandscape* v46 =
                    new rend::VertexLandscape[25 * maxCells];

                for (size_t textureIndex = 0; textureIndex < this->m_tilesTextures.size(); ++textureIndex)
                {
                    m3d::Landscape::TIVChunk* v48 = this->m_tilesTextures[textureIndex];

                    if (this->m_cellsPerTex[textureIndex].size() > 0)
                    {
                        // Reset chunk data
                        memset(v48->m_offsetsmap, 0, sizeof(v48->m_offsetsmap));
                        memset(v48->m_banknumber, 0, sizeof(v48->m_banknumber));
                        memset(v48->m_numCellsPerCellMap, 0, sizeof(v48->m_numCellsPerCellMap));
                        v48->iotherPassOffset = 0;
                        v48->iotherPassBankNumber = 0;

                        int vofs = 0;
                        std::vector<int> bankSwitchingMap;

                        // Build cells for different render types
                       BuildCells0(v46, *v48, vofs, this->m_cellsPerTex[textureIndex], RT_FIRSTPASSLIGHT, bankSwitchingMap);

                        BuildCells0(
                            &v46[v48->iotherPassOffset], *v48, vofs, this->m_cellsPerTex[textureIndex], RT_OTHERPASSES, bankSwitchingMap);

                        // Reset cell count
                        this->m_cellsPerTex[textureIndex].clear();

                        bankSwitchingMap.push_back(vofs);

                        // Create vertex buffers
                        char* vertexDataPtr = reinterpret_cast<char*>(v46);

                        for (size_t bufferIndex = 0; bufferIndex < bankSwitchingMap.size(); ++bufferIndex)
                        {
                            int vertexCount = bankSwitchingMap[bufferIndex];

                            // Create vertex buffer
                            m3d::rend::VbHandle vb = M3D_RENDERER->AddVb(rend::VERTEX_XYZNCT1_UV2_S1, vertexCount, "Landscape", 0);

                            // Copy vertex data
                            void* lockedBuffer = M3D_RENDERER->LockVb(vb, vertexCount, 0, 0);
                            memcpy(lockedBuffer, vertexDataPtr, sizeof(rend::VertexLandscape) * vertexCount);
                            M3D_RENDERER->UnlockVb(vb);

                            vertexDataPtr += sizeof(rend::VertexLandscape) * vertexCount;

                            // Store vertex buffer handle
                            v48->m_vbHandle.push_back(vb);
                        }
                    }
                }

                delete[] v46;

                // Build UV set
               BuildUVSet();
            }
        }
    }

    void Landscape::drawSpriteOverlayed2Projected(float, float, float, float, unsigned, bool, CClipper const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* Landscape::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    void Landscape::PostServersLoad()
    {
        CreateHelperStructures();
        ConstructCollisionData();
    }

    void Landscape::RemoveGrassTile(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawCells(cmn::vector<unsigned> const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::GetVisCellHeights(float&, float&, int, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::CreateIndicesTriLists(int*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::UpdateNodeCollisionGeoms(SgNode* node)
    {
        std::set<m3d::GeomObject*>* t;
        node->GetProperty(4357u, &t);
        if (t)
        {
            for (auto& elem : *t)
            {
                auto x = node->GetScale().x;
                CVector pos;
                pos.x = elem->m_translation.x * x;
                pos.y = elem->m_translation.y * x;
                pos.z = elem->m_translation.z * x;

                CMatrix childMat;
                CMatrix parentMat;

                childMat.rotTranslate(elem->m_rotation, pos);
                parentMat.rotTranslate(node->GetRotationWorldAbs(), node->GetOriginWorldAbs());

                CMatrix vv;
                vv._11 = (float)((float)((float)(parentMat._41 * childMat._14) + (float)(parentMat._31 * childMat._13))
                    + (float)(parentMat._21 * childMat._12))
                    + (float)(parentMat._11 * childMat._11);
                vv._12 = (float)((float)((float)(parentMat._42 * childMat._14) + (float)(parentMat._32 * childMat._13))
                    + (float)(parentMat._22 * childMat._12))
                    + (float)(parentMat._12 * childMat._11);
                vv._13 = (float)((float)((float)(parentMat._43 * childMat._14) + (float)(parentMat._33 * childMat._13))
                    + (float)(parentMat._23 * childMat._12))
                    + (float)(parentMat._13 * childMat._11);
                vv._14 = (float)((float)((float)(parentMat._44 * childMat._14) + (float)(parentMat._34 * childMat._13))
                    + (float)(parentMat._24 * childMat._12))
                    + (float)(parentMat._14 * childMat._11);
                vv._21 = (float)((float)((float)(childMat._24 * parentMat._41) + (float)(childMat._23 * parentMat._31))
                    + (float)(childMat._22 * parentMat._21))
                    + (float)(childMat._21 * parentMat._11);
                vv._22 = (float)((float)((float)(childMat._24 * parentMat._42) + (float)(childMat._23 * parentMat._32))
                    + (float)(childMat._22 * parentMat._22))
                    + (float)(childMat._21 * parentMat._12);
                vv._23 = (float)((float)((float)(childMat._24 * parentMat._43) + (float)(childMat._23 * parentMat._33))
                    + (float)(childMat._22 * parentMat._23))
                    + (float)(childMat._21 * parentMat._13);
                vv._24 = (float)((float)((float)(childMat._24 * parentMat._44) + (float)(childMat._23 * parentMat._34))
                    + (float)(childMat._22 * parentMat._24))
                    + (float)(childMat._21 * parentMat._14);
                vv._31 = (float)((float)((float)(childMat._34 * parentMat._41) + (float)(childMat._33 * parentMat._31))
                    + (float)(childMat._32 * parentMat._21))
                    + (float)(childMat._31 * parentMat._11);
                vv._32 = (float)((float)((float)(childMat._34 * parentMat._42) + (float)(childMat._33 * parentMat._32))
                    + (float)(childMat._32 * parentMat._22))
                    + (float)(childMat._31 * parentMat._12);
                vv._33 = (float)((float)((float)(childMat._34 * parentMat._43) + (float)(childMat._33 * parentMat._33))
                    + (float)(childMat._32 * parentMat._23))
                    + (float)(childMat._31 * parentMat._13);
                vv._34 = (float)((float)((float)(childMat._34 * parentMat._44) + (float)(childMat._33 * parentMat._34))
                    + (float)(childMat._32 * parentMat._24))
                    + (float)(childMat._31 * parentMat._14);
                vv._41 = (float)((float)((float)(childMat._44 * parentMat._41) + (float)(childMat._43 * parentMat._31))
                    + (float)(childMat._42 * parentMat._21))
                    + (float)(childMat._41 * parentMat._11);
                vv._42 = (float)((float)((float)(childMat._44 * parentMat._42) + (float)(childMat._43 * parentMat._32))
                    + (float)(childMat._42 * parentMat._22))
                    + (float)(childMat._41 * parentMat._12);
                vv._43 = (float)((float)((float)(childMat._44 * parentMat._43) + (float)(childMat._43 * parentMat._33))
                    + (float)(childMat._42 * parentMat._23))
                    + (float)(childMat._41 * parentMat._13);
                vv._44 = (float)((float)((float)(childMat._44 * parentMat._44) + (float)(childMat._43 * parentMat._34))
                    + (float)(childMat._42 * parentMat._24))
                    + (float)(childMat._41 * parentMat._14);

                Quaternion quat;
                quat.FromMatrix(vv);

                float odeq[4];
                odeq[0] = quat.w;
                odeq[1] = quat.x;
                odeq[2] = quat.y;
                odeq[3] = quat.z;
                dGeomSetQuaternion(elem->GetGeom(), odeq);
                dGeomSetPosition(elem->GetGeom(), vv._41, vv._42, vv._43);
            }
        }
    }

    void Landscape::ReleaseLod()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawMassBox(dMass*, CVector const&, Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::InitGrass()
    {
        m_grassVs = M3D_RENDERER->NewHlslShader("data/shaders/grassTest_vs11.vs", "GrassVS", rend::IHlslShader::VS_1_1);
        if (!m_grassVs->IsValid())
        {
            return false;
        }

        m_grassPs = M3D_RENDERER->NewHlslShader("data/shaders/grassTest_ps11.ps", "GrassPS", rend::IHlslShader::PS_1_1);
        if (!m_grassPs->IsValid())
        {
            return false;
        }

        m_grassArray = new TileGrass*[0x10000];
        memset(m_grassArray, 0, 0x10000 * sizeof(TileGrass*));

        return true;
    }

    void Landscape::StartWaterQuery()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::PutGrassToLandscape(CVector2 const&, CVector2 const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::GetNumTiles() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::setDrawRadius(int, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::GetDrawedCellHeights(float&, float&, int, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Landscape::GetNearestGrassInstance(CVector const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DoneGrass()
    {
        if (m_grassArray)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }

    float Landscape::getCameraHeight(float, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::getGrassModelIdByName(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::SaveCameraMap(CStr const&, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::Release()
    {
        DoneGrass();
        M3D_RENDERER->ReleaseVb(m_solidVb);
        for (auto& ib : m_solidIb)
        {
            M3D_RENDERER->ReleaseIb(ib);
        }

        delete[] m_heightMap;
        m_heightMap = nullptr;

        delete[] m_waterMap;
        m_waterMap = nullptr;

        delete[] m_cliffHeightMap;
        m_cliffHeightMap = nullptr;

        delete[] m_colormap;
        m_colormap = nullptr;

        delete[] m_texSetsmap;
        m_texSetsmap = nullptr;

        FreeShoresStuff();
        ReleaseReflectionRefractionTextures();

        delete[] m_cellParams;
        m_cellParams = nullptr;

        delete[] m_drawedCellParams;
        m_drawedCellParams = nullptr;

        RemoveCollisionTris(-1);
        ReleaseOdeCollisionData();
        FreeTiles();

        for (auto& wave : m_waves)
        {
            M3D_RENDERER->ReleaseTexture(wave.m_texHandle);
        }
        m_waves.clear();

        for (auto& col : m_collisions)
        {
            delete col;
        }
        m_collisions.clear();

        delete[] m_normalMap;
        m_normalMap = nullptr;

        delete[] m_vnormal;
        m_vnormal = nullptr;
    }

    void Landscape::EnableShoreRegion(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int frame = 0;
    const float VISCELL_EDGE_LENGTH_24 = 128.0;

    void Landscape::Render()
    {
        if (M3D_KERNEL->GetEngineCfg().m_lsWireframe.GetB())
        {
            M3D_RENDERER->PushFillMode(rend::FillMode::M3DFILL_WIREFRAME);
        }
        else
        {
            M3D_RENDERER->PushFillMode(rend::FillMode::M3DFILL_SOLID);
        }

        // TODO: check this (m3d::g_Kernel->GetEngineCfg(m3d::g_Kernel)->m_r_waterQuality.m_i)
        m_waterPlane.m_normal.x = 0.0;
        m_waterPlane.m_normal.y = 1.0;
        m_waterPlane.m_normal.z = 0.0;
        m_waterPlane.m_dist = this->m_owner->m_level->waterlevel;
        

        // TODO: save divider
        m_bindDevider = ((1.1 - M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF()) * 0.44999999) + 0.55000001;

        auto saveDistDivider = ((M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF() * 8.0) + 4.0);
        if (saveDistDivider >= 4)
        {
            if (saveDistDivider <= 12)
            {

            }
            else
            {
                saveDistDivider = 12;
            }
        }
        else
        {
            saveDistDivider = 4;
        }
        
        M3D_RENDERER->SetLighting(false, false);
        M3D_RENDERER->PushZFunc(rend::CmpFunc::M3DCMP_LESS);
        M3D_RENDERER->PushBlend(rend::BlendMode::BM_NONE);
        M3D_RENDERER->PushCull();
        M3D_RENDERER->PushZbState();
        M3D_RENDERER->PushFog(M3D_KERNEL->GetEngineCfg().m_r_enableFog.GetB());
        if (m_numWaterCells != 0 && m_isWaterVisible)
        {
            // TODO: implement water rendering
            RETRUXX_NOT_IMPLEMENTED;
        }

        m_profilerDraw->StartCountdown();
        M3D_RENDERER->SetFog(false, false);
        M3D_RENDERER->SetBlend(rend::BlendMode::BM_NONE, false);
        M3D_RENDERER->SetZbState(rend::ZbState::ZB_DISABLE, false);

        if ((M3D_KERNEL->GetEngineCfg().m_g_drawSky.GetB()))
        {
            if ((frame & 1) == 0)
            {
                m_owner->UpdateSkyParams();
            }
            ++frame;
            m_owner->RenderSky(LRM_DIRECT);
        }

        if ((M3D_KERNEL->GetEngineCfg().m_lgtFlares.GetB()))
        {
            m_flares.Render(FlareMode::FLARE_SUN, m_owner->m_sunDir, 1.0, 1.0);
        }


        auto v55 = VISCELL_EDGE_LENGTH_24;
        auto v56 = (float)(saveDistDivider - 4) * VISCELL_EDGE_LENGTH_24;
        auto v57 = (float)(saveDistDivider - 8) * VISCELL_EDGE_LENGTH_24;
        this->m_landscapeClip0z = v57;
        auto v58 = (float)saveDistDivider * v55;
        this->m_landscapeClip1zSq = v56 * v56;
        this->m_drawRadius = saveDistDivider;
        this->m_landscapeClip0 = saveDistDivider - 8;
        this->m_landscapeClip1 = saveDistDivider - 4;
        this->m_landscapeClip2 = saveDistDivider;
        this->m_landscapeClip1z = v56;
        this->m_landscapeClip2z = v58;
        this->m_landscapeClip0zSq = v57 * v57;
        this->m_landscapeClip2zSq = v58 * v58;

        if ((M3D_KERNEL->GetEngineCfg().m_r_renderZGuard.GetB()))
        {
            renderZGuard();
        }

        M3D_RENDERER->SetFog(M3D_KERNEL->GetEngineCfg().m_r_enableFog.GetB(), false);
        M3D_RENDERER->PushZbState(rend::ZbState::ZB_ENABLE);
        M3D_RENDERER->PushZFunc(rend::CmpFunc::M3DCMP_LESS);
        M3D_RENDERER->PushBlend(rend::BlendMode::BM_NONE);
        this->m_owner->m_sceneGraph.SetVisMask(1);
        M3D_RENDERER->SetCull(rend::Cull::M3DCULL_CW, false);
        DrawLandScapeTextures(VIS_DIRECT, false, false);

        M3D_RENDERER->SetZbState(rend::ZbState::ZB_ENABLE, false);
        M3D_RENDERER->SetZFunc(rend::CmpFunc::M3DCMP_LESS, false);
        M3D_RENDERER->SetCull(rend::Cull::M3DCULL_CCW, false);
        DrawSolidLandscape(LRM_DIRECT, 0);

        M3D_RENDERER->SetZbState(rend::ZbState::ZB_NOWRITE, false);
        M3D_RENDERER->SetBlend(rend::BlendMode::BM_ALPHA, false);
        DrawSolidLandscape(LRM_BIND, 0);

        M3D_RENDERER->SetZbState(rend::ZbState::ZB_ENABLE, false);
        RenderRoads();

        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopZFunc();
        M3D_RENDERER->PopBlend();

        m_profilerDraw->EndCountdown();

        DrawCollisionGeoms(true);

        M3D_RENDERER->SetFog(M3D_KERNEL->GetEngineCfg().m_r_enableFog.GetB(), false);
        M3D_RENDERER->SetCull(rend::Cull::M3DCULL_CCW, false);
        M3D_RENDERER->SetZbState(rend::ZbState::ZB_ENABLE, false);

        m_owner->m_sceneGraph.UpdateVis(false, m_frustumCull, true);
        m_owner->m_sceneGraph.Render(SGRF_DEFAULT_OPAQUE);

        if ((M3D_KERNEL->GetEngineCfg().m_r_waterInQuery.GetB()))
        {
            if (m_numWaterCells)
            {
                QueryWaterVisibility();
            }
        }
        else
        {
            m_isWaterVisible = true;
        }

        M3D_RENDERER->SetFog(false, false);
        m_owner->m_sceneGraph.Render(SGRF_SHADOWS);

        M3D_RENDERER->SetFog(M3D_KERNEL->GetEngineCfg().m_r_enableFog.GetB(), false);
        if (!M3D_KERNEL->GetEngineCfg().m_dsShadows.GetB() || !pClient->GetWorld().GetWeatherManager().GetShadowVisibilityFromWeather())
        {
            RenderGrass({});
        }

        if (m_numWaterCells /* && HIBYTE(v97) */)
        {
            // TODO: implement water rendering
            RETRUXX_NOT_IMPLEMENTED;
        }

        if ((M3D_KERNEL->GetEngineCfg().m_g_drawShores.GetB()))
        {
            DrawShoresLayer();
        }

        M3D_RENDERER->SetZbState(rend::ZbState::ZB_NOWRITE, false);
        m_owner->m_sceneGraph.Render(SGRF_DEFAULT_TRANS);
        m_owner->m_weatherManager.RenderWeatherParticles();

        if ((M3D_KERNEL->GetEngineCfg().m_lsWireframe.GetB()))
        {
            M3D_RENDERER->PopFillMode();
        }

        M3D_RENDERER->SetFog(false, false);
        M3D_RENDERER->SetZbState(rend::ZbState::ZB_DISABLE, false);
        m_owner->m_sceneGraph.Render(SGRF_OVERLAYS);
        m_owner->m_sceneGraph.RenderContouredNodes();

        m_profilerDraw->StartCountdown();
        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopZFunc();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopZbState();

        if ((M3D_KERNEL->GetEngineCfg().m_g_showReflRefrMaps.GetB()))
        {
            // TODO: implement reflections rendering
            RETRUXX_NOT_IMPLEMENTED;
        }
        m_profilerDraw->EndCountdown();
    }

    int Landscape::Render(SgNodeRenderFlags, void*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::ReadGrassFromXmlFile(char const*)
    {
        // TODO: implement Landscape::ReadGrassFromXmlFile
        // RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::WriteGrassToXmlFile(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Landscape::getNormal(float worldX, float worldZ)
    {
        // TODO: generated code
         // Convert world coordinates to heightmap coordinates (scale factor 0.125 = 1/8)
        int mapX = static_cast<int>(worldX * 0.125f);
        int mapZ = static_cast<int>(worldZ * 0.125f);

        int mapSize = this->m_mapSize;
        int mapSizePlusOne = mapSize + 1;


        // Check if coordinates are out of bounds
        if (mapX < 0 || mapZ < 0 ||
            mapX + 2 >= mapSizePlusOne ||
            mapZ + 2 >= mapSizePlusOne) {
            // Return default up vector for out-of-bounds coordinates

            CVector result;
            result.x = 0.0f;
            result.y = 1.0f;
            result.z = 0.0f;
            return result;
        }

        // Clamp coordinates to map boundaries
        if (mapX == mapSizePlusOne) {
            mapX = mapSize;
        }
        if (mapZ == mapSizePlusOne) {
            mapZ = mapSize;
        }

        // Arrays to store normal components for the 2x2 quad
        float normalX[8];
        float normalY[8];
        float normalZ[8];

        // Initialize normal arrays
        for (int i = 0; i < 8; i++) {
            normalZ[i] = 64.0f;  // Constant Z component
            normalY[i] = 64.0f;  // Constant Y component
        }

        // Calculate normals for the 2x2 quad around the point
        int index = 0;
        for (int z = mapZ; z <= mapZ + 1; z++) {
            for (int x = mapX; x <= mapX + 1; x++) {
                // Get height values for the current quad
                int currentIndex = z * (mapSize + 1) + x;
                int rightIndex = z * (mapSize + 1) + ((x + 1) % (mapSize + 1));
                int bottomIndex = ((z + 1) % (mapSize + 1)) * (mapSize + 1) + x;
                int bottomRightIndex = ((z + 1) % (mapSize + 1)) * (mapSize + 1) + ((x + 1) % (mapSize + 1));

                float currentHeight = m_heightMap[currentIndex];
                float rightHeight = m_heightMap[rightIndex];
                float bottomHeight = m_heightMap[bottomIndex];
                float bottomRightHeight = m_heightMap[bottomRightIndex];

                // Calculate X component of normal (derivative in X direction)
                // Based on height differences between right and current points
                normalX[index] = (0.0f - (rightHeight - currentHeight)) * 8.0f;
                normalX[index + 1] = (0.0f - (rightHeight - currentHeight)) * 8.0f;

                // Calculate Y component of normal (derivative in Z direction)  
                // Based on height differences between bottom and current points
                normalY[index] = (bottomHeight - currentHeight) * 8.0f;
                normalY[index + 1] = (bottomRightHeight - rightHeight) * 8.0f;

                index += 2;
            }
        }

        // Sum up all the normal components
        float sumX = (normalX[6] + normalX[2] + normalX[5] + normalX[1]);
        float sumY = (normalY[6] + normalY[2] + normalY[5] + normalY[1]);
        float sumZ = (normalZ[6] + normalZ[2] + normalZ[5] + normalZ[1]);

        // Normalize the resulting vector
        float length = sqrt(sumX * sumX + sumY * sumY + sumZ * sumZ);
        float invLength = 1.0f / length;

        CVector result;
        result.x = invLength * sumX;
        result.y = invLength * sumY;
        result.z = invLength * sumZ;

        return result;
    }

    unsigned char Landscape::GetColor(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::SetPresenceOnCollisionMap(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::IsThisVisCellHasWater(int, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::drawSpriteOverlayed(unsigned, CVector const&, CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::ReloadLightmapTexture(CStr const& fileName)
    {
        if (m_texLightmap.IsValid())
        {
            M3D_RENDERER->ReleaseTexture(m_texLightmap);
        }

        m_texLightmap = M3D_RENDERER->AddTexture(fileName, 2);
        if (!m_texLightmap.IsValid())
        {
            m_texLightmap = M3D_RENDERER->AddTexture("data\\grid.dds", 2);
        }

        M3D_RENDERER->SetTextureParameter(m_texLightmap, rend::TM_WRAP_S, 1);
        M3D_RENDERER->SetTextureParameter(m_texLightmap, rend::TM_WRAP_T, 1);

        // TODO: check this
        auto dbgFloat = M3D_ENGINE_CFG.m_dbg_floatVar2.GetF();
        M3D_RENDERER->SetTextureParameter(m_texLightmap, rend::TM_MIP_LOD_BIAS, dbgFloat);
    }

    Object* Landscape::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::LoadShoreLine(CStr const&)
    {
        // TODO: implement Landscape::LoadShoreLine
        //RETRUXX_NOT_IMPLEMENTED;
        return true;
    }

    void Landscape::GetFogStartAndEnd(float& s, float& e) const
    {
        auto m_f = m3d::g_Kernel->GetEngineCfg().m_lsViewDistanceDivider.GetF();
        auto v6 = (int)(float)((float)(m_f * 8.0) + 4.0);
        if (v6 >= 4)
        {
            if (v6 > 12)
                v6 = 12;
        }
        else
        {
            v6 = 4;
        }
        auto v8 = VISCELL_EDGE_LENGTH_24;
        auto v9 = (float)v6;
        if (m_curVisMode == VIS_REFLECTION)
        {
            s = (float)(v9 * 0.16666667) * VISCELL_EDGE_LENGTH_24;
            e = (float)(v9 - 0.80000001) * v8;
        }
        else if (m_curVisMode == VIS_REFRACTION)
        {
            s = (float)(v9 - 0.30000001) * VISCELL_EDGE_LENGTH_24;
            e = (float)(v9 - 0.050000001) * v8;
        }
        else
        {
            s = (float)(v9 * 0.16666667) * VISCELL_EDGE_LENGTH_24;
            e = (float)(v9 - 0.80000001) * v8;
        }
    }

    int Landscape::GetNumAlphas() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::CreateLod()
    {
        const auto land_size = m_owner->m_level->land_size;

        delete[] m_cellParams;
        m_cellParams = new CellParams[land_size * land_size];
        CreateHeights(m_cellParams, land_size, 16);

        const auto drawedCellSize = 4 * m_owner->m_level->land_size;
        delete[] m_drawedCellParams;
        m_drawedCellParams = new CellParams[drawedCellSize * drawedCellSize];
        CreateHeights(m_drawedCellParams, drawedCellSize, 4);
    }

    void Landscape::drawSpriteOverlayedProjected(unsigned, CVector const&, CMatrix const&, CClipper const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::CreateHeights(CellParams* dest, int ls, int cellSize)
    {
        std::unordered_map<int, int> counterForHeights;
        std::set<int> usedHeights;
        float maxCounts = 999999.0;
        for (int z =0; z < ls; ++z)
        {
            for (int x = 0; x < ls; ++x)
            {
                float v11 = 999999.0f;
                float z_max = -999999.0f;
                float z_min = 999999.0f;
                float wmax = -999999.0f;
                float wmin = 999999.0f;

                for (int yi = 0; yi < cellSize; ++yi)
                {
                    for (int xi = 0; xi < cellSize; ++xi)
                    {
                        auto idx = (xi + x * cellSize) + (yi + z * cellSize) * (m_mapSize + 1);
                        auto height = m_heightMap[idx];

                        if (height > z_max)
                        {
                            z_max = height + 64.0f;
                        }
                        if (z_min > height)
                        {
                            z_min = height - 64.0f;
                        }

                        if (cellSize == 4 && m_waterMap[4 * z * m_owner->m_level->land_size + x])
                        {
                            auto waterHeight = getWaterHeight(x, z);

                            int key = (int)(waterHeight * 8.333334);
                            counterForHeights[key]++;
                            usedHeights.insert(key);

                            if (waterHeight > wmax)
                                wmax = waterHeight;
                            if (wmin > waterHeight)
                                wmin = waterHeight;

                            auto v6 = z;
                            auto v9 = x;
                            auto v5 = v9 / 4 + m_owner->m_level->land_size * (v6 / 4);
                            m_cellParams[v5].m_iswatercell = true;
                        }

                    }
                }
                if (maxCounts > wmin)
                    maxCounts = wmin;

                auto& cellParams = dest[x + z * ls];
                cellParams.m_h0 = z_min;
                cellParams.m_h1 = z_max;

                if (cellSize == 4)
                {
                    // TODO: check this
                    auto v5 = x / 4 + m_owner->m_level->land_size * (z / 4);
                    auto v22 = v5;
                    m_cellParams[v22].m_minwater = wmin;
                    m_cellParams[v22].m_maxwater = wmax;
                }
            }
        }

        auto heightCandidate = 0;
        auto heightMaxCount = 0;
        for (auto& height : usedHeights)
        {
            auto count = counterForHeights[height];
            if (count > heightMaxCount)
            {
                heightMaxCount = count;
                heightCandidate = height;
            }
        }

        m_owner->m_level->waterlevel = heightCandidate * 0.12;
    }

    int Landscape::isWaterCell(int, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::Restore()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Landscape::getWaterHeight(int x, int z) const
    {
        auto v3 = 4 * this->m_owner->m_level->land_size;
        if (x < 0 || x >= v3 || z < 0 || z >= v3)
            return 0.0;
        else
            return (double)this->m_waterMap[x + z * v3] * 0.12;
    }

    void Landscape::ReleaseReflectionRefractionTextures()
    {
        if (this->m_texRtReflection.IsValid())
            M3D_RENDERER->ReleaseTexture(m_texRtReflection);
        if (this->m_texRtRefraction.IsValid())
            M3D_RENDERER->ReleaseTexture(m_texRtRefraction);
        if (this->m_waveBumpTex.IsValid())
            M3D_RENDERER->ReleaseTexture(m_waveBumpTex);
        if (this->m_fresnelTex.IsValid())
            M3D_RENDERER->ReleaseTexture(m_fresnelTex);
        if (this->m_waveBumpSmTex.IsValid())
            M3D_RENDERER->ReleaseTexture(m_waveBumpSmTex);

        if (m_waterVs)
        {
            m_waterVs->Release();
            this->m_waterVs = 0;
        }

        if (m_waterPs)
        {
            m_waterPs->Release();
            this->m_waterPs = 0;
        }

        if (m_waterDumbVs)
        {
            m_waterDumbVs->Release();
            this->m_waterDumbVs = 0;
        }

        if (m_waterDumbPs)
        {
            m_waterDumbPs->Release();
            this->m_waterDumbPs = 0;
        }

        if (waterPs)
        {
            waterPs->Release();
            waterPs = 0;
        }

        if (m_solidDeepVs)
        {
            m_solidDeepVs->Release();
            this->m_solidDeepVs = 0;
        }

        if (m_solidDeepPs)
        {
            m_solidDeepPs->Release();
            this->m_solidDeepPs = 0;
        }

    }

    char const* Landscape::GetGrassModelName(unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::renderZGuard()
    {
        // TODO: check this!!!!
        M3D_RENDERER->SetAlphaTest(0);
        M3D_RENDERER->SetColorWriteMask(0, false);
        M3D_RENDERER->PushZbState(rend::ZB_ENABLE);
        M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
        M3D_RENDERER->PushBlend(rend::BM_NONE);
        M3D_RENDERER->DisableTextureStages(0);

        auto vbHandle = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZC);

        int x = 0;
        int y = 0;
        int z = 0;

        auto* stream = (char*)M3D_RENDERER->LockVbStreaming(vbHandle, x, y, &z);

        m3d::rend::VertexXYZC poly[4];
        poly[3].c = 0xFFFFFF;
        poly[2].c = 0xFFFFFF;
        poly[1].c = 0xFFFFFF;
        poly[0].c = 0xFFFFFF;

        CVector r;
        CVector u;
        CVector f;
        M3D_RENDERER->MatGetBasis(r, u, f);

        CVector org;

        auto v13 = 5000.0;

        if (fabs(f.y) < 0.89999998)
        {
            auto v37 = 1.0 / sqrt(f.x * f.x + f.z * f.z + 0.00000011920929);
            f.x = f.x * v37;
            f.y = v37 * 0.0;
            f.z = f.z * v37;
            org = M3D_RENDERER->MatGetOrgInv();
            auto v10 = (float)((float)((float)this->m_drawRadius - 0.76999998) * VISCELL_EDGE_LENGTH_24) * f.y;
            auto v11 = org.x + (float)((float)((float)((float)this->m_drawRadius - 0.76999998) * VISCELL_EDGE_LENGTH_24) * f.x);
            auto v12 = org.z + (float)(f.z * (float)((float)((float)this->m_drawRadius - 0.76999998) * VISCELL_EDGE_LENGTH_24));
            r.z = f.x - (float)(f.y * 0.0);
            auto v41 = r.z * 10000.0;
            r.x = (float)(f.y * 0.0) - f.z;
            r.y = (float)(f.z * 0.0) - (float)(f.x * 0.0);
            poly[0].x = v11 - (float)(r.x * 10000.0);
            poly[0].z = v12 - (float)(r.z * 10000.0);
            poly[0].y = v10 - (float)(r.y * 10000.0);
            org.z = (float)(r.z * 10000.0) + v12;
            poly[1].x = (float)(r.x * 10000.0) + v11;
            poly[1].z = org.z;
            poly[1].y = (float)(r.y * 10000.0) + v10;
            org.x = poly[1].x;
            org.y = poly[1].y + 5000.0;
            poly[2].x = poly[1].x;
            poly[2].y = poly[1].y + 5000.0;
            poly[2].z = org.z;
            poly[3].x = poly[0].x;
            poly[3].y = poly[0].y + 5000.0;
            poly[3].z = poly[0].z;
            memcpy(stream, poly, 0x40u);
            stream += 64;
        }

        const auto land_scale_27 = 8.0;
        CVector v[4];

        auto v14 = (float)((float)this->m_owner->m_level->land_size * VISCELL_EDGE_LENGTH_24) - 8.0;
        v[0].x = land_scale_27;
        v[0].y = 0.0;
        v[0].z = land_scale_27;
        v[1].x = v14;
        auto v15 = v14;
        v[1].y = 0.0;
        v[1].z = land_scale_27;
        poly[1].z = land_scale_27;
        v[2].x = v14;
        poly[1].y = v13 + 0.0;
        v[2].z = v14;
        auto v44 = v14;
        auto v41 = v14;
        poly[2].x = v14;
        v[2].y = 0.0;
        auto v16 = v13 + 0.0;
        poly[2].y = v13 + 0.0;
        auto v17 = v14;
        poly[2].z = v14;
        v[3].x = land_scale_27;
        v[3].y = 0.0;
        auto v18 = v13 + 0.0;
        auto v19 = v13 + 0.0;
        v[3].z = v17;
        poly[0].x = land_scale_27;
        poly[0].y = v13 + 0.0;
        poly[0].z = land_scale_27;
        poly[1].x = v15;
        poly[3].x = land_scale_27;
        poly[3].y = v13 + 0.0;
        poly[3].z = v17;
        memcpy(stream, poly, 0x40u);
        poly[0].x = land_scale_27;
        poly[0].y = v[0].y;
        poly[0].z = land_scale_27;
        poly[1].x = v15;
        poly[1].y = v[1].y;
        poly[1].z = v[1].z;
        poly[2].x = v15;
        poly[2].y = v13 + 0.0;
        poly[2].z = land_scale_27;
        poly[3].x = land_scale_27;
        auto v20 = stream + 64;
        poly[3].y = v13 + 0.0;
        poly[3].z = land_scale_27;
        memcpy(v20, poly, 0x40u);
        poly[0].x = v15;
        poly[0].y = v[1].y;
        poly[0].z = v[1].z;
        poly[1].x = v[2].x;
        poly[1].y = v[2].y;
        poly[1].z = v[2].z;
        poly[2].x = v41;
        poly[2].y = v13 + 0.0;
        poly[2].z = v44;
        v20 += 64;
        poly[3].y = v13 + 0.0;
        x = v[2].x;
        poly[3].z = land_scale_27;
        y = v[3].y;
        poly[3].x = v15;
        memcpy(v20, poly, 0x40u);
        poly[0].x = x;
        poly[0].y = v[2].y;
        poly[0].z = v[2].z;
        auto v23 = v[3].x;
        poly[1].y = y;
        v20 += 64;
        poly[1].x = v[3].x;
        poly[1].z = v[3].z;
        org.y = v19;
        z = v[3].z;
        poly[2].x = v[3].x;
        poly[3].y = v16;
        auto v24 = v[3].y;
        poly[2].y = v19;
        poly[2].z = v[3].z;
        poly[3].x = v41;
        poly[3].z = v44;
        memcpy(v20, poly, 0x40u);
        poly[0].y = v24;
        poly[0].z = v[3].z;
        poly[1].x = land_scale_27;
        poly[1].y = v[0].y;
        poly[2].y = v18;
        poly[0].x = v23;
        poly[1].z = land_scale_27;
        poly[2].x = land_scale_27;
        poly[2].z = land_scale_27;
        poly[3].x = v23;
        poly[3].y = v19;
        poly[3].z = z;
        memcpy(v20 + 64, poly, 0x40u);

        auto v28 = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZC);
        M3D_RENDERER->UnlockVb(v28);

        auto v32 = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZC);
        M3D_RENDERER->SetToStream0(v32);

        unsigned vofs = 0;

        auto v33 = 0;
        if (fabs(f.y) < 0.89999998)
        {
            M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN,
                vofs,
                2u);
            v33 = 4;
        }

        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN,
            v33 + vofs,
            2u);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN,
            v33 + vofs + 4,
            2u);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN,
            v33 + vofs + 8,
            2u);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN,
            v33 + vofs + 12,
            2u);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN,
            v33 + vofs + 16,
            2u);
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->SetColorWriteMask(15u, false);
    }

    void Landscape::GetWaterCellHeights(float&, float&, int, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::ManageLandScapeCollisionTriMeshes()
    {
        using namespace ai;

        const auto landSize = m_owner->m_level->land_size;
        retruxx::set<ai::PhysicObj*> allPhysicObjs;

        // TODO: check this
        for (int y = 0; y < landSize; ++y)
        {
            for (int x = 0; x < landSize; ++x)
            {
                auto* collisionItem = m_oCollisionitems[x + y * landSize];
                const auto mustCheck = collisionItem->m_bMustCheck;
                collisionItem->m_bMustCheck = false;
                if (mustCheck)
                {
                    bool isCellEnabled = false;
                    for (const auto objId : collisionItem->m_physicObjIds)
                    {
                        auto* obj = theObjects->GetEntityByObjId(objId);
                        if (obj)
                        {
                            if (IS_KIND_OF(obj, PhysicObj))
                            {
                                auto* physObj = RT_DYNCAST(obj, PhysicObj);
                                allPhysicObjs.insert(physObj);
                                if (physObj->bIsUpdatingByODE())
                                {
                                    isCellEnabled = true;
                                    break;
                                }
                            }
                            else
                            {
                                M3D_LOG_INFO("Error: not PhysicObj is linked to collision cell x = " + CStr(x) + ", y = " + CStr(y) + ", id = " + CStr(objId));
                            }
                        }
                    }

                    if (isCellEnabled != collisionItem->m_wasEnabledLastFrame)
                    {
                        if (isCellEnabled)
                        {
                            for (auto* geom : collisionItem->m_geomsList)
                            {
                                geom->IncEnabledCellsCount();
                            }

                            for (const auto objId : collisionItem->m_physicObjIds)
                            {
                                auto* physObj = RT_DYNCAST(theObjects->GetEntityByObjId(objId), PhysicObj);
                                const auto physicState = physObj->GetPhysicState();
                                if ((physicState & 1) == 0 && (physicState & 2) != 0)
                                {
                                    physObj->IncEnabledCellsCount();
                                }
                            }
                        }
                        else
                        {
                            for (auto* geom : collisionItem->m_geomsList)
                            {
                                geom->DecEnabledCellsCount();
                            }

                            for (const auto objId : collisionItem->m_physicObjIds)
                            {
                                auto* physObj = RT_DYNCAST(theObjects->GetEntityByObjId(objId), PhysicObj);
                                const auto physicState = physObj->GetPhysicState();
                                if ((physicState & 1) == 0 && (physicState & 2) != 0)
                                {
                                    physObj->DecEnabledCellsCount();
                                }
                            }
                        }
                        collisionItem->m_wasEnabledLastFrame = isCellEnabled;
                    }
                }
            }
        }

        m_countPhysicObjsInCells->SetI(allPhysicObjs.size());
    }

    void Landscape::LinkPassMapCellToCollisionCell(const PointBase<int>& cellPos)
    {
        // TODO: implement Landscape::LinkPassMapCellToCollisionCell
        // RETRUXX_NOT_IMPLEMENTED;
        //const auto landSize = m_owner->m_level->land_size;
        //const auto v7 = ((landSize * 128.0) / (4 * landSize)) * 0.5;
        //const auto x = (cellPos.x + 0.5) * v7;
        //const auto z = (cellPos.y + 0.5) * v7;
        //const auto lsHeight = GetLsHeight(x, z);
        //
        //const float VISCELL_EDGE_LENGTH = 128.0;
        //auto v8 = (int)((z - (v7 * 0.5)) * (1.0 / VISCELL_EDGE_LENGTH));
        //v8 = std::clamp(v8, 0, landSize - 1);
        //
        //auto* passCell = M3D_KERNEL->New("GeomObjectPassCell");
        //auto* odeSpace = m_owner->GetOdeSpace();
        //auto* box = dCreateBox(odeSpace, lsHeight, 50.0, lsHeight);
    }

    void Landscape::ReBuildShoresVb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::LoadTiles(CStr const& filename)
    {
        // TODO: generated code (looks ok)
        // Free existing tiles if any
        FreeTiles();

        // Calculate land size and allocate memory for tiles
        const int landSize = 4 * m_owner->m_level->land_size;
        const int totalTiles = landSize * landSize;

        // Allocate memory for tile info
        m_tiles = new TileInfo[totalTiles];

        // Open the tagged file
        fs::auxTaggedFile file;
        int openResult = file.Open(filename.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC);

        if (openResult != fs::auxTaggedFile::eError::BAD_FORMAT)
        {
            if (openResult != fs::auxTaggedFile::eError::SUCCESS)
            {
                // Log error
                M3D_LOG_ERR("Cannot open tiles file " + filename);
                return 0;
            }
        }
        else
        {
            M3D_LOG_ERR("Tiles bad format " + filename);
            return 0;
        }

        // Check file format
        char* formatTitle = nullptr;
        file.getFormatTitle(&formatTitle);
        if (strcmp(formatTitle, "TILEMAP") != 0)
        {
            M3D_LOG_ERR("Error: Bad tiles format title: " + CStr(formatTitle));
            return 0;
        }

        // Check format version
        unsigned int formatVersion = 0;
        file.getFormatVersion(formatVersion);
        if (formatVersion != 1)
        {
            M3D_LOG_ERR("Error: Wrong tiles format version: " + CStr(formatVersion));
            return 0;
        }

        // Process the tile data
        unsigned char* data = nullptr;
        if (file.getChunkData(0xBADF00Du, reinterpret_cast<void**>(&data)))
        {
            return 0;
        }

        // Verify land size matches
        unsigned int fileLandSize = *reinterpret_cast<unsigned int*>(data);
        data += 4;

        if (fileLandSize != landSize)
        {
            M3D_LOG_ERR("Error: Bad tilemap file, landsize = " + CStr(landSize) + ", tilemap size in file = " + CStr(fileLandSize));
            return 0;
        }

        // Read tile path
        unsigned int pathLength = *reinterpret_cast<unsigned int*>(data);
        data += 4;
        m_pathTile = CStr(reinterpret_cast<char*>(data));
        data += pathLength + 1;

        // Ensure path ends with backslash
        if (m_pathTile[m_pathTile.length() - 1] != '\\')
        {
            m_pathTile += "\\";
        }

        // Read tile names
        unsigned int tileNameCount = *reinterpret_cast<unsigned int*>(data);
        data += 4;

        std::vector<CStr> tileNames;
        tileNames.reserve(tileNameCount);
        for (unsigned int i = 0; i < tileNameCount; ++i)
        {
            unsigned int nameLength = *reinterpret_cast<unsigned int*>(data);
            data += 4;

            CStr tileName(reinterpret_cast<char*>(data));
            tileNames.push_back(std::move(tileName));
            data += nameLength + 1;

            // Add to used textures set
            m_usedTexturesList.insert(tileNames.back());
        }

        // Read tile info
        ReadTileInfo(1); // Initialize tile info

        // Process each tile
        for (int y = 0; y < landSize; ++y)
        {
            for (int x = 0; x < landSize; ++x)
            {
                TileInfo& tile = m_tiles[y * landSize + x];

                // Read tile data (2 bytes: index, 2 bytes: angle)
                unsigned short tileIndex = *reinterpret_cast<unsigned short*>(data);
                unsigned short tileAngle = *reinterpret_cast<unsigned short*>(data + 2);
                data += 4;

                // Clamp angle
                if (tileAngle > 4) tileAngle = 4;
                tile.m_angle = tileAngle;

                // Create full texture path and add texture
                CStr texturePath = m_pathTile + tileNames[tileIndex];
                tile.m_texIndex0 = AddOneTexture(texturePath);
            }
        }

        // Clean up
        file.Close();

        // Update texture information
        const size_t textureCount = m_tilesTextures.size();
        ChangedNumberOfUsedTextures(textureCount);
        RecalcUV();
        UpdateTexturesFilters();

        return 1;
    }

    bool Landscape::traceLineThruCellLs(float&, int, int, CVector const&, CVector const&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::RecursiveDisableShore(unsigned char*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Landscape::Landscape(Landscape const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int CreateIndices(uint16_t* idxes, int szindex, int szvertex, int step)
    {
        // looks ok
        auto v5 = szvertex + 1;
        auto v6 = idxes;
        auto v7 = 0;
        auto v8 = step * (szvertex + 1);
        auto v14 = szindex;
        auto v15 = 64;
        bool v12 = false;
        do
        {
            if (szindex > 0)
            {
                auto v9 = szindex + 1;
                auto szvertexa = szindex;
                do
                {
                    for (int i = 0; i < v9; v6 += 2)
                    {
                        *v6 = v8 + v7;
                        v6[1] = v7;
                        if (i == v14)
                            v7 += v8 - szvertex;
                        else
                            v7 += step;
                        ++i;
                    }
                    *v6 = szvertex + v7 - v8;
                    v5 = szvertex + 1;
                    v6[1] = szvertex + 1 + v7;
                    v6 += 2;
                    --szvertexa;
                } while (szvertexa);
                szindex = v14;
            }
            auto v11 = v7 + v8 + szvertex + 1;
            v7 += v5;
            v12 = v15-- == 1;
            *(v6 - 1) = v11;
        } while (!v12);
        return (v6 - idxes) / 64;
    }

    struct VertexWaterTest
    {
        __int16 x;
        __int16 y;
        __int16 z;
        __int16 w;
    };

    Landscape::Landscape()
    {
        // Set up lockVis CVar
        m_lockVis = CVar("lockVis", "0", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);

        // Initialize pointers
        m_numGrassModels = 0;
        m_grassArray = nullptr;
        m_heightMap = nullptr;
        m_cliffHeightMap = nullptr;
        m_waterMap = nullptr;
        m_normalMap = nullptr;
        m_vnormal = nullptr;
        m_colormap = nullptr;
        m_texSetsmap = nullptr;
        m_remappedIndices = nullptr;
        m_oCollisionitems = nullptr;
        m_terrainObject = nullptr;
        m_owner = nullptr;
        m_cellParams = nullptr;
        m_drawedCellParams = nullptr;
        m_lastState = RT_LIGHTPASS;
        m_tiles = nullptr;

        // Create vertex buffers
        m_landUVVb = M3D_RENDERER->AddVb(rend::VertexType::VERTEX_STREAM_UV_S1, 0xFFFF, "LandUV", 0);
        m_landVb = M3D_RENDERER->AddVb(rend::VertexType::VERTEX_XYZNCT2, 1600, "LandEditor", 512);

        // Allocate dummy VB
        m_dummyVB = new rend::VertexXYZNCT2[1600];

        // Reserve space for water cells
        for (int i = 0; i < 16; i++)
        {
            waterCellsToDraw[i].reserve(100);
        }

        // Create water queries
        for (int i = 0; i < 3; i++)
        {
            m_waterQueries[i] = M3D_RENDERER->NewQuery(rend::IQuery::Type::QUERY_OCCLUSION); // OCCLUSION query type
        }

        // Initialize water state
        m_isWaterVisible = false;
        m_currWaterQuery = 0;
        m_drawtextured = true;
        m_passedCells = 0;
        m_CurAlphaSet = 0;
        m_loadAllTextures = false;

        // Register CVar with console
        M3D_KERNEL->GetEngineCfg().m_console->RegisterCVar(&m_lockVis, nullptr);

        // Create LOD index buffers
        unsigned int lodFactor = 1;
        m_maxLOD = 0;

        // loop looks ok
        for (int size = 4; size > 0; size >>= 1)
        {
            // Create index buffer for this LOD level
            int numIndices = ((size + 1) * (size + 1)) << 9;
            auto curIb = M3D_RENDERER->AddIb(numIndices, false);

            // Lock and fill index buffer
            unsigned short* v16 = (unsigned short*)M3D_RENDERER->LockIb(curIb, 384 * size * size, 0, 0);
            int createdIndices = CreateIndices(v16, size, 4, lodFactor);
            M3D_RENDERER->UnlockIb(curIb);

            // Store number of indices
            m_lsNumIndices.push_back(createdIndices);

            // Store IB handle
            m_landIbConst.push_back(curIb);

            // Update for next LOD level
            lodFactor *= 2;
            m_maxLOD++;
        }

        // Calculate max water cells per pass based on vertex shader constants
        m_maxWaterCellPerPass = M3D_RENDERER->GetMaxVertexShaderConst() - 20;

        // Create water vertex buffer
        m_waterVb = M3D_RENDERER->AddVb(m3d::rend::VertexType::VERTEX_WATERTEST, 81 * m_maxWaterCellPerPass, "Water", 0);

        // Initialize water vertices
        // looks ok
        VertexWaterTest* waterVertices = static_cast<VertexWaterTest*>(M3D_RENDERER->LockVb(m_waterVb, 0, 0, 0));
        constexpr uint8_t gridSize = 9;  // Since loops go up to 8 (0-8 inclusive)
        for (uint8_t zCoord = 0; zCoord < m_maxWaterCellPerPass; ++zCoord)
        {
            for (uint8_t yCoord = 0; yCoord < gridSize; ++yCoord)
            {
                for (uint8_t xCoord = 0; xCoord < gridSize; ++xCoord)
                {
                    // Copy vertex data
                    waterVertices->x = xCoord;
                    waterVertices->y = yCoord;
                    waterVertices->z = zCoord;
                    waterVertices->w = 0;
                    ++waterVertices;
                }
            }
        }
        M3D_RENDERER->UnlockVb(m_waterVb);

        // Allocate water tile info
        waterTileInfo = new CVector4[m_maxWaterCellPerPass];

        // Create water index buffers for 16 LOD levels (4 patterns x 4 levels each)
        // First loop (4 iterations)

        // This loop looks OK
        int i = 0;
        int v46 = 0;
        do
        {
            int v31 = 1 << i;
            int v32 = (unsigned __int16)(1 << i);
            int v35 = 16 * (8 / v32 + 2) / v32 * this->m_maxWaterCellPerPass;
            int _Val = 16 * (8 / v32 + 2) / v32;

            m_waterIb[i] = M3D_RENDERER->AddIb(v35, 0);
            unsigned short* v38 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[i], v35, 0, 0);
            int v39 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            int n = 0;
            if (!v40)
            {
                do
                {
                    int v192 = 0;
                    int v43 = 0;
                    do
                    {
                        int v41 = 0;
                        do
                        {
                            int v42 = v39 + 9 * v31;
                            *v38 = v39;
                            v38[1] = v42;
                            if (v41 == 8)
                                v39 = v42 - 8;
                            else
                                v39 += v31;
                            v41 += v32;
                            v38 += 2;
                        } while (v41 <= 8);
                        *v38 = v39 + 8;
                        v43 = v32 + v192;
                        v38[1] = v39;
                        v38 += 2;
                        v192 = v43;
                    } while (v43 < 8);
                    v39 += 9;
                    int v44 = n + 1;
                    *(v38 - 1) = v39;
                    v40 = v44 < this->m_maxWaterCellPerPass;
                    n = v44;
                } while (v40);
            }

            M3D_RENDERER->UnlockIb(m_waterIb[i]);
            m_wtNumTris[i] = _Val;
            v46 = ++i < 4;

        } while (v46);

        i = 1;
        int curIbIdx = 6;
        do
        {
            int v47 = 1 << i;
            int v48 = (unsigned __int16)(1 << i);
            int v51 = 16 * (8 / v48 + 3) / v48 * this->m_maxWaterCellPerPass;
            int v206 = 16 * (8 / v48 + 3) / v48;

            m_waterIb[curIbIdx] = M3D_RENDERER->AddIb(v51, 0);
            unsigned short* v54 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[curIbIdx], v51, 0, 0);

            int v55 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            int v192 = 0;
            if (!v40)
            {
                int _Val = (unsigned __int16)v47 >> 1;
                int step = 10 * v47;
                do
                {
                    int v56 = 0;
                    do
                    {
                        v54[1] = v55 + 9 * v47;
                        v54[2] = v55 + _Val;
                        int v57 = v55 + step;
                        *v54 = v55;
                        v54[3] = v57;
                        v56 += v48;
                        v55 += v47;
                        v54 += 4;
                    } while (v56 < 8);
                    *v54 = v55;
                    int v58 = 9 * v47 + v55;
                    v54[1] = v58;
                    v54[2] = v58;
                    auto v59 = v54 + 3;
                    int v60 = v58 - 8;
                    *v59 = v60;
                    v54 = v59 + 1;
                    int n = v48;
                    if (v48 < 8)
                    {
                        int v63 = 0;
                        do
                        {
                            int v61 = 0;
                            do
                            {
                                int v62 = v60 + 9 * v47;
                                *v54 = v60;
                                v54[1] = v62;
                                if (v61 == 8)
                                    v60 = v62 - 8;
                                else
                                    v60 += v47;
                                v61 += v48;
                                v54 += 2;
                            } while (v61 <= 8);
                            *v54 = v60 + 8;
                            v63 = v48 + n;
                            v54[1] = v60;
                            v54 += 2;
                            n = v63;
                        } while (v63 < 8);
                    }
                    v55 = v60 + 9;
                    int v64 = v192 + 1;
                    *(v54 - 1) = v55;
                    v40 = v64 < this->m_maxWaterCellPerPass;
                    v192 = v64;
                } while (v40);
            }

            M3D_RENDERER->UnlockIb(m_waterIb[curIbIdx]);
            m_wtNumTris[curIbIdx] = v206;
            curIbIdx += 4;
            v46 = ++i < 4;
        } while (v46);

        curIbIdx = 5;
        i = 1;
        do
        {
            int v66 = 1 << i;
            int v67 = (unsigned __int16)(1 << i);
            int v69 = (16 * (8 / v67 + 2) / v67 + 24 / v67) * this->m_maxWaterCellPerPass;
            int v206 = 16 * (8 / v67 + 2) / v67 + 24 / v67;

            m_waterIb[curIbIdx] = M3D_RENDERER->AddIb(v69, 0);
            unsigned short* v72 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[curIbIdx], v69, 0, 0);

            int v73 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            unsigned short* v74 = v72;
            int v192 = 0;
            if (!v40)
            {
                int _Val = 9 * v67 / 2;
                int step = 10 * v66;
                do
                {
                    int n = 0;
                    int v79 = 0;
                    do
                    {
                        v74[1] = v73 + _Val;
                        v74[3] = v73 + 9 * v66;
                        int v75 = v73 + step;
                        *v74 = v73;
                        v74[2] = v73 + v66;
                        v73 += v66;
                        v74[4] = v75;
                        int v76 = v67;

                        unsigned short* m = v74 + 5;
                        for (; v76 <= 8; m += 2)
                        {
                            int v78 = v73 + 9 * v66;
                            *m = v73;
                            m[1] = v78;
                            if (v76 == 8)
                                v73 = v78 - 8;
                            else
                                v73 += v66;
                            v76 += v67;
                        }
                        v79 = v67 + n;
                        *m = v73 + 8;
                        m[1] = v73;
                        v74 = m + 2;
                        n = v79;
                    } while (v79 < 8);
                    v73 += 9;
                    int v80 = v192 + 1;
                    *(v74 - 1) = v73;
                    v40 = v80 < this->m_maxWaterCellPerPass;
                    v192 = v80;
                } while (v40);
            }

            M3D_RENDERER->UnlockIb(m_waterIb[curIbIdx]);
            m_wtNumTris[curIbIdx] = v206;
            curIbIdx += 4;
            v46 = ++i < 4;
        } while (v46);

        i = 1;
        curIbIdx = 4;
        do
        {
            int v82 = 1 << i;
            __int16 v83 = (unsigned __int16)(1 << i);
            int step = 1 << i;

            int v86 = (16 * (8 / v83 + 2) / v83 + 24 / v83) * m_maxWaterCellPerPass;
            int formal = 16 * (8 / v83 + 2) / v83 + 24 / v83;

            m_waterIb[curIbIdx] = M3D_RENDERER->AddIb(v86, 0);
            unsigned short* v89 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[curIbIdx], v86, 0, 0);

            int v90 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            int _Val = 0;
            if (!v40)
            {
                auto v86 = (unsigned __int16)v82 >> 1;
                int v206 = 8 - v83;
                i = 9 * v82;
                int v208 = 9 * v82 + v86;
                int v207 = 10 * v82;
                while (1)
                {
                    int n = 0;
                    if (v206 > 0)
                    {
                        int v93 = 0;
                        do
                        {
                            int v91 = 0;
                            do
                            {
                                int v92 = v90 + i;
                                *v89 = v90;
                                v89[1] = v92;
                                if (v91 == 8)
                                    v90 = v92 - 8;
                                else
                                    v90 += v82;
                                v91 += v83;
                                v89 += 2;
                            } while (v91 <= 8);
                            *v89 = v90 + 8;
                            v93 = v83 + n;
                            v89[1] = v90;
                            v89 += 2;
                            n = v93;
                        } while (v93 < v206);
                    }
                    int v94 = 0;
                    int v95 = 0;
                    while (1)
                    {
                        v89[1] = v90 + i;
                        v95 = v90 + v82;
                        v89[3] = v90 + v208;
                        int v96 = v207;
                        *v89 = v90;
                        v89[4] = v96 + v90;
                        v89[2] = v95;
                        v94 += v83;
                        v89 += 5;
                        v90 = v95;
                        if (v94 >= 8)
                            break;
                        v82 = step;
                    }
                    int v97 = i;
                    int v98 = v95 + i;
                    v89[1] = v90 + i;
                    v89[2] = v98;
                    *v89 = v90;
                    v90 += v97 + 1;
                    int v99 = _Val;
                    auto v100 = v89 + 3;
                    *v100 = v90;
                    ++v99;
                    v89 = v100 + 1;
                    v40 = v99 < this->m_maxWaterCellPerPass;
                    _Val = v99;
                    if (!v40)
                        break;
                    v82 = step;
                }
            }

            M3D_RENDERER->UnlockIb(m_waterIb[curIbIdx]);
            m_wtNumTris[curIbIdx] = formal;
            curIbIdx += 4;
            v46 = ++i < 4;
        } while (v46);

        i = 1;
        curIbIdx = 7;
        do
        {
            int v102 = 1 <<i;
            auto v103 = (unsigned __int16)(1 << i);
            int v105 = (16 * (8 / v103 + 2) / v103 + 16 / v103) * this->m_maxWaterCellPerPass;
            int v208 = 16 * (8 / v103 + 2) / v103 + 16 / v103;

            m_waterIb[curIbIdx] = M3D_RENDERER->AddIb(v105, 0);
            unsigned short* v108 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[curIbIdx], v105, 0, 0);
           
            int v109 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            auto v110 = v108;
            int v206 = 0;
            if (!v40)
            {
                int _Val = 8 - v103;
                int formal = 9 * v103 / 2;
                int v207 = 10 * v102;
                do
                {
                    int v115 = 0;
                    for (int step = 0; step < 8; step = (unsigned __int16)v102 + v115)
                    {
                        for (int ii = 0; ii < _Val; v110 += 2)
                        {
                            int v112 = v109 + 9 * v102;
                            *v110 = v109;
                            v110[1] = v112;
                            if (ii == 8)
                                v109 = v112 - 8;
                            else
                                v109 += v102;
                            ii += v103;
                        }
                        v110[2] = v109 + v102;
                        int v113 = v109 + 9 * v102;
                        v110[1] = v113;
                        v110[4] = v113;
                        int v114 = v109 + v207;
                        v110[3] = formal + v109 + v102;
                        v115 = step;
                        *v110 = v109;
                        v110[5] = v114;
                        v110[6] = v114;
                        auto v116 = v110 + 7;
                        v103 = (unsigned __int16)v102;
                        v109 = v114 - 8;
                        *v116 = v114 - 8;
                        v110 = v116 + 1;
                    }
                    v109 += 9;
                    int v117 = v206 + 1;
                    *(v110 - 1) = v109;
                    v40 = v117 < this->m_maxWaterCellPerPass;
                    v206 = v117;
                } while (v40);
            }

            M3D_RENDERER->UnlockIb(m_waterIb[curIbIdx]);
            m_wtNumTris[curIbIdx] = v208;
            curIbIdx += 4;
            v46 = ++i < 4;
        } while (v46);

        // Setup profilers
        m_profilerUpdateVis = M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls update vis", 30));
        m_profilerDraw = M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls render", 30));
        m_profilerDrawGrass = M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls grass", 30));
        m_profilerDrawWater = M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls water", 30));

        // Setup debug counter
        m_countPhysicObjsInCells = M3D_APP->GetDbgCounterStack().GetCounter(M3D_APP->GetDbgCounterStack().AddCounter("PhysicObjs in cells"));

        // Initialize shaders to null
        m_solidDeepPs = nullptr;
        m_solidDeepVs = nullptr;
        m_waterPs = nullptr;
        m_waterVs = nullptr;
        m_waterDumbPs = nullptr;
        m_waterDumbVs = nullptr;

        // Load shores shader
        m_shoresShader = M3D_RENDERER->NewEffect("data/shaders/surf.fx", true);
        M3D_ASSERT(m_shoresShader);
        m_shoresShader->SetDefaultTechnique(true);

        // Load solid VS
        m_solidVs = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSolid_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_solidVs);

        // Load solid PS
        m_solidPs = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSolid_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_solidPs);

        // Load solid bind VS
        m_solidBindVs = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSolidBind_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_solidBindVs);

        // Load solid bind PS
        m_solidBindPs = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSolidBind_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_solidBindPs);

        // Load landscape VS
        m_landscapeVs = M3D_RENDERER->NewHlslShader("data/shaders/landscape_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_landscapeVs);

        // Load landscape FP PS
        m_landscapePsFP = M3D_RENDERER->NewHlslShader("data/shaders/landscapeFP_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_landscapePsFP);

        // Load landscape SP PS
        m_landscapePsSP = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSP_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_landscapePsSP);

        // Final null initializations
        overlayShader = nullptr;
        m_waterDumbVs = nullptr;
        m_waterDumbPs = nullptr;
    }

    void Landscape::BuildCells0(
        rend::VertexLandscape* vertices,
        TIVChunk& chunk,
        int& vertexOffset,
        cmn::vector<unsigned> const& cellsPerTexture,
        RenderTypes renderType,
        retruxx::vector<int, retruxx::allocator<int>>& bankSwitchingMap)
    {
        // TODO: generated code
        const int numCells = cellsPerTexture.size();
        const unsigned int* currentCell = &cellsPerTexture[0];

        if (numCells == 0)
        {
            chunk.iotherPassOffset = vertexOffset;
            return;
        }

        // State tracking variables
        int lastSquareX = -1;
        int lastSquareY = -1;
        int currentBank = chunk.iotherPassBankNumber;
        int cellsInCurrentPass = 0;
        int offsetToStore = vertexOffset;
        int totalVerticesProcessed = vertexOffset;

        const int CELLS_PER_TILE = 4;
        const int VERTICES_PER_CELL = 25;  // 5x5 grid
        const int MAX_VERTICES = 65535;    // 16-bit limit

        int remainingCells = numCells;

        while (remainingCells > 0)
        {
            // Determine how many cells to process in this batch
            int cellsToProcess = remainingCells;
            if (remainingCells < 0)
            {
                cellsToProcess = 0;
            }
            else if (remainingCells > 1)
            {
                cellsToProcess = 1;
            }

            int processedInBatch = cellsToProcess;

            // Process each cell in the current batch
            while (processedInBatch > 0)
            {
                const unsigned int cellData = *currentCell;

                // Extract cell information from packed data
                const unsigned char tileX = cellData & 0xFF;
                const unsigned char tileY = (cellData >> 8) & 0xFF;
                const int angle = (cellData >> 16) & 0x3;
                const int cornerType = (cellData >> 24) & 0xF;

                // Determine if we should process this cell based on render type
                bool shouldProcess = true;
                if (renderType != 0)
                {
                    // Alpha pass - only process if it's a special corner
                    if (cornerType == 0 || !specialMapper[cornerType].m_maskindex)
                    {
                        shouldProcess = false;
                    }
                }
                else
                {
                    // Base pass - skip special corners
                    if (cornerType != 0 && specialMapper[cornerType].m_maskindex)
                    {
                        shouldProcess = false;
                    }
                }

                if (shouldProcess)
                {
                    const int squareX = tileX / CELLS_PER_TILE;
                    const int squareY = tileY / CELLS_PER_TILE;
                    const int localX = tileX % CELLS_PER_TILE;
                    const int localY = tileY % CELLS_PER_TILE;

                    // Check if we're starting a new tile
                    if (squareX != lastSquareX || squareY != lastSquareY)
                    {
                        // Store previous tile data if we have a valid previous tile
                        if (lastSquareX != -1 && lastSquareY != -1)
                        {
                            const int storageShift = (renderType != 0) ? 4 : 0;

                            const char offsetShift = (4 * storageShift);
                            const char bankShift = (2 * storageShift);

                            assert(lastSquareX >= 0);
                            assert(lastSquareX < 64);
                            assert(lastSquareY >= 0);
                            assert(lastSquareY < 64);

                            // Store cell count for this tile
                            chunk.m_numCellsPerCellMap[lastSquareX + (lastSquareY * 64)] |= (cellsInCurrentPass << (2 *  storageShift));

                            const int tileIndex = lastSquareX  + (lastSquareY << 8);

                            // Store vertex offset
                            chunk.m_offsetsmap[tileIndex] |= (offsetToStore << offsetShift);

                            // Store bank number
                            chunk.m_banknumber[tileIndex] |= (currentBank << bankShift);
                        }

                        // Start new tile
                        lastSquareX = squareX;
                        lastSquareY = squareY;
                        cellsInCurrentPass = 0;
                        offsetToStore = vertexOffset;
                    }

                    ++cellsInCurrentPass;

                    // Get UV coordinates for the current angle
                    float* baseUVs = m_uvForAngles[angle][0];
                    float* alphaUVs = nullptr;

                    if (renderType == 1)
                    {
                        alphaUVs = m_setAndUVs.m_sets[m_CurAlphaSet][specialMapper[cornerType].m_maskindex].m_uvForAngles[specialMapper[cornerType].m_rotate][0];
                    }

                    const int mapStride = m_mapSize + 1;
                    const int heightMapBaseOffset = 4 * tileX + 4 * tileY * mapStride;

                    // Generate 5x5 grid of vertices (25 vertices total)
                    for (int row = 0; row < 5; ++row)
                    {
                        int verticesGenerated = 0;

                        if (renderType == 1)
                        {
                            // Alpha pass rendering
                            float* uvPtr = alphaUVs + 1;
                            auto vertexOffsetCalc = reinterpret_cast<char*>(vertices) - reinterpret_cast<char*>(alphaUVs);

                            for (int col = 0; col < 5; ++col)
                            {
                                // Calculate vertex index within the 32x32 vertex grid per tile
                                const short vertexIndex = localY + 32 * (col + 4 * localX);

                                // Store vertex index
                                vertices[verticesGenerated].xz = row + 4 * vertexIndex;

                                // Set vertex height from heightmap
                                vertices[verticesGenerated].y = m_heightMap[heightMapBaseOffset + col + row * mapStride];

                                // Pack UV coordinates
                                const float u = *uvPtr;
                                const float v = *(uvPtr - 1);
                                vertices[verticesGenerated].uv = static_cast<int>(u * 64.0f) - (static_cast<int>(v * -64.0f) << 9);

                                uvPtr += 2;
                                ++verticesGenerated;
                            }
                        }
                        else
                        {
                            // Base pass rendering
                            float* uvPtr = baseUVs + 1;
                            auto vertexOffsetCalc = reinterpret_cast<char*>(vertices) - reinterpret_cast<char*>(baseUVs);

                            for (int col = 0; col < 5; ++col)
                            {
                                // Calculate vertex index within the 32x32 vertex grid per tile
                                const short vertexIndex = localY + 32 * (col + 4 * localX);

                                // Store vertex index
                                vertices[verticesGenerated].xz = row + 4 * vertexIndex;

                                // Set vertex height from heightmap
                                vertices[verticesGenerated].y = m_heightMap[heightMapBaseOffset + col + row * mapStride];

                                // Pack UV coordinates
                                const float u = *uvPtr;
                                const float v = *(uvPtr - 1);
                                vertices[verticesGenerated].uv = static_cast<int>(u * 64.0f) - (static_cast<int>(v * -64.0f) << 9);

                                uvPtr += 2;
                                ++verticesGenerated;
                            }
                        }

                        // Move to next row
                        vertices += verticesGenerated;
                        baseUVs += 10;  // Move to next row in UV array (5 vertices * 2 floats)
                        if (alphaUVs)
                        {
                            alphaUVs += 10;
                        }
                    }

                    vertexOffset += VERTICES_PER_CELL;
                    totalVerticesProcessed += VERTICES_PER_CELL;

                    // Handle vertex buffer bank switching if we exceed 16-bit limit
                    if (vertexOffset > MAX_VERTICES)
                    {
                        // Add to bank switching map
                        bankSwitchingMap.push_back(offsetToStore);

                        vertexOffset -= offsetToStore;
                        ++currentBank;
                        offsetToStore = 0;
                    }
                }

                ++currentCell;
                --processedInBatch;
            }

            remainingCells -= cellsToProcess;
        }

        // Store final tile data if we have pending cells
        if (cellsInCurrentPass > 0)
        {
            const int storageShift = (renderType != 0) ? 4 : 0;
            const char offsetShift = (4 * storageShift);
            const char bankShift = (2 * storageShift);

            assert(lastSquareX >= 0);
            assert(lastSquareX < 64);
            assert(lastSquareY >= 0);
            assert(lastSquareY < 64);

            //const int tileIndex = lastSquareY * 64 + lastSquareX;

            // Store cell count for this tile
            chunk.m_numCellsPerCellMap[lastSquareY * 64 + lastSquareX] |= (cellsInCurrentPass << (2 * storageShift));

            const int tileIndex = lastSquareX + (lastSquareY << 8);

            // Store vertex offset
            chunk.m_offsetsmap[tileIndex] |= (offsetToStore << offsetShift);

            // Store bank number
            chunk.m_banknumber[tileIndex] |= (currentBank << bankShift);


            chunk.iotherPassBankNumber = currentBank;
            chunk.iotherPassOffset = totalVerticesProcessed;
        }
        else
        {
            chunk.iotherPassBankNumber = currentBank;
            chunk.iotherPassOffset = totalVerticesProcessed;
        }
    }

    void Landscape::DrawCellsFast0(cmn::vector<unsigned> const& cellsPerTex, TIVChunk& tivchunk, RenderTypes RenderType)
    {
        // TODO: generated code (looks ok)
        // Set up rendering state based on render type
        if (RenderType == RT_OTHERPASSES)
        {
            m3d::Application::g_pApp->m_renderer->SetBlend(rend::BM_ALPHA, 0);
            m3d::Application::g_pApp->m_renderer->SetAlphaTest(1);
            this->m_landscapePsSP->Apply();
        }
        else if (RenderType == RT_FIRSTPASSLIGHT)
        {
            m3d::Application::g_pApp->m_renderer->SetBlend(rend::BM_NONE, 0);
            m3d::Application::g_pApp->m_renderer->SetAlphaTest(0);
            this->m_landscapePsFP->Apply();
        }
        // Else case: no state changes for other render types

        this->m_lastState = RenderType;

        // Calculate shift values based on render type
        int typeShift = (RenderType != RT_FIRSTPASSLIGHT) ? 4 : 0;
        char cellCountShift = 2 * typeShift;      // Shift for cell count extraction
        char offsetShift = 4 * typeShift;         // Shift for offset extraction

        // Process each cell in the texture
        for (unsigned int cellIndex = 0; cellIndex < cellsPerTex.size(); ++cellIndex)
        {
            unsigned int cellData = cellsPerTex[cellIndex];

            // Extract position from cell data
            unsigned char posX = static_cast<unsigned char>(cellData);
            unsigned char posY = static_cast<unsigned char>(cellData >> 8);

            const float land_scale_27 = 8.0;

            // Set position constant for vertex shader
            CVector position;
            position.x = static_cast<float>(posX) * 128.0f;
            position.y = static_cast<float>(posY) * 128.0f;
            position.z = land_scale_27;

            m3d::Application::g_pApp->m_renderer->SetVsFloatConst(
                10u,
                reinterpret_cast<const float*>(&position),
                1u);

            // Extract rendering information from chunk data
            unsigned short cellMapIndex = 64 * posY + posX;
            unsigned char cellCount = static_cast<unsigned char>(
                tivchunk.m_numCellsPerCellMap[cellMapIndex] >> cellCountShift);

            unsigned char bankNumber = static_cast<unsigned char>(
                tivchunk.m_banknumber[static_cast<unsigned short>(cellData)] >> cellCountShift);

            unsigned short vertexOffset = static_cast<unsigned short>(
                tivchunk.m_offsetsmap[static_cast<unsigned short>(cellData)] >> offsetShift);

            // Skip if offset is invalid (0xFFFF indicates invalid)
            if (vertexOffset != 0xFFFF && cellCount > 0)
            {
                unsigned char primitiveCount = cellCount;
                unsigned int indexCount = primitiveCount * (this->m_lsNumIndices.front()) - 3;
                unsigned int vertexCount = 25 * primitiveCount;

                // Set up rendering resources
                M3D_RENDERER->SetIndices(m_landIbConst.front(), vertexOffset);

                M3D_RENDERER->SetToStream0(tivchunk.m_vbHandle[bankNumber]);

                // Draw the geometry
                M3D_RENDERER->DrawIndexedPrimitiveShader(
                    rend::M3DPT_TRIANGLESTRIP,
                    0,
                    vertexCount,
                    0,
                    indexCount);
            }
        }
    }

    void Landscape::RecursiveEnableShore(unsigned char*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Landscape::traceLineThruCellLs0(float&, int, int, CVector const&, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawCells0(cmn::vector<unsigned> const&, RenderTypes)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Landscape::IsBackfaced(int, int, rend::Cull)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::FreeTiles()
    {
        // TODO: generated code (looks ok)
        // Free tile data array
        delete[] m_tiles;
        m_tiles = nullptr;

        // Release textures and vertex buffers
        const size_t textureCount = m_tilesTextures.size();
        for (size_t i = 0; i < textureCount; ++i)
        {
            TIVChunk* texture = m_tilesTextures[i];
            if (texture)
            {
                // Release texture
                M3D_RENDERER->ReleaseTexture(texture->m_texHandle);

                // Release vertex buffers
                for (auto& vbHandle : texture->m_vbHandle)
                {
                    M3D_RENDERER->ReleaseVb(vbHandle);
                }
            }
        }

        // Free texture resources
        for (size_t i = 0; i < textureCount; ++i)
        {
            TIVChunk* texture = m_tilesTextures[i];
            delete texture;
        }

        // Free textures array
        m_tilesTextures.clear();

        // Free cells per texture data
        m_cellsPerTex.Deallocate();

        // Clear texture to index map
        m_texToIdx.clear();

        // Clear hash table for texture indices
        m_hashTexToIndex.clear();

        // Free passed cells array
        delete[] m_passedCells;
        m_passedCells = nullptr;
    }

    void Landscape::BuildUVSet()
    {
        auto vb = (float*)M3D_RENDERER->LockVb(this->m_landUVVb, 0, 0, 0);
        auto v4 = 2621;
        do
        {
            auto v5 = vb;
            vb += 50;
            --v4;
            memcpy(v5, this->m_uvForAngles, 0xC8u);
        } while (v4);
        M3D_RENDERER->UnlockVb(m_landUVVb);
    }

    Landscape::VisibilityMode Landscape::GetCurVisMode() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::SetCurVisMode(VisibilityMode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::DrawNonTransformGeom(dxGeom*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::RenderRoads()
    {
        // TODO: implement Landscape::RenderRoads
        //RETRUXX_NOT_IMPLEMENTED;
    }

    void Landscape::Register()
    {
        g_Kernel->AddClass(RT_CLASS_LOCAL(RoadNode));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObject));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectWater));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectLandscape));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectStatics));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectRoad));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectPassCell));
    }
}

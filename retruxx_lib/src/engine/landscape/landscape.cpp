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
#include <draftstructures.h>

extern "C" {
#include <ode/collision.h>
#include <ode/collision_trimesh.h>
}

namespace m3d
{
    extern CClient* pClient;

    m3d::RenderModes Landscape::m_renderMode = RM_GAME;

    RT_CLASS_EXPORTS_BEGIN(Landscape)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Landscape);

    void Landscape::LinkObstacleToCells(ai::Obstacle*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::SaveShoreLine(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::GetTileSize() const
    {
        return 4 * this->m_owner->m_level->land_size;
    }

    void Landscape::SetOverlayShader(rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
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

        if (!PhysicBodyPtr || isNodeHaveCollision) {
            // Link the main node
            LinkNodeCollisionGeomsToCell(node, x0, x1, z0, z1);
            UpdateNodeCollisionGeoms(node);

            // Process children using iterative DFS
            std::vector<m3d::Object*> stack;
            stack.push_back(dynamic_cast<m3d::Object*>(node));

            while (!stack.empty()) {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process all siblings of the current node
                m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current);
                while (sibling) {
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

    float Landscape::GetHeightWithCollisions(float, float, bool) const
    {
        throw retruxx::logic_error("Not implemented");
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
        if (m_waterShaderVersion == 20) {
            bool allowPS20 = M3D_KERNEL->GetEngineCfg().m_r_allowPS20.GetB();
            if (!allowPS20 || !M3D_RENDERER->IsFeatureSupported(m3d::rend::DeviceFeature::FEATURE_PS_2_0)) {
                m_waterShaderVersion = 14;
            }
        }

        if (m_waterShaderVersion == 14 && !M3D_RENDERER->IsFeatureSupported(m3d::rend::DeviceFeature::FEATURE_PS_1_4)) {
            m_waterShaderVersion = 11;
        }

        // Load appropriate shaders based on version
        if (m_waterShaderVersion == 11) {
            // PS1.1 shaders
            waterPs = M3D_RENDERER->NewAsmShader("data/shaders/waterTest_ps11.asm", m3d::rend::IAsmShader::Type::PIXEL_SHADER);
            m_waterVs = M3D_RENDERER->NewHlslShader("data/shaders/waterTest_ps11.vs", "WaterVS", m3d::rend::IHlslShader::VS_1_1);

            // Load wave bump texture
            m_waveBumpTex = M3D_RENDERER->AddTexture("data/textures/water_dsdt.shader", 2);
            M3D_RENDERER->SetTextureParameter(m_waveBumpTex, m3d::rend::TexParam::TM_WRAP_S, 3);
            M3D_RENDERER->SetTextureParameter(m_waveBumpTex, m3d::rend::TexParam::TM_WRAP_T, 1);
        }
        else {
            // PS1.4 or PS2.0 shaders
            if (m_waterShaderVersion == 14) {
                // PS1.4 shaders
                waterPs = M3D_RENDERER->NewAsmShader("data/shaders/waterTest_ps14.asm", m3d::rend::IAsmShader::Type::PIXEL_SHADER);
                m_waterVs = M3D_RENDERER->NewHlslShader("data/shaders/waterTest_ps11.vs", "WaterVS", m3d::rend::IHlslShader::VS_1_1);
            }
            else {
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
    }

    void Landscape::SetAllTexturesLoading(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::FreeShoresStuff()
    {
        throw retruxx::logic_error("Not implemented");
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
        float* buff = (float*)M3D_RENDERER->LockVb(m_solidVb, 289 * land_size * land_size, 0, 0);
        M3D_ASSERT(buff);

        auto sizeinCells = land_size;
        auto v9 = 0;
        auto v10 = buff;
        auto v = 0;
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

        auto v17 = 0;
        auto ia = 0;
        auto v25 = 0;
        do
        {
            if (m_solidIb->IsValid())
            {
                M3D_RENDERER->ReleaseIb(m_solidIb[ia]);
            }
            auto v19 = 1 << v17;
            auto v20 = (unsigned __int16)(1 << v17);
            m_solidIb[ia] = M3D_RENDERER->AddIb(32 * (16 / v20 + 2) / v20, 0);
            unsigned short* v21 = (unsigned short*)M3D_RENDERER->LockIb(m_solidIb[ia],
                32 * (16 / v20 + 2) / v20,
                0,
                0);

            auto str = M3D_RENDERER->GetLastErrorStr();
            auto v22 = 0;
            auto nna = 0;
            auto va = 0;
            auto v23 = 17 * v19;
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

            M3D_RENDERER->UnlockIb(m_solidIb[ia]);
            v17 = ia + 1;
            v25 = ++ia < 4;
        } while (v25);
    }

    void Landscape::DrawCellsOverlayedEditor(cmn::vector<unsigned> const&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawSolidLandscape(LandRenderMode, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassInstance(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::QueryWaterVisibility()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawLandScapeTextures(VisibilityMode visMode, bool drawMinimap, bool roadMap)
    {
        throw retruxx::logic_error("Not implemented");
        return;

        int landSize = this->m_owner->m_level->land_size;
        int gridSize = 4 * landSize;

        m_owner->m_sceneGraph.SortedCellsStartFetching(0, (M3D_KERNEL->GetEngineCfg().m_lsTransitionDevider.GetI() * this->m_drawRadius) + 1);
        
        auto fogReduceFactor = m_owner->GetFogReduceFactorFromWeather();

        float s = 0.0;
        float e = 0.0;
        GetFogStartAndEnd(s, e);

        s *= fogReduceFactor;
        e *= fogReduceFactor;

        M3D_RENDERER->SetFogStart(s, false);
        M3D_RENDERER->SetFogEnd(e, false);

        if (drawMinimap)
        {
            throw retruxx::logic_error("Not implemented");
        }
        else
        {
            int x = 0;
            int y = 0;
            int vis = 0;
            int radius = 0;
            while (m_owner->m_sceneGraph.SortedCellsFetch(x, y, vis, radius))
            {
                if (vis == 0) continue;

                // TODO: generated code check this
                throw retruxx::logic_error("Not implemented");

                bool isFullyUnderwater = true;
                bool hasUnderwaterParts = false;

                // Check water conditions for 4 sub-cells
                for (int i = 0; i < 4; i++)
                {
                    int subX = 4 * x + i;
                    int subY = 4 * y + i;
                    int cellIndex = subX + subY * landSize;

                    if (this->m_waterMap[cellIndex])
                    {
                        float waterHeight = this->getWaterHeight(subX, subY);
                        const m3d::Landscape::CellParams& params = this->m_drawedCellParams[cellIndex];

                        if (params.m_h1 > waterHeight) isFullyUnderwater = false;
                        if (waterHeight > params.m_h0) hasUnderwaterParts = true;

                        // Editor mode texture processing
                        if (m3d::Landscape::m_renderMode == RM_EDITOR)
                        {
                            const auto& tileInfo = this->GetTileInfo(subX, subY);
                            if (tileInfo.m_numTexs > 0)
                            {
                                for (int texIndex = 0; texIndex < tileInfo.m_numTexs; texIndex++)
                                {
                                    int texId = tileInfo.m_texFlags[texIndex];
                                    m3d::cmn::vector<unsigned int>& cells = this->m_cellsPerTex.m_data[texId];

                                    if (cells.m_numItems < cells.m_maxItems)
                                    {
                                        unsigned int cellId = subX + ((subY + ((tileInfo.m_angle + (texId << 8)) << 8)) << 8);
                                        cells.m_data[cells.m_numItems] = cellId;
                                        cells.m_numItems++;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        isFullyUnderwater = false;
                    }
                }

                // Visibility filtering
                if ((visMode == VIS_DIRECT) ||
                    (visMode == VIS_REFLECTION && !isFullyUnderwater) ||
                    (visMode == VIS_REFRACTION && hasUnderwaterParts))
                {
                    // Add cell to texture sets
                    int mapIndex = x + landSize * y;
                    std::set<unsigned int>& texSet = this->m_texSetsmap[mapIndex];

                    for (auto it = texSet.begin(); it != texSet.end(); ++it)
                    {
                        unsigned int texId = *it;
                        m3d::cmn::vector<unsigned int>& cells = this->m_cellsPerTex.m_data[texId];

                        if (cells.m_numItems < cells.m_maxItems)
                        {
                            cells.m_data[cells.m_numItems] = x + (y << 8);
                            cells.m_numItems++;
                        }
                    }
                }
            }

            // Setup lighting for non-minimap mode
            if (m3d::Landscape::m_renderMode == RM_EDITOR)
            {
                // TODO: generated code check this
                throw retruxx::logic_error("Not implemented");

                CVector sunDir = this->m_owner->m_sunDir;
                CVector lightDir(-sunDir.x, -sunDir.y, -sunDir.z);

                m3d::rend::LightSource light;
                light.m_type = rend::M3DLIGHT_DIRECTIONAL;
                light.m_direction = lightDir;
                light.m_range = 1000.0f;
                light.m_origin = CVector(0.0f, 20000.0f, 0.0f);

                light.m_diffuse = m_owner->GetWeatherDiffuseColor();
                light.m_ambient = m_owner->GetWeatherAmbientColor() * 0.5f;

                M3D_RENDERER->LightSet(0, light);
            }
        }

        throw retruxx::logic_error("Not implemented");
    }

    rend::TexHandle Landscape::GetLightmapTexture() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassRectangle(CVector2 const&, CVector2 const&)
    {
        throw retruxx::logic_error("Not implemented");
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
        int land_size = this->m_owner->m_level->land_size;
        int vertexCounter = 0;

        // Allocate collision items array
        this->m_oCollisionitems = new CollisionCellItem*[4 * land_size * land_size];
        for (int i = 0; i < 4 * land_size * land_size; i++)
        {
            m_oCollisionitems[i] = nullptr;
        }

        // Process each cell in the landscape
        for (int cellY = 0; cellY < land_size; cellY++)
        {
            int cellArrayOffset = 4 * land_size * cellY;

            for (int cellX = 0; cellX < land_size; cellX++)
            {
                // Create collision cell item
                m3d::Landscape::CollisionCellItem* item = new m3d::Landscape::CollisionCellItem;
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
                        if (this->m_waterMap[4 * worldY * this->m_owner->m_level->land_size + 4 * worldX])
                        {
                            // Create water geometry object
                            m3d::GeomObject* waterObj =
                                (m3d::GeomObject*)m3d::g_Kernel->New("GeomObjectWater");

                            // Allocate vertices and indices
                            waterObj->m_Vertices = (CVector*)m3d::g_Kernel->g_mar.AllocMem(48, 0, 0); // 4 vertices * 12 bytes each
                            waterObj->m_Indices = (int*)m3d::g_Kernel->g_mar.AllocMem(24, 0, 0); // 6 indices * 4 bytes each

                            // Set up indices for two triangles forming a quad
                            int* indices = waterObj->m_Indices;
                            indices[0] = 2; indices[1] = 1; indices[2] = 0; // First triangle
                            indices[3] = 3; indices[4] = 1; indices[5] = 2; // Second triangle

                            // Create vertices for water quad
                            vertexCounter = 0;
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
                            dxTriMeshData* triMeshData = dGeomTriMeshDataCreate();
                            waterObj->m_TriData = triMeshData;
                            dGeomTriMeshDataBuildSingle(
                                triMeshData,
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
                            std::pair<std::set<m3d::GeomObject*>::iterator, bool> result;
                            item->m_geomsList.insert(waterObj);
                        }
                    }
                }

                // Store collision cell item in the array
                this->m_oCollisionitems[cellArrayOffset + cellX] = item;
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
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::CollisionCellItem::CollisionCellItem()
    {
        m_obstacles = new retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >;
    }

    Landscape::CollisionCellItem::~CollisionCellItem()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CollisionCellItem::InsertPhysicObjId(int objId)
    {
        m_physicObjIds.insert(objId);
        m_bMustCheck = true;
    }

    void Landscape::CollisionCellItem::ErasePhysicObjId(int objId)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CollisionCellItem::InsertObstacle(ai::Obstacle* ob)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CollisionCellItem::EraseObstacle(ai::Obstacle* ob)
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Landscape::CollisionCellItem::
    GetPhysicObjIds() const
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle>>, retruxx::allocator<ref_ptr<ai::
    Obstacle>>>& Landscape::CollisionCellItem::GetObstacles() const
    {
        throw std::logic_error("Not implemented");
    }

    Landscape::CollisionInfo::~CollisionInfo()
    {
    }

    CStr const& Landscape::GetPathToTiles() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetEditorRenderMode()
    {
        throw retruxx::logic_error("Not implemented");
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
            const char* setName = alphaSetNode->GetAttribute("name");
            alphaMask.m_name = setName;

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
            m_AlphaSets.push_back(alphaMask);
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawJoint(dxJoint*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::~Landscape()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::UnlinkNodeCollisionGeomsFromCell(SgNode*, int, int, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::New(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::HandleCVar(CVar const*, CConsoleParams const&)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::AddCollisionTris(int, int, CVector*, int, unsigned short*, CMatrix const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawShoreLine()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReloadWaterTextures()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::SaveNormalMap(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::SaveColorMap(CStr const&, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetLsHeight(float, float, float)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::setHgtAtHfPoint(int, int, float)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SaveTileInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005BDC00, Landscape::Load)
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
        m_texSetsmap = new retruxx::set<unsigned>[m_owner->m_level->land_size * m_owner->m_level->land_size];
        
        
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
                    m_colormap[j + i * (m_mapSize + 1)] = data[j + i * m_mapSize];
                }
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
            if (streamSize == 2 * this->m_mapSize * this->m_mapSize) // 16-bit heightfield
            {
                //TODO: check this
                auto* data = new unsigned short[streamSize];
                stream->ReadBytes(data, streamSize);
                for (int i = 0; i < m_mapSize; ++i)
                {
                    for (int j = 0; j < m_mapSize; ++j)
                    {
                        m_heightMap[j + i * (m_mapSize + 1)] = data[j + i * m_mapSize] * 0.12;
                    }
                    m_heightMap[m_mapSize + i * (m_mapSize + 1)] = m_heightMap[(m_mapSize - 1) + i * (m_mapSize + 1)];
                }
                memcpy(&m_heightMap[m_mapSize * (m_mapSize + 1)],
                    &m_heightMap[(m_mapSize - 1) * (m_mapSize + 1)],
                    (m_mapSize + 1) * sizeof(float));
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
                // Expand last row
                memcpy(&m_heightMap[m_mapSize * (m_mapSize + 1)],
                    &m_heightMap[(m_mapSize - 1) * (m_mapSize + 1)],
                    (m_mapSize + 1) * sizeof(float));

                delete[] data;
            }
        }
        else
        {
            M3D_LOG_INFO("Cannot read heightfield: " + m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_hfName));
            return 0;
        }

        delete[] m_waterMap;

        auto landSize = 4 * this->m_owner->m_level->land_size;
        m_waterMap = new short[landSize * landSize];
        size_t waterMapSize = landSize * landSize * sizeof(short);

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
        M3D_LOG_INFO(CStr("Normal map loaded in: ") + loadTime);

        // Load tiles
        startTime =  M3D_KERNEL->GetTimer().GetCurTime();
        if (!LoadTiles(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_tilesFileName)))
        {
            return 0;
        }

        loadTime = M3D_KERNEL->GetTimer().GetCurTime() - startTime;
        M3D_LOG_INFO(CStr("Tiles loaded in: ") + loadTime);

        // Create LOD
        CreateLod();

        // Initialize passed cells
        delete[] m_passedCells;

        m_passedCells = new char[landSize * landSize];

        // Initialize reflection/refraction textures
        InitReflectionRefractionTextures();

        // Set draw radius
        float viewDist = M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF();
        m_drawRadius = (int)(viewDist * 8.0f + 4.0f);
        m_drawRadius = std::max(4, std::min(m_drawRadius, 12));

        // Load shoreline
        startTime = M3D_KERNEL->GetTimer().GetCurTime();
        //if (!LoadShoreLine(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_shoreLineName)))
        //{
        //    GenerateShoreLine();
        //}

        loadTime = M3D_KERNEL->GetTimer().GetCurTime() - startTime;
        M3D_LOG_INFO(CStr("ShoreLine loaded in: ") + loadTime);

        // Initialize grass
        //InitGrass();
        //ReadGrassFromXmlFile(m_owner->m_level->GetFullPathNameA("grass.xml").c_str());

        // Build final landscape
        BuildSolidLandscape();

        return 1;
    }

    void Landscape::RenderGrass(unsigned, GrassInstance**, int*, RenderGrassType)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RenderGrass(retruxx::deque<retruxx::pair<int, int>> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::GetLsSize() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RecalcUV()
    {
        // TODO: generated code
        // Initialize variables
        float* uvAnglePtr = &m_uvForAngles[0][0][1];
        float* setUVsPtr = &m_setAndUVs.m_sets[0][0].m_uvForAngles[0][0][1];
        const float half = 0.5f;
        const float quarter = 0.25f;
        const float angleStep = 1.5707964f;

        // Process 8 groups (32 angles divided by 4)
        for (int group = 0; group < 8; ++group)
        {
            // Process 5 mask types
            for (int masknum = 0; masknum < 5; ++masknum)
            {
                float* currentUVAngle = uvAnglePtr;

                // Process 4 angles per group
                for (int angleIdx = 0; angleIdx < 4; ++angleIdx)
                {
                    // Calculate rotation angle
                    float angle = -(angleIdx * angleStep);
                    float sinAngle = std::sin(angle);
                    float cosAngle = std::cos(angle);

                    // Create rotation matrix
                    CMatrix rotationMat;
                    rotationMat._11 = cosAngle;
                    rotationMat._12 = -sinAngle;
                    rotationMat._21 = sinAngle;
                    rotationMat._22 = cosAngle;

                    // Create transformation matrix
                    CMatrix transformMat;
                    transformMat._11 = rotationMat._11;
                    transformMat._12 = rotationMat._12;
                    transformMat._21 = rotationMat._21;
                    transformMat._22 = rotationMat._22;
                    transformMat._41 = -cosAngle * half;
                    transformMat._42 = -sinAngle * half;
                    transformMat._43 = -half;

                    // Process 5x5 grid
                    for (int yy = 0; yy < 5; ++yy)
                    {
                        float yPos = yy * quarter;
                        float xBase = yPos * rotationMat._21 + transformMat._41;
                        float yBase = yPos * rotationMat._22 + transformMat._42;

                        for (int xx = 0; xx < 5; ++xx)
                        {
                            float xPos = xx * quarter;

                            // Calculate transformed coordinates
                            float u = xPos * rotationMat._11 + xBase;
                            float v = xPos * rotationMat._12 + yBase;

                            // Store original UV coordinates
                            *(currentUVAngle - 1) = u;
                            *currentUVAngle = v;

                            // Calculate and store final UV coordinates
                            float finalU = (u * 0.125f) * 0.95f;
                            float finalV = (v * half) * 0.95f;

                            if (masknum != 0)
                            {
                                // Apply mask-specific offsets
                                finalU += ((group * 4 + masknum) % 8) * 0.125f;
                                finalV += ((group * 4 + masknum) / 8) * half;
                            }

                            // Store with small offsets
                            *(setUVsPtr - 1) = finalU + 0.0031250007f;
                            *setUVsPtr = finalV + 0.012500003f;

                            // Move pointers
                            currentUVAngle += 2;
                            setUVsPtr += 2;
                        }
                    }
                }
            }
        }
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::LinkNodeCollisionGeomsToCell(SgNode* node, int startX, int endX, int startY, int endY)
    {
        // TODO: generated code

        int land_size = m_owner->m_level->land_size;
        int numCells = land_size;
        m3d::SgNode* v8 = node;
        m3d::AnimatedModel* mdl = nullptr;

        int sh;
        if (node->GetProperty(4360u, &sh) && sh != -1)
        {
            m3d::DataServer* v10 = node->GetServer();
            v10->GetItemProperty(sh, 16394, &mdl);
        }

        std::set<m3d::GeomObject*>* geomObjsList = new std::set<m3d::GeomObject*>();

        if (mdl)
        {
            // Process collision points and triangles
            if (!mdl->GetCollisionTrimesh().Points.empty())
            {
                float scale = node->GetScale().x;
                size_t pointsCount = mdl->GetCollisionTrimesh().Points.size();

                // Create GeomObjectStatics
                m3d::GeomObjectStatics* geomStatic = static_cast<m3d::GeomObjectStatics*>(
                    m3d::g_Kernel->New("GeomObjectStatics"));

                // Allocate and copy points
                geomStatic->m_Vertices = new CVector(pointsCount);
                for (size_t i = 0; i < pointsCount; i++)
                {
                    geomStatic->m_Vertices[i].x = mdl->GetCollisionTrimesh().Points[i].x * scale;
                    geomStatic->m_Vertices[i].y = mdl->GetCollisionTrimesh().Points[i].y * scale;
                    geomStatic->m_Vertices[i].z = mdl->GetCollisionTrimesh().Points[i].z * scale;
                }

                // Allocate and copy triangles
                size_t trianglesCount = mdl->GetCollisionTrimesh().Triangles.size();
                geomStatic->m_Indices = new int(trianglesCount * 3);
                for (size_t i = 0; i < trianglesCount; i++)
                {
                    geomStatic->m_Indices[i * 3] = mdl->GetCollisionTrimesh().Triangles[i].I[0];
                    geomStatic->m_Indices[i * 3 + 1] = mdl->GetCollisionTrimesh().Triangles[i].I[1];
                    geomStatic->m_Indices[i * 3 + 2] = mdl->GetCollisionTrimesh().Triangles[i].I[2];
                }

                // Create ODE trimesh
                auto triMeshData = dGeomTriMeshDataCreate();
                dGeomTriMeshDataBuildSingle(triMeshData,
                    geomStatic->m_Vertices, sizeof(CVector), pointsCount,
                    geomStatic->m_Indices, trianglesCount * 3, sizeof(int));

                dxSpace* odeSpace = m_owner->GetOdeSpace();
                dxGeom* triMesh = dCreateTriMesh(odeSpace, triMeshData, 0, 0, 0);

                // Set up geom object
                m3d::GeomObject* geomObj = static_cast<m3d::GeomObject*>(geomStatic);
                geomObj->SetGeom(triMesh);

                // Set bounds
                PointBase<int> startCell(startX, startY);
                PointBase<int> endCell(endX, endY);
                geomObj->SetBounds(startCell, endCell);

                // Add to list
                geomObjsList->insert(geomObj);

                // Add to collision cells
                for (int y = startY; y <= endY; y++)
                {
                    for (int x = startX;
                        x <= endX; x++)
                    {
                        int cellIndex = x + y * numCells;
                        m_oCollisionitems[cellIndex]->m_geomsList.insert(geomObj);

                        if (m_oCollisionitems[cellIndex]->m_wasEnabledLastFrame)
                        {
                            geomObj->IncEnabledCellsCount();
                        }
                    }
                }
            }

            // Process individual geoms
            for (size_t i = 0; i < mdl->GetNumGeoms(); i++)
            {
                auto* geom = mdl->GetGeom(i);
                float scale = node->GetScale().x;

                m3d::GeomObjectStatics* geomStatic = static_cast<m3d::GeomObjectStatics*>(
                    m3d::g_Kernel->New("GeomObjectStatics"));

                dxGeom* odeGeom = nullptr;

                switch (geom->Type)
                {
                case 0: // Box
                {
                    float lx = geom->Sizes.BoxSizes.x * scale;
                    float ly = geom->Sizes.BoxSizes.y * scale;
                    float lz = geom->Sizes.BoxSizes.z * scale;
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

                m3d::GeomObject* geomObj = static_cast<m3d::GeomObject*>(geomStatic);
                geomObj->SetGeom(odeGeom);

                // Set bounds
                PointBase<int> startCell(startX, startY);
                PointBase<int> endCell(endX, endY);
                geomObj->SetBounds(startCell, endCell);

                // Add to list
                geomObjsList->insert(geomObj);

                // Add to collision cells
                for (int y = startY; y <= endY; y++)
                {
                    for (int x = startX;
                        x <= endX; x++)
                    {
                        int cellIndex = x + y * numCells;
                        m_oCollisionitems[cellIndex]->m_geomsList.insert(geomObj);
                    }
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawCellOverlayedShader(int, int, rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::TileInfo const& Landscape::GetTileInfo(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Invalidate()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Landscape::AddGrassInstance(int, CVector const&, float, float, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::EndWaterQuery()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetNodeCollisionGeomsEnabled(SgNode*, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::Save16bitDisplace(CStr const&, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCollisionGeoms(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ClearCollisionCellsMap()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::AddGrassModel(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GenerateOneDPVSCellMesh(int, int, CVector*, int*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::GetFloatToShortScale() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed2(float, float, float, float, unsigned, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::RecalcNormalMap(int, int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ChangeShoreState(int, int, bool)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    GeomObject* Landscape::GetTerrainGeomObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GetDPVSCollisionInfo(int, int, int&, int&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassRadius(CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DisableShoreRegion(int, int)
    {
        throw retruxx::logic_error("Not implemented");
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
                //throw retruxx::logic_error("Not implemented");
            }
            if (m_numWaterCells)
            {
                //throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReleaseOdeCollisionData()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SwitchDrawMode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RemoveCollisionTris(int tag)
    {
        if (tag >= 0)
        {
            throw retruxx::logic_error("Not implemented");
        }
        else
        {
            for (auto& collision : m_collisions)
            {
                if (collision)
                {
                    delete[] collision->m_verts;
                    delete[] collision->m_tris;
                    delete collision;
                }
            }
            m_collisions.clear();
        }
    }

    Object* Landscape::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::getMinMaxHeightForBox(float* box, float buldgeY)
    {
        constexpr float VISCELL_EDGE_LENGTH = 128.0f;
        const float invCellSize = 1.0f / VISCELL_EDGE_LENGTH;

        // Calculate grid indices for bounding box
        const unsigned minGridX = static_cast<unsigned>(box[0] * invCellSize);
        const unsigned minGridZ = static_cast<unsigned>(box[2] * invCellSize);
        const float maxGridX = box[3] * invCellSize;
        const float maxGridZ = box[5] * invCellSize;

        const unsigned landSize = m_owner->m_level->land_size;
        const float minInit = -999999.0f;
        float& minHeight = box[1];
        float& maxHeight = box[4];

        minHeight = minInit;
        maxHeight = minInit;

        // Process all grid cells in bounding box
        for (unsigned gridX = minGridX; gridX < maxGridX; ++gridX) {
            for (unsigned gridZ = minGridZ; gridZ < maxGridZ; ++gridZ) {
                // Skip out-of-bounds cells
                if (gridX >= landSize || gridZ >= landSize)
                    continue;

                const auto& cell = m_cellParams[gridX + gridZ * landSize];

                // Update min/max with terrain heights
                minHeight = std::min(minHeight, std::min(cell.m_h0, cell.m_h1));
                maxHeight = std::max(maxHeight, std::max(cell.m_h0, cell.m_h1));

                // Process water cells
                if (cell.m_iswatercell) {
                    maxHeight = std::max(maxHeight, cell.m_maxwater);
                }
            }
        }

        // Apply final offset to max height
        maxHeight += VISCELL_EDGE_LENGTH + buldgeY;
    }

    void Landscape::SetGameRenderMode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CollectGrassCell(int, int, unsigned&, GrassInstance**, int*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawGeom(dxGeom*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Update()
    {
    }

    void Landscape::DrawWaterLayer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CreateHelperStructures()
    {
        // TODO: implement Landscape::CreateHelperStructures
       // throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed2Projected(float, float, float, float, unsigned, bool, CClipper const&)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCells(cmn::vector<unsigned> const&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GetVisCellHeights(float&, float&, int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CreateIndicesTriLists(int*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawMassBox(dMass*, CVector const&, Quaternion const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::InitGrass()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::StartWaterQuery()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::PutGrassToLandscape(CVector2 const&, CVector2 const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::GetNumTiles() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::setDrawRadius(int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GetDrawedCellHeights(float&, float&, int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Landscape::GetNearestGrassInstance(CVector const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DoneGrass()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::getCameraHeight(float, float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::getGrassModelIdByName(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::SaveCameraMap(CStr const&, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::EnableShoreRegion(int, int)
    {
        throw retruxx::logic_error("Not implemented");
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
                saveDistDivider = 2;
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
            throw retruxx::logic_error("Not implemented");
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
            m_owner->RenderSky(LandRenderMode::LRM_DIRECT);
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
        M3D_RENDERER->SetBlend(rend::BlendMode::BM_NONE, false);
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
            throw retruxx::logic_error("Not implemented");
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
            throw retruxx::logic_error("Not implemented");
        }
        m_profilerDraw->EndCountdown();
    }

    int Landscape::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReadGrassFromXmlFile(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::WriteGrassToXmlFile(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Landscape::getNormal(float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned char Landscape::GetColor(float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetPresenceOnCollisionMap(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::IsThisVisCellHasWater(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed(unsigned, CVector const&, CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReloadLightmapTexture(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* Landscape::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::LoadShoreLine(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CreateHeights(CellParams* dest, int ls, int cellSize)
    {
        // TODO: generated code
            // Initialize variables
        float minHeight = 999999.0f;
        float maxHeight = -999999.0f;
        float minWaterHeight = 999999.0f;
        float maxWaterHeight = -999999.0f;

        // Initialize data structures
        std::unordered_map<unsigned int, int> heightCounts; // Tracks frequency of water heights
        std::set<int> usedHeights;                         // Tracks unique water heights

        // Process each cell in the landscape
        for (int cy = 0; cy < ls; cy++)
        {
            for (int cx = 0; cx < ls; cx++)
            {
                // Reset min/max for this cell
                float cellMinHeight = 999999.0f;
                float cellMaxHeight = -999999.0f;
                float cellMinWater = 999999.0f;
                float cellMaxWater = -999999.0f;

                // Process each point within the cell
                for (int y = 0; y < cellSize; y++)
                {
                    for (int x = 0; x < cellSize; x++)
                    {
                        // Get terrain height
                        int mapX = cx * cellSize + x;
                        int mapY = cy * cellSize + y;
                        float height = m_heightMap[mapX + mapY * (m_mapSize + 1)];

                        // Update terrain height bounds
                        cellMaxHeight = std::max(cellMaxHeight, height + 64.0f);
                        cellMinHeight = std::min(cellMinHeight, height - 64.0f);

                        // Process water if this is a high-res cell (4x4)
                        if (cellSize == 4 && m_waterMap[4 * cy * m_owner->m_level->land_size + cx])
                        {
                            float waterHeight = getWaterHeight(cx, cy);

                            // Track water height frequency
                            unsigned int quantizedHeight = static_cast<unsigned int>(waterHeight * 8.333334f);
                            heightCounts[quantizedHeight]++;
                            usedHeights.insert(quantizedHeight);

                            // Update water height bounds
                            cellMaxWater = std::max(cellMaxWater, waterHeight);
                            cellMinWater = std::min(cellMinWater, waterHeight);

                            // Mark as water cell in main params
                            int mainCellX = cx / 4;
                            int mainCellY = cy / 4;
                            m_cellParams[mainCellY * m_owner->m_level->land_size + mainCellX].m_iswatercell = 1;
                        }
                    }
                }

                // Store cell height bounds
                dest[cy * ls + cx].m_h0 = cellMinHeight;
                dest[cy * ls + cx].m_h1 = cellMaxHeight;

                // For high-res cells, store water bounds in main cell params
                if (cellSize == 4)
                {
                    int mainCellX = cx / 4;
                    int mainCellY = cy / 4;
                    m_cellParams[mainCellY * m_owner->m_level->land_size + mainCellX].m_minwater = cellMinWater;
                    m_cellParams[mainCellY * m_owner->m_level->land_size + mainCellX].m_maxwater = cellMaxWater;
                }

                // Track global min height
                minHeight = std::min(minHeight, cellMinHeight);
            }
        }

        // Determine most common water height
        int mostCommonHeight = 0;
        int maxCount = 0;
        for (const auto& entry : heightCounts)
        {
            if (entry.second > maxCount)
            {
                maxCount = entry.second;
                mostCommonHeight = entry.first;
            }
        }

        // Set global water level based on most common height
        m_owner->m_level->waterlevel = mostCommonHeight * 0.12f;

        // Clean up
        heightCounts.clear();
        usedHeights.clear();
    }

    int Landscape::isWaterCell(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Restore()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ManageLandScapeCollisionTriMeshes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::LinkPassMapCellToCollisionCell(PointBase<int> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReBuildShoresVb()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::LoadTiles(CStr const& filename)
    {
        // TODO: generated code
        // Free existing tiles if any
        FreeTiles();

        // Calculate land size and allocate memory for tiles
        const int landSize = m_owner->m_level->land_size;
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

        if (fileLandSize != static_cast<unsigned int>(landSize * sizeof(int)))
        {
            M3D_LOG_ERR("Error: Bad tilemap file, landsize = " + CStr(landSize) + ", tilemap size in file = " + CStr(fileLandSize));
            return 0;
        }

        // Read tile path
        unsigned int pathLength = *reinterpret_cast<unsigned int*>(data);
        data += 4;
        m_pathTile = CStr(reinterpret_cast<char*>(data), pathLength);
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
            tileNames.emplace_back(reinterpret_cast<char*>(data), nameLength);
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
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RecursiveDisableShore(unsigned char*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::Landscape(Landscape const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CreateIndices(uint16_t* indices, int sizeIndex, int sizeVertex, int step)
    {
        const int vertexPlusOne = sizeVertex + 1;
        uint16_t* currentIndex = indices;
        int vertexOffset = 0;
        const int stepVertexOffset = step * vertexPlusOne;
        const int loopLimit = 64;
        int loopCounter = loopLimit;

        do {
            if (sizeIndex > 0) {
                int remainingIndices = sizeIndex;
                do {
                    for (int i = 0; i <= sizeIndex; i++) {
                        *currentIndex = stepVertexOffset + vertexOffset;
                        currentIndex[1] = vertexOffset;

                        if (i == sizeIndex) {
                            vertexOffset += stepVertexOffset - sizeVertex;
                        }
                        else {
                            vertexOffset += step;
                        }
                        currentIndex += 2;
                    }

                    *currentIndex = sizeVertex + vertexOffset - stepVertexOffset;
                    currentIndex[1] = vertexPlusOne + vertexOffset;
                    currentIndex += 2;
                    remainingIndices--;
                } while (remainingIndices > 0);
            }

            const uint16_t finalIndex = vertexOffset + stepVertexOffset + vertexPlusOne;
            vertexOffset += vertexPlusOne;
            loopCounter--;

            *(currentIndex - 1) = finalIndex;
        } while (loopCounter != 1);

        return (currentIndex - indices) / loopLimit;
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
       RETRUXX_DLL_JMP_TO_CTOR(0x005B8950, Landscape::Landscape);

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
        for (int i = 0; i < 16; i++) {
            waterCellsToDraw[i].reserve(100);
        }

        // Create water queries
        for (int i = 0; i < 3; i++) {
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
        M3D_KERNEL->GetEngineCfg().m_console->RegisterCVar(&m_lockVis, false);

        // Create LOD index buffers
        unsigned int lodFactor = 1;
        m_maxLOD = 0;

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
        VertexWaterTest* waterVertices = static_cast<VertexWaterTest*>(M3D_RENDERER->LockVb(m_waterVb, 0, 0, 0));

        constexpr uint8_t gridSize = 9;  // Since loops go up to 8 (0-8 inclusive)
        VertexWaterTest currentVertex;

        for (uint8_t zCoord = 0; zCoord < m_maxWaterCellPerPass; ++zCoord) {
            currentVertex.z = zCoord;
            const int packedZ = *reinterpret_cast<int*>(&currentVertex.z);

            for (uint8_t yCoord = 0; yCoord < gridSize; ++yCoord) {
                currentVertex.y = yCoord;

                for (uint8_t xCoord = 0; xCoord < gridSize; ++xCoord) {
                    currentVertex.x = xCoord;

                    // Copy vertex data
                    *reinterpret_cast<int*>(&waterVertices->x) = *reinterpret_cast<int*>(&currentVertex.x);
                    *reinterpret_cast<int*>(&waterVertices->z) = packedZ;

                    ++waterVertices;
                }
            }
        }

        M3D_RENDERER->UnlockVb(m_waterVb);

        // Allocate water tile info
        waterTileInfo = new CVector4[m_maxWaterCellPerPass];

        // Create water index buffers for 16 LOD levels (4 patterns x 4 levels each)
        // First loop (4 iterations)
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

    void Landscape::BuildCells0(rend::VertexLandscape*, TIVChunk&, int&, cmn::vector<unsigned> const&, RenderTypes, retruxx::vector<int, retruxx::allocator<int>>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCellsFast0(cmn::vector<unsigned> const&, TIVChunk&, RenderTypes)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RecursiveEnableShore(unsigned char*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::traceLineThruCellLs0(float&, int, int, CVector const&, CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCells0(cmn::vector<unsigned> const&, RenderTypes)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::IsBackfaced(int, int, rend::Cull)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::FreeTiles()
    {
        // TODO: generated code
        // Free tile data array
        delete[] m_tiles;

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
            delete[] texture;
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
    }

    void Landscape::BuildUVSet()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::VisibilityMode Landscape::GetCurVisMode() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetCurVisMode(VisibilityMode)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawNonTransformGeom(dxGeom*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RenderRoads()
    {
        throw retruxx::logic_error("Not implemented");
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

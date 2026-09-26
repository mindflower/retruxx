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
#include "math/coremath.h"
#include "server/objects/base/physicobj.h"

#include <draftstructures.h>

#include "server/objects/physicbodies/physichelpers.h"
#include "server/objects/physicbodies/geoms/ray.h"
#include <server/objects/base/objcontainer.h>

extern "C"
{
#include <ode/collision.h>
#include <ode/collision_trimesh.h>
#include <ode/objects.h>
}

namespace
{
    // A visibility cell is split into four tiles a side; one tile spans four
    // heightmap steps, so it takes a 5x5 grid of vertices to redraw.
    int const TILE_EDGE_VERTS = 5;
    int const VERTS_PER_TILE = TILE_EDGE_VERTS * TILE_EDGE_VERTS;

    // How many tiles go into one streaming vertex buffer lock.
    int const CELLS_PER_DRAW_BATCH = 64;

    // The grass tile array is indexed by a packed 16 bit tile coordinate.
    int const GRASS_TILE_ARRAY_SIZE = 0x10000;

    // A visibility cell is 4x4 grass tiles of 32 world units each; the bound
    // sphere covers such a tile with room for the blades themselves.
    int const GRASS_TILES_PER_CELL = 4;
    float const GRASS_TILE_BOUND_RADIUS = 45.261669f;

    // At most two vertex shader constants per instance fit in one draw.
    unsigned const MAX_GRASS_INSTANCES_PER_BATCH = 246;

    // RVA 0x6AC750 - loads one grass model and bakes it into a vertex and index
    // buffer that already holds as many copies of the mesh as a single draw can
    // place. Each copy carries the index of its own constant register pair, so
    // the shader can look up where that blade goes.
    bool LoadGrassModel(CStr const& fileName, GrassModelInfo& modelInfo)
    {
        unsigned const maxInstancePerPass = (M3D_RENDERER->GetMaxVertexShaderConst() - 20) / 2;

        m3d::AnimatedModel animModel;

        // The extension's first character selects the binary or the text form:
        // ".s.." becomes ".g.." when loading from GAM.
        CStr newFileName(fileName);
        char const* dot = strrchr(newFileName.c_str(), '.');
        int const dotPos = dot ? static_cast<int>(dot - newFileName.c_str()) : -1;
        bool const fromGam = M3D_ENGINE_CFG.m_loadFromGAM.GetB();
        newFileName[dotPos + 1] = fromGam ? 'g' : 's';

        bool const loaded = fromGam ? animModel.LoadGAM(newFileName, false) : animModel.LoadSAM(newFileName, false);
        if (!loaded)
        {
            return false;
        }

        auto* mesh = &animModel.GetMesh(0);
        modelInfo.numVerts = static_cast<unsigned short>(mesh->m_numDrawVerts);
        modelInfo.numIndices = static_cast<unsigned short>(mesh->m_numDrawIndices);
        modelInfo.numTris = static_cast<unsigned short>(mesh->m_numFaces);

        modelInfo.tex = animModel.GetTexHandle(0, 0, 0);
        M3D_RENDERER->ReferenceTexture(modelInfo.tex);

        modelInfo.vb =
            M3D_RENDERER->AddVb(m3d::rend::VERTEX_GRASSTEST, maxInstancePerPass * modelInfo.numVerts, "Grass", 0);

        // Source vertices are 8 floats (position, normal, uv); the grass vertex
        // keeps position, the constant index and the uv.
        auto* dst = static_cast<float*>(M3D_RENDERER->LockVb(modelInfo.vb, 0, 0, 0));
        for (unsigned instance = 0; instance < maxInstancePerPass; ++instance)
        {
            auto const* src = static_cast<float const*>(mesh->m_drawVerts);
            float const constantIdx = static_cast<float>(instance) + static_cast<float>(instance);
            for (unsigned k = 0; k < modelInfo.numVerts; ++k)
            {
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = constantIdx;
                dst[4] = src[6];
                dst[5] = src[7];
                dst += 6;
                src += 8;
            }
        }
        M3D_RENDERER->UnlockVb(modelInfo.vb);

        modelInfo.ib = M3D_RENDERER->AddIb(maxInstancePerPass * modelInfo.numIndices, false);
        auto* idx = static_cast<unsigned short*>(M3D_RENDERER->LockIb(modelInfo.ib, 0, 0, 0));
        for (unsigned instance = 0; instance < maxInstancePerPass; ++instance)
        {
            auto const* srcIdx = mesh->m_drawIndices;
            unsigned short const base = static_cast<unsigned short>(instance * modelInfo.numVerts);
            for (unsigned k = 0; k < modelInfo.numIndices; ++k)
            {
                *idx++ = static_cast<unsigned short>(base + *srcIdx++);
            }
        }
        M3D_RENDERER->UnlockIb(modelInfo.ib);

        modelInfo.modelName = fileName;

        float const dx = animModel.m_box.m_box[3] - animModel.m_box.m_box[0];
        float const dy = animModel.m_box.m_box[4] - animModel.m_box.m_box[1];
        float const dz = animModel.m_box.m_box[5] - animModel.m_box.m_box[2];
        modelInfo.boundRadius = sqrtf(dz * dz + dy * dy + dx * dx);

        return true;
    }

    // Per frame grass culling counters, reported through the debug counter
    // stack by RenderGrass.
    int numTilesRejectedSphere = 0;
    int numInstancesFarAway = 0;
    int numInstancesBehindCamera = 0;
    int numInstancesRejected = 0;

    // RVA 0x6AB770 / 0x6ABFC0 - orders instance indices far to near. Only the
    // index is moved, the instances themselves stay put.
    struct GrassInstanceSortPred
    {
        GrassInstanceSortPred(m3d::Landscape::GrassInstance** instances, CVector const& cameraPos) :
            m_instances(instances),
            m_cameraPos(cameraPos)
        {
        }

        bool operator()(unsigned instanceIdx1, unsigned instanceIdx2) const
        {
            auto const& a = m_instances[instanceIdx1]->pos;
            auto const& b = m_instances[instanceIdx2]->pos;
            float const ax = a.x - m_cameraPos.x;
            float const ay = a.y - m_cameraPos.y;
            float const az = a.z - m_cameraPos.z;
            float const bx = b.x - m_cameraPos.x;
            float const by = b.y - m_cameraPos.y;
            float const bz = b.z - m_cameraPos.z;
            return (az * az + ay * ay) + ax * ax > (bz * bz + by * by) + bx * bx;
        }

        /* 0x0000 */ m3d::Landscape::GrassInstance** m_instances;
        /* 0x0004 */ CVector const& m_cameraPos;
    };
}  // namespace

GrassModelInfo m_grassModels[MAX_GRASS_MODELS];

m3d::Landscape::GrassInstance* visGrassInstances[MAX_VISIBLE_GRASS_INSTANCES];
int visModelsForGrassInstances[MAX_VISIBLE_GRASS_INSTANCES];

// RVA 0x5AB810 - Moller-Trumbore ray/triangle test, one sided: a triangle seen from
// behind (or edge on) is missed. u and v are written before they are range checked.
bool intersectTriangle(
    CVector const& orig,
    CVector const& dir,
    CVector const& a,
    CVector const& b,
    CVector const& c,
    float& t,
    float& u,
    float& v)
{
    float const e1x = b.x - a.x;
    float const e1y = b.y - a.y;
    float const e1z = b.z - a.z;
    float const e2x = c.x - a.x;
    float const e2y = c.y - a.y;
    float const e2z = c.z - a.z;
    float const px = dir.y * e2z - dir.z * e2y;
    float const py = dir.z * e2x - dir.x * e2z;
    float const pz = dir.x * e2y - dir.y * e2x;
    float const det = (e1z * pz + px * e1x) + py * e1y;
    if (det < 0.0001f)
    {
        return false;
    }
    float const tx = orig.x - a.x;
    float const ty = orig.y - a.y;
    float const tz = orig.z - a.z;
    u = (tz * pz + ty * py) + tx * px;
    if (u < 0.0f || u > det)
    {
        return false;
    }
    float const qx = ty * e1z - tz * e1y;
    float const qy = tz * e1x - e1z * tx;
    float const qz = tx * e1y - ty * e1x;
    v = (dir.z * qz + dir.y * qy) + dir.x * qx;
    if (v < 0.0f || v + u > det)
    {
        return false;
    }
    float const inv = 1.0f / det;
    t = ((e2z * qz + e2y * qy) + qx * e2x) * inv;
    u = u * inv;
    v = v * inv;
    return true;
}

namespace m3d
{
    extern CClient* pClient;

    m3d::RenderModes Landscape::m_renderMode = RM_GAME;

    struct AlphaSpecial
    {
        /* 0x0000 */ int m_maskindex;
        /* 0x0004 */ int m_rotate;
    }; /* size: 0x0008 */
    ;

    AlphaSpecial specialMapper[16] = {
        {0, 0},
        {3, 0},
        {3, 1},
        {1, 1},
        {3, 3},
        {1, 0},
        {4, 1},
        {2, 0},
        {3, 2},
        {4, 0},
        {1, 2},
        {2, 1},
        {1, 3},
        {2, 3},
        {2, 2},
        {0, 0}};

    RT_CLASS_EXPORTS_BEGIN(Landscape)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Landscape);

    void Landscape::LinkObstacleToCells(ai::Obstacle* obstacle)
    {
        // TODO: generated code
        // Get the obstacle's bounding box
        Aabb box = obstacle->GetAabb();

        float const VISCELL_EDGE_LENGTH_8 = 128.0;

        // Convert world coordinates to grid coordinates
        float const cellSizeInv = 1.0f / VISCELL_EDGE_LENGTH_8;
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

    bool Landscape::SaveShoreLine(CStr const& FileName)
    {
        // RVA 0x5B1A40
        fs::auxTaggedFile file;
        if (file.Open(FileName.c_str(), fs::auxTaggedFile::CREATE_IGNORE_CRC))
        {
            M3D_LOG_INFO(CStr("Couldn't save shore line to file ") + FileName);
            return false;
        }

        file.setFormatTitle("SFF");
        file.setFormatVersion(1u);
        file.addChunk(1u);
        unsigned const numShores = static_cast<unsigned>(m_shoreLines.size());
        file.addChunkDataCopy(1u, 4u, &numShores);
        for (unsigned i = 0; i < numShores; ++i)
        {
            auto const& line = m_shoreLines[i];
            unsigned const numPoints = static_cast<unsigned>(line.size());
            file.addChunkDataCopy(1u, 4u, &numPoints);
            file.addChunkDataCopy(1u, sizeof(CVector) * numPoints, line.data());
        }

        // The water cells whose shores were switched off in the editor.
        if (!m_noShoresSet.empty())
        {
            file.addChunk(2u);
            unsigned const num = static_cast<unsigned>(m_noShoresSet.size());
            file.addChunkDataCopy(2u, 4u, &num);
            for (unsigned const& key : m_noShoresSet)
            {
                file.addChunkDataCopy(2u, 4u, &key);
            }
        }
        file.Close();
        return true;
    }

    int Landscape::GetTileSize() const
    {
        return 4 * this->m_owner->m_level->land_size;
    }

    void Landscape::SetOverlayShader(rend::IEffect* os)
    {
        // RVA 0x8DF0B0
        overlayShader = os;
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
                    if (sibling->GetFirstChild())
                    {
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
        static CVector const down{0.0, -1.0, 0.0};
        static dContact contact;

        viewRay->SetDirection(down);
        dGeomSetPosition(viewRay->GetGeomId(), x, 3000.0, y);

        auto const aabb = viewRay->GetAabb();
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
        // Release any existing textures first
        ReleaseReflectionRefractionTextures();

        // Determine texture size based on water quality setting
        int textureSize = 256;
        int waterQuality = M3D_ENGINE_CFG.m_r_waterQuality.GetI();
        if (waterQuality >= 2 && waterQuality <= 3)
        {
            textureSize = 512;
        }

        // Create reflection texture
        m_texRtReflection = M3D_RENDERER->AddDynamicTexture("$RtReflection", textureSize, textureSize, 0);

        M3D_RENDERER->SetTextureParameter(m_texRtReflection, rend::TM_WRAP_S, 3u);
        M3D_RENDERER->SetTextureParameter(m_texRtReflection, rend::TM_WRAP_T, 3u);
        M3D_RENDERER->SetTextureParameter(m_texRtReflection, rend::TM_TEX_FILTER, 2u);

        // Create refraction texture
        m_texRtRefraction = M3D_RENDERER->AddDynamicTexture("$RtRefraction", textureSize, textureSize, 0);

        M3D_RENDERER->SetTextureParameter(m_texRtRefraction, rend::TM_WRAP_S, 3u);
        M3D_RENDERER->SetTextureParameter(m_texRtRefraction, rend::TM_WRAP_T, 3u);
        M3D_RENDERER->SetTextureParameter(m_texRtRefraction, rend::TM_TEX_FILTER, 2u);

        // Determine water shader version
        int forcedVersion = M3D_ENGINE_CFG.m_g_forceWaterPSVersion.GetI();
        m_waterShaderVersion = forcedVersion;

        // Validate shader version
        if (forcedVersion != 11 && forcedVersion != 14 && forcedVersion != 20)
        {
            m_waterShaderVersion = 20;
        }

        // Check PS2.0 support
        if (m_waterShaderVersion == 20)
        {
            bool allowPS20 = M3D_ENGINE_CFG.m_r_allowPS20.GetB();
            if (!allowPS20 || !M3D_RENDERER->IsFeatureSupported(rend::FEATURE_PS_2_0))
            {
                m_waterShaderVersion = 14;
            }
        }

        // Check PS1.4 support
        if (m_waterShaderVersion == 14 && !M3D_RENDERER->IsFeatureSupported(rend::FEATURE_PS_1_4))
        {
            m_waterShaderVersion = 11;
        }

        // Load appropriate shaders based on version
        switch (m_waterShaderVersion)
        {
        case 11:
            // PS1.1 shaders
            waterPs = M3D_RENDERER->NewAsmShader("data/shaders/waterTest_ps11.asm", rend::IAsmShader::PIXEL_SHADER);
            m_waterVs =
                M3D_RENDERER->NewHlslShader("data/shaders/waterTest_ps11.vs", "WaterVS", rend::IHlslShader::VS_1_1);
            break;

        case 14:
            // PS1.4 shaders
            waterPs = M3D_RENDERER->NewAsmShader("data/shaders/waterTest_ps14.asm", rend::IAsmShader::PIXEL_SHADER);
            m_waterVs =
                M3D_RENDERER->NewHlslShader("data/shaders/waterTest_ps14.vs", "WaterVS", rend::IHlslShader::VS_1_1);
            break;

        case 20:
        default:
            // PS2.0 shaders with quality variations
            int waterQualityLevel = M3D_ENGINE_CFG.m_r_waterQuality.GetI() - 2;

            switch (waterQualityLevel)
            {
            case 0:  // Medium quality
                m_waterVs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
                    "data/shaders/waterTestMed_ps20.vs", "WaterVS", rend::IHlslShader::VS_2_0);
                m_waterPs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
                    "data/shaders/waterTestMed_ps20.ps", "WaterPS", rend::IHlslShader::PS_2_0);
                break;

            case 1:  // High quality
                m_waterVs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
                    "data/shaders/waterTest_ps20.vs", "WaterVS", rend::IHlslShader::VS_2_0);
                m_waterPs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
                    "data/shaders/waterTest_ps20.ps", "WaterPS", rend::IHlslShader::PS_2_0);
                break;

            default:  // Low quality
                // Clean up existing shaders
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

                m_waterVs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
                    "data/shaders/waterTestLow_ps20.vs", "WaterVS", rend::IHlslShader::VS_2_0);
                m_waterPs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
                    "data/shaders/waterTestLow_ps20.ps", "WaterPS", rend::IHlslShader::PS_2_0);
                break;
            }

            // Load deep water shaders for PS2.0
            m_solidDeepVs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
                "data/shaders/landscapeDeep_ps20.vs", "LandscapeVS", rend::IHlslShader::VS_2_0);
            m_solidDeepPs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
                "data/shaders/landscapeDeep_ps20.ps", "LandscapePS", rend::IHlslShader::PS_2_0);

            // Load water textures and fresnel map
            ReloadWaterTextures();

            m_fresnelTex = m3d::Application::g_pApp->m_renderer->AddTexture("data/textures/fresnel.dds", 0);

            m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_fresnelTex, rend::TM_WRAP_S, 3u);
            m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_fresnelTex, rend::TM_WRAP_T, 3u);
            break;
        }

        // For PS1.1/1.4, load wave bump texture
        if (m_waterShaderVersion == 11 || m_waterShaderVersion == 14)
        {
            m_waveBumpTex = m3d::Application::g_pApp->m_renderer->AddTexture("data/textures/water_dsdt.shader", 2);

            m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_waveBumpTex, rend::TM_WRAP_S, 3u);
            m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_waveBumpTex, rend::TM_WRAP_T, 1u);
        }

        // Load simple water shaders
        m_waterDumbPs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
            "data/shaders/water_dumb.ps", "WaterPS", rend::IHlslShader::PS_1_1);
        m_waterDumbVs = m3d::Application::g_pApp->m_renderer->NewHlslShader(
            "data/shaders/water_dumb.vs", "WaterVS", rend::IHlslShader::VS_1_1);

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

    void Landscape::SetAllTexturesLoading(bool value)
    {
        // RVA 0x5AAC00
        m_loadAllTextures = value;
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
                            *((unsigned short*)v10 + 3) =
                                (int)(float)(*(float*)((char*)&this->m_vnormal->x + v16) * 1024.0);
                            *((unsigned short*)v10 + 4) =
                                (int)(float)(*(float*)((char*)&this->m_vnormal->z + v16) * 1024.0);
                            *((unsigned short*)v10 + 5) =
                                (int)(float)(*(float*)((char*)&this->m_vnormal->y + v16) * 1024.0);
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
            m_solidIb[ia] = M3D_RENDERER->AddIb(32 * (16 / v20 + 2) / v20, 0);
            auto asd = 32 * (16 / v20 + 2) / v20;
            short* v21 = (short*)M3D_RENDERER->LockIb(m_solidIb[ia], 32 * (16 / v20 + 2) / v20, 0, 0);
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

    void Landscape::DrawCellsOverlayedEditor(cmn::vector<unsigned> const& cellsPerTex, unsigned clr)
    {
        // RVA 0x8DF540 - the same grid as DrawCells, but lit: the vertices carry
        // a straight up normal and sit half a unit above the ground so the
        // overlay does not fight the terrain it is drawn on.
        auto vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZNC);

        int numCells = cellsPerTex.m_numItems;
        if (!numCells)
        {
            return;
        }

        unsigned const* curCell = cellsPerTex.m_data;
        int cellsToDraw = numCells;
        while (true)
        {
            cellsToDraw = std::clamp(cellsToDraw, 0, CELLS_PER_DRAW_BATCH);

            int const numVerts = VERTS_PER_TILE * cellsToDraw;
            int vofs = 0;
            auto* v = static_cast<rend::VertexXYZNC*>(M3D_RENDERER->LockVbStreaming(vb, numVerts, vofs, nullptr));

            for (int i = 0; i < cellsToDraw; ++i)
            {
                int const hx = 4 * (*curCell & 0xFFu);
                int const hz = 4 * ((*curCell >> 8) & 0xFFu);
                ++curCell;

                float const* h = &m_heightMap[hx + hz * (m_mapSize + 1)];
                for (int row = 0; row < TILE_EDGE_VERTS; ++row)
                {
                    for (int col = 0; col < TILE_EDGE_VERTS; ++col)
                    {
                        v->x = static_cast<float>(hx + col) * 8.0f;
                        v->y = h[col] + 0.5f;
                        v->z = static_cast<float>(hz + row) * 8.0f;
                        v->nx = 0.0f;
                        v->ny = 1.0f;
                        v->nz = 0.0f;
                        v->c = clr;
                        ++v;
                    }
                    h += m_mapSize + 1;
                }
            }

            M3D_RENDERER->UnlockVb(vb);

            int const numPrims = cellsToDraw * m_lsNumIndices[0] - 3;
            if (numPrims > 0)
            {
                M3D_RENDERER->SetToStream0(vb);
                M3D_RENDERER->SetIndices(m_landIbConst[0], vofs);
                if (overlayShader)
                {
                    M3D_RENDERER->DrawIndexedPrimitiveEffect(
                        rend::M3DPT_TRIANGLESTRIP, overlayShader, 0, numVerts, 0, numPrims);
                }
                else
                {
                    M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLESTRIP, 0, numVerts, 0, numPrims);
                }
            }

            numCells -= cellsToDraw;
            if (!numCells)
            {
                break;
            }
            cellsToDraw = numCells;
        }
    }

    void Landscape::DrawSolidLandscape(LandRenderMode landMode, int lod)
    {
        M3D_RENDERER->SetAlphaTest(0);

        auto const fogColor = m_owner->GetWeatherFogColor();
        M3D_RENDERER->SetFogColor(fogColor, false);
        M3D_RENDERER->PushFog(M3D_ENGINE_CFG.m_r_enableFog.GetB());

        float fogStart = 0.0;
        float fogEnd = 0.0;
        GetFogStartAndEnd(fogStart, fogEnd);

        auto const fogReduceFactor = m_owner->GetWeatherManager().GetFogReduceFactorFromWeather();

        float vsFloat[4] = {0};
        vsFloat[0] = fogReduceFactor * fogEnd;
        vsFloat[1] = 1.0 / ((fogReduceFactor * fogEnd) - (fogReduceFactor * fogStart));
        vsFloat[2] = fogReduceFactor * fogStart;
        vsFloat[3] = m_owner->m_level->waterlevel;
        M3D_RENDERER->SetVsFloatConst(4u, vsFloat, 1u);

        auto projMat = M3D_RENDERER->MatGetProj();
        auto mat = M3D_RENDERER->MatGet();

        CMatrix du;
        auto v14 = projMat._42 * mat._14;
        du._11 =
            (((mat._13 * projMat._31) + (projMat._21 * mat._12)) + (mat._11 * projMat._11)) + (mat._14 * projMat._41);
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
        du._32 =
            (((mat._34 * projMat._42) + (mat._33 * projMat._32)) + (projMat._22 * mat._32)) + (mat._31 * projMat._12);
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
            auto const transitionDivider = M3D_ENGINE_CFG.m_lsTransitionDevider.GetF();
            m_owner->GetGraph().SortedCellsStartFetching(m_drawRadius * transitionDivider + 1, m_drawRadius + 1);
            m_solidPs->Apply();
            m_solidVs->Apply();

            unsigned projMatrixHandle = m_solidVs->GetParamHandleByName("mViewProj");
            m_solidVs->SetMatrix(projMatrixHandle, du);

            auto lightmapTexture = GetLightmapTexture();
            M3D_RENDERER->SetTexture(0, lightmapTexture, -1.0);
            break;
        }
        case LRM_REFLECTION:
        {
            m_owner->GetGraph().SortedCellsStartFetching(0, m_drawRadius + 1);
            m_solidPs->Apply();
            m_solidVs->Apply();

            unsigned projMatrixHandle = m_solidVs->GetParamHandleByName("mViewProj");
            m_solidVs->SetMatrix(projMatrixHandle, du);

            auto lightmapTexture = GetLightmapTexture();
            M3D_RENDERER->SetTexture(0, lightmapTexture, -1.0);
            break;
        }
        case LRM_BIND:
        {
            auto const transitionDivider = M3D_ENGINE_CFG.m_lsTransitionDevider.GetF();
            m_owner->GetGraph().SortedCellsStartFetching(
                transitionDivider * m_drawRadius - 1, transitionDivider * m_drawRadius + 1);
            m_solidBindPs->Apply();
            m_solidBindVs->Apply();

            unsigned projMatrixHandle = m_solidBindVs->GetParamHandleByName("mViewProj");
            m_solidBindVs->SetMatrix(projMatrixHandle, du);

            unsigned viewPosHandle = m_solidBindVs->GetParamHandleByName("ViewPos");
            m_solidBindVs->SetVector3(viewPosHandle, du.getOrgInv());

            auto const transitionCFatror = M3D_ENGINE_CFG.m_lsTransitionCFactor.GetF();
            auto const viewDistanceDivider = M3D_ENGINE_CFG.m_lsViewDistanceDivider.GetF();
            float vsConst =
                ((this->m_drawRadius * transitionDivider) - (viewDistanceDivider * transitionCFatror)) * 128.0;
            M3D_RENDERER->SetVsFloatConst(7u, &vsConst, 1u);

            auto lightmapTexture = GetLightmapTexture();
            M3D_RENDERER->SetTexture(0, lightmapTexture, -1.0);
            break;
        }
        case LRM_DEEPMAP:
        {
            m_owner->GetGraph().SortedCellsStartFetching(0, m_drawRadius + 1);
            m_solidDeepVs->Apply();
            m_solidDeepPs->Apply();

            unsigned projMatrixHandle = m_solidDeepVs->GetParamHandleByName("mViewProj");
            m_solidDeepVs->SetMatrix(projMatrixHandle, du);

            CMatrix const textureMat = du * m_matScale;
            unsigned mTextureHandle = m_solidDeepVs->GetParamHandleByName("mTexture");
            m_solidDeepVs->SetMatrix(mTextureHandle, textureMat);

            CVector const viewPos = M3D_RENDERER->MatGet().getOrgInv();
            unsigned viewPosHandle = m_solidDeepVs->GetParamHandleByName("viewPos");
            m_solidDeepVs->SetVector3(viewPosHandle, viewPos);

            auto const* level = pClient->GetWorld().m_level;
            CVector waterDye;
            waterDye.x = level->m_waterAbsorptionRed;
            waterDye.y = level->m_waterAbsorptionGreen;
            waterDye.z = level->m_waterAbsorptionBlue;
            M3D_RENDERER->SetVsFloatConst(15u, (float*)&waterDye, 1u);

            auto const tex = M3D_RENDERER->GetFullFrameFrameBufferTexture();
            M3D_RENDERER->SetTextureParameter(tex, rend::TM_TEX_FILTER, 5u);
            M3D_RENDERER->SetTexture(0, tex, -1.0);
            M3D_RENDERER->SetFog(0, 0);
            break;
        }
        default:
            // Any other mode falls through to the common tail.
            break;
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
                float buff[3] = {0};

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
                M3D_RENDERER->DrawIndexedPrimitiveShader(
                    rend::M3DPT_TRIANGLESTRIP, 0, this->vertsPerCell, 0, this->trisPerCell[lod]);
            }
        }

        M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("lanscapeDips = " + CStr(landDips)).c_str());
        M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("lanscapeTris = " + CStr(v)).c_str());

        M3D_RENDERER->PopFog();
    }

    void Landscape::RemoveGrassInstance(unsigned instance)
    {
        // RVA 0x6B0080
        unsigned const tileIdx = instance >> 16;
        unsigned const modelSlot = (instance >> 8) & 0xFF;
        unsigned const instanceSlot = instance & 0xFF;

        TileGrass* tile = m_grassArray[tileIdx];
        if (!tile)
        {
            return;
        }
        GrassInstancesForModel* perModel = tile->instancesPerModel[modelSlot];
        if (!perModel)
        {
            return;
        }
        delete perModel->grass[instanceSlot];
        perModel->grass[instanceSlot] = nullptr;
        // NOTE: the tile's numInstances is not decremented, and neither the handle nor the
        // slot is checked (removing an empty slot still counts the model down).
        if (perModel->numInstances-- == 1)
        {
            delete tile->instancesPerModel[modelSlot];
            tile->instancesPerModel[modelSlot] = nullptr;
            --tile->numDiffModels;
        }
        if (!tile->numDiffModels)
        {
            delete m_grassArray[tileIdx];
            m_grassArray[tileIdx] = nullptr;
        }
    }

    void Landscape::QueryWaterVisibility()
    {
        // RVA 0x5C36A0
        // Runs an occlusion query over the water tiles in view with a cheap shader. The
        // results come in a few frames late, so the oldest of the three queries is read.
        if (m_waterQueries[(m_currWaterQuery + 1) % 3]->GetData().d)
        {
            m_isWaterVisible = true;
            return;
        }
        // NOTE: when the water just stopped passing, it is marked hidden and no new query is
        // issued this frame.
        if (m_isWaterVisible)
        {
            m_isWaterVisible = false;
            return;
        }

        m_currWaterQuery = (m_currWaterQuery + 1) % 3;
        m_waterQueries[m_currWaterQuery]->Begin();

        CellsToDraw().swap(waterCellsToDraw[0]);
        int const waterSide = 4 * m_owner->m_level->land_size;
        SceneGraph& graph = m_owner->GetGraph();
        for (int x = 0; x < waterSide; ++x)
        {
            for (int z = 0; z < waterSide; ++z)
            {
                if (!graph.IsCellEnabled(x / 4, z / 4))
                {
                    // The visibility cell is off: skip the rest of its tiles in z.
                    z += 3;
                    continue;
                }
                if (m_waterMap[4 * z * m_owner->m_level->land_size + x])
                {
                    waterCellsToDraw[0].push_back(std::pair<unsigned, float>(x + 256 * z, getWaterHeight(x, z)));
                }
            }
        }

        M3D_RENDERER->PushZbState(rend::ZB_ENABLE);
        M3D_RENDERER->PushBlend(rend::BM_NONE);
        M3D_RENDERER->PushFog(M3D_ENGINE_CFG.m_r_enableFog.GetB());
        M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
        M3D_RENDERER->DisableTextureStages(0);
        m_waterDumbVs->Apply();
        m_waterDumbPs->Apply();
        CMatrix const viewMatrix = M3D_RENDERER->MatGet();
        CMatrix const viewProjMatrix = viewMatrix * M3D_RENDERER->MatGetProj();
        m_waterDumbVs->SetMatrix(m_waterDumbVs->GetParamHandleByName("mViewProj"), viewProjMatrix);

        float s;
        float e;
        GetFogStartAndEnd(s, e);
        float const reduceFactor = m_owner->GetWeatherManager().GetFogReduceFactorFromWeather();
        CVector fogTerm;
        fogTerm.x = reduceFactor * e;
        fogTerm.z = reduceFactor * s;
        fogTerm.y = static_cast<float>(1.0 / (reduceFactor * e - reduceFactor * s));
        m_waterDumbVs->SetVector3(m_waterDumbVs->GetParamHandleByName("g_FogTerm"), fogTerm);

        float const distBetwVert = 4.0f;
        M3D_RENDERER->SetVsFloatConst(17u, &distBetwVert, 1u);
        M3D_RENDERER->SetToStream0(m_waterVb);
        M3D_RENDERER->SetIndices(m_waterIb[3], 0);

        // Tiles are instanced through vertex shader constants, m_maxWaterCellPerPass at a time.
        // NOTE: one draw is issued even when no water tile is in view.
        unsigned numWaterTris = 0;
        unsigned waterDips = 0;
        auto it = waterCellsToDraw[0].begin();
        auto const end = waterCellsToDraw[0].end();
        do
        {
            int numInPass = 0;
            if (it != end)
            {
                do
                {
                    unsigned const key = it->first;
                    waterTileInfo[numInPass].x = static_cast<float>(key & 0xFF) * 32.0f;
                    waterTileInfo[numInPass].y = it->second;
                    waterTileInfo[numInPass].z = static_cast<float>((key >> 8) & 0xFF) * 32.0f;
                    waterTileInfo[numInPass].w = 0.0f;
                    ++numInPass;
                    ++it;
                } while (numInPass != m_maxWaterCellPerPass && it != end);
            }
            M3D_RENDERER->SetVsFloatConst(20u, reinterpret_cast<float const*>(waterTileInfo), numInPass);
            ++waterDips;
            M3D_RENDERER->DrawIndexedPrimitiveShader(
                rend::M3DPT_TRIANGLESTRIP, 0, 81 * numInPass, 0, numInPass * m_wtNumTris[3] - 4);
            numWaterTris = numWaterTris + numInPass * m_wtNumTris[3] - 4;
        } while (it != end);

        M3D_APP->GetDbgCounterStack().DrawStringThisFrame((CStr("waterTris = ") + CStr(numWaterTris)).c_str());
        M3D_APP->GetDbgCounterStack().DrawStringThisFrame((CStr("waterDip = ") + CStr(waterDips)).c_str());

        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopCull();
        m_waterQueries[m_currWaterQuery]->End();
    }

    void Landscape::DrawLandScapeTextures(VisibilityMode visMode, bool drawMinimap, bool roadMap)
    {
        // RVA 0x7A9130 - sorts the visible tiles into per-texture lists and draws them, two passes per texture (the
        // first lit pass, then the blended ones). The editor gathers the tiles itself, lights the terrain with the
        // sun and maps the lightmap through the fixed-function pipeline; it can also draw the whole map
        // (drawMinimap), and roads and shores on top.
        int const landSize = m_owner->m_level->land_size;
        int const gridSize = 4 * landSize;
        auto& sceneGraph = m_owner->m_sceneGraph;
        sceneGraph.SortedCellsStartFetching(
            0, static_cast<int>(static_cast<float>(m_drawRadius) * M3D_ENGINE_CFG.m_lsTransitionDevider.GetF()) + 1);

        float const fogReduce = m_owner->m_weatherManager.GetFogReduceFactorFromWeather();
        float fogStart = 0.0f;
        float fogEnd = 0.0f;
        GetFogStartAndEnd(fogStart, fogEnd);
        fogStart = fogStart * fogReduce;
        M3D_RENDERER->SetFogStart(fogStart, false);
        fogEnd = fogEnd * fogReduce;
        M3D_RENDERER->SetFogEnd(fogEnd, false);

        // Adds a tile to the list of every texture it is painted with, keyed by position, angle and flags.
        auto const addTileTextures = [this](int x, int y) {
            TileInfo const& tileInfo = GetTileInfo(x, y);
            for (int k = 0; k < tileInfo.m_numTexs; ++k)
            {
                m_cellsPerTex.m_data[tileInfo.m_texIndices[k]].push_back(
                    x + ((y + ((tileInfo.m_angle + (tileInfo.m_texFlags[k] << 8)) << 8)) << 8));
            }
        };

        if (drawMinimap)
        {
            if (m_renderMode == RM_EDITOR)
            {
                for (int x = 0; x < gridSize; ++x)
                {
                    for (int y = 0; y < gridSize; ++y)
                    {
                        addTileTextures(x, y);
                    }
                }
                // Every road counts as seen this frame.
                int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
                for (Object* node = m_owner->m_roadManager.m_roadRoot->GetFirstChild(); node;
                     node = node->GetNextSibling())
                {
                    static_cast<RoadNode*>(node)->m_frameVisible = curFrame;
                }
                // A flat grey light from high above.
                rend::LightSource light;
                light.m_type = rend::M3DLIGHT_DIRECTIONAL;
                light.m_diffuse.init(0.0f, 0.0f, 0.0f, 1.0f);
                light.m_ambient.init(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
                light.m_origin = CVector(0.0f, 20000.0f, 0.0f);
                light.m_direction = light.m_origin;
                light.m_range = 1000.0f;
                M3D_RENDERER->LightSet(0, light);
            }
        }
        else
        {
            int cellX = 0;
            int cellY = 0;
            int vis = 0;
            int radius = 0;
            while (sceneGraph.SortedCellsFetch(cellX, cellY, vis, radius))
            {
                if (!vis)
                {
                    continue;
                }

                // The cell's 4x4 tiles against the water: all of them under it, or any of them reaching below it.
                bool isFullyUnderwater = true;
                bool hasUnderwaterParts = false;
                for (int subY = 0; subY < 4; ++subY)
                {
                    int const y = 4 * cellY + subY;
                    for (int subX = 0; subX < 4; ++subX)
                    {
                        int const x = 4 * cellX + subX;
                        int const tile = x + y * gridSize;
                        if (m_waterMap[tile])
                        {
                            float const h0 = m_drawedCellParams[tile].m_h0;
                            float const h1 = m_drawedCellParams[tile].m_h1;
                            float const waterHeight = getWaterHeight(x, y);
                            if (h1 > waterHeight)
                            {
                                isFullyUnderwater = false;
                            }
                            if (waterHeight > h0)
                            {
                                hasUnderwaterParts = true;
                            }
                        }
                        else
                        {
                            isFullyUnderwater = false;
                        }
                        if (m_renderMode == RM_EDITOR)
                        {
                            addTileTextures(x, y);
                        }
                    }
                }

                // In the game the cells come from the precomputed texture sets; reflections skip the cells wholly
                // under water, refractions keep only those reaching below it.
                if (m_renderMode != RM_GAME)
                {
                    continue;
                }
                if (visMode == VIS_REFLECTION)
                {
                    if (isFullyUnderwater)
                    {
                        continue;
                    }
                }
                else if (visMode != VIS_DIRECT && (visMode != VIS_REFRACTION || !hasUnderwaterParts))
                {
                    continue;
                }
                std::set<unsigned int> const& textureSets = m_texSetsmap[cellX + landSize * cellY];
                for (unsigned int texIndex : textureSets)
                {
                    m_cellsPerTex.m_data[texIndex].push_back(cellX + (cellY << 8));
                }
            }

            if (m_renderMode == RM_EDITOR)
            {
                // The editor lights the terrain with the weather's sun.
                rend::LightSource light;
                light.m_type = rend::M3DLIGHT_DIRECTIONAL;
                light.m_origin =
                    CVector(0.0f - m_owner->m_sunDir.x, 0.0f - m_owner->m_sunDir.y, 0.0f - m_owner->m_sunDir.z);
                light.m_direction = light.m_origin;
                light.m_range = 1000.0f;
                unsigned int const ambient = m_owner->GetWeatherAmbientColor();
                light.m_diffuse.init(m_owner->GetWeatherDiffuseColor());
                // The ambient is taken at half strength.
                float const HALF_BYTE_TO_FLOAT = 0.0019607844f;
                light.m_ambient.init(static_cast<float>((ambient >> 16) & 0xFF) * HALF_BYTE_TO_FLOAT,
                    static_cast<float>((ambient >> 8) & 0xFF) * HALF_BYTE_TO_FLOAT,
                    static_cast<float>(ambient & 0xFF) * HALF_BYTE_TO_FLOAT,
                    static_cast<float>(ambient >> 24) * HALF_BYTE_TO_FLOAT);
                M3D_RENDERER->LightSet(0, light);
            }
        }

        M3D_RENDERER->SetLighting(true, false);
        M3D_RENDERER->LightEnable(0, true);

        rend::Material material;
        memset(&material, 0, sizeof(material));
        material.m_diffuse.init(1.0f, 1.0f, 1.0f, 1.0f);
        material.m_ambient.init(1.0f, 1.0f, 1.0f, 1.0f);
        M3D_RENDERER->MaterialSet(material);

        m_firstpasscounter = 0;
        m_otherpasscounter = 0;

        if (M3D_ENGINE_CFG.m_lsShadows.GetB())
        {
            M3D_RENDERER->SetToStream(1, m_landUVVb);
            M3D_RENDERER->SetTexture(0, m_AlphaSets.front().m_texMasks[0].front(), -1.0);
            M3D_RENDERER->SetTexture(2, GetLightmapTexture(), -1.0);
            M3D_RENDERER->SetFogColor(m_owner->GetWeatherFogColor(), false);
            M3D_RENDERER->SetFogMode(rend::M3DFOG_LINEAR, false);

            float const VISCELL_EDGE_LENGTH_24 = 128.0f;
            float const lightmapScale = 1.0f / (static_cast<float>(landSize) * VISCELL_EDGE_LENGTH_24);
            if (m_renderMode == RM_EDITOR)
            {
                // The lightmap is mapped from the world position: u = x * scale, v = -z * scale. (The shipped code
                // multiplies two zero-filled matrices into this, so every other entry is 0.)
                CMatrix lightmapMatrix;
                memset(&lightmapMatrix, 0, sizeof(lightmapMatrix));
                lightmapMatrix._11 = lightmapScale;
                lightmapMatrix._32 = 0.0f - lightmapScale;
                lightmapMatrix._44 = 1.0f;
                M3D_RENDERER->TgEnableSetMatrixSt(2, &lightmapMatrix, false);
                M3D_RENDERER->SetStageState(2, rend::BM_COLOR, rend::TS_TEX_MODULATE2X_PREV);
                M3D_RENDERER->SetStageState(2, rend::BM_ALPHA, rend::TS_PREV);
                M3D_RENDERER->DisableTextureStages(3);
            }
            else
            {
                m_landscapeVs->Apply();

                CVector fogTerm;
                fogTerm.x = fogEnd;
                fogTerm.z = fogStart;
                fogTerm.y = 1.0f / (fogEnd - fogStart);
                m_landscapeVs->SetVector3(m_landscapeVs->GetParamHandleByName("g_FogTerm"), fogTerm);

                CMatrix const viewMatrix = M3D_RENDERER->MatGet();
                CMatrix const viewProjMatrix = viewMatrix * M3D_RENDERER->MatGetProj();
                CMatrix const worldMatrix = M3D_RENDERER->MatGetWorld();
                m_landscapeVs->SetMatrix(m_landscapeVs->GetParamHandleByName("mViewProj"), viewProjMatrix);

                CVector lightmapScaleVec;
                lightmapScaleVec.x = lightmapScale;
                lightmapScaleVec.y = 0.0f - lightmapScale;
                lightmapScaleVec.z = 0.0f;
                m_landscapeVs->SetVector3(m_landscapeVs->GetParamHandleByName("lightmapScale"), lightmapScaleVec);

                m_landscapeVs->SetMatrix(m_landscapeVs->GetParamHandleByName("mWorld"), worldMatrix);
            }

            for (unsigned int i = 0; i < m_tilesTextures.size(); ++i)
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
                if (m_renderMode != RM_GAME)
                {
                    // The road map is drawn without the terrain.
                    if (m_renderMode == RM_EDITOR && !roadMap)
                    {
                        M3D_RENDERER->SetZbState(rend::ZB_ENABLE, false);
                        DrawCells0(m_cellsPerTex[i], RT_FIRSTPASSLIGHT);
                        M3D_RENDERER->TgSetTcSource(1, rend::TC_FROM_VERTEX, 1);
                        M3D_RENDERER->SetZbState(rend::ZB_NOWRITE, false);
                        DrawCells0(m_cellsPerTex[i], RT_OTHERPASSES);
                    }
                }
                else
                {
                    // The shipped build inlines the second DrawCellsFast0 call.
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
            for (unsigned int i = 0; i < m_tilesTextures.size(); ++i)
            {
                m_cellsPerTex[i].clear();
            }
        }

        M3D_RENDERER->SetAlphaTest(false);
        M3D_RENDERER->SetBlend(rend::BM_NONE, false);
        M3D_RENDERER->TgSetTcSource(0, rend::TC_FROM_VERTEX, 0);
        M3D_RENDERER->TgSetTcSource(1, rend::TC_FROM_VERTEX, 1);
        M3D_RENDERER->TgDisable(2);
        M3D_RENDERER->DisableTextureStages(1);

        if (m_renderMode == RM_EDITOR && M3D_ENGINE_CFG.m_g_drawRoads.GetB())
        {
            // The roads on the visible cells, listed as x + (y << 16).
            retruxx::vector<unsigned int> visList;
            visList.reserve(1000);
            sceneGraph.SortedCellsStartFetching(0, m_drawRadius);
            // NOTE: the reservation is thrown away again before the list is filled.
            retruxx::vector<unsigned int>().swap(visList);
            int cellX = 0;
            int cellY = 0;
            int vis = 0;
            int radius = 0;
            while (sceneGraph.SortedCellsFetch(cellX, cellY, vis, radius))
            {
                if (vis)
                {
                    visList.push_back(cellX + (cellY << 16));
                }
            }
            if (roadMap)
            {
                M3D_RENDERER->PushBlend(rend::BM_NONE);
            }
            m_owner->m_roadManager.RenderRoads(visList, RRT_SIMPLE, nullptr, roadMap);
            if (roadMap)
            {
                M3D_RENDERER->PopBlend();
            }
        }
        if (drawMinimap && m_renderMode == RM_EDITOR && M3D_ENGINE_CFG.m_g_drawShores.GetB())
        {
            M3D_RENDERER->SetTexture(0, rend::TexHandle(), -1.0);
            DrawShoreLine();
        }

        M3D_RENDERER->LightEnable(0, false);
        M3D_RENDERER->SetLighting(false, false);
    }

    rend::TexHandle Landscape::GetLightmapTexture() const
    {
        return m_texLightmap;
    }

    void Landscape::RemoveGrassRectangle(CVector2 const& Min, CVector2 const& Max)
    {
        // RVA 0x6AF800
        if (!m_grassArray)
        {
            return;
        }

        // The upper bounds are exclusive and one past the tile holding the edge.
        int tileMinX = static_cast<int>(Min.x * (1.0f / 32.0f));
        int tileMinY = static_cast<int>(Min.y * (1.0f / 32.0f));
        int tileMaxX = static_cast<int>(Max.x * (1.0f / 32.0f)) + 1;
        int tileMaxY = static_cast<int>(Max.y * (1.0f / 32.0f)) + 1;
        tileMinX = std::clamp(tileMinX, 0, 256);
        tileMinY = std::clamp(tileMinY, 0, 256);
        tileMaxX = std::clamp(tileMaxX, 0, 256);
        tileMaxY = std::clamp(tileMaxY, 0, 256);

        for (int x = tileMinX; x < tileMaxX; ++x)
        {
            for (int y = tileMinY; y < tileMaxY; ++y)
            {
                TileGrass*& tile = m_grassArray[x + (y << 8)];
                if (!tile)
                {
                    continue;
                }
                for (unsigned i = 0; tile && i < tile->instancesPerModel.size(); ++i)
                {
                    GrassInstancesForModel* perModel = tile->instancesPerModel[i];
                    if (!perModel)
                    {
                        continue;
                    }
                    for (unsigned k = 0; k < perModel->grass.size(); ++k)
                    {
                        GrassInstance* gi = perModel->grass[k];
                        if (!gi || !(gi->pos.x > Min.x && Max.x > gi->pos.x && gi->pos.z > Min.y && Max.y > gi->pos.z))
                        {
                            continue;
                        }
                        delete gi;
                        perModel->grass[k] = nullptr;
                        // NOTE: the tile's numInstances is not decremented.
                        bool modelDeleted = false;
                        if (perModel->numInstances-- == 1)
                        {
                            delete tile->instancesPerModel[i];
                            tile->instancesPerModel[i] = nullptr;
                            --tile->numDiffModels;
                            modelDeleted = true;
                        }
                        if (!tile->numDiffModels)
                        {
                            delete tile;
                            tile = nullptr;
                        }
                        // NOTE: the original goes on reading the freed model and tile; their
                        // destructors have just nulled the vectors, so its loops end here.
                        if (modelDeleted)
                        {
                            break;
                        }
                    }
                }
            }
        }
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
                            waterObj->m_Vertices = new CVector[4];  // 4 vertices for the quad
                            waterObj->m_Indices = new int[6];       // 6 indices for two triangles

                            // Set up indices for two triangles forming a quad
                            int* indices = waterObj->m_Indices;
                            indices[0] = 2;
                            indices[1] = 1;
                            indices[2] = 0;  // First triangle
                            indices[3] = 3;
                            indices[4] = 1;
                            indices[5] = 2;  // Second triangle

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
                                6,               // index count
                                3 * sizeof(int)  // triangle stride
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
            1,                       // vertical scale
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
        // RVA 0x5AADD0
        return RT_CLASS_LOCAL(Landscape);
    }

    Landscape::CollisionCellItem::CollisionCellItem()
    {
        m_obstacles = new retruxx::
            set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle>>, retruxx::allocator<ref_ptr<ai::Obstacle>>>;
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
        // RVA 0x64C4B0
        m_obstacles->insert(ref_ptr<ai::Obstacle>(ob));
    }

    void Landscape::CollisionCellItem::EraseObstacle(ai::Obstacle* ob)
    {
        // RVA 0x64E6A0
        m_obstacles->erase(ref_ptr<ai::Obstacle>(ob));
    }

    retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>> const&
        Landscape::CollisionCellItem::GetPhysicObjIds() const
    {
        // RVA 0x6024E0
        return m_physicObjIds;
    }

    retruxx::set<
        ref_ptr<ai::Obstacle>,
        retruxx::less<ref_ptr<ai::Obstacle>>,
        retruxx::allocator<ref_ptr<ai::Obstacle>>> const&
        Landscape::CollisionCellItem::GetObstacles() const
    {
        // RVA 0x6024F0
        return *m_obstacles;
    }

    Landscape::CollisionInfo::CollisionInfo() : m_verts(nullptr), m_tris(nullptr)
    {
        // RVA 0x646110
    }

    Landscape::CollisionInfo::~CollisionInfo()
    {
        delete[] m_verts;
        delete[] m_tris;
    }

    float Landscape::CollisionInfo::TraceRay(CVector const& org, CVector const& dir)
    {
        // RVA 0x646120 - the first triangle hit, in index order, not the nearest.
        for (int i = 0; i < m_numTris; ++i)
        {
            unsigned short const* tri = &m_tris[3 * i];
            CVector const v0 = m_verts[tri[0]];
            CVector const v1 = m_verts[tri[1]];
            CVector const v2 = m_verts[tri[2]];
            float t;
            float u;
            float v;
            if (intersectTriangle(org, dir, v2, v1, v0, t, u, v))
            {
                return t;
            }
        }
        return -1.0f;
    }

    void Landscape::CollisionInfo::Create(
        int numVerts,
        CVector* verts,
        int numTris,
        unsigned short* tris,
        CMatrix const& toWorld)
    {
        // RVA 0x647CC0 - keeps a world space copy of the mesh, with a box around the
        // local vertices placed by toWorld and an axis aligned box around the world ones.
        m_verts = new CVector[numVerts];
        m_tris = new unsigned short[3 * numTris];
        m_numTris = numTris;
        m_numVerts = numVerts;
        memcpy(m_tris, tris, 3 * sizeof(unsigned short) * numTris);

        m_box.StartEmbracing();
        for (int i = 0; i < numVerts; ++i)
        {
            m_box.EmbracePoint(verts[i]);
        }
        CVector const max(m_box.m_box[3], m_box.m_box[4], m_box.m_box[5]);
        CVector const min(m_box.m_box[0], m_box.m_box[1], m_box.m_box[2]);
        m_obb.Create(min, max, toWorld, false);

        m_box.StartEmbracing();
        for (int i = 0; i < numVerts; ++i)
        {
            CVector const& v = verts[i];
            CVector& w = m_verts[i];
            w.x = toWorld._21 * v.y + toWorld._11 * v.x + v.z * toWorld._31 + toWorld._41;
            w.y = toWorld._12 * v.x + toWorld._32 * v.z + toWorld._22 * v.y + toWorld._42;
            w.z = v.y * toWorld._23 + toWorld._13 * v.x + v.z * toWorld._33 + toWorld._43;
            m_box.EmbracePoint(w);
        }
    }

    CStr const& Landscape::GetPathToTiles() const
    {
        // RVA 0x5AAC10
        return m_pathTile;
    }

    void Landscape::SetEditorRenderMode()
    {
        // RVA 0x5AADF0
        m_renderMode = RM_EDITOR;
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
                    char const* textureName = maskNode->GetAttribute("name");
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
                    char const* nameAttr = typeNode->GetAttribute("name");
                    landType.m_name = nameAttr;

                    char const* passmaskAttr = typeNode->GetAttribute("passmask");
                    landType.m_passmask = passmaskAttr ? atoi(passmaskAttr) : 0;

                    char const* alphasetAttr = typeNode->GetAttribute("alphaset");
                    landType.m_alphaset = 0;
                    m_hashAlphaToLand.add(alphasetAttr, landType.m_alphaset);

                    // Process tiles
                    ref_ptr tileNode(xmlFile->CreateNode());
                    typeNode->GetFirstChild(tileNode, "tile");

                    while (!tileNode->IsEmpty())
                    {
                        char const* tileFile = tileNode->GetAttribute("file");

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
        // RVA 0x6AFD90
        ReadGrassFromXmlFile("data/testGrass.xml");
    }

    void Landscape::DrawJoint(dxJoint* joint)
    {
        // RVA 0x644D50
        // Treats the joint as a universal joint: lines from both bodies to the anchor, then
        // both axes from the anchor.
        dxBody* body0 = dJointGetBody(joint, 0);
        dxBody* body1 = dJointGetBody(joint, 1);
        CVector const* pos0 = body0 ? reinterpret_cast<CVector const*>(dBodyGetPosition(body0)) : nullptr;
        CVector const* pos1 = body1 ? reinterpret_cast<CVector const*>(dBodyGetPosition(body1)) : nullptr;

        dVector3 anchor;
        dJointGetUniversalAnchor(joint, anchor);
        CVector const Anchor(anchor[0], anchor[1], anchor[2]);
        // The first body's address doubles as the line colour.
        unsigned const color = reinterpret_cast<unsigned>(body0) | 0xFF000000;
        if (pos0)
        {
            CVector const from = *pos0;
            M3D_APP->DrawLine(from, Anchor, color);
        }
        if (pos1)
        {
            CVector const from = *pos1;
            M3D_APP->DrawLine(from, Anchor, color);
        }

        dVector3 axis1;
        dJointGetUniversalAxis1(joint, axis1);
        M3D_APP->DrawLine(Anchor, CVector(axis1[0] + Anchor.x, axis1[1] + Anchor.y, axis1[2] + Anchor.z), 0xFF000000);
        dVector3 axis2;
        dJointGetUniversalAxis2(joint, axis2);
        M3D_APP->DrawLine(Anchor, CVector(Anchor.x + axis2[0], axis2[1] + Anchor.y, axis2[2] + Anchor.z), 0xFF000000);
    }

    Landscape::~Landscape()
    {
        // RVA 0x5BACB0
        Release();
        for (int i = 0; i < 3; ++i)
        {
            m_waterQueries[i]->Release();
        }
        // NOTE: waterTileInfo is freed but not nulled.
        delete[] waterTileInfo;

        for (auto& alphaSet : m_AlphaSets)
        {
            for (auto& masks : alphaSet.m_texMasks)
            {
                for (auto& tex : masks)
                {
                    M3D_RENDERER->ReleaseTexture(tex);
                }
            }
        }
        // NOTE: only the first 4 of the 16 water index buffers are released.
        for (int i = 0; i < 4; ++i)
        {
            M3D_RENDERER->ReleaseIb(m_waterIb[i]);
        }
        for (auto& cells : waterCellsToDraw)
        {
            CellsToDraw().swap(cells);
        }
        for (auto& ib : m_landIbConst)
        {
            M3D_RENDERER->ReleaseIb(ib);
        }
        M3D_RENDERER->ReleaseVb(m_waterVb);
        M3D_RENDERER->ReleaseVb(m_landUVVb);
        M3D_RENDERER->ReleaseVb(m_landVb);
        delete[] m_dummyVB;
        m_dummyVB = nullptr;
        M3D_ENGINE_CFG.m_console->UnregisterCVar(&m_lockVis);

        auto const releaseShader = [](auto*& shader)
        {
            if (shader)
            {
                shader->Release();
                shader = nullptr;
            }
        };
        releaseShader(m_shoresShader);
        releaseShader(m_solidVs);
        releaseShader(m_solidPs);
        releaseShader(m_solidBindVs);
        releaseShader(m_solidBindPs);
        releaseShader(m_landscapeVs);
        releaseShader(m_landscapePsFP);
        releaseShader(m_landscapePsSP);
    }

    void Landscape::UnlinkNodeCollisionGeomsFromCell(SgNode* node, int x, int y, bool deleteList)
    {
        // RVA 0x64C560
        int const numCells = m_owner->m_level->land_size;
        std::set<m3d::GeomObject*>* t = nullptr;
        node->GetProperty(4357u, &t);
        if (!t)
        {
            return;
        }

        if (m_oCollisionitems)
        {
            CollisionCellItem* item = m_oCollisionitems[x + y * m_owner->m_level->land_size];
            for (GeomObject* geomObj : *t)
            {
                item->m_geomsList.erase(geomObj);
            }
        }

        if (deleteList)
        {
            // Every geom also leaves the other cells it was linked into, then is deleted.
            for (GeomObject* geomObj : *t)
            {
                PointBase<int> const startCell = geomObj->GetStartCell();
                PointBase<int> const endCell = geomObj->GetEndCell();
                if (m_oCollisionitems)
                {
                    for (int cx = startCell.x; cx <= endCell.x; ++cx)
                    {
                        for (int cy = startCell.y; cy <= endCell.y; ++cy)
                        {
                            m_oCollisionitems[cx + numCells * cy]->m_geomsList.erase(geomObj);
                        }
                    }
                }
                geomObj->Release();
                delete geomObj;
            }

            t->clear();
            delete t;
            t = nullptr;
            node->SetProperty(4357u, &t);
        }
    }

    int Landscape::New(float heightLevel)
    {
        // RVA 0x5BF7B0
        // Starts an empty landscape for the editor: a flat height map at heightLevel, a
        // grey colour map, no water and every tile on texture 0.
        m_mapSize = 16 * m_owner->m_level->land_size;
        m_pathTile = CStr("data\\tiles\\");
        int const numPoints = (m_mapSize + 1) * (m_mapSize + 1);

        delete[] m_heightMap;
        m_heightMap = nullptr;
        m_heightMap = new float[numPoints];
        for (int i = 0; i < numPoints; ++i)
        {
            m_heightMap[i] = heightLevel;
        }

        delete[] m_cliffHeightMap;
        m_cliffHeightMap = nullptr;
        m_cliffHeightMap = new unsigned char[numPoints];

        delete[] m_colormap;
        m_colormap = nullptr;
        m_colormap = new unsigned[numPoints];
        for (int i = 0; i < (m_mapSize + 1) * (m_mapSize + 1); ++i)
        {
            m_colormap[i] = 0xFF7F7F7F;
        }

        delete[] m_texSetsmap;
        m_texSetsmap = nullptr;
        m_texSetsmap = new std::set<unsigned>[m_owner->m_level->land_size * m_owner->m_level->land_size];

        int const waterSide = 4 * m_owner->m_level->land_size;
        int const numTiles = waterSide * waterSide;
        delete[] m_waterMap;
        m_waterMap = nullptr;
        m_waterMap = new short[numTiles];
        memset(m_waterMap, 0, 2 * numTiles);

        CreateLod();

        delete[] m_tiles;
        m_tiles = nullptr;
        m_tiles = new TileInfo[numTiles];
        for (int i = 0; i < numTiles; ++i)
        {
            m_tiles[i].m_texIndex0 = 0;
            m_tiles[i].m_numTexs = 1;
        }

        // NOTE: a previous normal map is leaked.
        m_vnormal = new CVector[(m_mapSize + 1) * (m_mapSize + 1)];
        RecalcNormalMap(0, 0, m_mapSize, m_mapSize);

        delete[] m_passedCells;
        m_passedCells = nullptr;
        m_passedCells = new char[numTiles];

        ReleaseReflectionRefractionTextures();
        InitReflectionRefractionTextures();
        RecalcUV();
        CreateHelperStructures();
        m_loadAllTextures = true;
        ReadTileInfo(1);
        CreateHelperStructures();
        InitGrass();
        return 1;
    }

    bool Landscape::HandleCVar(CVar const*, CConsoleParams const&)
    {
        // RVA 0x5ABB80
        return false;
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
        // RVA 0x5ABB70
    }

    void Landscape::AddCollisionTris(
        int tag,
        int numVerts,
        CVector* verts,
        int numTris,
        unsigned short* tris,
        CMatrix const& toWorld)
    {
        // RVA 0x64CCD0
        auto* ci = new CollisionInfo();
        ci->Create(numVerts, verts, numTris, tris, toWorld);
        ci->m_tag = tag;
        m_collisions.push_back(ci);
    }

    void Landscape::DrawShoreLine()
    {
        // RVA 0x5C2B30
        M3D_RENDERER->SetWhiteTexture(0);
        for (auto const& line : m_shoreLines)
        {
            if (line.size() < 2)
            {
                continue;
            }
            for (unsigned i = 0; i < line.size() - 1; ++i)
            {
                M3D_APP->DrawLine(line[i], line[i + 1], 0xFFFF0000);
            }
        }
    }

    void Landscape::ReloadWaterTextures()
    {
        // TODO: generated code Landscape::ReloadWaterTextures
        // Only reload textures for PS2.0 shader version
        if (m_waterShaderVersion != 20)
        {
            return;
        }

        // Release and reload the large wave bump texture
        if (m_waveBumpTex.IsValid())
        {
            m3d::Application::g_pApp->m_renderer->ReleaseTexture(m_waveBumpTex);
        }

        // Build path for large water texture
        CStr basePath = M3D_ENGINE_CFG.m_weather_PathToTextures.GetS();
        CStr bigTexturePath = basePath + pClient->GetWorld().m_level->m_waterTexBig;

        // Load the large wave bump texture
        m_waveBumpTex = m3d::Application::g_pApp->m_renderer->AddTexture(bigTexturePath, 6);

        // Set texture parameters for large texture
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_waveBumpTex, rend::TM_WRAP_S, 1u);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_waveBumpTex, rend::TM_WRAP_T, 1u);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_waveBumpTex, rend::TM_TEX_FILTER, 5u);

        // Release and reload the small wave bump texture
        if (m_waveBumpSmTex.IsValid())
        {
            m3d::Application::g_pApp->m_renderer->ReleaseTexture(m_waveBumpSmTex);
        }

        // Build path for small water texture
        CStr smallTexturePath = basePath + pClient->GetWorld().m_level->m_waterTexSmall;

        // Load the small wave bump texture
        m_waveBumpSmTex = m3d::Application::g_pApp->m_renderer->AddTexture(smallTexturePath, 6);

        // Set texture parameters for small texture
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_waveBumpSmTex, rend::TM_WRAP_S, 1u);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_waveBumpSmTex, rend::TM_WRAP_T, 1u);
        m3d::Application::g_pApp->m_renderer->SetTextureParameter(m_waveBumpSmTex, rend::TM_TEX_FILTER, 5u);
    }

    bool Landscape::SaveNormalMap(CStr const& FileName)
    {
        // RVA 0x5AD920
        fs::auxTaggedFile File;
        if (File.Open(FileName.c_str(), fs::auxTaggedFile::CREATE_IGNORE_CRC))
        {
            M3D_LOG_INFO(CStr("Couldn't save normal map to file ") + FileName);
            return false;
        }

        File.setFormatTitle("RIV");
        File.setFormatVersion(1u);
        File.addChunk(0xBADF00Du);
        // Only x and y of each normal are stored, as 16 bit fixed point.
        unsigned const sizeOfMap = 4 * (m_mapSize + 1) * (m_mapSize + 1);
        // NOTE: twice the buffer that is written is allocated.
        auto* data = new short[sizeOfMap];
        short* out = data;
        for (int i = 0; i < (m_mapSize + 1) * (m_mapSize + 1); ++i)
        {
            // These conversions round to nearest (a bare fistp) rather than truncate.
            *out++ = static_cast<short>(lrintf(m_vnormal[i].x * 32767.0f));
            *out++ = static_cast<short>(lrintf(m_vnormal[i].y * 32767.0f));
        }
        File.addChunkDataCopy(0xBADF00Du, 4u, &sizeOfMap);
        File.addChunkDataCopy(0xBADF00Du, sizeOfMap, data);
        delete[] data;
        File.Close();
        return true;
    }

    bool Landscape::SaveColorMap(CStr const& Name, int stripe)
    {
        // RVA 0x5AD270
        // Writes the colour map framed by a 16 * stripe texel border on every side; the
        // border repeats the map's first texel.
        scoped_ptr stream(M3D_KERNEL->GetFileServer().CreateFileStream());
        if (!stream->Open(Name.c_str(), fs::IStream::OPEN_WRITE))
        {
            M3D_LOG_ERR(CStr("Couldn't save colormap to file:") + Name);
            return false;
        }

        int const border = 16 * stripe;
        for (int row = 0; row < m_mapSize + 2 * border; ++row)
        {
            for (int col = 0; col < m_mapSize + 2 * border; ++col)
            {
                unsigned v = m_colormap[0];
                if (col >= border && col < m_mapSize + border && row >= border && row < m_mapSize + border)
                {
                    v = m_colormap[col + (row - border) * (m_mapSize + 1) - border];
                }
                stream->WriteBytes(&v, 4);
            }
        }
        stream->Close();
        return true;
    }

    void Landscape::SetLsHeight(float x, float y, float h)
    {
        // RVA 0x5AB610
        // NOTE: the last row and column of height samples can't be set this way.
        int const ix = static_cast<int>(x * 0.125f);
        int const iy = static_cast<int>(y * 0.125f);
        if (ix >= 0 && iy >= 0 && ix < m_mapSize - 1 && iy < m_mapSize - 1)
        {
            m_heightMap[ix + iy * (m_mapSize + 1)] = h;
        }
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
            return (float)((float)((float)((float)(z2 - z4) * (float)(1.0 - xa)) +
                                   (float)((float)(z3 - z4) * (float)(1.0 - *(float*)&ox))) +
                           z4);
        else
            return (float)((float)((float)((float)(z3 - z1) * xa) + (float)((float)(z2 - z1) * *(float*)&ox)) + z1);
    }

    unsigned Landscape::GetNumGrassModels() const
    {
        // RVA 0x6AB660
        return m_numGrassModels;
    }

    void Landscape::setHgtAtHfPoint(int x, int y, float h)
    {
        // RVA 0x5AAC60
        m_heightMap[x + y * (m_mapSize + 1)] = h;
    }

    float Landscape::getHgtAtHfPoint(int x, int y) const
    {
        if (x > m_mapSize || x < 0 || y > m_mapSize || y < 0)
            return 0.0;
        else
            return this->m_heightMap[x + y * (m_mapSize + 1)];
    }

    rend::TexHandle Landscape::GetTexHandleFromList(unsigned num) const
    {
        if (num < this->m_tilesTextures.size())
        {
            return m_tilesTextures[num]->m_texHandle;
        }
        return {};
    }

    void Landscape::SaveTileInfo()
    {
        // RVA 0x5B1D10
        // Writes the alpha mask sets and land types back to TileInfo.xml in the tile folder.
        CStr err;
        CStr const name = m_pathTile + CStr("TileInfo.xml");

        ref_ptr xmlFileW = M3D_KERNEL->CreateXmlFile();
        ref_ptr rootW = xmlFileW->CreateNode(cmn::XML_NODE_ELEMENT, "TileSetName");
        ref_ptr AlphaSetsNode = xmlFileW->CreateNode(cmn::XML_NODE_ELEMENT, "AlphaSets");
        ref_ptr LandSetsNode = xmlFileW->CreateNode(cmn::XML_NODE_ELEMENT, "LandTypes");
        // cp1251 for "alpha mask sets" and "soil types".
        AlphaSetsNode->SetAttribute("name", "\xcd\xe0\xe1\xee\xf0\xfb \xe0\xeb\xfc\xf4\xe0 \xec\xe0\xf1\xee\xea");
        LandSetsNode->SetAttribute("name", "\xd2\xe8\xef\xfb \xef\xee\xf7\xe2\xfb");

        for (unsigned k = 0; k < m_AlphaSets.size(); ++k)
        {
            ref_ptr node2 = xmlFileW->CreateNode(cmn::XML_NODE_ELEMENT, "Set");
            node2->SetAttribute("name", m_AlphaSets[k].m_name.c_str());
            for (int j = 0; j < 4; ++j)
            {
                for (unsigned typez = 0; typez < m_AlphaSets[k].m_texMasks[j].size(); ++typez)
                {
                    ref_ptr maskNode =
                        xmlFileW->CreateNode(cmn::XML_NODE_ELEMENT, (CStr("mask") + CStr(j + 1)).c_str());
                    CStr texname;
                    bool const res = M3D_RENDERER->GetTextureName(m_AlphaSets[k].m_texMasks[j][typez], texname) != 0;
                    assert(res);
                    maskNode->SetAttribute("name", NameFromFileName(texname).c_str());
                    node2->AddChild(maskNode);
                }
            }
            AlphaSetsNode->AddChild(node2);
        }

        for (unsigned m = 0; m < m_Lands.size(); ++m)
        {
            LandType const& land = m_Lands[m];
            ref_ptr typeNode = xmlFileW->CreateNode(cmn::XML_NODE_ELEMENT, "type");
            typeNode->SetAttribute("name", land.m_name.c_str());
            typeNode->SetAttribute("passmask", CStr(land.m_passmask).c_str());
            typeNode->SetAttribute("alphaset", m_AlphaSets[land.m_alphaset].m_name.c_str());
            typeNode->SetAttribute("priority", CStr(land.m_priority).c_str());
            for (unsigned i = 0; i < land.m_texIndices.size(); ++i)
            {
                ref_ptr tileNode = xmlFileW->CreateNode(cmn::XML_NODE_ELEMENT, "tile");
                CStr filename;
                bool const res =
                    M3D_RENDERER->GetTextureName(m_tilesTextures[land.m_texIndices[i]]->m_texHandle, filename) != 0;
                assert(res);
                // The tile folder prefix is cut out of the texture path.
                // NOTE: when the folder is not found the position passed on is -1.
                int const pathLen = m_pathTile.c_str() ? static_cast<int>(strlen(m_pathTile.c_str())) : 0;
                char const* found = strstr(filename.c_str(), m_pathTile.c_str());
                filename.del(found ? static_cast<int>(found - filename.c_str()) : -1, pathLen);
                tileNode->SetAttribute("file", filename.c_str());
                typeNode->AddChild(tileNode);
            }
            LandSetsNode->AddChild(typeNode);
        }

        rootW->AddChild(AlphaSetsNode);
        rootW->AddChild(LandSetsNode);
        xmlFileW->AddChild(rootW);
        if (!WriteXmlFile(name.c_str(), xmlFileW, &err))
        {
            M3D_LOG_INFO(err);
        }
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
                    M3D_APP->m_renderer->SetTextureParameter(
                        texHandle, rend::TM_MIP_LOD_BIAS, M3D_KERNEL->GetEngineCfg().m_g_shoresMipBias.GetF());
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

        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream(); stream->Open(
                m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_colormapName).c_str(), fs::IStream::OPEN_READ))
        {
            // The file holds m_mapSize x m_mapSize texels; the map has one extra
            // column (and row), filled by repeating the last one.
            auto const streamSize = stream->GetSize();
            auto* data = new unsigned[streamSize / sizeof(unsigned) + 1];
            stream->ReadBytes(data, streamSize);
            stream->Close();
            for (int i = 0; i < m_mapSize; ++i)
            {
                for (int j = 0; j < m_mapSize; ++j)
                {
                    m_colormap[j + i * (m_mapSize + 1)] = data[j + i * m_mapSize];
                }
                m_colormap[m_mapSize + i * (m_mapSize + 1)] = m_colormap[m_mapSize + i * (m_mapSize + 1) - 1];
            }
            delete[] data;
        }
        else
        {
            M3D_LOG_INFO(
                "Cannot read colormap: " + m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_colormapName) +
                "using default values");
            for (int i = 0; i < (this->m_mapSize + 1) * (this->m_mapSize + 1); ++i)
                this->m_colormap[i] = 0xFF7F7F7F;  // Default grey
        }

        delete[] m_heightMap;
        m_heightMap = new float[(m_mapSize + 1) * (m_mapSize + 1)];

        delete[] m_cliffHeightMap;
        m_cliffHeightMap = new unsigned char[(m_mapSize + 1) * (m_mapSize + 1)];

        // TODO: check this
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream(); stream->Open(
                m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_hfName).c_str(), fs::IStream::OPEN_READ))
        {
            auto const streamSize = stream->GetSize();
            if (streamSize == 2 * m_mapSize * m_mapSize)  // 16-bit heightfield
            {
                auto* data = new unsigned short[streamSize];
                stream->ReadBytes(data, streamSize);
                for (int i = 0; i < m_mapSize; ++i)
                {
                    for (int j = 0; j < m_mapSize; ++j)
                    {
                        m_heightMap[j + i * (m_mapSize + 1)] = static_cast<float>(data[j + i * m_mapSize] * 0.12);
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
                    memcpy(&m_heightMap[j * (m_mapSize + 1)], &data[j * m_mapSize], m_mapSize * sizeof(float));
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

        memcpy(
            &m_heightMap[(m_mapSize + 1) * (m_mapSize - 2)],
            &m_heightMap[(m_mapSize + 1) * (m_mapSize - 3)],
            4 * m_mapSize + 4);
        memcpy(
            (char*)m_heightMap + (m_mapSize + 1) * (4 * m_mapSize - 4),
            &m_heightMap[(m_mapSize + 1) * (m_mapSize - 2)],
            4 * m_mapSize + 4);
        memcpy(
            &this->m_heightMap[m_mapSize * (m_mapSize + 1)],
            (char*)this->m_heightMap + (m_mapSize + 1) * (4 * m_mapSize - 4),
            4 * m_mapSize + 4);
        memcpy(
            &this->m_colormap[(m_mapSize + 1) * (m_mapSize - 2)],
            &this->m_colormap[(m_mapSize + 1) * (m_mapSize - 3)],
            4 * m_mapSize + 4);
        memcpy(
            (char*)this->m_colormap + (m_mapSize + 1) * (4 * m_mapSize - 4),
            &this->m_colormap[(m_mapSize + 1) * (m_mapSize - 2)],
            4 * m_mapSize + 4);
        memcpy(
            &this->m_colormap[m_mapSize * (m_mapSize + 1)],
            (char*)this->m_colormap + (m_mapSize + 1) * (4 * m_mapSize - 4),
            4 * m_mapSize + 4);

        delete[] m_waterMap;

        auto const landSize = 4 * this->m_owner->m_level->land_size;
        auto const waterMapSize = landSize * landSize * sizeof(short);
        m_waterMap = new short[landSize * landSize];
        memset(m_waterMap, 0, waterMapSize);

        // TODO check this!!
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream(); stream->Open(
                m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_waterName).c_str(), fs::IStream::OPEN_READ))
        {
            unsigned int waterDataSize = stream->GetSize();
            if (waterDataSize == waterMapSize)
            {
                // Directly read into watermap if sizes match
                stream->ReadBytes(m_waterMap, waterDataSize);
            }
            else
            {
                // An old style byte mask: 0xFF marks water at the
                // level's water height, anything else stays dry.
                // NOTE: landSize * landSize entries are read whatever the file size, so
                // a short file is read past its end.
                unsigned char* tempData = new unsigned char[waterDataSize];
                stream->ReadBytes(tempData, waterDataSize);
                for (int i = 0; i < landSize * landSize; i++)
                {
                    if (tempData[i] == 0xFF)
                    {
                        m_waterMap[i] = static_cast<short>(static_cast<int>(m_owner->m_level->waterlevel * 8.333334f));
                    }
                }
                delete[] tempData;
            }
            stream->Close();
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Cannot open watermap: ") + m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_waterName) +
                CStr(" using empty waterfield"));
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
        startTime = M3D_KERNEL->GetTimer().GetCurTime();
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

    void Landscape::RenderGrass(
        unsigned numVisibleInstances,
        GrassInstance** visGrassInstancesArr,
        int* visModelsForGrassInstancesArr,
        RenderGrassType rgt)
    {
        // RVA 0x6B0230 - every blade is an instance of one of a handful of
        // models, drawn by repeating the model's geometry and feeding the per
        // instance placement through vertex shader constants.
        CVector const cameraPos = M3D_RENDERER->GetViewOrigin();

        // Two constants per instance, leaving the first twenty for everything
        // else the shader needs.
        unsigned maxInstancesPerBatch = (M3D_RENDERER->GetMaxVertexShaderConst() - 20) / 2;
        if (maxInstancesPerBatch > MAX_GRASS_INSTANCES_PER_BATCH)
        {
            maxInstancesPerBatch = MAX_GRASS_INSTANCES_PER_BATCH;
        }

        auto const& timer = M3D_KERNEL->GetTimer();
        float const windPhase = timer.GetFrameStartTimeSec() + timer.GetFrameStartTimeSec();

        int numTris = 0;
        int numDips = 0;

        if (numVisibleInstances)
        {
            unsigned sorted[MAX_VISIBLE_GRASS_INSTANCES];
            for (unsigned i = 0; i < numVisibleInstances; ++i)
            {
                sorted[i] = i;
            }
            std::sort(sorted, sorted + numVisibleInstances, GrassInstanceSortPred(visGrassInstancesArr, cameraPos));

            float constants[8 * MAX_GRASS_INSTANCES_PER_BATCH];

            unsigned remaining = numVisibleInstances;
            unsigned first = 0;
            do
            {
                // The instances are sorted by distance, so a run of the same
                // model is however long it happens to be.
                int const model = visModelsForGrassInstancesArr[sorted[first]];
                GrassModelInfo* info = &m_grassModels[model];

                unsigned run = 0;
                unsigned const* scan = &sorted[first];
                do
                {
                    if (visModelsForGrassInstancesArr[*scan] != model)
                    {
                        break;
                    }
                    ++run;
                    ++scan;
                } while (run < remaining);

                numTris += run * info->numTris;
                remaining -= run;

                M3D_RENDERER->SetTexture(0, info->tex, -1.0);
                M3D_RENDERER->SetToStream0(info->vb);
                M3D_RENDERER->SetIndices(info->ib, 0);
                M3D_RENDERER->SetTexture(1, GetLightmapTexture(), -1.0);
                // The shadow pass has its own texture bound at stage 2.
                M3D_RENDERER->DisableTextureStages(rgt == RGT_FOR_SHADOW ? 3 : 2);

                unsigned cur = first;
                first += run;
                while (run)
                {
                    unsigned batch = run;
                    if (batch > maxInstancesPerBatch)
                    {
                        batch = maxInstancesPerBatch;
                    }

                    float* c = constants;
                    for (unsigned k = 0; k < batch; ++k)
                    {
                        GrassInstance const* gi = visGrassInstancesArr[sorted[cur]];
                        ++cur;
                        c[0] = gi->pos.x;
                        c[1] = gi->pos.y;
                        c[2] = gi->pos.z;
                        c[3] = gi->scale;
                        c[4] = gi->sinYaw;
                        c[5] = gi->cosYaw;
                        // Sway, offset by the blade's own x so a field does not
                        // move as one.
                        float const phase = windPhase + gi->pos.x;
                        c[6] = sinf(phase);
                        c[7] = cosf(phase);
                        c += 8;
                    }

                    M3D_RENDERER->SetVsFloatConst(20u, constants, 2 * batch);
                    M3D_RENDERER->DrawIndexedPrimitiveShader(
                        rend::M3DPT_TRIANGLELIST, 0, batch * info->numVerts, 0, batch * info->numTris);
                    ++numDips;

                    run -= batch;
                }
            } while (remaining);
        }

        if (rgt == RGT_SIMPLE)
        {
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("grass polys = " + CStr(numTris)).c_str());
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("grass dips = " + CStr(numDips)).c_str());
        }
    }

    void Landscape::RenderGrass(retruxx::deque<retruxx::pair<int, int>> const& excludedCells)
    {
        // RVA 0x6B0740 - collects every visible cell within the grass draw
        // distance, minus the ones the caller has already drawn itself, and
        // hands the lot to the instanced renderer above.
        auto const& cfg = M3D_ENGINE_CFG;
        float const drawDist = cfg.m_g_grassDrawDist.GetF();
        if (drawDist < 10.0f)
        {
            return;
        }

        m_profilerDrawGrass->StartCountdown();

        m_grassVs->Apply();
        m_grassPs->Apply();
        M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
        M3D_RENDERER->SetAlphaTest(cfg.m_g_grassAlphatest.GetI());
        M3D_RENDERER->SetBlend(rend::BM_ALPHA, false);
        M3D_RENDERER->SetFog(false, false);
        M3D_RENDERER->SetCull(rend::M3DCULL_NONE, false);

        CMatrix const viewMatrix = M3D_RENDERER->MatGet();
        CMatrix const viewProjMatrix = viewMatrix * M3D_RENDERER->MatGetProj();
        m_grassVs->SetMatrix(m_grassVs->GetParamHandleByName("mViewProj"), viewProjMatrix);

        CVector const viewPos = viewMatrix.getOrgInv();
        m_grassVs->SetVector3(m_grassVs->GetParamHandleByName("ViewPos"), viewPos);
        m_grassVs->SetFloat(m_grassVs->GetParamHandleByName("drawDist"), drawDist);

        float const VISCELL_EDGE_LENGTH_12 = 128.0f;
        CVector lightmapScale;
        lightmapScale.x = 1.0f / (static_cast<float>(m_owner->m_level->land_size) * VISCELL_EDGE_LENGTH_12);
        lightmapScale.y = -lightmapScale.x;
        lightmapScale.z = 0.0f;
        m_grassVs->SetVector3(m_grassVs->GetParamHandleByName("lightmapScale"), lightmapScale);

        numTilesRejectedSphere = 0;
        numInstancesFarAway = 0;
        numInstancesBehindCamera = 0;
        numInstancesRejected = 0;

        auto& graph = m_owner->GetGraph();
        int const radius = static_cast<int>(ceilf(drawDist * (1.0f / VISCELL_EDGE_LENGTH_12)));
        graph.SortedCellsStartFetching(0, radius + 1);

        m3d::Landscape::GrassInstance* visGrass[MAX_VISIBLE_GRASS_INSTANCES];
        int visModels[MAX_VISIBLE_GRASS_INSTANCES];

        unsigned numVisibleInstances = 0;
        int cellX = 0;
        int cellY = 0;
        int vis = 0;
        int cellRadius = 0;
        while (graph.SortedCellsFetch(cellX, cellY, vis, cellRadius))
        {
            if (!vis)
            {
                continue;
            }
            retruxx::pair<int, int> const cell(cellX, cellY);
            if (std::find(excludedCells.begin(), excludedCells.end(), cell) == excludedCells.end())
            {
                CollectGrassCell(cellX, cellY, numVisibleInstances, visGrass, visModels);
            }
        }

        RenderGrass(numVisibleInstances, visGrass, visModels, RGT_SIMPLE);

        auto& counters = M3D_APP->GetDbgCounterStack();
        counters.DrawStringThisFrame(("grass instances = " + CStr(static_cast<int>(numVisibleInstances))).c_str());
        counters.DrawStringThisFrame(("grass tiles cull s = " + CStr(numTilesRejectedSphere)).c_str());
        counters.DrawStringThisFrame(("grass instances far away = " + CStr(numInstancesFarAway)).c_str());
        counters.DrawStringThisFrame(("grass instances behind cam = " + CStr(numInstancesBehindCamera)).c_str());
        counters.DrawStringThisFrame(("grass instances cull = " + CStr(numInstancesRejected)).c_str());

        M3D_RENDERER->PopZbState();

        m_profilerDrawGrass->EndCountdown();
    }

    int Landscape::GetLsSize() const
    {
        // RVA 0x5C0440
        return m_mapSize + 1;
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
                    auto v4 = (float)((float)((float)(mat._41 * shiftHalf0._14) + (float)(mat._31 * shiftHalf0._13)) +
                                      (float)(shiftHalf0._12 * (float)(0.0 - v27))) +
                        v26;
                    auto v5 = (float)((float)((float)(mat._43 * shiftHalf0._14) + (float)(mat._23 * shiftHalf0._12)) +
                                      mat._13) +
                        shiftHalf0._13;
                    auto v36 = shiftHalf0._12 * v26 + mat._42 * shiftHalf0._14 + mat._32 * shiftHalf0._13 + v27;
                    auto v37 = (float)((float)((float)(mat._34 * shiftHalf0._13) + (float)(mat._24 * shiftHalf0._12)) +
                                       mat._14) +
                        shiftHalf0._14;
                    auto v6 = (float)((float)((float)(shiftHalf0._21 * v26) + (float)(shiftHalf0._24 * mat._41)) +
                                      (float)(shiftHalf0._23 * mat._31)) +
                        (float)(0.0 - v27);
                    auto v38 = (float)((float)((float)(shiftHalf0._23 * mat._34) + (float)(shiftHalf0._21 * mat._14)) +
                                       shiftHalf0._24) +
                        mat._24;
                    auto v39 = (float)((float)((float)(shiftHalf0._32 * v26) + (float)(shiftHalf0._31 * v27)) +
                                       (float)(shiftHalf0._34 * mat._42)) +
                        mat._32;
                    auto v40 = (float)((float)((float)(shiftHalf0._34 * mat._43) + (float)(shiftHalf0._32 * mat._23)) +
                                       (float)(shiftHalf0._31 * mat._13)) +
                        1.0;
                    auto v7 = (float)((float)((float)(shiftHalf0._21 * v27) + (float)(shiftHalf0._24 * mat._42)) +
                                      (float)(shiftHalf0._23 * mat._32)) +
                        v26;
                    auto v8 = (float)((float)((float)(shiftHalf0._24 * mat._43) + (float)(shiftHalf0._21 * mat._13)) +
                                      shiftHalf0._23) +
                        mat._23;
                    auto v9 = (float)((float)((float)(shiftHalf0._31 * v26) + (float)(shiftHalf0._34 * mat._41)) +
                                      (float)(shiftHalf0._32 * (float)(0.0 - v27))) +
                        mat._31;
                    auto v41 = (float)((float)((float)(shiftHalf0._32 * mat._24) + (float)(shiftHalf0._31 * mat._14)) +
                                       shiftHalf0._34) +
                        mat._34;
                    auto v33 = v26 * -0.5;
                    auto v42 = (float)((float)((float)(mat._31 * 0.0) - (float)((float)(0.0 - v27) * 0.5)) +
                                       (float)(v26 * -0.5)) +
                        mat._41;
                    auto v43 =
                        (float)((float)((float)(mat._32 * 0.0) - (float)(v27 * 0.5)) + (float)(v26 * -0.5)) + mat._42;
                    auto v44 = mat._43 - (float)((float)(mat._23 + mat._13) * 0.5);
                    auto v35 = v37 * 0.5;
                    auto v45 = (float)((float)(mat._34 * 0.0) - (float)((float)(mat._24 + mat._14) * 0.5)) + 1.0;
                    auto v52 = (float)((float)((float)(shiftHalf1._31 * v5) + (float)(shiftHalf1._21 * v36)) +
                                       (float)(v37 * 0.5)) +
                        v4;
                    auto v53 = (float)((float)((float)(shiftHalf1._32 * v5) + (float)(shiftHalf1._12 * v4)) +
                                       (float)(v37 * 0.5)) +
                        v36;
                    auto v54 = (float)((float)((float)(shiftHalf1._23 * v36) + (float)(shiftHalf1._13 * v4)) +
                                       (float)(v37 * 0.0)) +
                        v5;
                    auto v55 = (float)((float)((float)(shiftHalf1._34 * v5) + (float)(shiftHalf1._24 * v36)) +
                                       (float)(shiftHalf1._14 * v4)) +
                        v37;
                    auto v49 = v38 * 0.5;
                    auto v56 = (float)((float)((float)(shiftHalf1._31 * v8) + (float)(shiftHalf1._21 * v7)) +
                                       (float)(v38 * 0.5)) +
                        v6;
                    auto v57 = (float)((float)((float)(shiftHalf1._32 * v8) + (float)(shiftHalf1._12 * v6)) +
                                       (float)(v38 * 0.5)) +
                        v7;
                    auto v58 = (float)((float)((float)(shiftHalf1._23 * v7) + (float)(shiftHalf1._13 * v6)) +
                                       (float)(v38 * 0.0)) +
                        v8;
                    auto v34 = v41 * 0.5;
                    auto v59 = (float)((float)((float)(shiftHalf1._31 * v40) + (float)(shiftHalf1._21 * v39)) +
                                       (float)(v41 * 0.5)) +
                        v9;
                    auto v60 = (float)((float)((float)(shiftHalf1._32 * v40) + (float)(shiftHalf1._12 * v9)) +
                                       (float)(v41 * 0.5)) +
                        v39;
                    auto v61 = (float)((float)((float)(shiftHalf1._23 * v39) + (float)(shiftHalf1._13 * v9)) +
                                       (float)(v41 * 0.0)) +
                        v40;
                    auto v32 = v45 * 0.5;
                    auto v62 = (float)((float)((float)(shiftHalf1._31 * v44) + (float)(shiftHalf1._21 * v43)) +
                                       (float)(v45 * 0.5)) +
                        v42;
                    auto v63 = (float)((float)((float)(shiftHalf1._32 * v44) + (float)(shiftHalf1._12 * v42)) +
                                       (float)(v45 * 0.5)) +
                        v43;
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
                    v64[11] = (float)((float)((float)(shiftHalf1._34 * v40) + (float)(shiftHalf1._24 * v39)) +
                                      (float)(shiftHalf1._14 * v9)) +
                        v41;
                    v64[7] = (float)((float)((float)(shiftHalf1._34 * v8) + (float)(shiftHalf1._24 * v7)) +
                                     (float)(shiftHalf1._14 * v6)) +
                        v38;
                    auto v10 = 0.5;
                    v64[13] = v63;
                    v64[14] = (float)((float)((float)(shiftHalf1._23 * v43) + (float)(shiftHalf1._13 * v42)) +
                                      (float)(v45 * 0.0)) +
                        v44;
                    v64[15] = (float)((float)((float)(shiftHalf1._34 * v44) + (float)(shiftHalf1._24 * v43)) +
                                      (float)(shiftHalf1._14 * v42)) +
                        v45;
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
        auto const color = M3D_KERNEL->GetEngineCfg().m_g_texturesFilter.GetC();
        for (auto& tile : m_tilesTextures)
        {
            M3D_RENDERER->SetTextureParameter(tile->m_texHandle, m3d::rend::TexParam::TM_TEX_FILTER, color);
            if (color == 3)
            {
                auto const maxAnisotropy = M3D_RENDERER->GetMaxAnisotropy();
                M3D_RENDERER->SetTextureParameter(
                    tile->m_texHandle, m3d::rend::TexParam::TM_MAX_ANISOTROPY, maxAnisotropy);
            }
        }
    }

    void Landscape::ScaleGrassRadius(CVector const& pos, float radius, float coeff)
    {
        // RVA 0x6AE370
        if (!m_grassArray)
        {
            return;
        }

        // The upper bounds are exclusive and one past the tile holding the edge.
        int tileMinX = static_cast<int>((pos.x - radius) * (1.0f / 32.0f));
        int tileMinY = static_cast<int>((pos.z - radius) * (1.0f / 32.0f));
        int tileMaxX = static_cast<int>((pos.x + radius) * (1.0f / 32.0f)) + 1;
        int tileMaxY = static_cast<int>((pos.z + radius) * (1.0f / 32.0f)) + 1;
        tileMinX = std::clamp(tileMinX, 0, 256);
        tileMinY = std::clamp(tileMinY, 0, 256);
        tileMaxX = std::clamp(tileMaxX, 0, 256);
        tileMaxY = std::clamp(tileMaxY, 0, 256);

        for (int x = tileMinX; x < tileMaxX; ++x)
        {
            for (int y = tileMinY; y < tileMaxY; ++y)
            {
                TileGrass* tile = m_grassArray[x + (y << 8)];
                if (!tile)
                {
                    continue;
                }
                for (unsigned i = 0; i < tile->instancesPerModel.size(); ++i)
                {
                    GrassInstancesForModel* perModel = tile->instancesPerModel[i];
                    if (!perModel)
                    {
                        continue;
                    }
                    for (unsigned k = 0; k < perModel->grass.size(); ++k)
                    {
                        GrassInstance* gi = perModel->grass[k];
                        if (!gi)
                        {
                            continue;
                        }
                        double const dx = gi->pos.x - pos.x;
                        double const dz = gi->pos.z - pos.z;
                        if (radius > sqrt(dx * dx + dz * dz))
                        {
                            gi->scale = coeff * gi->scale;
                        }
                    }
                }
            }
        }
    }

    //static std::set<CStr> nen;

    void Landscape::LinkNodeCollisionGeomsToCell(SgNode* node, int startX, int endX, int startY, int endY)
    {
        // TODO: generated code
        auto const landSize = m_owner->m_level->land_size;

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

        if (!mdl->GetCollisionTrimesh().Points.empty())
        {
            // Create GeomObjectStatics
            auto* geomStatic = RT_DYNCAST(M3D_KERNEL->New("GeomObjectStatics"), GeomObjectStatics);

            // Allocate and copy points
            auto const pointsCount = mdl->GetCollisionTrimesh().Points.size();
            geomStatic->m_Vertices = new CVector[pointsCount];

            auto const scale = node->GetScale().x;
            for (size_t i = 0; i < pointsCount; i++)
            {
                geomStatic->m_Vertices[i].x = mdl->GetCollisionTrimesh().Points[i].x * scale;
                geomStatic->m_Vertices[i].y = mdl->GetCollisionTrimesh().Points[i].y * scale;
                geomStatic->m_Vertices[i].z = mdl->GetCollisionTrimesh().Points[i].z * scale;
            }

            // Allocate and copy triangles
            auto const trisCount = mdl->GetCollisionTrimesh().Triangles.size();
            geomStatic->m_Indices = new int[trisCount * 3];

            for (size_t i = 0; i < trisCount; ++i)
            {
                geomStatic->m_Indices[i * 3] = mdl->GetCollisionTrimesh().Triangles[i].I[0];
                geomStatic->m_Indices[i * 3 + 1] = mdl->GetCollisionTrimesh().Triangles[i].I[1];
                geomStatic->m_Indices[i * 3 + 2] = mdl->GetCollisionTrimesh().Triangles[i].I[2];
            }

            // Create ODE trimesh
            geomStatic->m_TriData = dGeomTriMeshDataCreate();

            dGeomTriMeshDataBuildSingle(
                geomStatic->m_TriData,
                geomStatic->m_Vertices,
                sizeof(CVector),
                pointsCount,
                geomStatic->m_Indices,
                trisCount * 3,
                12);

            dxSpace* odeSpace = m_owner->GetOdeSpace();
            dxGeom* triMesh = dCreateTriMesh(odeSpace, geomStatic->m_TriData, 0, 0, 0);
            geomStatic->SetGeom(triMesh);

            geomStatic->m_rotation = {0.0, 0.0, 0.0, 1.0};
            geomStatic->m_translation = {0.0, 0.0, 0.0};

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
            auto const scale = node->GetScale().x;

            auto* geomStatic = RT_DYNCAST(M3D_KERNEL->New("GeomObjectStatics"), GeomObjectStatics);

            dxGeom* odeGeom = nullptr;

            switch (geom->Type)
            {
            case 0:  // Box
            {
                auto const lx = geom->Sizes.BoxSizes.x * scale;
                auto const ly = geom->Sizes.BoxSizes.y * scale;
                auto const lz = geom->Sizes.BoxSizes.z * scale;
                dxSpace* odeSpace = m_owner->GetOdeSpace();
                odeGeom = dCreateBox(odeSpace, lx, ly, lz);
                break;
            }
            case 1:  // Sphere
            {
                float radius = geom->Sizes.BoxSizes.x * scale;
                dxSpace* odeSpace = m_owner->GetOdeSpace();
                odeGeom = dCreateSphere(odeSpace, radius);
                break;
            }
            case 2:  // Cylinder/Capsule
            {
                float radius = geom->Sizes.BoxSizes.x * scale;
                float length = geom->Sizes.BoxSizes.y * scale;
                dxSpace* odeSpace = m_owner->GetOdeSpace();

                // Apply rotation for cylinder (45 degrees around some axis)
                Quaternion rot(geom->Rotation[0], geom->Rotation[1], geom->Rotation[2], geom->Rotation[3]);
                Quaternion addRot(0.7071f, 0.0f, 0.0f, 0.7071f);  // 45 degrees
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
        // RVA 0x5C2640
        if (!m_shoresVb.IsValid())
        {
            return;
        }

        M3D_RENDERER->SetToStream0(m_shoresVb);
        M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
        M3D_RENDERER->SetCull(rend::M3DCULL_NONE, false);
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->SetAlphaTest(1);
        M3D_RENDERER->PushFog(M3D_ENGINE_CFG.m_r_enableFog.GetB());
        M3D_RENDERER->SetWhiteTexture(1);
        M3D_RENDERER->DisableTextureStages(1);
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        pClient->GetWorld().GetGraph().LightSetupSunForWorld();

        unsigned const ambient = m_owner->GetWeatherAmbientColor();
        CVector const colorAmbient(
            static_cast<float>((ambient >> 16) & 0xFF) * 0.0039215689f,
            static_cast<float>((ambient >> 8) & 0xFF) * 0.0039215689f,
            static_cast<float>(ambient & 0xFF) * 0.0039215689f);
        m_shoresShader->SetVector3(rend::IEffect::LightAmbient, colorAmbient);
        unsigned const diffuse = m_owner->GetWeatherDiffuseColor();
        CVector const colorDiffuse(
            static_cast<float>((diffuse >> 16) & 0xFF) * 0.0039215689f,
            static_cast<float>((diffuse >> 8) & 0xFF) * 0.0039215689f,
            static_cast<float>(diffuse & 0xFF) * 0.0039215689f);
        m_shoresShader->SetVector3(rend::IEffect::LightDiffuse, colorDiffuse);

        float s;
        float e;
        GetFogStartAndEnd(s, e);
        float const fogReduceFactor = m_owner->GetWeatherManager().GetFogReduceFactorFromWeather();
        CVector fogTerm;
        fogTerm.x = fogReduceFactor * e;
        fogTerm.z = fogReduceFactor * s;
        fogTerm.y = static_cast<float>(1.0 / (fogReduceFactor * e - fogReduceFactor * s));
        m_shoresShader->SetVector3(rend::IEffect::FogTerm, fogTerm);

        // One pass per wave layer, each sliding and stretching the surf texture on its own
        // clock.
        for (auto& wave : m_waves)
        {
            int const shiftPeriod = static_cast<int>(static_cast<float>(1000.0 / wave.m_tfreq));
            int const shiftTime = static_cast<int>(M3D_KERNEL->GetTimer().GetCurTime()) % shiftPeriod;
            float shift = wave.m_tcomp;
            if (wave.m_tcomp < 0.0f)
            {
                shift = static_cast<float>(
                    sin(static_cast<double>(shiftTime) * 6.2831855f / static_cast<double>(shiftPeriod) +
                        wave.m_tphase) *
                        wave.m_tamplitude +
                    wave.m_tlevel);
            }
            int const scalePeriod = static_cast<int>(static_cast<float>(1000.0 / wave.m_sfreq));
            int const curTime = static_cast<int>(M3D_KERNEL->GetTimer().GetCurTime());
            float scale = wave.m_scomp;
            if (scale < 0.0f)
            {
                // NOTE: the scale oscillation uses m_tphase, not m_sphase.
                scale = static_cast<float>(
                    sin(static_cast<double>(curTime % scalePeriod) * 6.2831855f / static_cast<double>(scalePeriod) +
                        wave.m_tphase) *
                        wave.m_samplitude +
                    wave.m_slevel);
            }

            CMatrix mat;
            memset(&mat, 0, sizeof(mat));
            mat._11 = 1.0f;
            mat._22 = scale;
            mat._32 = scale * shift;
            mat._33 = 1.0f;
            mat._44 = 1.0f;
            m_shoresShader->SetMatrix(rend::IEffect::User_float4x4_param, mat);
            M3D_RENDERER->SetTexture(0, wave.m_texHandle, -1.0);

            // The strips sit back to back in the vertex buffer; lines too short for a strip
            // were given no vertices.
            int baseVertex = 0;
            for (auto const& line : m_shoreLines)
            {
                int const numPoints = static_cast<int>(line.size());
                if (numPoints - 1 > 0)
                {
                    M3D_RENDERER->SetIndices(m_shoresIb, baseVertex);
                    M3D_RENDERER->DrawIndexedPrimitiveEffect(
                        rend::M3DPT_TRIANGLESTRIP, m_shoresShader, 0, 2 * numPoints, 0, 2 * numPoints - 2);
                    baseVertex += 2 * numPoints;
                }
            }
        }

        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopZbState();
    }

    void Landscape::drawCellOverlayedShader(int x, int z, rend::IEffect* shader)
    {
        // RVA 0x8E01A0
        float const VISCELL_EDGE_LENGTH_37 = 128.0f;

        if (m_renderMode != RM_GAME)
        {
            // The editor has no prebuilt solid geometry to reuse, so the cell is
            // re-emitted as a sprite patch. One unit is shaved off each half
            // extent so neighbouring cells do not overlap.
            float const halfSize = VISCELL_EDGE_LENGTH_37 * 0.5f - 1.0f;
            overlayShader = shader;
            drawSpriteOverlayed2(
                (static_cast<float>(x) + 0.5f) * VISCELL_EDGE_LENGTH_37,
                (static_cast<float>(z) + 0.5f) * VISCELL_EDGE_LENGTH_37,
                halfSize,
                halfSize,
                0xFFFFFFFFu,
                false);
            overlayShader = nullptr;
        }
        else
        {
            float const land_scale_41 = 8.0f;

            // The shader is told where this cell starts so it can rebuild world
            // coordinates from the cell local vertices; z carries the vertex
            // spacing rather than a height.
            CVector v;
            v.x = static_cast<float>(x) * 128.0f;
            v.y = static_cast<float>(z) * 128.0f;
            v.z = land_scale_41;
            shader->SetVector3(rend::IEffect::User_float3_param2, v);

            M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
            M3D_RENDERER->SetToStream0(m_solidVb);
            M3D_RENDERER->SetIndices(m_solidIb[0], vertsPerCell * (z + x * m_owner->m_level->land_size));
            M3D_RENDERER->DrawIndexedPrimitiveEffect(
                rend::M3DPT_TRIANGLESTRIP, shader, 0, vertsPerCell, 0, trisPerCell[0]);
            M3D_RENDERER->PopZbState();
        }
    }

    Landscape::TileInfo const& Landscape::GetTileInfo(int x, int y) const
    {
        return m_tiles[4 * y * m_owner->m_level->land_size + x];
    }

    void Landscape::Invalidate()
    {
        // RVA 0x5AB480
    }

    unsigned Landscape::AddGrassInstance(int modelId, CVector const& pos, float yaw, float scale, bool placeOnTerrain)
    {
        // RVA 0x6AF4F0
        unsigned const tileIdx =
            static_cast<int>(pos.x * (1.0f / 32.0f)) + (static_cast<int>(pos.z * (1.0f / 32.0f)) << 8);
        if (!m_grassArray[tileIdx])
        {
            m_grassArray[tileIdx] = new TileGrass;
            m_grassArray[tileIdx]->numDiffModels = 0;
            m_grassArray[tileIdx]->numInstances = 0;
        }
        TileGrass* tile = m_grassArray[tileIdx];

        // Find this model's list in the tile, remembering the first free slot on the way.
        GrassInstancesForModel* perModel = nullptr;
        int modelSlot = -1;
        for (unsigned i = 0; i < tile->instancesPerModel.size(); ++i)
        {
            if (modelSlot == -1 && !tile->instancesPerModel[i])
            {
                modelSlot = static_cast<int>(i);
            }
            if (tile->instancesPerModel[i] && tile->instancesPerModel[i]->modelId == modelId)
            {
                perModel = tile->instancesPerModel[i];
                // NOTE: modelSlot keeps the first free slot seen before the match, if any, so
                // the returned handle can point at the wrong model.
                break;
            }
        }
        if (!perModel)
        {
            perModel = new GrassInstancesForModel;
            if (modelSlot == -1)
            {
                tile->instancesPerModel.push_back(perModel);
                modelSlot = static_cast<int>(tile->instancesPerModel.size()) - 1;
            }
            else
            {
                tile->instancesPerModel[modelSlot] = perModel;
            }
            perModel->modelId = modelId;
            perModel->numInstances = 0;
            ++tile->numDiffModels;
        }

        auto* gi = new GrassInstance;
        gi->pos = pos;
        gi->scale = scale;
        gi->cosYaw = cosf(yaw);
        gi->sinYaw = sinf(yaw);
        if (placeOnTerrain)
        {
            gi->pos.y = GetLsHeight(pos.x, pos.z);
        }

        unsigned slot = 0;
        while (slot < perModel->grass.size() && perModel->grass[slot])
        {
            ++slot;
        }
        if (slot == static_cast<unsigned>(perModel->numInstances))
        {
            perModel->grass.push_back(nullptr);
        }
        // NOTE: the tile's numInstances is not incremented.
        ++perModel->numInstances;
        perModel->grass[slot] = gi;
        return slot + ((modelSlot + (tileIdx << 8)) << 8);
    }

    void Landscape::EndWaterQuery()
    {
        // RVA 0x5C05B0
        m_waterQueries[m_currWaterQuery]->End();
    }

    void Landscape::SetNodeCollisionGeomsEnabled(SgNode* node, bool enabled)
    {
        // RVA 0x64A210
        std::set<GeomObject*>* t = nullptr;
        node->GetProperty(4357u, &t);
        if (!t)
        {
            return;
        }
        for (GeomObject* geomObj : *t)
        {
            // The flag is raised while the geom is switched, then set to the new state.
            geomObj->m_bMayBeEnabled = true;
            if (enabled)
            {
                dGeomEnable(geomObj->m_geom);
            }
            else
            {
                dGeomDisable(geomObj->m_geom);
            }
            geomObj->m_bMayBeEnabled = enabled;
        }
    }

    bool Landscape::Save16bitDisplace(CStr const& Name, int stripe)
    {
        // RVA 0x5B4BF0
        // NOTE: despite the name, the heights are written as 32 bit floats. Like
        // SaveColorMap, the map gets a 16 * stripe border repeating its first sample.
        scoped_ptr stream(M3D_KERNEL->GetFileServer().CreateFileStream());
        if (!stream->Open(Name.c_str(), fs::IStream::OPEN_WRITE))
        {
            M3D_LOG_ERR(CStr("Couldn't save height map to file:") + Name);
            return false;
        }

        int const border = 16 * stripe;
        int const side = m_mapSize + 32 * stripe;
        std::vector<float> tempBuf;
        tempBuf.resize(side * side, 0.0f);
        for (int row = 0; row < m_mapSize + 32 * stripe; ++row)
        {
            for (int col = 0; col < m_mapSize + 32 * stripe; ++col)
            {
                float h = m_heightMap[0];
                if (col >= border && col < m_mapSize + border && row >= border && row < m_mapSize + border)
                {
                    h = m_heightMap[col + (row - border) * (m_mapSize + 1) - border];
                }
                tempBuf[col + row * (m_mapSize + 32 * stripe)] = h;
            }
        }
        stream->WriteBytes(tempBuf.data(), 4 * static_cast<unsigned>(tempBuf.size()));
        stream->Close();
        return true;
    }

    void Landscape::DrawCollisionGeoms(bool allGeoms)
    {
        bool const cgDraw = M3D_ENGINE_CFG.m_cgDraw.GetB();
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
        // RVA 0x644D10
        M3D_ASSERT(!"obsolete");
    }

    bool Landscape::AddGrassModel(char const* modelFileName)
    {
        // RVA 0x6ACB20 - the model table is a fixed twenty slots; a name that is
        // already in it counts as success without loading anything again.
        if (m_numGrassModels == MAX_GRASS_MODELS)
        {
            return false;
        }

        CStr name(modelFileName);
        name.toLower(0x400u);

        for (unsigned i = 0; i < m_numGrassModels; ++i)
        {
            if (name == m_grassModels[i].modelName)
            {
                return true;
            }
        }

        // NOTE: LoadGrassModel is handed the name as it came in, not the
        // lowercased one, so that is what ends up in modelName - which is what
        // the comparison above is then made against.
        CStr fileName(modelFileName);
        bool const loaded = LoadGrassModel(fileName, m_grassModels[m_numGrassModels]);
        if (loaded)
        {
            ++m_numGrassModels;
        }
        return loaded;
    }

    void Landscape::GenerateOneDPVSCellMesh(int, int, CVector*, int*)
    {
        // RVA 0x644C70
    }

    float Landscape::GetFloatToShortScale() const
    {
        // RVA 0x5AAD10
        return 0.12f;
    }

    void Landscape::drawSpriteOverlayed2(float cx, float cz, float hsx, float hsz, unsigned clr, bool all)
    {
        // RVA 0x8DFF20 - draws the axis aligned patch of tiles covered by the
        // rectangle, either through the overlay shader or in the editor's own
        // way. Tiles are a quarter the size of a visibility cell: 32 world
        // units, hence the 1/32 below and the 4 * land_size bound.
        float const TILE_SIZE_INV = 0.03125f;

        float const xMin = cx - hsx;
        float const xMax = cx + hsx;
        float const zMin = cz - hsz;
        float const zMax = cz + hsz;

        // Unless the caller asks for everything, a rectangle that touches any
        // collision volume is skipped entirely - those get their overlay from
        // the collision geometry instead.
        bool overCollision = false;
        for (auto* ci : m_collisions)
        {
            if (ci->m_box.m_box[0] <= xMax && xMin <= ci->m_box.m_box[3] && ci->m_box.m_box[2] <= zMax &&
                zMin <= ci->m_box.m_box[5])
            {
                overCollision = true;
            }
        }

        int const maxIdx = 4 * m_owner->m_level->land_size - 1;
        int const x0 = std::clamp(static_cast<int>(xMin * TILE_SIZE_INV), 0, maxIdx);
        int const x1 = std::clamp(static_cast<int>(xMax * TILE_SIZE_INV), 0, maxIdx);
        int const z0 = std::clamp(static_cast<int>(zMin * TILE_SIZE_INV), 0, maxIdx);
        int const z1 = std::clamp(static_cast<int>(zMax * TILE_SIZE_INV), 0, maxIdx);

        if (!all && overCollision)
        {
            return;
        }

        int const numX = x1 - x0 + 1;
        int const numZ = z1 - z0 + 1;

        cmn::vector<unsigned> cells;
        cells.Allocate(numX * numZ);

        // NOTE: the shipped build allocates this second buffer of the same size,
        // never writes a single entry and frees it again on the way out.
        cmn::vector<unsigned> cliffs;
        cliffs.Allocate(numX * numZ);

        for (int x = x0; x <= x1; ++x)
        {
            if (z0 > z1)
            {
                continue;
            }
            unsigned key = x + (z0 << 8);
            for (int k = 0; k < numZ; ++k)
            {
                cells.push_back(key);
                key += 0x100u;
            }
        }

        if (m_renderMode != RM_GAME)
        {
            DrawCellsOverlayedEditor(cells, clr);
        }
        else
        {
            DrawCells(cells, clr);
        }

        // NOTE: a push immediately followed by its pop - the shipped code leaves
        // the cull mode exactly as it found it.
        M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
        M3D_RENDERER->PopCull();
    }

    int Landscape::RecalcNormalMap(int x, int y, int sizex, int sizey)
    {
        // RVA 0x5AE6D0
        for (int row = y; row < y + sizey; ++row)
        {
            for (int col = x; col < x + sizex; ++col)
            {
                if (col >= 0 && row >= 0 && col < m_mapSize && row < m_mapSize)
                {
                    m_vnormal[row * (m_mapSize + 1) + col] =
                        getNormal(static_cast<float>(col) * 8.0f, static_cast<float>(row) * 8.0f);
                }
            }
        }
        return 1;
    }

    void Landscape::ChangeShoreState(int x, int z, bool enable)
    {
        // RVA 0x5C3580
        unsigned const key = x + (z << 16);
        if (enable)
        {
            m_noShoresSet.erase(key);
        }
        else
        {
            m_noShoresSet.insert(key);
        }
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

    void Landscape::CheckLandscapeCollisionTriMeshesForObjId(int objId)
    {
        // RVA 0x64A280
        // A debug check that no collision cell still lists an object that has died.
        int const size = m_owner->m_level->land_size;
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                for (int id : m_oCollisionitems[x + y * size]->m_physicObjIds)
                {
                    if (id == objId)
                    {
                        M3D_LOG_INFO(
                            CStr("Error: Dead objId: ") + CStr(objId) + CStr(" in cell (") + CStr(x) + CStr(", ") +
                            CStr(y) + CStr(")"));
                        SYS_ERROR("!\"Error in checking for dead obj ids, see log\"");
                    }
                }
            }
        }
    }

    GeomObject* Landscape::GetTerrainGeomObject() const
    {
        return this->m_terrainObject;
    }

    void Landscape::GetDPVSCollisionInfo(int, int, int& numVertices, int& numIndices)
    {
        // RVA 0x644C40
        // A fixed 4x4 block of 4x4-quad patches: 25 vertices and 96 indices each.
        for (int i = 0; i < 16; ++i)
        {
            numVertices += 25;
            numIndices += 96;
        }
    }

    void Landscape::RemoveGrassRadius(CVector const& pos, float radius)
    {
        // RVA 0x6AFAB0
        if (!m_grassArray)
        {
            return;
        }

        // The upper bounds are exclusive and one past the tile holding the edge.
        int tileMinX = static_cast<int>((pos.x - radius) * (1.0f / 32.0f));
        int tileMinY = static_cast<int>((pos.z - radius) * (1.0f / 32.0f));
        int tileMaxX = static_cast<int>((pos.x + radius) * (1.0f / 32.0f)) + 1;
        int tileMaxY = static_cast<int>((pos.z + radius) * (1.0f / 32.0f)) + 1;
        tileMinX = std::clamp(tileMinX, 0, 256);
        tileMinY = std::clamp(tileMinY, 0, 256);
        tileMaxX = std::clamp(tileMaxX, 0, 256);
        tileMaxY = std::clamp(tileMaxY, 0, 256);

        auto const inRadius = [&](GrassInstance const* gi)
        {
            double const dx = gi->pos.x - pos.x;
            double const dz = gi->pos.z - pos.z;
            return radius > sqrt(dx * dx + dz * dz);
        };
        for (int x = tileMinX; x < tileMaxX; ++x)
        {
            for (int y = tileMinY; y < tileMaxY; ++y)
            {
                TileGrass*& tile = m_grassArray[x + (y << 8)];
                if (!tile)
                {
                    continue;
                }
                for (unsigned i = 0; tile && i < tile->instancesPerModel.size(); ++i)
                {
                    GrassInstancesForModel* perModel = tile->instancesPerModel[i];
                    if (!perModel)
                    {
                        continue;
                    }
                    for (unsigned k = 0; k < perModel->grass.size(); ++k)
                    {
                        GrassInstance* gi = perModel->grass[k];
                        if (!gi || !(inRadius(gi)))
                        {
                            continue;
                        }
                        delete gi;
                        perModel->grass[k] = nullptr;
                        // NOTE: the tile's numInstances is not decremented.
                        bool modelDeleted = false;
                        if (perModel->numInstances-- == 1)
                        {
                            delete tile->instancesPerModel[i];
                            tile->instancesPerModel[i] = nullptr;
                            --tile->numDiffModels;
                            modelDeleted = true;
                        }
                        if (!tile->numDiffModels)
                        {
                            delete tile;
                            tile = nullptr;
                        }
                        // NOTE: the original goes on reading the freed model and tile; their
                        // destructors have just nulled the vectors, so its loops end here.
                        if (modelDeleted)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }

    void Landscape::DisableShoreRegion(int x, int z)
    {
        // RVA 0x5C3510
        int const side = 4 * m_owner->m_level->land_size;
        std::vector<unsigned char> visited(side * side, 0);
        RecursiveDisableShore(visited.data(), x, z);
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
                auto& graph = m_owner->GetGraph();
                graph.SortedCellsStartFetching(0, m_drawRadius);
                int x, y, vis, radius;
                while (graph.SortedCellsFetch(x, y, vis, radius))
                {
                    if (!vis)
                    {
                        continue;
                    }

                    for (int yy = 0; yy < 4; ++yy)
                    {
                        if (m_numWaterCells)
                        {
                            break;
                        }

                        for (int xx = 0; xx < 4; ++xx)
                        {
                            if (m_waterMap[4 * x + 4 * m_owner->m_level->land_size * (yy + 4 * y) + xx])
                            {
                                m_numWaterCells = 1;
                                break;
                            }
                        }
                    }
                }
            }
            if (m_numWaterCells)
            {
                // TODO: check this!!!
                // Create reflection matrix for water plane
                CMatrix mirror;
                CVector const& normal = m_waterPlane.m_normal;
                float dist = m_waterPlane.m_dist;

                mirror._11 = 1.0f - 2.0f * normal.x * normal.x;
                mirror._12 = -2.0f * normal.x * normal.y;
                mirror._13 = -2.0f * normal.x * normal.z;
                mirror._14 = 0.0f;

                mirror._21 = -2.0f * normal.x * normal.y;
                mirror._22 = 1.0f - 2.0f * normal.y * normal.y;
                mirror._23 = -2.0f * normal.y * normal.z;
                mirror._24 = 0.0f;

                mirror._31 = -2.0f * normal.x * normal.z;
                mirror._32 = -2.0f * normal.y * normal.z;
                mirror._33 = 1.0f - 2.0f * normal.z * normal.z;
                mirror._34 = 0.0f;

                mirror._41 = 2.0f * dist * normal.x;
                mirror._42 = 2.0f * dist * normal.y;
                mirror._43 = 2.0f * dist * normal.z;
                mirror._44 = 1.0f;

                // Get current view matrix and apply reflection
                CMatrix viewMatrix = M3D_RENDERER->MatGet();
                CMatrix im = mirror * viewMatrix;

                // Get viewport dimensions
                m3d::rend::Viewport viewport = M3D_RENDERER->GetViewport();
                float width = static_cast<float>(viewport.m_width);
                float height = static_cast<float>(viewport.m_height);

                // Calculate FOV based on aspect ratio
                float fovX, fovY;
                if (width <= height)
                {
                    fovX = 0.78539819f;
                    fovY = (width / height) * 0.78539819f;
                }
                else
                {
                    fovX = (width / height) * 0.78539819f;
                    fovY = 0.78539819f;
                }

                // Calculate reflection draw distance
                float distanceDivider = M3D_ENGINE_CFG.m_lsTransitionDevider.GetF();
                float reflectionModifier = M3D_ENGINE_CFG.m_g_reflectionDrawDistModifier.GetF();

                float reflectionDistance = (distanceDivider / reflectionModifier) * 8.0f + 4.0f;

                // Create reflected frustum
                CMatrix inverseMatrix = im.getInverse();
                CVector origin = inverseMatrix.getOrg();

                float const VISCELL_EDGE_LENGTH_24 = 128.0f;

                m_reflectedFrustum.createScreenFrustums(
                    origin, im, fovX, fovY, 1.0f, reflectionDistance * VISCELL_EDGE_LENGTH_24);

                // Enable visible cells for reflection
                m_owner->m_sceneGraph.EnableVisibleCells(m_reflectedFrustum, 2);
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
        // RVA 0x5C5FE0
        // Traces where the water plane cuts the terrain, marching squares style: in every
        // 8x8 height cell the plane crosses, it enters through one edge and leaves through
        // another, and the crossings are chained into lines. Edges are numbered 1 (z1),
        // 2 (x2), 3 (z2), 4 (x1).
        float const LAND_SCALE = 8.0f;
        std::set<unsigned> checked;
        m_shoreLines.clear();
        int const wsize = 4 * m_owner->m_level->land_size;
        int const lsSize = m_mapSize + 1;

        auto const waterHeightAt = [&](int cx, int cz) -> float
        {
            int const wx = cx / 4;
            int const wz = cz / 4;
            int const side = 4 * m_owner->m_level->land_size;
            if (wx < 0 || wx >= side || wz < 0 || wz >= side)
            {
                return 0.0f;
            }
            return static_cast<float>(m_waterMap[wx + wz * side]) * 0.12f;
        };
        // Where the edge o->e crosses the plane y = waterH, strictly between its ends.
        auto const crossWater = [](CVector const& o, CVector const& e, float waterH, CVector& hit) -> bool
        {
            float const dx = e.x - o.x;
            float const dy = e.y - o.y;
            float const dz = e.z - o.z;
            float const lenSq = dx * dx + dy * dy + dz * dz;
            float const inv = static_cast<float>(1.0 / sqrt(lenSq + 1.1920929e-7));
            CVector const dir(dx * inv, dy * inv, dz * inv);
            float const len = static_cast<float>(sqrt(lenSq));
            float const num = waterH - o.y;
            float const den = dir.y;
            if (fabs(num) < 1e-5f || fabs(den) < 1e-5f)
            {
                return false;
            }
            float const t = num / den;
            if (!(t > 0.0f && len > t))
            {
                return false;
            }
            hit = CVector(dir.x * t + o.x, dir.y * t + o.y, dir.z * t + o.z);
            return true;
        };
        // The edge a neighbour is entered through when it is left through the given one.
        auto const opposite = [](int edge, int& to)
        {
            switch (edge)
            {
            case 1:
                to = 3;
                break;
            case 2:
                to = 4;
                break;
            case 3:
                to = 1;
                break;
            case 4:
                to = 2;
                break;
            default:
                break;
            }
        };

        for (int z = 0; z < lsSize; ++z)
        {
            float const z1 = static_cast<float>(z) * 8.0f;
            float const z2 = z1 + 8.0f;
            for (int x = 0; x < lsSize; ++x)
            {
                float const x1 = static_cast<float>(x) * LAND_SCALE;
                float const x2 = x1 + LAND_SCALE;
                float const h00 = GetLsHeight(x1, z1);
                float const h10 = GetLsHeight(x2, z1);
                float const h01 = GetLsHeight(x1, z2);
                float const h11 = GetLsHeight(x2, z2);
                float const waterH = waterHeightAt(x, z);
                // NOTE: the last test reads the water map without a bounds check, past its
                // end on the last row and column.
                if ((h00 >= waterH && h10 >= waterH && h01 >= waterH && h11 >= waterH) ||
                    (waterH >= h00 && waterH >= h10 && waterH >= h01 && waterH >= h11) ||
                    !m_waterMap[x / 4 + wsize * (z / 4)])
                {
                    continue;
                }

                // Follow the line from this cell; when it ends (edge of the map, a visited
                // cell, a switched-off shore) go back to the start and follow it the other
                // way, prepending.
                std::vector<CVector> shoreline;
                int curx = x;
                int curz = z;
                int from = 0;
                int firstfrom = 0;
                bool secondpart = false;
                auto const restart = [&]
                {
                    secondpart = true;
                    curx = x;
                    curz = z;
                    opposite(firstfrom, from);
                };
                while (true)
                {
                    if (!(curx >= 0 && curx < lsSize && curz >= 0 && curz < lsSize))
                    {
                        if (secondpart)
                        {
                            break;
                        }
                        restart();
                    }
                    if (checked.count(curx + (curz << 16)))
                    {
                        if (secondpart)
                        {
                            if (curx != x || curz != z)
                            {
                                break;
                            }
                        }
                        else
                        {
                            restart();
                        }
                    }
                    if (m_noShoresSet.count(curx / 4 + ((curz / 4) << 16)))
                    {
                        if (secondpart)
                        {
                            break;
                        }
                        restart();
                    }

                    float const cx1 = static_cast<float>(curx) * 8.0f;
                    float const cz1 = static_cast<float>(curz) * 8.0f;
                    float const cx2 = cx1 + 8.0f;
                    float const cz2 = cz1 + 8.0f;
                    float const c00 = GetLsHeight(cx1, cz1);
                    float const c10 = GetLsHeight(cx2, cz1);
                    float const c01 = GetLsHeight(cx1, cz2);
                    float const c11 = GetLsHeight(cx2, cz2);
                    float const cellWaterH = waterHeightAt(curx, curz);

                    int to1 = 0;
                    int to2 = 0;
                    CVector hp1;
                    CVector hp2;
                    auto const tryEdge = [&](int edge, CVector const& o, CVector const& e)
                    {
                        CVector hit;
                        if (!crossWater(o, e, cellWaterH, hit))
                        {
                            return;
                        }
                        if (!to1)
                        {
                            to1 = edge;
                            hp1 = hit;
                        }
                        else if (!to2)
                        {
                            to2 = edge;
                            hp2 = hit;
                        }
                    };
                    tryEdge(1, CVector(cx1, c00, cz1), CVector(cx2, c10, cz1));
                    tryEdge(2, CVector(cx2, c10, cz1), CVector(cx2, c11, cz2));
                    tryEdge(3, CVector(cx2, c11, cz2), CVector(cx1, c01, cz2));
                    tryEdge(4, CVector(cx1, c01, cz2), CVector(cx1, c00, cz1));
                    if (!to1)
                    {
                        break;
                    }

                    // Leave through the crossing we did not come in by.
                    int exitEdge;
                    CVector p;
                    CVector pp;
                    if (from == to1)
                    {
                        exitEdge = to2;
                        p = hp2;
                        pp = hp1;
                    }
                    else
                    {
                        exitEdge = to1;
                        p = hp1;
                        pp = hp2;
                    }
                    if (!from)
                    {
                        shoreline.push_back(pp);
                        opposite(exitEdge, firstfrom);
                    }

                    unsigned const key = curx + (curz << 16);
                    CVector const first = shoreline.front();
                    // NOTE: the loop is taken as closed when x and y match; z is not compared.
                    if (first.x == p.x && first.y == p.y)
                    {
                        if (!secondpart)
                        {
                            shoreline.push_back(p);
                            checked.insert(key);
                            break;
                        }
                    }
                    else
                    {
                        // Points closer than 10 units to the last one are dropped.
                        // NOTE: the second half prepends, but still measures against the back.
                        CVector const& ep = shoreline.back();
                        double const ex = ep.x - p.x;
                        double const ey = ep.y - p.y;
                        double const ez = ep.z - p.z;
                        if (sqrt(ez * ez + ey * ey + ex * ex) > 10.0)
                        {
                            if (secondpart)
                            {
                                shoreline.insert(shoreline.begin(), p);
                            }
                            else
                            {
                                shoreline.push_back(p);
                            }
                        }
                    }
                    checked.insert(key);

                    // NOTE: a cell with a single crossing leaves exitEdge 0 and is processed
                    // again in place.
                    switch (exitEdge)
                    {
                    case 1:
                        --curz;
                        from = 3;
                        break;
                    case 2:
                        ++curx;
                        from = 4;
                        break;
                    case 3:
                        ++curz;
                        from = 1;
                        break;
                    case 4:
                        --curx;
                        from = 2;
                        break;
                    default:
                        break;
                    }
                }

                if (shoreline.size() > 2)
                {
                    m_shoreLines.push_back(shoreline);
                }
            }
        }

        ReBuildShoresVb();
        return 1;
    }

    void Landscape::ReleaseOdeCollisionData()
    {
        // TODO: check this
        if (m_oCollisionitems)
        {
            m_owner->m_roadManager.ReleaseCollision();

            auto const landSize = m_owner->m_level->land_size;
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
        // RVA 0x6AB5A0
        return 32.0f;
    }

    void Landscape::SwitchDrawMode()
    {
        // RVA 0x5AFC10
        // Toggles the wireframe cvar, respecting its read only flag.
        CVar& wireframe = M3D_ENGINE_CFG.m_lsWireframe;
        bool const isWireframe = wireframe.GetB();
        char Buffer[4];
        sprintf(Buffer, "%d", !isWireframe);
        wireframe.Set(Buffer, false);
    }

    void Landscape::RemoveCollisionTris(int tag)
    {
        // RVA 0x64BE70
        // A negative tag removes everything; otherwise only the first mesh with that tag.
        if (tag >= 0)
        {
            for (auto it = m_collisions.begin(); it != m_collisions.end(); ++it)
            {
                // NOTE: the entries are not checked for null.
                if ((*it)->m_tag == tag)
                {
                    delete *it;
                    *it = nullptr;
                    m_collisions.erase(it);
                    return;
                }
            }
        }
        else
        {
            for (auto& collision : m_collisions)
            {
                delete collision;
                collision = nullptr;
            }
            CollInfoVec().swap(m_collisions);
        }
    }

    Object* Landscape::CreateObject()
    {
        // RVA 0x5BCF20
        return new Landscape();
    }

    void Landscape::getMinMaxHeightForBox(float* box, float buldgeY)
    {
        // TODO: generated code
        float constexpr VISCELL_EDGE_LENGTH = 128.0f;
        float const invCellSize = 1.0f / VISCELL_EDGE_LENGTH;

        // Calculate grid indices for bounding box
        unsigned const minGridX = static_cast<unsigned>(box[0] * invCellSize);
        unsigned const minGridZ = static_cast<unsigned>(box[2] * invCellSize);
        float const maxGridX = box[3] * invCellSize;
        float const maxGridZ = box[5] * invCellSize;

        unsigned const landSize = m_owner->m_level->land_size;

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

                auto const& cell = m_cellParams[gridX + gridZ * landSize];

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
        // RVA 0x5AADE0
        m_renderMode = RM_GAME;
    }

    void Landscape::CollectGrassCell(
        int x,
        int z,
        unsigned& numVisibleInstances,
        GrassInstance** visGrassInstancesArr,
        int* visModelsForGrassInstancesArr)
    {
        // RVA 0x6AE710 - a visibility cell holds 4x4 grass tiles; each surviving
        // tile contributes its instances, three rejection tests deep.
        CMatrix const viewMatrix = M3D_RENDERER->MatGet();
        CMatrix const invView = viewMatrix.getInverse();
        CVector const cameraPos = invView.getOrg();

        float const drawDist = M3D_ENGINE_CFG.m_g_grassDrawDist.GetF();

        for (int tz = 0; tz < GRASS_TILES_PER_CELL; ++tz)
        {
            int const gz = 4 * z + tz;
            for (int tx = 0; tx < GRASS_TILES_PER_CELL; ++tx)
            {
                int const gx = 4 * x + tx;

                TileGrass* grassTile = m_grassArray[(z << 10) + gx + (tz << 8)];
                if (!grassTile)
                {
                    continue;
                }

                // Tile wide reject first: a sphere around the tile centre that
                // comfortably contains a 32 unit square of grass.
                CVector o;
                o.x = (static_cast<float>(gx) + 0.5f) * 32.0f;
                o.z = (static_cast<float>(gz) + 0.5f) * 32.0f;
                o.y = GetLsHeight(o.x, o.z);
                if (!m_frustumCull.testSphere(o, GRASS_TILE_BOUND_RADIUS))
                {
                    ++numTilesRejectedSphere;
                    continue;
                }

                for (unsigned m = 0; m < grassTile->instancesPerModel.size(); ++m)
                {
                    GrassInstancesForModel* perModel = grassTile->instancesPerModel[m];
                    if (!perModel)
                    {
                        continue;
                    }

                    for (unsigned i = 0; i < perModel->grass.size(); ++i)
                    {
                        if (numVisibleInstances == MAX_VISIBLE_GRASS_INSTANCES)
                        {
                            break;
                        }

                        GrassInstance* gi = perModel->grass[i];
                        if (!gi)
                        {
                            continue;
                        }

                        float const dx = gi->pos.x - cameraPos.x;
                        float const dy = gi->pos.y - cameraPos.y;
                        float const dz = gi->pos.z - cameraPos.z;
                        if ((dz * dz + dy * dy) + dx * dx > drawDist * drawDist)
                        {
                            ++numInstancesFarAway;
                            continue;
                        }

                        // Dot the camera-to-blade vector against the view
                        // direction; anything behind the eye is dropped before
                        // the more expensive frustum test.
                        float const behind =
                            ((cameraPos.y - gi->pos.y) * viewMatrix._23 + (cameraPos.z - gi->pos.z) * viewMatrix._33) +
                            (cameraPos.x - gi->pos.x) * viewMatrix._13;
                        if (behind > 0.0f)
                        {
                            ++numInstancesBehindCamera;
                            continue;
                        }

                        float const radius = m_grassModels[perModel->modelId].boundRadius * gi->scale;
                        if (!m_frustumCull.testSphere(gi->pos, radius))
                        {
                            ++numInstancesRejected;
                            continue;
                        }

                        visGrassInstancesArr[numVisibleInstances] = gi;
                        visModelsForGrassInstancesArr[numVisibleInstances] = perModel->modelId;
                        ++numVisibleInstances;
                    }
                }
            }
        }
    }

    void Landscape::DrawGeom(dxGeom* geom)
    {
        // TODO: generated code Landscape::DrawGeom
        dxGeom* currentGeom = geom;
        int geomClass = dGeomGetClass(geom);

        if (geomClass >= 0)
        {
            if (geomClass <= 2)  // Simple geometry types (0, 1, 2)
            {
                // Get position
                float const* positionData = dGeomGetPosition(geom);
                CVector const pos(positionData[0], positionData[1], positionData[2]);

                // Get quaternion orientation
                float quat[4];
                dGeomGetQuaternion(geom, quat);
                Quaternion rot(quat[1], quat[2], quat[3], quat[0]);

                CMatrix const mat1 = rot.ToMatrix();
                CMatrix mat2;
                mat2.setOrg(pos);

                CMatrix const worldMatrix = (mat1 * mat2) + mat1;

                m3d::Application::g_pApp->m_renderer->MatPushWorld();
                m3d::Application::g_pApp->m_renderer->MatSetWorld(worldMatrix);
            }
            else if (geomClass == 6)  // Transform geometry
            {
                // Get position
                float const* positionData = dGeomGetPosition(geom);
                CVector const pos(positionData[0], positionData[1], positionData[2]);

                // Get quaternion orientation
                float quat[4];
                dGeomGetQuaternion(geom, quat);
                Quaternion rot(quat[1], quat[2], quat[3], quat[0]);

                // Convert transform's quaternion to matrix
                CMatrix rotMat = rot.ToMatrix();
                CMatrix posMat;
                posMat.setOrg(pos);

                CMatrix const geomMatrix = (rotMat * posMat) + rotMat;

                // Now get the encapsulated geometry
                currentGeom = dGeomTransformGetGeom(geom);

                // Get position
                float const* transformPositionData = dGeomGetPosition(currentGeom);
                CVector const transformPos(
                    transformPositionData[0], transformPositionData[1], transformPositionData[2]);

                // Get quaternion orientation
                dGeomGetQuaternion(currentGeom, quat);
                Quaternion transformRot(quat[1], quat[2], quat[3], quat[0]);

                // Convert transform's quaternion to matrix
                CMatrix transformRotMat = transformRot.ToMatrix();
                CMatrix transformPosMat;
                transformPosMat.setOrg(transformPos);

                CMatrix const transformMatrix = (transformRotMat * transformPosMat) + transformRotMat;

                // Combine matrices
                CMatrix combinedMatrix = transformMatrix * geomMatrix;

                // Apply combined matrix to renderer
                m3d::Application::g_pApp->m_renderer->MatPushWorld();
                m3d::Application::g_pApp->m_renderer->MatSetWorld(combinedMatrix);
            }
        }

        // Draw the actual geometry
        DrawNonTransformGeom(currentGeom);

        // Restore world matrix unless it's class 7
        if (geomClass != 7)
        {
            m3d::Application::g_pApp->m_renderer->MatPopWorld();
        }
    }

    void Landscape::Update()
    {
    }

    void Landscape::DrawWaterLayer()
    {
        // TODO: generated code Landscape::DrawWaterLayer
        M3D_RENDERER->TgSetTcSource(0, rend::TC_FROM_VERTEX, 0);
        M3D_RENDERER->TgSetTcSource(1, rend::TC_FROM_VERTEX, 1);
        M3D_RENDERER->TgSetTcSource(2, rend::TC_FROM_VERTEX, 2);
        M3D_RENDERER->TgSetTcSource(3, rend::TC_FROM_VERTEX, 3);
        M3D_RENDERER->SetBlend(rend::BM_NONE, 0);
        if (M3D_ENGINE_CFG.m_r_enableFog.GetB())
        {
            M3D_RENDERER->PushFog(true);
        }
        M3D_RENDERER->SetAlphaTest(0);
        M3D_RENDERER->SetLighting(0, 0);

        if (M3D_ENGINE_CFG.m_r_waterInQuery.GetB())
        {
            m_currWaterQuery = (m_currWaterQuery + 1) % 3;
            m_waterQueries[m_currWaterQuery]->Begin();
        }

        switch (m_waterShaderVersion)
        {
        case 14:
            // ps1.4: the wave bump map twice, then reflection and refraction.
            M3D_RENDERER->SetTexture(0, m_waveBumpTex, -1.0);
            M3D_RENDERER->SetTexture(1, m_waveBumpTex, -1.0);
            M3D_RENDERER->SetTexture(2, m_texRtReflection, -1.0);
            M3D_RENDERER->SetTexture(3, m_texRtRefraction, -1.0);
            M3D_RENDERER->DisableTextureStages(4);
            break;

        case 11:
        {
            // ps1.1: as ps1.4, with the bump strength scaled by the cvars.
            M3D_RENDERER->SetTexture(0, m_waveBumpTex, -1.0);
            M3D_RENDERER->SetTexture(1, m_waveBumpTex, -1.0);
            M3D_RENDERER->SetTexture(2, m_texRtReflection, -1.0);
            M3D_RENDERER->SetTexture(3, m_texRtRefraction, -1.0);
            M3D_RENDERER->DisableTextureStages(4);
            auto const cvarF = [](CVar const& cvar)
            {
                return cvar.GetType() == CVar::CVAR_FLOAT ? cvar.GetF() : static_cast<float>(cvar.GetI());
            };
            float const reflectionAmount = cvarF(M3D_ENGINE_CFG.m_g_ps11_water_reflection_amount);
            float const refractionAmount = cvarF(M3D_ENGINE_CFG.m_g_ps11_water_refraction_amount);
            M3D_RENDERER->Set2x2BumpMatrix(2, reflectionAmount, 0.0f, 0.0f, reflectionAmount);
            M3D_RENDERER->Set2x2BumpMatrix(3, refractionAmount, 0.0f, 0.0f, refractionAmount);
            break;
        }

        case 20:
            M3D_RENDERER->SetTexture(0, m_waveBumpTex, -1.0);
            M3D_RENDERER->SetTexture(1, m_waveBumpSmTex, -1.0);
            M3D_RENDERER->SetTexture(2, m_fresnelTex, -1.0);
            M3D_RENDERER->SetTexture(3, m_texRtReflection, -1.0);
            M3D_RENDERER->SetTexture(4, m_texRtRefraction, -1.0);
            break;

        default:
            // No other version binds anything.
            break;
        }

        CMatrix const mat = M3D_RENDERER->MatGet();
        CMatrix const projMat = M3D_RENDERER->MatGetProj();
        CMatrix const resultViewMat = mat * projMat;
        CMatrix const textureMat = resultViewMat * m_matScale;

        unsigned const viewProjHandle = m_waterVs->GetParamHandleByName("mViewProj");
        m_waterVs->SetMatrix(viewProjHandle, resultViewMat);

        unsigned const textureHandle = m_waterVs->GetParamHandleByName("mTexture");
        m_waterVs->SetMatrix(textureHandle, textureMat);

        rend::Colorf const reflectionTint = m_owner->m_level->m_reflectionTint;
        rend::Colorf const refractionTint = m_owner->m_level->m_refractionTint;

        if (m_waterShaderVersion == 20)
        {
            Weather const* currentWeather = m_owner->GetWeatherManager().GetActiveWeather();

            auto const colorSpec = (uint8_t)m_owner->GetWeatherSpecularColor();
            CVector4 waveHeightSpecular(
                currentWeather->m_waterHeightSmall,
                currentWeather->m_waterHeightBig,
                currentWeather->m_waterSpecularS * (float)colorSpec * 0.0039215689,
                currentWeather->m_waterSpecularM);

            unsigned const waveHeightSpecularHandle = m_waterPs->GetParamHandleByName("waveHeightSpecular");
            m_waterPs->SetVector4(waveHeightSpecularHandle, waveHeightSpecular);

            // Set view position
            CMatrix invView = mat.getInverse();
            CVector viewPos = invView.getOrg();

            unsigned const viewPosHandle = m_waterVs->GetParamHandleByName("viewPos");
            m_waterVs->SetVector3(viewPosHandle, viewPos);

            // Set wave parameters
            CVector4 waveSize(
                currentWeather->m_waterSizeSmall,
                currentWeather->m_waterSizeBig,
                cos(currentWeather->m_waterCourseAng),
                sin(currentWeather->m_waterCourseAng));
            unsigned const waveSizeHandle = m_waterVs->GetParamHandleByName("waveSize");
            m_waterVs->SetVector4(waveSizeHandle, waveSize);

            // Set sun direction and fog color
            CVector sunDir = m_owner->m_sunDir * 20000.0f;
            M3D_RENDERER->SetVsFloatConst(15, reinterpret_cast<float const*>(&sunDir), 1);

            M3D_RENDERER->SetPsFloatConst(4, &currentWeather->m_currentColors[4][0], 1);
        }
        else
        {
            // The ps1.x shaders take a fixed fresnel curve and the level's tints.
            m_waterVs->SetFloat(m_waterVs->GetParamHandleByName("fresnelBias"), 0.25f);
            m_waterVs->SetFloat(m_waterVs->GetParamHandleByName("fresnelScale"), 1.0f);
            m_waterVs->SetFloat(m_waterVs->GetParamHandleByName("fresnelPower"), 4.0f);
            CVector const viewPos = mat.getInverse().getOrg();
            m_waterVs->SetVector3(m_waterVs->GetParamHandleByName("ViewPos"), viewPos);
            unsigned const reflectionRegister = m_waterShaderVersion == 14 ? 5 : 0;
            unsigned const refractionRegister = m_waterShaderVersion == 14 ? 6 : 1;
            M3D_RENDERER->SetPsFloatConst(reflectionRegister, reinterpret_cast<float const*>(&reflectionTint), 1);
            M3D_RENDERER->SetPsFloatConst(refractionRegister, reinterpret_cast<float const*>(&refractionTint), 1);
        }

        // Set fog parameters
        float fogStart, fogEnd;
        GetFogStartAndEnd(fogStart, fogEnd);

        float fogReduceFactor = m_owner->GetFogReduceFactorFromWeather();
        CVector fogTerm(
            fogReduceFactor * fogEnd,
            1.0f / (fogReduceFactor * fogEnd - fogReduceFactor * fogStart),
            fogReduceFactor * fogStart);

        unsigned const gFogTermHandle = m_waterVs->GetParamHandleByName("g_FogTerm");
        m_waterVs->SetVector3(gFogTermHandle, fogTerm);

        // Clear water cells to draw
        for (int i = 0; i < 16; ++i)
        {
            waterCellsToDraw[i].clear();
        }

        // Collect visible water cells
        CMatrix invView = mat.getInverse();
        int landSize = m_owner->m_level->land_size;

        for (int x = 0; x < landSize; ++x)
        {
            for (int y = 0; y < landSize; ++y)
            {
                // Check if cell is enabled in scene graph
                if (m_owner->m_sceneGraph.IsCellEnabled(x, y) == 0)
                {
                    continue;
                }

                // Process sub-cells (4x4 grid within each cell)
                for (int subY = 0; subY < 4; subY++)
                {
                    int waterMapY = subY + 4 * y;

                    for (int subX = 0; subX < 4; subX++)
                    {
                        int waterMapX = subX + 4 * x;

                        // Check if this cell has water
                        if (m_waterMap[4 * waterMapY * landSize + waterMapX])
                        {
                            int waterMapStride = 4 * landSize;

                            // Default to highest LOD (most detailed)
                            int lodLevel = 3;

                            // Get water height with bounds checking
                            float waterHeight = 0.0f;
                            if (waterMapX >= 0 && waterMapX < waterMapStride && waterMapY >= 0 &&
                                waterMapY < waterMapStride)
                            {
                                waterHeight =
                                    static_cast<float>(m_waterMap[waterMapX + waterMapStride * waterMapY]) * 0.12f;
                            }

                            bool isShaderVersion14 = (this->m_waterShaderVersion == 14);

                            // Only calculate advanced LOD if not using shader version 14 and water quality is not low
                            if (!isShaderVersion14 && m3d::g_Kernel->GetEngineCfg().m_r_waterQuality.GetI() != 1)
                            {
                                // Calculate world position of this water cell (center of cell)
                                float worldX = (static_cast<float>(waterMapX) + 0.5f) * 32.0f;
                                float worldY = (static_cast<float>(waterMapY) + 0.5f) * 32.0f;

                                // Calculate position relative to camera
                                CVector2 cellPos;
                                cellPos.x = invView._41 - worldX;  // Camera X - cell X
                                cellPos.y = invView._43 - worldY;  // Camera Z - cell Y

                                // Calculate angle from camera to cell relative to reference vector (1,1)
                                CVector2 referenceVec(1.0f, 1.0f);
                                float angle = -CalculateAngle(cellPos, referenceVec);

                                // Normalize angle to 0-2PI range
                                if (angle <= 0.0f)
                                {
                                    angle += 6.2831855f;  // 2 * PI
                                }

                                // Convert angle to quadrant (0-3)
                                int quadrant = static_cast<int>(angle * 0.63661975f);  // Multiply by 2/PI

                                // Calculate distance to this cell (using max of absolute X/Y distances)
                                float distanceX = std::abs(cellPos.x);
                                float distanceY = std::abs(cellPos.y);
                                float distanceToCell = (distanceY <= distanceX) ? distanceX : distanceY;

                                // Determine neighbor position based on quadrant for edge detection
                                CVector2 neighborPos(worldX, worldY);
                                switch (quadrant)
                                {
                                case 0:  // Right quadrant - check neighbor above
                                    neighborPos.y += 32.0f;
                                    break;
                                case 1:  // Left quadrant - check neighbor to the left
                                    neighborPos.x -= 32.0f;
                                    break;
                                case 2:  // Down quadrant - check neighbor below
                                    neighborPos.y -= 32.0f;
                                    break;
                                case 3:  // Up quadrant - check neighbor to the right
                                    neighborPos.x += 32.0f;
                                    break;
                                }

                                // Calculate distance to neighbor
                                float neighborDeltaX = neighborPos.x - invView._41;
                                float neighborDeltaY = neighborPos.y - invView._43;
                                float neighborDistX = std::abs(neighborDeltaX);
                                float neighborDistY = std::abs(neighborDeltaY);
                                float distanceToNeighbor =
                                    (neighborDistY <= neighborDistX) ? neighborDistX : neighborDistY;

                                // Determine LOD level based on shader version and distances
                                if (this->m_waterShaderVersion == 20)
                                {
                                    // Shader version 20 LOD thresholds
                                    if (distanceToCell >= 60.0f)
                                    {
                                        lodLevel = 2;  // Medium detail
                                        if (distanceToCell >= 150.0f)
                                        {
                                            lodLevel = 3;  // Low detail
                                        }
                                    }
                                    else
                                    {
                                        lodLevel = 1;  // High detail
                                    }

                                    // Determine neighbor LOD for edge detection
                                    int neighborLOD = 1;
                                    if (distanceToNeighbor >= 60.0f)
                                    {
                                        neighborLOD = 2;
                                        if (distanceToNeighbor >= 150.0f)
                                        {
                                            neighborLOD = 3;
                                        }
                                    }

                                    // If LOD levels differ, encode quadrant information
                                    if (lodLevel != neighborLOD)
                                    {
                                        lodLevel = quadrant + 4 * lodLevel;
                                    }
                                }
                                else
                                {
                                    // Older shader versions LOD thresholds
                                    if (distanceToCell >= 100.0f)
                                    {
                                        if (distanceToCell >= 200.0f)
                                        {
                                            lodLevel = 2;  // Medium detail
                                            if (distanceToCell >= 400.0f)
                                            {
                                                lodLevel = 3;  // Low detail
                                            }
                                        }
                                        else
                                        {
                                            lodLevel = 1;  // High detail
                                        }
                                    }
                                    else
                                    {
                                        lodLevel = 0;  // Highest detail
                                    }

                                    // Determine neighbor LOD for edge detection
                                    int neighborLOD = 0;
                                    if (distanceToNeighbor >= 100.0f)
                                    {
                                        if (distanceToNeighbor >= 200.0f)
                                        {
                                            neighborLOD = 2;
                                            if (distanceToNeighbor >= 400.0f)
                                            {
                                                neighborLOD = 3;
                                            }
                                        }
                                        else
                                        {
                                            neighborLOD = 1;
                                        }
                                    }

                                    // If LOD levels differ, encode quadrant information
                                    if (lodLevel != neighborLOD)
                                    {
                                        lodLevel = quadrant + 4 * lodLevel;
                                    }
                                }
                            }

                            // Add cell to appropriate LOD bucket
                            unsigned int encodedCoords =
                                static_cast<unsigned int>((waterMapY & 0xFF) << 8) | (waterMapX & 0xFF);
                            std::pair<unsigned int, float> cellData(encodedCoords, waterHeight);

                            // Add to the water cells vector for this LOD level
                            std::vector<std::pair<unsigned int, float>>& lodBucket = this->waterCellsToDraw[lodLevel];
                            lodBucket.push_back(cellData);
                        }
                    }
                }
            }
        }

        // Wave animation time; the timer's current time is read as is, without
        // advancing it.
        if (this->m_waterShaderVersion == 20)
        {
            // Scaled by the weather's water speed.
            float const waterSpeed = m_owner->GetWeatherManager().GetActiveWeather()->m_waterSpeed;
            float const timeVal =
                static_cast<float>(static_cast<double>(M3D_KERNEL->GetTimer().m_curTime) * waterSpeed * 0.001f);
            m_waterVs->SetFloat(m_waterVs->GetParamHandleByName("timeVal"), timeVal);
        }
        else
        {
            float const timeVal = static_cast<float>(static_cast<double>(M3D_KERNEL->GetTimer().m_curTime) * 0.00025f);
            m_waterVs->SetFloat(m_waterVs->GetParamHandleByName("timeVal"), timeVal);
            m_waterVs->SetFloat(m_waterVs->GetParamHandleByName("numVertsInWaterTileEdgeRec"), 0.125f);
        }

        // Setup rendering
        M3D_RENDERER->SetToStream0(m_waterVb);
        m_waterVs->Apply();

        if (this->m_waterShaderVersion == 20)
        {
            this->m_waterPs->Apply();
        }
        else
        {
            // The ps1.x water pixel shader is an assembly one, kept in a global.
            waterPs->Apply();
        }

        // Render water cells by LOD
        float distBetwVert = 4.0f;
        M3D_RENDERER->SetVsFloatConst(17, &distBetwVert, 1);

        int totalTris = 0;
        int totalDrawCalls = 0;

        for (int lod = 0; lod < 16; ++lod)
        {
            std::vector<std::pair<unsigned int, float>>& lodCells = this->waterCellsToDraw[lod];

            if (!lodCells.empty())
            {
                M3D_RENDERER->SetIndices(m_waterIb[lod], 0);

                auto cellIt = lodCells.begin();
                while (cellIt != lodCells.end())
                {
                    int cellsThisBatch = 0;
                    int maxCellsPerPass = this->m_maxWaterCellPerPass;

                    // Prepare batch of cells
                    while (cellsThisBatch < maxCellsPerPass && cellIt != lodCells.end())
                    {
                        unsigned int cellCoords = cellIt->first;
                        float height = cellIt->second;

                        // Decode coordinates and setup water tile info
                        int cellX = static_cast<int>(cellCoords & 0xFF);
                        int cellY = static_cast<int>((cellCoords >> 8) & 0xFF);

                        this->waterTileInfo[cellsThisBatch].x = static_cast<float>(cellX) * 32.0f;
                        this->waterTileInfo[cellsThisBatch].y = height;
                        this->waterTileInfo[cellsThisBatch].z = static_cast<float>(cellY) * 32.0f;
                        this->waterTileInfo[cellsThisBatch].w = 0.0f;

                        ++cellsThisBatch;
                        ++cellIt;
                    }

                    // Render batch
                    M3D_RENDERER->SetVsFloatConst(
                        20, reinterpret_cast<float const*>(this->waterTileInfo), cellsThisBatch);

                    int trisThisBatch = cellsThisBatch * this->m_wtNumTris[lod];
                    M3D_RENDERER->DrawIndexedPrimitiveShader(
                        rend::M3DPT_TRIANGLESTRIP, 0, 81 * cellsThisBatch, 0, trisThisBatch - 4);

                    totalTris += trisThisBatch - 4;
                    totalDrawCalls++;
                }
            }
        }

        // Update performance counters
        CStr triCountStr = "waterTris = " + CStr(totalTris);
        CStr dipCountStr = "waterDip = " + CStr(totalDrawCalls);
        M3D_APP->GetDbgCounterStack().DrawStringThisFrame(triCountStr.c_str());
        M3D_APP->GetDbgCounterStack().DrawStringThisFrame(dipCountStr.c_str());

        // Cleanup render states
        M3D_RENDERER->SetWhiteTexture(0);
        M3D_RENDERER->SetWhiteTexture(1);
        M3D_RENDERER->SetWhiteTexture(2);
        M3D_RENDERER->SetWhiteTexture(3);
        M3D_RENDERER->SetWhiteTexture(4);

        M3D_RENDERER->PopFog();

        // End occlusion query if active
        if (M3D_ENGINE_CFG.m_r_waterInQuery.GetB())
        {
            m_waterQueries[this->m_currWaterQuery]->End();
        }
    }

    void Landscape::CreateHelperStructures()
    {
        // TODO: generated code Landscape::CreateHelperStructures
        int const landSize = this->m_owner->m_level->land_size;
        int const sizeInCells = landSize;
        int const stride = 4 * landSize;

        // Initialize flags for the four corners of a tile
        int cornerFlags[4] = {1, 2, 4, 8};

        // Initialize maskHash
        m3d::CIntHash<unsigned int> maskHash;

        // Process each tile in the landscape
        for (int y = 0; y < landSize; ++y)
        {
            for (int xOffset = 0; xOffset < landSize; ++xOffset)
            {
                unsigned int const baseOffset = 4 * stride * y;

                // Process 4x4 subtile grid
                for (int subY = 0; subY < 4; ++subY)
                {
                    int const globalY = subY + 4 * y;
                    unsigned int currentOffset = baseOffset;

                    for (int subX = 0; subX < 4; ++subX)
                    {
                        int const globalX = 4 * xOffset + subX;

                        // Get the current tile info
                        m3d::Landscape::TileInfo* currentTile = &m_tiles[currentOffset + globalX];

                        // Clamp coordinates to valid range
                        int const clampedX = std::clamp(globalX, 0, stride - 1);
                        int const clampedY = std::clamp(globalY, 0, stride - 1);

                        // Get texture indices for the four corners of this subtile
                        int textureIndices[4];
                        textureIndices[0] = m_tiles[clampedX + stride * clampedY].m_texIndex0;
                        textureIndices[1] =
                            m_tiles[std::clamp(globalX + 1, 0, stride - 1) + stride * clampedY].m_texIndex0;
                        textureIndices[2] =
                            m_tiles[clampedX + stride * std::clamp(globalY + 1, 0, stride - 1)].m_texIndex0;
                        textureIndices[3] = m_tiles
                                                [std::clamp(globalX + 1, 0, stride - 1) +
                                                 stride * std::clamp(globalY + 1, 0, stride - 1)]
                                                    .m_texIndex0;

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
                                    if (!processedCorners[otherCorner] &&
                                        textureIndices[corner] == textureIndices[otherCorner])
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
                                        (globalX +
                                         ((globalY +
                                           ((currentTile->m_angle + (currentTile->m_texFlags[texIdx] << 8)) << 8))
                                          << 8));
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
                m3d::rend::VertexLandscape* v46 = new rend::VertexLandscape[25 * maxCells];

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
                        BuildCells0(
                            v46, *v48, vofs, this->m_cellsPerTex[textureIndex], RT_FIRSTPASSLIGHT, bankSwitchingMap);

                        BuildCells0(
                            &v46[v48->iotherPassOffset],
                            *v48,
                            vofs,
                            this->m_cellsPerTex[textureIndex],
                            RT_OTHERPASSES,
                            bankSwitchingMap);

                        // Reset cell count
                        this->m_cellsPerTex[textureIndex].clear();

                        bankSwitchingMap.push_back(vofs);

                        // Create vertex buffers
                        char* vertexDataPtr = reinterpret_cast<char*>(v46);

                        for (size_t bufferIndex = 0; bufferIndex < bankSwitchingMap.size(); ++bufferIndex)
                        {
                            int vertexCount = bankSwitchingMap[bufferIndex];

                            // Create vertex buffer
                            m3d::rend::VbHandle vb =
                                M3D_RENDERER->AddVb(rend::VERTEX_XYZNCT1_UV2_S1, vertexCount, "Landscape", 0);

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

    void Landscape::drawSpriteOverlayed2Projected(
        float cx,
        float cz,
        float hsx,
        float hsz,
        unsigned clr,
        bool all,
        CClipper const& clipper)
    {
        // RVA 0x8DFC80
        // Like drawSpriteOverlayed2, but only the 32 unit tiles whose bounding sphere is
        // inside the projector's clipper are drawn.
        // NOTE: 'all' is unused: there is no collision check and no editor path here.
        float const TILE_SIZE_INV = 0.03125f;
        int const maxIdx = 4 * m_owner->m_level->land_size - 1;
        int const x0 = std::clamp(static_cast<int>((cx - hsx) * TILE_SIZE_INV), 0, maxIdx);
        int const x1 = std::clamp(static_cast<int>((cx + hsx) * TILE_SIZE_INV), 0, maxIdx);
        int const z0 = std::clamp(static_cast<int>((cz - hsz) * TILE_SIZE_INV), 0, maxIdx);
        int const z1 = std::clamp(static_cast<int>((cz + hsz) * TILE_SIZE_INV), 0, maxIdx);

        int const numZ = z1 - z0 + 1;
        int const count = (x1 - x0 + 1) * numZ;
        float const startCenterZ = (static_cast<float>(z0) + 0.5f) * 32.0f;
        cmn::vector<unsigned> cells;
        cells.Allocate(count);
        // NOTE: as in drawSpriteOverlayed2, a second buffer is allocated and never used.
        cmn::vector<unsigned> unused;
        unused.Allocate(count);

        float centerX = (static_cast<float>(x0) + 0.5f) * 32.0f;
        for (int x = x0; x <= x1; ++x, centerX += 32.0f)
        {
            float centerZ = startCenterZ;
            unsigned key = x + (z0 << 8);
            for (int z = z0; z <= z1; ++z, centerZ += 32.0f, key += 0x100u)
            {
                CVector o;
                o.y = GetLsHeight(centerX, centerZ);
                o.x = centerX;
                o.z = centerZ;
                if (clipper.testSphere(o, 41.025642f))
                {
                    cells.push_back(key);
                }
            }
        }

        DrawCells(cells, clr);
        // NOTE: a push immediately followed by its pop, as in drawSpriteOverlayed2.
        M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
        M3D_RENDERER->PopCull();
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

    void Landscape::RemoveGrassTile(int x, int z)
    {
        // RVA 0x6B0160
        if (!m_grassArray)
        {
            return;
        }
        int const tileIdx = x + (z << 8);
        TileGrass* tile = m_grassArray[tileIdx];
        if (!tile)
        {
            return;
        }
        for (unsigned i = 0; i < tile->instancesPerModel.size(); ++i)
        {
            if (GrassInstancesForModel* perModel = tile->instancesPerModel[i])
            {
                emptyPtrContainer(perModel->grass);
            }
        }
        emptyPtrContainer(tile->instancesPerModel);
        delete tile;
        m_grassArray[tileIdx] = nullptr;
    }

    void Landscape::DrawCells(cmn::vector<unsigned> const& cellsPerTex, unsigned clr)
    {
        // RVA 0x8DF0C0 - re-emits the named tiles as flat coloured geometry
        // through the streaming vertex buffer. Each tile spans four heightmap
        // steps, so it needs a 5x5 grid of vertices.
        auto vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZC);

        int numCells = cellsPerTex.m_numItems;
        if (!numCells)
        {
            return;
        }

        unsigned const* curCell = cellsPerTex.m_data;
        int cellsToDraw = numCells;
        while (true)
        {
            cellsToDraw = std::clamp(cellsToDraw, 0, CELLS_PER_DRAW_BATCH);

            int const numVerts = VERTS_PER_TILE * cellsToDraw;
            int vofs = 0;
            auto* v = static_cast<rend::VertexXYZC*>(M3D_RENDERER->LockVbStreaming(vb, numVerts, vofs, nullptr));

            for (int i = 0; i < cellsToDraw; ++i)
            {
                int const hx = 4 * (*curCell & 0xFFu);
                int const hz = 4 * ((*curCell >> 8) & 0xFFu);
                ++curCell;

                float const* h = &m_heightMap[hx + hz * (m_mapSize + 1)];
                for (int row = 0; row < TILE_EDGE_VERTS; ++row)
                {
                    for (int col = 0; col < TILE_EDGE_VERTS; ++col)
                    {
                        v->x = static_cast<float>(hx + col) * 8.0f;
                        v->y = h[col];
                        v->z = static_cast<float>(hz + row) * 8.0f;
                        v->c = clr;
                        ++v;
                    }
                    h += m_mapSize + 1;
                }
            }

            M3D_RENDERER->UnlockVb(vb);

            int const numPrims = cellsToDraw * m_lsNumIndices[0] - 3;
            if (numPrims > 0)
            {
                M3D_RENDERER->SetToStream0(vb);
                M3D_RENDERER->SetIndices(m_landIbConst[0], vofs);
                if (overlayShader)
                {
                    M3D_RENDERER->DrawIndexedPrimitiveEffect(
                        rend::M3DPT_TRIANGLESTRIP, overlayShader, 0, numVerts, 0, numPrims);
                }
                else
                {
                    M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLESTRIP, 0, numVerts, 0, numPrims);
                }
            }

            numCells -= cellsToDraw;
            if (!numCells)
            {
                break;
            }
            cellsToDraw = numCells;
        }
    }

    void Landscape::GetVisCellHeights(float& h0, float& h1, int x, int y) const
    {
        // RVA 0x5AACD0
        CellParams const& cell = m_cellParams[x + y * m_owner->m_level->land_size];
        h0 = cell.m_h0;
        h1 = cell.m_h1;
    }

    void Landscape::CreateIndicesTriLists(int* indices, int BaseVertex, int sz)
    {
        // RVA 0x5ABA90
        // Two triangles per quad of an (sz + 1)-wide vertex grid.
        for (int row = 0; row < sz; ++row)
        {
            for (int col = 0; col < sz; ++col)
            {
                indices[0] = BaseVertex + sz + 1;
                indices[1] = BaseVertex + sz + 2;
                indices[2] = BaseVertex;
                indices[3] = BaseVertex + sz + 2;
                indices[4] = BaseVertex + 1;
                indices[5] = BaseVertex;
                indices += 6;
                ++BaseVertex;
            }
            ++BaseVertex;
        }
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
                vv._11 =
                    (float)((float)((float)(parentMat._41 * childMat._14) + (float)(parentMat._31 * childMat._13)) +
                            (float)(parentMat._21 * childMat._12)) +
                    (float)(parentMat._11 * childMat._11);
                vv._12 =
                    (float)((float)((float)(parentMat._42 * childMat._14) + (float)(parentMat._32 * childMat._13)) +
                            (float)(parentMat._22 * childMat._12)) +
                    (float)(parentMat._12 * childMat._11);
                vv._13 =
                    (float)((float)((float)(parentMat._43 * childMat._14) + (float)(parentMat._33 * childMat._13)) +
                            (float)(parentMat._23 * childMat._12)) +
                    (float)(parentMat._13 * childMat._11);
                vv._14 =
                    (float)((float)((float)(parentMat._44 * childMat._14) + (float)(parentMat._34 * childMat._13)) +
                            (float)(parentMat._24 * childMat._12)) +
                    (float)(parentMat._14 * childMat._11);
                vv._21 =
                    (float)((float)((float)(childMat._24 * parentMat._41) + (float)(childMat._23 * parentMat._31)) +
                            (float)(childMat._22 * parentMat._21)) +
                    (float)(childMat._21 * parentMat._11);
                vv._22 =
                    (float)((float)((float)(childMat._24 * parentMat._42) + (float)(childMat._23 * parentMat._32)) +
                            (float)(childMat._22 * parentMat._22)) +
                    (float)(childMat._21 * parentMat._12);
                vv._23 =
                    (float)((float)((float)(childMat._24 * parentMat._43) + (float)(childMat._23 * parentMat._33)) +
                            (float)(childMat._22 * parentMat._23)) +
                    (float)(childMat._21 * parentMat._13);
                vv._24 =
                    (float)((float)((float)(childMat._24 * parentMat._44) + (float)(childMat._23 * parentMat._34)) +
                            (float)(childMat._22 * parentMat._24)) +
                    (float)(childMat._21 * parentMat._14);
                vv._31 =
                    (float)((float)((float)(childMat._34 * parentMat._41) + (float)(childMat._33 * parentMat._31)) +
                            (float)(childMat._32 * parentMat._21)) +
                    (float)(childMat._31 * parentMat._11);
                vv._32 =
                    (float)((float)((float)(childMat._34 * parentMat._42) + (float)(childMat._33 * parentMat._32)) +
                            (float)(childMat._32 * parentMat._22)) +
                    (float)(childMat._31 * parentMat._12);
                vv._33 =
                    (float)((float)((float)(childMat._34 * parentMat._43) + (float)(childMat._33 * parentMat._33)) +
                            (float)(childMat._32 * parentMat._23)) +
                    (float)(childMat._31 * parentMat._13);
                vv._34 =
                    (float)((float)((float)(childMat._34 * parentMat._44) + (float)(childMat._33 * parentMat._34)) +
                            (float)(childMat._32 * parentMat._24)) +
                    (float)(childMat._31 * parentMat._14);
                vv._41 =
                    (float)((float)((float)(childMat._44 * parentMat._41) + (float)(childMat._43 * parentMat._31)) +
                            (float)(childMat._42 * parentMat._21)) +
                    (float)(childMat._41 * parentMat._11);
                vv._42 =
                    (float)((float)((float)(childMat._44 * parentMat._42) + (float)(childMat._43 * parentMat._32)) +
                            (float)(childMat._42 * parentMat._22)) +
                    (float)(childMat._41 * parentMat._12);
                vv._43 =
                    (float)((float)((float)(childMat._44 * parentMat._43) + (float)(childMat._43 * parentMat._33)) +
                            (float)(childMat._42 * parentMat._23)) +
                    (float)(childMat._41 * parentMat._13);
                vv._44 =
                    (float)((float)((float)(childMat._44 * parentMat._44) + (float)(childMat._43 * parentMat._34)) +
                            (float)(childMat._42 * parentMat._24)) +
                    (float)(childMat._41 * parentMat._14);

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
        // RVA 0x5ABA20
        delete[] m_cellParams;
        m_cellParams = nullptr;
        delete[] m_drawedCellParams;
        m_drawedCellParams = nullptr;
    }

    void Landscape::DrawMassBox(dMass* mass, CVector const& pos, Quaternion const& rot)
    {
        CMatrix tr;
        tr.zero();
        tr.rotTranslate(rot, pos);

        CMatrix const worldMat = tr * tr;

        M3D_RENDERER->MatPushWorld();
        M3D_RENDERER->MatSetWorld(worldMat);

        // TODO: check and refactor
        auto v11 = 1.0 / mass->mass;
        auto aa = (float)(mass->I[0] * v11) * 12.0;
        auto ba = (float)(mass->I[5] * v11) * 12.0;
        auto ca = (float)(mass->I[10] * v11) * 12.0;
        CVector size;
        size.x = sqrt((ca + ba - aa) * 0.5);
        size.y = sqrt((ca + aa - ba) * 0.5);
        size.z = sqrt((ba + aa - ca) * 0.5);

        Aabb aabb;
        aabb.m_box[0] = 0.0 - (float)(size.x * 0.5);
        aabb.m_box[1] = 0.0 - (float)(size.y * 0.5);
        aabb.m_box[2] = 0.0 - (float)(size.z * 0.5);
        aabb.m_box[3] = size.x * 0.5;
        aabb.m_box[4] = size.y * 0.5;
        aabb.m_box[5] = size.z * 0.5;

        aabb.Draw((unsigned int)mass | 0xFF000000);
        M3D_RENDERER->MatPopWorld();
    }

    bool Landscape::InitGrass()
    {
        m_grassVs = M3D_RENDERER->NewHlslShader("data/shaders/grassTest_vs11.vs", "GrassVS", rend::IHlslShader::VS_1_1);
        if (!m_grassVs)
        {
            return false;
        }

        m_grassPs = M3D_RENDERER->NewHlslShader("data/shaders/grassTest_ps11.ps", "GrassPS", rend::IHlslShader::PS_1_1);
        if (!m_grassPs)
        {
            return false;
        }

        m_grassArray = new TileGrass*[GRASS_TILE_ARRAY_SIZE];
        memset(m_grassArray, 0, GRASS_TILE_ARRAY_SIZE * sizeof(TileGrass*));

        return true;
    }

    void Landscape::StartWaterQuery()
    {
        // RVA 0x5C0580
        m_currWaterQuery = (m_currWaterQuery + 1) % 3;
        m_waterQueries[m_currWaterQuery]->Begin();
    }

    void Landscape::PutGrassToLandscape(CVector2 const& Min, CVector2 const& Max)
    {
        // RVA 0x6AE550
        // Drops every instance in the tiles touching the rectangle (not just those inside
        // it) onto the terrain.
        if (!m_grassArray)
        {
            return;
        }

        // The upper bounds are exclusive and one past the tile holding the edge.
        int tileMinX = static_cast<int>(Min.x * (1.0f / 32.0f));
        int tileMinY = static_cast<int>(Min.y * (1.0f / 32.0f));
        int tileMaxX = static_cast<int>(Max.x * (1.0f / 32.0f)) + 1;
        int tileMaxY = static_cast<int>(Max.y * (1.0f / 32.0f)) + 1;
        tileMinX = std::clamp(tileMinX, 0, 256);
        tileMinY = std::clamp(tileMinY, 0, 256);
        tileMaxX = std::clamp(tileMaxX, 0, 256);
        tileMaxY = std::clamp(tileMaxY, 0, 256);

        for (int x = tileMinX; x < tileMaxX; ++x)
        {
            for (int y = tileMinY; y < tileMaxY; ++y)
            {
                TileGrass* tile = m_grassArray[x + (y << 8)];
                if (!tile)
                {
                    continue;
                }
                for (unsigned i = 0; i < tile->instancesPerModel.size(); ++i)
                {
                    GrassInstancesForModel* perModel = tile->instancesPerModel[i];
                    if (!perModel)
                    {
                        continue;
                    }
                    for (unsigned k = 0; k < perModel->grass.size(); ++k)
                    {
                        if (GrassInstance* gi = perModel->grass[k])
                        {
                            gi->pos.y = GetLsHeight(gi->pos.x, gi->pos.z);
                        }
                    }
                }
            }
        }
    }

    int Landscape::GetNumTiles() const
    {
        return m_tilesTextures.size();
    }

    void Landscape::setDrawRadius(int clip0, int clip1, int clip2)
    {
        // RVA 0x7A43C0
        float const VISCELL_EDGE_LENGTH = 128.0f;
        float const clip1z = static_cast<float>(clip1) * VISCELL_EDGE_LENGTH;
        float const clip0z = static_cast<float>(clip0) * VISCELL_EDGE_LENGTH;
        float const clip2z = static_cast<float>(clip2) * VISCELL_EDGE_LENGTH;
        m_landscapeClip0 = clip0;
        m_landscapeClip1 = clip1;
        m_landscapeClip2 = clip2;
        m_drawRadius = clip2;
        m_landscapeClip0z = clip0z;
        m_landscapeClip1z = clip1z;
        m_landscapeClip2z = clip2z;
        m_landscapeClip0zSq = clip0z * clip0z;
        m_landscapeClip1zSq = clip1z * clip1z;
        m_landscapeClip2zSq = clip2z * clip2z;
    }

    void Landscape::GetDrawedCellHeights(float& h0, float& h1, int x, int y) const
    {
        // RVA 0x7A4480
        CellParams const& cell = m_drawedCellParams[x + 4 * y * m_owner->m_level->land_size];
        h0 = cell.m_h0;
        h1 = cell.m_h1;
    }

    unsigned Landscape::GetNearestGrassInstance(CVector const& point) const
    {
        // RVA 0x6AE1F0
        // Only the tile under the point is searched, for instances within 8 units.
        int const tileIdx =
            static_cast<int>(point.x * (1.0f / 32.0f)) + (static_cast<int>(point.z * (1.0f / 32.0f)) << 8);
        TileGrass const* tile = m_grassArray[tileIdx];
        if (!tile)
        {
            return static_cast<unsigned>(-1);
        }

        float bestDistSq = 102400.0f;
        int bestModel = -1;
        int bestInstance = -1;
        for (unsigned m = 0; m < tile->instancesPerModel.size(); ++m)
        {
            GrassInstancesForModel const* perModel = tile->instancesPerModel[m];
            if (!perModel)
            {
                continue;
            }
            for (unsigned i = 0; i < perModel->grass.size(); ++i)
            {
                GrassInstance const* gi = perModel->grass[i];
                if (!gi)
                {
                    continue;
                }
                float const dy = point.y - gi->pos.y;
                float const distSq = (point.z - gi->pos.z) * (point.z - gi->pos.z) +
                    (point.x - gi->pos.x) * (point.x - gi->pos.x) + dy * dy;
                if (distSq <= 64.0f && bestDistSq > distSq)
                {
                    bestDistSq = distSq;
                    bestModel = static_cast<int>(m);
                    bestInstance = static_cast<int>(i);
                }
            }
        }
        if (bestModel != -1)
        {
            return bestInstance + ((bestModel + (tileIdx << 8)) << 8);
        }
        return static_cast<unsigned>(-1);
    }

    void Landscape::DoneGrass()
    {
        // RVA 0x6AFF10 - nothing is released unless the tile array exists, so a
        // landscape that never got as far as InitGrass leaves the shaders and
        // the model table alone.
        if (!m_grassArray)
        {
            return;
        }

        for (int i = 0; i < GRASS_TILE_ARRAY_SIZE; ++i)
        {
            TileGrass* tile = m_grassArray[i];
            if (!tile)
            {
                continue;
            }

            for (unsigned m = 0; m < tile->instancesPerModel.size(); ++m)
            {
                if (auto* perModel = tile->instancesPerModel[m])
                {
                    emptyPtrContainer(perModel->grass);
                }
            }
            emptyPtrContainer(tile->instancesPerModel);
            delete tile;
        }

        delete[] m_grassArray;
        m_grassArray = nullptr;

        for (unsigned i = 0; i < m_numGrassModels; ++i)
        {
            M3D_RENDERER->ReleaseVb(m_grassModels[i].vb);
            M3D_RENDERER->ReleaseIb(m_grassModels[i].ib);
            M3D_RENDERER->ReleaseTexture(m_grassModels[i].tex);
        }
        m_numGrassModels = 0;

        if (m_grassVs)
        {
            m_grassVs->Release();
            m_grassVs = nullptr;
        }
        if (m_grassPs)
        {
            m_grassPs->Release();
            m_grassPs = nullptr;
        }
    }

    float Landscape::getCameraHeight(float, float) const
    {
        // RVA 0x5AB050
        return -99999.0f;
    }

    int Landscape::getGrassModelIdByName(char const* modelFileName) const
    {
        // RVA 0x6AB690
        CStr name(modelFileName);
        name.toLower(0x400u);

        for (unsigned i = 0; i < m_numGrassModels; ++i)
        {
            if (name == m_grassModels[i].modelName)
            {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    int Landscape::SaveCameraMap(CStr const&, int)
    {
        // RVA 0x5AB040
        return 1;
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

    void Landscape::EnableShoreRegion(int x, int z)
    {
        // RVA 0x5C5DE0
        int const side = 4 * m_owner->m_level->land_size;
        std::vector<unsigned char> visited(side * side, 0);
        RecursiveEnableShore(visited.data(), x, z);
    }

    unsigned int frame = 0;
    float const VISCELL_EDGE_LENGTH_24 = 128.0;

    void Landscape::Render()
    {
        // RVA 0x7AAFF0
        if (M3D_KERNEL->GetEngineCfg().m_lsWireframe.GetB())
        {
            M3D_RENDERER->PushFillMode(rend::FillMode::M3DFILL_WIREFRAME);
        }
        else
        {
            M3D_RENDERER->PushFillMode(rend::FillMode::M3DFILL_SOLID);
        }

        int const waterQuality = M3D_ENGINE_CFG.m_r_waterQuality.GetI();

        m_waterPlane.m_normal.x = 0.0;
        m_waterPlane.m_normal.y = 1.0;
        m_waterPlane.m_normal.z = 0.0;
        m_waterPlane.m_dist = this->m_owner->m_level->waterlevel;

        m_bindDevider = ((1.1 - M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF()) * 0.44999999) + 0.55000001;

        // The draw radius is truncated to a whole number of cells before it is
        // clamped, so the clip distances derived from it below stay on cell
        // boundaries.
        int saveDistDivider =
            static_cast<int>((M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF() * 8.0f) + 4.0f);
        if (saveDistDivider < 4)
        {
            saveDistDivider = 4;
        }
        else if (saveDistDivider > 12)
        {
            saveDistDivider = 12;
        }

        M3D_RENDERER->SetLighting(false, false);
        M3D_RENDERER->PushZFunc(rend::CmpFunc::M3DCMP_LESS);
        M3D_RENDERER->PushBlend(rend::BlendMode::BM_NONE);
        M3D_RENDERER->PushCull();
        M3D_RENDERER->PushZbState();
        M3D_RENDERER->PushFog(M3D_KERNEL->GetEngineCfg().m_r_enableFog.GetB());
        if (m_numWaterCells != 0 && m_isWaterVisible)
        {
            // TODO: generated code
            // The shipped build renders only the reflection here; there is no refraction pass.
            m_profilerDrawWater->StartCountdown();

            bool const drawReflectedTerrain = M3D_ENGINE_CFG.m_g_drawReflectedTerrain.GetB();
            bool const drawReflectedModels = M3D_ENGINE_CFG.m_g_drawReflectedModels.GetB();
            // If reflection needs updating
            if (m_dirtyReflection)
            {
                m_dirtyReflection = false;
                m_curVisMode = VIS_REFLECTION;

                // Start rendering to reflection texture
                M3D_RENDERER->RenderToTexStart(m_texRtReflection, true);

                // Clear with fog color
                unsigned int fogColor = m_owner->GetWeatherFogColor();
                M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, fogColor);

                // Create reflection matrix from water plane
                CMatrix matReflect;
                matReflect.reflect(m_waterPlane);

                // Save current view and projection matrices
                CMatrix const currentView = M3D_RENDERER->GetViewMatrix();
                CMatrix saveView(currentView);

                CMatrix const currentProj = M3D_RENDERER->MatGetProj();
                CMatrix saveProj(currentProj);

                // Apply reflection matrix to view
                M3D_RENDERER->MatPush(matReflect);

                // Calculate reflected view-projection matrix
                CMatrix reflectedViewProj = saveView * matReflect;
                M3D_RENDERER->SetViewMatrix(reflectedViewProj);

                // Set up reflection projection matrix
                int screenWidth = M3D_ENGINE_CFG.m_r_width.GetI();
                int screenHeight = M3D_ENGINE_CFG.m_r_height.GetI();
                float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

                float baseFov = 0.3926990926265717f;  // ~22.5 degrees
                float fovX = std::atan2(std::tan(baseFov) * 1.1f, 1.0f);
                float fovY = fovX + fovX;

                CMatrix reflectionProj;
                reflectionProj.perspectiveFovLH(fovY, aspectRatio, 1.0f, 5000.0f);
                M3D_RENDERER->MatSetProj(reflectionProj);

                // Disable fog for reflection pass
                M3D_RENDERER->SetFog(false, 0);

                // Render sky to reflection if enabled
                bool drawSky = M3D_ENGINE_CFG.m_g_drawSky.GetB();

                if (drawSky)
                {
                    M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
                    m_owner->RenderSky(LRM_REFLECTION);
                    M3D_RENDERER->PopZbState();
                }

                // Render lens flares if enabled
                bool drawFlares = M3D_ENGINE_CFG.m_lgtFlares.GetB();

                if (drawFlares)
                {
                    m_flares.Render(FLARE_SUN, this->m_owner->m_sunDir, 1.0f, 1.0f);
                }

                // Re-enable fog based on settings
                bool enableFog = M3D_ENGINE_CFG.m_r_enableFog.GetB();

                M3D_RENDERER->SetFog(enableFog, 0);

                // Enable depth testing for main reflection rendering
                M3D_RENDERER->PushZbState(rend::ZB_ENABLE);

                // Render terrain and models to reflection (for advanced shaders)
                if (m_waterShaderVersion != 11 && (drawReflectedTerrain || drawReflectedModels) &&
                    (waterQuality == 3 || waterQuality == 2))
                {
                    // Calculate reflection draw distance
                    float viewDistanceDivider = M3D_ENGINE_CFG.m_lsViewDistanceDivider.GetF();

                    int originalDrawRadius = this->m_drawRadius;
                    float savedViewDistanceDivider = viewDistanceDivider;

                    float reflectionModifier = M3D_ENGINE_CFG.m_g_reflectionDrawDistModifier.GetF();

                    float reflectionDistanceScale = viewDistanceDivider / reflectionModifier;
                    m_drawRadius = static_cast<int>((reflectionDistanceScale * 8.0f) + 4.0f);

                    // Temporarily modify view distance for reflection
                    M3D_ENGINE_CFG.m_lsViewDistanceDivider.SetF(reflectionDistanceScale, false);

                    // Set up clipping plane below water surface
                    m3d::CWorld* world = this->m_owner;
                    m3d::Weather const* weather = world->m_weatherManager.GetActiveWeather();
                    m3d::Level* level = world->m_level;

                    float averageWaterHeight = (weather->m_waterHeightSmall + weather->m_waterHeightBig) * 2.0f;
                    float clipPlaneHeight = level->waterlevel - averageWaterHeight - 0.5f;

                    CPlane waterClipPlane;
                    waterClipPlane.m_normal = CVector(0.0f, 1.0f, 0.0f);
                    waterClipPlane.m_dist = clipPlaneHeight;

                    // Enable clipping plane if supported
                    if (M3D_RENDERER->GetMaxClipPlanes() > 0)
                    {
                        M3D_RENDERER->SetClipPlane(0, &waterClipPlane);
                        M3D_RENDERER->EnableClipPlane(0, true);
                    }

                    // Set visibility mask for reflection and cull clockwise
                    this->m_owner->m_sceneGraph.SetVisMask(2);
                    M3D_RENDERER->SetCull(rend::M3DCULL_CW, false);

                    // Render terrain to reflection
                    if (drawReflectedTerrain)
                    {
                        DrawSolidLandscape(LRM_REFLECTION, true);
                    }

                    // Update clipping plane for models
                    waterClipPlane.m_dist = level->waterlevel;

                    if (M3D_RENDERER->GetMaxClipPlanes() > 0)
                    {
                        M3D_RENDERER->SetClipPlane(0, &waterClipPlane);
                        M3D_RENDERER->EnableClipPlane(0, true);
                    }

                    M3D_RENDERER->SetCull(rend::M3DCULL_CW, false);

                    // Render models to reflection
                    if (drawReflectedModels && waterQuality != 2)
                    {
                        m_owner->m_sceneGraph.UpdateVis(false, this->m_reflectedFrustum, false);
                        m_owner->m_sceneGraph.Render(SGRF_LOW_DETAIL);
                    }

                    // Disable clipping plane
                    if (M3D_RENDERER->GetMaxClipPlanes() > 0)
                    {
                        M3D_RENDERER->EnableClipPlane(0, false);
                    }

                    // Restore original view distance settings
                    M3D_ENGINE_CFG.m_lsViewDistanceDivider.SetF(savedViewDistanceDivider, false);
                    m_drawRadius = originalDrawRadius;
                }

                // Finish reflection rendering and restore state
                M3D_RENDERER->RenderToTexFinish();
                M3D_RENDERER->PopZbState();
                M3D_RENDERER->MatPop(false);
                M3D_RENDERER->SetViewMatrix(saveView);
                M3D_RENDERER->MatSetProj(saveProj);
                this->m_owner->m_sceneGraph.SetVisMask(1);
            }
            m_profilerDrawWater->EndCountdown();
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
        if (!M3D_KERNEL->GetEngineCfg().m_dsShadows.GetB() ||
            !pClient->GetWorld().GetWeatherManager().GetShadowVisibilityFromWeather())
        {
            RenderGrass({});
        }

        if (m_numWaterCells && m_isWaterVisible)
        {
            m_profilerDrawWater->StartCountdown();

            int const waterQ = M3D_ENGINE_CFG.m_r_waterQuality.GetI();
            if (m_waterShaderVersion == 20 && (waterQ == 3 || waterQ == 2))
            {
                auto fullFrameTexture = M3D_RENDERER->GetFullFrameFrameBufferTexture();
                M3D_RENDERER->CopyRenderTargetToTexture(fullFrameTexture);
                M3D_RENDERER->RenderToTexStart(m_texRtRefraction, true);

                auto const fogColor = m_owner->GetWeatherFogColor();
                M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, fogColor & 0xFF000000);
                M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
                M3D_RENDERER->PushZbState(rend::ZB_ENABLE);
                M3D_RENDERER->PushBlend(rend::BM_NONE);

                DrawSolidLandscape(LRM_DEEPMAP, 0);

                M3D_RENDERER->PopBlend();
                M3D_RENDERER->PopZbState();
                M3D_RENDERER->PopCull();
                M3D_RENDERER->RenderToTexFinish();
            }
            else
            {
                M3D_RENDERER->CopyRenderTargetToTexture(m_texRtRefraction);
            }

            M3D_RENDERER->PushCull(rend::M3DCULL_NONE);
            M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);

            DrawWaterLayer();

            M3D_RENDERER->PopZbState();
            M3D_RENDERER->PopCull();

            m_profilerDrawWater->EndCountdown();
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
            // Debug overlay: the reflection and refraction render targets drawn
            // as two stacked quads in the top-left corner.
            M3D_RENDERER->SetBlend(rend::BlendMode::BM_NONE, false);
            M3D_RENDERER->SetAlphaTest(0);
            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
            M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
            M3D_RENDERER->DisableTextureStages(1);

            float x0 = 0.0f;
            float y0 = 0.0f;
            float x1 = 255.0f;
            float y1 = 255.0f;
            M3D_RENDERER->SetTexture(0, m_texRtReflection, -1.0);
            M3D_RENDERER->RelToAbs(x0, y0);
            M3D_RENDERER->RelToAbs(x1, y1);
            M3D_APP->PutSprite2Abs(x0, y0, 0.0f, 0.0f, x1, y1, 1.0f, 1.0f, 0xFFFFFFFF);

            x0 = 0.0f;
            y0 = 255.0f;
            x1 = 255.0f;
            y1 = 512.0f;
            M3D_RENDERER->SetTexture(0, m_texRtRefraction, -1.0);
            M3D_RENDERER->RelToAbs(x0, y0);
            M3D_RENDERER->RelToAbs(x1, y1);
            M3D_APP->PutSprite2Abs(x0, y0, 0.0f, 0.0f, x1, y1, 1.0f, 1.0f, 0xFFFFFFFF);
        }
        m_profilerDraw->EndCountdown();
    }

    int Landscape::Render(SgNodeRenderFlags, void*, int, int)
    {
        // The landscape is driven by the parameterless Render(); the shipped
        // build implements this SgNode override as a fatal error so that
        // reaching it through the node interface is reported rather than
        // silently drawing nothing.
        SYS_ERROR("0");
        return 0;
    }

    void Landscape::ReadGrassFromXmlFile(char const* fileName)
    {
        // RVA 0x6AEF30 - despite the name this reads the tagged binary grass
        // file, not XML: chunk 1 holds the counts, chunk 2 the model names and
        // chunk 3 the per tile placement.
        fs::auxTaggedFile file;
        if (file.Open(fileName, fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC) != fs::auxTaggedFile::SUCCESS)
        {
            M3D_LOG_INFO(CStr("Grass::Load - could not read file ") + fileName);
            return;
        }

        char* formatTitle = nullptr;
        file.getFormatTitle(&formatTitle);
        if (strcmp(formatTitle, "Grass"))
        {
            M3D_LOG_INFO(CStr("Grass::Load - bad format for file ") + fileName);
            return;
        }

        unsigned formatVersion = 0;
        file.getFormatVersion(formatVersion);
        if (formatVersion != 1 && formatVersion != 2)
        {
            M3D_LOG_INFO(CStr("Grass::Load - bad format version for file ") + fileName);
            return;
        }

        unsigned char* dataPtr = nullptr;
        file.getChunkData(1u, reinterpret_cast<void**>(&dataPtr));
        unsigned const numModels = *reinterpret_cast<unsigned*>(dataPtr);
        dataPtr += 4;
        unsigned const numTilesWithGrass = *reinterpret_cast<unsigned*>(dataPtr);

        file.getChunkData(2u, reinterpret_cast<void**>(&dataPtr));
        for (unsigned i = 0; i < numModels; ++i)
        {
            char const* modelName = reinterpret_cast<char const*>(dataPtr);
            if (!AddGrassModel(modelName))
            {
                M3D_LOG_INFO(CStr("Error loading level grass: could not load model ") + modelName);
                return;
            }
            dataPtr += strlen(modelName) + 1;
        }

        file.getChunkData(3u, reinterpret_cast<void**>(&dataPtr));
        for (unsigned t = 0; t < numTilesWithGrass; ++t)
        {
            unsigned const tileIdx = *reinterpret_cast<unsigned*>(dataPtr);
            dataPtr += 4;
            unsigned const numDiffModels = *reinterpret_cast<unsigned*>(dataPtr);
            dataPtr += 4;
            unsigned const numInstances = *reinterpret_cast<unsigned*>(dataPtr);
            dataPtr += 4;

            auto* tile = new TileGrass;
            m_grassArray[tileIdx] = tile;
            tile->numDiffModels = numDiffModels;
            tile->numInstances = numInstances;
            tile->instancesPerModel.resize(numDiffModels, nullptr);

            for (unsigned j = 0; j < numDiffModels; ++j)
            {
                auto* perModel = new GrassInstancesForModel;
                tile->instancesPerModel[j] = perModel;
                perModel->modelId = *reinterpret_cast<int*>(dataPtr);
                dataPtr += 4;
                perModel->numInstances = *reinterpret_cast<int*>(dataPtr);
                dataPtr += 4;
                perModel->grass.resize(perModel->numInstances, nullptr);

                for (int k = 0; k < perModel->numInstances; ++k)
                {
                    auto* gi = new GrassInstance;
                    perModel->grass[k] = gi;

                    auto const* f = reinterpret_cast<float const*>(dataPtr);
                    gi->pos.x = f[0];
                    gi->pos.y = f[1];
                    gi->pos.z = f[2];
                    if (formatVersion == 1)
                    {
                        // Version 1 has no per blade scale.
                        gi->sinYaw = f[3];
                        gi->cosYaw = f[4];
                        gi->scale = 1.0f;
                        dataPtr += 20;
                    }
                    else
                    {
                        gi->scale = f[3];
                        gi->sinYaw = f[4];
                        gi->cosYaw = f[5];
                        dataPtr += 24;
                    }
                }
            }
        }
    }

    bool Landscape::WriteGrassToXmlFile(char const* fileName)
    {
        // RVA 0x6ADED0
        // Writes the tagged binary format that ReadGrassFromXmlFile reads, version 2.
        fs::auxTaggedFile file;
        if (file.Open(fileName, fs::auxTaggedFile::CREATE_IGNORE_CRC))
        {
            M3D_LOG_INFO(CStr("Grass::Save - could not create file ") + CStr(fileName));
            return false;
        }

        file.setFormatTitle("Grass");
        file.setFormatVersion(2u);
        file.addChunk(1u);
        file.addChunkData(1u, 4u, &m_numGrassModels);
        unsigned numTilesWithGrass = 0;
        for (int i = 0; i < GRASS_TILE_ARRAY_SIZE; ++i)
        {
            if (m_grassArray[i])
            {
                ++numTilesWithGrass;
            }
        }
        file.addChunkData(1u, 4u, &numTilesWithGrass);

        file.addChunk(2u);
        for (unsigned i = 0; i < m_numGrassModels; ++i)
        {
            CStr const& name = m_grassModels[i].modelName;
            file.addChunkData(2u, name.c_str() ? static_cast<unsigned>(strlen(name.c_str())) + 1 : 1u, name.c_str());
        }

        file.addChunk(3u);
        for (unsigned tileIdx = 0; tileIdx < GRASS_TILE_ARRAY_SIZE; ++tileIdx)
        {
            TileGrass const* tile = m_grassArray[tileIdx];
            if (!tile)
            {
                continue;
            }
            file.addChunkDataCopy(3u, 4u, &tileIdx);
            file.addChunkData(3u, 4u, &tile->numDiffModels);
            // NOTE: the tile's numInstances is stale, as the grass editors never update it.
            file.addChunkData(3u, 4u, &tile->numInstances);
            for (unsigned i = 0; i < tile->instancesPerModel.size(); ++i)
            {
                GrassInstancesForModel const* perModel = tile->instancesPerModel[i];
                if (!perModel)
                {
                    continue;
                }
                file.addChunkData(3u, 4u, &perModel->modelId);
                file.addChunkData(3u, 4u, &perModel->numInstances);
                for (unsigned k = 0; k < perModel->grass.size(); ++k)
                {
                    if (perModel->grass[k])
                    {
                        file.addChunkData(3u, sizeof(GrassInstance), perModel->grass[k]);
                    }
                }
            }
        }
        file.Close();
        return true;
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
        if (mapX < 0 || mapZ < 0 || mapX + 2 >= mapSizePlusOne || mapZ + 2 >= mapSizePlusOne)
        {
            // Return default up vector for out-of-bounds coordinates

            CVector result;
            result.x = 0.0f;
            result.y = 1.0f;
            result.z = 0.0f;
            return result;
        }

        // Clamp coordinates to map boundaries
        if (mapX == mapSizePlusOne)
        {
            mapX = mapSize;
        }
        if (mapZ == mapSizePlusOne)
        {
            mapZ = mapSize;
        }

        // Arrays to store normal components for the 2x2 quad
        float normalX[8];
        float normalY[8];
        float normalZ[8];

        // Initialize normal arrays
        for (int i = 0; i < 8; i++)
        {
            normalZ[i] = 64.0f;  // Constant Z component
            normalY[i] = 64.0f;  // Constant Y component
        }

        // Calculate normals for the 2x2 quad around the point
        int index = 0;
        for (int z = mapZ; z <= mapZ + 1; z++)
        {
            for (int x = mapX; x <= mapX + 1; x++)
            {
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
        // RVA 0x5ABA80
        return 0xFF;
    }

    void Landscape::SetPresenceOnCollisionMap(int, int)
    {
        // RVA 0x644D30
        M3D_ASSERT(!"obsolete");
    }

    bool Landscape::IsThisVisCellHasWater(int x, int z) const
    {
        // RVA 0x5AAD70
        // NOTE: the bounds test uses || where && was meant, so it never rejects a cell.
        int const landSize = m_owner->m_level->land_size;
        return (x >= 0 || z >= 0 || x < landSize || z < landSize) && m_cellParams[x + z * landSize].m_iswatercell;
    }

    void Landscape::drawSpriteOverlayed(unsigned clr, CVector const& o, CVector const& scale, float rotZ)
    {
        // RVA 0x8E0330
        M3D_RENDERER->TgEnableSetLinearSt(0, scale.x, scale.y, o.x, o.z, rotZ, false, 0.0f, 0.0f, 1.0f, 1.0f);
        drawSpriteOverlayed2(o.x, o.z, 0.5f / scale.x, 0.5f / scale.y, clr, false);
        M3D_RENDERER->TgDisable(0);
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
        // RVA 0x5BCEF0
        return new Landscape(*this);
    }

    bool Landscape::LoadShoreLine(CStr const& FileName)
    {
        // RVA 0x5BB670
        fs::auxTaggedFile file;
        if (file.Open(FileName.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_INFO(CStr("Couldn't load shore line from file ") + FileName);
            return false;
        }

        char* formatTitle = nullptr;
        file.getFormatTitle(&formatTitle);
        if (strcmp(formatTitle, "SFF"))
        {
            M3D_LOG_ERR(CStr("Error Wrong shore line file format: '") + CStr(formatTitle) + CStr("'"));
            return false;
        }

        unsigned formatVersion = 0;
        file.getFormatVersion(formatVersion);
        if (formatVersion != 1)
        {
            M3D_LOG_ERR(CStr("Error: Wrong shore line file format version: ") + CStr(formatVersion));
            return false;
        }

        if (!file.isChunkPresent(1u))
        {
            SYS_ERROR("File.isChunkPresent( SHORES_CHUNK )");
        }
        unsigned char* data = nullptr;
        file.getChunkData(1u, reinterpret_cast<void**>(&data));
        unsigned const numShores = *reinterpret_cast<unsigned*>(data);
        data += 4;
        m_shoreLines.resize(numShores);
        for (unsigned i = 0; i < numShores; ++i)
        {
            unsigned const numPoints = *reinterpret_cast<unsigned*>(data);
            data += 4;
            m_shoreLines[i].resize(numPoints);
            memcpy(m_shoreLines[i].data(), data, sizeof(CVector) * numPoints);
            data += sizeof(CVector) * numPoints;
        }
        ReBuildShoresVb();

        // Only the editor needs to know which shores were switched off.
        if (m_renderMode == RM_EDITOR && file.isChunkPresent(2u))
        {
            unsigned* keys = nullptr;
            file.getChunkData(2u, reinterpret_cast<void**>(&keys));
            unsigned const num = *keys++;
            for (unsigned i = 0; i < num; ++i)
            {
                m_noShoresSet.insert(keys[i]);
            }
        }
        file.Close();
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
        // RVA 0x5AD220
        return static_cast<int>(m_AlphaSets.size());
    }

    void Landscape::CreateLod()
    {
        auto const land_size = m_owner->m_level->land_size;

        delete[] m_cellParams;
        m_cellParams = new CellParams[land_size * land_size];
        CreateHeights(m_cellParams, land_size, 16);

        auto const drawedCellSize = 4 * m_owner->m_level->land_size;
        delete[] m_drawedCellParams;
        m_drawedCellParams = new CellParams[drawedCellSize * drawedCellSize];
        CreateHeights(m_drawedCellParams, drawedCellSize, 4);
    }

    void Landscape::drawSpriteOverlayedProjected(
        unsigned clr,
        CVector const& o,
        CMatrix const& projectorMatrix,
        CClipper const& clipper)
    {
        // RVA 0x8E03D0
        M3D_RENDERER->TgEnableSetMatrixStr(0, &projectorMatrix, true);
        M3D_RENDERER->TgSetTransformMode(0, rend::TG_PROJ_3);
        drawSpriteOverlayed2Projected(o.x, o.z, 128.0f, 128.0f, clr, false, clipper);
        M3D_RENDERER->TgDisable(0);
    }

    void Landscape::CreateHeights(CellParams* dest, int ls, int cellSize)
    {
        std::unordered_map<int, int> counterForHeights;
        std::set<int> usedHeights;
        float maxCounts = 999999.0;
        for (int z = 0; z < ls; ++z)
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

    int Landscape::isWaterCell(int x, int y) const
    {
        // RVA 0x5AAC90
        return m_waterMap[4 * y * m_owner->m_level->land_size + x] != 0;
    }

    void Landscape::Restore()
    {
        // RVA 0x5AB470
        m_dirtyReflection = true;
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

    char const* Landscape::GetGrassModelName(unsigned modelIdx) const
    {
        // RVA 0x6AB670
        return m_grassModels[modelIdx].modelName.c_str();
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
            auto v11 = org.x +
                (float)((float)((float)((float)this->m_drawRadius - 0.76999998) * VISCELL_EDGE_LENGTH_24) * f.x);
            auto v12 = org.z +
                (float)(f.z * (float)((float)((float)this->m_drawRadius - 0.76999998) * VISCELL_EDGE_LENGTH_24));
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

        auto const land_scale_27 = 8.0;
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
            M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN, vofs, 2u);
            v33 = 4;
        }

        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN, v33 + vofs, 2u);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN, v33 + vofs + 4, 2u);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN, v33 + vofs + 8, 2u);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN, v33 + vofs + 12, 2u);
        M3D_RENDERER->DrawPrimitive(rend::M3DPT_TRIANGLEFAN, v33 + vofs + 16, 2u);
        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->SetColorWriteMask(15u, false);
    }

    void Landscape::GetWaterCellHeights(float& h0, float& h1, int x, int z) const
    {
        // RVA 0x5AAD20
        CellParams const& cell = m_cellParams[x + z * m_owner->m_level->land_size];
        h0 = cell.m_minwater;
        h1 = cell.m_maxwater;
    }

    void Landscape::ManageLandScapeCollisionTriMeshes()
    {
        using namespace ai;

        auto const landSize = m_owner->m_level->land_size;
        retruxx::set<ai::PhysicObj*> allPhysicObjs;

        // TODO: check this
        for (int y = 0; y < landSize; ++y)
        {
            for (int x = 0; x < landSize; ++x)
            {
                auto* collisionItem = m_oCollisionitems[x + y * landSize];
                auto const mustCheck = collisionItem->m_bMustCheck;
                collisionItem->m_bMustCheck = false;
                if (mustCheck)
                {
                    bool isCellEnabled = false;
                    for (auto const objId : collisionItem->m_physicObjIds)
                    {
                        auto* obj = theObjects->GetEntityByObjId(objId);
                        if (obj)
                        {
                            if (IS_KIND_OF(obj, PhysicObj))
                            {
                                auto* physObj = RT_DYNCAST(obj, PhysicObj);
                                allPhysicObjs.insert(physObj);
                                if (physObj->bIsUpdatingByODE() && (physObj->GetPhysicState() & 1) != 0)
                                {
                                    isCellEnabled = true;
                                    break;
                                }
                            }
                            else
                            {
                                M3D_LOG_INFO(
                                    "Error: not PhysicObj is linked to collision cell x = " + CStr(x) +
                                    ", y = " + CStr(y) + ", id = " + CStr(objId));
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

                            for (auto const objId : collisionItem->m_physicObjIds)
                            {
                                auto* physObj = RT_DYNCAST(theObjects->GetEntityByObjId(objId), PhysicObj);
                                auto const physicState = physObj->GetPhysicState();
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

                            for (auto const objId : collisionItem->m_physicObjIds)
                            {
                                auto* physObj = RT_DYNCAST(theObjects->GetEntityByObjId(objId), PhysicObj);
                                auto const physicState = physObj->GetPhysicState();
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

    void Landscape::LinkPassMapCellToCollisionCell(PointBase<int> const& cellPos)
    {
        // RVA 0x64C960
        // Puts a 50 unit tall box over a pass map cell and links it into every collision
        // cell it overlaps.
        float const VISCELL_EDGE_LENGTH = 128.0f;
        int const landSize = m_owner->m_level->land_size;
        float const passCellEdgeLen = static_cast<float>(landSize) * 128.0f / static_cast<float>(4 * landSize) * 0.5f;

        CVector passGeomPos;
        passGeomPos.x = (static_cast<float>(cellPos.x) + 0.5f) * passCellEdgeLen;
        passGeomPos.z = (static_cast<float>(cellPos.y) + 0.5f) * passCellEdgeLen;
        passGeomPos.y = GetLsHeight(passGeomPos.x, passGeomPos.z);

        float const half = passCellEdgeLen * 0.5f;
        float const inv = 1.0f / VISCELL_EDGE_LENGTH;
        int const x0 = std::clamp(static_cast<int>((passGeomPos.x - half) * inv), 0, landSize - 1);
        int const z0 = std::clamp(static_cast<int>((passGeomPos.z - half) * inv), 0, landSize - 1);
        int const x1 = std::clamp(static_cast<int>((half + passGeomPos.x) * inv), 0, landSize - 1);
        int const z1 = std::clamp(static_cast<int>((half + passGeomPos.z) * inv), 0, landSize - 1);

        auto* geomObject = static_cast<GeomObject*>(M3D_KERNEL->New("GeomObjectPassCell"));
        dxGeom* box = dCreateBox(m_owner->GetOdeSpace(), passCellEdgeLen, 50.0f, passCellEdgeLen);
        dGeomSetPosition(box, passGeomPos.x, passGeomPos.y, passGeomPos.z);
        geomObject->m_translation = ZeroVector;
        geomObject->m_rotation = IdentityQuaternion;
        geomObject->SetGeom(box);
        geomObject->m_needToDeleteInUnlink = false;
        // NOTE: the bounds are passed as (x0, x1) and (z0, z1) rather than as the corners
        // (x0, z0) and (x1, z1), so the cells recorded on the geom are wrong.
        geomObject->SetBounds(PointBase<int>(x0, x1), PointBase<int>(z0, z1));

        for (int z = z0; z <= z1; ++z)
        {
            for (int x = x0; x <= x1; ++x)
            {
                GetCollisionCellItem(x, z)->m_geomsList.insert(geomObject);
            }
        }
    }

    void Landscape::ReBuildShoresVb()
    {
        // RVA 0x5C1BD0
        // Every shore line becomes a triangle strip, two vertices per point: one on the
        // beach side, opaque and clamped under the terrain, and one out over the water,
        // fully transparent.
        unsigned numVerts = 0;
        for (auto const& line : m_shoreLines)
        {
            numVerts += 2 * static_cast<unsigned>(line.size());
        }

        if (m_shoresVb.IsValid())
        {
            M3D_RENDERER->ReleaseVb(m_shoresVb);
        }
        if (m_shoresIb.IsValid())
        {
            M3D_RENDERER->ReleaseIb(m_shoresIb);
        }
        if (!numVerts)
        {
            return;
        }

        m_shoresVb = M3D_RENDERER->AddVb(rend::VERTEX_XYZCT1, numVerts, CStr("Shores"), 0);
        m_shoresIb = M3D_RENDERER->AddIb(numVerts, false);
        auto* indices = static_cast<unsigned short*>(M3D_RENDERER->LockIb(m_shoresIb, 0, 0, 0));
        // NOTE: the counter is 16 bit, so more than 65535 shore vertices hang here.
        unsigned short index = 0;
        do
        {
            indices[index] = index;
            ++index;
        } while (index < numVerts);
        M3D_RENDERER->UnlockIb(m_shoresIb);

        auto* v = static_cast<rend::VertexXYZCT1*>(M3D_RENDERER->LockVb(m_shoresVb, 0, 0, 0));
        auto const cvarF = [](CVar const& cvar)
        {
            return cvar.GetType() == CVar::CVAR_FLOAT ? cvar.GetF() : static_cast<float>(cvar.GetI());
        };
        float const SHORE_LENGTH = cvarF(M3D_ENGINE_CFG.m_g_shoresWidth);
        float const SHORE_PENETRATION = cvarF(M3D_ENGINE_CFG.m_g_shoresDeep);
        float const SHORE_ELEVATION = cvarF(M3D_ENGINE_CFG.m_g_shoresElevation);
        unsigned const SHORE_ALPHA = M3D_ENGINE_CFG.m_g_shoresOpaque.GetI() << 24;
        unsigned const beachColor = SHORE_ALPHA | 0xFFFFFF;

        // Writes the vertex pair for point p, with the strip running along the given side.
        auto const emitPair =
            [&](rend::VertexXYZCT1* out, CVector const& p, CVector const& side, CVector const& otherSide, float tu)
        {
            CVector water = p + side * SHORE_LENGTH;
            CVector beach = p + otherSide * SHORE_PENETRATION;
            // If that side runs into the terrain, the strip is flipped over.
            if (GetLsHeight(water.x, water.z) > water.y)
            {
                water = p + otherSide * SHORE_LENGTH;
                beach = p + side * SHORE_PENETRATION;
            }
            float beachY = beach.y + SHORE_ELEVATION;
            float const groundY = GetLsHeight(beach.x, beach.z);
            if (beachY > groundY)
            {
                beachY = groundY;
            }
            out[0].x = beach.x;
            out[0].y = beachY;
            out[0].z = beach.z;
            out[0].c = beachColor;
            out[0].tu = tu;
            out[0].tv = 1.0f;
            out[1].x = water.x;
            out[1].y = water.y;
            out[1].z = water.z;
            out[1].c = 0xFFFFFF;
            out[1].tu = tu;
            out[1].tv = 0.0f;
        };

        for (auto const& line : m_shoreLines)
        {
            int const numSegments = static_cast<int>(line.size()) - 1;
            if (numSegments <= 0)
            {
                continue;
            }
            rend::VertexXYZCT1* const lineStart = v;
            for (int i = 0; i < numSegments; ++i)
            {
                CVector const& p1 = line[i];
                CVector const& p2 = line[i + 1];
                float const dx = p1.x - p2.x;
                float const dy = p1.y - p2.y;
                float const dz = p1.z - p2.z;
                float const inv = static_cast<float>(1.0 / sqrt(dx * dx + dz * dz + dy * dy + 1.1920929e-7));
                float const nx = inv * dx;
                float const nz = dz * inv;
                // The horizontal perpendicular to the segment, up x dir, and its opposite.
                CVector const side(nz, 0.0f, -nx);
                CVector const otherSide(-nz, 0.0f, nx);

                emitPair(v, p1, side, otherSide, (i & 1) ? 1.0f : 0.0f);
                v += 2;

                if (i == numSegments - 1)
                {
                    double const cx = line.front().x - line.back().x;
                    double const cy = line.front().y - line.back().y;
                    double const cz = line.front().z - line.back().z;
                    if (sqrt(cz * cz + cy * cy + cx * cx) >= 0.1)
                    {
                        // An open line gets a closing pair at its last point, along the last
                        // segment's sides.
                        emitPair(v, p2, side, otherSide, ((i + 1) & 1) ? 1.0f : 0.0f);
                    }
                    else
                    {
                        // A closed loop repeats its first pair.
                        float const tu = (line.size() & 1) == 0 ? 1.0f : 0.0f;
                        v[0] = lineStart[0];
                        v[0].tu = tu;
                        v[0].tv = 1.0f;
                        v[1] = lineStart[1];
                        v[1].tu = tu;
                        v[1].tv = 0.0f;
                    }
                    v += 2;
                }
            }
        }
        M3D_RENDERER->UnlockVb(m_shoresVb);
    }

    int Landscape::LoadTiles(CStr const& filename)
    {
        // TODO: generated code (looks ok)
        // Free existing tiles if any
        FreeTiles();

        // Calculate land size and allocate memory for tiles
        int const landSize = 4 * m_owner->m_level->land_size;
        int const totalTiles = landSize * landSize;

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
            M3D_LOG_ERR(
                "Error: Bad tilemap file, landsize = " + CStr(landSize) +
                ", tilemap size in file = " + CStr(fileLandSize));
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
        ReadTileInfo(1);  // Initialize tile info

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
                if (tileAngle > 4)
                    tileAngle = 4;
                tile.m_angle = tileAngle;

                // Create full texture path and add texture
                CStr texturePath = m_pathTile + tileNames[tileIndex];
                tile.m_texIndex0 = AddOneTexture(texturePath);
            }
        }

        // Clean up
        file.Close();

        // Update texture information
        size_t const textureCount = m_tilesTextures.size();
        ChangedNumberOfUsedTextures(textureCount);
        RecalcUV();
        UpdateTexturesFilters();

        return 1;
    }

    bool Landscape::traceLineThruCellLs(
        float& ttt,
        int cellX,
        int cellZ,
        CVector const& start,
        CVector const& dir,
        bool allowColInfo)
    {
        // RVA 0x5B0FA0
        // The nearest of the terrain hit in this cell and, if allowed, any collision mesh
        // hit anywhere.
        float t0 = 0.0f;
        bool const hitTerrain = traceLineThruCellLs0(t0, cellX, cellZ, start, dir);
        float best = 10000.0f;
        bool hitCollision = false;
        if (allowColInfo)
        {
            for (auto* ci : m_collisions)
            {
                float const t = ci->TraceRay(start, dir);
                if (t != -1.0f && best > t)
                {
                    best = t;
                    hitCollision = true;
                }
            }
        }
        if (hitCollision)
        {
            ttt = (hitTerrain && t0 <= best) ? t0 : best;
            return true;
        }
        if (hitTerrain)
        {
            ttt = t0;
            return true;
        }
        return false;
    }

    void Landscape::RecursiveDisableShore(unsigned char* marks, int x, int z)
    {
        // RVA 0x5C2F00
        // Flood fills the connected water cells from (x, z); the step to z + 1 is a loop,
        // the other three neighbours recurse.
        for (;; ++z)
        {
            M3D_ASSERT(m_waterMap);
            int const side = 4 * m_owner->m_level->land_size;
            if (x < 0 || x >= side || z < 0 || z >= side)
            {
                return;
            }
            unsigned char& mark = marks[z * side + x];
            if (mark || !m_waterMap[x + z * side])
            {
                return;
            }
            unsigned const key = x + (z << 16);
            m_noShoresSet.insert(key);
            mark = 1;
            RecursiveDisableShore(marks, x - 1, z);
            RecursiveDisableShore(marks, x + 1, z);
            RecursiveDisableShore(marks, x, z - 1);
        }
    }

    Landscape::Landscape(Landscape const&) : SgNode()
    {
        // RVA 0x5B8340 - NOTE: nothing is copied from the source; the members are only
        // default constructed, so Clone yields an empty landscape.
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
            m_waterQueries[i] = M3D_RENDERER->NewQuery(rend::IQuery::Type::QUERY_OCCLUSION);  // OCCLUSION query type
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
        m_waterVb =
            M3D_RENDERER->AddVb(m3d::rend::VertexType::VERTEX_WATERTEST, 81 * m_maxWaterCellPerPass, "Water", 0);

        // Initialize water vertices
        // looks ok
        VertexWaterTest* waterVertices = static_cast<VertexWaterTest*>(M3D_RENDERER->LockVb(m_waterVb, 0, 0, 0));
        uint8_t constexpr gridSize = 9;  // Since loops go up to 8 (0-8 inclusive)
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
                int i = 9 * v82;
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
            int v102 = 1 << i;
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
        m_profilerUpdateVis =
            M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls update vis", 30));
        m_profilerDraw =
            M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls render", 30));
        m_profilerDrawGrass =
            M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls grass", 30));
        m_profilerDrawWater =
            M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls water", 30));

        // Setup debug counter
        m_countPhysicObjsInCells =
            M3D_APP->GetDbgCounterStack().GetCounter(M3D_APP->GetDbgCounterStack().AddCounter("PhysicObjs in cells"));

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
        m_solidVs = M3D_RENDERER->NewHlslShader(
            "data/shaders/landscapeSolid_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_solidVs);

        // Load solid PS
        m_solidPs = M3D_RENDERER->NewHlslShader(
            "data/shaders/landscapeSolid_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_solidPs);

        // Load solid bind VS
        m_solidBindVs = M3D_RENDERER->NewHlslShader(
            "data/shaders/landscapeSolidBind_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_solidBindVs);

        // Load solid bind PS
        m_solidBindPs = M3D_RENDERER->NewHlslShader(
            "data/shaders/landscapeSolidBind_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_solidBindPs);

        // Load landscape VS
        m_landscapeVs = M3D_RENDERER->NewHlslShader(
            "data/shaders/landscape_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_landscapeVs);

        // Load landscape FP PS
        m_landscapePsFP = M3D_RENDERER->NewHlslShader(
            "data/shaders/landscapeFP_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_landscapePsFP);

        // Load landscape SP PS
        m_landscapePsSP = M3D_RENDERER->NewHlslShader(
            "data/shaders/landscapeSP_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
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
        int const numCells = cellsPerTexture.size();
        unsigned int const* currentCell = &cellsPerTexture[0];

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

        int const CELLS_PER_TILE = 4;
        int const VERTICES_PER_CELL = 25;  // 5x5 grid
        int const MAX_VERTICES = 65535;    // 16-bit limit

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
                unsigned int const cellData = *currentCell;

                // Extract cell information from packed data
                unsigned char const tileX = cellData & 0xFF;
                unsigned char const tileY = (cellData >> 8) & 0xFF;
                int const angle = (cellData >> 16) & 0x3;
                int const cornerType = (cellData >> 24) & 0xF;

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
                    int const squareX = tileX / CELLS_PER_TILE;
                    int const squareY = tileY / CELLS_PER_TILE;
                    int const localX = tileX % CELLS_PER_TILE;
                    int const localY = tileY % CELLS_PER_TILE;

                    // Check if we're starting a new tile
                    if (squareX != lastSquareX || squareY != lastSquareY)
                    {
                        // Store previous tile data if we have a valid previous tile
                        if (lastSquareX != -1 && lastSquareY != -1)
                        {
                            int const storageShift = (renderType != 0) ? 4 : 0;

                            char const offsetShift = (4 * storageShift);
                            char const bankShift = (2 * storageShift);

                            assert(lastSquareX >= 0);
                            assert(lastSquareX < 64);
                            assert(lastSquareY >= 0);
                            assert(lastSquareY < 64);

                            // Store cell count for this tile
                            chunk.m_numCellsPerCellMap[lastSquareX + (lastSquareY * 64)] |=
                                (cellsInCurrentPass << (2 * storageShift));

                            int const tileIndex = lastSquareX + (lastSquareY << 8);

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
                        alphaUVs = m_setAndUVs.m_sets[m_CurAlphaSet][specialMapper[cornerType].m_maskindex]
                                       .m_uvForAngles[specialMapper[cornerType].m_rotate][0];
                    }

                    int const mapStride = m_mapSize + 1;
                    int const heightMapBaseOffset = 4 * tileX + 4 * tileY * mapStride;

                    // Generate 5x5 grid of vertices (25 vertices total)
                    for (int row = 0; row < 5; ++row)
                    {
                        int verticesGenerated = 0;

                        if (renderType == 1)
                        {
                            // Alpha pass rendering
                            float* uvPtr = alphaUVs + 1;
                            auto vertexOffsetCalc =
                                reinterpret_cast<char*>(vertices) - reinterpret_cast<char*>(alphaUVs);

                            for (int col = 0; col < 5; ++col)
                            {
                                // Calculate vertex index within the 32x32 vertex grid per tile
                                short const vertexIndex = localY + 32 * (col + 4 * localX);

                                // Store vertex index
                                vertices[verticesGenerated].xz = row + 4 * vertexIndex;

                                // Set vertex height from heightmap
                                vertices[verticesGenerated].y =
                                    m_heightMap[heightMapBaseOffset + col + row * mapStride];

                                // Pack UV coordinates
                                float const u = *uvPtr;
                                float const v = *(uvPtr - 1);
                                vertices[verticesGenerated].uv =
                                    static_cast<int>(u * 64.0f) - (static_cast<int>(v * -64.0f) << 9);

                                uvPtr += 2;
                                ++verticesGenerated;
                            }
                        }
                        else
                        {
                            // Base pass rendering
                            float* uvPtr = baseUVs + 1;
                            auto vertexOffsetCalc =
                                reinterpret_cast<char*>(vertices) - reinterpret_cast<char*>(baseUVs);

                            for (int col = 0; col < 5; ++col)
                            {
                                // Calculate vertex index within the 32x32 vertex grid per tile
                                short const vertexIndex = localY + 32 * (col + 4 * localX);

                                // Store vertex index
                                vertices[verticesGenerated].xz = row + 4 * vertexIndex;

                                // Set vertex height from heightmap
                                vertices[verticesGenerated].y =
                                    m_heightMap[heightMapBaseOffset + col + row * mapStride];

                                // Pack UV coordinates
                                float const u = *uvPtr;
                                float const v = *(uvPtr - 1);
                                vertices[verticesGenerated].uv =
                                    static_cast<int>(u * 64.0f) - (static_cast<int>(v * -64.0f) << 9);

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
            int const storageShift = (renderType != 0) ? 4 : 0;
            char const offsetShift = (4 * storageShift);
            char const bankShift = (2 * storageShift);

            assert(lastSquareX >= 0);
            assert(lastSquareX < 64);
            assert(lastSquareY >= 0);
            assert(lastSquareY < 64);

            //const int tileIndex = lastSquareY * 64 + lastSquareX;

            // Store cell count for this tile
            chunk.m_numCellsPerCellMap[lastSquareY * 64 + lastSquareX] |= (cellsInCurrentPass << (2 * storageShift));

            int const tileIndex = lastSquareX + (lastSquareY << 8);

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
        char cellCountShift = 2 * typeShift;  // Shift for cell count extraction
        char offsetShift = 4 * typeShift;     // Shift for offset extraction

        // Process each cell in the texture
        for (unsigned int cellIndex = 0; cellIndex < cellsPerTex.size(); ++cellIndex)
        {
            unsigned int cellData = cellsPerTex[cellIndex];

            // Extract position from cell data
            unsigned char posX = static_cast<unsigned char>(cellData);
            unsigned char posY = static_cast<unsigned char>(cellData >> 8);

            float const land_scale_27 = 8.0;

            // Set position constant for vertex shader
            CVector position;
            position.x = static_cast<float>(posX) * 128.0f;
            position.y = static_cast<float>(posY) * 128.0f;
            position.z = land_scale_27;

            m3d::Application::g_pApp->m_renderer->SetVsFloatConst(10u, reinterpret_cast<float const*>(&position), 1u);

            // Extract rendering information from chunk data
            unsigned short cellMapIndex = 64 * posY + posX;
            unsigned char cellCount =
                static_cast<unsigned char>(tivchunk.m_numCellsPerCellMap[cellMapIndex] >> cellCountShift);

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
                M3D_RENDERER->DrawIndexedPrimitiveShader(rend::M3DPT_TRIANGLESTRIP, 0, vertexCount, 0, indexCount);
            }
        }
    }

    void Landscape::RecursiveEnableShore(unsigned char* marks, int x, int z)
    {
        // RVA 0x5C3270
        // Flood fills the connected water cells from (x, z); the step to z + 1 is a loop,
        // the other three neighbours recurse.
        for (;; ++z)
        {
            M3D_ASSERT(m_waterMap);
            int const side = 4 * m_owner->m_level->land_size;
            if (x < 0 || x >= side || z < 0 || z >= side)
            {
                return;
            }
            unsigned char& mark = marks[z * side + x];
            if (mark || !m_waterMap[x + z * side])
            {
                return;
            }
            unsigned const key = x + (z << 16);
            m_noShoresSet.erase(key);
            mark = 1;
            RecursiveEnableShore(marks, x - 1, z);
            RecursiveEnableShore(marks, x + 1, z);
            RecursiveEnableShore(marks, x, z - 1);
        }
    }

    bool Landscape::traceLineThruCellLs0(float& ttt, int cellX, int cellZ, CVector const& start, CVector const& dir)
    {
        // RVA 0x5AE7C0
        // Tests the ray against the height map quads of visibility cell (cellX, cellZ), in
        // scan order.
        // NOTE: the loops run 17 times a side, so the row and column just past the cell are
        // tested as well, and the first hit in scan order wins, not the nearest.
        ttt = -1.0f;
        int const n = m_mapSize;
        auto const heightAt = [&](int x, int z)
        {
            return (x < 0 || x > n || z < 0 || z > n) ? 0.0f : m_heightMap[x + z * (n + 1)];
        };
        for (int zi = 0; zi <= 16; ++zi)
        {
            int const z = 16 * cellZ + zi;
            float const zNear = static_cast<float>(z) * 8.0f;
            float const zFar = static_cast<float>(z + 1) * 8.0f;
            for (int xi = 0; xi <= 16; ++xi)
            {
                int const x = 16 * cellX + xi;
                CVector v[4];
                v[0] = CVector(static_cast<float>(x) * 8.0f, heightAt(x, z), zNear);
                v[1] = CVector(static_cast<float>(x + 1) * 8.0f, heightAt(x + 1, z), zNear);
                v[2] = CVector(static_cast<float>(x + 1) * 8.0f, heightAt(x + 1, z + 1), zFar);
                v[3] = CVector(static_cast<float>(x) * 8.0f, heightAt(x, z + 1), zFar);
                float tt;
                float tu;
                float tv;
                if (intersectTriangle(start, dir, v[3], v[1], v[0], tt, tu, tv) ||
                    intersectTriangle(start, dir, v[3], v[2], v[1], tt, tu, tv))
                {
                    ttt = tt;
                    return true;
                }
            }
        }
        return false;
    }

    void Landscape::DrawCells0(cmn::vector<unsigned> const& cellsPerTex, RenderTypes RenderType)
    {
        // RVA 0x7A7160
        // The editor's terrain path: every tile's 5x5 vertices are built on the CPU into
        // m_dummyVB and streamed out, up to m_clampCells tiles per draw.
        float const LAND_SCALE = 8.0f;
        rend::VbHandle vb = m_landVb;
        int const colorAdd = m_mapSize + 1;
        if (m_lastState != RenderType)
        {
            if (RenderType == RT_FIRSTPASSLIGHT || RenderType == RT_OTHERPASSES)
            {
                rend::TextureState colorOp;
                if (RenderType == RT_OTHERPASSES)
                {
                    M3D_RENDERER->SetBlend(rend::BM_ALPHA, false);
                    M3D_RENDERER->SetAlphaTest(1);
                    colorOp = rend::TS_TEXTURE;
                }
                else
                {
                    M3D_RENDERER->SetBlend(rend::BM_NONE, false);
                    M3D_RENDERER->SetAlphaTest(0);
                    colorOp = rend::TS_DIFFUSE;
                }
                M3D_RENDERER->SetStageState(0, rend::BM_COLOR, colorOp);
                M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
                M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_TEXTURE);
                M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_PREV);
            }
            m_lastState = RenderType;
        }

        unsigned const* curCell = cellsPerTex.m_data;
        int numCells = cellsPerTex.m_numItems;
        int const ls = 4 * m_owner->m_level->land_size;
        // NOTE: the UV tables are read as [angle][25][2] (200 bytes an angle), not as the
        // [2][25][4] the header declares.
        float const* const uvBase = &m_uvForAngles[0][0][0];
        float const* const alphaUvBase = &m_setAndUVs.m_sets[0][0].m_uvForAngles[0][0][0];
        // NOTE: a m_clampCells of 0 or less never finishes.
        while (numCells)
        {
            int numCellsToDraw = numCells;
            int const clampCells = M3D_ENGINE_CFG.m_clampCells.GetI();
            if (numCells < 0)
            {
                numCellsToDraw = 0;
            }
            if (numCellsToDraw > clampCells)
            {
                numCellsToDraw = clampCells;
            }

            rend::VertexXYZNCT2* vert = m_dummyVB;
            int vofs = 0;
            int realCellsToDraw = numCellsToDraw;
            for (int i = 0; i < numCellsToDraw; ++i, ++curCell)
            {
                unsigned const cell = *curCell;
                int const tx = cell & 0xFF;
                int const tz = (cell >> 8) & 0xFF;
                int const corner = (static_cast<int>(cell) >> 24) & 0xF;
                bool const alphaCorner = corner && specialMapper[corner].m_maskindex;
                // The first pass takes the plain tiles, the others only the alpha corners.
                if (RenderType == RT_FIRSTPASSLIGHT)
                {
                    if (alphaCorner)
                    {
                        --realCellsToDraw;
                        continue;
                    }
                    ++m_firstpasscounter;
                }
                else
                {
                    m_passedCells[ls * tz + tx] = 1;
                    if (!alphaCorner)
                    {
                        --realCellsToDraw;
                        continue;
                    }
                    if (RenderType == RT_OTHERPASSES)
                    {
                        ++m_otherpasscounter;
                    }
                    else
                    {
                        ++m_firstpasscounter;
                    }
                }

                float const* srcUv = uvBase;
                float const* srcUv2 = nullptr;
                if (RenderType == RT_OTHERPASSES)
                {
                    srcUv2 = alphaUvBase + 200 * (m_CurAlphaSet * 5 + specialMapper[corner].m_maskindex) +
                        50 * specialMapper[corner].m_rotate;
                }

                int const idx0 = 4 * tx + 4 * tz * (m_mapSize + 1);
                float const* srcHeight = &m_heightMap[idx0];
                unsigned const* srcColor = &m_colormap[idx0];
                CVector const* srcNormal = &m_vnormal[idx0];
                float xs[5];
                xs[0] = static_cast<float>(4 * tx) * LAND_SCALE;
                for (int k = 1; k < 5; ++k)
                {
                    xs[k] = xs[k - 1] + LAND_SCALE;
                }
                float z = static_cast<float>(4 * tz) * LAND_SCALE;

                for (int row = 0; row < 5; ++row)
                {
                    // NOTE: the light pass writes no vertices, yet still draws whatever
                    // m_dummyVB holds.
                    if (RenderType != RT_LIGHTPASS)
                    {
                        for (int col = 0; col < 5; ++col)
                        {
                            rend::VertexXYZNCT2& v = vert[col];
                            v.x = xs[col];
                            v.y = srcHeight[col];
                            v.z = z;
                            // The normal goes out with y and z swapped.
                            v.nx = srcNormal[col].x;
                            v.ny = srcNormal[col].z;
                            v.nz = srcNormal[col].y;
                            v.c = srcColor[col];
                            if (RenderType == RT_OTHERPASSES)
                            {
                                // Stage 0 samples the alpha mask, stage 1 the tile.
                                v.tu0 = srcUv2[2 * col];
                                v.tv0 = srcUv2[2 * col + 1];
                                v.tu1 = srcUv[2 * col];
                                v.tv1 = srcUv[2 * col + 1];
                            }
                            else
                            {
                                v.tu0 = srcUv[2 * col];
                                v.tv0 = srcUv[2 * col + 1];
                            }
                        }
                        vert += 5;
                    }
                    srcHeight += colorAdd;
                    srcNormal += colorAdd;
                    srcColor += colorAdd;
                    srcUv += 10;
                    if (srcUv2)
                    {
                        srcUv2 += 10;
                    }
                    z += LAND_SCALE;
                }
            }

            int const numIndices = realCellsToDraw * m_lsNumIndices[0] - 3;
            if (numIndices > 0)
            {
                void* dst = M3D_RENDERER->LockVbStreaming(vb, 25 * realCellsToDraw, vofs, nullptr);
                memcpy(dst, m_dummyVB, sizeof(rend::VertexXYZNCT2) * 25 * realCellsToDraw);
                M3D_RENDERER->UnlockVb(vb);
                M3D_RENDERER->SetToStream0(vb);
                M3D_RENDERER->SetIndices(m_landIbConst[0], vofs);
                M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLESTRIP, 0, 25 * realCellsToDraw, 0, numIndices);
            }
            numCells -= numCellsToDraw;
        }
    }

    int Landscape::IsBackfaced(int xx, int yy, rend::Cull cull)
    {
        // RVA 0x7A5560
        // Projects the 5x5 height samples of the 32 unit tile (xx, yy) and reports whether
        // it faces away from the camera under the given cull mode.
        CMatrix const mat = M3D_RENDERER->MatGet() * M3D_RENDERER->MatGetProj();
        int const stride = m_mapSize + 1;
        CVector cellVertsScr[25];
        CVector* out = cellVertsScr;
        for (int row = 0; row < 5; ++row)
        {
            float const* heights = &m_heightMap[(4 * yy + row) * stride + 4 * xx];
            float const z = static_cast<float>(4 * yy + row) * 8.0f;
            float x = static_cast<float>(4 * xx) * 8.0f;
            for (int col = 0; col < 5; ++col, x += 8.0f)
            {
                float const h = heights[col];
                // NOTE: the sample goes through the matrix as (x, z, height) rather than
                // (x, height, z).
                float const X = ((mat._31 * h + mat._11 * x) + mat._21 * z) + mat._41;
                float const Y = ((mat._32 * h + mat._12 * x) + mat._22 * z) + mat._42;
                float const Z = ((mat._33 * h + mat._13 * x) + mat._23 * z) + mat._43;
                float const W = ((mat._34 * h + mat._14 * x) + mat._24 * z) + mat._44;
                float const inv = 1.0f / W;
                *out++ = CVector(X * inv, Y * inv, Z * inv);
            }
        }

        // NOTE: only one triangle of each quad is tested.
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                CVector const& a = cellVertsScr[5 * row + col];
                CVector const& b = cellVertsScr[5 * row + col + 1];
                CVector const& c = cellVertsScr[5 * (row + 1) + col + 1];
                float const area = (((a.y - b.y) * c.x + (b.y - c.y) * a.x) + (c.y - a.y) * b.x) * 0.5f;
                if (cull == rend::M3DCULL_CW)
                {
                    if (!(area <= 0.0f))
                    {
                        return 0;
                    }
                }
                else if (cull == rend::M3DCULL_CCW)
                {
                    if (!(area >= 0.0f))
                    {
                        return 0;
                    }
                }
            }
        }
        return 1;
    }

    void Landscape::FreeTiles()
    {
        // TODO: generated code (looks ok)
        // Free tile data array
        delete[] m_tiles;
        m_tiles = nullptr;

        // Release textures and vertex buffers
        size_t const textureCount = m_tilesTextures.size();
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
        // RVA 0x7A4460
        return m_curVisMode;
    }

    void Landscape::SetCurVisMode(VisibilityMode visMode)
    {
        // RVA 0x7A4450
        m_curVisMode = visMode;
    }

    void Landscape::DrawNonTransformGeom(dxGeom* geom)
    {
        // TODO: generated code Landscape::DrawNonTransformGeom
        unsigned int color = reinterpret_cast<unsigned int>(geom) | 0xFF000000;
        int geomClass = dGeomGetClass(geom);

        switch (geomClass)
        {
        case 0:  // Sphere geometry
        {
            float radius = dGeomSphereGetRadius(geom);

            // Initialize vertex buffer
            CVector vertices[4];

            // Subdivision parameters
            int h = 0;  // Horizontal subdivisions
            int l = 0;  // Vertical subdivisions

            // Create sphere through subdivision
            for (h = 0; h < 8; ++h)
            {
                for (l = 0; l < 8; ++l)
                {
                    // Generate 4 vertices for this patch
                    for (int v = 0; v <= 1; ++v)
                    {
                        for (int u = 0; u <= 1; ++u)
                        {
                            // Calculate spherical coordinates
                            double theta = ((double)(v + l) * 0.39269909 - 1.5707964) * 0.5;  // pi/8 steps
                            double phi = (double)(u + h) * 0.78539819 * 0.5;                  //pi/4 steps

                            // Calculate quaternion rotation
                            float q1 = sin(theta);
                            float q3 = cos(theta);
                            float q4 = sin(phi);
                            float q5 = cos(phi);

                            // Rotate base vector
                            CVector rotatedVec;
                            rotatedVec.x = (q1 * q4) + (q3 * 0.0f) + (q5 * 0.0f);
                            rotatedVec.y = (q3 * q4) + (q5 * 0.0f) - (q1 * 0.0f);
                            rotatedVec.z = (q1 * q5) + (q3 * 0.0f) - (q4 * 0.0f);

                            // Create rotation matrix from quaternion
                            CMatrix rotationMatrix;
                            float w = (q3 * q5) - (q4 * 0.0f) - (q1 * 0.0f);

                            // Calculate rotation matrix components
                            float xx = rotatedVec.x * rotatedVec.x;
                            float yy = rotatedVec.y * rotatedVec.y;
                            float zz = rotatedVec.z * rotatedVec.z;
                            float xy = rotatedVec.x * rotatedVec.y;
                            float xz = rotatedVec.x * rotatedVec.z;
                            float yz = rotatedVec.y * rotatedVec.z;
                            float wx = w * rotatedVec.x;
                            float wy = w * rotatedVec.y;
                            float wz = w * rotatedVec.z;

                            rotationMatrix._11 = 1.0f - 2.0f * (yy + zz);
                            rotationMatrix._12 = 2.0f * (xy + wz);
                            rotationMatrix._13 = 2.0f * (xz - wy);

                            rotationMatrix._21 = 2.0f * (xy - wz);
                            rotationMatrix._22 = 1.0f - 2.0f * (xx + zz);
                            rotationMatrix._23 = 2.0f * (yz + wx);

                            rotationMatrix._31 = 2.0f * (xz + wy);
                            rotationMatrix._32 = 2.0f * (yz - wx);
                            rotationMatrix._33 = 1.0f - 2.0f * (xx + yy);

                            // Apply rotation and scale by radius
                            int vertexIndex = v * 2 + u;
                            vertices[vertexIndex].x = (rotationMatrix._11 * 0.0f) + (rotationMatrix._12 * 0.0f) +
                                (rotationMatrix._13 * radius);
                            vertices[vertexIndex].y = (rotationMatrix._21 * 0.0f) + (rotationMatrix._22 * 0.0f) +
                                (rotationMatrix._23 * radius);
                            vertices[vertexIndex].z = (rotationMatrix._31 * 0.0f) + (rotationMatrix._32 * 0.0f) +
                                (rotationMatrix._33 * radius);
                        }
                    }

                    // Create first triangle
                    CVector triangle1[3];
                    triangle1[0] = vertices[0];
                    triangle1[1] = vertices[1];
                    triangle1[2] = vertices[2];

                    // Draw first triangle
                    M3D_APP->DrawTri(triangle1, color);

                    // Create second triangle
                    CVector triangle2[3];
                    triangle2[0] = vertices[2];
                    triangle2[1] = vertices[1];
                    triangle2[2] = vertices[3];

                    // Draw second triangle
                    M3D_APP->DrawTri(triangle2, color);
                }
            }
            break;
        }

        case 1:  // Box
        {
            float lengths[3];
            dGeomBoxGetLengths(geom, lengths);

            Aabb aabb;
            aabb.m_box[0] = -lengths[0] * 0.5f;
            aabb.m_box[1] = -lengths[1] * 0.5f;
            aabb.m_box[2] = -lengths[2] * 0.5f;
            aabb.m_box[3] = lengths[0] * 0.5f;
            aabb.m_box[4] = lengths[1] * 0.5f;
            aabb.m_box[5] = lengths[2] * 0.5f;

            aabb.Draw(color);
            break;
        }

        case 2:  // Capsule/Cylinder
        {
            float radius, length;
            dGeomCCylinderGetParams(geom, &radius, &length);

            // Draw top and bottom caps
            for (int segment = 0; segment < 8; ++segment)
            {
                CVector vertices[3];

                // Top triangle
                vertices[0] = CVector(0.0f, 0.0f, length * 0.5f);

                for (int i = 0; i <= 1; ++i)
                {
                    double angle = (segment + i) * 0.78539819 * 0.5;
                    float sinAngle = sin(angle);
                    float cosAngle = cos(angle);

                    // Build rotation matrix
                    float m11 = 1.0f - 2.0f * sinAngle * sinAngle;
                    float m12 = 2.0f * cosAngle * sinAngle;
                    float m13 = 0.0f;

                    float m21 = -2.0f * cosAngle * sinAngle;
                    float m22 = m11;
                    float m23 = 0.0f;

                    float m31 = 0.0f;
                    float m32 = 0.0f;
                    float m33 = 1.0f;

                    // Calculate vertex position
                    vertices[i + 1].x = m11 * radius;
                    vertices[i + 1].y = m12 * radius;
                    vertices[i + 1].z = length * 0.5f;
                }

                M3D_APP->DrawTri(vertices, color);

                // Bottom triangle (same vertices but with negative Z)
                vertices[0].z = -length * 0.5f;
                vertices[1].z = -length * 0.5f;
                vertices[2].z = -length * 0.5f;

                M3D_APP->DrawTri(vertices, color);
            }

            // Draw side quads
            for (int segment = 0; segment < 8; ++segment)
            {
                CVector vertices[4];

                for (int i = 0; i <= 1; ++i)
                {
                    double angle = (segment + i) * 0.78539819 * 0.5;
                    float sinAngle = sin(angle);
                    float cosAngle = cos(angle);

                    // Build rotation matrix
                    float m11 = 1.0f - 2.0f * sinAngle * sinAngle;
                    float m12 = 2.0f * cosAngle * sinAngle;

                    float m21 = -2.0f * cosAngle * sinAngle;
                    float m22 = m11;

                    // Top vertex
                    vertices[i].x = m11 * radius;
                    vertices[i].y = m12 * radius;
                    vertices[i].z = length * 0.5f;

                    // Bottom vertex
                    vertices[i + 2].x = m11 * radius;
                    vertices[i + 2].y = m12 * radius;
                    vertices[i + 2].z = -length * 0.5f;
                }

                // Draw two triangles for the quad
                CVector tri1[3] = {vertices[0], vertices[1], vertices[2]};
                CVector tri2[3] = {vertices[1], vertices[3], vertices[2]};

                M3D_APP->DrawTri(tri1, color);
                M3D_APP->DrawTri(tri2, color);
            }
            break;
        }

        case 7:  // Triangle Mesh
        {
            int numTriangles, numVertices;
            dGeomTriMeshGetInfo(geom, &numTriangles, &numVertices);

            for (int triIndex = 0; triIndex < numTriangles; ++triIndex)
            {
                float triangle[3][4];
                dGeomTriMeshGetTriangle(geom, triIndex, &triangle[0], &triangle[1], &triangle[2]);

                CVector vertices[3];
                vertices[0] = CVector(triangle[0][0], triangle[0][1], triangle[0][2]);
                vertices[1] = CVector(triangle[1][0], triangle[1][1], triangle[1][2]);
                vertices[2] = CVector(triangle[2][0], triangle[2][1], triangle[2][2]);

                M3D_APP->DrawTri(vertices, color);
            }
            break;
        }

        default:  // Invalid geometry class
        {
            M3D_LOG_ERR("Error: invalid geom class: " + CStr(geomClass));
            break;
        }
        }
    }

    void Landscape::RenderRoads()
    {
        if (M3D_ENGINE_CFG.m_g_drawRoads.GetB())
        {
            std::vector<unsigned int> visList;
            visList.reserve(0x3E8u);

            auto& graph = m_owner->GetGraph();
            graph.SortedCellsStartFetching(0, m_drawRadius);

            int x = 0;
            int y = 0;
            int vis = 0;
            int radius = 0;
            while (graph.SortedCellsFetch(x, y, vis, radius))
            {
                if (!vis)
                {
                    continue;
                }

                visList.push_back(x + (y << 16));
            }

            M3D_RENDERER->PushBlend(rend::BlendMode::BM_NONE);
            m_owner->GetRoadManager().RenderRoads(visList, RRT_SIMPLE, nullptr, false);
            M3D_RENDERER->PopBlend();
        }

        if (M3D_ENGINE_CFG.m_g_drawWheelTraces.GetB())
        {
            m_owner->GetWheelTracesMgr().Render();
        }
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
}  // namespace m3d

#define NOMINMAX
#include "roadmanager.h"
#include <algorithm>
#include <stdexcept>
#include <math/coremath.h>
#include <ode/collision.h>
#include <ode/collision_trimesh.h>
#include <core/kernel.h>
#include <config.h>
#include <landscape.h>
#include <core/timer.h>
#include <world.h>
#include <level.h>

#include "geomobject.h"
#include "m3dapp.h"
#include "road.h"
#include "skelmodel.h"
#include "core/ini.h"
#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include <client.h>

namespace m3d
{
    RoadInRadius2dTest::RoadInRadius2dTest(CVector const& origin, float r) : org(origin), radius(r)
    {
        // RVA 0x8A2240
    }

    bool RoadInRadius2dTest::TestRoadNode(RoadNode* rn) const
    {
        // RVA 0x7B3970 - the height of the node plays no part, so a road is
        // accepted whenever its bounding circle overlaps the test circle when
        // both are flattened onto the ground.
        float const dx = rn->m_boundCenter.x - org.x;
        float const dz = rn->m_boundCenter.z - org.z;
        float const r = rn->m_boundRadius + radius;
        return r * r > dx * dx + dz * dz;
    }

    CStr const RoadManager::GetRoadSetNameByHandle(int handle)
    {
        CStr toRet;
        if (static_cast<unsigned>(handle) < m_roadSets.size())
        {
            toRet = m_roadSets[handle]->m_name;
        }
        return toRet;
    }

    void RoadManager::UpdateVis()
    {
        if (this->m_coveredCells)
        {
            auto m_f = M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF();
            auto v4 = (int)(float)((float)(m_f * 8.0) + 4.0);
            if (v4 >= 4)
            {
                if (v4 > 12)
                    v4 = 12;
            }
            else
            {
                v4 = 4;
            }

            auto& v5 = m3d::g_Kernel->GetTimer();

            auto v7 = m_owner->m_owner;

            auto curFrame = v5.GetCurFrame();

            auto levelsize = v7->m_level->land_size;
            auto* Graph = m_owner->GetGraph();
            Graph->SortedCellsStartFetching(0, v4);

            int x = 0;
            int z = 0;
            int v = 0;
            int radius = 0;
            if (Graph->SortedCellsFetch(x, z, v, radius))
            {
                do
                {
                    if (v)
                    {
                        auto& v10 = this->m_coveredCells[x + levelsize * z];
                        for (int i = 0; i < v10.size(); ++i)
                        {
                            if (m_owner->m_frustumCull.testSphere(v10[i]->m_boundCenter, v10[i]->m_boundRadius))
                            {
                                v10[i]->m_frameVisible = curFrame;
                            }
                        }
                    }
                } while (Graph->SortedCellsFetch(x, z, v, radius));
            }
        }
    }

    void RoadManager::ReleaseCollisionForRoadNode(RoadNode* rn)
    {
        if (rn->m_geomObject)
        {
            rn->m_geomObject->Release();
            rn->m_geomObject->DecRef();
            rn->m_geomObject = nullptr;
            rn->m_cachedVertices = nullptr;
        }
    }

    void RoadManager::RebuildStructures()
    {
        LinkRoadNodes();

        for (auto* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node;
             node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            if (node->m_type == 2 || node->m_type == 1)
            {
                CalcNodeData(node);
            }
        }

        for (auto* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node;
             node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            if (node->m_type == 3)
            {
                CalcNodeData(node);
            }
        }

        for (auto* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node;
             node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            if (node->m_type == 0)
            {
                CalcNodeData(node);
            }
        }

        RecalcCoveredCells();
    }

    int RoadManager::GetRoadSetHandleByName(CStr const& name)
    {
        for (int i = 0; i < m_roadSets.size(); ++i)
        {
            if (m_roadSets[i]->m_name == name)
            {
                return i;
            }
        }
        return -1;
    }

    void RoadManager::RebuildSomeNodes(retruxx::set<RoadNode*> nodesToRebuild, bool bNeedToRelink)
    {
        if (bNeedToRelink)
        {
            LinkRoadNodes();
        }

        for (RoadNode* node : nodesToRebuild)
        {
            UnlinkRoadNodeCollisionFromCells(node);
            ReleaseCollisionForRoadNode(node);
        }

        // Junctions are rebuilt before the segments that butt against them, so
        // the segments can read the neighbours' freshly cached border vertices.
        for (RoadNode* node : nodesToRebuild)
        {
            if (node->m_type == 2 || node->m_type == 1)
            {
                CalcNodeData(node);
            }
        }

        for (RoadNode* node : nodesToRebuild)
        {
            if (node->m_type == 3)
            {
                CalcNodeData(node);
            }
        }

        for (RoadNode* node : nodesToRebuild)
        {
            if (node->m_type == 0)
            {
                CalcNodeData(node);
            }
        }

        RecalcCoveredCells();
    }

    void RoadManager::SetOwner(Landscape* landscape)
    {
        m_owner = landscape;
    }

    int RoadManager::RenderRoads(
        retruxx::vector<unsigned>& visList,
        RenderRoadType rrt,
        RoadTestCallBack const* rnTest,
        bool bForRoadMap)
    {
        // TODO: generated code RoadManager::RenderRoads

        // Early exits if no roads to render
        if (m_coveredCells == 0)
            return 0;

        if (m_roadSets.empty())
            return 0;

        // Initialize rendering data
        std::vector<m3d::RoadNode*> roadsToDraw;
        roadsToDraw.reserve(400);  // 0x190 = 400

        int curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        int numRoadPolys = 0;

        // Get weather and lighting information
        m3d::CWorld* world = m_owner->m_owner;

        rend::Colorf ambientColor = world->GetWeatherAmbientColor();
        CVector colorAmbient;
        colorAmbient.x = ambientColor.r;
        colorAmbient.y = ambientColor.g;
        colorAmbient.z = ambientColor.b;

        rend::Colorf diffuseColor = world->GetWeatherDiffuseColor();
        CVector colorDiffuse;
        colorDiffuse.x = diffuseColor.r;
        colorDiffuse.y = diffuseColor.g;
        colorDiffuse.z = diffuseColor.b;

        // Get fog parameters
        float fogStart, fogEnd;
        world->GetLandscape().GetFogStartAndEnd(fogStart, fogEnd);

        float fogReduceFactor = world->GetWeatherManager().GetFogReduceFactorFromWeather();
        CVector fogTerm;
        fogTerm.x = fogReduceFactor * fogEnd;
        fogTerm.z = fogReduceFactor * fogStart;
        fogTerm.y = 1.0f / (fogTerm.x - fogTerm.z);

        float const VISCELL_EDGE_LENGTH_30 = 128.0;

        // Setup lighting
        m3d::pClient->GetWorld().GetGraph().LightSetupSunForWorld();

        // Process visible cells
        for (unsigned int i = 0; i < visList.size(); ++i)
        {
            unsigned int cellIndex = visList[i];
            unsigned short xCoord = static_cast<unsigned short>(cellIndex);
            unsigned short yCoord = static_cast<unsigned short>(cellIndex >> 16);

            std::vector<m3d::RoadNode*>* cellRoads =
                &m_coveredCells[xCoord + m_owner->m_owner->m_level->land_size * yCoord];

            // Process each road node in the cell
            for (unsigned int j = 0; j < cellRoads->size(); ++j)
            {
                m3d::RoadNode* roadNode = (*cellRoads)[j];

                // Check if road node should be rendered
                if (!roadNode->m_bRoadDrawn && roadNode->m_frameVisible == curFrame &&
                    (!rnTest || rnTest->TestRoadNode(roadNode)))
                {
                    roadNode->m_bRoadDrawn = true;
                    roadsToDraw.push_back(roadNode);

                    // Get the road model for this node
                    m3d::RoadSet* roadSet = m_roadSets[roadNode->m_roadSetHandle];
                    m3d::AnimatedModel* roadModel = roadSet->m_roadModels[roadNode->m_type][roadNode->m_modelNum];

                    // Setup render states for simple road rendering
                    if (rrt == RRT_SIMPLE)
                    {
                        M3D_RENDERER->SetFog(1, 0);
                        M3D_RENDERER->SetCull(rend::M3DCULL_CCW, 0);
                        M3D_RENDERER->SetFillMode(rend::M3DFILL_SOLID, 0);
                        M3D_RENDERER->SetBlend(rend::BM_ALPHA, 0);
                        M3D_RENDERER->SetAlphaTest(10);
                        M3D_RENDERER->PushZbState(rend::ZB_ENABLE);
                    }

                    // Special rendering for road map
                    if (bForRoadMap)
                    {
                        M3D_RENDERER->SetFog(0, 0);
                        M3D_RENDERER->SetBlend(rend::BM_NONE, 0);
                        M3D_RENDERER->SetAlphaTest(0);
                    }

                    // Render all meshes in the road model
                    if (roadModel->GetNumMeshes() > 0)
                    {
                        for (unsigned int meshIndex = 0; meshIndex < roadModel->GetNumMeshes(); ++meshIndex)
                        {
                            auto& mesh = roadModel->GetMesh(meshIndex);

                            // Set vertex and index buffers
                            M3D_RENDERER->SetIndices(roadNode->m_IbPoolField, roadNode->m_VbPoolField.RealOffset);
                            M3D_RENDERER->SetToStream0(roadNode->m_VbPoolField);

                            // Select appropriate shader based on rendering mode
                            m3d::rend::IEffect* effect = nullptr;

                            auto& graph = m3d::pClient->GetWorld().GetGraph();
                            switch (rrt)
                            {
                            case 1:
                                effect = graph.GetRoadShadowShader();
                                break;
                            case 2:
                                effect = graph.GetRoadProjectorShader();
                                break;
                            case 3:
                                effect = graph.GetRoadDetShadowShader();
                                break;
                            case 4:
                                effect = graph.GetRoadLightShader();
                                break;
                            case 5:
                                effect = graph.GetRoadSpriteShader();
                                break;
                            default:
                                // Use material-based shader
                                if (roadNode->m_skinNumber < roadModel->GetNumSkins())
                                {
                                    std::vector<m3d::DSurfaceMaterial>& skin =
                                        roadModel->GetSkin(roadNode->m_skinNumber);
                                    int materialIndex = mesh.m_MaterialNumber;
                                    effect = roadModel->ApplyMaterial(skin[materialIndex]);
                                }
                                else
                                {
                                    std::vector<m3d::DSurfaceMaterial>& defaultSkin = roadModel->GetSkin(0);
                                    int materialIndex = mesh.m_MaterialNumber;
                                    effect = roadModel->ApplyMaterial(defaultSkin[materialIndex]);
                                }
                                break;
                            }

                            if (!effect)
                            {
                                // Handle error - drawing road without shader
                                continue;
                            }

                            // Set shader parameters
                            if (effect->IsParameterUsed(rend::IEffect::LightAmbient))
                                effect->SetVector3(rend::IEffect::LightAmbient, colorAmbient);

                            if (effect->IsParameterUsed(rend::IEffect::LightDiffuse))
                                effect->SetVector3(rend::IEffect::LightDiffuse, colorDiffuse);

                            if (effect->IsParameterUsed(rend::IEffect::FogTerm))
                                effect->SetVector3(rend::IEffect::FogTerm, fogTerm);

                            // Set lightmap if needed
                            if (effect->IsParameterUsed(rend::IEffect::LightMap0))
                            {
                                float scale = 1.0f / (m_owner->m_owner->m_level->land_size * VISCELL_EDGE_LENGTH_30);
                                CVector lightmapScale(-scale, scale, 0.0f);
                                effect->SetVector3(rend::IEffect::User_float3_param, lightmapScale);

                                m3d::rend::TexHandle lightmap = m_owner->GetLightmapTexture();
                                effect->SetTexture(rend::IEffect::LightMap0, &lightmap);
                            }

                            // Draw the mesh
                            M3D_RENDERER->DrawIndexedPrimitiveEffect(
                                rend::M3DPT_TRIANGLELIST,
                                effect,
                                0,
                                mesh.m_numDrawVerts,
                                roadNode->m_IbPoolField.RealOffset,
                                mesh.m_numFaces);

                            numRoadPolys += mesh.m_numFaces;
                        }
                    }

                    // Restore render states for simple road rendering
                    if (rrt == RRT_SIMPLE)
                    {
                        M3D_RENDERER->PopZbState();
                    }
                }
            }
        }

        // Display debug information for simple road rendering
        if (rrt == RRT_SIMPLE)
        {
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("# road tris = " + CStr(numRoadPolys)).c_str());
        }

        // Reset visibility flags for rendered road nodes
        for (m3d::RoadNode* roadNode : roadsToDraw)
        {
            roadNode->m_bRoadDrawn = false;
        }

        return 1;
    }

    void RoadManager::GetRoadMinMaxZByHandle(int handle, int roadType, float& minz, float& maxz)
    {
        if (static_cast<unsigned>(handle) < m_roadSets.size())
        {
            minz = m_roadSets[handle]->m_minZ[roadType];
            maxz = m_roadSets[handle]->m_maxZ[roadType];
        }
    }

    void RoadSet::Clear()
    {
        for (auto& roadModel : m_roadModels)
        {
            for (auto& model : roadModel)
            {
                delete model;
            }
            roadModel.clear();
        }
    }

    int RoadSet::ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node)
    {
        Clear();

        m3d::SafeVectorAttrib(m_scale, node, "scale");
        m3d::SafeStrAttrib(m_name, node, "name");
        m3d::SafeStrAttrib(m_wheeltraceTexName, node, "WheelTrace");

        ref_ptr itemNode = file->CreateNode();
        for (node->GetFirstChild(itemNode, "Item"); !itemNode->IsEmpty(); itemNode->GetNextSibling(itemNode, "Item"))
        {
            CStr fileName = itemNode->GetAttribute("model");

            int type = 0;
            m3d::SafeIntAttrib(type, itemNode, "type");

            auto* model = new AnimatedModel;
            auto const loadRes =
                M3D_ENGINE_CFG.m_loadFromGAM.GetB() ? model->LoadGAM(fileName, false) : model->LoadSAM(fileName, false);
            if (loadRes)
            {
                m_roadModels[type].push_back(model);

                if (m_roadModels[type].size() <= 1)
                {
                    float minX = 10000.0;
                    float minZ = 10000.0;
                    float maxX = -10000.0;
                    float maxZ = -10000.0;

                    auto& firstMesh = model->GetMesh(0);
                    float* verts = static_cast<float*>(firstMesh.m_verts);
                    auto const vertexStride = firstMesh.m_VertexTypeSize / sizeof(float);

                    // TODO: check this
                    for (int i = 0; i < firstMesh.m_numVertices; ++i)
                    {
                        minX = std::min(minX, verts[0]);
                        minZ = std::min(minZ, verts[2]);
                        maxX = std::max(maxX, verts[0]);
                        maxZ = std::max(maxZ, verts[2]);

                        verts += vertexStride;
                    }

                    m_sizeZ[type] = maxZ - minZ;
                    m_sizeX[type] = maxX - minX;
                    m_minX[type] = minX;
                    m_minZ[type] = minZ;
                    m_maxX[type] = maxX;
                    m_maxZ[type] = maxZ;
                }
            }
            else
            {
                M3D_LOG_ERR("RoadManager error( model not loaded ): " + CStr(fileName));
                delete model;
            }
        }

        // TODO: generated code
        // Process road boundaries and indices
        float const BOUNDARY_TOLERANCE = 0.1f;
        float const VERTEX_MATCH_TOLERANCE = 0.01f;

        for (unsigned int roadType = 0; roadType < 4; ++roadType)
        {
            auto& models = m_roadModels[roadType];

            for (unsigned int modelIndex = 0; modelIndex < models.size(); ++modelIndex)
            {
                auto* model = models[modelIndex];
                if (!model)
                {
                    continue;
                }

                // Initialize boundary data structures
                m_boundVerts[roadType].resize(modelIndex + 1);
                m_boundVerts[roadType][modelIndex].resize(4);

                m_fakeBoundVerts[roadType].resize(modelIndex + 1);
                m_fakeBoundVerts[roadType][modelIndex].resize(4);

                m_cliffBorders[roadType].resize(modelIndex + 1);
                m_cliffBorders[roadType][modelIndex].resize(1);

                // Process mesh vertices for boundary detection
                auto& mesh = model->GetMesh(0);
                int numVertices = mesh.m_numVertices;
                float* vertices = static_cast<float*>(mesh.m_verts);
                auto const vertexStride = mesh.m_VertexTypeSize / sizeof(float);

                float minX = m_minX[roadType];
                float maxX = m_maxX[roadType];
                float minZ = m_minZ[roadType];
                float maxZ = m_maxZ[roadType];

                for (int vertexIndex = 0; vertexIndex < numVertices; ++vertexIndex)
                {
                    float x = vertices[0];
                    float y = vertices[1];
                    float z = vertices[2];

                    // Check which boundary this vertex belongs to
                    if (std::fabs(z - minZ) < BOUNDARY_TOLERANCE)
                    {
                        // Bottom boundary
                        auto& boundary = m_boundVerts[roadType][modelIndex][1];  // BOTTOM
                        auto& fakeBoundary = m_fakeBoundVerts[roadType][modelIndex][1];

                        bool found = false;
                        for (unsigned int existingIndex : boundary)
                        {
                            float* existingVertex = reinterpret_cast<float*>(
                                static_cast<char*>(mesh.m_verts) + existingIndex * mesh.m_VertexTypeSize);

                            float dx = existingVertex[0] - x;
                            float dy = existingVertex[1] - y;
                            float dz = existingVertex[2] - z;
                            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

                            if (distance < VERTEX_MATCH_TOLERANCE)
                            {
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            auto it = boundary.begin();
                            while (it != boundary.end())
                            {
                                float* existingVertex = reinterpret_cast<float*>(
                                    static_cast<char*>(mesh.m_verts) + (*it) * mesh.m_VertexTypeSize);

                                if (x < existingVertex[0])
                                    break;
                                ++it;
                            }
                            boundary.insert(it, vertexIndex);
                        }
                        else
                        {
                            fakeBoundary.push_back(vertexIndex);
                        }

                        // Add to cliff borders
                        if (std::fabs(x - maxX) < BOUNDARY_TOLERANCE)
                        {
                            m_cliffBorders[roadType][modelIndex][0].push_back(vertexIndex);
                        }
                    }
                    else if (std::fabs(z - maxZ) < BOUNDARY_TOLERANCE)
                    {
                        // Top boundary
                        std::vector<unsigned int>& boundary = m_boundVerts[roadType][modelIndex][0];  // TOP
                        std::vector<unsigned int>& fakeBoundary = m_fakeBoundVerts[roadType][modelIndex][0];

                        bool found = false;
                        for (unsigned int existingIndex : boundary)
                        {
                            float* existingVertex = reinterpret_cast<float*>(
                                static_cast<char*>(mesh.m_verts) + existingIndex * mesh.m_VertexTypeSize);

                            float dx = existingVertex[0] - x;
                            float dy = existingVertex[1] - y;
                            float dz = existingVertex[2] - z;
                            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

                            if (distance < VERTEX_MATCH_TOLERANCE)
                            {
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            auto it = boundary.begin();
                            while (it != boundary.end())
                            {
                                float* existingVertex = reinterpret_cast<float*>(
                                    static_cast<char*>(mesh.m_verts) + (*it) * mesh.m_VertexTypeSize);

                                if (x < existingVertex[0])
                                    break;
                                ++it;
                            }
                            boundary.insert(it, vertexIndex);
                        }
                        else
                        {
                            fakeBoundary.push_back(vertexIndex);
                        }
                    }
                    else if (std::fabs(x - minX) < BOUNDARY_TOLERANCE)
                    {
                        // Left boundary
                        std::vector<unsigned int>& boundary = m_boundVerts[roadType][modelIndex][2];  // LEFT
                        std::vector<unsigned int>& fakeBoundary = m_fakeBoundVerts[roadType][modelIndex][2];

                        bool found = false;
                        for (unsigned int existingIndex : boundary)
                        {
                            float* existingVertex = reinterpret_cast<float*>(
                                static_cast<char*>(mesh.m_verts) + existingIndex * mesh.m_VertexTypeSize);

                            float dx = existingVertex[0] - x;
                            float dy = existingVertex[1] - y;
                            float dz = existingVertex[2] - z;
                            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

                            if (distance < VERTEX_MATCH_TOLERANCE)
                            {
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            auto it = boundary.begin();
                            while (it != boundary.end())
                            {
                                float* existingVertex = reinterpret_cast<float*>(
                                    static_cast<char*>(mesh.m_verts) + (*it) * mesh.m_VertexTypeSize);

                                if (z < existingVertex[2])
                                    break;
                                ++it;
                            }
                            boundary.insert(it, vertexIndex);
                        }
                        else
                        {
                            fakeBoundary.push_back(vertexIndex);
                        }
                    }
                    else if (std::fabs(x - maxX) < BOUNDARY_TOLERANCE)
                    {
                        // Right boundary
                        std::vector<unsigned int>& boundary = m_boundVerts[roadType][modelIndex][3];  // RIGHT
                        std::vector<unsigned int>& fakeBoundary = m_fakeBoundVerts[roadType][modelIndex][3];

                        bool found = false;
                        for (unsigned int existingIndex : boundary)
                        {
                            float* existingVertex = reinterpret_cast<float*>(
                                static_cast<char*>(mesh.m_verts) + existingIndex * mesh.m_VertexTypeSize);

                            float dx = existingVertex[0] - x;
                            float dy = existingVertex[1] - y;
                            float dz = existingVertex[2] - z;
                            float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

                            if (distance < VERTEX_MATCH_TOLERANCE)
                            {
                                found = true;
                                break;
                            }
                        }

                        if (!found)
                        {
                            auto it = boundary.begin();
                            while (it != boundary.end())
                            {
                                float* existingVertex = reinterpret_cast<float*>(
                                    static_cast<char*>(mesh.m_verts) + (*it) * mesh.m_VertexTypeSize);

                                if (z < existingVertex[2])
                                    break;
                                ++it;
                            }
                            boundary.insert(it, vertexIndex);
                        }
                        else
                        {
                            fakeBoundary.push_back(vertexIndex);
                        }

                        // Add to cliff borders
                        m_cliffBorders[roadType][modelIndex][0].push_back(vertexIndex);
                    }

                    vertices += vertexStride;
                }
            }
        }

        return true;
    }

    RoadSet::RoadSet()
    {
        m_wheeltraceTexName = M3D_ENGINE_CFG.m_skidTexName.GetS();
        m_soilType = -1;
    }

    RoadSet::~RoadSet()
    {
        Clear();
    }

    RoadManager::RoadManager()
    {
        m_owner = nullptr;
        m_coveredCells = nullptr;
        m_roadRoot = nullptr;
    }

    int RoadManager::WriteRoadsToXmlFile(char const* filename)
    {
        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        ref_ptr roadsNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Roads");

        for (Object* node = m_roadRoot->GetFirstChild(); node; node = node->GetNextSibling())
        {
            ref_ptr roadNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "RoadNode");
            node->WriteToXmlNode(xmlFile, roadNode);
            roadsNode->AddChild(roadNode);
        }
        xmlFile->AddChild(roadsNode);

        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(filename, fs::IStream::OPEN_WRITE))
        {
            M3D_LOG_INFO("Write roads level: could not save servers into file " + CStr(filename));
            return 0;
        }

        xmlFile->Write(*stream);
        stream->Close();
        return 1;
    }

    void RoadManager::GetRoadMinMaxXByHandle(int handle, int roadType, float& minx, float& maxx)
    {
        if (static_cast<unsigned>(handle) < m_roadSets.size())
        {
            minx = m_roadSets[handle]->m_minX[roadType];
            maxx = m_roadSets[handle]->m_maxX[roadType];
        }
    }

    void RoadManager::UnlinkRoadNodeCollisionFromCells(RoadNode* rn)
    {
        if (!rn->m_geomObject)
        {
            return;
        }
        PointBase<int> const startCell = rn->m_geomObject->GetStartCell();
        PointBase<int> const endCell = rn->m_geomObject->GetEndCell();

        for (int x = startCell.x; x <= endCell.x; ++x)
        {
            for (int y = startCell.y; y <= endCell.y; ++y)
            {
                auto* cell = m_owner->GetCollisionCellItem(x, y);
                auto it = cell->m_geomsList.find(rn->m_geomObject);
                if (it != cell->m_geomsList.end())
                {
                    cell->m_geomsList.erase(it);
                }
            }
        }
    }

    int RoadManager::ReadRoadSetConfigFromXmlFile(char const* configName)
    {
        CStr error;
        ref_ptr xmlFile = m3d::ReadXmlFile(configName, &error);
        if (xmlFile)
        {
            ClearRoadSets();
            ref_ptr roadsNode = xmlFile->CreateNode();
            xmlFile->GetFirstChild(roadsNode, "Roads");

            ref_ptr setNode = xmlFile->CreateNode();
            for (roadsNode->GetFirstChild(setNode, "Set"); !setNode->IsEmpty(); setNode->GetNextSibling(setNode, "Set"))
            {
                auto* roadSet = new RoadSet;
                if (roadSet->ReadFromXmlNode(xmlFile, setNode))
                {
                    m_roadSets.push_back(roadSet);
                }
                else
                {
                    delete roadSet;
                }
            }
            return 1;
        }

        M3D_LOG_INFO("Can't read roadset file: " + CStr(configName));

        xmlFile = m3d::ReadXmlFile("data/Roads.xml", &error);
        if (xmlFile)
        {
            M3D_LOG_INFO("Can't read RoadConfig file: " + CStr(configName));
        }
        else
        {
            M3D_LOG_INFO("Can't read roadset file: data/Roads.xml");
        }

        return 0;
    }

    int RoadManager::ReadRoadsFromXmlFile(char const* name)
    {
        M3D_ASSERT(m_roadRoot);

        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(name, fs::IStream::OPEN_READ))
        {
            M3D_LOG_ERR("Error: RoadManager can't read the file: " + CStr(name));
            return 0;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (!xmlFile->Read(*stream))
        {
            M3D_LOG_ERR("Error: RoadManager: error in parsing: " + CStr(name) + "(" + CStr(xmlFile->GetError()) + ")");
            return 0;
        }

        ref_ptr roadsNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(roadsNode, "Roads");
        if (roadsNode->IsEmpty())
        {
            M3D_LOG_ERR("Error: RoadManager can't find Root node in " + CStr(name));
            return 0;
        }

        ref_ptr roadNode = xmlFile->CreateNode();
        for (roadsNode->GetFirstChild(roadNode, "RoadNode"); !roadNode->IsEmpty();
             roadNode->GetNextSibling(roadNode, "RoadNode"))
        {
            CStr name = roadNode->GetAttribute("name");
            CStr cls = roadNode->GetAttribute("class");
            auto* obj = M3D_KERNEL->New(cls.c_str());
            if (obj->ReadFromXmlNode(xmlFile, roadNode))
            {
                auto* road = static_cast<RoadNode*>(obj);
                road->m_roadSetHandle = GetRoadSetHandleByName(road->m_roadSetName);
                m_roadRoot->AddChild(road);
            }
            else
            {
                // TODO: refcountedbase delete
                delete obj;
            }
        }

        RebuildStructures();
        return 1;
    }

    void RoadManager::Release()
    {
        if (m_roadRoot)
        {
            m_roadRoot->RemoveAllChildren();
        }

        ClearRoadSets();

        delete m_roadRoot;
        m_roadRoot = nullptr;

        delete[] m_coveredCells;
        m_coveredCells = nullptr;
    }

    void RoadManager::Init()
    {
        M3D_ASSERT(m_roadRoot == nullptr);
        m_roadRoot = (m3d::RoadNode*)M3D_KERNEL->New("RoadNode");

        M3D_ASSERT(m_coveredCells == nullptr);
    }

    void RoadManager::ClearRoadSets()
    {
        for (auto& roadSet : m_roadSets)
        {
            delete roadSet;
        }

        m_roadSets.clear();
    }

    void RoadManager::ReleaseCollision()
    {
        for (auto* child = dynamic_cast<m3d::RoadNode*>(m_roadRoot->GetFirstChild()); child;
             child = dynamic_cast<m3d::RoadNode*>(child->GetNextSibling()))
        {
            auto& geom = child->m_geomObject;
            if (geom)
            {
                geom->Release();
                delete geom;
                geom = nullptr;
            }
        }
    }

    void RoadManager::RecalcCoveredCells()
    {
        int const levelsize = m_owner->m_owner->m_level->land_size;

        delete[] m_coveredCells;
        m_coveredCells = new retruxx::vector<RoadNode*>[levelsize * levelsize];

        float const cellSizeInv = 1.0f / 128.0f;
        for (auto* rn = static_cast<RoadNode*>(m_roadRoot->GetFirstChild()); rn;
             rn = static_cast<RoadNode*>(rn->GetNextSibling()))
        {
            if (!rn->m_geomObject)
            {
                continue;
            }

            int const maxIdx = levelsize - 1;
            int const x0 = std::clamp(static_cast<int>(rn->minb.x * cellSizeInv), 0, maxIdx);
            int const y0 = std::clamp(static_cast<int>(rn->minb.z * cellSizeInv), 0, maxIdx);
            int const x1 = std::clamp(static_cast<int>(rn->maxb.x * cellSizeInv), 0, maxIdx);
            int const y1 = std::clamp(static_cast<int>(rn->maxb.z * cellSizeInv), 0, maxIdx);

            PointBase<int> startCell;
            startCell.x = x0;
            startCell.y = y0;
            PointBase<int> endCell;
            endCell.x = x1;
            endCell.y = y1;
            rn->m_geomObject->SetBounds(startCell, endCell);

            for (int x = x0; x <= x1; ++x)
            {
                for (int y = y0; y <= y1; ++y)
                {
                    int const cell = levelsize * y + x;
                    m_owner->m_oCollisionitems[cell]->m_geomsList.insert(rn->m_geomObject);
                    m_coveredCells[cell].push_back(rn);
                }
            }
        }
    }

    void RoadManager::LinkToBorder(RoadNode* rn, unsigned idx, int link, CVector& res)
    {
        // Walks to the node joined on `link`, finds which of its own four link
        // slots points back here, and samples that shared border.
        RoadNode* neighbour = nullptr;
        RoadNode* expected = rn;

        switch (link)
        {
        case 0:
            neighbour = rn->m_linkedNodes[0];
            if (!neighbour)
            {
                return;
            }
            if (neighbour->m_friend && neighbour->m_friend->m_linkedNodes[1])
            {
                expected = neighbour->m_friend;
                neighbour = neighbour->m_friend->m_linkedNodes[1];
            }
            break;

        case 1:
            neighbour = rn->m_linkedNodes[1];
            if (!neighbour)
            {
                neighbour = rn->m_friend;
                if (!neighbour || !neighbour->m_linkedNodes[0])
                {
                    return;
                }
                expected = nullptr;
            }
            break;

        case 2:
        case 3:
            neighbour = rn->m_linkedNodes[link];
            if (!neighbour)
            {
                return;
            }
            break;

        default:
            return;
        }

        for (unsigned i = 0; i < 4; ++i)
        {
            if (neighbour->m_linkedNodes[i] == expected)
            {
                GetAdjPoint(neighbour, idx, i, res);
                return;
            }
        }
    }

    void RoadManager::LinkRoadNodes()
    {
        // TODO: generated code
        // Find friend nodes first
        FindFriends();

        // Iterate through all road nodes
        for (RoadNode* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node != nullptr;
             node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            node->m_owner = this;
            int linkCounter = 0;

            // Process all 4 possible linked nodes
            for (int linkIndex = 0; linkIndex < 4; ++linkIndex)
            {
                CStr const& linkedName = node->m_linkedNames[linkIndex];
                if (!linkedName.empty())
                {
                    // Search for the linked node by name
                    RoadNode* foundNode = nullptr;
                    for (RoadNode* child = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); child != nullptr;
                         child = dynamic_cast<RoadNode*>(child->GetNextSibling()))
                    {
                        if (child->GetName() == linkedName)
                        {
                            foundNode = child;
                            break;
                        }
                    }

                    node->m_linkedNodes[linkIndex] = foundNode;
                    if (foundNode != nullptr)
                    {
                        ++linkCounter;
                    }
                }
                else
                {
                    node->m_linkedNodes[linkIndex] = nullptr;
                }
            }

            // Special case: if first link exists but has no name, don't count it
            RoadNode* firstLinked = node->m_linkedNodes[0];
            if (firstLinked != nullptr)
            {
                if (firstLinked->m_linkedNames[0].empty())
                {
                    --linkCounter;
                }
            }

            node->m_bInverted = false;

            // Determine node type based on link count
            switch (linkCounter)
            {
            case 0:
                node->m_type = 3;  // Dead end
                break;

            case 1:
                node->m_type = 3;  // Dead end
                if (firstLinked && node->m_linkedNodes[1] != nullptr)
                {
                    node->m_bInverted = true;
                }
                break;

            case 2:
                node->m_type = 0;  // Straight road
                break;

            case 3:
                node->m_type = 1;  // T-junction
                {
                    // Determine inversion based on cross product of vectors
                    RoadNode* link1 = node->m_linkedNodes[1];
                    RoadNode* link2 = node->m_linkedNodes[2];
                    RoadNode* link3 = node->m_linkedNodes[3];

                    // If link1 is null, use current node as reference
                    if (link1 == nullptr)
                    {
                        firstLinked = node;
                    }

                    if (link2 != nullptr && link3 != nullptr && firstLinked != nullptr)
                    {
                        // Calculate cross product to determine winding order
                        float dx1 = link3->m_origin.z - link2->m_origin.z;
                        float dz1 = firstLinked->m_origin.x - link2->m_origin.x;
                        float dx2 = firstLinked->m_origin.z - link2->m_origin.z;
                        float dz2 = link3->m_origin.x - link2->m_origin.x;

                        float crossProduct = (dx1 * dz1) - (dx2 * dz2);

                        if (crossProduct < 0.0f)
                        {
                            node->m_bInverted = true;
                        }
                    }
                }
                break;

            case 4:
                node->m_type = 2;  // Crossroad
                break;

            default:
                M3D_LOG_INFO("RoadNode not connected: " + CStr(node->GetName()));
                node->m_type = 3;  // Default to dead end
                break;
            }

            // Validate and clamp model number
            RoadSet* roadSet = m_roadSets[node->m_roadSetHandle];
            if (roadSet != nullptr)
            {
                std::vector<AnimatedModel*>& models = roadSet->m_roadModels[node->m_type];

                if (!models.empty())
                {
                    unsigned int maxModelIndex = models.size() - 1;
                    if (node->m_modelNum > maxModelIndex)
                    {
                        node->m_modelNum = maxModelIndex;
                    }

                    // Validate and clamp skin number
                    AnimatedModel* model = models[node->m_modelNum];
                    if (model != nullptr)
                    {
                        unsigned int maxSkinIndex = model->GetNumSkins() - 1;
                        if (node->m_skinNumber > maxSkinIndex)
                        {
                            node->m_skinNumber = maxSkinIndex;
                        }
                    }
                }
            }

            // Override type for friend nodes
            if (node->m_friend != nullptr)
            {
                node->m_type = 0;  // Force straight road
                node->m_bInverted = false;
            }

            // Override type for nodes with specific linked node properties
            RoadNode* firstLink = node->m_linkedNodes[0];
            if (firstLink != nullptr && !firstLink->m_linkedNames[0].empty())
            {
                node->m_type = 0;  // Force straight road
                node->m_bInverted = false;
            }

            // Set bounds from road set
            if (node->m_roadSetHandle < m_roadSets.size())
            {
                RoadSet* currentSet = m_roadSets[node->m_roadSetHandle];
                if (currentSet != nullptr)
                {
                    node->m_minX = currentSet->m_minX[node->m_type];
                    node->m_maxX = currentSet->m_maxX[node->m_type];
                }
            }
        }
    }

    CVector2 RoadManager::FindLeftProjection(RoadNode* rn, float x, float z)
    {
        RoadNode const* next = rn->m_linkedNodes[0];

        // Perpendicular to the segment in the XZ plane, pointing left.
        float const perpX = -(next->m_origin.z - rn->m_origin.z);
        float const perpZ = next->m_origin.x - rn->m_origin.x;

        float const halfWidth = m_roadSets[rn->m_roadSetHandle]->m_sizeX[rn->m_type] * 0.5f;
        float const invPerpLen = 1.0f / sqrtf(perpZ * perpZ + perpX * perpX + 0.00000011920929f);
        float const offsetX = invPerpLen * perpX * halfWidth;
        float const offsetZ = invPerpLen * perpZ * halfWidth;

        // Both segment ends shifted onto the road's left edge.
        float const ax = rn->m_origin.x + offsetX;
        float const az = rn->m_origin.z + offsetZ;
        float const bx = next->m_origin.x + offsetX;
        float const bz = next->m_origin.z + offsetZ;

        float const invEdgeLen = 1.0f / sqrtf((bz - az) * (bz - az) + (bx - ax) * (bx - ax) + 0.00000011920929f);
        float const dirX = (bx - ax) * invEdgeLen;
        float const dirZ = (bz - az) * invEdgeLen;

        float const t = (z - az) * dirZ + (x - ax) * dirX;

        CVector2 result;
        result.x = dirX * t + ax;
        result.y = dirZ * t + az;
        return result;
    }

    bool RoadManager::GetAdjPoint(RoadNode* rn, unsigned idx, int link, CVector& acceptor)
    {
        if (!rn || !rn->m_cachedVertices)
        {
            return false;
        }
        if (rn->m_bInverted)
        {
            if (link == 0)
            {
                link = 1;
            }
            else if (link == 1)
            {
                link = 0;
            }
        }

        int const type = rn->m_type;
        auto const& bound = m_roadSets[rn->m_roadSetHandle]->m_boundVerts[type][rn->m_modelNum][link];
        if (bound.empty() || idx >= bound.size())
        {
            return false;
        }

        // Which of the neighbour's four slots links back to rn (4 when absent).
        auto backLinkIndex = [rn](RoadNode const* neighbour)
        {
            unsigned i = 0;
            while (i < 4 && neighbour->m_linkedNodes[i] != rn)
            {
                ++i;
            }
            return i;
        };

        // The two nodes sharing a border walk it from opposite ends, so one of
        // them has to read the vertex list backwards for the seam to line up.
        bool reversed = false;
        if (type == 2)
        {
            if (link == 2)
            {
                reversed = backLinkIndex(rn->m_linkedNodes[2]) == 0;
            }
            else if (link == 3)
            {
                reversed = backLinkIndex(rn->m_linkedNodes[3]) == 1;
            }
        }
        else if (type == 1)
        {
            if (link == 2)
            {
                unsigned const back = backLinkIndex(rn->m_linkedNodes[2]);
                if (back == 1)
                {
                    reversed = rn->m_bInverted;
                }
                else if (back == 0)
                {
                    reversed = !rn->m_bInverted;
                }
            }
            else if (link == 3)
            {
                unsigned const back = backLinkIndex(rn->m_linkedNodes[3]);
                if (back == 0)
                {
                    reversed = rn->m_bInverted;
                }
                else if (back == 1)
                {
                    reversed = !rn->m_bInverted;
                }
            }
        }
        else if (type == 0)
        {
            RoadNode const* target = link == 0 ? rn->m_linkedNodes[0] : (link == 1 ? rn : nullptr);
            reversed = target && target->m_friend;
        }

        acceptor = rn->m_cachedVertices[bound[reversed ? bound.size() - 1 - idx : idx]];
        return true;
    }

    void RoadManager::FindFriends()
    {
        for (RoadNode* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node != nullptr;
             node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            node->m_friend = nullptr;
        }

        for (RoadNode* node1 = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node1 != nullptr;
             node1 = dynamic_cast<RoadNode*>(node1->GetNextSibling()))
        {
            for (RoadNode* node2 = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node2 != nullptr;
                 node2 = dynamic_cast<RoadNode*>(node2->GetNextSibling()))
            {
                if (node1 != node2)
                {
                    auto const x = node2->m_origin.x - node1->m_origin.x;
                    auto const y = node2->m_origin.y - node1->m_origin.y;
                    auto const z = node2->m_origin.z - node1->m_origin.z;
                    if ((((x * x) + (y * y)) + (z * z)) < 1.0)
                    {
                        node2->m_friend = node1;
                        node1->m_friend = node2;
                    }
                }
            }
        }
    }

    void RoadManager::CalcNodeData(RoadNode* rn)
    {
        if (!rn->m_linkedNodes[0])
        {
            rn->m_geomObject = nullptr;
            return;
        }

        UnlinkRoadNodeCollisionFromCells(rn);
        ReleaseCollisionForRoadNode(rn);
        rn->m_cellsCovered.clear();

        RoadSet* roadSet = m_roadSets[rn->m_roadSetHandle];
        AnimatedModel* model = roadSet->m_roadModels[rn->m_type][rn->m_modelNum];
        AnimatedModel::Mesh* mesh = &model->GetMesh(0);
        float const sizeZ = roadSet->m_sizeZ[rn->m_type];

        rn->minb = CVector(10000.0f, 0.0f, 10000.0f);
        rn->maxb = CVector(-10000.0f, 0.0f, -10000.0f);
        rn->m_maxWorldY = -1000.0f;

        RoadNode const* next = rn->m_linkedNodes[0];
        CVector2 dir;
        dir.x = next->m_origin.x - rn->m_origin.x;
        dir.y = next->m_origin.z - rn->m_origin.z;
        CVector2 ref;
        ref.x = 0.0f;
        ref.y = 1.0f;
        float const sinAngle = sinf(CalculateAngle(dir, ref));

        int const numVertices = mesh->m_numVertices;
        int const stride = mesh->m_VertexTypeSize;
        int const tailSize = stride - 12;
        char const* const srcVerts = static_cast<char const*>(mesh->m_verts);

        CVector* worldVerts = new CVector[numVertices];

        if (rn->m_VbPoolField.Vb.IsValid())
        {
            M3D_RENDERER->ReleaseVbPoolField(rn->m_VbPoolField);
        }
        if (rn->m_IbPoolField.Ib.IsValid())
        {
            M3D_RENDERER->ReleaseIbPoolField(rn->m_IbPoolField);
        }
        rn->m_VbPoolField = M3D_RENDERER->AddVbPoolField(mesh->m_VertexType, numVertices);
        rn->m_IbPoolField = M3D_RENDERER->AddIbPoolField(3 * mesh->m_numFaces);
        char* vbCursor = static_cast<char*>(M3D_RENDERER->LockVbPoolField(rn->m_VbPoolField));

        // Pass 1: place every model vertex onto the road spline, then pull the
        // ones sitting on a shared border onto the neighbour's matching vertex
        // so the seam stays watertight.
        for (int k = 0; k < numVertices; ++k)
        {
            float const* src = reinterpret_cast<float const*>(srcVerts + stride * k);
            float const mx = src[0];
            float const my = src[1];
            // An inverted node mirrors the model along Z. The shipped build runs
            // this through a matrix it never fills in, so all that survives of
            // the intended rotation is the sign flip.
            float const mz = rn->m_bInverted ? -src[2] : src[2];

            float t = (sizeZ * 0.5f + mz) / sizeZ;
            if (t < 0.0f)
            {
                t = 0.0f;
            }
            else if (t > 1.0f)
            {
                t = 1.0f;
            }

            CVector onRoad = rn->GetLinkPoint(t, mx);
            float const height = rn->m_asCliff ? onRoad.y + my : m_owner->GetLsHeight(onRoad.x, onRoad.z) + my;

            CVector placed;
            placed.x = onRoad.x;
            placed.y = height;
            placed.z = onRoad.z;

            auto const& bounds = roadSet->m_boundVerts[rn->m_type][rn->m_modelNum];
            for (unsigned b = 0; b < bounds.size(); ++b)
            {
                auto const& list = bounds[b];
                auto found = std::find(list.begin(), list.end(), static_cast<unsigned>(k));
                if (found == list.end())
                {
                    continue;
                }
                unsigned const borderIdx = static_cast<unsigned>(found - list.begin());
                unsigned link = b;
                if (rn->m_bInverted)
                {
                    switch (b)
                    {
                    case 0:
                        link = 1;
                        break;
                    case 1:
                        link = 0;
                        break;
                    case 2:
                        link = 3;
                        break;
                    case 3:
                        link = 2;
                        break;
                    default:
                        break;
                    }
                }
                LinkToBorder(rn, borderIdx, link, placed);
                break;
            }

            worldVerts[k] = placed;

            rn->minb.x = std::min(rn->minb.x, placed.x);
            rn->minb.z = std::min(rn->minb.z, placed.z);
            rn->maxb.x = std::max(rn->maxb.x, placed.x);
            rn->maxb.z = std::max(rn->maxb.z, placed.z);
            rn->m_maxWorldY = std::max(rn->m_maxWorldY, placed.y);
        }

        // Pass 2: vertices on a "fake" border are duplicates of real border
        // vertices, so snap each one onto whichever real border vertex shares
        // its model-space position.
        auto const& fakeBounds = roadSet->m_fakeBoundVerts[rn->m_type][rn->m_modelNum];
        auto const& realBounds = roadSet->m_boundVerts[rn->m_type][rn->m_modelNum];
        for (int i = 0; i < numVertices; ++i)
        {
            for (unsigned f = 0; f < fakeBounds.size(); ++f)
            {
                auto const& fakeList = fakeBounds[f];
                if (std::find(fakeList.begin(), fakeList.end(), static_cast<unsigned>(i)) == fakeList.end())
                {
                    continue;
                }
                float const* self = reinterpret_cast<float const*>(srcVerts + stride * i);
                for (unsigned other : realBounds[f])
                {
                    float const* cand = reinterpret_cast<float const*>(srcVerts + stride * other);
                    float const dx = self[0] - cand[0];
                    float const dy = self[1] - cand[1];
                    float const dz = self[2] - cand[2];
                    if (sqrtf(dx * dx + dy * dy + dz * dz) < 0.1f)
                    {
                        worldVerts[i] = worldVerts[other];
                    }
                }
                break;
            }
        }

        // Pass 3: fill the vertex buffer - placed position, the rest of the
        // source vertex verbatim, then a corrected normal.
        for (int i = 0; i < numVertices; ++i)
        {
            char const* src = srcVerts + stride * i;
            float* dst = reinterpret_cast<float*>(vbCursor);
            dst[0] = worldVerts[i].x;
            dst[1] = worldVerts[i].y;
            dst[2] = worldVerts[i].z;
            memcpy(dst + 3, src + 12, tailSize);

            float* normal = dst + 3;
            if (!rn->m_asCliff)
            {
                CVector const n = m_owner->getNormal(worldVerts[i].x, worldVerts[i].z);
                normal[0] = n.x;
                normal[1] = n.z;
                normal[2] = n.y;
            }
            else
            {
                if (rn->m_bInverted)
                {
                    normal[2] = -normal[2];
                }
                // NOTE: the shipped build shears the normal with a rotation
                // matrix it only ever fills in one element of, and the X term
                // additionally multiplies an uninitialised stack slot. Only the
                // deterministic part is reproduced here.
                normal[1] = normal[1] - sinAngle * normal[0];
            }
            vbCursor += stride;
        }
        M3D_RENDERER->UnlockVbPoolField(rn->m_VbPoolField);

        rn->m_geomObject = static_cast<GeomObjectRoad*>(M3D_KERNEL->New("GeomObjectRoad"));
        rn->m_geomObject->m_TriData = dGeomTriMeshDataCreate();
        rn->m_geomObject->m_Vertices = worldVerts;
        rn->m_geomObject->m_Indices = new int[3 * mesh->m_numFaces];
        rn->m_geomObject->SetRoadNode(rn);

        // An inverted node mirrors its geometry, so the winding has to flip too.
        unsigned short* ibCursor = static_cast<unsigned short*>(M3D_RENDERER->LockIbPoolField(rn->m_IbPoolField));
        for (int f = 0; f < mesh->m_numFaces; ++f)
        {
            unsigned short const* tri = mesh->m_tris + 3 * f;
            int const a = rn->m_bInverted ? tri[2] : tri[0];
            int const c = rn->m_bInverted ? tri[0] : tri[2];
            rn->m_geomObject->m_Indices[3 * f] = a;
            rn->m_geomObject->m_Indices[3 * f + 1] = tri[1];
            rn->m_geomObject->m_Indices[3 * f + 2] = c;
            ibCursor[3 * f] = static_cast<unsigned short>(a);
            ibCursor[3 * f + 1] = tri[1];
            ibCursor[3 * f + 2] = static_cast<unsigned short>(c);
        }
        M3D_RENDERER->UnlockIbPoolField(rn->m_IbPoolField);

        dGeomTriMeshDataBuildSingle(
            rn->m_geomObject->m_TriData,
            worldVerts,
            12,
            numVertices,
            rn->m_geomObject->m_Indices,
            3 * mesh->m_numFaces,
            12);
        dxGeom* const triMesh =
            dCreateTriMesh(m_owner->m_owner->GetOdeSpace(), rn->m_geomObject->m_TriData, nullptr, nullptr, nullptr);
        rn->m_geomObject->SetGeom(triMesh);
        rn->m_cachedVertices = worldVerts;

        // Bounding sphere over the border vertices only.
        Aabb bBox;
        bBox.m_box[0] = 10000.0f;
        bBox.m_box[1] = 10000.0f;
        bBox.m_box[2] = 10000.0f;
        bBox.m_box[3] = -10000.0f;
        bBox.m_box[4] = -10000.0f;
        bBox.m_box[5] = -10000.0f;
        for (auto const& list : roadSet->m_boundVerts[rn->m_type][rn->m_modelNum])
        {
            for (unsigned vi : list)
            {
                float const* v = &worldVerts[vi].x;
                for (int axis = 0; axis < 3; ++axis)
                {
                    bBox.m_box[axis] = std::min(bBox.m_box[axis], v[axis]);
                    bBox.m_box[axis + 3] = std::max(bBox.m_box[axis + 3], v[axis]);
                }
            }
        }

        float const sx = bBox.m_box[3] - bBox.m_box[0];
        float const sy = bBox.m_box[4] - bBox.m_box[1];
        float const sz = bBox.m_box[5] - bBox.m_box[2];
        rn->m_boundCenter.x = (bBox.m_box[3] + bBox.m_box[0]) * 0.5f;
        rn->m_boundCenter.y = (bBox.m_box[4] + bBox.m_box[1]) * 0.5f;
        rn->m_boundCenter.z = (bBox.m_box[5] + bBox.m_box[2]) * 0.5f;
        rn->m_boundRadius = sqrtf(sz * sz + sy * sy + sx * sx) * 0.5f;
    }
}  // namespace m3d

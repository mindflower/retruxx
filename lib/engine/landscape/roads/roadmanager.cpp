#define NOMINMAX
#include "roadmanager.h"
#include <algorithm>
#include <cassert>
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

    RoadInRadius3dTest::RoadInRadius3dTest(CVector const& origin, float r) : org(origin), radius(r)
    {
        // RVA 0x76BA50
    }

    bool RoadInRadius3dTest::TestRoadNode(RoadNode* rn) const
    {
        // RVA 0x7B39D0 - the bounding sphere of the road node overlaps the test sphere.
        float const dz = rn->m_boundCenter.z - org.z;
        float const dx = rn->m_boundCenter.x - org.x;
        float const dy = rn->m_boundCenter.y - org.y;
        float const r = rn->m_boundRadius + radius;
        return r * r > dz * dz + dx * dx + dy * dy;
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

    RoadInFrustumTest::RoadInFrustumTest(CClipper* fr)
    {
        // RVA 0x75ED30
        frustum = fr;
    }

    bool RoadInFrustumTest::TestRoadNode(RoadNode* rn) const
    {
        // RVA 0x7B3A40
        return frustum->testSphere(rn->m_boundCenter, rn->m_boundRadius) != 0;
    }

    int RoadManager::RenderRoads(
        retruxx::vector<unsigned>& visList,
        RenderRoadType rrt,
        RoadTestCallBack const* rnTest,
        bool bForRoadMap)
    {
        // RVA 0x7B7D80 - draws every road node visible this frame in the listed cells, each
        // once even when it covers several of them. visList packs a cell as x | (z << 16).
        if (!m_coveredCells || m_roadSets.empty())
        {
            return 0;
        }

        retruxx::vector<RoadNode*> roadsToDraw;
        roadsToDraw.reserve(400);
        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        int numRoadPolys = 0;

        CWorld* const world = m_owner->m_owner;
        rend::Colorf const ambient(world->GetWeatherAmbientColor());
        CVector const colorAmbient(ambient.r, ambient.g, ambient.b);
        rend::Colorf const diffuse(world->GetWeatherDiffuseColor());
        CVector const colorDiffuse(diffuse.r, diffuse.g, diffuse.b);

        float fogStart;
        float fogEnd;
        world->GetLandscape().GetFogStartAndEnd(fogStart, fogEnd);
        float const fogReduce = world->GetWeatherManager().GetFogReduceFactorFromWeather();
        CVector fogTerm;
        fogTerm.x = fogReduce * fogEnd;
        fogTerm.z = fogReduce * fogStart;
        fogTerm.y = 1.0f / (fogReduce * fogEnd - fogReduce * fogStart);

        float const VISCELL_EDGE_LENGTH_30 = 128.0;
        SceneGraph& graph = m3d::pClient->GetWorld().GetGraph();
        graph.LightSetupSunForWorld();

        int const landSize = world->m_level->land_size;
        for (unsigned const cell : visList)
        {
            auto const& cellRoads = m_coveredCells[(cell & 0xFFFF) + landSize * (cell >> 16)];
            for (RoadNode* const roadNode : cellRoads)
            {
                if (roadNode->m_bRoadDrawn || roadNode->m_frameVisible != curFrame ||
                    (rnTest && !rnTest->TestRoadNode(roadNode)))
                {
                    continue;
                }

                roadNode->m_bRoadDrawn = true;
                roadsToDraw.push_back(roadNode);
                AnimatedModel* const model =
                    m_roadSets[roadNode->m_roadSetHandle]->m_roadModels[roadNode->m_type][roadNode->m_modelNum];

                if (rrt == RRT_SIMPLE)
                {
                    M3D_RENDERER->SetFog(true, false);
                    M3D_RENDERER->SetCull(rend::M3DCULL_CCW, false);
                    M3D_RENDERER->SetFillMode(rend::M3DFILL_SOLID, false);
                    M3D_RENDERER->SetBlend(rend::BM_ALPHA, false);
                    M3D_RENDERER->SetAlphaTest(10);
                    M3D_RENDERER->PushZbState(rend::ZB_ENABLE);
                }
                if (bForRoadMap)
                {
                    M3D_RENDERER->SetFog(false, false);
                    M3D_RENDERER->SetBlend(rend::BM_NONE, false);
                    M3D_RENDERER->SetAlphaTest(0);
                }

                for (unsigned meshIdx = 0; meshIdx < model->GetNumMeshes(); ++meshIdx)
                {
                    auto& mesh = model->GetMesh(meshIdx);
                    M3D_RENDERER->SetPoolIndices(roadNode->m_IbPoolField, roadNode->m_VbPoolField.RealOffset);
                    M3D_RENDERER->SetPoolToStream0(roadNode->m_VbPoolField);

                    rend::IEffect* effect = nullptr;
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
                        {
                            // A skin the model lacks falls back to the first one.
                            unsigned const skin = roadNode->m_skinNumber < model->GetNumSkins() ? roadNode->m_skinNumber : 0;
                            effect = model->ApplyMaterial(model->GetSkin(skin)[mesh.m_MaterialNumber]);
                            break;
                        }
                    }

                    // NOTE: without a shader the original only asserts, and then draws with
                    // no effect and none of the parameters set.
                    if (!effect)
                    {
                        assert(!"by plus: drawing road w/o shader");
                    }
                    else
                    {
                        if (effect->IsParameterUsed(rend::IEffect::LightAmbient))
                        {
                            effect->SetVector3(rend::IEffect::LightAmbient, colorAmbient);
                        }
                        if (effect->IsParameterUsed(rend::IEffect::LightDiffuse))
                        {
                            effect->SetVector3(rend::IEffect::LightDiffuse, colorDiffuse);
                        }
                        if (effect->IsParameterUsed(rend::IEffect::FogTerm))
                        {
                            effect->SetVector3(rend::IEffect::FogTerm, fogTerm);
                        }
                        if (effect->IsParameterUsed(rend::IEffect::LightMap0))
                        {
                            // The lightmap is addressed as (x, -z) in world units scaled to the whole map.
                            float const scale = 1.0f / (static_cast<float>(landSize) * VISCELL_EDGE_LENGTH_30);
                            effect->SetVector3(rend::IEffect::User_float3_param, CVector(scale, 0.0f - scale, 0.0f));
                            rend::TexHandle lightmap = m_owner->GetLightmapTexture();
                            effect->SetTexture(rend::IEffect::LightMap0, &lightmap);
                        }
                    }

                    M3D_RENDERER->DrawIndexedPrimitiveEffect(
                        rend::M3DPT_TRIANGLELIST, effect, 0, mesh.m_numDrawVerts, roadNode->m_IbPoolField.RealOffset,
                        mesh.m_numFaces);
                    numRoadPolys += mesh.m_numFaces;
                }

                if (rrt == RRT_SIMPLE)
                {
                    M3D_RENDERER->PopZbState();
                }
            }
        }

        if (rrt == RRT_SIMPLE)
        {
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame((CStr("# road tris = ") + CStr(numRoadPolys)).c_str());
        }

        for (RoadNode* const roadNode : roadsToDraw)
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
        // RVA 0x7BA040
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

                    // The first piece of a type gives the bounds of all its pieces.
                    auto& firstMesh = model->GetMesh(0);
                    float* verts = static_cast<float*>(firstMesh.m_verts);
                    auto const vertexStride = firstMesh.m_VertexTypeSize / sizeof(float);
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
                M3D_LOG_INFO(CStr("RoadManager error( model not loaded ): ") + fileName);
                delete model;
            }
        }

        // Sorts the border vertices of every piece by side, so that LinkRoadNodes can stitch
        // the pieces together: [0] the far end (max z), [1] the near end (min z), [2] the
        // left side (min x) and [3] the right side (max x). The ends are ordered by x and
        // the sides by z. A vertex that duplicates one already on the side goes to the fake
        // list instead. The bounds used are those of the type's first piece.
        for (int type = 0; type < 4; ++type)
        {
            for (unsigned modelIdx = 0; modelIdx < m_roadModels[type].size(); ++modelIdx)
            {
                AnimatedModel* const model = m_roadModels[type][modelIdx];

                m_boundVerts[type].push_back({});
                auto& boundVerts = m_boundVerts[type].back();
                boundVerts.resize(4);
                m_fakeBoundVerts[type].push_back({});
                auto& fakeBoundVerts = m_fakeBoundVerts[type].back();
                fakeBoundVerts.resize(4);
                m_cliffBorders[type].push_back({});
                auto& cliffBorders = m_cliffBorders[type].back();
                cliffBorders.resize(1);

                auto& mesh = model->GetMesh(0);
                auto const vertexAt = [&mesh](unsigned idx)
                {
                    return reinterpret_cast<float const*>(static_cast<char const*>(mesh.m_verts) + idx * mesh.m_VertexTypeSize);
                };

                // Walks the side up to the first vertex past the new one along sortAxis,
                // which is where it is inserted unless one of the vertices passed, or the
                // one stopped at, lies within tolerance of it.
                auto const addToSide = [&](int side, int sortAxis, double tolerance, unsigned idx, float const* v)
                {
                    auto& verts = boundVerts[side];
                    bool isNew = true;
                    auto it = verts.begin();
                    for (; it != verts.end(); ++it)
                    {
                        float const* other = vertexAt(*it);
                        double const dx = static_cast<double>(other[0]) - v[0];
                        double const dy = static_cast<double>(other[1]) - v[1];
                        double const dz = static_cast<double>(other[2]) - v[2];
                        if (fabs(sqrt(dz * dz + dy * dy + dx * dx)) < tolerance)
                        {
                            isNew = false;
                        }
                        if (other[sortAxis] > v[sortAxis])
                        {
                            break;
                        }
                    }

                    if (isNew)
                    {
                        verts.insert(it, idx);
                    }
                    else
                    {
                        fakeBoundVerts[side].push_back(idx);
                    }
                };

                for (int idx = 0; idx < mesh.m_numVertices; ++idx)
                {
                    float const* const v = vertexAt(idx);
                    if (fabs(v[2] - m_minZ[type]) < 0.1)
                    {
                        addToSide(1, 0, 0.1, idx, v);
                    }
                    else if (fabs(v[2] - m_maxZ[type]) < 0.1)
                    {
                        // NOTE: the far end matches duplicates within 0.01, the other sides within 0.1.
                        addToSide(0, 0, 0.0099999998, idx, v);
                    }
                    else if (fabs(v[0] - m_minX[type]) < 0.1)
                    {
                        addToSide(2, 2, 0.1, idx, v);
                        continue;
                    }
                    else if (fabs(v[0] - m_maxX[type]) < 0.1)
                    {
                        addToSide(3, 2, 0.1, idx, v);
                        cliffBorders[0].push_back(idx);
                        continue;
                    }
                    else
                    {
                        continue;
                    }

                    // NOTE: for both ends the test has no fabs, so every end vertex left of
                    // the right side also counts as a cliff border.
                    if (v[0] - m_maxX[type] < 0.1f)
                    {
                        cliffBorders[0].push_back(idx);
                    }
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
        // RVA 0x7B8550 - the errors are logged as info.
        M3D_ASSERT(m_roadRoot);

        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(name, fs::IStream::OPEN_READ))
        {
            M3D_LOG_INFO(CStr("Error: RoadManager can't read the file: ") + CStr(name));
            return 0;
        }

        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        if (!xmlFile->Read(*stream))
        {
            M3D_LOG_INFO(
                CStr("Error: RoadManager: error in parsing: ") + CStr(name) + CStr(" (") + CStr(xmlFile->GetError()) +
                CStr(") "));
            return 0;
        }

        ref_ptr roadsNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(roadsNode, "Roads");
        if (roadsNode->IsEmpty())
        {
            M3D_LOG_INFO(CStr("Error: RoadManager can't find Root node in ") + CStr(name));
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
                // The original destroys it through the virtual destructor, as delete does.
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
        // RVA 0x7B6570 - resolves the linked node names and derives each node's piece from
        // the number of links: an end, a straight piece, a T junction or a crossroad.
        FindFriends();

        for (auto* node = static_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node;
             node = static_cast<RoadNode*>(node->GetNextSibling()))
        {
            node->m_owner = this;

            int numLinks = 0;
            for (int link = 0; link < 4; ++link)
            {
                CStr const& linkedName = node->m_linkedNames[link];
                if (linkedName.empty())
                {
                    node->m_linkedNodes[link] = nullptr;
                    continue;
                }

                RoadNode* found = nullptr;
                for (auto* other = static_cast<RoadNode*>(m_roadRoot->GetFirstChild()); other;
                     other = static_cast<RoadNode*>(other->GetNextSibling()))
                {
                    if (other->GetName() == linkedName)
                    {
                        found = other;
                        break;
                    }
                }
                node->m_linkedNodes[link] = found;
                if (found)
                {
                    ++numLinks;
                }
            }

            // A next node that leads nowhere is the end of the road and does not count.
            RoadNode* next = node->m_linkedNodes[0];
            if (next && next->m_linkedNames[0].empty())
            {
                --numLinks;
            }

            node->m_bInverted = false;
            switch (numLinks)
            {
                case 0:
                    node->m_type = 3;
                    break;
                case 1:
                    // An end piece faces the way of its only link.
                    node->m_type = 3;
                    if (next && node->m_linkedNodes[1])
                    {
                        node->m_bInverted = true;
                    }
                    break;
                case 2:
                    node->m_type = 0;
                    break;
                case 3:
                {
                    // A T junction is mirrored when its stem, taken from the side links, turns
                    // the other way. Without a previous node the node itself stands in for the next.
                    node->m_type = 1;
                    if (!node->m_linkedNodes[1])
                    {
                        next = node;
                    }
                    RoadNode const* left = node->m_linkedNodes[2];
                    RoadNode const* right = node->m_linkedNodes[3];
                    if (left && right && next &&
                        (right->m_origin.z - left->m_origin.z) * (next->m_origin.x - left->m_origin.x) -
                                (next->m_origin.z - left->m_origin.z) * (right->m_origin.x - left->m_origin.x) <
                            0.0f)
                    {
                        node->m_bInverted = true;
                    }
                    break;
                }
                case 4:
                    node->m_type = 2;
                    break;
                default:
                    M3D_LOG_INFO(CStr("RoadNode not connected: ") + CStr(node->GetName()));
                    node->m_type = 3;
                    break;
            }

            // NOTE: nothing guards against a road set without pieces of this type: the
            // clamp wraps around and the lookup reads past the end. And the skin number is
            // clamped to the number of skins, one past the last; RenderRoads then falls
            // back to the first skin.
            RoadSet* const roadSet = m_roadSets[node->m_roadSetHandle];
            auto const& models = roadSet->m_roadModels[node->m_type];
            unsigned const lastModel = static_cast<unsigned>(models.size()) - 1;
            if (node->m_modelNum > lastModel)
            {
                node->m_modelNum = lastModel;
            }
            unsigned const numSkins = models[node->m_modelNum]->GetNumSkins();
            if (node->m_skinNumber > numSkins)
            {
                node->m_skinNumber = numSkins;
            }

            // Nodes paired with a friend, or leading into one, are straight pieces.
            if (node->m_friend)
            {
                node->m_type = 0;
                node->m_bInverted = false;
            }
            if (node->m_linkedNodes[0] && node->m_linkedNodes[0]->m_friend)
            {
                node->m_type = 0;
                node->m_bInverted = false;
            }

            if (static_cast<unsigned>(node->m_roadSetHandle) < m_roadSets.size())
            {
                node->m_minX = m_roadSets[node->m_roadSetHandle]->m_minX[node->m_type];
                node->m_maxX = m_roadSets[node->m_roadSetHandle]->m_maxX[node->m_type];
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

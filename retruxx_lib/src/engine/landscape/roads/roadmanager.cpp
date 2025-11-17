#define NOMINMAX
#include "roadmanager.h"
#include <stdexcept>
#include <core/kernel.h>
#include <config.h>
#include <landscape.h>
#include <core/timer.h>
#include <world.h>
#include <level.h>

#include "geomobject.h"
#include "road.h"
#include "skelmodel.h"
#include "core/ini.h"
#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"

namespace m3d
{
    CStr const RoadManager::GetRoadSetNameByHandle(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::UpdateVis()
    {
        // TODO: implement RoadManager::UpdateVis
        return;
        // TODO: check and refactor this
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

    void RoadManager::ReleaseCollisionForRoadNode(RoadNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::RebuildStructures()
    {
        LinkRoadNodes();

        for (auto* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node; node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            if (node->m_type == 2 || node->m_type == 1)
            {
                CalcNodeData(node);
            }
        }

        for (auto* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node; node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            if (node->m_type == 3)
            {
                CalcNodeData(node);
            }
        }

        for (auto* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node; node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            if (node->m_type == 0)
            {
                CalcNodeData(node);
            }
        }

        RecalcCoveredCells();
    }

    int RoadManager::GetRoadSetHandleByName(const CStr& name)
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

    void RoadManager::RebuildSomeNodes(retruxx::set<RoadNode*>, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::SetOwner(Landscape* landscape)
    {
        m_owner = landscape;
    }

    int RoadManager::RenderRoads(retruxx::vector<unsigned>&, RenderRoadType, RoadTestCallBack const*, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::GetRoadMinMaxZByHandle(int, int, float&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
            const auto loadRes = M3D_ENGINE_CFG.m_loadFromGAM.GetB() ? model->LoadGAM(fileName, false) : model->LoadSAM(fileName, false);
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
                    const auto vertexStride = firstMesh.m_VertexTypeSize / sizeof(float);

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
        const float BOUNDARY_TOLERANCE = 0.1f;
        const float VERTEX_MATCH_TOLERANCE = 0.01f;

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
                const auto vertexStride = mesh.m_VertexTypeSize / sizeof(float);

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
                            float* existingVertex = reinterpret_cast<float*>(static_cast<char*>(mesh.m_verts) + existingIndex * mesh.m_VertexTypeSize);

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
                                float* existingVertex = reinterpret_cast<float*>(static_cast<char*>(mesh.m_verts) + (*it) * mesh.m_VertexTypeSize);

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
                            float* existingVertex = reinterpret_cast<float*>(static_cast<char*>(mesh.m_verts) + existingIndex * mesh.m_VertexTypeSize);

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
                                float* existingVertex = reinterpret_cast<float*>(static_cast<char*>(mesh.m_verts) + (*it) * mesh.m_VertexTypeSize);

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
                            float* existingVertex = reinterpret_cast<float*>(static_cast<char*>(mesh.m_verts) + existingIndex * mesh.m_VertexTypeSize);

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
                                float* existingVertex = reinterpret_cast<float*>(static_cast<char*>(mesh.m_verts) + (*it) * mesh.m_VertexTypeSize);

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
                            float* existingVertex = reinterpret_cast<float*>(static_cast<char*>(mesh.m_verts) + existingIndex * mesh.m_VertexTypeSize);

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
                                float* existingVertex = reinterpret_cast<float*>(static_cast<char*>(mesh.m_verts) + (*it) * mesh.m_VertexTypeSize);

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    RoadManager::RoadManager()
    {
        this->m_owner = 0;
        this->m_coveredCells = 0;
        this->m_roadRoot = 0;
    }

    int RoadManager::WriteRoadsToXmlFile(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::GetRoadMinMaxXByHandle(int, int, float&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::UnlinkRoadNodeCollisionFromCells(RoadNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        for (roadsNode->GetFirstChild(roadNode, "RoadNode"); !roadNode->IsEmpty(); roadNode->GetNextSibling(roadNode, "RoadNode"))
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

        delete m_coveredCells;
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
        for (auto* child = dynamic_cast<m3d::RoadNode*>(m_roadRoot->GetFirstChild()); child; child = dynamic_cast<m3d::RoadNode*>(child->GetNextSibling()))
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
        // TODO: implement RoadManager::RecalcCoveredCells
        // RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::LinkToBorder(RoadNode*, unsigned, int, CVector&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::LinkRoadNodes()
    {
        // TODO: generated code
        // Find friend nodes first
        FindFriends();

        // Iterate through all road nodes
        for (RoadNode* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node != nullptr; node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            node->m_owner = this;
            int linkCounter = 0;

            // Process all 4 possible linked nodes
            for (int linkIndex = 0; linkIndex < 4; ++linkIndex)
            {
                const CStr& linkedName = node->m_linkedNames[linkIndex];
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

    CVector2 RoadManager::FindLeftProjection(RoadNode*, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RoadManager::GetAdjPoint(RoadNode*, unsigned, int, CVector&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RoadManager::FindFriends()
    {
        for (RoadNode* node = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node != nullptr; node = dynamic_cast<RoadNode*>(node->GetNextSibling()))
        {
            node->m_friend = nullptr;
        }

        for (RoadNode* node1 = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node1 != nullptr; node1 = dynamic_cast<RoadNode*>(node1->GetNextSibling()))
        {
            for (RoadNode* node2 = dynamic_cast<RoadNode*>(m_roadRoot->GetFirstChild()); node2 != nullptr;
                 node2 = dynamic_cast<RoadNode*>(node2->GetNextSibling()))
            {
                if (node1 != node2)
                {
                    const auto x = node2->m_origin.x - node1->m_origin.x;
                    const auto y = node2->m_origin.y - node1->m_origin.y;
                    const auto z = node2->m_origin.z - node1->m_origin.z;
                    if ((((x * x) + (y * y)) + (z * z)) < 1.0)
                    {
                        node2->m_friend = node1;
                        node1->m_friend = node2;
                    }
                }
            }
        }
    }

    void RoadManager::CalcNodeData(RoadNode*)
    {
        // TODO: implement RoadManager::CalcNodeData
        // RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace m3d

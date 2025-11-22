#include "config.h"
#include "world.h"

#include <m3dapp.h>
#include <particles.h>
#include <stdexcept>
#include <scene/servers/serverparticles.h>
#include <poolmanager.h>
#include <scene/nodes/sgnodegameunit.h>
#include <core/scoped_ptr.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <client.h>
#include <algorithm>

bool loadedViaBPS = false;

namespace
{
    constexpr int MAX_NODES_PER_CLASS = 0x7D0;

    
    struct ParticlesInfo
    {
        /* 0x0000 */ m3d::SgNode* pNode = nullptr;
        /* 0x0004 */ m3d::ParticlesList* pList = nullptr;
        /* 0x0008 */ m3d::ParticleSystem* pSystem = nullptr;
    }; /* size: 0x000c */

    struct FxParamsSet
    {
        /* 0x0000 */ CVector cAmbient = ZeroVector;
        /* 0x000c */ CVector cDiffuse = ZeroVector;
        /* 0x0018 */ CVector fogTerm = ZeroVector;
        /* 0x0024 */ bool ambientNotSet = true;
        /* 0x0025 */ bool diffuseNotSet = true;
        /* 0x0026 */ bool fogNotSet = true;
        void Clear();
    }; /* size: 0x0028 */

    struct ParticlesSortPred
    {
        ParticlesSortPred(ParticlesInfo* parts) : m_particles(parts){}

        bool operator()(unsigned int partIdx1, unsigned int partIdx2) const
        {
            // TODO: check and refactor this
            auto pSystem = this->m_particles[partIdx1].pSystem;
            auto v4 = this->m_particles[partIdx2].pSystem;
            auto m_shader = pSystem->m_shader;
            auto v6 = v4->m_shader;
            if (m_shader < v6)
                return true;
            if (m_shader == v6)
                return &pSystem->m_texAdd < &v4->m_texAdd;
            return false;
        }
        /* 0x0000 */ ParticlesInfo* m_particles;
    }; /* size: 0x0004 */
}

namespace m3d
{
    struct PropInternalGetMeshPoints
    {
        /* 0x0000 */ int m_numMesh;
        /* 0x0004 */ m3d::SgNode* m_node;
        /* 0x0008 */ void** m_verts;
        retruxx::vector<m3d::rend::VertexType> m_VertexTypes;
        retruxx::vector<unsigned int> m_VertexTypeSizes;
        /* 0x002c */ int* m_numVerts;
        /* 0x0030 */ unsigned short** m_indxs;
        /* 0x0034 */ int* m_numIndxs;
        /* 0x0038 */ bool* m_strips;
        /* 0x003c */ CMatrix** m_localmatr;
        /* 0x0040 */ int m_numSkinMesh;
    }; /* size: 0x0044 */

    int ParticlesServer::SetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ParticlesServer::~ParticlesServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticlesServer::RenderNodeSet(SgNode** nodes, unsigned numNodes, m3d::RenderNodeInfo rni)
    {
        m_profiler->StartCountdown();
        assert(numNodes < MAX_NODES_PER_CLASS);
        if (!numNodes || rni.rnt)
        {
            m_profiler->EndCountdown();
            return 0;
        }

        M3D_RENDERER->PushCull(rend::M3DCULL_CW);
        M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
        M3D_RENDERER->PushBlend(rend::BM_NONE);
        M3D_RENDERER->PushFog(false);
        M3D_RENDERER->PushLighting(false);
        M3D_RENDERER->SetAlphaTest (0);
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(1, rend::BM_COLOR, rend::TS_NONE);
        M3D_RENDERER->SetStageState(1, rend::BM_ALPHA, rend::TS_NONE);
        M3D_RENDERER->TgDisable(0);
        M3D_RENDERER->TgDisable(1);
        M3D_RENDERER->TgDisable(2);
        M3D_RENDERER->TgDisable(3);
        M3D_RENDERER->TgDisable(4);
        M3D_RENDERER->TgDisable(5);
        M3D_RENDERER->TgDisable(6);
        M3D_RENDERER->TgDisable(7);

        if (M3D_ENGINE_CFG.m_lsWireframe.GetB())
        {
            M3D_RENDERER->SetFillMode(rend::M3DFILL_WIREFRAME, false);
        }

        
        // TODO: check this!!!!
        pClient->GetWorld().GetGraph().LightSetupSunForWorld();
        rend::Colorf ambientColor = pClient->GetWorld().GetWeatherAmbientColor();
        rend::Colorf diffuseColor = pClient->GetWorld().GetWeatherDiffuseColor();

        float fogStart = 0.0;
        float fogEnd = 0.0;
        pClient->GetWorld().GetLandscape().GetFogStartAndEnd(fogStart, fogEnd);

        // Initialize effect parameters
        FxParamsSet fxParams;

        // Get ambient color and convert from 0-255 to 0.0-1.0
        fxParams.cAmbient.x = ambientColor.r;
        fxParams.cAmbient.y = ambientColor.g;
        fxParams.cAmbient.z = ambientColor.b;

        // Get diffuse color
        fxParams.cDiffuse.x = diffuseColor.r;
        fxParams.cDiffuse.y = diffuseColor.g;
        fxParams.cDiffuse.z = diffuseColor.b;


        float fogReduceFactor = pClient->GetWorld().GetWeatherManager().GetFogReduceFactorFromWeather();
        fxParams.fogTerm.z = fogReduceFactor * fogStart;                        // fogStart
        fxParams.fogTerm.x = fogReduceFactor * fogEnd;                          // fogEnd
        fxParams.fogTerm.y = 1.0f / (fxParams.fogTerm.x - fxParams.fogTerm.z);  // fogInvRange

        // Prepare particle data arrays
        unsigned int nodeIndices[MAX_NODES_PER_CLASS] = {};
        ParticlesInfo nodeData[MAX_NODES_PER_CLASS] = {};

        // Extract particle data from each node
        for (unsigned int i = 0; i < numNodes; i++)
        {
            SgNode* node = nodes[i];

            // Get particle system info from node property (property ID 1)
            PsInfoForNode* propertyData = nullptr;
            node->GetProperty(1, &propertyData);

            // Get particle system instance from models array
            auto* particleSystem = (ParticleSystem*)m_models[node->GetServerHandle()].m_ptr;

            nodeIndices[i] = i;
            nodeData[i].pNode = node;
            nodeData[i].pList = propertyData->m_list;
            nodeData[i].pSystem = particleSystem;
        }

        // Sort particles for proper rendering order (back-to-front for alpha blending)
        if (numNodes > 0)
        {
            std::stable_sort(nodeIndices, nodeIndices + numNodes, ParticlesSortPred(nodeData));
        }

        // Render sorted particle systems
        for (unsigned int i = 0; i < numNodes; i++)
        {
            unsigned int sortedIndex = nodeIndices[i];
            ParticlesInfo& data = nodeData[sortedIndex];
            ParticleSystem* particleSystem = data.pSystem;

            // Apply shader parameters if particle system has a shader
            if (particleSystem && particleSystem->m_shader)
            {
                auto* shader = particleSystem->m_shader;

                // Apply ambient color to first particle system that supports it
                if (fxParams.ambientNotSet && shader->IsParameterUsed(rend::IEffect::LightAmbient))
                {
                    shader->SetVector3(rend::IEffect::LightAmbient, fxParams.cAmbient);
                    fxParams.ambientNotSet = false;
                }

                // Apply diffuse color to first particle system that supports it
                if (fxParams.diffuseNotSet && shader->IsParameterUsed(rend::IEffect::LightDiffuse))
                {
                    shader->SetVector3(rend::IEffect::LightDiffuse, fxParams.cDiffuse);
                    fxParams.diffuseNotSet = false;
                }

                // Apply fog parameters to first particle system that supports it
                if (fxParams.fogNotSet && shader->IsParameterUsed(rend::IEffect::FogTerm))
                {
                    shader->SetVector3(rend::IEffect::FogTerm, fxParams.fogTerm);
                    fxParams.fogNotSet = false;
                }
            }

            // Render the particle system
            // Parameters: transform matrix (from particle system colors array) and particles list
            particleSystem->Render(&data.pNode->GetCurrentMatrix(), data.pList);
        }

        M3D_RENDERER->PopCull();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopFog();
        M3D_RENDERER->PopLighting();
        M3D_RENDERER->SetAlphaTest(0);

        m_profiler->EndCountdown();
        return 1;
    }

    void ParticlesServer::UpdateItem(int id, void* params)
    {
        // TODO: generated code ParticlesServer::UpdateItem

        struct RenderInfo
        {
            m3d::SgNode* m_node;
            unsigned int m_dt;
        };

        m_profiler->StartCountdown();

        RenderInfo* ri = (RenderInfo*)params;
        SgNode* node = ri->m_node;

        // Get particle system info from node property
        PsInfoForNode* particleInfo = nullptr;
        node->GetProperty(1, &particleInfo);

        // Convert delta time from milliseconds to seconds
        float deltaTime = (float)ri->m_dt * 0.001f;

        // Get particle system instance
        ParticleSystem* particleSystem = (ParticleSystem*)m_models[id].m_ptr;
        ParticlesList* particlesList = particleInfo->m_list;

        // Calculate world velocity if this is not the first update
        if (particlesList->m_updateCalled)
        {
            // Extract position from current transform matrices
            const auto currentPos = ri->m_node->GetCurrentMatrix().getOrg();
            const auto prevPos = particlesList->m_curXFormToWorld.getOrg();

            // Calculate velocity as (current_pos - previous_pos) / delta_time
            particlesList->m_worldVel.x = (currentPos.x - prevPos.x) / deltaTime;
            particlesList->m_worldVel.y = (currentPos.y - prevPos.y) / deltaTime;
            particlesList->m_worldVel.z = (currentPos.z - prevPos.z) / deltaTime;
        }
        else
        {
            // First update - zero out velocity
            particlesList->m_worldVel = ZeroVector;
        }

        // Store current transform for next frame's velocity calculation
        memcpy(&particlesList->m_curXFormToWorld, &ri->m_node->GetCurrentMatrix(), sizeof(particlesList->m_curXFormToWorld));

        // Update the particle system
        particleSystem->Update(particlesList, deltaTime, 1.0f);

        m_profiler->EndCountdown();
    }

    int ParticlesServer::GetItemProperty(int id, int prop, void* dest)
    {
        if (prop == 12288)
        {
            m3d::PropSrvBoundingBox* destConverted = (m3d::PropSrvBoundingBox*)dest;
            auto srvId = destConverted->m_node->GetServerHandle();
            if (srvId >= 0 && srvId < m_models.size())
            {
                ParticleSystem* system = (ParticleSystem*)m_models[srvId].m_ptr;
                *destConverted->m_destBox = system->m_bBox;
                return 1;
            }
            else
            {
                (destConverted->m_destBox->m_box)[0] = 0.0;
                (destConverted->m_destBox->m_box)[1] = 0.0;
                (destConverted->m_destBox->m_box)[2] = 0.0;
                (destConverted->m_destBox->m_box)[3] = 0.0;
                (destConverted->m_destBox->m_box)[4] = 0.0;
                (destConverted->m_destBox->m_box)[5] = 0.0;
                return 1;
            }
        }
        else
        {
            if (prop == 12293)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }
            return m3d::DataServer::GetItemProperty(id, prop, dest);
        }
    }

    ParticlesServer::ParticlesServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("particles", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
        id = Application::g_pApp->GetProfilerStack().AddProfiler("particles update", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profilerUpdate = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
        QuadPS::CreateIb();
        SpritePS::CreateIb();
        GlowQuadPS::CreateIb();
        PolyPS::CreateIb();
        Poly1PS::CreateIb();
        RainPS::CreateIb();
        StripAllPS::CreateIb();
        StripOnePS::CreateIb();
    }

    void ParticlesServer::MoveParticles(m3d::SgNode*, retruxx::vector<CVector> const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticlesServer::Init()
    {
        m_valid = true;
        return true;
    }

    int ParticlesServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticlesServer::RenderItem(int, void*)
    {
    }

    void ParticlesServer::SaveAllLoadedEntitiesToBPS()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticlesServer::RemoveItem(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticlesServer::ResetItem(m3d::SgNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticlesServer::UnregisterNode(m3d::SgNode* node)
    {
        PsInfoForNode* info = nullptr;
        node->GetProperty(1u, &info);
        if (info->m_list->m_meshEmitterVerts)
        {
            m3d::PropInternalGetMeshPoints prop;
            prop.m_node = info->m_nodeForMesh;
            prop.m_numMesh = info->m_numMesh;
            prop.m_verts = info->m_list->m_meshEmitterVerts;
            prop.m_numVerts = info->m_list->m_numMeshEmitterVerts;
            prop.m_indxs = (unsigned short**)info->m_list->m_meshEmitterInds;
            prop.m_numIndxs = info->m_list->m_numMeshEmitterInds;
            prop.m_localmatr = info->m_list->m_local;
            if (info->m_serverForMesh)
            {
                info->m_serverForMesh->SetItemProperty(info->m_itemForMesh, 16392, &prop);
            }
        }

        PL_PoolManager.Delete(info->m_list);
        Info_PoolManager.Delete(info);
        node->SetProperty(1u, &info);
    }

    void ParticlesServer::AddParticle(m3d::SgNode* node, CVector const* pos)
    {
        auto* system = static_cast<ParticleSystem*>(m_models[node->GetServerHandle()].m_ptr);

        PsInfoForNode* info = nullptr;
        node->GetProperty(PROP_SERVER_SLOT, &info);
        if (info)
        {
            if (!info->m_list->m_updateCalled)
            {
                info->m_list->m_curXFormToWorld = node->GetCurrentMatrix();
                info->m_list->m_worldVel = ZeroVector;
                system->Update(info->m_list, 0.001, 1.0);
            }
            system->AddParticle(info->m_list, pos);
        }
    }

    int ParticlesServer::AddItem(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticlesServer::SaveAllLoadedEntities(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticlesServer::RegisterNode(m3d::SgNode* node)
    {
        // TODO: check this!!
        auto* info = Info_PoolManager.New();
        auto* particlesList = PL_PoolManager.New();
        info->m_list = particlesList;
        memcpy(&info->m_list->m_curXFormToWorld, &node->GetCurrentMatrix(), sizeof(info->m_list->m_curXFormToWorld));
        info->m_serverForMesh = 0;
        info->m_nodeForMesh = 0;
        info->m_itemForMesh = -1;
        info->m_numMesh = -1;
        node->SetProperty(1u, &info);
        info->m_list->m_TLM.SetTransparentBody(node);

        int numMesh = -1;
        node->GetProperty(PROP_PS_NUM_EMITTER_MESH, &numMesh);
        if (numMesh >= 0)
        {
            SgNode* m_parent = dynamic_cast<SgNode*>(node->GetParent());
            if (m_parent)
            {
                while (m_parent->GetClass() != &m3d::SgGameUnitNode::m_classSgGameUnitNode)
                {
                    m_parent = dynamic_cast<SgNode*>(m_parent->GetParent());
                    if (!m_parent)
                        return;
                }
                if (m_parent->GetClass() == &m3d::SgGameUnitNode::m_classSgGameUnitNode)
                {
                    info->m_nodeForMesh = m_parent;
                    info->m_serverForMesh = m_parent->GetServer();
                    info->m_numMesh = numMesh;
                    info->m_itemForMesh = -1;
                    m_parent->GetProperty(4360u, &info->m_itemForMesh);
                    if (info->m_itemForMesh != -1)
                    {
                        m3d::PropInternalGetMeshPoints prop;
                        prop.m_node = m_parent;
                        prop.m_numMesh = numMesh;
                        prop.m_verts = 0;
                        prop.m_strips = 0;
                        info->m_serverForMesh->GetItemProperty(info->m_itemForMesh, 16391, &prop);

                        if (prop.m_strips)
                            delete[] prop.m_strips;
                        prop.m_strips = 0;

                        info->m_list->SetMeshEmitterPoints(
                            prop.m_localmatr,
                            prop.m_numMesh,
                            prop.m_verts,
                            prop.m_numVerts,
                            prop.m_VertexTypes,
                            prop.m_VertexTypeSizes,
                            prop.m_numSkinMesh);

                        auto m_list = info->m_list;
                        auto m_indxs = prop.m_indxs;
                        m_list->m_numMeshEmitterInds = prop.m_numIndxs;
                        m_list->m_meshEmitterInds = (int**)m_indxs;
                    }
                    info->m_serverForMesh = m_parent->GetServer();
                }
            }
        }
    }

    void ParticlesServer::AddParticles(m3d::SgNode* node, retruxx::vector<CVector> const* newPoses)
    {
        for (auto& pos : *newPoses)
        {
            AddParticle(node, &pos);
        }
    }

    void ParticlesServer::AddItemsByOne(retruxx::vector<m3d::DataServer::ServerItem>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticlesServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>& itemslist)
    {
        if (!loadedViaBPS)
        {
            scoped_ptr fileStream = M3D_KERNEL->GetFileServer().CreateFileStream();
            if (fileStream->Open("data\\models\\effects.bps", fs::IStream::OPEN_READ))
            {
                unsigned version = 0;
                fileStream->ReadBytes(&version, 4u);
                if (version != 2)
                {
                    AddItemsByOne(itemslist);
                    fileStream->Close();
                    return;
                }

                retruxx::vector<AttrProps> m_Attractors;
                char buffer[52];
                PSProps psProps;
                unsigned psNum = 0;
                fileStream->ReadBytes(&psNum, 4u);
                for (int i = 0; i < psNum; ++i)
                {
                    fileStream->ReadBytes(buffer, 50);
                    buffer[50] = 0;
                    fileStream->ReadBytes(&psProps, sizeof(PSProps));

                    unsigned attrNum = 0;
                    fileStream->ReadBytes(&attrNum, 4u);
                    
                    AttrProps attrProps;
                    m_Attractors.resize(attrNum, attrProps);

                    for (int j = 0; j < attrNum; ++j)
                    {
                        fileStream->ReadBytes(&m_Attractors[j], sizeof(AttrProps));
                    }

                    auto* system = m3d::ParticleSystem::Factory(psProps, m_Attractors);
                    m3d::DataServer::Model model(system, buffer, buffer, buffer);
                    m_models.push_back(std::move(model));
                }
                fileStream->Close();
                loadedViaBPS = true;
            }
            else
            {
                AddItemsByOne(itemslist);
            }
        }
    }
}

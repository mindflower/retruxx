#include <m3dapp.h>
#include <particles.h>
#include <stdexcept>
#include <scene/servers/serverparticles.h>
#include <poolmanager.h>
#include <scene/nodes/sgnodegameunit.h>

struct PsInfoForNode
{
    /* 0x0000 */ m3d::ParticlesList* m_list = nullptr;
    /* 0x0004 */ m3d::SgNode* m_nodeForMesh = nullptr;
    /* 0x0008 */ m3d::DataServer* m_serverForMesh = nullptr;
    /* 0x000c */ int m_itemForMesh = 0;
    /* 0x0010 */ int m_numMesh = 0;
}; /* size: 0x0014 */

m3d::PoolManager<PsInfoForNode> Info_PoolManager(0);
m3d::PoolManager<m3d::ParticlesList> PL_PoolManager(0);

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
        throw retruxx::logic_error("Not implemented");
    }

    ParticlesServer::~ParticlesServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::RenderNodeSet(SgNode**, unsigned, m3d::RenderNodeInfo)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::UpdateItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::Init()
    {
        m_valid = true;
        return true;
    }

    int ParticlesServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::RenderItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::SaveAllLoadedEntitiesToBPS()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::ResetItem(m3d::SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::UnregisterNode(m3d::SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::AddParticle(m3d::SgNode*, CVector const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
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
        node->GetProperty(9472, &numMesh);
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

    void ParticlesServer::AddParticles(m3d::SgNode*, retruxx::vector<CVector> const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::AddItemsByOne(retruxx::vector<m3d::DataServer::ServerItem>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>&)
    {
        throw retruxx::logic_error("Not implemented");
    }
}

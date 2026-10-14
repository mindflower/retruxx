#pragma once
#include "DataServer.h"

#include "poolmanager.h"
#include "particles.h"

struct PsInfoForNode
{
    m3d::ParticlesList* m_list = nullptr;
    m3d::SgNode* m_nodeForMesh = nullptr;
    m3d::DataServer* m_serverForMesh = nullptr;
    int m_itemForMesh = 0;
    int m_numMesh = 0;
};

struct ParticleBases
{
    m3d::ParticleBase AllPB[15];
};

namespace m3d
{
    class Profiler;

    class ParticlesServer : public m3d::DataServer
    {
    public:
        ParticlesServer();
        virtual  ~ParticlesServer() override;
        virtual int Init() override;
        virtual int Release() override;
        virtual int AddItem(const char* params, const char* id) override;
        virtual int RemoveItem(int id) override;
        virtual int GetItemProperty(int id, int prop, void* dest) override;
        virtual int SetItemProperty(int id, int prop, void* src) override;
        virtual void UpdateItem(int id, void* params) override;
        virtual void RegisterNode(m3d::SgNode* node) override;
        virtual void UnregisterNode(m3d::SgNode* node) override;
        virtual int SaveAllLoadedEntities(const char* filename) override;
        void SaveAllLoadedEntitiesToBPS();
        void ResetItem(m3d::SgNode* node);
        virtual void RenderItem(int id, void* params) override;
        virtual int RenderNodeSet(m3d::SgNode** nodes, unsigned int numNodes, m3d::RenderNodeInfo rni) override;

        using CVectorVector = retruxx::vector<CVector, retruxx::allocator<CVector> >;

    public:
        void AddParticle(m3d::SgNode* node, const CVector* pos);
        void AddParticles(m3d::SgNode* node, const retruxx::vector<CVector, retruxx::allocator<CVector> >* newPoses);
        void MoveParticles(m3d::SgNode* node, const retruxx::vector<CVector, retruxx::allocator<CVector> >* newPoses);

    protected:
        void AddItemsByOne(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist);
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override;

    private:
        m3d::Profiler* m_profiler = nullptr;
        m3d::Profiler* m_profilerUpdate = nullptr;
    };

    extern PoolManager<PsInfoForNode> Info_PoolManager;
    extern PoolManager<m3d::ParticlesList> PL_PoolManager;
    extern PoolManager<m3d::Particle> ParticlesPool;
    extern PoolManager<ParticleBases> TrailsPool;
}

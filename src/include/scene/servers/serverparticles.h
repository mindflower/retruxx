#pragma once
#include "DataServer.h"

namespace m3d
{
    class Profiler;

    class ParticlesServer : public DataServer
    {
    public:
        virtual int SetItemProperty(int, int, void*);
        virtual ~ParticlesServer();
        virtual int RenderNodeSet(SgNode**, unsigned int, m3d::RenderNodeInfo);
        virtual void UpdateItem(int, void*);
        virtual int GetItemProperty(int, int, void*);
        ParticlesServer();
        void MoveParticles(m3d::SgNode*, std::vector<CVector> const*);
        virtual int Init();
        virtual int Release();
        virtual void RenderItem(int, void*);
        void SaveAllLoadedEntitiesToBPS();
        virtual int RemoveItem(int);
        void ResetItem(m3d::SgNode*);
        virtual void UnregisterNode(m3d::SgNode*);
        void AddParticle(m3d::SgNode*, CVector const*);
        virtual int AddItem(char const*, char const*);
        virtual int SaveAllLoadedEntities(char const*);
        virtual void RegisterNode(m3d::SgNode*);
        void AddParticles(m3d::SgNode*, std::vector<CVector> const*);

    protected:
        void AddItemsByOne(std::vector<m3d::DataServer::ServerItem>&);
        virtual void AddItemsList(std::vector<m3d::DataServer::ServerItem>&);

    private:
        Profiler* m_profiler = nullptr;
        Profiler* m_profilerUpdate = nullptr;
    };
}

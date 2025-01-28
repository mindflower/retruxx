#pragma once
#include "DataServer.h"

namespace m3d
{
    class Profiler;

    class ParticlesServer : public m3d::DataServer
    {
    public:
        ParticlesServer();
        virtual  ~ParticlesServer() override /* 0x00 */;
        virtual int Init() override /* 0x04 */;
        virtual int Release() override /* 0x08 */;
        virtual int AddItem(const char* params, const char* id) override /* 0x0c */;
        virtual int RemoveItem(int id) override /* 0x18 */;
        virtual int GetItemProperty(int id, int prop, void* dest) override /* 0x10 */;
        virtual int SetItemProperty(int id, int prop, void* src) override /* 0x14 */;
        virtual void UpdateItem(int id, void* params) override /* 0x30 */;
        virtual void RegisterNode(m3d::SgNode* node) override /* 0x34 */;
        virtual void UnregisterNode(m3d::SgNode* node) override /* 0x38 */;
        virtual int SaveAllLoadedEntities(const char* filename) override /* 0x4c */;
        void SaveAllLoadedEntitiesToBPS();
        void ResetItem(m3d::SgNode* node);
        virtual void RenderItem(int id, void* params) override /* 0x1c */;
        virtual int RenderNodeSet(m3d::SgNode** nodes, unsigned int numNodes, m3d::RenderNodeInfo rni) override /* 0x20 */;

        using CVectorVector = retruxx::vector<CVector, retruxx::allocator<CVector> >;

    public:
        void AddParticle(m3d::SgNode* node, const CVector* pos);
        void AddParticles(m3d::SgNode* node, const retruxx::vector<CVector, retruxx::allocator<CVector> >* newPoses);
        void MoveParticles(m3d::SgNode* node, const retruxx::vector<CVector, retruxx::allocator<CVector> >* newPoses);

    protected:
        void AddItemsByOne(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist);
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override /* 0x58 */;

    private:
        /* 0x0048 */ m3d::Profiler* m_profiler = nullptr;
        /* 0x004c */ m3d::Profiler* m_profilerUpdate = nullptr;
    }; /* size: 0x0050 */

    static_assert(sizeof(ParticlesServer) == 0x0050);
}

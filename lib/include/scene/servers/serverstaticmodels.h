#pragma once
#include "DataServer.h"
#include <renderer/i_renderer.h>

namespace m3d
{
    class Profiler;

    class StaticModelsServer : public m3d::DataServer
    {
    private:
        m3d::rend::VbHandle m_vbArrayForAllModels[4];
        m3d::rend::IbHandle m_ibArrayForAllModels[4];
        int m_numDrawIndices = 0;
        int m_numDrawVerts = 0;
        int m_curvbBank = 0;
        int m_curibBank = 0;
        int m_numvbBanks = 0;
        int m_numibBanks = 0;
        bool m_preparedToRender = true;
        bool m_alreadyCached = false;
        int PrepareToRender();
        retruxx::vector<unsigned char*, retruxx::allocator<unsigned char*>> m_shots;
        bool m_tessellate = true;

    public:
        StaticModelsServer();
        virtual ~StaticModelsServer() override;
        virtual int Init() override;
        virtual int Release() override;
        virtual int AddItem(char const* params, char const* id) override;
        virtual int RemoveItem(int id) override;
        virtual void RenderItem(int id, void* params) override;
        virtual int GetItemProperty(int id, int prop, void* dest) override;
        virtual int SetItemProperty(int id, int prop, void* src) override;
        void ReleaseBuffers();
        virtual int SaveAllLoadedEntities(char const* fileName) override;

    protected:
        virtual void AddItemsList(
            retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem>>& itemslist)
            override;

    private:
        m3d::Profiler* m_profiler = nullptr;
    };
}  // namespace m3d

#pragma once
#include "DataServer.h"
#include <renderer/i_renderer.h>

namespace m3d
{
    class Profiler;

    class StaticModelsServer : public DataServer
    {
    public:
        virtual int GetItemProperty(int, int, void*);
        virtual int SaveAllLoadedEntities(char const*);
        void ReleaseBuffers();
        virtual void RenderItem(int, void*);
        virtual int SetItemProperty(int, int, void*);
        virtual int RemoveItem(int);
        virtual int Release();
        StaticModelsServer();
        virtual int Init();
        virtual ~StaticModelsServer();
        virtual int AddItem(char const*, char const*);

    protected:
        virtual void AddItemsList(std::vector<m3d::DataServer::ServerItem>&);

    private:
        int PrepareToRender();

    private:
        rend::VbHandle m_vbArrayForAllModels[4];
        rend::IbHandle m_ibArrayForAllModels[4];
        int m_numDrawIndices;
        int m_numDrawVerts;
        int m_curvbBank;
        int m_curibBank;
        int m_numvbBanks;
        int m_numibBanks;
        bool m_preparedToRender;
        bool m_alreadyCached;
        std::vector<unsigned char*> m_shots;
        bool m_tessellate;
        Profiler* m_profiler;
    };
}

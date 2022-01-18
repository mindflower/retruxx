#pragma once
#include "DataServer.h"
#include <renderer/i_renderer.h>

namespace m3d
{
    class Profiler;

    class ProjectorsServer : public DataServer
    {
    public:
        virtual ~ProjectorsServer();
        virtual int GetItemProperty(int, int, void*);
        virtual int SetItemProperty(int, int, void*);
        virtual int SaveAllLoadedEntities(char const*);
        virtual int Init();
        ProjectorsServer();
        virtual int Release();
        virtual int AddItem(char const*, char const*);
        virtual void RenderItem(int, void*);
        virtual int RemoveItem(int);

    protected:
        virtual void AddItemsList(std::vector<m3d::DataServer::ServerItem>&);

    private:
        rend::TexHandle m_attenuationTex;
        CMatrix m_texBiasMat;
        CMatrix m_texProjMat;
        CMatrix m_texProjBiasMat;
        CMatrix m_attenMat;
        Profiler* m_profiler = nullptr;
    };
}

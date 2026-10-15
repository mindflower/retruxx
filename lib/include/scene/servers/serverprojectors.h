#pragma once
#include "DataServer.h"
#include <renderer/i_renderer.h>

namespace m3d
{
    class Profiler;

    class ProjectorsServer : public m3d::DataServer
    {
    public:
        ProjectorsServer();
        virtual ~ProjectorsServer() override;
        virtual int Init() override;
        virtual int Release() override;
        virtual int AddItem(char const* params, char const* id) override;
        virtual int RemoveItem(int id) override;
        virtual int GetItemProperty(int id, int prop, void* dest) override;
        virtual int SetItemProperty(int id, int prop, void* src) override;
        virtual void RenderItem(int id, void* params) override;
        virtual int SaveAllLoadedEntities(char const* filename) override;

    protected:
        virtual void AddItemsList(
            retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem>>& itemslist)
            override;

    private:
        m3d::rend::TexHandle m_attenuationTex;
        CMatrix m_texBiasMat;
        CMatrix m_texProjMat;
        CMatrix m_texProjBiasMat;
        CMatrix m_attenMat;
        m3d::Profiler* m_profiler = nullptr;
    };
}  // namespace m3d

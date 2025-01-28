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
        virtual  ~ProjectorsServer() override /* 0x00 */;
        virtual int Init() override /* 0x04 */;
        virtual int Release() override /* 0x08 */;
        virtual int AddItem(const char* params, const char* id) override /* 0x0c */;
        virtual int RemoveItem(int id) override /* 0x18 */;
        virtual int GetItemProperty(int id, int prop, void* dest) override /* 0x10 */;
        virtual int SetItemProperty(int id, int prop, void* src) override /* 0x14 */;
        virtual void RenderItem(int id, void* params) override /* 0x1c */;
        virtual int SaveAllLoadedEntities(const char* filename) override /* 0x4c */;

    protected:
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override /* 0x58 */;

    private:
        /* 0x0048 */ m3d::rend::TexHandle m_attenuationTex;
        /* 0x004c */ CMatrix m_texBiasMat;
        /* 0x008c */ CMatrix m_texProjMat;
        /* 0x00cc */ CMatrix m_texProjBiasMat;
        /* 0x010c */ CMatrix m_attenMat;
        /* 0x014c */ m3d::Profiler* m_profiler = nullptr;
    }; /* size: 0x0150 */

    static_assert(sizeof(ProjectorsServer) == 0x0150);
}

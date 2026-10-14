#pragma once
#include "DataServer.h"

namespace m3d
{
    class Profiler;

    class DecalsServer : public m3d::DataServer
    {
    public:
        DecalsServer();
        virtual ~DecalsServer() override;
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
        m3d::Profiler* m_profiler = nullptr;
    };
}  // namespace m3d

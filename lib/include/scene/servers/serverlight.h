#pragma once
#include "DataServer.h"

namespace m3d
{
    class Profiler;

    struct RiForLightsServer
    {
        CMatrix m_localXForm;
        float m_radius;
    };

    class LightsServer : public m3d::DataServer
    {
    public:
        LightsServer();
        virtual  ~LightsServer() override;
        virtual int Init() override;
        virtual int Release() override;
        virtual int AddItem(const char* params, const char* id) override;
        virtual int RemoveItem(int id) override;
        virtual void RenderItem(int id, void* params) override;
        virtual int GetItemProperty(int id, int prop, void* dest) override;
        virtual int SetItemProperty(int id, int prop, void* src) override;
        virtual int SaveAllLoadedEntities(const char* filename) override;

    protected:
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override;

    private:
        m3d::Profiler* m_profiler = nullptr;
    };

    struct PointLightModel
    {
        int m_radius;
        CVector m_color;
        float m_ttl;
    };
}

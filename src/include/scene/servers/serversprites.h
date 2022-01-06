#pragma once
#include "DataServer.h"

namespace m3d
{
    class Profiler;

    class SpritesServer : public DataServer
    {
    public:
        virtual int GetItemProperty(int, int, void*);
        virtual int SetItemProperty(int, int, void*);
        virtual ~SpritesServer();
        virtual int Release();
        virtual void RenderItem(int, void*);
        virtual int SaveAllLoadedEntities(char const*);
        virtual int RemoveItem(int);
        virtual int AddItem(char const*, char const*);
        SpritesServer();
        virtual int Init();

    protected:
        virtual void AddItemsList(std::vector<m3d::DataServer::ServerItem>&);

    private:
        Profiler* m_profiler;
    };
}

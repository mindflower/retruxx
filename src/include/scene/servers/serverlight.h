#pragma once
#include "DataServer.h"

namespace m3d
{
    class Profiler;

    class LightsServer : public DataServer
    {
    public:
        virtual int SaveAllLoadedEntities(char const*);
        virtual int Init();
        virtual int Release();
        virtual int AddItem(char const*, char const*);
        virtual ~LightsServer();
        virtual void RenderItem(int, void*);
        virtual int RemoveItem(int);
        virtual int GetItemProperty(int, int, void*);
        LightsServer();
        virtual int SetItemProperty(int, int, void*)
        ;
    protected:
        virtual void AddItemsList(std::vector<ServerItem>&);

    private:
        Profiler* m_profiler = nullptr;
    };
}

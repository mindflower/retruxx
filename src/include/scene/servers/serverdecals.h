#pragma once
#include "DataServer.h"

namespace m3d
{
    class Profiler;

    class DecalsServer : public DataServer
    {
    public:
        virtual int AddItem(char const*, char const*);
        virtual int RemoveItem(int);
        virtual void RenderItem(int, void*);
        virtual int Init();
        virtual int SaveAllLoadedEntities(char const*);
        virtual ~DecalsServer();
        virtual int Release();
        virtual int SetItemProperty(int, int, void*);
        DecalsServer();
        virtual int GetItemProperty(int, int, void*);

    protected:
        virtual void AddItemsList(std::vector<ServerItem>&);

    private:
        Profiler* m_profiler;
    };
}

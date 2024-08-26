#pragma once
#include "DataServer.h"

namespace m3d
{
    class LinesServer : public DataServer
    {
    public:
        virtual int AddItem(char const*, char const*);
        virtual int SaveAllLoadedEntities(char const*);
        virtual ~LinesServer();
        virtual int RemoveItem(int);
        virtual int Init();
        virtual int Release();
        LinesServer();
        virtual void RenderItem(int, void*);
        virtual int SetItemProperty(int, int, void*);
        virtual int GetItemProperty(int, int, void*);

    protected:
        virtual void AddItemsList(std::vector<ServerItem>&);
    };
}

#pragma once
#include "DataServer.h"

namespace m3d
{
    class MusicServer : public DataServer
    {
    public:
        virtual ~MusicServer();
        virtual int GetItemProperty(int, int, void*);
        virtual int SetItemProperty(int, int, void*);
        virtual void RenderItem(int, void*);
        virtual int Release();
        virtual void PostLoad();
        virtual int RemoveItem(int);
        virtual int SaveAllLoadedEntities(char const*);
        virtual int AddItem(char const*, char const*);

    protected:
        virtual void AddItemsList(std::vector<m3d::DataServer::ServerItem>&);

    private:
        int _GetSoundIdByServerHandle(int) const;
    };
}

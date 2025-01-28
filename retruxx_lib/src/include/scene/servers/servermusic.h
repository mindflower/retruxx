#pragma once
#include "DataServer.h"

namespace m3d
{
    class MusicServer : public m3d::DataServer
    {
    public:
        virtual  ~MusicServer() override /* 0x00 */;
        virtual int Release() override /* 0x08 */;
        virtual int AddItem(const char* params, const char* id) override /* 0x0c */;
        virtual int RemoveItem(int id) override /* 0x18 */;
        virtual void RenderItem(int id, void* params) override /* 0x1c */;
        virtual void PostLoad() override /* 0x50 */;
        virtual int GetItemProperty(int id, int prop, void* dest) override /* 0x10 */;
        virtual int SetItemProperty(int id, int prop, void* src) override /* 0x14 */;
        virtual int SaveAllLoadedEntities(const char* filename) override /* 0x4c */;

    protected:
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override /* 0x58 */;

    private:
        int _GetSoundIdByServerHandle(int sh) const;
    }; /* size: 0x0048 */

    static_assert(sizeof(MusicServer) == 0x0048);
}

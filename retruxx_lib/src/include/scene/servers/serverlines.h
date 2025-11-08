#pragma once
#include "DataServer.h"

namespace m3d
{
    class LinesServer : public m3d::DataServer
    {
    public:
        LinesServer();
        virtual  ~LinesServer() override /* 0x00 */;
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
    }; /* size: 0x0048 */

    static_assert(sizeof(LinesServer) == 0x0048);
}

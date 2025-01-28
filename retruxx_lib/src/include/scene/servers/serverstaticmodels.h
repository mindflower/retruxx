#pragma once
#include "DataServer.h"
#include <renderer/i_renderer.h>

namespace m3d
{
    class Profiler;

    class StaticModelsServer : public m3d::DataServer
    {
    private:
        /* 0x0048 */ m3d::rend::VbHandle m_vbArrayForAllModels[4];
        /* 0x0058 */ m3d::rend::IbHandle m_ibArrayForAllModels[4];
        /* 0x0068 */ int m_numDrawIndices = 0;
        /* 0x006c */ int m_numDrawVerts = 0;
        /* 0x0070 */ int m_curvbBank = 0;
        /* 0x0074 */ int m_curibBank = 0;
        /* 0x0078 */ int m_numvbBanks = 0;
        /* 0x007c */ int m_numibBanks = 0;
        /* 0x0080 */ bool m_preparedToRender = true;
        /* 0x0081 */ bool m_alreadyCached = false;
        int PrepareToRender();
        /* 0x0082 */ char Padding_237[2];
        /* 0x0084 */ retruxx::vector<unsigned char*, retruxx::allocator<unsigned char*> > m_shots;
        /* 0x0094 */ bool m_tessellate = true;

    public:
        StaticModelsServer();
        virtual  ~StaticModelsServer() override /* 0x00 */;
        virtual int Init() override /* 0x04 */;
        virtual int Release() override /* 0x08 */;
        virtual int AddItem(const char* params, const char* id) override /* 0x0c */;
        virtual int RemoveItem(int id) override /* 0x18 */;
        virtual void RenderItem(int id, void* params) override /* 0x1c */;
        virtual int GetItemProperty(int id, int prop, void* dest) override /* 0x10 */;
        virtual int SetItemProperty(int id, int prop, void* src) override /* 0x14 */;
        void ReleaseBuffers();
        virtual int SaveAllLoadedEntities(const char* fileName) override /* 0x4c */;

    protected:
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override /* 0x58 */;
        /* 0x0095 */ char Padding_238[3];

    private:
        /* 0x0098 */ m3d::Profiler* m_profiler = nullptr;
    }; /* size: 0x009c */

    static_assert(sizeof(StaticModelsServer) == 0x009c);
}

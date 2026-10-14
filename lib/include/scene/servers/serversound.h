#pragma once
#include "DataServer.h"

namespace m3d
{
    class Sound3DServer : public m3d::DataServer
    {
    public:
        virtual  ~Sound3DServer() override;
        virtual int Release() override;
        virtual int AddItem(const char* fileName, const char* id) override;
        virtual int RemoveItem(int id) override;
        virtual void RenderItem(int id, void* params) override;
        virtual void RegisterNode(m3d::SgNode* node) override;
        virtual void UnregisterNode(m3d::SgNode* node) override;
        virtual void PostLoad() override;
        virtual int GetItemProperty(int id, int prop, void* dest) override;
        virtual int SaveAllLoadedEntities(const char* filename) override;

    protected:
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override;

    private:
        void _AddItemFromXmlNode(const m3d::cmn::XmlNode* xmlNode);
        int _AddFakeItem();
        int _AddItem(const char* fileName, const char* id, const char* groupName);
        int _AddDoubleItem(CStr f1, CStr f2, const char* id, const char* groupName);
        int _AddTripleItem(CStr f1, CStr f2, CStr f3, const char* id, const char* groupName);

        enum SoundTypeEnum
        {
            SOUND_TYPE_SIMPLE = 0,
            SOUND_TYPE_DOUBLE = 1,
            SOUND_TYPE_TRIPLE = 2,
        };

        struct SoundItem
        {
            SoundItem();
            m3d::Sound3DServer::SoundTypeEnum type;
            int soundIds[3];
        };
    };
}

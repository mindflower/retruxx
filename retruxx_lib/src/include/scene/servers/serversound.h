#pragma once
#include "DataServer.h"

namespace m3d
{
    class Sound3DServer : public m3d::DataServer
    {
    public:
        virtual  ~Sound3DServer() override /* 0x00 */;
        virtual int Release() override /* 0x08 */;
        virtual int AddItem(const char* fileName, const char* id) override /* 0x0c */;
        virtual int RemoveItem(int id) override /* 0x18 */;
        virtual void RenderItem(int id, void* params) override /* 0x1c */;
        virtual void RegisterNode(m3d::SgNode* node) override /* 0x34 */;
        virtual void UnregisterNode(m3d::SgNode* node) override /* 0x38 */;
        virtual void PostLoad() override /* 0x50 */;
        virtual int GetItemProperty(int id, int prop, void* dest) override /* 0x10 */;
        virtual int SaveAllLoadedEntities(const char* filename) override /* 0x4c */;

    protected:
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >& itemslist) override /* 0x58 */;

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
            /* 0x0000 */ m3d::Sound3DServer::SoundTypeEnum type;
            /* 0x0004 */ int soundIds[3];
        }; /* size: 0x0010 */
    }; /* size: 0x0048 */

    static_assert(sizeof(Sound3DServer) == 0x0048);
}

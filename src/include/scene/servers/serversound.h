#pragma once
#include "DataServer.h"

namespace m3d
{
    class Sound3DServer : public DataServer
    {
    public:
        virtual void UnregisterNode(m3d::SgNode*);
        virtual int RemoveItem(int);
        virtual ~Sound3DServer();
        virtual int AddItem(char const*, char const*);
        virtual void PostLoad();
        virtual int GetItemProperty(int, int, void*);
        virtual void RegisterNode(m3d::SgNode*);
        virtual int SaveAllLoadedEntities(char const*);
        virtual void RenderItem(int, void*);
        virtual int Release();
    protected:
        virtual void AddItemsList(std::vector<m3d::DataServer::ServerItem>&);

    private:
        void _AddItemFromXmlNode(m3d::cmn::XmlNode const*);
        int _AddFakeItem();
        int _AddTripleItem(CStr, CStr, CStr, char const*, char const*);
        int _AddItem(char const*, char const*, char const*);
        int _AddDoubleItem(CStr, CStr, char const*, char const*);
    };
}

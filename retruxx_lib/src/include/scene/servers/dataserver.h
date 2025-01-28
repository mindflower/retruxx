#pragma once
#include <scene/nodes/sgnode.h>

namespace m3d
{
    enum RenderNodeType
    {
        RNT_SIMPLE = 0x0,
        RNT_FOR_SHADOW = 0x1,
        RNT_FOR_PROJECTOR = 0x2,
        RNT_FOR_POINTLIGHT = 0x3,
        RNT_FOR_CONTOUR = 0x4,
    };

    struct RenderNodeInfo
    {
        /* 0x0000 */ m3d::RenderNodeType rnt;
        /* 0x0004 */ CMatrix projTansform;
        /* 0x0044 */ CVector projOrg;
        /* 0x0050 */ CVector projDir;
        /* 0x005c */ bool isCullInverted;
        /* 0x005d */ bool isPrimaryRender;
        /* 0x005e */ bool isUseImpostors;
        RenderNodeInfo(const m3d::RenderNodeInfo& __that);
        RenderNodeInfo(m3d::RenderNodeType t);
        RenderNodeInfo();
    }; /* size: 0x0060 */

    static_assert(sizeof(RenderNodeInfo) == 0x0060);

    class DataServer
    {
    public:
        DataServer(const m3d::DataServer&);
        DataServer();
        virtual  ~DataServer() /* 0x00 */;
        int GetNumItems() const;
        CStr GetOriginalFileName(int sh);
        /* 0x0004 */ void* m_fnLoadCallbackData = nullptr;
        /* 0x0008 */ void (*m_fnLoadCallback)(int, void*) = nullptr;
        virtual int Init() /* 0x04 */;
        virtual int Release() /* 0x08 */;
        virtual int AddItem(const char*, const char*) = 0 /* 0x0c */;
        void GenerateItemsRemap();
        int GetItemByName(const char* id, bool viaMap) const;
        const CStr& GetNameByItem(int sh) const;
        virtual int GetItemProperty(int id, int prop, void* dest) /* 0x10 */;
        virtual int SetItemProperty(int id, int prop, void* src) /* 0x14 */;
        virtual int RemoveItem(int) = 0 /* 0x18 */;
        virtual void RenderItem(int, void*) = 0 /* 0x1c */;
        virtual int RenderNodeSet(m3d::SgNode** nodes, unsigned int numNodes, m3d::RenderNodeInfo rni) /* 0x20 */;
        virtual void RenderTransparents(m3d::SgNode** nodes, unsigned int numNodes) /* 0x24 */;
        virtual int RenderShadowVolumesSet(m3d::SgNode** nodes, unsigned int numNodes) /* 0x28 */;
        virtual int GenerateImpostorsIfNeeded() /* 0x2c */;
        virtual void UpdateItem(int id, void* params) /* 0x30 */;
        virtual void RegisterNode(m3d::SgNode* node) /* 0x34 */;
        virtual void UnregisterNode(m3d::SgNode* node) /* 0x38 */;
        virtual void Invalidate() /* 0x3c */;
        virtual void Restore() /* 0x40 */;
        const char* GetLastError();
        int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
        virtual int WriteItemToXmlNode(int sh, m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) /* 0x44 */;
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* root) /* 0x48 */;
        virtual int SaveAllLoadedEntities(const char*) = 0 /* 0x4c */;
        virtual void PostLoad() /* 0x50 */;
        virtual bool ReportServerInfo(const char* fileName) /* 0x54 */;

        struct Model
        {
            Model(void* p, const char* filename, const char* originalFileName, const char* id);
            /* 0x0000 */ void* m_ptr = nullptr;
            /* 0x0004 */ CStr m_name;
            /* 0x0010 */ CStr m_fileName;
            /* 0x001c */ CStr m_originalFileName;
            /* 0x0028 */ unsigned int m_additionalData[16] = { 0 };
        }; /* size: 0x0068 */

        struct ServerItem
        {
            /* 0x0000 */ CStr m_id;
            /* 0x000c */ CStr m_filename;
            /* 0x0018 */ CStr m_params;
            /* 0x0024 */ bool m_fileWasRead = false;
        }; /* size: 0x0028 */

        using ServerItemVector = retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >;
        using ModelVector = retruxx::vector<m3d::DataServer::Model, retruxx::allocator<m3d::DataServer::Model> >;
        class ModelsVecIter;
        using ServerHandlesRemap = retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > >;

    protected:
        /* 0x000c */ retruxx::vector<m3d::DataServer::Model, retruxx::allocator<m3d::DataServer::Model> > m_models;
        /* 0x001c */ retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > > m_shRemap;
        /* 0x0028 */ retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> > m_itemslist;
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >&) = 0 /* 0x58 */;
        bool IsValid() const;

        enum Proto
        {
            PROTO_NONE = 0,
            PROTO_FILE = 1,
            PROTO_NEW = 2,
        };

    protected:
        /* 0x0038 */ CStr m_lastError;
        int ParseProto(const char* in, m3d::DataServer::Proto* protocol, int* paramsPos);
        void SetError(const CStr& err);

    protected:
        /* 0x0044 */ bool m_valid = false;
    }; /* size: 0x0048 */

    static_assert(sizeof(DataServer) == 0x0048);
}

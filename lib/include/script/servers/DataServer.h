#pragma once

namespace m3d
{
    class SgNode;

    enum class RenderNodeType
    {
        RNT_SIMPLE = 0,
        RNT_FOR_SHADOW,
        RNT_FOR_PROJECTOR,
        RNT_FOR_POINTLIGHT,
        RNT_FOR_CONTOUR,
    };

    class RenderNodeInfo
    {
    public:
        RenderNodeInfo(m3d::RenderNodeInfo const&);
        RenderNodeInfo(m3d::RenderNodeType);
        RenderNodeInfo(void);
    };

    class m3d::DataServer
    {
    public:
        DataServer(m3d::DataServer const&);
        DataServer();
        virtual ~DataServer() /* 0x00 */;
        int GetNumItems() const;
        CStr GetOriginalFileName(int sh);
        /* 0x0004 */ void* m_fnLoadCallbackData;
        /* 0x0008 */ void (*m_fnLoadCallback)(int, void*);
        virtual int Init() /* 0x04 */;
        virtual int Release() /* 0x08 */;
        virtual int AddItem(char const*, char const*) = 0 /* 0x0c */;
        void GenerateItemsRemap();
        int GetItemByName(char const* id, bool viaMap) const;
        CStr const& GetNameByItem(int sh) const;
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
        char const* GetLastError();
        int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
        virtual int WriteItemToXmlNode(int sh, m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) /* 0x44 */;
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* root) /* 0x48 */;
        virtual int SaveAllLoadedEntities(char const*) = 0 /* 0x4c */;
        virtual void PostLoad() /* 0x50 */;
        virtual bool ReportServerInfo(char const* fileName) /* 0x54 */;

        struct Model
        {
            Model(m3d::DataServer::Model const& __that);
            Model(void* p, char const* filename, char const* originalFileName, char const* id);
            void* m_ptr;
            CStr m_name;
            CStr m_fileName;
            CStr m_originalFileName;
            unsigned int m_additionalData[16];
        };

        struct ServerItem
        {
            CStr m_id;
            CStr m_filename;
            CStr m_params;
            bool m_fileWasRead;
            ServerItem(m3d::DataServer::ServerItem const& __that);
            ServerItem();
        };

        using ServerItemVector = std::vector<m3d::DataServer::ServerItem, std::allocator<m3d::DataServer::ServerItem>>;
        using ModelVector = std::vector<m3d::DataServer::Model, std::allocator<m3d::DataServer::Model>>;
        class ModelsVecIter;
        using ServerHandlesRemap = std::map<CStr, int, std::less<CStr>, std::allocator<std::pair<CStr const, int>>>;

    protected:
        std::vector<m3d::DataServer::Model, std::allocator<m3d::DataServer::Model>> m_models;
        std::map<CStr, int, std::less<CStr>, std::allocator<std::pair<CStr const, int>>> m_shRemap;
        std::vector<m3d::DataServer::ServerItem, std::allocator<m3d::DataServer::ServerItem>> m_itemslist;
        virtual void AddItemsList(std::vector<m3d::DataServer::ServerItem, std::allocator<m3d::DataServer::ServerItem>>&) = 0 /* 0x58 */;
        bool IsValid() const;

        enum Proto
        {
            PROTO_NONE = 0,
            PROTO_FILE = 1,
            PROTO_NEW = 2,
        };

    protected:
        CStr m_lastError;
        int ParseProto(char const* in, m3d::DataServer::Proto* protocol, int* paramsPos);
        void SetError(CStr const& err);

    private:
        bool m_valid;
    };
}  // namespace m3d
#pragma once
#include <map>
#include <vector>
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

    class RenderNodeInfo
    {
    public:
        RenderNodeInfo(RenderNodeInfo const&);
        RenderNodeInfo(RenderNodeType);

    private:
        RenderNodeType rnt;
        CMatrix projTansform;
        CVector projOrg;
        CVector projDir;
        bool isCullInverted;
        bool isPrimaryRender;
        bool isUseImpostors;
    };

    class DataServer
    {
    public:
        enum Proto
        {
            PROTO_NONE = 0x0,
            PROTO_FILE = 0x1,
            PROTO_NEW = 0x2,
        };

        class ServerItem
        {
        public:
            ServerItem(ServerItem const&);

        private:
            CStr m_id;
            CStr m_filename;
            CStr m_params;
            bool m_fileWasRead;
        };

        class Model
        {
        public:
            Model(void*, char const*, char const*, char const*);

        private:
            void* m_ptr;
            CStr m_name;
            CStr m_fileName;
            CStr m_originalFileName;
            unsigned int m_additionalData[16];
        };

    public:
        int GetItemByName(char const*, bool) const;
        virtual int RenderShadowVolumesSet(SgNode**, unsigned int);
        virtual int RenderNodeSet(SgNode**, unsigned int, RenderNodeInfo);
        virtual void RenderTransparents(SgNode**, unsigned int);
        virtual void PostLoad();
        CStr const& GetNameByItem(int) const;
        virtual bool ReportServerInfo(char const*);
        virtual int WriteItemToXmlNode(int, cmn::XmlFile*, cmn::XmlNode*);
        int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual void UpdateItem(int, void*);
        virtual int Release();
        virtual int GenerateImpostorsIfNeeded();
        virtual int Init();
        virtual void Restore();
        virtual void UnregisterNode(SgNode*);
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        DataServer();
        virtual int SetItemProperty(int, int, void*);
        void GenerateItemsRemap();
        CStr GetOriginalFileName(int);
        virtual ~DataServer();
        virtual void RegisterNode(SgNode*);
        virtual void Invalidate();
        virtual int GetItemProperty(int, int, void*);
        int GetNumItems() const;

        virtual int AddItem(char const*, char const*) = 0;

    protected:
        void SetError(CStr const&);
        bool IsValid() const;
        int ParseProto(char const*, Proto*, int*);

    protected:
        void* m_fnLoadCallbackData = nullptr;
        void(__fastcall* m_fnLoadCallback)(int, void*) = nullptr;
        std::vector<Model> m_models;
        std::map<CStr, int> m_shRemap;
        std::vector<ServerItem> m_itemslist;
        CStr m_lastError;
        bool m_valid = false;
    };
}

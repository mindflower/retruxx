#pragma once
#include "cmn/xmlfile.h"

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

    class DataServer
    {
    public:
        class Model
        {
        public:
            Model(m3d::DataServer::Model const&);
            Model(void*, char const*, char const*, char const*);
            ~Model(void);
            void operator=(m3d::DataServer::Model const&);
        };

        class ServerItem
        {
            ServerItem(m3d::DataServer::ServerItem const&);
            ServerItem(void);
            ~ServerItem(void);
            void operator=(m3d::DataServer::ServerItem const&);
        };

        enum class Proto
        {
            PROTO_NONE = 0,
            PROTO_FILE,
            PROTO_NEW,
        };

    public:
        virtual ~DataServer() = default;
        bool Init();

        DataServer(void);
        void GenerateImpostorsIfNeeded(void);
        void GenerateItemsRemap(void);
        void GetItemByName(char const*, bool);
        void GetItemProperty(int, int, void*);
        void GetNameByItem(int);
        void GetNumItems(void);
        void GetOriginalFileName(int);
        void Invalidate(void);
        void IsValid(void);
        void ParseProto(char const*, m3d::DataServer::Proto*, int*);
        void PostLoad(void);
        void ReadFromXmlNode(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*);
        void RegisterNode(m3d::SgNode*);
        void Release(void);
        void RenderNodeSet(m3d::SgNode**, size_t, m3d::RenderNodeInfo);
        void RenderShadowVolumesSet(m3d::SgNode**, size_t);
        void RenderTransparents(m3d::SgNode**, size_t);
        void ReportServerInfo(char const*);
        void Restore(void);
        void SetError(CStr const&);
        void SetItemProperty(int, int, void*);
        void UnregisterNode(m3d::SgNode*);
        void UpdateItem(int, void*);
        void WriteItemToXmlNode(int, m3d::cmn::XmlFile*, m3d::cmn::XmlNode*);
        void WriteToXmlNode(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*);
    };
}
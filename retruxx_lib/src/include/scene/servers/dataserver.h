#pragma once
#include <scene/nodes/sgnode.h>

namespace m3d
{
    enum
    {
        PROP_SERVER_SLOT = 1,
        PROP_SERVER_SLOT_1 = 2,
        PROP_NODE_LAST_DEFAULT = 3,
        PROP_MODEL_CAST_SHADOW = 0,
        PROP_MODEL_WIND_WAVY = 1,
        PROP_MODEL_TRANS = 2,
        PROP_MODEL_2SIDED = 3,
        PROP_MODEL_TESSELLATE = 4,
        PROP_MODEL_TRACK_LAND = 5,
        PROP_MODEL_CAST_SHADOWVOL = 6,
        PROP_MODEL_LAST_DEFAULT = 7,
        PROP_MODEL_NAME = 256,
        PROP_MODEL_FILENAME = 257,
        PROP_NODE_DEBUG_STR = 4096,
        PROP_NODE_FIRST = 4352,
        PROP_NODE_BELONG = 4353,
        PROP_NODE_FOV = 4354,
        PROP_NODE_HAVE_CHILDSOUND = 4355,
        PROP_NODE_PHYSICBODY = 4356,
        PROP_NODE_GEOMOBJSLIST = 4357,
        PROP_NODE_HAVECOLLISION = 4358,
        PROP_NODE_LASTPARENT = 4359,
        PROP_NODE_HANDLE = 4360,
        PROP_NODE_GAMEOBJECT = 4361,
        PROP_NODE_LAST = 4362,
        PROP_LS_FIRST = 8448,
        PROP_LS_COLOR = 8448,
        PROP_LS_RADIUS = 8449,
        PROP_LS_BRIGHTNESS = 8450,
        PROP_LS_LAST = 8451,
        PROP_DM_ACTION = 8704,
        PROP_DM_SKIN = 8706,
        PROP_DM_CFG = 8707,
        PROP_DM_EFFECT_ACTION = 8708,
        PROP_DM_MODEL_ACTION = 8709,
        PROP_DM_ARRAY_EFFECT_ACTIONS = 8710,
        PROP_DM_ADD_EFFECT_ACTION = 8711,
        PROP_DM_DEL_EFFECT_ACTION = 8712,
        PROP_DM_CHECK_ACTION = 8713,
        PROP_DM_SUPPRESS_LPS = 8714,
        PROP_DM_RESTART_EFFECT_ACTIONS = 8715,
        PROP_DM_MANUAL_ANIMCONTROL = 8716,
        PROP_DM_PASSABLE = 8717,
        PROP_DM_OBSTACLE = 8718,
        PROP_DM_LAST_TIME_UPDATED = 8719,
        PROP_DM_IS_IMPOSTED = 8720,
        PROP_DM_CAST_SHADOW = 8721,
        PROP_SPR_FIRST = 9216,
        PROP_SPR_MODE = 9216,
        PROP_SPR_COLOR = 9217,
        PROP_SPR_TIME0 = 9218,
        PROP_SPR_ANIM_LEN = 9219,
        PROP_SPR_ANGULAR_SPEED = 9220,
        PROP_SPR_LOOPED = 9221,
        PROP_SPR_LAST = 9222,
        PROP_PS_FIRST = 9472,
        PROP_PS_NUM_EMITTER_MESH = 9472,
        PROP_PS_ADD_PARTICLE = 9473,
        PROP_PS_MOVE_PARTICLES = 9474,
        PROP_PS_ADD_PARTICLES = 9475,
        PROP_PS_LAST = 9476,
        PROP_SND_FIRST = 9728,
        PROP_SND_LOOPED = 9728,
        PROP_SND_CHANNELID = 9729,
        PROP_SND_MAXVOLUME = 9730,
        PROP_SND_BASE_FREQUENCY = 9731,
        PROP_SND_PLAYBACK_COEFF = 9732,
        PROP_SND_SOUND_ENABLED = 9733,
        PROP_SND_LAST = 9734,
        PROP_MUSIC_END_CALLBACK = 9856,
        PROP_LINES_FIRST = 9984,
        PROP_LINES_COLOR = 9984,
        PROP_LINES_RAD = 9985,
        PROP_LINES_LAST = 9986,
        PROP_PROJECTOR_FIRST = 10240,
        PROP_PROJECTOR_RADIUS = 10241,
        PROP_PROJECTOR_LAST = 10242,
        PROP_DECAL_LIST = 10496,
        PROP_DECAL_ADD = 10497,
        PROP_DECAL_RECALC = 10498,
        PROP_SRV_BOUNDING_BOX = 12288,
        PROP_SRV_SOUND_FOR_ACTION = 12289,
        PROP_SRV_SOUND_IS_LOOPED = 12294,
        PROP_SRV_COLLISION_DATA_SZ = 12290,
        PROP_SRV_COLLISION_DATA = 12291,
        PROP_SRV_LOADPOINT = 12292,
        PROP_SRV_IS_LOCAL = 12293,
        PROP_SRV_SNAPSHOT = 12295,
        PROP_SRV_ACTION_TIME = 12296,
        PROP_SRV_ATTACK_FRAMETIME = 12297,
        PROP_SRV_SND_ID = 12320,
        PROP_SRV_SND_SIMPLE = 12321,
        PROP_SRV_SND_DOUBLE = 12322,
        PROP_SRV_SND_TRIPLE = 12323,
        PROP_INTERNAL_GET_MESH_POINTS = 16391,
        PROP_INTERNAL_FREE_MESH_POINTS = 16392,
        PROP_INTERNAL_GETGSMODEL = 16393,
        PROP_INTERNAL_GETMODEL = 16394,
    };

    struct PropSrvBoundingBox
    {
        /* 0x0000 */ m3d::SgNode* m_node;
        /* 0x0004 */ Aabb* m_destBox;
    }; /* size: 0x0008 */

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

#pragma once
#include <scene/nodes/sgnode.h>
#include <renderer/i_renderer_vertex.h>

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
        m3d::SgNode* m_node;
        Aabb* m_destBox;
    };

    // PROP_SRV_ACTION_TIME: m_action in, the action's length out.
    struct PropSrvActionTime
    {
        int m_action;
        float m_delta;
    };

    // PROP_DM_CHECK_ACTION: m_action in, what the model defines for it out. The PDB names no type for it; this
    // follows the bytes the animated models server writes.
    struct PropDmCheckAction
    {
        int m_action;
        bool m_hasFrames;
        bool m_hasEffects;
        bool m_hasSkin;
        bool m_hasCfg;
    };

    // PROP_SRV_ATTACK_FRAMETIME: m_action in, the time of the action's attack frame out.
    struct PropSrvAttackframeTime
    {
        int m_action;
        float m_attackFrameTime;
    };

    // PROP_INTERNAL_GET_MESH_POINTS / PROP_INTERNAL_FREE_MESH_POINTS: a copy of a model's meshes, used by particle
    // systems that emit from a mesh.
    struct PropInternalGetMeshPoints
    {
        int m_numMesh;
        m3d::SgNode* m_node;
        void** m_verts;
        retruxx::vector<m3d::rend::VertexType> m_VertexTypes;
        retruxx::vector<unsigned int> m_VertexTypeSizes;
        int* m_numVerts;
        unsigned short** m_indxs;
        int* m_numIndxs;
        bool* m_strips;
        CMatrix** m_localmatr;
        int m_numSkinMesh;
    };
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
        m3d::RenderNodeType rnt;
        CMatrix projTansform;
        CVector projOrg;
        CVector projDir;
        bool isCullInverted;
        bool isPrimaryRender;
        bool isUseImpostors;
        RenderNodeInfo(m3d::RenderNodeType t);
        RenderNodeInfo();
    };

    class DataServer
    {
    public:
        DataServer(const m3d::DataServer&);
        DataServer();
        virtual  ~DataServer();
        int GetNumItems() const;
        CStr GetOriginalFileName(int sh);
        void* m_fnLoadCallbackData = nullptr;
        void (*m_fnLoadCallback)(int, void*) = nullptr;
        virtual int Init();
        virtual int Release();
        virtual int AddItem(const char*, const char*) = 0;
        void GenerateItemsRemap();
        int GetItemByName(const char* id, bool viaMap) const;
        const CStr& GetNameByItem(int sh) const;
        virtual int GetItemProperty(int id, int prop, void* dest);
        virtual int SetItemProperty(int id, int prop, void* src);
        virtual int RemoveItem(int) = 0;
        virtual void RenderItem(int, void*) = 0;
        virtual int RenderNodeSet(m3d::SgNode** nodes, unsigned int numNodes, m3d::RenderNodeInfo rni);
        virtual void RenderTransparents(m3d::SgNode** nodes, unsigned int numNodes);
        virtual int RenderShadowVolumesSet(m3d::SgNode** nodes, unsigned int numNodes);
        virtual int GenerateImpostorsIfNeeded();
        virtual void UpdateItem(int id, void* params);
        virtual void RegisterNode(m3d::SgNode* node);
        virtual void UnregisterNode(m3d::SgNode* node);
        virtual void Invalidate();
        virtual void Restore();
        const char* GetLastError();
        int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
        virtual int WriteItemToXmlNode(int sh, m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* root);
        virtual int SaveAllLoadedEntities(const char*) = 0;
        virtual void PostLoad();
        virtual bool ReportServerInfo(const char* fileName);

        struct Model
        {
            Model(void* p, const char* filename, const char* originalFileName, const char* id);
            void* m_ptr = nullptr;
            CStr m_name;
            CStr m_fileName;
            CStr m_originalFileName;
            unsigned int m_additionalData[16] = { 0 };
        };

        struct ServerItem
        {
            CStr m_id;
            CStr m_filename;
            CStr m_params;
            bool m_fileWasRead = false;
        };

        using ServerItemVector = retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >;
        using ModelVector = retruxx::vector<m3d::DataServer::Model, retruxx::allocator<m3d::DataServer::Model> >;
        class ModelsVecIter;
        using ServerHandlesRemap = retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > >;

    protected:
        retruxx::vector<m3d::DataServer::Model, retruxx::allocator<m3d::DataServer::Model> > m_models;
        retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > > m_shRemap;
        retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> > m_itemslist;
        virtual void AddItemsList(retruxx::vector<m3d::DataServer::ServerItem, retruxx::allocator<m3d::DataServer::ServerItem> >&) = 0;
        bool IsValid() const;

        enum Proto
        {
            PROTO_NONE = 0,
            PROTO_FILE = 1,
            PROTO_NEW = 2,
        };

    protected:
        CStr m_lastError;
        int ParseProto(const char* in, m3d::DataServer::Proto* protocol, int* paramsPos);
        void SetError(const CStr& err);

    protected:
        bool m_valid = false;
    };
}

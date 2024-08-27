#pragma once
#include <draftstructures.h>
#include <map>
#include <set>
#include <math/aabb.h>
#include <math/matrix.h>
#include <math/quaternion.h>

enum ActionType
{
    AT_STAND1 = 0x0,
    AT_STAND2 = 0x1,
    AT_MOVE1 = 0x2,
    AT_MOVE2 = 0x3,
    AT_ATTACK1 = 0x4,
    AT_ATTACK2 = 0x5,
    AT_PAIN1 = 0x6,
    AT_PAIN2 = 0x7,
    AT_DEATH1 = 0x8,
    AT_DEATH2 = 0x9,
    AT_BLOCK1 = 0xA,
    AT_BLOCK2 = 0xB,
    AT_RESERVED1 = 0xC,
    AT_RESERVED2 = 0xD,
    AT_RESERVED3 = 0xE,
    AT_RESERVED4 = 0xF,
    AT_SND_SELECT = 0x10,
    AT_SND_SELECT2 = 0x11,
    AT_SND_SELECT3 = 0x12,
    AT_SND_SELECT4 = 0x13,
    AT_SND_ATTACK = 0x14,
    AT_SND_ATTACK2 = 0x15,
    AT_SND_ATTACK3 = 0x16,
    AT_SND_ATTACK4 = 0x17,
    AT_SND_WALK = 0x18,
    AT_SND_WALK2 = 0x19,
    AT_SND_WALK3 = 0x1A,
    AT_SND_WALK4 = 0x1B,
    AT_SND_MOVE1A = 0x1C,
    AT_SND_MOVE2A = 0x1D,
    AT_SND_UNREACH = 0x1E,
    AT_ALL_FRAMES = 0x1F,
    AT_NUMTYPES = 0x20,
};

namespace m3d
{
    class Configuration;
    class AnimInfo;

    class LoadSkins
    {
    public:
        LoadSkins();

    private:
        bool loadAllSkins = true;
        std::set<int> loadSkins;
    };

    class BoneAnim
    {
    private:
        int m_lastUpdatedFrame;
        CMatrix m_curMatrix;
        Quaternion m_rotation;
        CVector m_translation;
        int m_parentIdx;
    };

    class MeshesGroup
    {
    public:
        void GetNextVariant(class std::vector<unsigned int, class std::allocator<unsigned int> >&);

    private:
        std::string Name;
        std::vector<unsigned int> MeshesId;
        int VisibleAtOnceMax;
        int VisibleAtOnceMin;
        std::vector<std::vector<unsigned int>> m_variants;
    };

    class AnimatedModel
    {
    public:
        class Bone
        {
        public:
            Bone(Bone const&);
            Bone();

        public:
            char m_boneName[40];
            int m_ownIdx = -1;
            int m_parentIdx;
            CVector m_translation0;
            Quaternion m_quaternion0;
        };

        struct HierarchyChange
        {
            __int16 ownIdx;
            DRAFT_Change changeType;
            __int16 newParentIdx;
        };

        struct AnimationTransform
        {
            __int16 idx;
            float tx;
            float ty;
            float tz;
            float qx;
            float qy;
            float qz;
            float qw;
        };

        class Animation
        {
        public:
            ~Animation();
            Animation();

        public:
            __int16 m_numNodes;
            __int16 m_numFrames;
            __int16 m_fps;
            __int16 m_numChanges;
            __int16 m_nextAnimation;
            ActionType m_action;
            char m_name[25];
            HierarchyChange* m_hierChanges;
            AnimationTransform* m_nodesPositions;
        };

        class Influences
        {
        public:
            class Infl
            {
            private:
                __int16 m_boneIdx;
                float m_boneWeight;
                CVector m_offsetVec;
                CVector m_offsetNormal;
            };

        private:
            unsigned __int16 m_numBones;
            Infl m_influences[4];
        };

        class Mesh
        {
        public:
            void ComputeShadowsRelatedStuff();
            ~Mesh();
            Mesh();
            DSurfaceMaterial& GetMaterial(unsigned int) const;

        public:
            char m_meshName[40];
            int m_meshType;
            int m_numNode;
            unsigned int meshId;
            unsigned int groupId;
            int m_MaterialNumber;
            std::vector<std::vector<DSurfaceMaterial>>* m_pModelSkins;
            std::vector<DRAFT_VertexComponent> m_VCHs;
            unsigned int m_VertexTypeSize;
            rend::VertexType m_VertexType;
            int m_numVertices;
            void* m_verts;
            int m_numDrawVerts;
            void* m_drawVerts;
            Influences* m_vertsInfluences;
            int* m_vertsRemap;
            unsigned __int16* m_tris;
            int m_numFaces;
            unsigned __int16* m_drawIndices;
            int m_numDrawIndices;
            int m_lastFrameUpdated;
            rend::VbPoolField m_VbPoolField;
            rend::IbPoolField m_IbPoolField;
            int m_numFacesWelded;
            unsigned __int16* m_trisWelded;
            int m_numVertsWelded;
            CVector* m_vertsWelded;
            CVector* m_faceNormals;
        };

        struct ModelInfo
        {
            __int16 m_numNodes;
            __int16 m_numTriMeshes;
            __int16 m_numSkinMeshes;
            __int16 m_numStaticMeshes;
            __int16 m_numAnimations;
        };

    public:
        static void CreateTexFileMapping();

    public:
        static inline std::map<CStr, CStr> m_textureFiles;
        static inline CStr m_cubemapTexPath;
        static inline CStr m_cubemapTexName;

    public:
        DRAFT_HierGeom const * GetHierGeom(unsigned int) const ;
        AnimatedModel();
        Mesh & GetMesh(unsigned int);
        Mesh const & GetMesh(unsigned int) const ;
        DRAFT_BoneBounds const & GetBoneBounds(unsigned int);
        CMatrix GetBoneMatrix(int) const ;
        void SetSkinsToLoad(LoadSkins const &);
        bool LoadGAM(CStr const &,bool);
        bool LoadSAM(CStr const &,bool);
        Bone & GetBone(unsigned int) const ;
        unsigned int GetNumMeshes() const ;
        void DeleteSkin(unsigned int);
        void UnloadSkin(unsigned int);
        static void __fastcall UpdateTexturesFilter();
        unsigned int GetNumSkins() const ;
        ~AnimatedModel();
        rend::TexHandle GetTexHandle(unsigned int,unsigned int,unsigned int) const ;
        bool Load(CStr const &,bool);
        unsigned int GetGroupsNum() const ;
        int GetFrames(int,int) const ;
        void RenderNormals(CMatrix const &,AnimInfo *);
        void AddSkin(unsigned int);
        int GetLoadPointIdByName(char const *) const ;
        DRAFT_Geom const * GetGeom(unsigned int) const ;
        int ChangeShader(CStr const &,unsigned int,unsigned int);
        bool bIsPassable() const ;
        void FromGroupVariants(Configuration &) const ;
        void ReloadSkins(LoadSkins const &);
        void CalculateMeshes(Configuration &) const ;
        void RenderHierGeoms(CMatrix const &,AnimInfo *,unsigned int);
        int ChangeTexture(CStr const &,unsigned int,unsigned int,unsigned int);
        int Render(CMatrix const &,AnimInfo *,unsigned int,bool);
        int Render(CMatrix const &,AnimInfo *,Configuration const &,unsigned int);
        int Render(CMatrix const &,AnimInfo *,Configuration const &,std::vector<DSurfaceMaterial *,std::allocator<DSurfaceMaterial *> > const &);
        int GetFps(int) const ;
        void SetFps(int,short);
        void RenderLoadPoints(CMatrix const &,AnimInfo *,Configuration const &);
        DSurfaceMaterial & GetMeshMaterial(unsigned int,unsigned int);
        void AddTexture(unsigned int,unsigned int,CStr const &,DRAFT_TextureType,unsigned int);
        DCollisionData const & GetCollisionTrimesh() const ;
        int GetBoneMatrixByName(CStr const &,CMatrix &,bool) const ;
        void FromCfgNum(Configuration &) const ;
        char const * GetName() const ;
        Aabb & GetAabb();
        int Update(AnimInfo *,bool,Configuration *);
        void UpdateCubemap();
        unsigned int GetNumGeoms() const ;
        unsigned int GetCfgSize() const ;
        MeshesGroup const & GetGroup(unsigned int) const ;
        DSurfaceMaterial & GetMaterial(unsigned int,unsigned int);
        rend::IEffect * ApplyMaterial(DSurfaceMaterial &);
        rend::IEffect * ApplyMaterial(unsigned int);
        bool Save(CStr const &);
        unsigned int GetNumBones() const ;
        void UpdateVertices(AnimInfo *,bool,Configuration *);
        void RenderMesh(unsigned int,AnimInfo *,rend::IEffect *);
        void RenderCollisions(CMatrix const &,unsigned int);
        unsigned int GetNumMaterials() const ;
        unsigned int GetNumHierGeoms() const ;
        bool SaveGAM(CStr const &);
        static void __fastcall SetCubeMapTexName(CStr const &);
        void LoadSkin(unsigned int);
        void SetNextForAnimation(ActionType,int);
        LoadSkins const & GetLoadedSkins();

    protected:
        void CalculateCfgSize();
        bool Convert(CStr const &,bool,std::vector<DRAFT_Bone,std::allocator<DRAFT_Bone> > const &,std::vector<DMesh,std::allocator<DMesh> > const &,std::vector<DAnimation,std::allocator<DAnimation> > const &);
        void MatrixForBone(AnimInfo *,int,int);
        void NewEffect(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,rend::IEffect * &);
        void CreateVariants();
        void SetTexture(CStr const &,DRAFT_TextureType,rend::TexHandle &);
        void ReadMaterial(DSurfaceMaterial &,unsigned char * &,bool);
        void VertsForSkinmesh(AnimInfo *,Mesh &,void *,bool);
        void DrawBones(AnimInfo *);
        void CheckConfigurations();
        CStr DefinePathToTexture(CStr const &);

    private:
        static int __cdecl sortBonesFunc(void const *,void const *);
        CMatrix *m_initialBoneInvMatrices = nullptr;
        ModelInfo m_header;
        std::string m_Name;
        std::string m_PathToFile;
        bool m_composite = false;
        bool m_passable = false;
        Bone *m_boneInitialPos = nullptr;
        CMatrix m_Dummy;
        Mesh *m_meshes = nullptr;
        unsigned int m_numMeshes = 0;
        bool m_bVerification = false;
        Animation *m_animations = nullptr;
        __int16 m_animRemap[32] = {0};
        DCollisionData m_Collision;
        std::vector<DRAFT_Geom> m_Geoms;
        std::vector<DRAFT_HierGeom> m_HierGeoms;
        std::map<unsigned int,DRAFT_BoneBounds> m_BonesBounds;
        std::vector<std::vector<DSurfaceMaterial>> m_Skins;
        LoadSkins m_loadSkins;
        bool m_hasCubemap = false;
        std::vector<MeshesGroup> m_MhGroups;
        unsigned int m_cfgSize;
        Aabb m_box;
    };

    class Configuration
    {
    private:
        unsigned int m_num;
        std::vector<AnimatedModel::Mesh*> m_meshes;
        std::vector<unsigned char> m_groupVariants;
    };

    class AnimInfo
    {
    public:
        bool IsAnimation(ActionType);
        void CreateCopyMesh(int&, void**&, int*&, unsigned short**&, int*&, bool*&, CMatrix**&, std::vector<rend::VertexType, std::allocator<rend::VertexType> >&, std::vector<unsigned int, std::allocator<unsigned int> >&, int*);
        void SetEmpty();
        void InterpolateBones(int);
        int SetAnimationIdx(int);
        AnimatedModel::Mesh const& GetMesh(unsigned int) const;
        void CreateFor(AnimatedModel*);
        void SetBoneCurMatrix(unsigned int, CMatrix const&);
        CMatrix const& GetCurrentLoadpointMatrix(int) const;
        int SetCurFrame(float);
        void Release();
        AnimatedModel::Animation const* GetCurAnimation() const;
        int GetStickToLastFrame() const;
        void RemoveCopyMesh(void**&, int*&, unsigned short**&, int*&, CMatrix**&);
        bool IsEmpty();
        int& CurAnimFrame();
        ~AnimInfo();
        AnimInfo();
        int SetAnimation(ActionType);
        void MoveFrame(unsigned int);
        BoneAnim& GetBoneAnim(unsigned int);

    private:
        Aabb m_curBox;
        bool m_Empty;
        BoneAnim* m_bonesAnim;
        AnimatedModel* m_forModel;
        AnimatedModel::Animation* m_curAnimation;
        int m_curAnimFrame;
        int m_timeOutToNextFrame;
        std::vector<void*> m_meshesVerts;
        std::vector<rend::VertexType> m_VertTypes;
        std::vector<unsigned int> m_VertTypesSize;
        int m_lastInterpolationUpdate;
        int m_stickToLastFrame;
        int m_blendFramesNum;
        bool m_isBlending;
        BoneAnim* m_bonesAnimPrev;
        AnimatedModel::Animation* m_curAnimationPrev;
        int m_curAnimFramePrev;
        int m_timeOutToNextFramePrev;
        int m_lastInterpolationUpdatePrev;
        int m_stickToLastFramePrev;
    };
}
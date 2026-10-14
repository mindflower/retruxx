#pragma once
#include <cstdint>
#include <draftstructures.h>
#include <math/aabb.h>
#include <math/matrix.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include "retruxx/common.h"

namespace ai
{
    class CompositeObj;
    class JointedObj;
    class SubmarinePrototypeInfo;
}


enum ActionType : int32_t
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

    struct LoadSkins
    {
        bool loadAllSkins = true;
        char Padding_101[3];
        retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> > loadSkins;
    };

    struct BoneAnim
    {
        int m_lastUpdatedFrame;
        CMatrix m_curMatrix;
        Quaternion m_rotation;
        CVector m_translation;
        int m_parentIdx;
    };

    struct MeshesGroup
    {
        retruxx::string Name;
        retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > MeshesId;
        int VisibleAtOnceMax;
        int VisibleAtOnceMin;
        retruxx::vector<retruxx::vector<unsigned int, retruxx::allocator<unsigned int> >, retruxx::allocator<retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > > > m_variants;
        void GetNextVariant(retruxx::vector<unsigned int, retruxx::allocator<unsigned int> >& variant);
    };

    struct AnimAction
    {
        char* m_name;
        ActionType m_action;
    };

    AnimAction* GetAnimActions();

    // Pushes the shared per-frame lighting / fog / time constants into a shader,
    // skipping any parameter the current technique does not use.
    void SetShaderParams(m3d::rend::IEffect* pShader);

    class AnimatedModel
    {
        friend class AnimatedModelsServer;
        // ai::CompositeObj takes a model apart into one physical piece per hierarchy geom.
        friend class ai::CompositeObj;
        // SubmarinePrototypeInfo::RefreshFromXml retunes the platform animation.
        friend class ai::SubmarinePrototypeInfo;
        friend class AnimInfo;
    public:
        struct HierarchyChange
        {
            short ownIdx;
            char Padding_244[2];
            m3d::DRAFT_Change changeType;
            short newParentIdx;
        };

        struct ModelInfo
        {
            short m_numNodes = 0;
            short m_numTriMeshes = 0;
            short m_numSkinMeshes = 0;
            short m_numStaticMeshes = 0;
            short m_numAnimations = 0;
        };

        struct AnimationTransform
        {
            short idx;
            char Padding_245[2];
            float tx;
            float ty;
            float tz;
            float qx;
            float qy;
            float qz;
            float qw;
        };

        struct Influences
        {
            unsigned short m_numBones;

            struct Infl
            {
                short m_boneIdx;
                char Padding_247[2];
                float m_boneWeight;
                CVector m_offsetVec;
                CVector m_offsetNormal;
            };

            char Padding_248[2];
            m3d::AnimatedModel::Influences::Infl m_influences[4];
        };

        struct Mesh
        {
            Mesh(const m3d::AnimatedModel::Mesh&);
            Mesh();
            ~Mesh();
            char m_meshName[40];
            int m_meshType;
            int m_numNode;
            unsigned int meshId;
            unsigned int groupId;
            int m_MaterialNumber;
            retruxx::vector<retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> >, retruxx::allocator<retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> > > >* m_pModelSkins;
            retruxx::vector<m3d::DRAFT_VertexComponent, retruxx::allocator<m3d::DRAFT_VertexComponent> > m_VCHs;
            unsigned int m_VertexTypeSize;
            m3d::rend::VertexType m_VertexType;
            int m_numVertices;
            void* m_verts;
            int m_numDrawVerts;
            void* m_drawVerts;
            m3d::AnimatedModel::Influences* m_vertsInfluences;
            int* m_vertsRemap;
            unsigned short* m_tris;
            int m_numFaces;
            unsigned short* m_drawIndices;
            int m_numDrawIndices;
            int m_lastFrameUpdated;
            m3d::rend::VbPoolField m_VbPoolField;
            m3d::rend::IbPoolField m_IbPoolField;
            m3d::DSurfaceMaterial& GetMaterial(unsigned int SkinNumber) const;
            int m_numFacesWelded;
            unsigned short* m_trisWelded;
            int m_numVertsWelded;
            CVector* m_vertsWelded;
            CVector* m_faceNormals;
            void ComputeShadowsRelatedStuff();
        };

        struct Bone
        {
            Bone(const m3d::AnimatedModel::Bone& __that);
            Bone();
            char m_boneName[40];
            int m_ownIdx;
            int m_parentIdx;
            CVector m_translation0;
            Quaternion m_quaternion0;
        };

    private:
        CMatrix* m_initialBoneInvMatrices;
        static int __cdecl sortBonesFunc(const void* a, const void* b);

    public:
        struct Animation
        {
            Animation();
            ~Animation();
            short m_numNodes;
            short m_numFrames;
            short m_fps;
            short m_numChanges;
            short m_nextAnimation;
            char Padding_249[2];
            ActionType m_action;
            char m_name[25];
            char Padding_250[3];
            m3d::AnimatedModel::HierarchyChange* m_hierChanges;
            m3d::AnimatedModel::AnimationTransform* m_nodesPositions;
        };

    protected:
        static inline retruxx::map<CStr, CStr, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, CStr> > > m_textureFiles;
        static inline CStr m_cubemapTexName;
        static inline CStr m_cubemapTexPath;
        m3d::AnimatedModel::ModelInfo m_header;
        char Padding_251[2];
        retruxx::string m_Name;
        retruxx::string m_PathToFile;
        bool m_composite;
        bool m_passable;
        char Padding_252[2];
        m3d::AnimatedModel::Bone* m_boneInitialPos;
        CMatrix m_Dummy;
        m3d::AnimatedModel::Mesh* m_meshes;
        unsigned int m_numMeshes;
        bool m_bVerification;
        char Padding_253[3];
        m3d::AnimatedModel::Animation* m_animations;
        short m_animRemap[32];
        m3d::DCollisionData m_Collision;
        retruxx::vector<m3d::DRAFT_Geom, retruxx::allocator<m3d::DRAFT_Geom> > m_Geoms;
        retruxx::vector<m3d::DRAFT_HierGeom, retruxx::allocator<m3d::DRAFT_HierGeom> > m_HierGeoms;
        retruxx::map<unsigned int, m3d::DRAFT_BoneBounds, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, m3d::DRAFT_BoneBounds> > > m_BonesBounds;
        retruxx::vector<retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> >, retruxx::allocator<retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> > > > m_Skins;
        m3d::LoadSkins m_loadSkins;
        bool m_hasCubemap;
        char Padding_254[3];
        retruxx::vector<m3d::MeshesGroup, retruxx::allocator<m3d::MeshesGroup> > m_MhGroups;
        unsigned int m_cfgSize;
        CStr DefinePathToTexture(const CStr& fileName);
        void SetTexture(const CStr& path, m3d::DRAFT_TextureType type, m3d::rend::TexHandle& texHandle);
        void NewEffect(const retruxx::string& name, m3d::rend::IEffect*& shader);
        void ReadMaterial(m3d::DSurfaceMaterial& SurfaceMaterial, unsigned char*& Data, bool load);
        void MatrixForBone(m3d::AnimInfo* ai, int curFrame, int bIdx);
        void DrawBones(m3d::AnimInfo* ai);
        void VertsForSkinmesh(m3d::AnimInfo* ai, m3d::AnimatedModel::Mesh& mh, void* dstVerts, bool OnlyXYZN);
        bool Convert(const CStr& fname, const bool forcenextanimation, const retruxx::vector<m3d::DRAFT_Bone, retruxx::allocator<m3d::DRAFT_Bone> >& Bones, const retruxx::vector<m3d::DMesh, retruxx::allocator<m3d::DMesh> >& Meshes, const retruxx::vector<m3d::DAnimation, retruxx::allocator<m3d::DAnimation> >& Animations);
        void CreateVariants();
        void CalculateCfgSize();
        void CheckConfigurations();

    public:
        AnimatedModel(const m3d::AnimatedModel&);
        AnimatedModel();
        ~AnimatedModel();
        static void __fastcall CreateTexFileMapping();
        static void __fastcall SetCubeMapTexName(const CStr& texName);
        static void __fastcall UpdateTexturesFilter();
        void SetVerification(bool);
        bool Load(const CStr& FileName, const bool bForceNextAnimation);
        bool LoadSAM(const CStr& FileName, const bool ForceNextAnimation);
        bool LoadGAM(const CStr& FileName, const bool bForceNextAnimation);
        bool Save(const CStr& FileName);
        bool SaveGAM(const CStr& FileName);
        int Update(m3d::AnimInfo* ai, bool NotFirstTime, m3d::Configuration* cfg);
        void UpdateVertices(m3d::AnimInfo* ai, bool NotFirstTime, m3d::Configuration* cfg);
        int Render(const CMatrix& matT, m3d::AnimInfo* ai, const m3d::Configuration& cfg, unsigned int SkinNum);
        int Render(const CMatrix& matT, m3d::AnimInfo* ai, const m3d::Configuration& cfg, const retruxx::vector<m3d::DSurfaceMaterial*, retruxx::allocator<m3d::DSurfaceMaterial*> >& MeshMaterials);
        int Render(const CMatrix& matT, m3d::AnimInfo* ai, unsigned int SkinNum, bool setShaderParams);
        void RenderMesh(unsigned int cc, m3d::AnimInfo* ai, m3d::rend::IEffect* Shader);
        void RenderNormals(const CMatrix& matT, m3d::AnimInfo* ai);
        void RenderCollisions(const CMatrix& matT, unsigned int clr);
        void RenderLoadPoints(const CMatrix& matT, m3d::AnimInfo* ai, const m3d::Configuration& cfg);
        void RenderHierGeoms(const CMatrix& matT, m3d::AnimInfo* ai, unsigned int clr);
        m3d::rend::IEffect* ApplyMaterial(unsigned int meshnumber);
        m3d::rend::IEffect* ApplyMaterial(m3d::DSurfaceMaterial& SurfaceMaterial);
        void SetNextForAnimation(ActionType Action, int NextAction);
        m3d::AnimatedModel::ModelInfo& GetHeader();
        int GetFps(int anim) const;
        void SetFps(int anim, short fps);
        int GetFrames(int anim, int pol) const;
        const char* GetName() const;
        const char* GetPath() const;
        bool IsComposite() const;
        void SetComposite(bool);
        unsigned int GetNumSkins() const;
        retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> >& GetSkin(unsigned int);
        unsigned int GetNumMaterials() const;
        m3d::DSurfaceMaterial& GetMaterial(unsigned int SkinNumber, unsigned int MatNum);
        m3d::rend::TexHandle GetTexHandle(unsigned int SkinNumber, unsigned int MatNum, unsigned int TexNum) const;
        Aabb& GetAabb();
        void SetSkinsToLoad(const m3d::LoadSkins& skinsToLoad);
        void ReloadSkins(const m3d::LoadSkins& skinsToLoad);
        const m3d::LoadSkins& GetLoadedSkins();
        void UnloadSkin(unsigned int j);
        void LoadSkin(unsigned int j);
        unsigned int GetNumGeoms() const;
        unsigned int GetNumHierGeoms() const;
        const m3d::DRAFT_HierGeom* GetHierGeom(unsigned int num) const;
        const m3d::DRAFT_Geom* GetGeom(unsigned int num) const;
        const m3d::DCollisionData& GetCollisionTrimesh() const;
        void AddSkin(unsigned int CopyFrom);
        void DeleteSkin(unsigned int SkinNumber);
        void SetMaterial(unsigned int, unsigned int, const m3d::DSurfaceMaterial&);
        int ChangeShader(const CStr& Name, unsigned int SkinNumber, unsigned int i);
        int ChangeTexture(const CStr& name, unsigned int SkinNumber, unsigned int MatNum, unsigned int TexNum);
        void AddTexture(unsigned int SkinNumber, unsigned int MatNum, const CStr& Name, m3d::DRAFT_TextureType TexType, unsigned int UVSet);
        void UpdateCubemap();
        const m3d::AnimatedModel::Mesh& GetMesh(unsigned int MeshNum) const;
        m3d::AnimatedModel::Mesh& GetMesh(unsigned int MeshNum);
        m3d::DSurfaceMaterial& GetMeshMaterial(unsigned int MeshNum, unsigned int SkinNumber);
        unsigned int GetNumMeshes() const;
        int GetLoadPointIdByName(const char* lpName) const;
        int GetBoneMatrixByName(const CStr& boneName, CMatrix& res, bool theLastOneOnly) const;
        CMatrix GetBoneMatrix(int i) const;
        unsigned int GetNumBones() const;
        m3d::AnimatedModel::Bone& GetBone(unsigned int N) const;
        const m3d::DRAFT_BoneBounds& GetBoneBounds(unsigned int N);
        unsigned int GetCfgSize() const;
        unsigned int GetGroupsNum() const;
        const m3d::MeshesGroup& GetGroup(unsigned int Num) const;
        void FromCfgNum(m3d::Configuration& cfg) const;
        void FromGroupVariants(m3d::Configuration& cfg) const;
        void CalculateMeshes(m3d::Configuration& cfg) const;
        unsigned int GetNumAnimations() const;
        const m3d::AnimatedModel::Animation& GetAnimation(unsigned int) const;
        bool bIsPassable() const;
        Aabb m_box;
    };

    struct Configuration
    {
        unsigned int m_num = 0;
        retruxx::vector<m3d::AnimatedModel::Mesh*, retruxx::allocator<m3d::AnimatedModel::Mesh*> > m_meshes;
        retruxx::vector<unsigned char, retruxx::allocator<unsigned char> > m_groupVariants;
    };

    class AnimInfo
    {
        friend class AnimatedModelsServer;
        friend class AnimatedModel;
        friend class ::ai::JointedObj;

    public:
        AnimInfo(const m3d::AnimInfo&);
        AnimInfo();
        ~AnimInfo();
        void Release();
        void CreateFor(m3d::AnimatedModel* am);
        bool IsLowFrames();
        bool IsEmpty();
        void SetEmpty();
        int SetAnimation(ActionType action);
        int SetAnimationIdx(int num);
        int& CurAnimFrame();
        int SetCurFrame(float progress);
        bool IsAnimation(ActionType action);
        void MoveFrame(unsigned int dti);
        void InterpolateBones(int curUpdateFrame);
        void CreateCopyMesh(int& numMesh, void**& verts, int*& numVerts, unsigned short**& indxs, int*& numIndxs, bool*& strips, CMatrix**& mat, retruxx::vector<enum m3d::rend::VertexType, retruxx::allocator<enum m3d::rend::VertexType> >& VertexTypes, retruxx::vector<unsigned int, retruxx::allocator<unsigned int> >& VertexTypeSizes, int* numSkinMesh);
        void RemoveCopyMesh(void**& verts, int*& numVerts, unsigned short**& indxs, int*& numIndxs, CMatrix**& mat);
        const CMatrix& GetCurrentLoadpointMatrix(int lpId) const;
        m3d::BoneAnim& GetBoneAnim(unsigned int j);
        void SetBoneCurMatrix(unsigned int j, const CMatrix& m);
        Aabb m_curBox;
        const m3d::AnimatedModel::Animation* GetCurAnimation() const;
        int GetStickToLastFrame() const;
        const m3d::AnimatedModel::Mesh& GetMesh(unsigned int MeshNum) const;

    protected:
        bool m_Empty;
        char Padding_270[3];
        m3d::BoneAnim* m_bonesAnim;
        m3d::AnimatedModel* m_forModel;
        m3d::AnimatedModel::Animation* m_curAnimation;
        int m_curAnimFrame;
        int m_timeOutToNextFrame;
        retruxx::vector<void*, retruxx::allocator<void*> > m_meshesVerts;
        retruxx::vector<enum m3d::rend::VertexType, retruxx::allocator<enum m3d::rend::VertexType> > m_VertTypes;
        retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > m_VertTypesSize;
        int m_lastInterpolationUpdate;
        int m_stickToLastFrame;
        int m_blendFramesNum;
        bool m_isBlending;
        char Padding_271[3];
        m3d::BoneAnim* m_bonesAnimPrev;
        m3d::AnimatedModel::Animation* m_curAnimationPrev;
        int m_curAnimFramePrev;
        int m_timeOutToNextFramePrev;
        int m_lastInterpolationUpdatePrev;
        int m_stickToLastFramePrev;
    };

    ActionType GetActionByName(char const *);
}

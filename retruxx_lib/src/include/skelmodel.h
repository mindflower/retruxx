#pragma once
#include <draftstructures.h>
#include <math/aabb.h>
#include <math/matrix.h>
#include <math/quaternion.h>
#include <thirdparty/containers.h>

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

    struct LoadSkins
    {
        /* 0x0000 */ bool loadAllSkins = true;
        /* 0x0001 */ char Padding_101[3];
        /* 0x0004 */ retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> > loadSkins;
    }; /* size: 0x0010 */

    static_assert(sizeof(LoadSkins) == 0x0010);

    struct BoneAnim
    {
        /* 0x0000 */ int m_lastUpdatedFrame;
        /* 0x0004 */ CMatrix m_curMatrix;
        /* 0x0044 */ Quaternion m_rotation;
        /* 0x0054 */ CVector m_translation;
        /* 0x0060 */ int m_parentIdx;
    }; /* size: 0x0064 */

    static_assert(sizeof(BoneAnim) == 0x0064);

    struct MeshesGroup
    {
        /* 0x0000 */ retruxx::string Name;
        /* 0x001c */ retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > MeshesId;
        /* 0x002c */ int VisibleAtOnceMax;
        /* 0x0030 */ int VisibleAtOnceMin;
        /* 0x0034 */ retruxx::vector<retruxx::vector<unsigned int, retruxx::allocator<unsigned int> >, retruxx::allocator<retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > > > m_variants;
        void GetNextVariant(retruxx::vector<unsigned int, retruxx::allocator<unsigned int> >& variant);
    }; /* size: 0x0044 */

    static_assert(sizeof(MeshesGroup) == 0x0044);

    class AnimatedModel
    {
    public:
        struct HierarchyChange
        {
            /* 0x0000 */ short ownIdx;
            /* 0x0002 */ char Padding_244[2];
            /* 0x0004 */ m3d::DRAFT_Change changeType;
            /* 0x0008 */ short newParentIdx;
        }; /* size: 0x000c */

        struct ModelInfo
        {
            /* 0x0000 */ short m_numNodes;
            /* 0x0002 */ short m_numTriMeshes;
            /* 0x0004 */ short m_numSkinMeshes;
            /* 0x0006 */ short m_numStaticMeshes;
            /* 0x0008 */ short m_numAnimations;
        }; /* size: 0x000a */

        struct AnimationTransform
        {
            /* 0x0000 */ short idx;
            /* 0x0002 */ char Padding_245[2];
            /* 0x0004 */ float tx;
            /* 0x0008 */ float ty;
            /* 0x000c */ float tz;
            /* 0x0010 */ float qx;
            /* 0x0014 */ float qy;
            /* 0x0018 */ float qz;
            /* 0x001c */ float qw;
        }; /* size: 0x0020 */

        struct Influences
        {
            /* 0x0000 */ unsigned short m_numBones;

            struct Infl
            {
                /* 0x0000 */ short m_boneIdx;
                /* 0x0002 */ char Padding_247[2];
                /* 0x0004 */ float m_boneWeight;
                /* 0x0008 */ CVector m_offsetVec;
                /* 0x0014 */ CVector m_offsetNormal;
            }; /* size: 0x0020 */

            /* 0x0002 */ char Padding_248[2];
            /* 0x0004 */ m3d::AnimatedModel::Influences::Infl m_influences[4];
        }; /* size: 0x0084 */

        struct Mesh
        {
            Mesh(const m3d::AnimatedModel::Mesh&);
            Mesh();
            ~Mesh();
            /* 0x0000 */ char m_meshName[40];
            /* 0x0028 */ int m_meshType;
            /* 0x002c */ int m_numNode;
            /* 0x0030 */ unsigned int meshId;
            /* 0x0034 */ unsigned int groupId;
            /* 0x0038 */ int m_MaterialNumber;
            /* 0x003c */ retruxx::vector<retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> >, retruxx::allocator<retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> > > >* m_pModelSkins;
            /* 0x0040 */ retruxx::vector<m3d::DRAFT_VertexComponent, retruxx::allocator<m3d::DRAFT_VertexComponent> > m_VCHs;
            /* 0x0050 */ unsigned int m_VertexTypeSize;
            /* 0x0054 */ m3d::rend::VertexType m_VertexType;
            /* 0x0058 */ int m_numVertices;
            /* 0x005c */ void* m_verts;
            /* 0x0060 */ int m_numDrawVerts;
            /* 0x0064 */ void* m_drawVerts;
            /* 0x0068 */ m3d::AnimatedModel::Influences* m_vertsInfluences;
            /* 0x006c */ int* m_vertsRemap;
            /* 0x0070 */ unsigned short* m_tris;
            /* 0x0074 */ int m_numFaces;
            /* 0x0078 */ unsigned short* m_drawIndices;
            /* 0x007c */ int m_numDrawIndices;
            /* 0x0080 */ int m_lastFrameUpdated;
            /* 0x0084 */ m3d::rend::VbPoolField m_VbPoolField;
            /* 0x0098 */ m3d::rend::IbPoolField m_IbPoolField;
            m3d::DSurfaceMaterial& GetMaterial(unsigned int SkinNumber) const;
            /* 0x00a8 */ int m_numFacesWelded;
            /* 0x00ac */ unsigned short* m_trisWelded;
            /* 0x00b0 */ int m_numVertsWelded;
            /* 0x00b4 */ CVector* m_vertsWelded;
            /* 0x00b8 */ CVector* m_faceNormals;
            void ComputeShadowsRelatedStuff();
        }; /* size: 0x00bc */

        struct Bone
        {
            Bone(const m3d::AnimatedModel::Bone& __that);
            Bone();
            /* 0x0000 */ char m_boneName[40];
            /* 0x0028 */ int m_ownIdx;
            /* 0x002c */ int m_parentIdx;
            /* 0x0030 */ CVector m_translation0;
            /* 0x003c */ Quaternion m_quaternion0;
        }; /* size: 0x004c */

    private:
        /* 0x0000 */ CMatrix* m_initialBoneInvMatrices;
        static int __cdecl sortBonesFunc(const void* a, const void* b);

    public:
        struct Animation
        {
            Animation();
            ~Animation();
            /* 0x0000 */ short m_numNodes;
            /* 0x0002 */ short m_numFrames;
            /* 0x0004 */ short m_fps;
            /* 0x0006 */ short m_numChanges;
            /* 0x0008 */ short m_nextAnimation;
            /* 0x000a */ char Padding_249[2];
            /* 0x000c */ ActionType m_action;
            /* 0x0010 */ char m_name[25];
            /* 0x0029 */ char Padding_250[3];
            /* 0x002c */ m3d::AnimatedModel::HierarchyChange* m_hierChanges;
            /* 0x0030 */ m3d::AnimatedModel::AnimationTransform* m_nodesPositions;
        }; /* size: 0x0034 */

    protected:
        static inline retruxx::map<CStr, CStr, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, CStr> > > m_textureFiles;
        static inline CStr m_cubemapTexName;
        static inline CStr m_cubemapTexPath;
        /* 0x0004 */ m3d::AnimatedModel::ModelInfo m_header;
        /* 0x000e */ char Padding_251[2];
        /* 0x0010 */ retruxx::string m_Name;
        /* 0x002c */ retruxx::string m_PathToFile;
        /* 0x0048 */ bool m_composite;
        /* 0x0049 */ bool m_passable;
        /* 0x004a */ char Padding_252[2];
        /* 0x004c */ m3d::AnimatedModel::Bone* m_boneInitialPos;
        /* 0x0050 */ CMatrix m_Dummy;
        /* 0x0090 */ m3d::AnimatedModel::Mesh* m_meshes;
        /* 0x0094 */ unsigned int m_numMeshes;
        /* 0x0098 */ bool m_bVerification;
        /* 0x0099 */ char Padding_253[3];
        /* 0x009c */ m3d::AnimatedModel::Animation* m_animations;
        /* 0x00a0 */ short m_animRemap[32];
        /* 0x00e0 */ m3d::DCollisionData m_Collision;
        /* 0x0100 */ retruxx::vector<m3d::DRAFT_Geom, retruxx::allocator<m3d::DRAFT_Geom> > m_Geoms;
        /* 0x0110 */ retruxx::vector<m3d::DRAFT_HierGeom, retruxx::allocator<m3d::DRAFT_HierGeom> > m_HierGeoms;
        /* 0x0120 */ retruxx::map<unsigned int, m3d::DRAFT_BoneBounds, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, m3d::DRAFT_BoneBounds> > > m_BonesBounds;
        /* 0x012c */ retruxx::vector<retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> >, retruxx::allocator<retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial> > > > m_Skins;
        /* 0x013c */ m3d::LoadSkins m_loadSkins;
        /* 0x014c */ bool m_hasCubemap;
        /* 0x014d */ char Padding_254[3];
        /* 0x0150 */ retruxx::vector<m3d::MeshesGroup, retruxx::allocator<m3d::MeshesGroup> > m_MhGroups;
        /* 0x0160 */ unsigned int m_cfgSize;
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
        /* 0x0164 */ Aabb m_box;
    }; /* size: 0x017c */

    static_assert(sizeof(AnimatedModel) == 0x017c);

    struct Configuration
    {
        /* 0x0000 */ unsigned int m_num = 0;
        /* 0x0004 */ retruxx::vector<m3d::AnimatedModel::Mesh*, retruxx::allocator<m3d::AnimatedModel::Mesh*> > m_meshes;
        /* 0x0014 */ retruxx::vector<unsigned char, retruxx::allocator<unsigned char> > m_groupVariants;
    }; /* size: 0x0024 */

    static_assert(sizeof(Configuration) == 0x0024);

    class AnimInfo
    {
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
        /* 0x0000 */ Aabb m_curBox;
        const m3d::AnimatedModel::Animation* GetCurAnimation() const;
        int GetStickToLastFrame() const;
        const m3d::AnimatedModel::Mesh& GetMesh(unsigned int MeshNum) const;

    protected:
        /* 0x0018 */ bool m_Empty;
        /* 0x0019 */ char Padding_270[3];
        /* 0x001c */ m3d::BoneAnim* m_bonesAnim;
        /* 0x0020 */ m3d::AnimatedModel* m_forModel;
        /* 0x0024 */ m3d::AnimatedModel::Animation* m_curAnimation;
        /* 0x0028 */ int m_curAnimFrame;
        /* 0x002c */ int m_timeOutToNextFrame;
        /* 0x0030 */ retruxx::vector<void*, retruxx::allocator<void*> > m_meshesVerts;
        /* 0x0040 */ retruxx::vector<enum m3d::rend::VertexType, retruxx::allocator<enum m3d::rend::VertexType> > m_VertTypes;
        /* 0x0050 */ retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > m_VertTypesSize;
        /* 0x0060 */ int m_lastInterpolationUpdate;
        /* 0x0064 */ int m_stickToLastFrame;
        /* 0x0068 */ int m_blendFramesNum;
        /* 0x006c */ bool m_isBlending;
        /* 0x006d */ char Padding_271[3];
        /* 0x0070 */ m3d::BoneAnim* m_bonesAnimPrev;
        /* 0x0074 */ m3d::AnimatedModel::Animation* m_curAnimationPrev;
        /* 0x0078 */ int m_curAnimFramePrev;
        /* 0x007c */ int m_timeOutToNextFramePrev;
        /* 0x0080 */ int m_lastInterpolationUpdatePrev;
        /* 0x0084 */ int m_stickToLastFramePrev;
    }; /* size: 0x0088 */

    static_assert(sizeof(AnimInfo) == 0x0088);
}
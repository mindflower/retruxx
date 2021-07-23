#pragma once

namespace m3d
{
    class AnimatedModel
    {
    public:
        DRAFT_HierGeom const * GetHierGeom(unsigned int) const ;
        static void __fastcall CreateTexFileMapping();
        AnimatedModel();
        struct Mesh & GetMesh(unsigned int);
        struct Mesh const & GetMesh(unsigned int) const ;
        DRAFT_BoneBounds const & GetBoneBounds(unsigned int);
        struct CMatrix GetBoneMatrix(int) const ;
        void SetSkinsToLoad(LoadSkins const &);
        bool LoadGAM(class CStr const &,bool);
        bool LoadSAM(class CStr const &,bool);
        struct Bone & GetBone(unsigned int) const ;
        unsigned int GetNumMeshes() const ;
        void DeleteSkin(unsigned int);
        void UnloadSkin(unsigned int);
        static void __fastcall UpdateTexturesFilter();
        unsigned int GetNumSkins() const ;
        ~AnimatedModel();
        rend::TexHandle GetTexHandle(unsigned int,unsigned int,unsigned int) const ;
        bool Load(class CStr const &,bool);
        unsigned int GetGroupsNum() const ;
        int GetFrames(int,int) const ;
        void RenderNormals(struct CMatrix const &,AnimInfo *);
        void AddSkin(unsigned int);
        int GetLoadPointIdByName(char const *) const ;
        DRAFT_Geom const * GetGeom(unsigned int) const ;
        int ChangeShader(class CStr const &,unsigned int,unsigned int);
        bool bIsPassable() const ;
        void FromGroupVariants(Configuration &) const ;
        void ReloadSkins(LoadSkins const &);
        void CalculateMeshes(Configuration &) const ;
        void RenderHierGeoms(struct CMatrix const &,AnimInfo *,unsigned int);
        int ChangeTexture(class CStr const &,unsigned int,unsigned int,unsigned int);
        int Render(struct CMatrix const &,AnimInfo *,unsigned int,bool);
        int Render(struct CMatrix const &,AnimInfo *,Configuration const &,unsigned int);
        int Render(struct CMatrix const &,AnimInfo *,Configuration const &,class std::vector<DSurfaceMaterial *,class std::allocator<DSurfaceMaterial *> > const &);
        int GetFps(int) const ;
        void SetFps(int,short);
        void RenderLoadPoints(struct CMatrix const &,AnimInfo *,Configuration const &);
        DSurfaceMaterial & GetMeshMaterial(unsigned int,unsigned int);
        void AddTexture(unsigned int,unsigned int,class CStr const &,DRAFT_TextureType,unsigned int);
        DCollisionData const & GetCollisionTrimesh() const ;
        int GetBoneMatrixByName(class CStr const &,struct CMatrix &,bool) const ;
        void FromCfgNum(Configuration &) const ;
        char const * GetName() const ;
        class Aabb & GetAabb();
        int Update(AnimInfo *,bool,Configuration *);
        void UpdateCubemap();
        unsigned int GetNumGeoms() const ;
        unsigned int GetCfgSize() const ;
        MeshesGroup const & GetGroup(unsigned int) const ;
        DSurfaceMaterial & GetMaterial(unsigned int,unsigned int);
        rend::IEffect * ApplyMaterial(DSurfaceMaterial &);
        rend::IEffect * ApplyMaterial(unsigned int);
        bool Save(class CStr const &);
        unsigned int GetNumBones() const ;
        void UpdateVertices(AnimInfo *,bool,Configuration *);
        void RenderMesh(unsigned int,AnimInfo *,rend::IEffect *);
        void RenderCollisions(struct CMatrix const &,unsigned int);
        unsigned int GetNumMaterials() const ;
        unsigned int GetNumHierGeoms() const ;
        bool SaveGAM(class CStr const &);
        static void __fastcall SetCubeMapTexName(class CStr const &);
        void LoadSkin(unsigned int);
        void SetNextForAnimation(enum ActionType,int);
        LoadSkins const & GetLoadedSkins();
    protected:
        void CalculateCfgSize();
        bool Convert(class CStr const &,bool,class std::vector<DRAFT_Bone,class std::allocator<DRAFT_Bone> > const &,class std::vector<DMesh,class std::allocator<DMesh> > const &,class std::vector<DAnimation,class std::allocator<DAnimation> > const &);
        void MatrixForBone(AnimInfo *,int,int);
        void NewEffect(class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const &,rend::IEffect * &);
        void CreateVariants();
        void SetTexture(class CStr const &,DRAFT_TextureType,rend::TexHandle &);
        void ReadMaterial(DSurfaceMaterial &,unsigned char * &,bool);
        void VertsForSkinmesh(AnimInfo *,struct Mesh &,void *,bool);
        void DrawBones(AnimInfo *);
        void CheckConfigurations();
        class CStr DefinePathToTexture(class CStr const &);
    private:
        std::_Copy_opt<Mesh * *,Mesh * *>(Mesh * *,Mesh * *,Mesh * *,std::_Scalar_ptr_iterator_tag);
        static int __cdecl sortBonesFunc(void const *,void const *);
        std::_Destroy<Mesh *>(Mesh * *);
        std::copy<Mesh * *,Mesh * *>(Mesh * *,Mesh * *,Mesh * *);
        std::_Allocate<Mesh *>(uint,Mesh * *);
        std::copy_backward<Mesh * *,Mesh * *>(Mesh * *,Mesh * *,Mesh * *);
        std::allocator<Mesh *>::allocator<Mesh *>();
        std::allocator<Mesh *>::allocator<Mesh *>();
        std::_Ptr_cat<Mesh>(Mesh * *,Mesh * *);
        std::_Copy_backward_opt<Mesh * *,Mesh * *>(Mesh * *,Mesh * *,Mesh * *,std::_Scalar_ptr_iterator_tag);
        std::_Construct<Mesh *,Mesh *>(Mesh * *,Mesh * const &);
        std::fill<Mesh * *,Mesh *>(Mesh * *,Mesh * *,Mesh * const &);
        std::_Uninit_copy<Mesh *,Mesh *>(Mesh * *,Mesh * *,Mesh * *,std::allocator<Mesh *> &,std::_Scalar_ptr_iterator_tag);
        CMatrix *m_initialBoneInvMatrices;
        AnimatedModel::ModelInfo m_header;
        std::string m_Name;
        std::string m_PathToFile;
        bool m_composite;
        bool m_passable;
        AnimatedModel::Bone *m_boneInitialPos;
        CMatrix m_Dummy;
        AnimatedModel::Mesh *m_meshes;
        unsigned int m_numMeshes;
        bool m_bVerification;
        AnimatedModel::Animation *m_animations;
        __int16 m_animRemap[32];
        DCollisionData m_Collision;
        std::vector<DRAFT_Geom> m_Geoms;
        std::vector<DRAFT_HierGeom> m_HierGeoms;
        std::map<unsigned int,DRAFT_BoneBounds> m_BonesBounds;
        std::vector<std::vector<DSurfaceMaterial>> m_Skins;
        LoadSkins m_loadSkins;
        bool m_hasCubemap;
        std::vector<MeshesGroup> m_MhGroups;
        unsigned int m_cfgSize;
        Aabb m_box;
    };
}

namespace AnimatedModel
{
    namespace m3d
    {
        class Animation
        {
        public:
            ~Animation();
            Animation();
        protected:
        private:
            __int16 m_numNodes;
            __int16 m_numFrames;
            __int16 m_fps;
            __int16 m_numChanges;
            __int16 m_nextAnimation;
            ActionType m_action;
            char m_name[25];
            HierarchyChange *m_hierChanges;
            AnimationTransform *m_nodesPositions;
        };
    }
}

namespace AnimatedModel
{
    namespace m3d
    {
        class Bone
        {
        public:
            Bone(struct Bone const &);
        protected:
        private:
            char m_boneName[40];
            int m_ownIdx;
            int m_parentIdx;
            CVector m_translation0;
            Quaternion m_quaternion0;
        };
    }
}

namespace m3d
{
    class Configuration
    {
    public:
        Configuration();
    protected:
    private:
        unsigned int m_num;
        std::vector<AnimatedModel::Mesh *> m_meshes;
        std::vector<unsigned char> m_groupVariants;
    };
}

namespace m3d
{
    class LoadSkins
    {
    public:
        LoadSkins();
    protected:
    private:
        bool loadAllSkins;
        std::set<int> loadSkins;
    };
}

namespace m3d
{
    class AnimInfo
    {
    public:
        bool IsAnimation(enum ActionType);
        void CreateCopyMesh(int &,void * * &,int * &,unsigned short * * &,int * &,bool * &,struct CMatrix * * &,class std::vector<rend::VertexType,class std::allocator<rend::VertexType> > &,class std::vector<unsigned int,class std::allocator<unsigned int> > &,int *);
        void SetEmpty();
        void InterpolateBones(int);
        int SetAnimationIdx(int);
        AnimatedModel::Mesh const & GetMesh(unsigned int) const ;
        void CreateFor(AnimatedModel *);
        void SetBoneCurMatrix(unsigned int,struct CMatrix const &);
        struct CMatrix const & GetCurrentLoadpointMatrix(int) const ;
        int SetCurFrame(float);
        void Release();
        AnimatedModel::Animation const * GetCurAnimation() const ;
        int GetStickToLastFrame() const ;
        void RemoveCopyMesh(void * * &,int * &,unsigned short * * &,int * &,struct CMatrix * * &);
        bool IsEmpty();
        int & CurAnimFrame();
        ~AnimInfo();
        AnimInfo();
        int SetAnimation(enum ActionType);
        void MoveFrame(unsigned int);
        BoneAnim & GetBoneAnim(unsigned int);
    protected:
    private:
        Aabb m_curBox;
        bool m_Empty;
        BoneAnim *m_bonesAnim;
        AnimatedModel *m_forModel;
        AnimatedModel::Animation *m_curAnimation;
        int m_curAnimFrame;
        int m_timeOutToNextFrame;
        std::vector<void *> m_meshesVerts;
        std::vector<rend::VertexType> m_VertTypes;
        std::vector<unsigned int> m_VertTypesSize;
        int m_lastInterpolationUpdate;
        int m_stickToLastFrame;
        int m_blendFramesNum;
        bool m_isBlending;
        BoneAnim *m_bonesAnimPrev;
        AnimatedModel::Animation *m_curAnimationPrev;
        int m_curAnimFramePrev;
        int m_timeOutToNextFramePrev;
        int m_lastInterpolationUpdatePrev;
        int m_stickToLastFramePrev;
    };
}

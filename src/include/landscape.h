#pragma once

namespace Landscape
{
    namespace m3d
    {
        class CollisionCellItem
        {
        public:
            void InsertObstacle(class ai::Obstacle *);
            ~CollisionCellItem();
            CollisionCellItem();
            class std::set<class ref_ptr<class ai::Obstacle>,struct std::less<class ref_ptr<class ai::Obstacle> >,class std::allocator<class ref_ptr<class ai::Obstacle> > > const & GetObstacles() const ;
            void InsertPhysicObjId(int);
            class std::set<int,struct std::less<int>,class std::allocator<int> > const & GetPhysicObjIds() const ;
            void EraseObstacle(class ai::Obstacle *);
            void ErasePhysicObjId(int);
        protected:
        private:
            std::set<GeomObject *> m_geomsList;
            bool m_wasEnabledLastFrame;
            bool m_bMustCheck;
            std::set<int> m_physicObjIds;
            std::set<ref_ptr<ai::Obstacle>> *m_obstacles;
        };
    }
}

namespace Landscape
{
    namespace m3d
    {
        class WaveSets
        {
        public:
            WaveSets(struct WaveSets const &);
        protected:
        private:
            float m_tcomp;
            float m_tlevel;
            float m_tamplitude;
            float m_tphase;
            float m_tfreq;
            float m_scomp;
            float m_slevel;
            float m_samplitude;
            float m_sphase;
            float m_sfreq;
            rend::TexHandle m_texHandle;
        };
    }
}

namespace m3d
{
    class Landscape :  public SgNode, IConHandler
    {
    public:
        void LinkObstacleToCells(class ai::Obstacle *);
        bool SaveShoreLine(class CStr const &);
        int GetTileSize() const ;
        void SetOverlayShader(rend::IEffect *);
        void LinkNodeAndChildrenCollisionGeomsToCell(SgNode *);
        float GetHeightWithCollisions(float,float,bool) const ;
        void InitReflectionRefractionTextures();
        void SetAllTexturesLoading(bool);
        void FreeShoresStuff();
        void BuildSolidLandscape();
        void DrawCellsOverlayedEditor(cmn::vector<unsigned int> const &,unsigned int);
        void DrawSolidLandscape(enum LandRenderMode,int);
        void RemoveGrassInstance(unsigned int);
        void QueryWaterVisibility();
        void DrawLandScapeTextures(enum VisibilityMode,bool,bool);
        rend::TexHandle GetLightmapTexture() const ;
        void RemoveGrassRectangle(struct CVector2 const &,struct CVector2 const &);
        void ChangedNumberOfUsedTextures(unsigned int);
        int ConstructCollisionData();
        void setOwner(CWorld *);
        virtual Class * GetClass() const ;
        class CStr const & GetPathToTiles() const ;
        static void __fastcall SetEditorRenderMode();
        void LinkNodeObstacleToCells(SgNode *);
        void ReadTileInfo(int);
        void _dbgGenerateGrass();
        void DrawJoint(struct dxJoint *);
        virtual ~Landscape();
        void UnlinkNodeCollisionGeomsFromCell(SgNode *,int,int,bool);
        int New(float);
        virtual bool HandleCVar(CVar const *,CConsoleParams const &);
        int AddOneTexture(class CStr const &);
        virtual void HandleCommand(int,CConsoleParams const &);
        void AddCollisionTris(int,int,struct CVector *,int,unsigned short *,struct CMatrix const &);
        void DrawShoreLine();
        void ReloadWaterTextures();
        bool SaveNormalMap(class CStr const &);
        bool SaveColorMap(class CStr const &,int);
        void SetLsHeight(float,float,float);
        float GetLsHeight(float,float) const ;
        unsigned int GetNumGrassModels() const ;
        void setHgtAtHfPoint(int,int,float);
        float getHgtAtHfPoint(int,int) const ;
        rend::TexHandle GetTexHandleFromList(unsigned int) const ;
        void SaveTileInfo();
        int Load();
        void RenderGrass(unsigned int,struct GrassInstance * *,int *,enum RenderGrassType);
        void RenderGrass(class std::deque<struct std::pair<int,int>,class std::allocator<struct std::pair<int,int> > > const &);
        int GetLsSize() const ;
        void RecalcUV();
        void UpdateTexturesFilters();
        void ScaleGrassRadius(struct CVector const &,float,float);
        void LinkNodeCollisionGeomsToCell(SgNode *,int,int,int,int);
        void DrawShoresLayer();
        void drawCellOverlayedShader(int,int,rend::IEffect *);
        struct TileInfo const & GetTileInfo(int,int) const ;
        void Invalidate();
        unsigned int AddGrassInstance(int,struct CVector const &,float,float,bool);
        void EndWaterQuery();
        void SetNodeCollisionGeomsEnabled(SgNode *,bool);
        bool Save16bitDisplace(class CStr const &,int);
        void DrawCollisionGeoms(bool);
        void ClearCollisionCellsMap();
        bool AddGrassModel(char const *);
        void GenerateOneDPVSCellMesh(int,int,struct CVector *,int *);
        float GetFloatToShortScale() const ;
        void drawSpriteOverlayed2(float,float,float,float,unsigned int,bool);
        int RecalcNormalMap(int,int,int,int);
        void ChangeShoreState(int,int,bool);
        float GetHeight(float,float,int,bool);
        void CheckLandscapeCollisionTriMeshesForObjId(int);
        GeomObject * GetTerrainGeomObject() const ;
        void GetDPVSCollisionInfo(int,int,int &,int &);
        void RemoveGrassRadius(struct CVector const &,float);
        void DisableShoreRegion(int,int);
        void UpdateVis(bool);
        struct CollisionCellItem * GetCollisionCellItem(int,int) const ;
        int GenerateShoreLine();
        void ReleaseOdeCollisionData();
        bool LoadNormalMap(class CStr const &);
        float GetScaleForTile() const ;
        void SwitchDrawMode();
        void RemoveCollisionTris(int);
        static Object * __fastcall CreateObject();
        void getMinMaxHeightForBox(float *,float);
        static void __fastcall SetGameRenderMode();
        void CollectGrassCell(int,int,unsigned int &,struct GrassInstance * *,int *);
        void DrawGeom(struct dxGeom *);
        void Update();
        void DrawWaterLayer();
        void CreateHelperStructures();
        void drawSpriteOverlayed2Projected(float,float,float,float,unsigned int,bool,class CClipper const &);
        static Class * __fastcall GetBaseClass();
        void PostServersLoad();
        void RemoveGrassTile(int,int);
        void DrawCells(cmn::vector<unsigned int> const &,unsigned int);
        void GetVisCellHeights(float &,float &,int,int) const ;
        void CreateIndicesTriLists(int *,int,int);
        void UpdateNodeCollisionGeoms(SgNode *);
        void ReleaseLod();
        void DrawMassBox(struct dMass *,struct CVector const &,struct Quaternion const &);
        static void __fastcall Register();
        bool InitGrass();
        void StartWaterQuery();
        void PutGrassToLandscape(struct CVector2 const &,struct CVector2 const &);
        int GetNumTiles() const ;
        void setDrawRadius(int,int,int);
        void GetDrawedCellHeights(float &,float &,int,int) const ;
        unsigned int GetNearestGrassInstance(struct CVector const &) const ;
        void DoneGrass();
        float getCameraHeight(float,float) const ;
        int getGrassModelIdByName(char const *) const ;
        int SaveCameraMap(class CStr const &,int);
        void Release();
        void EnableShoreRegion(int,int);
        void Render();
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        void ReadGrassFromXmlFile(char const *);
        bool WriteGrassToXmlFile(char const *);
        struct CVector getNormal(float,float);
        unsigned char GetColor(float,float);
        void SetPresenceOnCollisionMap(int,int);
        bool IsThisVisCellHasWater(int,int) const ;
        void drawSpriteOverlayed(unsigned int,struct CVector const &,struct CVector const &,float);
        void ReloadLightmapTexture(class CStr const &);
        virtual Object * Clone();
        bool LoadShoreLine(class CStr const &);
        void GetFogStartAndEnd(float &,float &) const ;
        int GetNumAlphas() const ;
        void CreateLod();
        void drawSpriteOverlayedProjected(unsigned int,struct CVector const &,struct CMatrix const &,class CClipper const &);
        void CreateHeights(struct CellParams *,int,int);
        int isWaterCell(int,int) const ;
        void Restore();
        float getWaterHeight(int,int) const ;
        void ReleaseReflectionRefractionTextures();
        char const * GetGrassModelName(unsigned int) const ;
        void renderZGuard();
        void GetWaterCellHeights(float &,float &,int,int) const ;
        void ManageLandScapeCollisionTriMeshes();
        void LinkPassMapCellToCollisionCell(class PointBase<int> const &);
        void ReBuildShoresVb();
    protected:
        int LoadTiles(class CStr const &);
        bool traceLineThruCellLs(float &,int,int,struct CVector const &,struct CVector const &,bool);
        void RecursiveDisableShore(unsigned char *,int,int);
        Landscape(class Landscape const &);
        Landscape();
        void BuildCells0(rend::VertexLandscape *,struct TIVChunk &,int &,cmn::vector<unsigned int> const &,enum RenderTypes,class std::vector<int,class std::allocator<int> > &);
        void DrawCellsFast0(cmn::vector<unsigned int> const &,struct TIVChunk &,enum RenderTypes);
        void RecursiveEnableShore(unsigned char *,int,int);
        bool traceLineThruCellLs0(float &,int,int,struct CVector const &,struct CVector const &);
        void DrawCells0(cmn::vector<unsigned int> const &,enum RenderTypes);
        int IsBackfaced(int,int,rend::Cull);
        void FreeTiles();
        void BuildUVSet();
    private:
        std::_Allocate<LandType>(uint,LandType *);
        std::fill<AlphaMask *,AlphaMask>(AlphaMask *,AlphaMask *,AlphaMask const &);
        std::_Uninit_copy<GrassInstance *,GrassInstance *>(GrassInstance * *,GrassInstance * *,GrassInstance * *,std::allocator<GrassInstance *> &,std::_Scalar_ptr_iterator_tag);
        std::_Uninit_copy<CollisionInfo *,CollisionInfo *>(CollisionInfo * *,CollisionInfo * *,CollisionInfo * *,std::allocator<CollisionInfo *> &,std::_Scalar_ptr_iterator_tag);
        std::fill<LandType *,LandType>(LandType *,LandType *,LandType const &);
        std::_Construct<LandType,LandType>(LandType *,LandType const &);
        std::_Allocate<AlphaMask>(uint,AlphaMask *);
        std::_Copy_backward_opt<LandType *,LandType *>(LandType *,LandType *,LandType *,std::_Nonscalar_ptr_iterator_tag);
        std::_Construct<AlphaMask,AlphaMask>(AlphaMask *,AlphaMask const &);
        std::_Destroy<AlphaMask>(AlphaMask *);
        std::_Copy_backward_opt<CollisionInfo * *,CollisionInfo * *>(CollisionInfo * *,CollisionInfo * *,CollisionInfo * *,std::_Scalar_ptr_iterator_tag);
        std::copy<CollisionInfo * *,CollisionInfo * *>(CollisionInfo * *,CollisionInfo * *,CollisionInfo * *);
        std::_Copy_backward_opt<GrassInstance * *,GrassInstance * *>(GrassInstance * *,GrassInstance * *,GrassInstance * *,std::_Scalar_ptr_iterator_tag);
        std::_Allocate<WaveSets>(uint,WaveSets *);
        std::copy<GrassInstance * *,GrassInstance * *>(GrassInstance * *,GrassInstance * *,GrassInstance * *);
        std::_Allocate<TIVChunk *>(uint,TIVChunk * *);
        std::_Copy_opt<GrassInstancesForModel * *,GrassInstancesForModel * *>(GrassInstancesForModel * *,GrassInstancesForModel * *,GrassInstancesForModel * *,std::_Scalar_ptr_iterator_tag);
        std::_Ptr_cat<AlphaMask *,AlphaMask *>(AlphaMask * &,AlphaMask * &);
        std::_Allocate<GrassInstancesForModel *>(uint,GrassInstancesForModel * *);
        std::_Ptr_cat<LandType *,LandType *>(LandType * &,LandType * &);
        std::_Copy_opt<GrassInstance * *,GrassInstance * *>(GrassInstance * *,GrassInstance * *,GrassInstance * *,std::_Scalar_ptr_iterator_tag);
        std::allocator<GrassInstance *>::allocator<GrassInstance *>();
        std::allocator<GrassInstance *>::allocator<GrassInstance *>();
        std::allocator<CollisionInfo *>::allocator<CollisionInfo *>();
        std::allocator<CollisionInfo *>::allocator<CollisionInfo *>();
        std::copy<GrassInstancesForModel * *,GrassInstancesForModel * *>(GrassInstancesForModel * *,GrassInstancesForModel * *,GrassInstancesForModel * *);
        std::_Copy_opt<CollisionInfo * *,CollisionInfo * *>(CollisionInfo * *,CollisionInfo * *,CollisionInfo * *,std::_Scalar_ptr_iterator_tag);
        std::copy_backward<WaveSets *,WaveSets *>(WaveSets *,WaveSets *,WaveSets *);
        std::_Uninit_copy<GrassInstancesForModel *,GrassInstancesForModel *>(GrassInstancesForModel * *,GrassInstancesForModel * *,GrassInstancesForModel * *,std::allocator<GrassInstancesForModel *> &,std::_Scalar_ptr_iterator_tag);
        std::_Destroy<LandType>(LandType *);
        std::copy_backward<AlphaMask *,AlphaMask *>(AlphaMask *,AlphaMask *,AlphaMask *);
        std::_Ptr_cat<CollisionInfo>(CollisionInfo * *,CollisionInfo * *);
        std::allocator<LandType>::allocator<LandType>();
        std::allocator<LandType>::allocator<LandType>();
        std::_Copy_backward_opt<AlphaMask *,AlphaMask *>(AlphaMask *,AlphaMask *,AlphaMask *,std::_Nonscalar_ptr_iterator_tag);
        std::allocator<TIVChunk *>::allocator<TIVChunk *>();
        std::allocator<TIVChunk *>::allocator<TIVChunk *>();
        std::fill<WaveSets *,WaveSets>(WaveSets *,WaveSets *,WaveSets const &);
        std::_Construct<WaveSets,WaveSets>(WaveSets *,WaveSets const &);
        std::copy_backward<GrassInstancesForModel * *,GrassInstancesForModel * *>(GrassInstancesForModel * *,GrassInstancesForModel * *,GrassInstancesForModel * *);
        std::allocator<GrassInstancesForModel *>::allocator<GrassInstancesForModel *>();
        std::allocator<GrassInstancesForModel *>::allocator<GrassInstancesForModel *>();
        std::_Ptr_cat<GrassInstancesForModel>(GrassInstancesForModel * *,GrassInstancesForModel * *);
        std::_Allocate<CollisionInfo *>(uint,CollisionInfo * *);
        std::copy_backward<TIVChunk * *,TIVChunk * *>(TIVChunk * *,TIVChunk * *,TIVChunk * *);
        std::fill<GrassInstance * *,GrassInstance *>(GrassInstance * *,GrassInstance * *,GrassInstance * const &);
        enum VisibilityMode GetCurVisMode() const ;
        void SetCurVisMode(enum VisibilityMode);
        std::fill<CollisionInfo * *,CollisionInfo *>(CollisionInfo * *,CollisionInfo * *,CollisionInfo * const &);
        std::_Copy_backward_opt<WaveSets *,WaveSets *>(WaveSets *,WaveSets *,WaveSets *,std::_Nonscalar_ptr_iterator_tag);
        void DrawNonTransformGeom(struct dxGeom *);
        std::_Copy_backward_opt<GrassInstancesForModel * *,GrassInstancesForModel * *>(GrassInstancesForModel * *,GrassInstancesForModel * *,GrassInstancesForModel * *,std::_Scalar_ptr_iterator_tag);
        std::copy_backward<LandType *,LandType *>(LandType *,LandType *,LandType *);
        std::copy_backward<CollisionInfo * *,CollisionInfo * *>(CollisionInfo * *,CollisionInfo * *,CollisionInfo * *);
        std::fill<GrassInstancesForModel * *,GrassInstancesForModel *>(GrassInstancesForModel * *,GrassInstancesForModel * *,GrassInstancesForModel * const &);
        std::copy_backward<GrassInstance * *,GrassInstance * *>(GrassInstance * *,GrassInstance * *,GrassInstance * *);
        std::allocator<AlphaMask>::allocator<AlphaMask>();
        std::allocator<AlphaMask>::allocator<AlphaMask>();
        std::allocator<WaveSets>::allocator<WaveSets>();
        std::allocator<WaveSets>::allocator<WaveSets>();
        void RenderRoads();
        std::_Uninit_copy<TIVChunk *,TIVChunk *>(TIVChunk * *,TIVChunk * *,TIVChunk * *,std::allocator<TIVChunk *> &,std::_Scalar_ptr_iterator_tag);
        std::_Destroy<WaveSets>(WaveSets *);
        std::_Allocate<GrassInstance *>(uint,GrassInstance * *);
        std::_Ptr_cat<WaveSets *,WaveSets *>(WaveSets * &,WaveSets * &);
        std::fill<TIVChunk * *,TIVChunk *>(TIVChunk * *,TIVChunk * *,TIVChunk * const &);
        std::_Ptr_cat<GrassInstance>(GrassInstance * *,GrassInstance * *);
        std::_Ptr_cat<TIVChunk>(TIVChunk * *,TIVChunk * *);
        std::_Copy_backward_opt<TIVChunk * *,TIVChunk * *>(TIVChunk * *,TIVChunk * *,TIVChunk * *,std::_Scalar_ptr_iterator_tag);
        Landscape::CollisionCellItem **m_oCollisionitems;
        GeomObject *m_terrainObject;
        int m_maxLOD;
        std::vector<Landscape::CollisionInfo *> m_collisions;
        float *m_heightMap;
        __int16 *m_waterMap;
        CVector *m_vnormal;
        std::vector<rend::IbHandle> m_landIbConst;
        rend::VbHandle m_landUVVb;
        rend::VbHandle m_landVb;
        rend::IbHandle m_waterIb[16];
        rend::VbHandle m_waterVb;
        rend::IQuery *m_waterQueries[3];
        int m_currWaterQuery;
        bool m_isWaterVisible;
        bool m_currentWaterVis;
        rend::IbHandle m_shoresIb;
        rend::VbHandle m_shoresVb;
        rend::IEffect *m_shoresShader;
        rend::VbHandle m_solidVb;
        rend::IbHandle m_solidIb[4];
        rend::IHlslShader *m_solidVs;
        rend::IHlslShader *m_solidPs;
        rend::IHlslShader *m_solidBindVs;
        rend::IHlslShader *m_solidBindPs;
        float m_bindDevider;
        rend::IHlslShader *m_solidDeepVs;
        rend::IHlslShader *m_solidDeepPs;
        rend::IHlslShader *m_landscapeVs;
        rend::IHlslShader *m_landscapePsFP;
        rend::IHlslShader *m_landscapePsSP;
        CMatrix m_matScale;
        int vertsPerCell;
        int trisPerCell[4];
        std::vector<Landscape::WaveSets> m_waves;
        std::vector<std::vector<CVector>> m_shoreLines;
        std::set<unsigned int> m_noShoresSet;
        rend::VertexXYZNCT2 *m_dummyVB;
        unsigned __int16 *m_remappedIndices;
        unsigned __int16 *m_lsIndicesDubb;
        int m_ver;
        CPlane m_waterPlane;
        CClipper m_frustumCull;
        int m_numWaterCells;
        int m_drawRadius;
        int m_landscapeClip0;
        int m_landscapeClip1;
        int m_landscapeClip2;
        float m_landscapeClip0z;
        float m_landscapeClip1z;
        float m_landscapeClip2z;
        float m_landscapeClip0zSq;
        float m_landscapeClip1zSq;
        float m_landscapeClip2zSq;
        int m_saveClip[3];
        CClipper m_reflectedFrustum;
        CVar m_lockVis;
        Landscape::CellParams *m_cellParams;
        Landscape::CellParams *m_drawedCellParams;
        CFlare m_flares;
        bool m_dirtyReflection;
        rend::TexHandle m_texRtReflection;
        rend::TexHandle m_texRtRefraction;
        rend::TexHandle m_baseWaterTex;
        rend::TexHandle m_texLightmap;
        int m_texNormalMapSize;
        std::vector<Landscape::AlphaMask> m_AlphaSets;
        std::vector<Landscape::LandType> m_Lands;
        CIntHash<int> m_hashIdxToPass;
        CStrHash<int> m_hashAlphaToLand;
        int m_CurAlphaSet;
        char *m_passedCells;
        unsigned __int8 *m_cliffHeightMap;
        unsigned int *m_normalMap;
        CWorld *m_owner;
        Landscape::RenderTypes m_lastState;
        int m_firstpasscounter;
        int m_otherpasscounter;
        std::set<unsigned int> *m_texSetsmap;
        rend::IEffect *overlayShader;
        CIntHash<int> m_hashTexToIndex;
        CStrHash<int> m_texToIdx;
        CIntHash<int> m_hashIdxToLandType;
        std::vector<Landscape::TIVChunk *> m_tilesTextures;
        Landscape::TileInfo *m_tiles;
        unsigned int *m_colormap;
        float m_uvForAngles[4][25][2];
        Landscape::TextureAlphaSet m_setAndUVs;
        cmn::vector<cmn::vector<unsigned int> > m_cellsPerTex;
        CStr m_pathTile;
        bool m_loadAllTextures;
        std::set<CStr> m_usedTexturesList;
        std::vector<int> m_lsNumIndices;
        int m_wtNumTris[16];
        int m_drawtextured;
        int m_mapSize;
        Profiler *m_profilerDraw;
        Profiler *m_profilerUpdateVis;
        Profiler *m_profilerDrawGrass;
        Profiler *m_profilerDrawWater;
        DbgCounter *m_countPhysicObjsInCells;
        rend::TexHandle m_waveBumpTex;
        rend::TexHandle m_waveBumpSmTex;
        rend::TexHandle m_fresnelTex;
        rend::TexHandle m_waveReflMap;
        rend::IHlslShader *m_waterVs;
        rend::IHlslShader *m_waterPs;
        rend::IHlslShader *m_waterDumbVs;
        rend::IHlslShader *m_waterDumbPs;
        int m_waterShaderVersion;
        int m_maxWaterCellPerPass;
        std::vector<std::pair<unsigned int,float>> waterCellsToDraw[16];
        CVector4 *waterTileInfo;
        rend::IHlslShader *m_grassVs;
        rend::IHlslShader *m_grassPs;
        Landscape::VisibilityMode m_curVisMode;
        Landscape::TileGrass **m_grassArray;
        unsigned int m_numGrassModels;
    };
}

namespace Landscape
{
    namespace m3d
    {
        class CellParams
        {
        public:
            CellParams();
        protected:
        private:
            float m_h0;
            float m_h1;
            bool m_iswatercell;
            float m_minwater;
            float m_maxwater;
            float m_lodDelta[5];
            bool m_lodDeltaCreated;
            float m_normalTheta;
            CVector m_normalAverage;
        };
    }
}

namespace cmn
{
    namespace m3d
    {
        class vector<unsigned int>
        {
        public:
        protected:
        private:
            unsigned int *m_data;
            int m_numItems;
            int m_maxItems;
        };
    }
}

namespace Landscape
{
    namespace m3d
    {
        class CollisionInfo
        {
        public:
            ~CollisionInfo();
            void Draw(unsigned int);
            float GetHeight(float,float);
            void Create(int,struct CVector *,int,unsigned short *,struct CMatrix const &);
            CollisionInfo();
            float TraceRay(struct CVector const &,struct CVector const &);
        protected:
        private:
            int m_tag;
            int m_numVerts;
            CVector *m_verts;
            int m_numTris;
            unsigned __int16 *m_tris;
            Aabb m_box;
            Obb m_obb;
        };
    }
}

namespace Landscape
{
    namespace m3d
    {
        class TileInfo
        {
        public:
            ~TileInfo();
            TileInfo();
        protected:
        private:
            int m_texIndex0;
            int m_angle;
            int m_texIndices[4];
            int m_texFlags[4];
            int m_numTexs;
            int m_maskIndex;
            int m_rotate;
        };
    }
}

namespace Landscape
{
    namespace m3d
    {
        class TIVChunk
        {
        public:
            TIVChunk();
        protected:
        private:
            std::vector<rend::VbHandle> m_vbHandle;
            rend::TexHandle m_texHandle;
            unsigned int m_offsetsmap[65536];
            unsigned __int16 m_banknumber[65536];
            unsigned __int16 m_numCellsPerCellMap[4096];
            int iotherPassOffset;
            int iotherPassBankNumber;
        };
    }
}

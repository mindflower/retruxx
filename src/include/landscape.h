#pragma once
#include <clipper.h>
#include <deque>
#include <flares.h>
#include <vector>
#include <core/containers.h>
#include <core/ref_ptr.h>
#include <core/console/console.h>
#include <core/console/cvar.h>
#include <math/obb.h>
#include <math/plane.h>
#include <renderer/i_renderer.h>
#include <scene/nodes/sgnode.h>

struct dxJoint;
struct dxGeom;
struct dMass;

namespace ai
{
    class Obstacle;
}

namespace m3d
{
    class CWorld;
    class Profiler;
    class DbgCounter;
    class GeomObject;

    class Landscape :  public SgNode, public IConHandler
    {
    public:
        class TileInfo
        {
        public:
            ~TileInfo();
            TileInfo();

        private:
            int m_texIndex0;
            int m_angle;
            int m_texIndices[4];
            int m_texFlags[4];
            int m_numTexs;
            int m_maskIndex;
            int m_rotate;

        };
        class TIVChunk
        {
        public:
            TIVChunk();

        private:
            std::vector<rend::VbHandle> m_vbHandle;
            rend::TexHandle m_texHandle;
            unsigned int m_offsetsmap[65536];
            unsigned __int16 m_banknumber[65536];
            unsigned __int16 m_numCellsPerCellMap[4096];
            int iotherPassOffset;
            int iotherPassBankNumber;

        };
        class WaveSets
        {
        public:
            WaveSets(WaveSets const&);

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

        class CellParams
        {
        public:
            CellParams();

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

        class CollisionInfo
        {
        public:
            ~CollisionInfo();
            void Draw(unsigned int);
            float GetHeight(float, float);
            void Create(int, struct CVector*, int, unsigned short*, struct CMatrix const&);
            CollisionInfo();
            float TraceRay(struct CVector const&, struct CVector const&);

        private:
            int m_tag;
            int m_numVerts;
            CVector* m_verts;
            int m_numTris;
            unsigned __int16* m_tris;
            Aabb m_box;
            Obb m_obb;
        };

        class CollisionCellItem
        {
        public:
            void InsertObstacle(ai::Obstacle*);
            ~CollisionCellItem();
            CollisionCellItem();
            std::set<ref_ptr<ai::Obstacle>> const& GetObstacles() const;
            void InsertPhysicObjId(int);
            std::set<int> const& GetPhysicObjIds() const;
            void EraseObstacle(ai::Obstacle*);
            void ErasePhysicObjId(int);

        private:
            std::set<GeomObject*> m_geomsList;
            bool m_wasEnabledLastFrame;
            bool m_bMustCheck;
            std::set<int> m_physicObjIds;
            std::set<ref_ptr<ai::Obstacle>>* m_obstacles;
        };

        class AlphaMask
        {
        public:
        protected:
        private:
            CStr m_name;
            std::vector<rend::TexHandle> m_texMasks[4];
        };

        enum VisibilityMode
        {
            VIS_DIRECT = 0x0,
            VIS_REFLECTION = 0x1,
            VIS_REFRACTION = 0x2,
        };

        enum LandRenderMode
        {
            LRM_DIRECT = 0x0,
            LRM_REFLECTION = 0x1,
            LRM_DEEPMAP = 0x2,
            LRM_BIND = 0x3,
        };

        enum RenderTypes
        {
            RT_FIRSTPASSLIGHT = 0x0,
            RT_OTHERPASSES = 0x1,
            RT_LIGHTPASS = 0x2,
        };

        enum RenderGrassType
        {
            RGT_SIMPLE = 0x0,
            RGT_FOR_SHADOW = 0x1,
            RGT_FOR_PROJECTOR = 0x2,
        };

        struct AlphaSetUnit
        {
            float m_uvForAngles[4][25][2];
        public:
        protected:
        private:
        };

        /* 2792 */
        struct TextureAlphaSet
        {
            AlphaSetUnit m_sets[8][5];
        public:
        protected:
        private:
        };

        class LandType
        {
        public:
        protected:
        private:
            CStr m_name;
            int m_passmask;
            int m_alphaset;
            int m_priority;
            std::vector<int> m_texIndices;
        };

        class GrassInstance
        {
        public:
        protected:
        private:
            CVector pos;
            float scale;
            float sinYaw;
            float cosYaw;
        };

        class GrassInstancesForModel
        {
        public:
        protected:
        private:
            int modelId;
            int numInstances;
            std::vector<GrassInstance*> grass;
        };

        class TileGrass
        {
        public:
        protected:
        private:
            unsigned int numDiffModels;
            unsigned int numInstances;
            std::vector<GrassInstancesForModel*> instancesPerModel;
        };

    public:
        void LinkObstacleToCells(ai::Obstacle *);
        bool SaveShoreLine(CStr const &);
        int GetTileSize() const ;
        void SetOverlayShader(rend::IEffect *);
        void LinkNodeAndChildrenCollisionGeomsToCell(SgNode *);
        float GetHeightWithCollisions(float,float,bool) const ;
        void InitReflectionRefractionTextures();
        void SetAllTexturesLoading(bool);
        void FreeShoresStuff();
        void BuildSolidLandscape();
        void DrawCellsOverlayedEditor(std::vector<unsigned int> const &,unsigned int);
        void DrawSolidLandscape(LandRenderMode,int);
        void RemoveGrassInstance(unsigned int);
        void QueryWaterVisibility();
        void DrawLandScapeTextures(VisibilityMode,bool,bool);
        rend::TexHandle GetLightmapTexture() const ;
        void RemoveGrassRectangle(CVector2 const &,CVector2 const &);
        void ChangedNumberOfUsedTextures(unsigned int);
        int ConstructCollisionData();
        void setOwner(CWorld *);
        virtual Class * GetClass() const ;
        CStr const & GetPathToTiles() const ;
        static void __fastcall SetEditorRenderMode();
        void LinkNodeObstacleToCells(SgNode *);
        void ReadTileInfo(int);
        void _dbgGenerateGrass();
        void DrawJoint(dxJoint *);
        virtual ~Landscape();
        void UnlinkNodeCollisionGeomsFromCell(SgNode *,int,int,bool);
        int New(float);
        virtual bool HandleCVar(CVar const *,CConsoleParams const &);
        int AddOneTexture(CStr const &);
        virtual void HandleCommand(int,CConsoleParams const &);
        void AddCollisionTris(int,int,CVector *,int,unsigned short *,CMatrix const &);
        void DrawShoreLine();
        void ReloadWaterTextures();
        bool SaveNormalMap(CStr const &);
        bool SaveColorMap(CStr const &,int);
        void SetLsHeight(float,float,float);
        float GetLsHeight(float,float) const ;
        unsigned int GetNumGrassModels() const ;
        void setHgtAtHfPoint(int,int,float);
        float getHgtAtHfPoint(int,int) const ;
        rend::TexHandle GetTexHandleFromList(unsigned int) const ;
        void SaveTileInfo();
        int Load();
        void RenderGrass(unsigned int,GrassInstance * *,int *,RenderGrassType);
        void RenderGrass(std::deque<std::pair<int,int>> const &);
        int GetLsSize() const ;
        void RecalcUV();
        void UpdateTexturesFilters();
        void ScaleGrassRadius(CVector const &,float,float);
        void LinkNodeCollisionGeomsToCell(SgNode *,int,int,int,int);
        void DrawShoresLayer();
        void drawCellOverlayedShader(int,int,rend::IEffect *);
        TileInfo const & GetTileInfo(int,int) const ;
        void Invalidate();
        unsigned int AddGrassInstance(int,CVector const &,float,float,bool);
        void EndWaterQuery();
        void SetNodeCollisionGeomsEnabled(SgNode *,bool);
        bool Save16bitDisplace(CStr const &,int);
        void DrawCollisionGeoms(bool);
        void ClearCollisionCellsMap();
        bool AddGrassModel(char const *);
        void GenerateOneDPVSCellMesh(int,int,CVector *,int *);
        float GetFloatToShortScale() const ;
        void drawSpriteOverlayed2(float,float,float,float,unsigned int,bool);
        int RecalcNormalMap(int,int,int,int);
        void ChangeShoreState(int,int,bool);
        float GetHeight(float,float,int,bool);
        void CheckLandscapeCollisionTriMeshesForObjId(int);
        GeomObject * GetTerrainGeomObject() const ;
        void GetDPVSCollisionInfo(int,int,int &,int &);
        void RemoveGrassRadius(CVector const &,float);
        void DisableShoreRegion(int,int);
        void UpdateVis(bool);
        CollisionCellItem * GetCollisionCellItem(int,int) const ;
        int GenerateShoreLine();
        void ReleaseOdeCollisionData();
        bool LoadNormalMap(CStr const &);
        float GetScaleForTile() const ;
        void SwitchDrawMode();
        void RemoveCollisionTris(int);
        static Object * __fastcall CreateObject();
        void getMinMaxHeightForBox(float *,float);
        static void __fastcall SetGameRenderMode();
        void CollectGrassCell(int,int,unsigned int &,GrassInstance * *,int *);
        void DrawGeom(dxGeom *);
        void Update();
        void DrawWaterLayer();
        void CreateHelperStructures();
        void drawSpriteOverlayed2Projected(float,float,float,float,unsigned int,bool,CClipper const &);
        static Class * __fastcall GetBaseClass();
        void PostServersLoad();
        void RemoveGrassTile(int,int);
        void DrawCells(std::vector<unsigned int> const &,unsigned int);
        void GetVisCellHeights(float &,float &,int,int) const ;
        void CreateIndicesTriLists(int *,int,int);
        void UpdateNodeCollisionGeoms(SgNode *);
        void ReleaseLod();
        void DrawMassBox(dMass *,CVector const &,Quaternion const &);
        static void Register();
        bool InitGrass();
        void StartWaterQuery();
        void PutGrassToLandscape(CVector2 const &,CVector2 const &);
        int GetNumTiles() const ;
        void setDrawRadius(int,int,int);
        void GetDrawedCellHeights(float &,float &,int,int) const ;
        unsigned int GetNearestGrassInstance(CVector const &) const ;
        void DoneGrass();
        float getCameraHeight(float,float) const ;
        int getGrassModelIdByName(char const *) const ;
        int SaveCameraMap(CStr const &,int);
        void Release();
        void EnableShoreRegion(int,int);
        void Render();
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        void ReadGrassFromXmlFile(char const *);
        bool WriteGrassToXmlFile(char const *);
        CVector getNormal(float,float);
        unsigned char GetColor(float,float);
        void SetPresenceOnCollisionMap(int,int);
        bool IsThisVisCellHasWater(int,int) const ;
        void drawSpriteOverlayed(unsigned int,CVector const &,CVector const &,float);
        void ReloadLightmapTexture(CStr const &);
        virtual Object * Clone();
        bool LoadShoreLine(CStr const &);
        void GetFogStartAndEnd(float &,float &) const ;
        int GetNumAlphas() const ;
        void CreateLod();
        void drawSpriteOverlayedProjected(unsigned int,CVector const &,CMatrix const &,CClipper const &);
        void CreateHeights(CellParams *,int,int);
        int isWaterCell(int,int) const ;
        void Restore();
        float getWaterHeight(int,int) const ;
        void ReleaseReflectionRefractionTextures();
        char const * GetGrassModelName(unsigned int) const ;
        void renderZGuard();
        void GetWaterCellHeights(float &,float &,int,int) const ;
        void ManageLandScapeCollisionTriMeshes();
        void LinkPassMapCellToCollisionCell(PointBase<int> const &);
        void ReBuildShoresVb();
    protected:
        int LoadTiles(CStr const &);
        bool traceLineThruCellLs(float &,int,int,CVector const &,CVector const &,bool);
        void RecursiveDisableShore(unsigned char *,int,int);
        Landscape(Landscape const &);
        Landscape();
        void BuildCells0(rend::VertexLandscape *,TIVChunk &,int &,std::vector<unsigned int> const &,RenderTypes,std::vector<int,std::allocator<int> > &);
        void DrawCellsFast0(std::vector<unsigned int> const &,TIVChunk &,RenderTypes);
        void RecursiveEnableShore(unsigned char *,int,int);
        bool traceLineThruCellLs0(float &,int,int,CVector const &,CVector const &);
        void DrawCells0(std::vector<unsigned int> const &,RenderTypes);
        int IsBackfaced(int,int,rend::Cull);
        void FreeTiles();
        void BuildUVSet();
    private:
        VisibilityMode GetCurVisMode() const ;
        void SetCurVisMode(VisibilityMode);
        void DrawNonTransformGeom(dxGeom *);
        void RenderRoads();
        Landscape::CollisionCellItem **m_oCollisionitems;
        GeomObject *m_terrainObject;
        int m_maxLOD;
        std::vector<CollisionInfo *> m_collisions;
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
        std::vector<WaveSets> m_waves;
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
        CellParams *m_cellParams;
        CellParams *m_drawedCellParams;
        CFlare m_flares;
        bool m_dirtyReflection;
        rend::TexHandle m_texRtReflection;
        rend::TexHandle m_texRtRefraction;
        rend::TexHandle m_baseWaterTex;
        rend::TexHandle m_texLightmap;
        int m_texNormalMapSize;
        std::vector<AlphaMask> m_AlphaSets;
        std::vector<LandType> m_Lands;
        CIntHash<int> m_hashIdxToPass;
        CStrHash<int> m_hashAlphaToLand;
        int m_CurAlphaSet;
        char *m_passedCells;
        unsigned __int8 *m_cliffHeightMap;
        unsigned int *m_normalMap;
        CWorld *m_owner;
        RenderTypes m_lastState;
        int m_firstpasscounter;
        int m_otherpasscounter;
        std::set<unsigned int> *m_texSetsmap;
        rend::IEffect *overlayShader;
        CIntHash<int> m_hashTexToIndex;
        CStrHash<int> m_texToIdx;
        CIntHash<int> m_hashIdxToLandType;
        std::vector<TIVChunk *> m_tilesTextures;
        TileInfo *m_tiles;
        unsigned int *m_colormap;
        float m_uvForAngles[4][25][2];
        TextureAlphaSet m_setAndUVs;
        std::vector<std::vector<unsigned int> > m_cellsPerTex;
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
        VisibilityMode m_curVisMode;
        TileGrass **m_grassArray;
        unsigned int m_numGrassModels;
    };
}

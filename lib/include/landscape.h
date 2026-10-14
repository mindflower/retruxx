#pragma once
#include <clipper.h>
#include <flares.h>
#include <core/containers.h>
#include <core/ref_ptr.h>
#include <core/console/console.h>
#include <core/console/cvar.h>
#include <math/obb.h>
#include <math/plane.h>
#include <renderer/i_renderer.h>
#include <scene/nodes/sgnode.h>

#include "server/obstacle.h"

struct dxJoint;
struct dxGeom;
struct dMass;

namespace m3d
{
    class CWorld;
    class Profiler;
    class DbgCounter;
    class GeomObject;
    class AnimatedModel;

    namespace cmn
    {
        template<class T>
        struct vector
        {
            ~vector()
            {
                Deallocate();
            }
            void Allocate(int sz)
            {
                m_data = new T[sz];
                m_maxItems = sz;
                m_numItems = 0;
            }

            void Deallocate()
            {
                delete[] m_data;
                m_data = nullptr;
                m_maxItems = 0;
                m_numItems = 0;
            }
            void clear()
            {
                m_numItems = 0;
            }
            T& operator[](int index)
            {
                return m_data[index];
            }

            const T& operator[](int index) const
            {
                return m_data[index];
            }

            void push_back(const T& shit)
            {
                if (m_numItems < m_maxItems)
                {
                    m_data[m_numItems] = shit;
                    ++m_numItems;
                }
            }

            int empty() const
            {
                return m_numItems == 0;
            }

            int size() const
            {
                return m_numItems;
            }

            T* m_data = nullptr;
            int m_numItems = 0;
            int m_maxItems = 0;
        };
    }

    enum CliffGeometryTypes
    {
        CT_NORMAL = 0,
        CT_OPEN_ENDED = 1,
        CT_LAST = 2,
    };

    enum CliffOpenEndOrientation
    {
        COEO_NORTH = 0,
        COEO_EAST = 1,
        COEO_SOUTH = 2,
        COEO_WEST = 3,
    };

    enum RenderModes
    {
        RM_GAME = 0,
        RM_EDITOR = 1,
    };

    class Landscape : public m3d::SgNode, public m3d::IConHandler
    {
        friend class CWorld;
        friend class RoadManager;
        friend class LightsServer;
        // SceneGraph::TraceLine traces through the landscape cells.
        friend class SceneGraph;
        // ProjectorsServer::RenderItem reads the cell heights to cull cells against a projector.
        friend class ProjectorsServer;

    protected:
        Landscape();
        Landscape(const m3d::Landscape& ls);

    public:
        virtual ~Landscape() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classLandscape;

        using GeomObjectList = retruxx::set<m3d::GeomObject*, retruxx::less<m3d::GeomObject*>, retruxx::allocator<m3d::GeomObject*> >;

        struct CollisionCellItem
        {
            retruxx::set<m3d::GeomObject*, retruxx::less<m3d::GeomObject*>, retruxx::allocator<m3d::GeomObject*> > m_geomsList;
            bool m_wasEnabledLastFrame;
            bool m_bMustCheck;
            CollisionCellItem();
            ~CollisionCellItem();
            void InsertPhysicObjId(int objId);
            void ErasePhysicObjId(int objId);
            void InsertObstacle(ai::Obstacle* ob);
            void EraseObstacle(ai::Obstacle* ob);
            const retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& GetPhysicObjIds() const;
            const retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >& GetObstacles() const;
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> > m_physicObjIds;
            retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >* m_obstacles;
        };

    public:
        m3d::Landscape::CollisionCellItem** m_oCollisionitems;
        m3d::GeomObject* m_terrainObject;
        int m_maxLOD;

        class CollisionInfo
        {
        public:
            int m_tag;
            int m_numVerts;
            CVector* m_verts;
            int m_numTris;
            unsigned short* m_tris;
            Aabb m_box;
            Obb m_obb;
            CollisionInfo(const m3d::Landscape::CollisionInfo&);
            CollisionInfo();
            ~CollisionInfo();
            void Create(int numVerts, CVector* verts, int numTris, unsigned short* tris, const CMatrix& toWorld);
            float TraceRay(const CVector& org, const CVector& dir);
            float GetHeight(float x, float y);
            void Draw(unsigned int clr);
        };

        using CollInfoVec = retruxx::vector<m3d::Landscape::CollisionInfo*, retruxx::allocator<m3d::Landscape::CollisionInfo*> >;
        class CollInfoVecIter;

    public:
        retruxx::vector<m3d::Landscape::CollisionInfo*, retruxx::allocator<m3d::Landscape::CollisionInfo*> > m_collisions;
        float* m_heightMap;
        short* m_waterMap;
        CVector* m_vnormal;
        retruxx::vector<m3d::rend::IbHandle, retruxx::allocator<m3d::rend::IbHandle> > m_landIbConst;
        m3d::rend::VbHandle m_landUVVb;
        m3d::rend::VbHandle m_landVb;
        m3d::rend::IbHandle m_waterIb[16];
        m3d::rend::VbHandle m_waterVb;
        m3d::rend::IQuery* m_waterQueries[3];
        int m_currWaterQuery;
        bool m_isWaterVisible;
        bool m_currentWaterVis;
        m3d::rend::IbHandle m_shoresIb;
        m3d::rend::VbHandle m_shoresVb;
        m3d::rend::IEffect* m_shoresShader;
        m3d::rend::VbHandle m_solidVb;
        m3d::rend::IbHandle m_solidIb[4];
        m3d::rend::IHlslShader* m_solidVs;
        m3d::rend::IHlslShader* m_solidPs;
        m3d::rend::IHlslShader* m_solidBindVs;
        m3d::rend::IHlslShader* m_solidBindPs;
        float m_bindDevider;
        m3d::rend::IHlslShader* m_solidDeepVs;
        m3d::rend::IHlslShader* m_solidDeepPs;
        m3d::rend::IHlslShader* m_landscapeVs;
        m3d::rend::IHlslShader* m_landscapePsFP;
        m3d::rend::IHlslShader* m_landscapePsSP;
        CMatrix m_matScale;
        int vertsPerCell;
        int trisPerCell[4];
        void BuildSolidLandscape();

        struct WaveSets
        {
            float m_tcomp = -1.0f;
            float m_tlevel = 0.1f;
            float m_tamplitude = 0.25f;
            float m_tphase = 0.0f;
            float m_tfreq = 0.2f;
            float m_scomp = -1.0f;
            float m_slevel = 2.0f;
            float m_samplitude = 0.89999998f;
            float m_sphase = 0.15000001f;
            float m_sfreq = 0.2f;
            rend::TexHandle m_texHandle;
        };

    public:
        retruxx::vector<m3d::Landscape::WaveSets, retruxx::allocator<m3d::Landscape::WaveSets> > m_waves;

        using CVectorVec = retruxx::vector<CVector, retruxx::allocator<CVector> >;

    public:
        retruxx::vector<retruxx::vector<CVector, retruxx::allocator<CVector> >, retruxx::allocator<retruxx::vector<CVector, retruxx::allocator<CVector> > > > m_shoreLines;
        retruxx::set<unsigned int, retruxx::less<unsigned int>, retruxx::allocator<unsigned int> > m_noShoresSet;
        m3d::rend::VertexXYZNCT2* m_dummyVB;
        unsigned short* m_remappedIndices;
        unsigned short* m_lsIndicesDubb;
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
        void PushLsClip(int, int);
        void PopLsClip();

        struct CellParams
        {
            float m_h0;
            float m_h1;
            bool m_iswatercell = false;
            float m_minwater;
            float m_maxwater;
            float m_lodDelta[5];
            bool m_lodDeltaCreated = false;
            float m_normalTheta = 0.0;
            CVector m_normalAverage;
        };

    public:
        int isWaterCell(int x, int y) const;
        unsigned int getLsColorByDist(const CVector&, const CVector&, unsigned char);
        void DrawWaterLayer();
        void QueryWaterVisibility();
        void StartWaterQuery();
        void EndWaterQuery();
        void DrawShoresLayer();
        void DrawShoreLine();
        int ReflectionWaterClipCell(int, int);
        void renderZGuard();
        void drawPolygonOverlayed(CVector2*, int, unsigned int);
        CClipper m_reflectedFrustum;
        m3d::CVar m_lockVis;
        void CreateHeights(m3d::Landscape::CellParams* dest, int ls, int cellSize);
        void CreateLod();
        void ReleaseLod();
        m3d::Landscape::CellParams* m_cellParams;
        m3d::Landscape::CellParams* m_drawedCellParams;
        virtual void HandleCommand(int cmdID, const m3d::CConsoleParams& params) override;
        virtual bool HandleCVar(const m3d::CVar* cvar, const m3d::CConsoleParams& params) override;

        enum VisibilityMode
        {
            VIS_DIRECT = 0,
            VIS_REFLECTION = 1,
            VIS_REFRACTION = 2,
        };

    public:
        void DrawLandScapeTextures(m3d::Landscape::VisibilityMode visMode, bool drawMinimap, bool roadMap);

        enum LandRenderMode
        {
            LRM_DIRECT = 0,
            LRM_REFLECTION = 1,
            LRM_DEEPMAP = 2,
            LRM_BIND = 3,
        };

    public:
        void DrawSolidLandscape(m3d::Landscape::LandRenderMode landMode, int lod);
        m3d::CFlare m_flares;
        bool m_dirtyReflection;
        m3d::rend::TexHandle m_texRtReflection;
        m3d::rend::TexHandle m_texRtRefraction;
        m3d::rend::TexHandle m_baseWaterTex;
        m3d::rend::TexHandle m_texLightmap;
        int m_texNormalMapSize;

        struct AlphaMask
        {
            CStr m_name;
            using maskVector = retruxx::vector<m3d::rend::TexHandle, retruxx::allocator<m3d::rend::TexHandle> >;
            retruxx::vector<m3d::rend::TexHandle, retruxx::allocator<m3d::rend::TexHandle> > m_texMasks[4];
        };

        struct LandType
        {
            CStr m_name;
            int m_passmask;
            int m_alphaset;
            int m_priority;
            retruxx::vector<int, retruxx::allocator<int> > m_texIndices;
        };

    public:
        retruxx::vector<m3d::Landscape::AlphaMask, retruxx::allocator<m3d::Landscape::AlphaMask> > m_AlphaSets;
        retruxx::vector<m3d::Landscape::LandType, retruxx::allocator<m3d::Landscape::LandType> > m_Lands;
        m3d::CIntHash<int> m_hashIdxToPass;
        m3d::CStrHash<int> m_hashAlphaToLand;
        int GetNumAlphas() const;
        CStr GetAlphaName(int) const;
        int GetUsedAlphaIdx(int) const;
        CStr GetUsedAlpha(int) const;
        int GetNumLands() const;
        const CStr& GetLandName(int) const;
        int GetLandByName(const CStr&) const;
        int GetNumTexturesInLand(int) const;
        int GetTexIndexFromLand(int, int) const;
        void DeleteIndexFromLand(int, int);
        int GetPriorityofLand(int) const;
        void SetPriorityofLand(int, int);
        void SetAlphaofLand(int, int);
        int m_CurAlphaSet;
        int AddOneTexture(const CStr& name);
        void UpdateTexturesFilters();
        char* m_passedCells;
        unsigned char* m_cliffHeightMap;
        unsigned int* m_normalMap;
        void AddCollisionTris(int tag, int numVerts, CVector* verts, int numTris, unsigned short* tris, const CMatrix& toWorld);
        void RemoveCollisionTris(int tag);
        void GetVisCellHeights(float& h0, float& h1, int x, int y) const;
        void GetWaterCellHeights(float& h0, float& h1, int x, int z) const;
        void GetDrawedCellHeights(float& h0, float& h1, int x, int y) const;
        bool IsThisVisCellHasWater(int x, int z) const;
        static void __fastcall Register();

    protected:
        m3d::CWorld* m_owner;
        static m3d::RenderModes m_renderMode;
        bool traceLineThruBox(float&, float*, const CVector&, const CVector&);
        bool traceLineThruCellLs0(float& ttt, int cellX, int cellZ, const CVector& start, const CVector& dir);
        bool traceLineThruCellLs(float& ttt, int cellX, int cellZ, const CVector& start, const CVector& dir, bool allowColInfo);

        struct wallCells
        {
            int m_x;
            int m_y;
            wallCells(int, int);
        };

    protected:
        int IsBackfaced(int xx, int yy, m3d::rend::Cull cull);

        enum RenderTypes
        {
            RT_FIRSTPASSLIGHT = 0,
            RT_OTHERPASSES = 1,
            RT_LIGHTPASS = 2,
        };

    protected:
        m3d::Landscape::RenderTypes m_lastState;
        int m_firstpasscounter;
        int m_otherpasscounter;

        struct TIVChunk
        {
            retruxx::vector<m3d::rend::VbHandle, retruxx::allocator<m3d::rend::VbHandle> > m_vbHandle;
            m3d::rend::TexHandle m_texHandle;
            unsigned int m_offsetsmap[65536];
            unsigned short m_banknumber[65536];
            unsigned short m_numCellsPerCellMap[4096];
            int iotherPassOffset = 0;
            int iotherPassBankNumber = 0;
        };

        using TexVec = retruxx::vector<m3d::Landscape::TIVChunk*, retruxx::allocator<m3d::Landscape::TIVChunk*> >;
        class TexVecIter;

    protected:
        retruxx::set<unsigned int, retruxx::less<unsigned int>, retruxx::allocator<unsigned int> >* m_texSetsmap;
        void DrawCells0(const m3d::cmn::vector<unsigned int>& cellsPerTex, m3d::Landscape::RenderTypes RenderType);
        void DrawCellsFast0(const m3d::cmn::vector<unsigned int>& cellsPerTex, m3d::Landscape::TIVChunk& tivchunk, m3d::Landscape::RenderTypes RenderType);
        void BuildCells0(m3d::rend::VertexLandscape* vert, m3d::Landscape::TIVChunk& chunk, int& vofs, const m3d::cmn::vector<unsigned int>& cellsPerTex, m3d::Landscape::RenderTypes RenderType, retruxx::vector<int, retruxx::allocator<int> >& bankSwitchingMap);
        void BuildUVSet();
        void CompleteShores();
        void RecursiveEnableShore(unsigned char* marks, int x, int z);
        void RecursiveDisableShore(unsigned char* marks, int x, int z);

    public:
        void ReBuildShoresVb();
        void FreeShoresStuff();
        m3d::Landscape::CollisionCellItem* GetCollisionCellItem(int x, int y) const;
        m3d::GeomObject* GetTerrainGeomObject() const;
        void ClearCollisionCellsMap();
        void SetPresenceOnCollisionMap(int x, int y);
        void ManageLandScapeCollisionTriMeshes();
        void CheckLandscapeCollisionTriMeshesForObjId(int objId);
        void GetDPVSCollisionInfo(int bcx, int bcz, int& numVertices, int& numIndices);
        void GenerateOneDPVSCellMesh(int bcx, int bcz, CVector* vertexMap, int* indexMap);
        void CreateIndicesTriLists(int* indices, int BaseVertex, int sz);
        int ConstructCollisionData();
        void LinkNodeAndChildrenCollisionGeomsToCell(m3d::SgNode* node);
        void LinkNodeCollisionGeomsToCell(m3d::SgNode* node, int startX, int endX, int startY, int endY);
        void UnlinkNodeCollisionGeomsFromCell(m3d::SgNode* node, int x, int y, bool deleteList);
        void UpdateNodeCollisionGeoms(m3d::SgNode* node);
        void SetNodeCollisionGeomsEnabled(m3d::SgNode* node, bool enabled);
        void ReleaseOdeCollisionData();
        void LinkNodeObstacleToCells(m3d::SgNode* node);
        void LinkObstacleToCells(ai::Obstacle* obstacle);
        void LinkPassMapCellToCollisionCell(const PointBase<int>& cellPos);
        void DrawCollisionGeoms(bool allGeoms);
        void DrawGeom(dxGeom* geom);
        void DrawJoint(dxJoint* joint);
        void DrawMassBox(dMass* mass, const CVector& pos, const Quaternion& rot);
        void GetFogStartAndEnd(float& s, float& e) const;
        static void __fastcall SetRenderMode(m3d::RenderModes);
        void PostServersLoad();
        int GenerateShoreLine();
        void EnableShoreRegion(int x, int z);
        void DisableShoreRegion(int x, int z);
        void ChangeShoreState(int x, int z, bool enable);
        bool traceLineThruWallCells(const CVector&, CVector&, bool);
        void setDrawRadius(int clip0, int clip1, int clip2);
        m3d::rend::IEffect* overlayShader;
        void SetOverlayShader(m3d::rend::IEffect* os);
        int getClip0() const;
        void DrawCells(const m3d::cmn::vector<unsigned int>& cellsPerTex, unsigned int clr);
        void DrawCellsOverlayedEditor(const m3d::cmn::vector<unsigned int>& cellsPerTex, unsigned int clr);
        float getHgtAtHfPoint(int x, int y) const;
        void setHgtAtHfPoint(int x, int y, float h);
        unsigned int getClrAtHfPoint(int, int) const;
        void setClrAtHfPoint(int, int, unsigned int);
        const CVector& getNormalAtPoint(int, int) const;
        void getMinMaxHeightForBox(float* box, float buldgeY);
        static void __fastcall SetGameRenderMode();
        static void __fastcall SetEditorRenderMode();

        struct TileInfo
        {
            int m_texIndex0;
            int m_angle = 0;
            int m_texIndices[4];
            int m_texFlags[4];
            int m_numTexs;
            int m_maskIndex;
            int m_rotate;
        };

        struct DrawCliffInfo
        {
            m3d::AnimatedModel* m_modelToDraw;
            int m_cliffSetId;
            int m_cliffSubIndex;
            int m_cliffType;
            m3d::CliffGeometryTypes m_cliffGeometryType;
            m3d::CliffOpenEndOrientation m_cliffOrientation;
            bool m_mirrored;
            int m_rotate;
            int m_numVertices;
            int m_numIndices;
            unsigned int m_mask;
            float m_heightToDraw;
            CVector m_scale;
            m3d::rend::VbHandle m_vbHandle;
            ~DrawCliffInfo();
            DrawCliffInfo(const m3d::Landscape::DrawCliffInfo&);
            DrawCliffInfo();
        };

        struct CliffSet
        {
            CStr m_name;
            unsigned int m_id;
            float m_height;
            CStr m_baseTileName;
            CStr m_topTileName;
            CVector m_scale;
            retruxx::vector<m3d::AnimatedModel*, retruxx::allocator<m3d::AnimatedModel*> > m_cliffShapeHandles[5];
            ~CliffSet();
        };

    public:
        m3d::rend::TexHandle GetTexByIdx(int) const;

    private:
        m3d::CIntHash<int> m_hashTexToIndex;
        m3d::CStrHash<int> m_texToIdx;
        m3d::CIntHash<int> m_hashIdxToLandType;
        retruxx::vector<m3d::Landscape::TIVChunk*, retruxx::allocator<m3d::Landscape::TIVChunk*> > m_tilesTextures;
        m3d::Landscape::TileInfo* m_tiles;
        unsigned int* m_colormap;
        float m_uvForAngles[4][25][2];

        struct AlphaSetUnit
        {
            float m_uvForAngles[4][25][2];
        };

        struct TextureAlphaSet
        {
            m3d::Landscape::AlphaSetUnit m_sets[8][5];
        };

    private:
        m3d::Landscape::TextureAlphaSet m_setAndUVs;
        m3d::cmn::vector<m3d::cmn::vector<unsigned int> > m_cellsPerTex;
        CStr m_pathTile;
        bool m_loadAllTextures;
        retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> > m_usedTexturesList;

    public:
        m3d::rend::TexHandle GetLightmapTexture() const;
        void ReloadLightmapTexture(const CStr& fileName);
        void SetAllTexturesLoading(bool value);
        void GetTilesLoadParams(m3d::CIntHash<int>**, m3d::CStrHash<int>**, retruxx::vector<m3d::Landscape::TIVChunk*, retruxx::allocator<m3d::Landscape::TIVChunk*> >**);
        void AddIdxToLand(int, int);

    protected:
        int LoadTiles(const CStr& filename);
        void FreeTiles();
        retruxx::vector<int, retruxx::allocator<int> > m_lsNumIndices;
        int m_wtNumTris[16];

    public:
        bool LoadNormalMap(const CStr& FileName);
        bool SaveNormalMap(const CStr& FileName);
        bool LoadShoreLine(const CStr& FileName);
        bool SaveShoreLine(const CStr& FileName);
        void DrawSpotInColorMap(float, float, int);
        unsigned char GetColor(float x, float y);
        void ReadTileInfo(int loadExtraTextures);
        void SaveTileInfo();
        int GetPassForTexture(int) const;
        float GetFloatToShortScale() const;
        const CStr& GetPathToTiles() const;
        int GetLsSize() const;
        int GetTileSize() const;
        float GetScaleForTile() const;
        int GetTileRot(int, int) const;
        int GetTileIndex(int, int) const;
        void SetTileRot(int, int, int);
        void SetTile(int, int, int);
        m3d::rend::TexHandle GetTexHandleFromList(unsigned int num) const;
        int m_drawtextured;
        int RecalcNormalMap(int x, int y, int sizex, int sizey);
        void RecalcUV();
        void CreateHelperStructures();
        int GetNumTiles() const;
        bool Save16bitDisplace(const CStr& Name, int stripe);
        bool SaveColorMap(const CStr& Name, int stripe);
        int SaveCameraMap(const CStr& Name, int stripe);
        short* GetWaterPtr();
        m3d::Landscape::TileInfo* GetTilesPtr();
        float* GetHeightPtr();
        const m3d::Landscape::TileInfo& GetTileInfo(int x, int y) const;
        void SwitchDrawMode();
        float GetLsHeight(float x, float y) const;
        void SetLsHeight(float x, float y, float h);
        float GetHeight(float x, float y, int excludeTag, bool notForCamera);
        float GetHeightWithCollisions(float x, float y, bool forVehicle) const;
        CVector getNormal(float xx, float yy);
        unsigned int GetRotatedInt(unsigned int, int) const;
        bool WaterSave(const CStr&);
        void WaterClear();
        int Load();
        int New(float heightLevel);
        /*virtual*/ void Render();
        virtual int Render(m3d::SgNodeRenderFlags flags, void* data, int dt, int curTime) override;
        void Update();
        void UpdateVis(bool vp);
        void Restore();
        void Invalidate();
        void Release();
        void InitReflectionRefractionTextures();
        void ReloadWaterTextures();
        void ReleaseReflectionRefractionTextures();
        float getWaterHeight(int x, int z) const;
        float getCameraHeight(float x, float z) const;
        void drawSpriteOverlayed(unsigned int clr, const CVector& o, const CVector& scale, float rotZ);
        void drawSpriteOverlayedProjected(unsigned int clr, const CVector& o, const CMatrix& projectorMatrix, const CClipper& clipper);
        void drawSpriteOverlayed2(float cx, float cz, float hsx, float hsz, unsigned int clr, bool all);
        void drawSpriteOverlayed2Projected(float cx, float cz, float hsx, float hsz, unsigned int clr, bool all, const CClipper& clipper);
        void drawCellOverlayedShader(int x, int z, m3d::rend::IEffect* shader);
        void setOwner(m3d::CWorld* cl);
        void ChangedNumberOfUsedTextures(unsigned int numTexs);
        int m_mapSize;

        struct GrassInstance
        {
            CVector pos;
            float scale;
            float sinYaw;
            float cosYaw;
        };

        enum RenderGrassType
        {
            RGT_SIMPLE = 0,
            RGT_FOR_SHADOW = 1,
            RGT_FOR_PROJECTOR = 2,
        };

    public:
        bool InitGrass();
        void DoneGrass();
        bool AddGrassModel(const char* modelFileName);
        unsigned int GetNumGrassModels() const;
        const char* GetGrassModelName(unsigned int modelIdx) const;
        unsigned int AddGrassInstance(int modelId, const CVector& pos, float yaw, float scale, bool placeOnTerrain);
        void RemoveGrassInstance(unsigned int instance);
        void RemoveGrassTile(int x, int z);
        void RemoveGrassRadius(const CVector& pos, float radius);
        void RemoveGrassRectangle(const CVector2& Min, const CVector2& Max);
        void ScaleGrassRadius(const CVector& pos, float radius, float coeff);
        void PutGrassToLandscape(const CVector2& Min, const CVector2& Max);
        unsigned int GetNearestGrassInstance(const CVector& point) const;
        void _dbgGenerateGrass();
        void RenderGrass(unsigned int numVisibleInstances, m3d::Landscape::GrassInstance** visGrassInstances, int* visModelsForGrassInstances, m3d::Landscape::RenderGrassType rgt);
        void RenderGrass(const retruxx::deque<retruxx::pair<int, int>, retruxx::allocator<retruxx::pair<int, int> > >& excludedCells);
        void CollectGrassCell(int x, int z, unsigned int& numVisibleInstances, m3d::Landscape::GrassInstance** visGrassInstances, int* visModelsForGrassInstances);
        int getGrassModelIdByName(const char* modelFileName) const;
        bool WriteGrassToXmlFile(const char* fileName);
        void ReadGrassFromXmlFile(const char* fileName);

    private:
        m3d::Profiler* m_profilerDraw;
        m3d::Profiler* m_profilerUpdateVis;
        m3d::Profiler* m_profilerDrawGrass;
        m3d::Profiler* m_profilerDrawWater;
        m3d::DbgCounter* m_countPhysicObjsInCells;
        m3d::rend::TexHandle m_waveBumpTex;
        m3d::rend::TexHandle m_waveBumpSmTex;
        m3d::rend::TexHandle m_fresnelTex;
        m3d::rend::TexHandle m_waveReflMap;
        m3d::rend::IHlslShader* m_waterVs;
        m3d::rend::IHlslShader* m_waterPs;
        m3d::rend::IHlslShader* m_waterDumbVs;
        m3d::rend::IHlslShader* m_waterDumbPs;
        int m_waterShaderVersion;
        int m_maxWaterCellPerPass;

        using CellsToDraw = retruxx::vector<retruxx::pair<unsigned int, float>, retruxx::allocator<retruxx::pair<unsigned int, float> > >;

    private:
        retruxx::vector<retruxx::pair<unsigned int, float>, retruxx::allocator<retruxx::pair<unsigned int, float> > > waterCellsToDraw[16];
        CVector4* waterTileInfo;
        void DrawNonTransformGeom(dxGeom* geom);
        m3d::rend::IHlslShader* m_grassVs;
        m3d::rend::IHlslShader* m_grassPs;
        m3d::Landscape::VisibilityMode m_curVisMode;
        void SetCurVisMode(m3d::Landscape::VisibilityMode visMode);
        m3d::Landscape::VisibilityMode GetCurVisMode() const;
        void RenderRoads();

        struct GrassInstancesForModel
        {
            int modelId;
            int numInstances;
            retruxx::vector<m3d::Landscape::GrassInstance*, retruxx::allocator<m3d::Landscape::GrassInstance*> > grass;
        };

        struct TileGrass
        {
            unsigned int numDiffModels;
            unsigned int numInstances;
            retruxx::vector<m3d::Landscape::GrassInstancesForModel*, retruxx::allocator<m3d::Landscape::GrassInstancesForModel*> > instancesPerModel;
        };

    public:
        m3d::Landscape::TileGrass** m_grassArray;
        unsigned int m_numGrassModels;
    };
}

// Scratch buffers the grass passes collect a cell into before handing it to
// Landscape::RenderGrass. They live in the global namespace in the original
// (0xA37410 and 0xA354D0), shared between the landscape and the shadow code.
int const MAX_VISIBLE_GRASS_INSTANCES = 2000;
extern m3d::Landscape::GrassInstance* visGrassInstances[MAX_VISIBLE_GRASS_INSTANCES];
extern int visModelsForGrassInstances[MAX_VISIBLE_GRASS_INSTANCES];

bool intersectTriangle(CVector const& orig, CVector const& dir, CVector const& a, CVector const& b, CVector const& c, float& t, float& u, float& v);

struct GrassModelInfo
{
    m3d::rend::VbHandle vb;
    m3d::rend::IbHandle ib;
    m3d::rend::TexHandle tex;
    unsigned short numVerts;
    unsigned short numTris;
    unsigned short numIndices;
    CStr modelName;
    float boundRadius;
};

// 0xA17A30 - a fixed table, not a growable one; m_numGrassModels says how many
// of the slots are in use.
int const MAX_GRASS_MODELS = 20;
extern GrassModelInfo m_grassModels[MAX_GRASS_MODELS];

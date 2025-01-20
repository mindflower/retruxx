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
    class AnimatedModel;

    namespace cmn
    {
        template<class T>
        struct vector
        {
            vector();
            ~vector();
            void Allocate(int sz);
            void Deallocate();
            void clear();
            T& operator[](int);
            const T& operator[](int) const;
            void push_back(const T& shit);
            int empty() const;
            int size() const;
            /* 0x0000 */ T* m_data;
            /* 0x0004 */ int m_numItems;
            /* 0x0008 */ int m_maxItems;
        }; /* size: 0x000c */
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

    protected:
        Landscape();
        Landscape(const m3d::Landscape& ls);

    public:
        virtual ~Landscape() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classLandscape;

        using GeomObjectList = retruxx::set<m3d::GeomObject*, retruxx::less<m3d::GeomObject*>, retruxx::allocator<m3d::GeomObject*> >;

        struct CollisionCellItem
        {
            /* 0x0000 */ retruxx::set<m3d::GeomObject*, retruxx::less<m3d::GeomObject*>, retruxx::allocator<m3d::GeomObject*> > m_geomsList;
            /* 0x000c */ bool m_wasEnabledLastFrame;
            /* 0x000d */ bool m_bMustCheck;
            CollisionCellItem(const m3d::Landscape::CollisionCellItem&);
            CollisionCellItem();
            ~CollisionCellItem();
            void InsertPhysicObjId(int objId);
            void ErasePhysicObjId(int objId);
            void InsertObstacle(ai::Obstacle* ob);
            void EraseObstacle(ai::Obstacle* ob);
            const retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& GetPhysicObjIds() const;
            const retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >& GetObstacles() const;
            /* 0x000e */ char Padding_152[2];
            /* 0x0010 */ retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> > m_physicObjIds;
            /* 0x001c */ retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >* m_obstacles;
        }; /* size: 0x0020 */

    public:
        /* 0x01d8 */ m3d::Landscape::CollisionCellItem** m_oCollisionitems;
        /* 0x01dc */ m3d::GeomObject* m_terrainObject;
        /* 0x01e0 */ int m_maxLOD;

        class CollisionInfo
        {
        public:
            /* 0x0000 */ int m_tag;
            /* 0x0004 */ int m_numVerts;
            /* 0x0008 */ CVector* m_verts;
            /* 0x000c */ int m_numTris;
            /* 0x0010 */ unsigned short* m_tris;
            /* 0x0014 */ Aabb m_box;
            /* 0x002c */ Obb m_obb;
            CollisionInfo(const m3d::Landscape::CollisionInfo&);
            CollisionInfo();
            ~CollisionInfo();
            void Create(int numVerts, CVector* verts, int numTris, unsigned short* tris, const CMatrix& toWorld);
            float TraceRay(const CVector& org, const CVector& dir);
            float GetHeight(float x, float y);
            void Draw(unsigned int clr);
        }; /* size: 0x0074 */

        using CollInfoVec = retruxx::vector<m3d::Landscape::CollisionInfo*, retruxx::allocator<m3d::Landscape::CollisionInfo*> >;
        class CollInfoVecIter;

    public:
        /* 0x01e4 */ retruxx::vector<m3d::Landscape::CollisionInfo*, retruxx::allocator<m3d::Landscape::CollisionInfo*> > m_collisions;
        /* 0x01f4 */ float* m_heightMap;
        /* 0x01f8 */ short* m_waterMap;
        /* 0x01fc */ CVector* m_vnormal;
        /* 0x0200 */ retruxx::vector<m3d::rend::IbHandle, retruxx::allocator<m3d::rend::IbHandle> > m_landIbConst;
        /* 0x0210 */ m3d::rend::VbHandle m_landUVVb;
        /* 0x0214 */ m3d::rend::VbHandle m_landVb;
        /* 0x0218 */ m3d::rend::IbHandle m_waterIb[16];
        /* 0x0258 */ m3d::rend::VbHandle m_waterVb;
        /* 0x025c */ m3d::rend::IQuery* m_waterQueries[3];
        /* 0x0268 */ int m_currWaterQuery;
        /* 0x026c */ bool m_isWaterVisible;
        /* 0x026d */ bool m_currentWaterVis;
        /* 0x026e */ char Padding_157[2];
        /* 0x0270 */ m3d::rend::IbHandle m_shoresIb;
        /* 0x0274 */ m3d::rend::VbHandle m_shoresVb;
        /* 0x0278 */ m3d::rend::IEffect* m_shoresShader;
        /* 0x027c */ m3d::rend::VbHandle m_solidVb;
        /* 0x0280 */ m3d::rend::IbHandle m_solidIb[4];
        /* 0x0290 */ m3d::rend::IHlslShader* m_solidVs;
        /* 0x0294 */ m3d::rend::IHlslShader* m_solidPs;
        /* 0x0298 */ m3d::rend::IHlslShader* m_solidBindVs;
        /* 0x029c */ m3d::rend::IHlslShader* m_solidBindPs;
        /* 0x02a0 */ float m_bindDevider;
        /* 0x02a4 */ m3d::rend::IHlslShader* m_solidDeepVs;
        /* 0x02a8 */ m3d::rend::IHlslShader* m_solidDeepPs;
        /* 0x02ac */ m3d::rend::IHlslShader* m_landscapeVs;
        /* 0x02b0 */ m3d::rend::IHlslShader* m_landscapePsFP;
        /* 0x02b4 */ m3d::rend::IHlslShader* m_landscapePsSP;
        /* 0x02b8 */ CMatrix m_matScale;
        /* 0x02f8 */ int vertsPerCell;
        /* 0x02fc */ int trisPerCell[4];
        void BuildSolidLandscape();

        struct WaveSets
        {
            /* 0x0000 */ float m_tcomp = -1.0;
            /* 0x0004 */ float m_tlevel = 0.1;
            /* 0x0008 */ float m_tamplitude = 0.25;
            /* 0x000c */ float m_tphase = 0.0;
            /* 0x0010 */ float m_tfreq = 0.2;
            /* 0x0014 */ float m_scomp = -1.0;
            /* 0x0018 */ float m_slevel = 2.0;
            /* 0x001c */ float m_samplitude = 0.89999998;
            /* 0x0020 */ float m_sphase = 0.15000001;
            /* 0x0024 */ float m_sfreq = 0.2;
            /* 0x0028 */ rend::TexHandle m_texHandle;
            WaveSets(const m3d::Landscape::WaveSets& __that);
            WaveSets();
        }; /* size: 0x002c */

    public:
        /* 0x030c */ retruxx::vector<m3d::Landscape::WaveSets, retruxx::allocator<m3d::Landscape::WaveSets> > m_waves;

        using CVectorVec = retruxx::vector<CVector, retruxx::allocator<CVector> >;

    public:
        /* 0x031c */ retruxx::vector<retruxx::vector<CVector, retruxx::allocator<CVector> >, retruxx::allocator<retruxx::vector<CVector, retruxx::allocator<CVector> > > > m_shoreLines;
        /* 0x032c */ retruxx::set<unsigned int, retruxx::less<unsigned int>, retruxx::allocator<unsigned int> > m_noShoresSet;
        /* 0x0338 */ m3d::rend::VertexXYZNCT2* m_dummyVB;
        /* 0x033c */ unsigned short* m_remappedIndices;
        /* 0x0340 */ unsigned short* m_lsIndicesDubb;
        /* 0x0344 */ int m_ver;
        /* 0x0348 */ CPlane m_waterPlane;
        /* 0x0360 */ CClipper m_frustumCull;
        /* 0x0778 */ int m_numWaterCells;
        /* 0x077c */ int m_drawRadius;
        /* 0x0780 */ int m_landscapeClip0;
        /* 0x0784 */ int m_landscapeClip1;
        /* 0x0788 */ int m_landscapeClip2;
        /* 0x078c */ float m_landscapeClip0z;
        /* 0x0790 */ float m_landscapeClip1z;
        /* 0x0794 */ float m_landscapeClip2z;
        /* 0x0798 */ float m_landscapeClip0zSq;
        /* 0x079c */ float m_landscapeClip1zSq;
        /* 0x07a0 */ float m_landscapeClip2zSq;
        /* 0x07a4 */ int m_saveClip[3];
        void PushLsClip(int, int);
        void PopLsClip();

        struct CellParams
        {
            CellParams(const m3d::Landscape::CellParams&);
            CellParams();
            /* 0x0000 */ float m_h0;
            /* 0x0004 */ float m_h1;
            /* 0x0008 */ bool m_iswatercell;
            /* 0x0009 */ char Padding_153[3];
            /* 0x000c */ float m_minwater;
            /* 0x0010 */ float m_maxwater;
            /* 0x0014 */ float m_lodDelta[5];
            /* 0x0028 */ bool m_lodDeltaCreated;
            /* 0x0029 */ char Padding_154[3];
            /* 0x002c */ float m_normalTheta;
            /* 0x0030 */ CVector m_normalAverage;
        }; /* size: 0x003c */

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
        /* 0x07b0 */ CClipper m_reflectedFrustum;
        /* 0x0bc8 */ m3d::CVar m_lockVis;
        void CreateHeights(m3d::Landscape::CellParams* dest, int ls, int cellSize);
        void CreateLod();
        void ReleaseLod();
        /* 0x0bf4 */ m3d::Landscape::CellParams* m_cellParams;
        /* 0x0bf8 */ m3d::Landscape::CellParams* m_drawedCellParams;
        virtual void HandleCommand(int cmdID, const m3d::CConsoleParams& params) override /* 0x00 */;
        virtual bool HandleCVar(const m3d::CVar* cvar, const m3d::CConsoleParams& params) override /* 0x04 */;

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
        /* 0x0bfc */ m3d::CFlare m_flares;
        /* 0x0c14 */ bool m_dirtyReflection;
        /* 0x0c15 */ char Padding_158[3];
        /* 0x0c18 */ m3d::rend::TexHandle m_texRtReflection;
        /* 0x0c1c */ m3d::rend::TexHandle m_texRtRefraction;
        /* 0x0c20 */ m3d::rend::TexHandle m_baseWaterTex;
        /* 0x0c24 */ m3d::rend::TexHandle m_texLightmap;
        /* 0x0c28 */ int m_texNormalMapSize;

        struct AlphaMask
        {
            /* 0x0000 */ CStr m_name;
            using maskVector = retruxx::vector<m3d::rend::TexHandle, retruxx::allocator<m3d::rend::TexHandle> >;
            /* 0x000c */ retruxx::vector<m3d::rend::TexHandle, retruxx::allocator<m3d::rend::TexHandle> > m_texMasks[4];
        }; /* size: 0x004c */

        struct LandType
        {
            /* 0x0000 */ CStr m_name;
            /* 0x000c */ int m_passmask;
            /* 0x0010 */ int m_alphaset;
            /* 0x0014 */ int m_priority;
            /* 0x0018 */ retruxx::vector<int, retruxx::allocator<int> > m_texIndices;
        }; /* size: 0x0028 */

    public:
        /* 0x0c2c */ retruxx::vector<m3d::Landscape::AlphaMask, retruxx::allocator<m3d::Landscape::AlphaMask> > m_AlphaSets;
        /* 0x0c3c */ retruxx::vector<m3d::Landscape::LandType, retruxx::allocator<m3d::Landscape::LandType> > m_Lands;
        /* 0x0c4c */ m3d::CIntHash<int> m_hashIdxToPass;
        /* 0x0c74 */ m3d::CStrHash<int> m_hashAlphaToLand;
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
        /* 0x0c80 */ int m_CurAlphaSet;
        int AddOneTexture(const CStr& name);
        void UpdateTexturesFilters();
        /* 0x0c84 */ char* m_passedCells;
        /* 0x0c88 */ unsigned char* m_cliffHeightMap;
        /* 0x0c8c */ unsigned int* m_normalMap;
        void AddCollisionTris(int tag, int numVerts, CVector* verts, int numTris, unsigned short* tris, const CMatrix& toWorld);
        void RemoveCollisionTris(int tag);
        void GetVisCellHeights(float& h0, float& h1, int x, int y) const;
        void GetWaterCellHeights(float& h0, float& h1, int x, int z) const;
        void GetDrawedCellHeights(float& h0, float& h1, int x, int y) const;
        bool IsThisVisCellHasWater(int x, int z) const;
        static void __fastcall Register();

    protected:
        /* 0x0c90 */ m3d::CWorld* m_owner;
        static m3d::RenderModes m_renderMode;
        bool traceLineThruBox(float&, float*, const CVector&, const CVector&);
        bool traceLineThruCellLs0(float& ttt, int cellX, int cellZ, const CVector& start, const CVector& dir);
        bool traceLineThruCellLs(float& ttt, int cellX, int cellZ, const CVector& start, const CVector& dir, bool allowColInfo);

        struct wallCells
        {
            /* 0x0000 */ int m_x;
            /* 0x0004 */ int m_y;
            wallCells(int, int);
        }; /* size: 0x0008 */

    protected:
        int IsBackfaced(int xx, int yy, m3d::rend::Cull cull);

        enum RenderTypes
        {
            RT_FIRSTPASSLIGHT = 0,
            RT_OTHERPASSES = 1,
            RT_LIGHTPASS = 2,
        };

    protected:
        /* 0x0c94 */ m3d::Landscape::RenderTypes m_lastState;
        /* 0x0c98 */ int m_firstpasscounter;
        /* 0x0c9c */ int m_otherpasscounter;

        struct TIVChunk
        {
            TIVChunk(const m3d::Landscape::TIVChunk&);
            TIVChunk();
            /* 0x0000 */ retruxx::vector<m3d::rend::VbHandle, retruxx::allocator<m3d::rend::VbHandle> > m_vbHandle;
            /* 0x0010 */ m3d::rend::TexHandle m_texHandle;
            /* 0x0014 */ unsigned int m_offsetsmap[65536];
            /* 0x40014 */ unsigned short m_banknumber[65536];
            /* 0x60014 */ unsigned short m_numCellsPerCellMap[4096];
            /* 0x62014 */ int iotherPassOffset;
            /* 0x62018 */ int iotherPassBankNumber;
        }; /* size: 0x6201c */

        using TexVec = retruxx::vector<m3d::Landscape::TIVChunk*, retruxx::allocator<m3d::Landscape::TIVChunk*> >;
        class TexVecIter;

    protected:
        /* 0x0ca0 */ retruxx::set<unsigned int, retruxx::less<unsigned int>, retruxx::allocator<unsigned int> >* m_texSetsmap;
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
        /* 0x0ca4 */ m3d::rend::IEffect* overlayShader;
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
            /* 0x0000 */ int m_texIndex0;
            /* 0x0004 */ int m_angle;
            /* 0x0008 */ int m_texIndices[4];
            /* 0x0018 */ int m_texFlags[4];
            /* 0x0028 */ int m_numTexs;
            /* 0x002c */ int m_maskIndex;
            /* 0x0030 */ int m_rotate;
            TileInfo();
            ~TileInfo();
        }; /* size: 0x0034 */

        struct DrawCliffInfo
        {
            /* 0x0000 */ m3d::AnimatedModel* m_modelToDraw;
            /* 0x0004 */ int m_cliffSetId;
            /* 0x0008 */ int m_cliffSubIndex;
            /* 0x000c */ int m_cliffType;
            /* 0x0010 */ m3d::CliffGeometryTypes m_cliffGeometryType;
            /* 0x0014 */ m3d::CliffOpenEndOrientation m_cliffOrientation;
            /* 0x0018 */ bool m_mirrored;
            /* 0x0019 */ char Padding_156[3];
            /* 0x001c */ int m_rotate;
            /* 0x0020 */ int m_numVertices;
            /* 0x0024 */ int m_numIndices;
            /* 0x0028 */ unsigned int m_mask;
            /* 0x002c */ float m_heightToDraw;
            /* 0x0030 */ CVector m_scale;
            /* 0x003c */ m3d::rend::VbHandle m_vbHandle;
            ~DrawCliffInfo();
            DrawCliffInfo(const m3d::Landscape::DrawCliffInfo&);
            DrawCliffInfo();
        }; /* size: 0x0040 */

        struct CliffSet
        {
            /* 0x0000 */ CStr m_name;
            /* 0x000c */ unsigned int m_id;
            /* 0x0010 */ float m_height;
            /* 0x0014 */ CStr m_baseTileName;
            /* 0x0020 */ CStr m_topTileName;
            /* 0x002c */ CVector m_scale;
            /* 0x0038 */ retruxx::vector<m3d::AnimatedModel*, retruxx::allocator<m3d::AnimatedModel*> > m_cliffShapeHandles[5];
            ~CliffSet();
        }; /* size: 0x0088 */

    public:
        m3d::rend::TexHandle GetTexByIdx(int) const;

    private:
        /* 0x0ca8 */ m3d::CIntHash<int> m_hashTexToIndex;
        /* 0x0cd0 */ m3d::CStrHash<int> m_texToIdx;
        /* 0x0cdc */ m3d::CIntHash<int> m_hashIdxToLandType;
        /* 0x0d04 */ retruxx::vector<m3d::Landscape::TIVChunk*, retruxx::allocator<m3d::Landscape::TIVChunk*> > m_tilesTextures;
        /* 0x0d14 */ m3d::Landscape::TileInfo* m_tiles;
        /* 0x0d18 */ unsigned int* m_colormap;
        /* 0x0d1c */ float m_uvForAngles[2][25][4];

        struct AlphaSetUnit
        {
            /* 0x0000 */ float m_uvForAngles[2][25][4];
        }; /* size: 0x0320 */

        struct TextureAlphaSet
        {
            /* 0x0000 */ m3d::Landscape::AlphaSetUnit m_sets[5][8];
        }; /* size: 0x7d00 */

    private:
        /* 0x103c */ m3d::Landscape::TextureAlphaSet m_setAndUVs;
        /* 0x8d3c */ m3d::cmn::vector<m3d::cmn::vector<unsigned int> > m_cellsPerTex;
        /* 0x8d48 */ CStr m_pathTile;
        /* 0x8d54 */ bool m_loadAllTextures;
        /* 0x8d55 */ char Padding_159[3];
        /* 0x8d58 */ retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> > m_usedTexturesList;

    public:
        m3d::rend::TexHandle GetLightmapTexture() const;
        void ReloadLightmapTexture(const CStr& fileName);
        void SetAllTexturesLoading(bool value);
        void GetTilesLoadParams(m3d::CIntHash<int>**, m3d::CStrHash<int>**, retruxx::vector<m3d::Landscape::TIVChunk*, retruxx::allocator<m3d::Landscape::TIVChunk*> >**);
        void AddIdxToLand(int, int);

    protected:
        int LoadTiles(const CStr& filename);
        void FreeTiles();
        /* 0x8d64 */ retruxx::vector<int, retruxx::allocator<int> > m_lsNumIndices;
        /* 0x8d74 */ int m_wtNumTris[16];

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
        /* 0x8db4 */ int m_drawtextured;
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
        /*virtual*/ void Render() /* 0x00 */;
        virtual int Render(m3d::SgNodeRenderFlags flags, void* data, int dt, int curTime) override /* 0x00 */;
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
        /* 0x8db8 */ int m_mapSize;

        struct GrassInstance
        {
            /* 0x0000 */ CVector pos;
            /* 0x000c */ float scale;
            /* 0x0010 */ float sinYaw;
            /* 0x0014 */ float cosYaw;
        }; /* size: 0x0018 */

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
        /* 0x8dbc */ m3d::Profiler* m_profilerDraw;
        /* 0x8dc0 */ m3d::Profiler* m_profilerUpdateVis;
        /* 0x8dc4 */ m3d::Profiler* m_profilerDrawGrass;
        /* 0x8dc8 */ m3d::Profiler* m_profilerDrawWater;
        /* 0x8dcc */ m3d::DbgCounter* m_countPhysicObjsInCells;
        /* 0x8dd0 */ m3d::rend::TexHandle m_waveBumpTex;
        /* 0x8dd4 */ m3d::rend::TexHandle m_waveBumpSmTex;
        /* 0x8dd8 */ m3d::rend::TexHandle m_fresnelTex;
        /* 0x8ddc */ m3d::rend::TexHandle m_waveReflMap;
        /* 0x8de0 */ m3d::rend::IHlslShader* m_waterVs;
        /* 0x8de4 */ m3d::rend::IHlslShader* m_waterPs;
        /* 0x8de8 */ m3d::rend::IHlslShader* m_waterDumbVs;
        /* 0x8dec */ m3d::rend::IHlslShader* m_waterDumbPs;
        /* 0x8df0 */ int m_waterShaderVersion;
        /* 0x8df4 */ int m_maxWaterCellPerPass;

        using CellsToDraw = retruxx::vector<retruxx::pair<unsigned int, float>, retruxx::allocator<retruxx::pair<unsigned int, float> > >;

    private:
        /* 0x8df8 */ retruxx::vector<retruxx::pair<unsigned int, float>, retruxx::allocator<retruxx::pair<unsigned int, float> > > waterCellsToDraw[16];
        /* 0x8ef8 */ CVector4* waterTileInfo;
        void DrawNonTransformGeom(dxGeom* geom);
        /* 0x8efc */ m3d::rend::IHlslShader* m_grassVs;
        /* 0x8f00 */ m3d::rend::IHlslShader* m_grassPs;
        /* 0x8f04 */ m3d::Landscape::VisibilityMode m_curVisMode;
        void SetCurVisMode(m3d::Landscape::VisibilityMode visMode);
        m3d::Landscape::VisibilityMode GetCurVisMode() const;
        void RenderRoads();

        struct GrassInstancesForModel
        {
            /* 0x0000 */ int modelId;
            /* 0x0004 */ int numInstances;
            /* 0x0008 */ retruxx::vector<m3d::Landscape::GrassInstance*, retruxx::allocator<m3d::Landscape::GrassInstance*> > grass;
        }; /* size: 0x0018 */

        struct TileGrass
        {
            /* 0x0000 */ unsigned int numDiffModels;
            /* 0x0004 */ unsigned int numInstances;
            /* 0x0008 */ retruxx::vector<m3d::Landscape::GrassInstancesForModel*, retruxx::allocator<m3d::Landscape::GrassInstancesForModel*> > instancesPerModel;
        }; /* size: 0x0018 */

    public:
        /* 0x8f08 */ m3d::Landscape::TileGrass** m_grassArray;
        /* 0x8f0c */ unsigned int m_numGrassModels;
    }; /* size: 0x8f10 */

    static_assert(sizeof(Landscape) == 0x8f10);
}

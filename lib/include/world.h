#pragma once
#include <math/vector.h>
#include <landscape.h>
#include <map>
#include <core/ref_ptr.h>
#include <core/console/cvar.h>
#include <engine/landscape/roads/roadmanager.h>
#include <engine/weather/weathermanager.h>
#include <engine/wheeltraces/wheeltraces.h>
#include <renderer/i_renderer.h>
#include <scene/scenegraph.h>

struct dxSpace;
class CCamera;

namespace ai
{
    class Vehicle;
}

namespace m3d
{
    class Profiler;
    class Level;
    class CClient;

    class IGeneratedTexture
    {
    };

    class TexClouds : public IGeneratedTexture
    {
    private:
        rend::TexHandle m_texBasicNoise[3];
        rend::TexHandle m_texPrevBasicNoise[3];
        rend::TexHandle m_texCurrentInterpolatedNoise[3];
        rend::TexHandle m_texCurrentCompiledOctavesNoise;
        rend::TexHandle m_texClouds;
        float m_cloudSpeeds[3];
        rend::VertexXYZWCT1 m_vertsCompositRects[3][6];
        int m_elapsedTime[3];
        int m_updateInterval[3];
        int m_totalElapsedTime;
        int m_density;
        rend::VbHandle m_vb;
        int m_vofs;
    };

    class CWorld
    {
        friend class Landscape;
        // SceneGraph::DrawDetailedShadows / DrawShadowsToTexture read m_sunDir
        // directly, exactly as the shipped code does.
        friend class SceneGraph;

    public:
        /* 0x0000 */ m3d::CVar m_lsInscatterCoeff;
        /* 0x002c */ m3d::CVar m_lsOutscatterCoeff;
        /* 0x0058 */ m3d::CVar m_skyInscatterCoeff;
        /* 0x0084 */ m3d::CVar m_skyOutscatterCoeff;
        /* 0x00b0 */ m3d::CVar m_sunColorR;
        /* 0x00dc */ m3d::CVar m_sunColorG;
        /* 0x0108 */ m3d::CVar m_sunColorB;
        /* 0x0134 */ m3d::CVar m_fogStart;
        /* 0x0160 */ bool m_isWeatherActual;
        /* 0x0161 */ char Padding_168[3];
        /* 0x0164 */ dxGeom* m_borderWallGeoms[6];

    protected:
        /* 0x017c */ m3d::Landscape m_landscape;
        /* 0x908c */ char Padding_169[116];
        /* 0x9100 */ m3d::SceneGraph m_sceneGraph;
        /* 0x39e980 */ m3d::RoadManager m_roadManager;
        /* 0x39e99c */ m3d::WeatherManager m_weatherManager;
        /* 0x39e9fc */ m3d::WheelTraceMgr m_wheelTracesMgr;
        /* 0x39ea2c */ int m_lastId;

    public:
        int GetLastID();
        void SetLastID(int);
        dxSpace* GetOdeSpace();

    protected:
        /* 0x39ea30 */ m3d::rend::TexHandle m_texMiniMap;
        /* 0x39ea34 */ m3d::rend::TexHandle m_texSunGlow;
        /* 0x39ea38 */ m3d::TexClouds* m_texClouds;
        bool LoadWorld(const CStr& filename);

    public:
        void ProcessCollisionStuff();
        void ProcessCollisionStuffOnNode(m3d::SgNode* node);
        bool SaveWorld(const CStr& filename);
        void UpdateSkyParams();
        int RenderSky(m3d::Landscape::LandRenderMode rendMode);
        unsigned int GetWeatherFogColor() const;
        unsigned int GetWeatherAmbientColor() const;
        unsigned int GetWeatherDiffuseColor() const;
        unsigned int GetWeatherPlantColor() const;
        unsigned int GetWeatherSunColor() const;
        unsigned int GetWeatherSpecularColor() const;
        m3d::WeatherManager& GetWeatherManager();
        m3d::GlobalTimeParams GetCurDayTimeFromWeatherManager() const;
        float GetSForShadowsFromWeather() const;
        float GetFogReduceFactorFromWeather() const;
        float GetShadowTransparencyFromWeather() const;
        bool GetShadowVisibilityFromWeather() const;

    protected:
        /* 0x39ea3c */ m3d::CClient* m_owner;

    public:
        /* 0x39ea40 */ m3d::Level* m_level;
        static void __fastcall Register();
        void SetOwner(m3d::CClient* cl);
        const CVector& GetSun(float azimuth) const;
        float GetSunAscention() const;
        CVector GetScatteringSunColor() const;
        void UpdateSun();

    protected:
        /* 0x39ea44 */ CVector m_sunDir;
        /* 0x39ea50 */ float m_sunAzimuth;
        /* 0x39ea54 */ float m_sunAscention;

    public:
        void RefreshObjectsOnLandscapeRect(int, int, int, int);
        void RefreshObjectsOnLandscapeRect(const CVector& org, float r);
        CWorld();
        ~CWorld();
        m3d::SceneGraph& GetGraph();
        m3d::Landscape& GetLandscape();
        m3d::RoadManager& GetRoadManager();
        m3d::WheelTraceMgr& GetWheelTracesMgr();
        void Release();
        void New(CCamera& cam, int lsSize, float hgt0);
        int Load(const CStr& levelname, CCamera& Cam, bool bQuiet);
        void Render();
        void Update();
        int CreateGeomsRepresentingSceneNodes();
        ai::Vehicle* GetVehicleControlledByPlayer();
        m3d::rend::TexHandle GetMiniMapTex();
        void Invalidate();
        void Restore();

        struct EffectsData
        {
            /* 0x0000 */ bool onlyOne;
            /* 0x0001 */ char Padding_166[3];
            /* 0x0004 */ m3d::SgNode* effect;
            /* 0x0008 */ retruxx::vector<m3d::SgNode*, retruxx::allocator<m3d::SgNode*> > effects;
        }; /* size: 0x0018 */

        using FxRemap = retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > >;
        using FxVector = retruxx::vector<m3d::CWorld::EffectsData, retruxx::allocator<m3d::CWorld::EffectsData> >;

    protected:
        /* 0x39ea58 */ retruxx::vector<m3d::CWorld::EffectsData, retruxx::allocator<m3d::CWorld::EffectsData> > m_effectsFactory;
        /* 0x39ea68 */ retruxx::vector<CStr, retruxx::allocator<CStr> > fxNames;
        /* 0x39ea78 */ retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > > fxRemap;
        m3d::SgNode* ReadPrefab(ref_ptr<m3d::cmn::XmlFile> file, ref_ptr<m3d::cmn::XmlNode> pnode);

    public:
        const retruxx::vector<CStr, retruxx::allocator<CStr> >& GetFxNames() const;
        int GetFxId(const CStr& fxName);
        m3d::SgNode* CreatePrefabsNode(int type);
        int CreatePrefabsFromFile(const char* fileName);
        void ReleasePrefabs();

    private:
        /* 0x39ea84 */ m3d::Profiler* m_profilerUpdateOde;
        /* 0x39ea88 */ char Padding_170[120];

        void LoadStaticObstacles();
        void CreatePlayerPassMapGeoms();
    }; /* size: 0x39eb00 */

    static_assert(sizeof(CWorld) == 0x39eb00);
}

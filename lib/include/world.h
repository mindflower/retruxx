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
        // Application::SaveUsedOnlyServers walks the scene graph directly.
        friend class Application;
        // SceneGraph::DrawDetailedShadows / DrawShadowsToTexture read m_sunDir
        // directly, exactly as the shipped code does.
        friend class SceneGraph;
        // AnimatedModelsServer::RenderShadowVolumesSet lights the stencil shadows from m_sunDir.
        friend class AnimatedModelsServer;
        // ProjectorsServer::RenderItem draws each projector over the landscape, roads and models.
        friend class ProjectorsServer;
        // GameAttractor blows particles along with the current weather's wind.
        friend class GameAttractor;

    public:
        m3d::CVar m_lsInscatterCoeff;
        m3d::CVar m_lsOutscatterCoeff;
        m3d::CVar m_skyInscatterCoeff;
        m3d::CVar m_skyOutscatterCoeff;
        m3d::CVar m_sunColorR;
        m3d::CVar m_sunColorG;
        m3d::CVar m_sunColorB;
        m3d::CVar m_fogStart;
        bool m_isWeatherActual;
        char Padding_168[3];
        dxGeom* m_borderWallGeoms[6];

    protected:
        m3d::Landscape m_landscape;
        char Padding_169[116];
        m3d::SceneGraph m_sceneGraph;
        m3d::RoadManager m_roadManager;
        m3d::WeatherManager m_weatherManager;
        m3d::WheelTraceMgr m_wheelTracesMgr;
        int m_lastId;

    public:
        int GetLastID();
        void SetLastID(int);
        dxSpace* GetOdeSpace();

    protected:
        m3d::rend::TexHandle m_texMiniMap;
        m3d::rend::TexHandle m_texSunGlow;
        m3d::TexClouds* m_texClouds;
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
        m3d::CClient* m_owner;

    public:
        m3d::Level* m_level;
        static void __fastcall Register();
        void SetOwner(m3d::CClient* cl);
        const CVector& GetSun(float azimuth) const;
        float GetSunAscention() const;
        CVector GetScatteringSunColor() const;
        void UpdateSun();

    protected:
        CVector m_sunDir;
        float m_sunAzimuth;
        float m_sunAscention;

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
            bool onlyOne;
            char Padding_166[3];
            m3d::SgNode* effect;
            retruxx::vector<m3d::SgNode*, retruxx::allocator<m3d::SgNode*> > effects;
        };

        using FxRemap = retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > >;
        using FxVector = retruxx::vector<m3d::CWorld::EffectsData, retruxx::allocator<m3d::CWorld::EffectsData> >;

    protected:
        retruxx::vector<m3d::CWorld::EffectsData, retruxx::allocator<m3d::CWorld::EffectsData> > m_effectsFactory;
        retruxx::vector<CStr, retruxx::allocator<CStr> > fxNames;
        retruxx::map<CStr, int, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, int> > > fxRemap;
        m3d::SgNode* ReadPrefab(ref_ptr<m3d::cmn::XmlFile> file, ref_ptr<m3d::cmn::XmlNode> pnode);

    public:
        const retruxx::vector<CStr, retruxx::allocator<CStr> >& GetFxNames() const;
        int GetFxId(const CStr& fxName);
        m3d::SgNode* CreatePrefabsNode(int type);
        int CreatePrefabsFromFile(const char* fileName);
        void ReleasePrefabs();

    private:
        m3d::Profiler* m_profilerUpdateOde;
        char Padding_170[120];

        void LoadStaticObstacles();
        void CreatePlayerPassMapGeoms();
    };
}

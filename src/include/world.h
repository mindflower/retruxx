#pragma once
#include <math/vector.h>
#include <landscape.h>
#include <map>
#include <core/ref_ptr.h>
#include <core/console/cvar.h>
#include <engine/landscape/roads/roadmanager.h>
#include <engine/weather/weathermanager.h>
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

    class CWorld
    {
    public:
        bool GetShadowVisibilityFromWeather() const ;
        WeatherManager & GetWeatherManager();
        void Invalidate();
        WheelTraceMgr & GetWheelTracesMgr();
        static void __fastcall Register();
        void Render();
        int RenderSky(Landscape::LandRenderMode);
        void New(CCamera &,int,float);
        SceneGraph & GetGraph();
        void SetOwner(CClient *);
        RoadManager & GetRoadManager();
        unsigned int GetWeatherFogColor() const ;
        int Load(CStr const &,CCamera &,bool);
        bool SaveWorld(CStr const &);
        void RefreshObjectsOnLandscapeRect(CVector const &,float);
        CWorld();
        void UpdateSun();
        void ReleasePrefabs();
        int CreateGeomsRepresentingSceneNodes();
        dxSpace * GetOdeSpace();
        unsigned int GetWeatherSunColor() const ;
        unsigned int GetWeatherPlantColor() const ;
        void Release();
        unsigned int GetWeatherSpecularColor() const ;
        ai::Vehicle * GetVehicleControlledByPlayer();
        float GetSForShadowsFromWeather() const ;
        void ProcessCollisionStuffOnNode(SgNode *);
        float GetShadowTransparencyFromWeather() const ;
        GlobalTimeParams GetCurDayTimeFromWeatherManager() const ;
        void Restore();
        CVector GetScatteringSunColor() const ;
        std::vector<CStr,std::allocator<CStr> > const & GetFxNames() const ;
        unsigned int GetWeatherAmbientColor() const ;
        int GetFxId(CStr const &);
        CVector const & GetSun(float) const ;
        ~CWorld();
        void Update();
        unsigned int GetWeatherDiffuseColor() const ;
        Landscape & GetLandscape();
        void ProcessCollisionStuff();
        int CreatePrefabsFromFile(char const *);
        SgNode * CreatePrefabsNode(int);
        void UpdateSkyParams();
        float GetSunAscention() const ;
        float GetFogReduceFactorFromWeather() const ;

    protected:
        SgNode * ReadPrefab(ref_ptr<cmn::XmlFile>,ref_ptr<cmn::XmlNode>);
        bool LoadWorld(CStr const &);

    private:
        ReleasePrefabs();
        void LoadStaticObstacles();

    private:
        CVar m_lsInscatterCoeff;
        CVar m_lsOutscatterCoeff;
        CVar m_skyInscatterCoeff;
        CVar m_skyOutscatterCoeff;
        CVar m_sunColorR;
        CVar m_sunColorG;
        CVar m_sunColorB;
        CVar m_fogStart;
        bool m_isWeatherActual;
        dxGeom *m_borderWallGeoms[6];
        Landscape m_landscape;
        SceneGraph m_sceneGraph;
        RoadManager m_roadManager;
        WeatherManager m_weatherManager;
        WheelTraceMgr m_wheelTracesMgr;
        int m_lastId;
        rend::TexHandle m_texMiniMap;
        rend::TexHandle m_texSunGlow;
        TexClouds *m_texClouds;
        CClient *m_owner;
        Level *m_level;
        CVector m_sunDir;
        float m_sunAzimuth;
        float m_sunAscention;
        std::vector<CWorld::EffectsData> m_effectsFactory;
        std::vector<CStr> fxNames;
        std::map<CStr,int> fxRemap;
        Profiler *m_profilerUpdateOde;
    };
}

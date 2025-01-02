#include "weather/weather.h"
#include <stdexcept>
#include <world.h>
#include <core/kernel.h>
#include <core/ini.h>

#include "config.h"
#include "level.h"
#include "m3dapp.h"
#include "core/log.h"
#include "core/timer.h"
#include "server/dynamicscene.h"
#include "server/server.h"

namespace ai
{
    extern CServer* pServer;
    extern DynamicScene* gDynamicScene;
}

namespace m3d
{
    bool CWorld::GetShadowVisibilityFromWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    WeatherManager& CWorld::GetWeatherManager()
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::Invalidate()
    {
        throw std::logic_error("Not implemented");
    }

    WheelTraceMgr& CWorld::GetWheelTracesMgr()
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::Render()
    {
        m_landscape.UpdateVis(true);
        m_landscape.Render();
        ai::gDynamicScene->RenderDebugInfo();
    }

    int CWorld::RenderSky(Landscape::LandRenderMode)
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::New(CCamera&, int, float)
    {
        throw std::logic_error("Not implemented");
    }

    SceneGraph& CWorld::GetGraph()
    {
        return m_sceneGraph;
    }

    void CWorld::SetOwner(CClient* client)
    {
        m_owner = client;
    }

    RoadManager& CWorld::GetRoadManager()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherFogColor() const
    {
        throw std::logic_error("Not implemented");
    }

    int CWorld::Load(CStr const& levelname, CCamera& cam, bool bQuiet)
    {
        //TODO: implement CWorld::Load
        auto timeStart = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- World Loading");
        M3D_KERNEL->GetEngineCfg().m_levFileName.Set(levelname.c_str(), true);
        if (m_level->Load(levelname, cam, bQuiet) == 0)
        {
            M3D_LOG_INFO("Level file " + levelname + " not found");
            return 0;
        }
        ai::pServer->Init(this);
        m_weatherManager.CreateSky();

        auto const landscapeStart = M3D_KERNEL->GetTimer().GetCurTime();
        if (!m_landscape.Load())
        {
            return 0;
        }
        auto const landscapeEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- Landscape loaded in: " + CStr(landscapeEnd - landscapeStart));

        bool res = true;
        if (m_level->m_serversname.empty() || !M3D_APP->LoadServers(m_level->m_serversname, bQuiet))
        {
            res = M3D_APP->LoadServers("data\\models\\servers.xml", bQuiet);
        }
        if (m_level->m_staticServers.empty() || !M3D_APP->LoadServers(m_level->m_staticServers, bQuiet))
        {
            res |= M3D_APP->LoadServers("data\\models\\commonservers.xml", bQuiet);
        }
        m_landscape.PostServersLoad();
        if (!res)
        {
            return 0;
        }
        return 1;
        throw std::logic_error("Not implemented");
    }

    bool CWorld::SaveWorld(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::RefreshObjectsOnLandscapeRect(CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    CWorld::CWorld() :
        m_lsInscatterCoeff("lsIC", "50", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_lsOutscatterCoeff("lsOC", "50", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_skyInscatterCoeff("skyIC", "50", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_skyOutscatterCoeff("skyOC", "50", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_sunColorR("sunColorR", "100", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_sunColorG("sunColorG", "40", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_sunColorB("sunColorB", "10", CVar::CVAR_INT, CVar::CVAR_ARCHIVE),
        m_fogStart("lsFogStart", "70", CVar::CVAR_INT, CVar::CVAR_ARCHIVE)
    {
        this->m_sceneGraph.SetOwner(this);
        this->m_landscape.setOwner(this);
        this->m_weatherManager.SetOwner(this);
        this->m_roadManager.SetOwner(&m_landscape);
        this->m_sunAzimuth = 45.0;
        this->m_lastId = 0;
        this->m_level = dynamic_cast<Level*>(m3d::g_Kernel->New("Level"));
        m3d::g_Kernel->UnRegisterGlobal("CurrentLevel");
        m3d::g_Kernel->RegisterGlobal(this->m_level, "CurrentLevel");
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_lsInscatterCoeff, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_lsOutscatterCoeff, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_skyInscatterCoeff, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_skyOutscatterCoeff, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_sunColorR, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_sunColorG, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_sunColorB, 0);
        g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_fogStart, 0);
        this->m_profilerUpdateOde = Application::g_pApp->GetProfilerStack().GetProfiler(Application::g_pApp->GetProfilerStack().AddProfiler("AI other update", 0x1Eu));
        this->m_borderWallGeoms[0] = 0;
        this->m_borderWallGeoms[1] = 0;
        this->m_borderWallGeoms[2] = 0;
        this->m_borderWallGeoms[3] = 0;
        this->m_borderWallGeoms[4] = 0;
        this->m_borderWallGeoms[5] = 0;
        this->m_texClouds = 0;
        this->m_isWeatherActual = 1;
    }

    void CWorld::UpdateSun()
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::ReleasePrefabs()
    {
        throw std::logic_error("Not implemented");
    }

    int CWorld::CreateGeomsRepresentingSceneNodes()
    {
        throw std::logic_error("Not implemented");
    }

    dxSpace* CWorld::GetOdeSpace()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherSunColor() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherPlantColor() const
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::Release()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherSpecularColor() const
    {
        throw std::logic_error("Not implemented");
    }

    ai::Vehicle* CWorld::GetVehicleControlledByPlayer()
    {
        throw std::logic_error("Not implemented");
    }

    float CWorld::GetSForShadowsFromWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::ProcessCollisionStuffOnNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    float CWorld::GetShadowTransparencyFromWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    GlobalTimeParams CWorld::GetCurDayTimeFromWeatherManager() const
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::Restore()
    {
        throw std::logic_error("Not implemented");
    }

    CVector CWorld::GetScatteringSunColor() const
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<CStr, std::allocator<CStr>> const& CWorld::GetFxNames() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherAmbientColor() const
    {
        throw std::logic_error("Not implemented");
    }

    int CWorld::GetFxId(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& CWorld::GetSun(float) const
    {
        throw std::logic_error("Not implemented");
    }

    CWorld::~CWorld()
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::Update()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherDiffuseColor() const
    {
        throw std::logic_error("Not implemented");
    }

    Landscape& CWorld::GetLandscape()
    {
        return m_landscape;
    }

    void CWorld::ProcessCollisionStuff()
    {
        throw std::logic_error("Not implemented");
    }

    int CWorld::CreatePrefabsFromFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    SgNode* CWorld::CreatePrefabsNode(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::UpdateSkyParams()
    {
        throw std::logic_error("Not implemented");
    }

    float CWorld::GetSunAscention() const
    {
        throw std::logic_error("Not implemented");
    }

    float CWorld::GetFogReduceFactorFromWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    SgNode* CWorld::ReadPrefab(ref_ptr<cmn::XmlFile>, ref_ptr<cmn::XmlNode>)
    {
        throw std::logic_error("Not implemented");
    }

    bool CWorld::LoadWorld(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::LoadStaticObstacles()
    {
        throw std::logic_error("Not implemented");
    }

    void CWorld::Register()
    {
        g_Kernel->AddClass(RT_CLASS_LOCAL(Weather));
        g_Kernel->AddClass(RT_CLASS_LOCAL(WeatherClear));
        g_Kernel->AddClass(RT_CLASS_LOCAL(WeatherInclement));
        g_Kernel->AddClass(RT_CLASS_LOCAL(WeatherThunderstorm));
        g_Kernel->AddClass(RT_CLASS_LOCAL(WeatherFoggy));
    }
}

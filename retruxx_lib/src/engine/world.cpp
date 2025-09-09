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
#include <ode/collision.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

namespace ai
{
    extern CServer* pServer;
}

namespace m3d
{
    bool CWorld::GetShadowVisibilityFromWeather() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    WeatherManager& CWorld::GetWeatherManager()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Invalidate()
    {
        throw retruxx::logic_error("Not implemented");
    }

    WheelTraceMgr& CWorld::GetWheelTracesMgr()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Render()
    {
        m_landscape.UpdateVis(true);
        m_landscape.Render();
        ai::gDynamicScene->RenderDebugInfo();
    }

    int CWorld::RenderSky(Landscape::LandRenderMode)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::New(CCamera&, int, float)
    {
        throw retruxx::logic_error("Not implemented");
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
        return m_roadManager;
    }

    unsigned CWorld::GetWeatherFogColor() const
    {
        throw retruxx::logic_error("Not implemented");
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

        if (!CreatePrefabsFromFile("prefabs.xml") && !CreatePrefabsFromFile("data\\models\\prefabs.xml"))
        {
            M3D_LOG_INFO("Could not find prefabs.xml");
        }

        M3D_APP->PostLoadServers();

        auto const serversEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- Servers loaded in: " + CStr(serversEnd - landscapeEnd));

        const auto worldBegin = M3D_KERNEL->GetTimer().GetCurTime();
        if (!bQuiet)
        {
            const auto str = M3D_APP->GetStringByStringId0("LoadWorld");
            M3D_APP->PutSplash(100, str.c_str());
        }

        LoadWorld(m_level->GetFullPathNameA("world.xml"));

        const auto worldEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- Nodes loaded in: " + CStr(worldEnd - worldBegin));

        const auto initWorldBegin = M3D_KERNEL->GetTimer().GetCurTime();
        if (!bQuiet)
        {
            const auto str = M3D_APP->GetStringByStringId0("InitWorld");
            M3D_APP->PutSplash(100, str.c_str());
        }

        const auto initWorldEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- Prefabs loaded in: " + CStr(initWorldEnd - initWorldBegin));

        ProcessCollisionStuff();
        m_weatherManager.ReadFromXmlFile(M3D_KERNEL->GetEngineCfg().m_weather_ConfigFile.GetS());
        m_weatherManager.UpdateDayTime();

        const auto worldLoadedEnd = M3D_KERNEL->GetTimer().GetCurTime();
        M3D_LOG_INFO("----------------------- World loaded in: " + CStr(worldLoadedEnd - timeStart));

        if (m_borderWallGeoms[0])
        {
            dGeomPlaneSetParams(m_borderWallGeoms[0], 1.0, 0.0, 0.0, m_level->m_minSafex);
        }
        else
        {
            m_borderWallGeoms[0] = dCreatePlane(ai::gGlobalSpace, 1.0, 0.0, 0.0, m_level->m_minSafex);
        }
        dGeomSetCategoryBits(this->m_borderWallGeoms[0], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[0], 0xFFFFFFFE);
        if (m_borderWallGeoms[1])
            dGeomPlaneSetParams(m_borderWallGeoms[1], -1.0, 0.0, 0.0, 0.0 - this->m_level->m_maxSafex);
        else
            this->m_borderWallGeoms[1] = dCreatePlane(ai::gGlobalSpace, -1.0, 0.0, 0.0, 0.0 - this->m_level->m_maxSafex);
        dGeomSetCategoryBits(this->m_borderWallGeoms[1], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[1], 0xFFFFFFFE);
        if (m_borderWallGeoms[3])
            dGeomPlaneSetParams(m_borderWallGeoms[3], 0.0, 0.0, 1.0, this->m_level->m_minSafey);
        else
            this->m_borderWallGeoms[3] = dCreatePlane(ai::gGlobalSpace, 0.0, 0.0, 1.0, this->m_level->m_minSafey);
        dGeomSetCategoryBits(this->m_borderWallGeoms[3], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[3], 0xFFFFFFFE);
        if (m_borderWallGeoms[4])
            dGeomPlaneSetParams(m_borderWallGeoms[4], 0.0, 0.0, -1.0, 0.0 - this->m_level->m_maxSafey);
        else
            this->m_borderWallGeoms[4] = dCreatePlane(ai::gGlobalSpace, 0.0, 0.0, -1.0, 0.0 - this->m_level->m_maxSafey);
        dGeomSetCategoryBits(this->m_borderWallGeoms[4], 1u);
        dGeomSetCollideBits(this->m_borderWallGeoms[4], 0xFFFFFFFE);




        throw retruxx::logic_error("Not implemented");
        return 1;
    }

    bool CWorld::SaveWorld(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::RefreshObjectsOnLandscapeRect(CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::ReleasePrefabs()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CWorld::CreateGeomsRepresentingSceneNodes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    dxSpace* CWorld::GetOdeSpace()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherSunColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherPlantColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherSpecularColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ai::Vehicle* CWorld::GetVehicleControlledByPlayer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float CWorld::GetSForShadowsFromWeather() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::ProcessCollisionStuffOnNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float CWorld::GetShadowTransparencyFromWeather() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    GlobalTimeParams CWorld::GetCurDayTimeFromWeatherManager() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Restore()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector CWorld::GetScatteringSunColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> const& CWorld::GetFxNames() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherAmbientColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CWorld::GetFxId(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector const& CWorld::GetSun(float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CWorld::~CWorld()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::Update()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherDiffuseColor() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape& CWorld::GetLandscape()
    {
        return m_landscape;
    }

    void CWorld::ProcessCollisionStuff()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CWorld::CreatePrefabsFromFile(char const* fileName)
    {
        scoped_ptr fileStream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!fileStream->Open(fileName, fs::IStream::OPEN_READ))
        {
            return 0;
        }

        throw retruxx::logic_error("Not implemented");
    }

    SgNode* CWorld::CreatePrefabsNode(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::UpdateSkyParams()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float CWorld::GetSunAscention() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float CWorld::GetFogReduceFactorFromWeather() const
    {
        return m_weatherManager.GetFogReduceFactorFromWeather();
    }

    SgNode* CWorld::ReadPrefab(ref_ptr<cmn::XmlFile>, ref_ptr<cmn::XmlNode>)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool CWorld::LoadWorld(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void CWorld::LoadStaticObstacles()
    {
        throw retruxx::logic_error("Not implemented");
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

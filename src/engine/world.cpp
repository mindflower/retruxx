#include "weather/weather.h"
#include <stdexcept>
#include <world.h>
#include <core/kernel.h>

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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void CWorld::SetOwner(CClient*)
    {
        throw std::logic_error("Not implemented");
    }

    RoadManager& CWorld::GetRoadManager()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CWorld::GetWeatherFogColor() const
    {
        throw std::logic_error("Not implemented");
    }

    int CWorld::Load(CStr const&, CCamera&, bool)
    {
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

    CWorld::CWorld()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

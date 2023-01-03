#include "weathermanager.h"
#include <stdexcept>

namespace m3d
{
    int WeatherManager::DeleteWeather(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::CreateSky()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::WriteToXmlFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::ChangeCloudsTexture()
    {
        throw std::logic_error("Not implemented");
    }

    float WeatherManager::SetGlobalTimeParam(unsigned, float)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::SetupSkyParams()
    {
        throw std::logic_error("Not implemented");
    }

    float WeatherManager::GetShadowTransparencyFromWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    GlobalTimeParams WeatherManager::GetCurrentDayTime() const
    {
        throw std::logic_error("Not implemented");
    }

    Weather const* WeatherManager::GetActiveWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned WeatherManager::GetWeatherColor(ColorItems) const
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::ChangeLightmapTexture()
    {
        throw std::logic_error("Not implemented");
    }

    float WeatherManager::GetGlobalTimeParam(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::LoadWeatherStateFromXMLNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::UpdateDayTime()
    {
        throw std::logic_error("Not implemented");
    }

    float WeatherManager::GetFogReduceFactorFromWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::SetActiveWeather(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::RenderWeatherParticles()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::RenderWeather(Landscape::LandRenderMode)
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::SetOwner(CWorld* world)
    {
        m_owner = world;
    }

    int WeatherManager::SaveWeatherStateToXMLNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    bool WeatherManager::GetShadowVisibilityFromWeather() const
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::UpdateWheatherParticles()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::ReadFromXmlFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    WeatherManager::WeatherManager()
    {
    }

    void WeatherManager::DoneSky()
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::AddWeather(CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned WeatherManager::GetNumWeathers() const
    {
        throw std::logic_error("Not implemented");
    }

    char const* WeatherManager::GetGlobalTimeParamName(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::SetActiveWeatherByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherManager::ChangeStarsTexture(CStr&)
    {
        throw std::logic_error("Not implemented");
    }

    Weather* WeatherManager::GetWeather(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    Weather* WeatherManager::GetWeatherByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }
}

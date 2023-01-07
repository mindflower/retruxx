#include "weathermanager.h"
#include <stdexcept>

#include "game/m3dgame.h"

namespace m3d
{
    int WeatherManager::DeleteWeather(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherManager::CreateSky()
    {
        m_vbSky = M3D_APP->m_renderer->AddVb(rend::VERTEX_XYZCT2, 400, "Sky", 0);
        m_ibSky = M3D_APP->m_renderer->AddIb(2166, false);
        auto mem = static_cast<WORD*>(M3D_APP->m_renderer->LockIb(m_ibSky, 0, 0, 0));
        //TODO: check this and refactor
        auto v3 = 0;
        auto v7 = 19;
        do
        {
            auto v4 = v3 + 21;
            auto v5 = 19;
            do
            {
                *mem = v3;
                mem[1] = v4;
                mem[3] = v3;
                mem[5] = v4;
                mem[2] = v4 - 20 + 19;
                mem[4] = v4 - 20;
                mem += 6;
                ++v3;
                ++v4;
                --v5;
            } while (v5);
            ++v3;
            --v7;
        } while (v7);
        M3D_APP->m_renderer->UnlockIb(m_ibSky);
        return 1;
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

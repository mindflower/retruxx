#include "weather/weather.h"
#include <world.h>
#include <core/kernel.h>

namespace m3d
{
    void CWorld::Register()
    {
        g_Kernel->AddClass(&Weather::m_classWeather);
        g_Kernel->AddClass(&WeatherClear::m_classWeatherClear);
        g_Kernel->AddClass(&WeatherInclement::m_classWeatherInclement);
        g_Kernel->AddClass(&WeatherThunderstorm::m_classWeatherThunderstorm);
        g_Kernel->AddClass(&WeatherFoggy::m_classWeatherFoggy);
    }
}

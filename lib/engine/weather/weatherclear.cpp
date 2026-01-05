#include "weather.h"
#include <stdexcept>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(WeatherClear)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WeatherClear);

    Class* WeatherClear::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Weather);
    }

    Object* WeatherClear::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherClear::~WeatherClear() = default;

    Class* WeatherClear::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* WeatherClear::CreateObject()
    {
        return new WeatherClear();
    }

    WeatherClear::WeatherClear(WeatherClear const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherClear::WeatherClear()
    {
    }
}

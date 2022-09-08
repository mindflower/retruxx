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
        throw std::logic_error("Not implemented");
    }

    WeatherClear::~WeatherClear()
    {
        throw std::logic_error("Not implemented");
    }

    Class* WeatherClear::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* WeatherClear::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    WeatherClear::WeatherClear(WeatherClear const&)
    {
        throw std::logic_error("Not implemented");
    }

    WeatherClear::WeatherClear()
    {
        throw std::logic_error("Not implemented");
    }
}

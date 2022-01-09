#include "weather.h"
#include <stdexcept>

namespace m3d
{
    Class* WeatherClear::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
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

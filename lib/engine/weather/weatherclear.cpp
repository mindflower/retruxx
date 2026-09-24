#include "weather.h"
#include <stdexcept>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(WeatherClear)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WeatherClear);

    Class* WeatherClear::GetBaseClass()
    {
        // RVA 0x7B1240 - NOTE: the shipped class registers m3d::Object, not Weather, as its base, so
        // IsKindOf(Weather) is false for it.
        return RT_CLASS_LOCAL(Object);
    }

    Object* WeatherClear::Clone()
    {
        // RVA 0x7B3620 - a fresh default weather; nothing is copied.
        return new WeatherClear();
    }

    // RVA 0x7B1440
    WeatherClear::~WeatherClear() = default;

    Class* WeatherClear::GetClass() const
    {
        // RVA 0x7B1250
        return RT_CLASS_LOCAL(WeatherClear);
    }

    Object* WeatherClear::CreateObject()
    {
        return new WeatherClear();
    }

    WeatherClear::WeatherClear(WeatherClear const&) : Weather()
    {
        // RVA 0x7B35E0 - NOTE: copies nothing, like Weather's copy constructor.
    }

    WeatherClear::WeatherClear()
    {
    }
}

#include "weather.h"
#include <stdexcept>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(WeatherInclement)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WeatherInclement);

    int WeatherInclement::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherInclement::SetUp()
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherInclement::RecreateEffect()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherInclement::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    Class* WeatherInclement::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Weather);
    }

    WeatherInclement::~WeatherInclement()
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherInclement::DefaultInitialize()
    {
        throw std::logic_error("Not implemented");
    }

    Class* WeatherInclement::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* WeatherInclement::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Object* WeatherInclement::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherInclement::Render()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherInclement::TurnOffEffects()
    {
        throw std::logic_error("Not implemented");
    }

    WeatherInclement::WeatherInclement(WeatherInclement const&)
    {
        throw std::logic_error("Not implemented");
    }

    WeatherInclement::WeatherInclement()
    {
    }
}

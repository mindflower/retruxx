#include "weather.h"
#include <stdexcept>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(WeatherThunderstorm)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WeatherThunderstorm);

    void WeatherThunderstorm::DefaultInitialize()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherThunderstorm::Update(float, int)
    {
        throw std::logic_error("Not implemented");
    }

    Class* WeatherThunderstorm::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    WeatherThunderstorm::~WeatherThunderstorm()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherThunderstorm::Render()
    {
        throw std::logic_error("Not implemented");
    }

    Class* WeatherThunderstorm::GetBaseClass()
    {
        return RT_CLASS_LOCAL(WeatherInclement);
    }

    Object* WeatherThunderstorm::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherThunderstorm::UpdateColors(ColorItems, ColorTypes)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherThunderstorm::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherThunderstorm::TurnOffEffects()
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherThunderstorm::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void WeatherThunderstorm::SetUp()
    {
        throw std::logic_error("Not implemented");
    }

    Object* WeatherThunderstorm::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    WeatherThunderstorm::WeatherThunderstorm()
    {
        throw std::logic_error("Not implemented");
    }

    WeatherThunderstorm::WeatherThunderstorm(WeatherThunderstorm const&)
    {
        throw std::logic_error("Not implemented");
    }
}

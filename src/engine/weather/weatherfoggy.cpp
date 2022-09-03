#include "weather.h"
#include <stdexcept>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(WeatherFoggy)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WeatherFoggy);

    int WeatherFoggy::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int WeatherFoggy::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    WeatherFoggy::~WeatherFoggy()
    {
        throw std::logic_error("Not implemented");
    }

    Class* WeatherFoggy::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Class* WeatherFoggy::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* WeatherFoggy::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Object* WeatherFoggy::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    WeatherFoggy::WeatherFoggy(WeatherFoggy const&)
    {
        throw std::logic_error("Not implemented");
    }

    WeatherFoggy::WeatherFoggy()
    {
        throw std::logic_error("Not implemented");
    }
}

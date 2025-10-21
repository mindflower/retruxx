#include "weather.h"
#include <stdexcept>

#include "core/ini.h"

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(WeatherFoggy)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WeatherFoggy);

    int WeatherFoggy::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        m3d::Weather::ReadFromXmlNode(file, node);
        this->m_reduceDistFactor = 0.5;
        m3d::SafeFloatAttrib(m_reduceDistFactor, node, "reduceDistFactor");
        return 1;
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
        return RT_CLASS_LOCAL(Weather);
    }

    Class* WeatherFoggy::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* WeatherFoggy::CreateObject()
    {
        return new WeatherFoggy;
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
    }
}

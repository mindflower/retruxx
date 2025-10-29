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
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherFoggy::~WeatherFoggy()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* WeatherFoggy::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Weather);
    }

    Class* WeatherFoggy::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* WeatherFoggy::CreateObject()
    {
        return new WeatherFoggy;
    }

    Object* WeatherFoggy::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherFoggy::WeatherFoggy(WeatherFoggy const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherFoggy::WeatherFoggy()
    {
    }
}

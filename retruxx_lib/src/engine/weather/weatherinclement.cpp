#include "weather.h"
#include <stdexcept>

#include "core/ini.h"

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(WeatherInclement)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WeatherInclement);

    int WeatherInclement::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        Weather::ReadFromXmlNode(file, node);
        m3d::SafeFloatAttrib(this->m_reduceDistFactor, node, "reduceDistFactor");
        m3d::SafeStrAttrib(this->m_inclementNodeName, node, "inclementWeatherPS");
        m3d::SafeStrAttrib(this->m_soundNodeName, node, "soundName");
        m3d::SafeIntAttrib(this->m_weatherDensity, node, "density");
        m3d::SafeFloatAttrib(this->m_weatherDist, node, "distance");

        if (m_weatherDensity >= 1)
        {
            if (m_weatherDensity > 10)
                m_weatherDensity = 10;
        }
        else
        {
            m_weatherDensity = 1;
        }
        return 1;
    }

    void WeatherInclement::SetUp()
    {
        RecreateEffect();
    }

    void WeatherInclement::RecreateEffect()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WeatherInclement::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* WeatherInclement::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Weather);
    }

    WeatherInclement::~WeatherInclement()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WeatherInclement::DefaultInitialize()
    {
        m3d::Weather::DefaultInitialize();
        m_inclementNodeName = "ET_PS_RAIN_WEATHER";
        m_soundNodeName = "ET_S_RAIN";
        this->m_reduceDistFactor = 0.80000001;
        this->m_weatherDensity = 5;
        this->m_weatherDist = 20.0;
    }

    Class* WeatherInclement::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* WeatherInclement::CreateObject()
    {
        return new WeatherInclement;
    }

    Object* WeatherInclement::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WeatherInclement::Render()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WeatherInclement::TurnOffEffects()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherInclement::WeatherInclement(WeatherInclement const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherInclement::WeatherInclement()
    {
    }
}

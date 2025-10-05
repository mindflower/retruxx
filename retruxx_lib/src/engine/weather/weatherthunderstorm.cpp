#include "weather.h"
#include <stdexcept>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(WeatherThunderstorm)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WeatherThunderstorm);

    void WeatherThunderstorm::DefaultInitialize()
    {
        WeatherInclement::DefaultInitialize();
        this->m_thunderLerpValue = 0.0;
        this->m_minThunderDuty = 300;
        this->m_maxThunderDuty = 600;
        this->m_minThunderPeriod = 30000;
        this->m_maxThunderPeriod = 60000;
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
        return new WeatherThunderstorm;
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
    }

    WeatherThunderstorm::WeatherThunderstorm(WeatherThunderstorm const&)
    {
        throw std::logic_error("Not implemented");
    }
}

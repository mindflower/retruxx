#include "weather.h"
#include <stdexcept>
#include <core/ini.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Weather)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Weather);

    void WindInfo::Write(ref_ptr<cmn::XmlNode>)
    {
        throw std::logic_error("Not implemented");
    }

    void WindInfo::Read(ref_ptr<cmn::XmlNode>)
    {
        throw std::logic_error("Not implemented");
    }

    WindInfo::WindInfo()
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& WindInfo::GetCurWind() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& WindInfo::GetDeltaVel() const
    {
        throw std::logic_error("Not implemented");
    }

    void WindInfo::CalculateCurWind(float)
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Weather::GetWeatherName() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Weather::GetShadowVisibility(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    Class* Weather::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& Weather::CurrentColor(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    Weather::~Weather()
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetWaveHBig() const
    {
        throw std::logic_error("Not implemented");
    }

    char const* Weather::ColorTypeName(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void Weather::ChangeCloudTexture(CStr&)
    {
        throw std::logic_error("Not implemented");
    }

    Object* Weather::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    int Weather::UpdateColors(ColorItems, ColorTypes)
    {
        throw std::logic_error("Not implemented");
    }

    void Weather::SetWeatherName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Weather::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int Weather::TurnOffEffects()
    {
        throw std::logic_error("Not implemented");
    }

    int Weather::WriteDetailToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetWaveSizeBig() const
    {
        throw std::logic_error("Not implemented");
    }

    int Weather::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetWaterSpecularS() const
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetWaterSpecularM() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Weather::GetLightmapTexName(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    Object* Weather::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void Weather::SetUp()
    {
        throw std::logic_error("Not implemented");
    }

    int Weather::ReadDetailFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    WindInfo const& Weather::GetWindInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetWaterSpeed() const
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetWaterCourseAngle() const
    {
        throw std::logic_error("Not implemented");
    }

    int Weather::Update(float, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Weather::DefaultInitialize()
    {
        throw std::logic_error("Not implemented");
    }

    char const* Weather::ColorItemName(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    Class* Weather::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetWaveHSmall() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Weather::GetCloudsTexName(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void Weather::Release()
    {
        throw std::logic_error("Not implemented");
    }

    int Weather::Render()
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetShadowTransparency(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    float Weather::GetWaveSizeSmall() const
    {
        throw std::logic_error("Not implemented");
    }

    Weather::Weather(Weather const&)
    {
        throw std::logic_error("Not implemented");
    }

    Weather::Weather()
    {
        throw std::logic_error("Not implemented");
    }
}

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
        // RVA 0x8FADB0
        m3d::Weather::ReadFromXmlNode(file, node);
        this->m_reduceDistFactor = 0.5;
        m3d::SafeFloatAttrib(m_reduceDistFactor, node, "reduceDistFactor");
        return 1;
    }

    int WeatherFoggy::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        // RVA 0x8FAE10
        Weather::WriteToXmlNode(file, node);
        node->SetAttribute("reduceDistFactor", CStr(m_reduceDistFactor).c_str());
        return 1;
    }

    // RVA 0x7B1450
    WeatherFoggy::~WeatherFoggy() = default;

    Class* WeatherFoggy::GetBaseClass()
    {
        // RVA 0x7B12A0 - NOTE: the shipped class registers m3d::Object, not Weather, as its base, so
        // IsKindOf(Weather) is false for it.
        return RT_CLASS_LOCAL(Object);
    }

    Class* WeatherFoggy::GetClass() const
    {
        // RVA 0x7B12B0
        return RT_CLASS_LOCAL(WeatherFoggy);
    }

    Object* WeatherFoggy::CreateObject()
    {
        return new WeatherFoggy;
    }

    Object* WeatherFoggy::Clone()
    {
        // RVA 0x7B3650 - a fresh default weather; nothing is copied.
        return new WeatherFoggy();
    }

    WeatherFoggy::WeatherFoggy(WeatherFoggy const&) : Weather()
    {
        // RVA 0x7B3600 - NOTE: copies nothing, like Weather's copy constructor.
    }

    WeatherFoggy::WeatherFoggy()
    {
    }
}

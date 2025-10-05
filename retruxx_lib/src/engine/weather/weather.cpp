#include "weather.h"
#include <stdexcept>
#include <core/ini.h>

CVector fullColor(255.0, 255.0, 255.0);
CVector halfColor(128.0, 128.0, 128.0);
CVector quarterColor(64.0, 64.0, 64.0);
const CVector m_colorItemsInit[4] = { halfColor, fullColor, halfColor, quarterColor };

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
        this->m_changeVelTime = 100000.0;
        this->m_changeDirTime = 100000.0;
        this->m_maxVel = 0.0;
        this->m_minVel = 0.0;
        this->m_prevValue.x = 0.0099999998;
        this->m_prevValue.y = 0.0;
        this->m_prevValue.z = 0.0099999998;
        this->m_curValue.x = 0.0099999998;
        this->m_curValue.y = 0.0;
        this->m_curValue.z = 0.0099999998;
        this->m_deltaValue.x = 0.0;
        this->m_deltaValue.y = 0.0;
        this->m_deltaValue.z = 0.0;
        this->m_DeltaVelChanged = 0.0;
        this->m_DeltaDirChanged = 0.0;
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
        return RT_CLASS_LOCAL(Object);
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

    int Weather::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
    {
        // TODO: implement Weather::ReadFromXmlNode
        return 0;
        //m_Name = xmlNode->GetAttribute("name");

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
        for (int i = 0; i < 7; ++i)
        {
            m_currentColors[i] = fullColor;
            m_colorSets[0][i] = m_colorItemsInit[0];
            m_colorSets[1][i] = m_colorItemsInit[1];
            m_colorSets[2][i] = m_colorItemsInit[2];
            m_colorSets[3][i] = m_colorItemsInit[3];
        }

        this->m_waterSpeed = 0.51999998;
        this->m_waterHeightBig = 0.0;
        this->m_waterHeightSmall = 0.0;
        this->m_waterCourseAng = 0.0;
        this->m_waterSpecularM = 200.0;
        this->m_reduceDistFactor = 1.0;
        this->m_weatherWeight = 1.0;
        this->m_waterSizeBig = 1.0;
        this->m_waterSizeSmall = 1.0;
        this->m_waterSpecularS = 0.0049999999;
        this->m_weatherSkyDomeFactor = 1.0;

        for (int i = 0; i < 4; ++i)
        {
            m_cloudsSpeed[i] = 1.0;
            m_shadowTransparency[i] = 0.5;
            m_lightmapTextureName[i] = "LoadMe.txt";
            m_cloudsTextureName[i] = "LoadMe.txt";
            m_PostEffectName[i] = "NoEffect";
        }
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
    }
}

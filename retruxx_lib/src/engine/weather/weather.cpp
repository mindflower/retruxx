#include "weather.h"
#include <stdexcept>
#include <core/ini.h>

#include "core/log.h"
#include "core/timer.h"
#include "math/matrix.h"

CVector fullColor(255.0, 255.0, 255.0);
CVector halfColor(128.0, 128.0, 128.0);
CVector quarterColor(64.0, 64.0, 64.0);
const CVector m_colorItemsInit[4] = { halfColor, fullColor, halfColor, quarterColor };
const char* m_colorTypesNames[4] = { "sunrise" , "day" , "sunset", "night" };
const char* m_colorItemsNames[7] = {
    "Sky",
    "Fog",
    "Ambient",
    "Diffuse",
    "Sun",
    "Plant",
    "Specular",
};

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Weather)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Weather);

    void WindInfo::Write(ref_ptr<cmn::XmlNode>)
    {
        throw std::logic_error("Not implemented");
    }

    void WindInfo::Read(ref_ptr<cmn::XmlNode> node)
    {
        m3d::SafeFloatAttrib(m_maxVel, node, "windMaxVel");
        m3d::SafeFloatAttrib(m_minVel, node, "windMinVel");
        m3d::SafeFloatAttrib(m_changeVelTime, node, "windChangeVelTime");
        m3d::SafeFloatAttrib(m_changeDirTime, node, "windChangeDirTime");
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

    void WindInfo::CalculateCurWind(float dt)
    {
        // TODO: check this
        auto prevVel = this->m_prevValue;
        auto p_m_curValue = &this->m_curValue;
        auto y = this->m_curValue.y;
        auto z = this->m_curValue.z;
        this->m_prevValue.x = this->m_curValue.x;
        auto v8 = m_DeltaVelChanged + dt;
        auto v9 = v8 <= this->m_changeVelTime;
        this->m_prevValue.y = y;
        this->m_prevValue.z = z;
        this->m_DeltaVelChanged = v8;
        this->m_DeltaDirChanged = m_DeltaDirChanged + dt;
        if (!v9)
        {
            this->m_DeltaVelChanged = v8 - this->m_changeVelTime;
            auto v10 = rand();
            auto v11 = p_m_curValue->z;
            auto v12 = (((this->m_maxVel - this->m_minVel) * (10000 * v10 / 0x8000)) * 0.000099999997) + this->m_minVel;
            auto dta = 1.0
                / sqrt(p_m_curValue->x * p_m_curValue->x + p_m_curValue->y * p_m_curValue->y + v11 * v11 + 0.00000011920929);
            auto v13 = (p_m_curValue->y * dta) * v12;
            p_m_curValue->x = (dta * p_m_curValue->x) * v12;
            p_m_curValue->y = v13;
            p_m_curValue->z = (v11 * dta) * v12;
        }
        auto v14 = this->m_DeltaDirChanged;
        if (v14 > this->m_changeDirTime)
        {
            this->m_DeltaDirChanged = v14 - this->m_changeDirTime;
            auto v15 = rand();
            auto x = p_m_curValue->x;

            CMatrix rot;
            rot.zero();
            auto v17 = (10000 * v15 / 0x8000) * 0.00031415926 - 1.5707964;
            auto dtb = sin(v17);
            auto v25 = cos(v17);
            auto v18 = v25 * p_m_curValue->z;
            auto v19 = ((p_m_curValue->x * rot._12) + (rot._32 * p_m_curValue->z)) + p_m_curValue->y;
            auto v20 = p_m_curValue->x * (0.0 - dtb);
            auto v21 = rot._23 * p_m_curValue->y;
            p_m_curValue->x = ((x * v25) + (rot._21 * p_m_curValue->y)) + (dtb * p_m_curValue->z);
            p_m_curValue->y = v19;
            p_m_curValue->z = (v20 + v21) + v18;
        }
        auto v22 = p_m_curValue->z - prevVel.z;
        auto v23 = p_m_curValue->y - prevVel.y;
        auto p_m_deltaValue = &this->m_deltaValue;
        p_m_deltaValue->x = p_m_curValue->x - prevVel.x;
        p_m_deltaValue->y = v23;
        p_m_deltaValue->z = v22;
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

    int Weather::UpdateColors(ColorItems colorItem, ColorTypes curTime)
    {
        auto v3 = &this->m_colorSets[colorItem][curTime];
        auto v4 = &this->m_currentColors[colorItem];
        v4->x = v3->x;
        v4->y = v3->y;
        v4->z = v3->z;
        return 1;
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
        m_Name = xmlNode->GetAttribute("name");
        for (int i = 0; i < 7; ++i)
        {
            CStr effName = m_colorItemsNames[i];
            ref_ptr effNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(effNode, effName.c_str());
            if (effNode->IsEmpty())
            {
                M3D_LOG_ERR("Colorset for " + effName + " not found");
                continue;
            }
            for (int j = 0; j < 4; ++j)
            {
                m_colorSets[i][j] = m_colorItemsInit[j];
                CStr typeName = m_colorTypesNames[j] + CStr("Color");
                m3d::SafeVectorAttrib(m_colorSets[i][j], effNode, typeName.c_str());
            }
        }

        ref_ptr postEffectNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(postEffectNode, "PostEffect");
        if (!postEffectNode->IsEmpty())
        {
            for (int i = 0; i < 4; ++i)
            {
                m3d::SafeStrAttrib(m_PostEffectName[i], postEffectNode, m_colorItemsNames[i]);
            }
        }

        ref_ptr shadowVisibilityNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(shadowVisibilityNode, "ShadowVisibility");
        if (!shadowVisibilityNode->IsEmpty())
        {
            for (int i = 0; i < 4; ++i)
            {
                m_shadowVisibility[i] = true;
                m3d::SafeBoolAttrib(m_shadowVisibility[i], shadowVisibilityNode, m_colorItemsNames[i]);
            }
        }

        ref_ptr shadowTransparencyNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(shadowTransparencyNode, "ShadowTransparency");
        if (!shadowTransparencyNode->IsEmpty())
        {
            for (int i = 0; i < 4; ++i)
            {
                m_shadowTransparency[i] = 0.5;
                m3d::SafeFloatAttrib(m_shadowTransparency[i], shadowTransparencyNode, m_colorItemsNames[i]);
            }
        }

        ref_ptr cloudsSpeedNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(cloudsSpeedNode, "cloudsSpeed");
        if (!cloudsSpeedNode->IsEmpty())
        {
            for (int i = 0; i < 4; ++i)
            {
                m_cloudsSpeed[i] = 0.1;
                m3d::SafeFloatAttrib(m_cloudsSpeed[i], cloudsSpeedNode, m_colorItemsNames[i]);
            }
        }

        m3d::SafeFloatAttrib(m_weatherSkyDomeFactor, xmlNode, "skyDomeFactor");
        m3d::SafeFloatAttrib(m_waterSpeed, xmlNode, "waterSpeed");
        m3d::SafeFloatAttrib(m_waterHeightBig, xmlNode, "waterWaveHBig");
        m3d::SafeFloatAttrib(m_waterHeightSmall, xmlNode, "waterWaveHSmall");
        m3d::SafeFloatAttrib(m_waterSizeBig, xmlNode, "waterWaveSizeBig");
        m3d::SafeFloatAttrib(m_waterSizeSmall, xmlNode, "waterWaveSizeSmall");
        m3d::SafeFloatAttrib(m_waterCourseAng, xmlNode, "waterCourseAngle");
        m3d::SafeFloatAttrib(m_waterSpecularM, xmlNode, "waterSpecularM");
        m3d::SafeFloatAttrib(m_waterSpecularS, xmlNode, "waterSpecularS");

        ref_ptr windInfoNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(windInfoNode, "WindInfo");
        m_wind.Read(windInfoNode);

        return 1;
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
    }

    int Weather::ReadDetailFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        ref_ptr colorTypeNode = file->CreateNode();
        for (node->GetFirstChild(colorTypeNode, "ColorType"); !colorTypeNode->IsEmpty(); colorTypeNode->GetNextSibling(colorTypeNode, "ColorType"))
        {
            CStr name = colorTypeNode->GetAttribute("name");
            for (int i = 0; i < 4; ++i)
            {
                if (name == m_colorTypesNames[i])
                {
                    m_lightmapTextureName[i] = colorTypeNode->GetAttribute("LightmapTexture");
                    m_cloudsTextureName[i] = colorTypeNode->GetAttribute("CloudsTexture");
                    break;
                }
            }
        }
        return 1;
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

    int Weather::Update(float amount, int)
    {
        auto dt = M3D_KERNEL->GetTimer().GetLastFrameTime() * 0.001;
        m_wind.CalculateCurWind(dt);
        this->m_weatherWeight = amount;
        return 1;
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
        return 1;
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

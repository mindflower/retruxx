#include "weather.h"
#include <stdexcept>
#include <core/ini.h>

#include "core/log.h"
#include "core/timer.h"
#include "math/matrix.h"

#include <cmath>

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

    void WindInfo::Write(ref_ptr<cmn::XmlNode> node)
    {
        // RVA 0x7B1A30
        node->SetAttribute("windMaxVel", CStr(m_maxVel).c_str());
        node->SetAttribute("windMinVel", CStr(m_minVel).c_str());
        node->SetAttribute("windChangeVelTime", CStr(m_changeVelTime).c_str());
        node->SetAttribute("windChangeDirTime", CStr(m_changeDirTime).c_str());
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
        // RVA 0x9500D0
        return m_curValue;
    }

    CVector const& WindInfo::GetDeltaVel() const
    {
        // RVA 0x9500E0
        return m_deltaValue;
    }

    void WindInfo::CalculateCurWind(float dt)
    {
        // RVA 0x7B1C10
        CVector const prevVel = m_prevValue;
        m_prevValue = m_curValue;
        m_DeltaVelChanged = m_DeltaVelChanged + dt;
        m_DeltaDirChanged = m_DeltaDirChanged + dt;

        if (m_DeltaVelChanged > m_changeVelTime)
        {
            // A new speed in [min, max] in 1/10000 steps; the direction is kept.
            m_DeltaVelChanged = m_DeltaVelChanged - m_changeVelTime;
            int const r = 10000 * rand() / 0x8000;
            float const speed = (m_maxVel - m_minVel) * static_cast<float>(r) * 0.000099999997f + m_minVel;

            // The length is taken on the x87 stack; FLT_EPSILON keeps a zero vector from dividing by zero.
            double const lenSq = double(m_curValue.x) * m_curValue.x + double(m_curValue.y) * m_curValue.y +
                double(m_curValue.z) * m_curValue.z + double(1.1920929e-7f);
            float const invLen = static_cast<float>(1.0 / std::sqrt(lenSq));
            m_curValue.x = invLen * m_curValue.x * speed;
            m_curValue.y = m_curValue.y * invLen * speed;
            m_curValue.z = m_curValue.z * invLen * speed;
        }

        if (m_DeltaDirChanged > m_changeDirTime)
        {
            // Turns the wind about the vertical axis by a random angle in [-pi/2, pi/2). The shipped
            // code multiplies by a memset-zeroed CMatrix whose off-axis entries stay zero, so only the
            // yaw terms below survive.
            m_DeltaDirChanged = m_DeltaDirChanged - m_changeDirTime;
            int const r = 10000 * rand() / 0x8000;
            double const angle = r * double(0.00031415926f) - double(1.5707964f);
            float const s = static_cast<float>(std::sin(angle));
            float const c = static_cast<float>(std::cos(angle));

            CVector const v = m_curValue;
            m_curValue.x = v.x * c + s * v.z;
            m_curValue.z = v.x * (0.0f - s) + c * v.z;
        }

        m_deltaValue.x = m_curValue.x - prevVel.x;
        m_deltaValue.y = m_curValue.y - prevVel.y;
        m_deltaValue.z = m_curValue.z - prevVel.z;
    }

    CStr const& Weather::GetWeatherName() const
    {
        // RVA 0x65D5F0
        return m_Name;
    }

    bool Weather::GetShadowVisibility(unsigned curDayTime) const
    {
        // RVA 0x65D610
        return m_shadowVisibility[curDayTime];
    }

    Class* Weather::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    CVector const& Weather::CurrentColor(unsigned Item) const
    {
        // RVA 0x5C0470
        return m_currentColors[Item];
    }

    // RVA 0x7B1310
    Weather::~Weather() = default;

    float Weather::GetWaveHBig() const
    {
        // RVA 0x5C04B0
        return m_waterHeightBig;
    }

    char const* Weather::ColorTypeName(unsigned i) const
    {
        // RVA 0x7B12F0
        return m_colorTypesNames[i];
    }

    void Weather::ChangeCloudTexture(CStr&)
    {
        // RVA 0x7B1300 - an empty stub in the shipped build.
    }

    Object* Weather::CreateObject()
    {
        // RVA 0x7B3560
        return new Weather();
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

    void Weather::SetWeatherName(CStr const& Name)
    {
        // RVA 0x65DA20
        m_Name = Name;
    }

    int Weather::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        // RVA 0x7B2D50 - the mirror of ReadFromXmlNode.
        node->SetAttribute("name", m_Name.c_str());

        for (int i = 0; i < CI_NUM_COLORITEMS; ++i)
        {
            ref_ptr<cmn::XmlNode> itemNode = file->CreateNode(cmn::XML_NODE_ELEMENT, CStr(m_colorItemsNames[i]).c_str());
            for (int j = 0; j < CT_NUM_COLORTYPES; ++j)
            {
                CStr const typeName = CStr(m_colorTypesNames[j]) + CStr("Color");
                itemNode->SetAttribute(typeName.c_str(), CStr(m_colorSets[i][j]).c_str());
            }
            node->AddChild(itemNode);
        }

        ref_ptr<cmn::XmlNode> postEffectsNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "PostEffect");
        for (unsigned i = 0; i < CT_NUM_COLORTYPES; ++i)
        {
            postEffectsNode->SetAttribute(m_colorTypesNames[i], m_PostEffectName[i].c_str());
        }
        node->AddChild(postEffectsNode);

        ref_ptr<cmn::XmlNode> shadowVisibilityNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "ShadowVisibility");
        for (unsigned i = 0; i < CT_NUM_COLORTYPES; ++i)
        {
            shadowVisibilityNode->SetAttribute(m_colorTypesNames[i], CStr(static_cast<int>(m_shadowVisibility[i])).c_str());
        }
        node->AddChild(shadowVisibilityNode);

        ref_ptr<cmn::XmlNode> shadowTransparencyNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "ShadowTransparency");
        for (unsigned i = 0; i < CT_NUM_COLORTYPES; ++i)
        {
            shadowTransparencyNode->SetAttribute(m_colorTypesNames[i], CStr(m_shadowTransparency[i]).c_str());
        }
        node->AddChild(shadowTransparencyNode);

        ref_ptr<cmn::XmlNode> cloudsSpeedNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "cloudsSpeed");
        for (unsigned i = 0; i < CT_NUM_COLORTYPES; ++i)
        {
            cloudsSpeedNode->SetAttribute(m_colorTypesNames[i], CStr(m_cloudsSpeed[i]).c_str());
        }
        node->AddChild(cloudsSpeedNode);

        node->SetAttribute("skyDomeFactor", CStr(m_weatherSkyDomeFactor).c_str());
        node->SetAttribute("waterSpeed", CStr(m_waterSpeed).c_str());
        node->SetAttribute("waterWaveHBig", CStr(m_waterHeightBig).c_str());
        node->SetAttribute("waterWaveHSmall", CStr(m_waterHeightSmall).c_str());
        node->SetAttribute("waterWaveSizeBig", CStr(m_waterSizeBig).c_str());
        node->SetAttribute("waterWaveSizeSmall", CStr(m_waterSizeSmall).c_str());
        node->SetAttribute("waterCourseAngle", CStr(m_waterCourseAng).c_str());
        node->SetAttribute("waterSpecularM", CStr(m_waterSpecularM).c_str());
        node->SetAttribute("waterSpecularS", CStr(m_waterSpecularS).c_str());

        // The wind attributes are written after the node is already attached.
        ref_ptr<cmn::XmlNode> windInfoNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "WindInfo");
        node->AddChild(windInfoNode);
        m_wind.Write(windInfoNode);
        return 1;
    }

    int Weather::TurnOffEffects()
    {
        return 1;
    }

    int Weather::WriteDetailToXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        // RVA 0x7B18E0 - the mirror of ReadDetailFromXmlNode: one ColorType per time of day.
        for (unsigned i = 0; i < CT_NUM_COLORTYPES; ++i)
        {
            ref_ptr<cmn::XmlNode> colorTypeNode = file->CreateNode(cmn::XML_NODE_ELEMENT, "ColorType");
            colorTypeNode->SetAttribute("name", m_colorTypesNames[i]);
            colorTypeNode->SetAttribute("LightmapTexture", m_lightmapTextureName[i].c_str());
            colorTypeNode->SetAttribute("CloudsTexture", m_cloudsTextureName[i].c_str());
            node->AddChild(colorTypeNode);
        }
        return 1;
    }

    float Weather::GetWaveSizeBig() const
    {
        // RVA 0x5C04D0
        return m_waterSizeBig;
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
                m3d::SafeStrAttrib(m_PostEffectName[i], postEffectNode, m_colorTypesNames[i]);
            }
        }

        ref_ptr shadowVisibilityNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(shadowVisibilityNode, "ShadowVisibility");
        if (!shadowVisibilityNode->IsEmpty())
        {
            for (int i = 0; i < 4; ++i)
            {
                m_shadowVisibility[i] = true;
                m3d::SafeBoolAttrib(m_shadowVisibility[i], shadowVisibilityNode, m_colorTypesNames[i]);
            }
        }

        ref_ptr shadowTransparencyNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(shadowTransparencyNode, "ShadowTransparency");
        if (!shadowTransparencyNode->IsEmpty())
        {
            for (int i = 0; i < 4; ++i)
            {
                m_shadowTransparency[i] = 0.5;
                m3d::SafeFloatAttrib(m_shadowTransparency[i], shadowTransparencyNode, m_colorTypesNames[i]);
            }
        }

        ref_ptr cloudsSpeedNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(cloudsSpeedNode, "cloudsSpeed");
        if (!cloudsSpeedNode->IsEmpty())
        {
            for (int i = 0; i < 4; ++i)
            {
                m_cloudsSpeed[i] = 0.1;
                m3d::SafeFloatAttrib(m_cloudsSpeed[i], cloudsSpeedNode, m_colorTypesNames[i]);
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
        // RVA 0x5C0500
        return m_waterSpecularS;
    }

    float Weather::GetWaterSpecularM() const
    {
        // RVA 0x5C04F0
        return m_waterSpecularM;
    }

    CStr const& Weather::GetLightmapTexName(unsigned curDayTime) const
    {
        // RVA 0x65D620
        return m_lightmapTextureName[curDayTime];
    }

    Object* Weather::Clone()
    {
        // RVA 0x7B3530
        return new Weather(*this);
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
        // RVA 0x9500F0
        return m_wind;
    }

    float Weather::GetWaterSpeed() const
    {
        // RVA 0x5C0490
        return m_waterSpeed;
    }

    float Weather::GetWaterCourseAngle() const
    {
        // RVA 0x5C04E0
        return m_waterCourseAng;
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
            // RVA 0x7B1460 walks m_colorSets with a stride of 4 CVectors per
            // iteration (`v3 += 4`), i.e. the outer index is the ColorItem and
            // the four defaults fill that item's ColorTypes. The PDB-derived
            // header declares this array as [4][7]; the real layout is [7][4],
            // so the indices have to be the other way round here.
            m_colorSets[i][0] = m_colorItemsInit[0];
            m_colorSets[i][1] = m_colorItemsInit[1];
            m_colorSets[i][2] = m_colorItemsInit[2];
            m_colorSets[i][3] = m_colorItemsInit[3];
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
            m_cloudsSpeed[i] = 0.1;
            m_shadowVisibility[i] = true;
            m_shadowTransparency[i] = 0.5;
            m_lightmapTextureName[i] = "LoadMe.txt";
            m_cloudsTextureName[i] = "LoadMe.txt";
            m_PostEffectName[i] = "NoEffect";
        }
    }

    char const* Weather::ColorItemName(unsigned i) const
    {
        // RVA 0x7B12E0
        return m_colorItemsNames[i];
    }

    Class* Weather::GetClass() const
    {
        // RVA 0x7B1230
        return RT_CLASS_LOCAL(Weather);
    }

    float Weather::GetWaveHSmall() const
    {
        // RVA 0x5C04A0
        return m_waterHeightSmall;
    }

    CStr const& Weather::GetCloudsTexName(unsigned curDayTime) const
    {
        // RVA 0x65D640
        return m_cloudsTextureName[curDayTime];
    }

    void Weather::Release()
    {
        // default
    }

    int Weather::Render()
    {
        return 1;
    }

    float Weather::GetShadowTransparency(unsigned curDayTime) const
    {
        // RVA 0x65D600
        return m_shadowTransparency[curDayTime];
    }

    float Weather::GetWaveSizeSmall() const
    {
        // RVA 0x5C04C0
        return m_waterSizeSmall;
    }

    Weather::Weather(Weather const&) : Object()
    {
        // RVA 0x7B1EC0 - NOTE: the copy constructor copies nothing. It only default-constructs the
        // members, exactly like Weather(), so Clone() returns a blank weather with uninitialized
        // floats and colours.
    }

    Weather::Weather()
    {
    }
}

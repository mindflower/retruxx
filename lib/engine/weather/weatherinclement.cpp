#include "weather.h"
#include <stdexcept>

#include "client.h"
#include "config.h"
#include "m3dapp.h"
#include "world.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/timer.h"
#include "math/quaternion.h"
#include "scene/scenegraph.h"

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
        // RVA 0x8FB780 - spawns m_weatherDensity particle-system nodes plus one sound node.
        TurnOffEffects();

        int const psModelId = M3D_ENGINE_CFG.GetModelIdByName(m_inclementNodeName);
        if (psModelId != 0x200000)
        {
            for (int i = 0; i < m_weatherDensity; ++i)
            {
                SgNode* weatherNode = pClient->CreateServerControlledNode(psModelId);
                if (weatherNode)
                {
                    pClient->GetWorld().GetGraph().GetRootNode()->AddChild(weatherNode);
                    weatherNode->SetScale(CVector(1.0f, 1.0f, 1.0f));
                    weatherNode->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
                    weatherNode->SetPersistance(false);
                    weatherNode->UpdateXForm(false, true);
                    pClient->GetWorld().GetGraph().LinkNode(weatherNode);
                    m_weatherNodes.push_back(weatherNode);
                }
            }
        }

        int const soundModelId = M3D_ENGINE_CFG.GetModelIdByName(m_soundNodeName);
        if (soundModelId != 0x200000)
        {
            SgNode* weatherNode = pClient->CreateServerControlledNode(soundModelId);
            if (weatherNode)
            {
                pClient->GetWorld().GetGraph().GetRootNode()->AddChild(weatherNode);
                pClient->GetWorld().GetGraph().LinkNode(weatherNode);
                m_weatherNodes.push_back(weatherNode);
            }
        }
    }

    int WeatherInclement::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        // RVA 0x8FB020
        Weather::WriteToXmlNode(file, node);
        node->SetAttribute("reduceDistFactor", CStr(m_reduceDistFactor).c_str());
        node->SetAttribute("inclementWeatherPS", m_inclementNodeName.c_str());
        node->SetAttribute("soundName", m_soundNodeName.c_str());
        node->SetAttribute("density", CStr(m_weatherDensity).c_str());
        node->SetAttribute("distance", CStr(m_weatherDist).c_str());
        return 1;
    }

    Class* WeatherInclement::GetBaseClass()
    {
        // RVA 0x7B1260 - NOTE: the shipped class registers m3d::Object, not Weather, as its base, so
        // IsKindOf(Weather) is false for it.
        return RT_CLASS_LOCAL(Object);
    }

    WeatherInclement::~WeatherInclement()
    {
        // RVA 0x8FB6E0
        WeatherInclement::TurnOffEffects();
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
        // RVA 0x7B1270
        return RT_CLASS_LOCAL(WeatherInclement);
    }

    Object* WeatherInclement::CreateObject()
    {
        return new WeatherInclement;
    }

    Object* WeatherInclement::Clone()
    {
        // RVA 0x7B3750 - the copy constructor copies nothing, so this is a blank weather.
        return new WeatherInclement(*this);
    }

    int WeatherInclement::Render()
    {
        // RVA 0x8FB110 - keeps every particle node at m_weatherDist along the ray through the
        // top-centre of the screen, so the rain always falls in front of the camera.
        // NOTE: the current time is read (which notches the timer) and then thrown away.
        M3D_KERNEL->GetTimer().GetCurTime();

        for (unsigned i = 0; i < m_weatherNodes.size(); ++i)
        {
            SgNode* weatherNode = m_weatherNodes[i];
            if (!weatherNode)
            {
                SYS_ERROR("weatherNode");
            }

            CVector2 const point(static_cast<float>(M3D_ENGINE_CFG.m_r_width.GetI()) * 0.5f, 0.0f);
            CVector const finger = M3D_RENDERER->Unproject(point);

            float dist = m_weatherDist;
            if (dist < 0.0f)
            {
                dist = 0.0f;
            }
            else if (dist > 100.0f)
            {
                dist = 100.0f;
            }

            CVector const offset(finger.x * dist, finger.y * dist, finger.z * dist);
            CVector const org = M3D_RENDERER->MatGetOrgInv();
            weatherNode->SetOriginAbs(CVector(org.x + offset.x, org.y + offset.y, org.z + offset.z));
            pClient->GetWorld().GetGraph().RelinkNode(weatherNode, false);
        }
        return 1;
    }

    int WeatherInclement::TurnOffEffects()
    {
        // RVA 0x8FB470
        for (unsigned i = 0; i < m_weatherNodes.size(); ++i)
        {
            SgNode* weatherNode = m_weatherNodes[i];
            if (!weatherNode)
            {
                SYS_ERROR("weatherNode");
            }
            pClient->GetWorld().GetGraph().UnlinkNode(weatherNode);
            pClient->GetWorld().GetGraph().RemoveNode(weatherNode);
        }
        // The storage is freed, not just cleared.
        std::vector<SgNode*>().swap(m_weatherNodes);
        return 1;
    }

    WeatherInclement::WeatherInclement(WeatherInclement const&) : Weather()
    {
        // RVA 0x7B3680 - NOTE: copies nothing, like Weather's copy constructor.
    }

    WeatherInclement::WeatherInclement()
    {
    }
}

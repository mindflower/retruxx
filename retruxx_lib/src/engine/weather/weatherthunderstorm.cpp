#include "weather.h"
#include <stdexcept>

#include "config.h"
#include "core/ini.h"
#include "core/kernel.h"
#include "core/timer.h"

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
        this->m_lastthunderTime = M3D_KERNEL->GetTimer().GetCurTime();

        this->m_thunderWorktime = rand() % m_maxThunderDuty;
        if (this->m_thunderWorktime < m_minThunderDuty)
            this->m_thunderWorktime = m_minThunderDuty;
        if (this->m_thunderWorktime > m_maxThunderDuty)
            this->m_thunderWorktime = m_maxThunderDuty;

        this->m_thunderTimeOut = rand() % m_maxThunderPeriod;
        if (this->m_thunderTimeOut < m_minThunderPeriod)
            this->m_thunderTimeOut = m_minThunderPeriod;
        if (this->m_thunderTimeOut > m_maxThunderPeriod)
            this->m_thunderTimeOut = m_maxThunderPeriod;
        this->m_thunderActive = 0;
        this->m_effectCount = 1;

        m_effectNames.push_back("ET_PS_LIGHT_WEATHER1");
        auto id = M3D_ENGINE_CFG.GetModelIdByName(this->m_effectNames.front());
        if (id != 0x200000)
            m_effectId.push_back(id);
    }

    int WeatherThunderstorm::Update(float amount, int curServerTime)
    {
        Weather::Update(amount, curServerTime);
        return 1;
    }

    Class* WeatherThunderstorm::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherThunderstorm::~WeatherThunderstorm()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WeatherThunderstorm::Render()
    {
        // TODO: implement WeatherThunderstorm::Render
        // RETRUXX_NOT_IMPLEMENTED;
        return 1;
    }

    Class* WeatherThunderstorm::GetBaseClass()
    {
        return RT_CLASS_LOCAL(WeatherInclement);
    }

    Object* WeatherThunderstorm::CreateObject()
    {
        return new WeatherThunderstorm;
    }

    int WeatherThunderstorm::UpdateColors(ColorItems colorItem, ColorTypes curTime)
    {
        Weather::UpdateColors(colorItem, curTime);
        if (this->m_thunderActive)
        {
            auto& vec = m_currentColors[colorItem];
            vec.x = ((vec.x - 255.0) * m_thunderLerpValue) + 255.0;
            vec.y = ((vec.y - 255.0) * m_thunderLerpValue) + 255.0;
            vec.z = ((vec.z - 255.0) * m_thunderLerpValue) + 255.0;
        }
        return 1;
    }

    int WeatherThunderstorm::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WeatherThunderstorm::TurnOffEffects()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WeatherThunderstorm::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        WeatherInclement::ReadFromXmlNode(file, node);
        m3d::SafeIntAttrib(m_minThunderDuty, node, "minThunderDuty");
        m3d::SafeIntAttrib(m_maxThunderDuty, node, "maxThunderDuty");
        m3d::SafeIntAttrib(m_minThunderPeriod, node, "minThunderPeriod");
        m3d::SafeIntAttrib(m_maxThunderPeriod, node, "maxThunderPeriod");
        m3d::SafeIntAttrib(m_effectCount, node, "lightEffectCount");


        this->m_thunderWorktime = rand() % m_maxThunderDuty;
        if (this->m_thunderWorktime < m_minThunderDuty)
            this->m_thunderWorktime = m_minThunderDuty;
        if (this->m_thunderWorktime > m_maxThunderDuty)
            this->m_thunderWorktime = m_maxThunderDuty;

        this->m_thunderTimeOut = rand() % m_maxThunderPeriod;
        if (this->m_thunderTimeOut < m_minThunderPeriod)
            this->m_thunderTimeOut = m_minThunderPeriod;
        if (this->m_thunderTimeOut > m_maxThunderPeriod)
            this->m_thunderTimeOut = m_maxThunderPeriod;

        m_effectNames.clear();
        for (int i = 0; i < m_effectCount; ++i)
        {
            CStr effectName = "lightEffectName" + CStr(i + 1);
            CStr attr;
            m3d::SafeStrAttrib(attr, node, effectName.c_str());
            m_effectNames.push_back(attr);
        }

        // TODO: check clearing m_effectId
        for (const auto& name : m_effectNames)
        {
            auto id = M3D_ENGINE_CFG.GetModelIdByName(name);
            if (id != 0x200000)
                m_effectId.push_back(id);
        }

        return 1;
    }

    void WeatherThunderstorm::SetUp()
    {
        WeatherInclement::SetUp();
        this->m_thunderTimeOut = 0;
    }

    Object* WeatherThunderstorm::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WeatherThunderstorm::WeatherThunderstorm()
    {
    }

    WeatherThunderstorm::WeatherThunderstorm(WeatherThunderstorm const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

#include "weather.h"
#include <stdexcept>

#include "client.h"
#include "config.h"
#include "m3dapp.h"
#include "world.h"
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
        // RVA 0x7B1290
        return RT_CLASS_LOCAL(WeatherThunderstorm);
    }

    // RVA 0x8FC160 - the effects are turned off by ~WeatherInclement.
    WeatherThunderstorm::~WeatherThunderstorm() = default;

    int WeatherThunderstorm::Render()
    {
        // RVA 0x8FBCB0 - after a random timeout, starts a lightning flash: the colours jump to white
        // (see UpdateColors) and fade back over m_thunderWorktime ms, and up to three lightning
        // effects are dropped 300 units above the ground near the far edge of the view.
        WeatherInclement::Render();

        int const curtime = static_cast<int>(M3D_KERNEL->GetTimer().GetCurTime());
        // NOTE: the timer is fetched a second time and not used.
        M3D_KERNEL->GetTimer();

        if (curtime - m_lastthunderTime > m_thunderTimeOut)
        {
            m_thunderWorktime = rand() % m_maxThunderDuty;
            if (m_thunderWorktime < m_minThunderDuty)
                m_thunderWorktime = m_minThunderDuty;
            if (m_thunderWorktime > m_maxThunderDuty)
                m_thunderWorktime = m_maxThunderDuty;

            m_thunderTimeOut = rand() % m_maxThunderPeriod;
            if (m_thunderTimeOut < m_minThunderPeriod)
                m_thunderTimeOut = m_minThunderPeriod;
            if (m_thunderTimeOut > m_maxThunderPeriod)
                m_thunderTimeOut = m_maxThunderPeriod;

            m_thunderActive = true;
            m_curthunderTime = curtime;
            m_lastthunderTime = curtime;
            m_thunderLerpValue = 1.0f;

            if (!m_effectId.empty())
            {
                int const numEffects = rand() % 4;
                for (int n = 0; n < numEffects; ++n)
                {
                    unsigned idx = 0;
                    if (m_effectId.size() > 1)
                    {
                        idx = rand() % m_effectId.size();
                    }

                    SgNode* effectNode = pClient->CreateServerControlledNode(m_effectId[idx]);
                    if (!effectNode)
                    {
                        continue;
                    }

                    // A random column of the screen at half height, unprojected into a view ray.
                    int const width = M3D_ENGINE_CFG.m_r_width.GetI();
                    CVector2 const point(static_cast<float>(rand() % width),
                                         static_cast<float>(M3D_ENGINE_CFG.m_r_height.GetI()) * 0.5f);
                    CVector const finger = M3D_RENDERER->Unproject(point);

                    // 95% of the landscape view distance, in visibility cells of 128 units.
                    int cells = static_cast<int>(M3D_ENGINE_CFG.m_lsViewDistanceDivider.GetF() * 8.0f + 4.0f);
                    if (cells < 4)
                        cells = 4;
                    else if (cells > 12)
                        cells = 12;
                    float const VISCELL_EDGE_LENGTH = 128.0f;
                    float const dist = static_cast<float>(cells) * VISCELL_EDGE_LENGTH * 0.94999999f;

                    float const dx = finger.x * dist;
                    float const dz = finger.z * dist;
                    CVector const org = M3D_RENDERER->MatGetOrgInv();
                    CVector effectPos;
                    effectPos.x = org.x + dx;
                    effectPos.z = org.z + dz;
                    effectPos.y =
                        pClient->GetWorld().GetLandscape().GetHeightWithCollisions(effectPos.x, effectPos.z, false) +
                        300.0f;

                    effectNode->SetOriginAbs(effectPos);
                    effectNode->SetPersistance(false);
                    effectNode->UpdateXForm(false, true);
                    pClient->GetWorld().GetGraph().GetRootNode()->AddChild(effectNode);
                    pClient->GetWorld().GetGraph().LinkNode(effectNode);
                    pClient->GetWorld().GetGraph().InsertInRemoveIfFree(effectNode);
                }
            }
        }

        int const elapsed = curtime - m_curthunderTime;
        if (elapsed > m_thunderWorktime)
        {
            m_thunderActive = false;
        }
        if (m_thunderActive)
        {
            m_thunderLerpValue = static_cast<float>(elapsed) / static_cast<float>(m_thunderWorktime);
        }
        return 1;
    }

    Class* WeatherThunderstorm::GetBaseClass()
    {
        // RVA 0x7B1280 - NOTE: the shipped class registers m3d::Object, not WeatherInclement, as its
        // base, so IsKindOf(WeatherInclement) and IsKindOf(Weather) are false for it.
        return RT_CLASS_LOCAL(Object);
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

    int WeatherThunderstorm::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        // RVA 0x8FBA10
        WeatherInclement::WriteToXmlNode(file, node);
        node->SetAttribute("minThunderDuty", CStr(m_minThunderDuty).c_str());
        node->SetAttribute("maxThunderDuty", CStr(m_maxThunderDuty).c_str());
        node->SetAttribute("minThunderPeriod", CStr(m_minThunderPeriod).c_str());
        node->SetAttribute("maxThunderPeriod", CStr(m_maxThunderPeriod).c_str());
        node->SetAttribute("lightEffectCount", CStr(m_effectCount).c_str());

        unsigned i = 0;
        while (i < m_effectNames.size())
        {
            char const* effectName = m_effectNames[i].c_str();
            ++i;
            CStr const attrName = CStr("lightEffectName") + CStr(i);
            node->SetAttribute(attrName.c_str(), effectName);
        }
        if (i == 0)
        {
            // NOTE: with no effects the shipped code writes an empty, unnumbered "lightEffectName",
            // which ReadFromXmlNode never reads (it looks for lightEffectName1..N).
            node->SetAttribute(CStr("lightEffectName").c_str(), "");
        }
        return 1;
    }

    int WeatherThunderstorm::TurnOffEffects()
    {
        WeatherInclement::TurnOffEffects();
        return 1;
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

        // NOTE: m_effectNames is rebuilt but m_effectId is not cleared, so the ids are appended to
        // whatever DefaultInitialize already put there (ET_PS_LIGHT_WEATHER1), as in the binary.
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
        // RVA 0x7B3850 - a fresh default weather; nothing is copied.
        return new WeatherThunderstorm();
    }

    WeatherThunderstorm::WeatherThunderstorm()
    {
    }

    WeatherThunderstorm::WeatherThunderstorm(WeatherThunderstorm const&) : WeatherInclement()
    {
        // RVA 0x7B37A0 - NOTE: copies nothing, like Weather's copy constructor.
    }
}
